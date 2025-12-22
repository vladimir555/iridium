#include "process.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/io/event.h"


using std::chrono::milliseconds;
using std::chrono::system_clock;
using std::string;
using iridium::convertion::convert;


milliseconds DEFAULT_PROCESS_TIMEOUT        (1000);
milliseconds DEFAULT_PROCESS_TIMEOUT_STEP   (100);


using iridium::io::URI;


#include "iridium/logging/logger.h"
namespace iridium::system::implementation::platform {


std::atomic<uint64_t> CProcessStream::m_process_counter{0};


CProcessStream::CProcessStream(
    std::string const &app,
    std::string const &args)
:
    CStreamPort             (URI("process://" + app + " " + args)),
    m_app                   (app),
    m_args                  (args),
    m_command_line          (m_app + " " + m_args),
    m_process               {0},
    m_uri                   (io::URI::create("process://" + m_app + " " + m_args)),
    m_security_attributes   {0}
{}


CProcessStream::CProcessStream(
    std::string const &app,
    std::vector<std::string> const &args)
:
    CStreamPort         (URI("process://" + app)),
    m_app               (app),
    m_process           {0}
{
    m_command_line = m_app;
    for (auto const &arg: args) {
        m_args += arg + " ";
        m_command_line += " " + arg;
    }
    if (!m_args.empty() && m_args.back() == ' ')
        m_args.pop_back();
}


void CProcessStream::initialize() {
    try {
        if (m_reader_fd || m_writer_fd)
            throw std::runtime_error("not finalized");

        STARTUPINFOA startup_info{0};
        startup_info.dwFlags = STARTF_USESTDHANDLES;
        SECURITY_ATTRIBUTES sa{ sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE };

        // Pipe for reading / writing child's STDOUT / STDIN
        HANDLE  stdout_writer_fd, stdin_reader_fd;
        auto    stdout_pipe_name = "\\\\.\\pipe\\local\\" + m_app + "." + convert<string>(m_process_counter.fetch_add(1)) + ".stdout";
        auto    stdin_pipe_name  = "\\\\.\\pipe\\local\\" + m_app + "." + convert<string>(m_process_counter.fetch_add(1)) + ".stdin";

        // ----- stdout
        m_reader_fd = CreateNamedPipeA(
            stdout_pipe_name.c_str(),
            PIPE_ACCESS_INBOUND     |
            FILE_FLAG_OVERLAPPED,
            PIPE_TYPE_BYTE          |
            PIPE_READMODE_BYTE      |
            PIPE_WAIT,
            1,//PIPE_UNLIMITED_INSTANCES,
            1024 * 4,
            1024 * 4,
            0,
            &sa);
        checkResult(m_reader_fd, "CreateNamedPipe stdout reader");

        stdout_writer_fd = CreateFileA(
            stdout_pipe_name.c_str(),
            FILE_GENERIC_WRITE,
            0,
            &sa,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr);
        checkResult(stdout_writer_fd, "CreateFile stdout writer");
        // -----

        // ----- stdin
        m_writer_fd = CreateNamedPipeA(
            stdin_pipe_name.c_str(),
            PIPE_ACCESS_OUTBOUND    |
            FILE_FLAG_OVERLAPPED,
            PIPE_TYPE_BYTE          |
            PIPE_READMODE_BYTE      |
            PIPE_WAIT,
            1,//PIPE_UNLIMITED_INSTANCES,
            1024 * 4,
            1024 * 4,
            0,
            &sa);
        checkResult(m_writer_fd, "CreateNamedPipe stdout reader");

        stdin_reader_fd = CreateFileA(
            stdin_pipe_name.c_str(),
            FILE_GENERIC_READ,
            0,
            &sa,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr);
        checkResult(stdin_reader_fd, "CreateFile stdin reader");
        // -----

        // Connect child-side handles
        // child writes stdout here
        startup_info.hStdOutput = stdout_writer_fd;
        // (optional) also redirect stderr
        startup_info.hStdError  = stdout_writer_fd;
        // child reads stdin from here
        startup_info.hStdInput  = stdin_reader_fd;

        checkResult(
            CreateProcessA(
                nullptr,
                const_cast<LPSTR>(m_command_line.c_str()),
                nullptr,
                nullptr,
                TRUE,
                CREATE_NO_WINDOW,
                nullptr,
                nullptr,
                &startup_info,
                &m_process
            ),
            "CreateProcessA"
        );

        // Close child-side handles in parent (no longer needed)
        checkResult(
            CloseHandle(stdout_writer_fd),
           "CloseHandle stdout");
        checkResult(
            CloseHandle(stdin_reader_fd),
           "CloseHandle stdout");

        // LOGT << "create process cmdline: '" << m_command_line << "'";
    } catch (std::exception const &e) {
        closeFDs();
        throw std::runtime_error(convert<string>(m_uri) + "' initialization error: " + e.what());
    }
}


void CProcessStream::finalize() {
    try {
        if (!m_reader_fd)
            throw std::runtime_error("not initialized");

        auto result = WaitForSingleObject(
            m_process.hProcess,
            static_cast<DWORD>(DEFAULT_PROCESS_TIMEOUT.count()));

        if (checkOneOf<int64_t>(result, WAIT_TIMEOUT, WAIT_FAILED)) {
            LOGW << "kill process " << m_app;
            checkResult(TerminateProcess(m_process.hProcess, 1), "TerminateProcess");
            WaitForSingleObject(m_process.hProcess, INFINITE);
        }

        checkResult(CloseHandle(m_process.hProcess), "CloseHandle");
        checkResult(CloseHandle(m_process.hThread) , "CloseHandle");

        closeFDs();

    } catch (std::exception const &e) {
        closeFDs();
        throw std::runtime_error(convert<string>(m_uri) + "' finalization error: " + e.what());
    }
}


IProcess::TState CProcessStream::getState() {
    try {
        if (!m_reader_fd) {
            if (m_finalized_state)
                return *m_finalized_state;
            return { IProcess::TState::TCondition::UNKNOWN, nullptr };
        }

        IProcess::TState    result      {};
        DWORD               exit_code   {};

        checkResult(GetExitCodeProcess(m_process.hProcess, &exit_code), "GetExitCodeProcess");

        switch (exit_code) {
            case STATUS_ACCESS_VIOLATION:
                result.condition = TState::TCondition::CRASHED;
                break;
            case STILL_ACTIVE:
                result.condition = TState::TCondition::RUNNING;
                break;
            default:
                result.condition = TState::TCondition::DONE;
                result.exit_code = std::make_shared<int>(exit_code);
                break;
        }

        m_finalized_state = TState::create(result);

        return result;
    } catch (std::exception const &e) {
        throw std::runtime_error("process '" + m_command_line + "' get state error: " + e.what());
    }
}


void CProcessStream::sendSignal(TSignal const& signal) {
    try {
        DWORD exitCode = STILL_ACTIVE;
        checkResult(
            GetExitCodeProcess(m_process.hProcess, &exitCode),
            "GetExitCodeProcess"
        );

        if (exitCode != STILL_ACTIVE)
            return;

        switch (signal) {
        case TSignal::INTERRUPT: {
            DWORD pid = checkResult(GetProcessId(m_process.hProcess), "GetProcessId");

            if (AttachConsole(pid)) {
                checkResult(GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0), "GenerateConsoleCtrlEvent");
            }
            else {
                throw std::runtime_error("can not send CTRL+C to non console app");
            }

            checkResult(
                WaitForSingleObject(
                    m_process.hProcess,
                    static_cast<DWORD>(DEFAULT_PROCESS_TIMEOUT.count())),
                "WaitForSingleObject");
            break;
        }

        case TSignal::TERMINATE:
        case TSignal::KILL:
            checkResult(
                TerminateProcess(
                    m_process.hProcess, 0),
                "TerminateProcess");
            checkResult(
                WaitForSingleObject(
                    m_process.hProcess, INFINITE),
                "WaitForSingleObject");
            break;

        default:
            throw std::runtime_error("unknown signal type");
        }
    } catch (std::exception const& e) {
        throw std::runtime_error(
            "sending signal '" + convert<string>(signal) +
            "' to process '" + m_app + "' error: " + e.what());
    }
}


} // iridium::system::implementation::platform


#endif // WINDOWS_PLATFORM
