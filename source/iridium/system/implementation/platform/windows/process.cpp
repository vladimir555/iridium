#include "process.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/logging/logger.h"


using std::chrono::milliseconds;
using std::chrono::system_clock;


milliseconds DEFAULT_PROCESS_TIMEOUT        (1000);
milliseconds DEFAULT_PROCESS_TIMEOUT_STEP   (100);


namespace iridium {
namespace system {
namespace implementation {
namespace platform {


template<typename TResult>
TResult assertOK(TResult const &result, std::string const &method) {
    if (result) {
        return result;
    } else {
        DWORD error_message_code    = ::GetLastError();
        LPSTR buffer                = nullptr;

        size_t size = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER  | 
            FORMAT_MESSAGE_FROM_SYSTEM      | 
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, error_message_code, 
            MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), 
            (LPSTR)&buffer, 0, NULL);

        std::string message(buffer, size);
        LocalFree(buffer);
        throw std::runtime_error(method + " error: 0x" + convertion::convert<std::string, uint64_t>(error_message_code, 16) + " " + message); // ----->
    }
}


std::atomic<uint64_t> CProcessStream::m_process_counter{0};


CProcessStream::CProcessStream(
    std::string const &app,
    std::string const &args)
: 
    m_app                   (app),
    m_args                  (args),
    m_command_line          (m_app + " " + m_args),
    m_file_stdout_writer    (nullptr),
    m_pipe_stdout_reader    (nullptr),
    m_process               { 0 },
    m_overlapped            { 0 },
    m_uri                   (io::URI::create("process://" + m_app + " " + m_args))
{}


CProcessStream::CProcessStream(
    std::string const &app,
    std::vector<std::string> const &args)
: 
    m_app               (app),
    m_file_stdout_writer(nullptr),
    m_pipe_stdout_reader(nullptr),
    m_process           { 0 },
    m_overlapped        { 0 }
{
    for (auto const &arg: args)
        m_args += arg + " ";
}


void CProcessStream::initialize() {
    try {
        if (m_pipe_stdout_reader)
            throw std::runtime_error("not finalized"); // ----->

        //LOGT << "start process '" << m_command_line << "' ...";

        STARTUPINFOA startup_info{};
        startup_info.dwFlags |= STARTF_USESTDHANDLES;

        SECURITY_ATTRIBUTES security_attributes{ 0 };
        security_attributes.bInheritHandle  = true;
        security_attributes.nLength         = sizeof(SECURITY_ATTRIBUTES);

        m_pipe_name = "\\\\.\\pipe\\local\\" + m_app + "." + 
            convertion::convert<std::string>(m_process_counter.fetch_add(1)) + ".stdout";

        m_pipe_stdout_reader = assertOK(
            CreateNamedPipeA(
                m_pipe_name.c_str(),
                PIPE_ACCESS_DUPLEX      |
                FILE_FLAG_OVERLAPPED,
                PIPE_TYPE_BYTE          |
                PIPE_READMODE_BYTE      |
                PIPE_WAIT,
                1,//PIPE_UNLIMITED_INSTANCES,
                1024 * 4,
                1024 * 4,
                0,
                &security_attributes),
           "CreateNamedPipe");

        m_file_stdout_writer = assertOK(
            CreateFileA(m_pipe_name.c_str(), FILE_GENERIC_WRITE, 0, &security_attributes, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr),
           "CreateFile");

        startup_info.hStdOutput = m_file_stdout_writer;
        startup_info.hStdError  = m_file_stdout_writer;

        assertOK(
            CreateProcessA(nullptr, const_cast<LPSTR>(m_command_line.c_str()), nullptr, nullptr, true, 0, nullptr, nullptr, &startup_info, &m_process),
           "CreateProcess"
        );

        m_overlapped = { 0 };
        m_overlapped.hEvent = assertOK(CreateEvent(&security_attributes, true, false, nullptr), "CreateEvent");
        ConnectNamedPipe(m_pipe_stdout_reader, &m_overlapped);

        {
            auto result  = WaitForSingleObject(m_overlapped.hEvent, 1000);
            if  (result != WAIT_OBJECT_0)
                assertOK(result, "WaitForSingleObject");
        }

        ResetEvent(m_overlapped.hEvent);

        //m_overlapped.hEvent = assertOK(CreateEvent(&security_attributes, true, false, nullptr), "CreateEvent");
        //read(0);

        //LOGT << "start process '" << m_app << "' OK";
    } catch (std::exception const &e) {
        throw std::runtime_error("process '" + m_command_line + "' initialization error: " + e.what()); // ----->
    }
}


void CProcessStream::finalize() {
    try {
        if (!m_pipe_stdout_reader)
            throw std::runtime_error("not initialized"); // ----->

        //LOGT << "stop process '" << m_command_line << "' ...";

        auto result = WaitForSingleObject(m_process.hProcess, static_cast<DWORD>(DEFAULT_PROCESS_TIMEOUT.count()));
        if  (result) {
            LOGW << "kill process " << m_app;
            assertOK(TerminateProcess(m_process.hProcess, 1), "TerminateProcess");
        }

        assertOK(CloseHandle(m_process.hProcess), "CloseHandle");
        assertOK(CloseHandle(m_process.hThread) , "CloseHandle");

        //if (m_file_stdin_reader) {
        //    assertOK(CloseHandle(m_file_stdin_reader), "CloseHandle");
        //    m_file_stdin_reader = nullptr;
        //}

        if (m_file_stdout_writer) {
            assertOK(CloseHandle(m_file_stdout_writer), "CloseHandle");
            m_file_stdout_writer = nullptr;
        }

        if (m_pipe_stdout_reader) {
            CloseHandle(m_pipe_stdout_reader);
            m_pipe_stdout_reader = nullptr;
        }

        //if (m_pipe_stdout_writer) {
        //    CloseHandle(m_pipe_stdout_writer);
        //    m_pipe_stdout_writer = nullptr;
        //}

        //LOGT << "stop process '" << m_app << "' OK";
    } catch (std::exception const& e) {
        throw std::runtime_error("process '" + m_command_line + "' finalization error: " + e.what()); // ----->
    }
}


IProcess::TState CProcessStream::getState() {
    try {
        if (!m_pipe_stdout_reader)
            return { IProcess::TState::TCondition::UNKNOWN, nullptr };

        IProcess::TState    result      {};
        DWORD               exit_code   {};

        assertOK(GetExitCodeProcess(m_process.hProcess, &exit_code), "GetExitCodeProcess");

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

        LOGT << "process: id: " << uint64_t(m_pipe_stdout_reader) << ", state: " << result.condition;
        return result; // ----->
    } catch (std::exception const& e) {
        throw std::runtime_error("process '" + m_command_line + "' get state error: " + e.what()); // ----->
    }
}


io::URI::TSharedPtr CProcessStream::getURI() const {
    return m_uri;
}


std::list<uintptr_t> CProcessStream::getHandles() const {
    try {
        if (!m_pipe_stdout_reader)
            return {};
            //throw std::runtime_error("not initialized"); // ----->

        //return m_overlapped.hEvent; // waitformultipleobjects----->
        return std::list<uintptr_t>{ reinterpret_cast<uintptr_t>(m_pipe_stdout_reader) }; // ----->

    } catch (std::exception const& e) {
        throw std::runtime_error("process '" + m_command_line + "' get id error: " + e.what()); // ----->
    }
}


io::Buffer::TSharedPtr CProcessStream::read(size_t const &size) {
    try {
        if (!m_pipe_stdout_reader)
            throw std::runtime_error("not initialized"); // ----->

        auto buffer = io::Buffer::create(size, 0);

        //LOGT << "read ...";
        auto result = ReadFile(m_pipe_stdout_reader, buffer->data(), static_cast<DWORD>(buffer->size()), nullptr, &m_overlapped);
        if  (result) {
            assertOK(result, "ReadFile");
            DWORD size = 0;
            GetOverlappedResult(m_pipe_stdout_reader, &m_overlapped, &size, true);
            buffer->resize(size);
            //LOGT << "read, size: " << size;
        } else {
            auto code = GetLastError();
            if (code == ERROR_IO_PENDING) {
                //buffer->clear();
                DWORD size = 0;
                GetOverlappedResult(m_pipe_stdout_reader, &m_overlapped, &size, true);
                buffer->resize(size);
                //LOGT << "read, pending, size: " << size;
            } else
                assertOK(result, "ReadFile");
        }
        //LOGT << "overlapped: " << m_overlapped.Offset << " " << m_overlapped.OffsetHigh << " " << (uintptr_t)m_overlapped.Pointer;
        //LOGT << "read OK, size: " << buffer->size();

        return buffer; // ----->
    } catch (std::exception const& e) {
        throw std::runtime_error("process '" + m_command_line + "' stdout read error: " + e.what()); // ----->
    }
}


size_t CProcessStream::write(io::Buffer::TSharedPtr const &buffer) {
    throw std::runtime_error("process stream is not implemented"); // ----->
}


} // platform
} // implementation
} // system
} // iridium


#endif // WINDOWS_PLATFORM
