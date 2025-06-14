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
    m_command_line      (app + " " + args)
{}


CProcessStream::CProcessStream(
    std::string const &app,
    std::vector<std::string> const &args)
:
    CStreamPort         (URI("process://" + app)),
    m_state_internal    {},
    m_app               (app),
    m_args              (args),
    m_command_line      (app),
    m_pid               (0)
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
        if (m_fd_reader || m_fd_writer)
            throw std::runtime_error("not finalized"); // ----->

        int cin_pipe[2]  = { 0 };
        int cout_pipe[2] = { 0 };
        int cerr_pipe[2] = { 0 };

        assertOK(pipe(cin_pipe),  "pipe");
        assertOK(pipe(cout_pipe), "pipe");
        assertOK(pipe(cerr_pipe), "pipe");
        
        posix_spawn_file_actions_t actions;
        assertOK(
            posix_spawn_file_actions_init(&actions),
           "posix_spawn_file_actions_init"
        );
        
        assertOK(
            posix_spawn_file_actions_adddup2(&actions,  cin_pipe[0], 0),
           "posix_spawn_file_actions_adddup2, stdin");

        assertOK(
            posix_spawn_file_actions_addclose(&actions, cout_pipe[0]),
           "posix_spawn_file_actions_addclose, cout_pipe"
        );
        
        assertOK(
            posix_spawn_file_actions_addclose(&actions, cerr_pipe[0]),
           "posix_spawn_file_actions_addclose, cerr_pipe"
        );
        
        assertOK(
            posix_spawn_file_actions_adddup2(&actions, cout_pipe[1], 1),
           "posix_spawn_file_actions_adddup2, cout_pipe"
        );
        
        assertOK(
            posix_spawn_file_actions_adddup2(&actions, cerr_pipe[1], 2),
           "posix_spawn_file_actions_adddup2, cerr_pipe"
        );
    
        assertOK(
            posix_spawn_file_actions_adddup2(&actions, 1, 2),
           "posix_spawn_file_actions_adddup2"
        );

        assertOK(
            posix_spawn_file_actions_addclose(&actions, cin_pipe[0]),
           "posix_spawn_file_actions_addclose(stdin_read_end)");

        assertOK(
            posix_spawn_file_actions_addclose(&actions, cout_pipe[1]),
           "posix_spawn_file_actions_addclose"
        );
        
        assertOK(
            posix_spawn_file_actions_addclose(&actions, cerr_pipe[1]),
           "posix_spawn_file_actions_addclose"
        );
        
        std::vector<char *> argv(1 + m_args.size() + 1);
        argv[0] = (char *)m_app.data();
        for (size_t i = 0; i < m_args.size(); i++)
            argv[i + 1] = (char *)m_args[i].data();

        argv[1 + m_args.size()] = nullptr;

        //    LOGT << "start process: " << m_command_line << " pid: " << m_pid << " fd: " << m_fd;
        
#ifdef POSIX_SPAWN_SETSID
        posix_spawnattr_t attr = { 0 };
        posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSID);
#elif defined(POSIX_SPAWN_SETSID_NP)
        posix_spawnattr_t attr = { 0 };
        posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSID_NP);
#else
        posix_spawnattr_t attr = { 0 };
//            posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSIGMASK);
//            throw std::runtime_error("posix_spawnattr_setflags error: POSIX_SPAWN_SETSID is not defined");
#endif
        
        pid_t pid = m_pid;
        
        assertOK(
//            posix_spawnp(&pid, m_app.c_str(), &actions, &attr, argv, environ),
            posix_spawnp(&pid, m_app.c_str(), &actions, 0, argv.data(), environ),
           "posix_spawnp"
        );
        
        m_pid = pid;
        
        //    auto r =
        //    posix_spawnp(&m_pid, m_app.c_str(), &action, &attr, argv, environ);
        //    LOGT << "posix_spawnp: " << r << " " << m_pid;
        //    assertOK(r, "posix_spawnp");

        close(cin_pipe[1]);
        close(cout_pipe[1]);
        close(cerr_pipe[1]);

        m_fd_writer = cin_pipe[1];
        m_fd_reader = cout_pipe[0];
        
//        LOGT << "initialize process '" << m_command_line << "', fd: " << m_fd_reader;
        
        setBlockingMode(false);
        
        m_exit_code.reset();
        
        auto state = getState();
        if  (!checkOneOf(state.condition, TState::TCondition::RUNNING, TState::TCondition::DONE))
            throw std::runtime_error("process is not running, condition: " + convert<string>(state.condition)); // ----->
    } catch (std::exception const &e) {
        throw std::runtime_error("initialization process '" + m_command_line + "' error: " + e.what()); // ----->
    }

//    LOGT << "initialize process '" << m_command_line << "', fd: " << static_cast<int>(m_fd_reader);
}


void CProcessStream::finalize() {
//    LOGT << "finalize   process '" << m_command_line << "', fd: " << static_cast<int>(m_fd_reader);
    try {
        if (m_pid == 0)
            throw std::runtime_error("not initialized"); // ----->

//            LOGT << "stop process: " << m_command_line << " pid: " << m_pid << " fd: " << m_fd_reader;
//            LOGT << "WAIT: " << m_command_line << " pid: " << m_pid << " fd: " << m_fd_reader << " ...";

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
        
//            LOGT << "WAIT: " << m_command_line << " pid: " << m_pid << " fd: " << m_fd_reader << " DONE";

        if (getState().condition == TState::TCondition::RUNNING) {
            LOGW << "finalization: kill pid " << m_pid << " " << m_command_line;
//                 << ", timeout: " << system_clock::now() - start;
            assertOK(kill(m_pid, SIGKILL), "kill");
            //        m_state_internal.is_signaled = true;
            //todo: timeout condition
        }
        
        if (m_fd_reader) {
            close(m_fd_reader);
            m_fd_reader = 0;
        }

        if (m_fd_writer) {
            close(m_fd_writer);
            m_fd_writer = 0;
        }

        //    m_state_internal = { 0 };
//            LOGT << "stop process: " << m_command_line << " pid: " << m_pid << " fd: " << m_fd_reader << " done";
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
//        LOGT << "process '" << m_command_line << "' state: " << process_state_str << " fd: " << m_fd_reader;

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
