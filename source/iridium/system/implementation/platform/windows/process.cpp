#include "process.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/io/event.h"


using std::chrono::milliseconds;
using std::chrono::system_clock;


milliseconds DEFAULT_PROCESS_TIMEOUT        (1000);
milliseconds DEFAULT_PROCESS_TIMEOUT_STEP   (100);


#include "iridium/logging/logger.h"
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
        throw std::runtime_error(method + " error: 0x" + 
            convertion::convert<std::string, uint64_t>(error_message_code, 16) + " " + message); // ----->
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
    //m_overlapped            { 0 },
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
    //m_overlapped        { 0 },
    m_buffer            (io::Buffer::create())
{
    for (auto const &arg: args)
        m_args += arg + " ";
}


void CProcessStream::initialize() {
    try {
        //LOGT << "start process ...\n" << m_command_line;

        if (m_pipe_stdout_reader)
            throw std::runtime_error("not finalized"); // ----->

        STARTUPINFOA startup_info{};
        startup_info.dwFlags |= STARTF_USESTDHANDLES;

        SECURITY_ATTRIBUTES &security_attributes = m_security_attributes;
        security_attributes = { 0 };
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
            CreateFileA(
                m_pipe_name.c_str(), 
                FILE_GENERIC_WRITE, 0, 
                &security_attributes, 
                OPEN_EXISTING, 
                FILE_ATTRIBUTE_NORMAL, 
                nullptr),
           "CreateFile");

        startup_info.hStdOutput = m_file_stdout_writer;
        startup_info.hStdError  = m_file_stdout_writer;

        assertOK(
            CreateProcessA(
                nullptr, 
                const_cast<LPSTR>(m_command_line.c_str()), 
                nullptr, 
                nullptr, 
                true, 0, 
                nullptr, 
                nullptr, 
                &startup_info, 
                &m_process),
           "CreateProcess"
        );

        //m_overlapped = { 0 };
        //m_overlapped.hEvent = assertOK(CreateEvent(&security_attributes, true, false, nullptr), "CreateEvent");
        //ConnectNamedPipe(m_pipe_stdout_reader, &m_overlapped);

        //{
        //    auto result  = WaitForSingleObject(m_overlapped.hEvent, 1000);
        //    if  (result != WAIT_OBJECT_0)
        //        assertOK(result, "WaitForSingleObject");
        //}

        //ResetEvent(m_overlapped.hEvent);

        //m_overlapped.hEvent = assertOK(CreateEvent(&security_attributes, true, false, nullptr), "CreateEvent");
        //read(0);

        //LOGT << "start process OK\n" << m_app;
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

        //LOGT << "process: fd: " << uint64_t(m_pipe_stdout_reader) << ", state: " << result.condition;
        return result; // ----->
    } catch (std::exception const& e) {
        throw std::runtime_error("process '" + m_command_line + "' get state error: " + e.what()); // ----->
    }
}


io::URI::TSharedPtr CProcessStream::getURI() const {
    return m_uri;
}


std::list<uintptr_t> CProcessStream::getHandles() const {
    //try {
        //LOGT << "get process fd: " << (uint64_t)m_pipe_stdout_reader;
        if (m_pipe_stdout_reader)
            return std::list<uintptr_t>{ reinterpret_cast<uintptr_t>(m_pipe_stdout_reader) }; // ----->
        else
            return {}; // ----->

    //} catch (std::exception const& e) {
    //    throw std::runtime_error("process '" + m_command_line + "' get id error: " + e.what()); // ----->
    //}
}


