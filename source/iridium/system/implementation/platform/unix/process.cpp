#include "process.h"


#ifdef UNIX_PLATFORM


#include "iridium/items.h"
#include "iridium/logging/logger.h"

#include <unistd.h>
#include <spawn.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <chrono>
#include <array>


using iridium::io::implementation::CStreamPort;
using iridium::io::URI;
using iridium::convertion::convert;
using std::string;
using std::chrono::milliseconds;
using std::chrono::system_clock;


milliseconds DEFAULT_PROCESS_TIMEOUT        (5000);
milliseconds DEFAULT_PROCESS_TIMEOUT_STEP   (100);


extern char **environ;


namespace iridium {
namespace system {
namespace implementation {
namespace platform {
namespace unix_ {


CProcessStream::CProcessStream(
    std::string const &app,
    std::string const &args)
:
    CStreamPort(URI("process://" + app + " " + args)),
    m_state_internal    ({}),
    m_app               (app),
    m_args              (assign(split(args, " "))),
    m_command_line      (app + " " + args),
    m_pipe_out          (0)
{}


CProcessStream::CProcessStream(
    std::string const &app,
    std::vector<std::string> const &args)
:
    CStreamPort         (URI("file://" + app)),
    m_state_internal    {},
    m_app               (app),
    m_args              (args),
    m_command_line      (app),
    m_pid               (0),
    m_pipe_out          (0)
{
    for (auto const &arg: args)
        m_command_line  += " " + arg;
}


void CProcessStream::initialize() {
//    LOGT << "CProcessStream::initialize";
    try {
//        if (m_fd) {
//            LOGW << "initializing process stream '" + m_command_line + "' error: already initialized";
//            return;
//        }
        if (m_fd)
            throw std::runtime_error("not finalized"); // ----->
        
        int cout_pipe[2] = { 0 };
        int cerr_pipe[2] = { 0 };
        
        assertOK(pipe(cout_pipe), "pipe");
        assertOK(pipe(cerr_pipe), "pipe");
        
        posix_spawn_file_actions_t actions;
        assertOK(
            posix_spawn_file_actions_init(&actions),
           "posix_spawn_file_actions_init"
        );
        
        assertOK(
            posix_spawn_file_actions_addclose(&actions, cout_pipe[0]),
           "posix_spawn_file_actions_addclose"
        );
        
        assertOK(
            posix_spawn_file_actions_addclose(&actions, cerr_pipe[0]),
           "posix_spawn_file_actions_addclose"
        );
        
        assertOK(
            posix_spawn_file_actions_adddup2(&actions, cout_pipe[1], 1),
           "posix_spawn_file_actions_adddup2"
        );
        
        assertOK(
            posix_spawn_file_actions_adddup2(&actions, cerr_pipe[1], 2),
           "posix_spawn_file_actions_adddup2"
        );
    
        assertOK(
            posix_spawn_file_actions_adddup2(&actions, 1, 2),
           "posix_spawn_file_actions_adddup2"
        );
        
        assertOK(
            posix_spawn_file_actions_addclose(&actions, cout_pipe[1]),
           "posix_spawn_file_actions_addclose"
        );
        
        assertOK(
            posix_spawn_file_actions_addclose(&actions, cerr_pipe[1]),
           "posix_spawn_file_actions_addclose"
        );
        
        char *argv[1 + m_args.size() + 1];
        argv[0] = (char *)m_app.data();
        for (size_t i = 0; i < m_args.size(); i++)
            argv[i + 1] = (char *)m_args[i].data();
        
        argv[1 + m_args.size()] = nullptr;
        
        //    LOGT << "start process: " << m_command_line << " pid: " << m_pid << " fd: " << m_fd;
        
        //#ifdef POSIX_SPAWN_SETSID
        //    posix_spawnattr_t attr = { 0 };
        //    posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSID);
        //#elif defined(POSIX_SPAWN_SETSID_NP)
        //    posix_spawnattr_t attr = { 0 };
        //    posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSID_NP);
        //#else
        ////    posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSIGMASK);
        ////    throw std::runtime_error("posix_spawnattr_setflags error: POSIX_SPAWN_SETSID is not defined");
        //#endif
        
        pid_t pid = m_pid;
        
        assertOK(
            //        posix_spawnp(&m_pid, m_app.c_str(), &actions, &attr, argv, environ),
            posix_spawnp(&pid, m_app.c_str(), &actions, 0, argv, environ),
           "posix_spawnp"
        );
        
        m_pid = pid;
        
        //    auto r =
        //    posix_spawnp(&m_pid, m_app.c_str(), &action, &attr, argv, environ);
        //    LOGT << "posix_spawnp: " << r << " " << m_pid;
        //    assertOK(r, "posix_spawnp");
        
        close(cout_pipe[1]);
        close(cerr_pipe[1]);
        
        m_fd = m_pipe_out = cout_pipe[0];
        
        LOGT << "initialize process '" << m_command_line << "', fd: " << m_fd;
        
        setBlockingMode(m_fd, false);
        
        m_exit_code.reset();
        
        auto state = getState();
        if  (!checkOneOf(state.condition, TState::TCondition::RUNNING, TState::TCondition::DONE))
            throw std::runtime_error("process is not running, condition: " + convert<string>(state.condition)); // ----->
    } catch (std::exception const &e) {
        throw std::runtime_error("initialization process '" + m_command_line + "' error: " + e.what()); // ----->
    }
}


void CProcessStream::finalize() {
    try {
        if (m_pid == 0)
            throw std::runtime_error("not initialized"); // ----->
        
        LOGT << "finalize   process '" << m_command_line << "', id: " << m_fd;
        
        //    LOGT << "stop process: " << m_command_line << " pid: " << m_pid << " fd: " << m_fd;
        //    LOGT << "WAIT: " << m_command_line << " pid: " << m_pid << " fd: " << m_fd << " ...";

        {
            io::Buffer::TSharedPtr b;
            do {
                b = read();
            } while (b && !b->empty());
        }
        
//        auto start   = system_clock::now();
//        auto timeout = start + DEFAULT_PROCESS_TIMEOUT;
//        while (system_clock::now() < timeout && getState().condition == TState::TCondition::RUNNING) {
//            LOGT << "\n" << m_command_line << "\n"
//                 << system_clock::now() << " < " << timeout << " " << getState().condition << " " << timeout - system_clock::now();
//            std::this_thread::sleep_for(DEFAULT_PROCESS_TIMEOUT_STEP);
//        }
        
        //    LOGT << "WAIT: " << m_command_line << " pid: " << m_pid << " fd: " << m_fd << " DONE";
        
        if (getState().condition == TState::TCondition::RUNNING) {
            LOGW << "finalization: kill pid " << m_pid << " " << m_command_line;
//                 << ", timeout: " << system_clock::now() - start;
            assertOK(kill(m_pid, SIGKILL), "kill");
            //        m_state_internal.is_signaled = true;
            //todo: timeout condition
        }
        
        if (m_pipe_out) {
            close(m_pipe_out);
            m_pipe_out  = 0;
            m_fd        = 0;
        }
        
        //    m_state_internal = { 0 };
        //    LOGT << "stop process: " << m_command_line << " pid: " << m_pid << " fd: " << m_fd << " done";
    } catch (std::exception const &e) {
        throw std::runtime_error("finalization process '" + m_command_line + "' error: " + e.what()); // ----->
    }
}


IProcess::TState CProcessStream::getState() {
    LOCK_SCOPE();
    
    TState::TCondition condition = TState::TCondition::UNKNOWN;

    if (m_pid != 0) {
        int  pid_state = 0;
        auto result = waitpid(m_pid, &pid_state, WNOHANG);

//        LOGT << "waitpid: " << m_command_line << " pid: " << m_pid << " result(pid): " << result << " state: " << pid_state;
//        {
//            auto result = kill(m_pid, 0);
//            LOGT << "kill: " << m_command_line << " pid: " << m_pid << " result: " << result;
//        }

        if  (result == 0 && pid_state == 0)
            condition = TState::TCondition::RUNNING;

        if  (result > 0) {
            m_state_internal.is_exited        = WIFEXITED     (pid_state);
            m_state_internal.exit_status      = WEXITSTATUS   (pid_state);
            m_state_internal.is_signaled      = WIFSIGNALED   (pid_state);
            m_state_internal.terminate_signal = WTERMSIG      (pid_state);
            m_state_internal.is_coredumped    = WCOREDUMP     (pid_state);
            m_state_internal.is_stopped       = WIFSTOPPED    (pid_state);
            m_state_internal.stop_signal      = WSTOPSIG      (pid_state);
            m_state_internal.is_continued     = WIFCONTINUED  (pid_state);
        }
    }

    string process_state_str;

    if (m_state_internal.is_exited)
        process_state_str += " exit code: " +
        convert<string>(m_state_internal.exit_status);

    if (m_state_internal.is_signaled)
        process_state_str += " terminate signal code: " +
        convert<string>(m_state_internal.terminate_signal);

    if (m_state_internal.is_stopped)
        process_state_str += " stop signal code: " +
        convert<string>(m_state_internal.stop_signal);

    if (m_state_internal.is_coredumped)
        process_state_str += " coredumped";

    if (m_state_internal.is_continued)
        process_state_str += " continued";

//    if (!process_state_str.empty())
//        LOGT << "process '" << m_command_line << "' state: " << process_state_str;

    if ( m_state_internal.is_exited && !m_state_internal.is_signaled) {
        m_exit_code = std::make_shared<int>(m_state_internal.exit_status);
        condition = TState::TCondition::DONE;
    }

    if (!m_state_internal.is_exited &&  m_state_internal.is_signaled)
        condition = TState::TCondition::CRASHED;

//    LOGT << "process '" << m_app << " " << m_args.back() << "' condition: " << condition;

    return {
        .condition = condition,
        .exit_code = m_exit_code
    };
}


} // unix_
} // platform
} // implementation
} // system
} // iridium


