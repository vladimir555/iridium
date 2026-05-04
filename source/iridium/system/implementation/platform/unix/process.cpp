#include "process.h"


#ifdef UNIX_PLATFORM


#include "iridium/items.h"
#include "iridium/logging/logger.h"

#include <unistd.h>
#include <spawn.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/socket.h>

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


namespace iridium::system::implementation::platform::unix_ {


CProcessStream::CProcessStream(
    std::string const &app,
    std::string const &args)
:
    CStreamPort         (URI("process://" + app + " " + args)),
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
    m_command_line      (app)
{
    for (auto const &arg: args)
        m_command_line  += " " + arg;
}


void CProcessStream::initialize() {
    int cin_pipe[2]  = { 0 };
    int cout_pipe[2] = { 0 };

    posix_spawn_file_actions_t actions = {};

    try {
        if (m_fd_reader || m_fd_writer)
            throw std::runtime_error("not finalized");

        assertOK(
            socketpair(AF_UNIX, SOCK_STREAM, 0, cin_pipe),
           "socketpair(stdin)");

        assertOK(
            socketpair(AF_UNIX, SOCK_STREAM, 0, cout_pipe),
           "socketpair(stdout, stderr)");

        // Устанавливаем FD_CLOEXEC для всех дескрипторов
        fcntl(cin_pipe[0],  F_SETFD, FD_CLOEXEC);
        fcntl(cin_pipe[1],  F_SETFD, FD_CLOEXEC);
        fcntl(cout_pipe[0], F_SETFD, FD_CLOEXEC);
        fcntl(cout_pipe[1], F_SETFD, FD_CLOEXEC);

#ifdef SO_NOSIGPIPE
        static int const option_value = 1;
        setsockopt(cin_pipe[0],  SOL_SOCKET, SO_NOSIGPIPE, &option_value, sizeof(option_value));
        setsockopt(cin_pipe[1],  SOL_SOCKET, SO_NOSIGPIPE, &option_value, sizeof(option_value));
        setsockopt(cout_pipe[0], SOL_SOCKET, SO_NOSIGPIPE, &option_value, sizeof(option_value));
        setsockopt(cout_pipe[1], SOL_SOCKET, SO_NOSIGPIPE, &option_value, sizeof(option_value));
#endif // SO_NOSIGPIPE

        assertOK(
            posix_spawn_file_actions_init(&actions),
           "posix_spawn_file_actions_init");

        // stdin: child reads from read-end, parent writes to write-end
        assertOK(
            posix_spawn_file_actions_adddup2(&actions, cin_pipe[0], 0),
           "posix_spawn_file_actions_adddup2 (stdin)");
        assertOK(
            posix_spawn_file_actions_addclose(&actions, cin_pipe[1]),
           "posix_spawn_file_actions_addclose (stdin write-end)");

        // stdout -> general output socket
        assertOK(
            posix_spawn_file_actions_adddup2(&actions, cout_pipe[1], 1),
           "posix_spawn_file_actions_adddup2 (stdout)");
        // stderr -> same output socket (stream merging)
        assertOK(
            posix_spawn_file_actions_adddup2(&actions, cout_pipe[1], 2),
           "posix_spawn_file_actions_adddup2 (stderr)");

        // close both ends of the output socket in the child process after duplication
        assertOK(
            posix_spawn_file_actions_addclose(&actions, cout_pipe[0]),
           "posix_spawn_file_actions_addclose (output read-end)");
        assertOK(
            posix_spawn_file_actions_addclose(&actions, cout_pipe[1]),
           "posix_spawn_file_actions_addclose (output write-end)");

        std::vector<char *> argv(1 + m_args.size() + 1);
        argv[0] = (char *)m_app.data();
        for (size_t i = 0; i < m_args.size(); i++)
            argv[i + 1] = (char *)m_args[i].data();

        argv[1 + m_args.size()] = nullptr;

        pid_t pid = m_pid;
        assertOK(
            posix_spawnp(&pid, m_app.c_str(), &actions, nullptr, argv.data(), environ),
           "posix_spawnp");

        assertOK(
            posix_spawn_file_actions_destroy(&actions),
           "posix_spawn_file_actions_destroy");

        m_pid = pid;

        // read-end stdin
        close(cin_pipe[0]);
        // write-end stdout
        close(cout_pipe[1]);

        m_fd_reader = cout_pipe[0];
        m_fd_writer = cin_pipe[1];

        setBlockingMode(false);
        m_exit_code.reset();

        auto state = getState();

        if (!checkOneOf(state.condition, TState::TCondition::RUNNING, TState::TCondition::DONE))
            throw std::runtime_error("process is not running, condition: " + convert<string>(state.condition)); // ----->

    } catch (std::exception const &e) {
        // cleanup sockets on error to avoid fd leak
        if (cin_pipe[0] > 0)
            ::close(cin_pipe[0]);
        if (cin_pipe[1] > 0)
            ::close(cin_pipe[1]);
        if (cout_pipe[0] > 0)
            ::close(cout_pipe[0]);
        if (cout_pipe[1] > 0)
            ::close(cout_pipe[1]);

        posix_spawn_file_actions_destroy(&actions);

        throw std::runtime_error("initialization process '" + m_command_line + "' error: " + e.what()); // ----->
    }
    // LOGT << "initialize OK : " << getHandles();
}


void CProcessStream::finalize() {
    // LOGT << "finalize: " << getHandles();
    try {
        if (m_pid == 0)
            throw std::runtime_error("not initialized"); // ----->

//            LOGT << "stop process: " << m_command_line << " pid: " << m_pid << " fd: " << m_fd_reader;
//            LOGT << "WAIT: " << m_command_line << " pid: " << m_pid << " fd: " << m_fd_reader << " ...";

        auto buffer  = read();
        auto start   = system_clock::now();
        auto timeout = start + DEFAULT_PROCESS_TIMEOUT;
        while (system_clock::now() < timeout && getState().condition == TState::TCondition::RUNNING) {
        //    LOGT << "\n" << m_command_line
        //         << "\n" << system_clock::now()
        //         << " < " << timeout << " "  << getState().condition
        //         << " "   << timeout - system_clock::now()
        //         << "\n"  << read();
            buffer->emplace_back(read());
            std::this_thread::sleep_for(DEFAULT_PROCESS_TIMEOUT_STEP);
        }

//            LOGT << "WAIT: " << m_command_line << " pid: " << m_pid << " fd: " << m_fd_reader << " DONE";

        if (getState().condition == TState::TCondition::RUNNING) {
            LOGW
                << "finalization: kill pid " << m_pid << " " << m_command_line
                << "\noutput:\n" << buffer;

//                << ", timeout: " << system_clock::now() - start
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

        m_pid = 0;

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

    // {
    //     string process_state_str;

    //     if (m_state_internal.is_exited)
    //         process_state_str += "\nexit code: " +
    //         convert<string>(m_state_internal.exit_status);

    //     if (m_state_internal.is_signaled)
    //         process_state_str += "\nterminate signal code: " +
    //         convert<string>(m_state_internal.terminate_signal);

    //     if (m_state_internal.is_stopped)
    //         process_state_str += "\nstop signal code: " +
    //         convert<string>(m_state_internal.stop_signal);

    //     if (m_state_internal.is_coredumped)
    //         process_state_str += "\ncoredumped";

    //     if (m_state_internal.is_continued)
    //         process_state_str += "\ncontinued";

    //     if (!process_state_str.empty())
    //     LOGT << "\n" << m_uri << "\npid: " << m_pid << process_state_str;
    // }

    if ( m_state_internal.is_exited && !m_state_internal.is_signaled) {
        m_exit_code = std::make_shared<int>(m_state_internal.exit_status);
        condition = TState::TCondition::DONE;
    }

    if (!m_state_internal.is_exited &&  m_state_internal.is_signaled) {
        condition =
            checkOneOf<int>(m_state_internal.terminate_signal, SIGINT, SIGTERM, SIGHUP) ?
            TState::TCondition::INTERRUPTED :
            TState::TCondition::CRASHED;
    }

    // LOGT << "process '" << m_app << " " << m_args.back() << "' condition: " << condition;

    return {
        .condition = condition,
        .exit_code = m_exit_code
    };
}


void CProcessStream::sendSignal(TSignal const &signal) {
    // LOGT << "send signal: " << signal;

    pid_t pid = m_pid.load();
    if (pid <= 0) {
        throw std::runtime_error("sendSignal error: invalid or not running process");
    }

    int sig;

    switch (signal) {
        case TSignal::INTERRUPT:
            sig = SIGINT;
            break;
        case TSignal::TERMINATE:
            sig = SIGTERM;
            break;
        case TSignal::KILL:
            sig = SIGKILL;
            break;
        default:
            throw std::runtime_error("sendSignal error: unknown signal type '" +
                convert<std::string>(signal) + "'");
    }

    assertOK(kill(m_pid, sig), "kill");
}


} // iridium::system::implementation::platform::unix_


#endif  // UNIX_PLATFORM