io::Buffer::TSharedPtr CProcessStream::read(size_t const &size) {
    try {
        if (!m_pipe_stdout_reader)
            throw std::runtime_error("not initialized"); // ----->

        if (m_overlapped && m_buffer) {
            DWORD      size        = 0;
            //OVERLAPPED overlapped  = { 0 };

            auto result = GetOverlappedResult(m_pipe_stdout_reader, m_overlapped.get(), &size, false);
            //LOGT << "GetOverlappedResult, result: " << result;

            //if (result) {
            //    LOGT    
            //        <<  "overlapped offset  : "
            //        << m_overlapped->OffsetHigh << ":" 
            //        << m_overlapped->Offset;
            //    LOGT 
            //        <<  "overlapped interlal: "
            //        << m_overlapped->InternalHigh << ":" 
            //        << m_overlapped->Internal;
            //    LOGT 
            //        <<  "overlapped pointer : "
            //        << io::Event::TOperation(reinterpret_cast<intptr_t>(m_overlapped->Pointer));
            //}

            if (result) {
                //LOGT << "GetOverlappedResult, size: " << size;
                m_buffer->resize(size);

                //LOGT
                //    << "read, fd: " << reinterpret_cast<uintptr_t>(m_pipe_stdout_reader)
                //    << ", pending, buffer:\n'" << m_buffer
                //    << "'\nsize: " << (m_buffer ? m_buffer->size() : 0);
            } else {
                return io::Buffer::create();
            }
        }

        auto buffer = m_buffer;
        // new buffer for next reading
        m_buffer = io::Buffer::create(size, 0);

        //LOGT
        //    << "read, fd: " << reinterpret_cast<uintptr_t>(m_pipe_stdout_reader)
        //    << ", ReadFile ...";

        //auto overlapped = new OVERLAPPED { 0 };
        m_overlapped = std::make_shared<OVERLAPPED>();
        m_overlapped->Pointer = reinterpret_cast<PVOID>(static_cast<intptr_t>(io::Event::TOperation::READ));
        //LOGT << "overlapped->Pointer: " << 
        //    static_cast<io::Event::TOperation>(
        //    reinterpret_cast<intptr_t>(m_overlapped->Pointer));

        DWORD size_ = 0;
        auto result = ReadFile(
            m_pipe_stdout_reader, 
            m_buffer->data(), 
            static_cast<DWORD>(m_buffer->size()), 
            &size_, 
            m_overlapped.get());

        //LOGT
        //    << "read, fd: " << reinterpret_cast<uintptr_t>(m_pipe_stdout_reader)
        //    << ", ReadFile size: " << size_;

        if (result) {
            assertOK(result, "ReadFile");
        } else {
            auto code = GetLastError();
            //LOGT << "code: " << code;
            if (code == ERROR_IO_PENDING) {
                //if (size_ == 0)
                //    m_buffer.reset();
            } else {
                assertOK(result, "ReadFile");
                if (size_ == 0) {
                    m_buffer.reset();

                    //auto overlapped = new OVERLAPPED{ 0 };
                    //overlapped->Pointer = reinterpret_cast<PVOID>(static_cast<intptr_t>(Event::TOperation::OPEN));
                    //overlapped->hEvent = CreateEvent(nullptr, false, false, nullptr);

                    //assertOK(
                    //    PostQueuedCompletionStatus(
                    //        m_iocp, 0,
                    //        m_pipe_stdout_reader,
                    //        overlapped),
                    //    "send event OPEN END");

                }
            }
        }

        //LOGT 
        //    << "read, fd: " << reinterpret_cast<uintptr_t>(m_pipe_stdout_reader)
        //    << ", return pending buffer:\n'" << buffer
        //    << "'\nsize: " << (buffer ? buffer->size() : 0);

        //threading::sleep(1000);
        //if (buffer->empty() && m_buffer->empty())
        //    return nullptr;
        //else
        return buffer; // ----->
    } catch (std::exception const &e) {
        throw std::runtime_error("process '" + m_command_line + "' stdout read error: " + e.what()); // ----->
    }
}


size_t CProcessStream::write(io::Buffer::TSharedPtr const &buffer) {
    throw std::runtime_error("process stream write is not implemented"); // ----->
}


} // platform
} // implementation
} // system
} // iridium


#endif // WINDOWS_PLATFORM