#endif  // UNIX_PLATFORM


//    sleep(1);
//    int status = 0;
//    auto waitpid_result = waitpid(m_pid, &status, WNOHANG);
//    LOGT << "waitpid result = " << waitpid_result;
//    LOGT << "waitpid status = " << status;
//    LOGT << "waitpid errno  = " << strerror(errno);
//    LOGT << "waitpid WIFEXITED      = " << WIFEXITED(status);
//    LOGT << "waitpid WEXITSTATUS    = " << WEXITSTATUS(status);
//    LOGT << "waitpid WIFSIGNALED    = " << WIFSIGNALED(status);
//    LOGT << "waitpid WTERMSIG       = " << WTERMSIG(status);
//    LOGT << "waitpid WIFSTOPPED     = " << WIFSTOPPED(status);
//    LOGT << "waitpid WSTOPSIG       = " << WSTOPSIG(status);
//  child1: segfault
//    06:16:48.070 T 139900355278592 CProcessStream::finalize waitpid result = 262420
//    06:16:48.070 T 139900355278592 CProcessStream::finalize waitpid status = 136
//    06:16:48.070 T 139900355278592 CProcessStream::finalize waitpid errno  = Success
//    06:25:13.638 T 140529980577536 CProcessStream::finalize waitpid WIFEXITED      = false
//    06:25:13.638 T 140529980577536 CProcessStream::finalize waitpid WEXITSTATUS    = 0
//    06:25:13.638 T 140529980577536 CProcessStream::finalize waitpid WIFSIGNALED    = true
//    06:25:13.638 T 140529980577536 CProcessStream::finalize waitpid WTERMSIG       = 8
//    06:25:13.638 T 140529980577536 CProcessStream::finalize waitpid WIFSTOPPED     = false
//    06:25:13.638 T 140529980577536 CProcessStream::finalize waitpid WSTOPSIG       = 0
//  child2: done
//    06:20:24.506 T 140719520171776 CProcessStream::finalize waitpid result = 263746
//    06:20:24.506 T 140719520171776 CProcessStream::finalize waitpid status = 0
//    06:20:24.506 T 140719520171776 CProcessStream::finalize waitpid errno  = Success
//    06:24:21.763 T 140410433943296 CProcessStream::finalize waitpid WIFEXITED      = true
//    06:24:21.763 T 140410433943296 CProcessStream::finalize waitpid WEXITSTATUS    = 0
//    06:24:21.763 T 140410433943296 CProcessStream::finalize waitpid WIFSIGNALED    = false
//    06:24:21.764 T 140410433943296 CProcessStream::finalize waitpid WTERMSIG       = 0
//    06:24:21.764 T 140410433943296 CProcessStream::finalize waitpid WIFSTOPPED     = false
//    06:24:21.764 T 140410433943296 CProcessStream::finalize waitpid WSTOPSIG       = 0
//  child3: running
//    06:18:42.494 T 140372416132864 CProcessStream::finalize waitpid result = 0
//    06:18:42.495 T 140372416132864 CProcessStream::finalize waitpid status = 0
//    06:18:42.495 T 140372416132864 CProcessStream::finalize waitpid errno  = Success
//    06:27:00.336 T 139888016455424 CProcessStream::finalize waitpid WIFEXITED      = true
//    06:27:00.336 T 139888016455424 CProcessStream::finalize waitpid WEXITSTATUS    = 0
//    06:27:00.336 T 139888016455424 CProcessStream::finalize waitpid WIFSIGNALED    = false
//    06:27:00.336 T 139888016455424 CProcessStream::finalize waitpid WTERMSIG       = 0
//    06:27:00.336 T 139888016455424 CProcessStream::finalize waitpid WIFSTOPPED     = false
//    06:27:00.336 T 139888016455424 CProcessStream::finalize waitpid WSTOPSIG       = 0
//    auto r = kill(m_pid, 0);
//    LOGT << "r = " << r;
//    //LOGT << "ptrace result = " << ptrace();
//    if (r == 0) {
//        LOGT << "kill " << m_pid;
//        kill(m_pid, SIGKILL);
//    } else {
//        LOGT << "skip kill " << m_pid;
//    }
