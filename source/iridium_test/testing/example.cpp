#include <iridium/testing/tester.h>

#include <string>
#include <unordered_map>


using namespace std;


//TEST(bool_) {
//    ASSERT(1 == 2);
//    ASSERT(true);
//}


//TEST(throw_) {
//    std::string s;
//    ASSERT(s.empty(), std::exception);
//}


//TEST(comparing) {
//    ASSERT(7, less, 5);
//}


//TEST(uncached_throw) {
//    throw std::runtime_error("uncached_throw");
//}


#include <iridium/threading/implementation/worker_pool.h>
#include <iridium/items.h>
#include <sys/types.h>
#include <unistd.h>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <fcntl.h>
#include <cstring>

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <spawn.h>
#include <signal.h>

#include "iridium/system/shell.h"


//using iridium::threading::IWorkerPool;
using iridium::threading::IWorker;
using iridium::threading::implementation::CWorkerPool;
using iridium::convertion::convert;

using iridium::system::Command;


#include "iridium/io/implementation/pipe.h"
using iridium::io::IPipe;
using iridium::io::implementation::CPipe;
using iridium::pattern::IInitializable;


//#ifdef WINDOWS_PLATFORM
//string getCurrentPID() {
//    return convertion::convert<string>(getpid());
//}#include <iostream>
//#include <stdexcept>
//#include <stdio.h>
//#include <string>
//#else
//string getCurrentPID() {
//    return convert<string>(getpid());
//}
//#endif

//struct sigaction sigchld_action = {
//  .sa_handler   = SIG_DFL,
//  .sa_flags     = SA_NOCLDWAIT
//};


using namespace iridium::io;


#include <iridium/system/implementation/process.h>


//using iridium::system::implementation::CProcessStream;
//using iridium::system::IProcess;


//class Process {
//public:
//    Process() = default;
//   ~Process() = default;

//    struct TResult {
//        DEFINE_ENUM(TCode,
//            OK,
//            ERROR,
//            CRASH,
//            TIMEOUT
//        );

//        std::list<std::string> output;
//        TCode code = TCode::UNKNOWN;
//    };

//    TResult run(
//        std::string const &command,
//        std::chrono::seconds const &timeout_ms  = std::chrono::seconds(10))
//    {
//        IListener::TSharedPtr   listener        = implementation::CListener::create();
//        IProcess::TSharedPtr    process         = CProcessStream::create(command, "", listener);

//        listener->initialize();
//        process->initialize();
//        listener->add(process);

//        TResult result;
////        sleep(2);
////        auto buffer = process->read(256);
////        result.output.push_back(convert<string>(*buffer));

//        auto end = std::chrono::system_clock::now() + timeout_ms;
//        bool is_continue = true;
//        while (is_continue) {
//            LOGT << "state: " << process->getState();
//            if (end < std::chrono::system_clock::now()) {
//                LOGT << "timeout";
//                process->finalize();
//                is_continue = false;
//                break;
//            }

////            LOGT << "wait 1";
//            for (auto const &event: listener->wait()) {
////                LOGT << 11;
////                LOGT << event->stream->getID() << " " << event->type;
////                LOGT << 22;
//                auto buffer = process->read();
//                if (buffer) {
////                    LOGT << "stdout: " << buffer;
////                    result.output.push_back(convert<string>(buffer));
//                } else {
//                    LOGT << "buffer = null";
//                }
//                if (iridium::checkOneOf(
//                    event->type,
//                    IListener::Event::TType::EOF_,
//                    IListener::Event::TType::ERROR))
//                {
//                    is_continue = false;
//                    break;
//                }
//            }
////            LOGT << "wait 2";
//        }

//        process->finalize();
//        LOGT << "finalized state: " << process->getState();
//        listener->del(process);
//        listener->finalize();

//        LOGT << "exit";
//        return result;


////        TResult result_;
////        char buffer[128];

////        FILE* pipe = popen(command.c_str(), "r");

////        if (!pipe) throw std::runtime_error("popen() failed!");

////        try {
////            while (fgets(buffer, sizeof buffer, pipe) != NULL) {
////                result_.output.push_back(buffer);
////                LOGT << "buffer = '" << buffer << "'";
////            }
////        } catch (...) {
////            pclose(pipe);
////            throw;
////        }
////        pclose(pipe);
////        result_.code = TResult::TCode::OK;
////        return result_;
//    }
//};


//IMPLEMENT_ENUM(Process::TResult::TCode);


//class CTestHandler: public iridium::threading::IWorkerHandler<int> {
//public:
//    DEFINE_IMPLEMENTATION(CTestHandler)
//    CTestHandler() = default;

//};


//class CTesterHandler: public IWorkerHandler<std::string> {
//public:
//    DEFINE_IMPLEMENTATION(CTesterHandler)
//    CTesterHandler() = default;

//    void initialize() override {};
//    void finalize() override {};

//    TItems handle(TItems const &paths) override {
//        for (auto &path: paths) {
//            Process process;
//            auto result = process.run(path);

//            LOGT << path << ": code: " << result.code;
//            for (auto const &line: result.output)
//                LOGT << path << ": line: " << line;
//        }
//        return {};

//        for (auto &path: paths) {
//            auto run_path = "./iridium_test run " + path + " 2>&1 > ." + path + ".log";
//            LOGT << "! run process " << run_path << " ...";
////            system(run_path.c_str());


////            int ret = 0;
//            pid_t child_pid;
//            posix_spawn_file_actions_t child_fd_actions;
//            if (int ret = posix_spawn_file_actions_init (&child_fd_actions))
//                perror ("posix_spawn_file_actions_init"), exit(ret);
//            if (int ret = posix_spawn_file_actions_addopen (&child_fd_actions, 1, ("/tmp/" + path + ".log").c_str(),
//                   O_WRONLY | O_CREAT | O_TRUNC, 0644))
//                perror ("posix_spawn_file_actions_addopen"), exit(ret);
//            if (int ret = posix_spawn_file_actions_adddup2 (&child_fd_actions, 1, 2))
//                perror ("posix_spawn_file_actions_adddup2"), exit(ret);

//            string s0 = "iridium_test", s1 = "--help";
//            int argc = 2;
//            char *argv[2];
//            char *env[2];
//            argv[0] = const_cast<char *>(s0.c_str());
//            argv[1] = const_cast<char *>(s1.c_str());
//            if (int ret = posix_spawnp (&child_pid, "date", &child_fd_actions, NULL, argv, env))
//                perror ("posix_spawn"), exit(ret);

//            LOGT << "! run process " << run_path << " done";
//        }
//    }
//};


TEST(OK) {
    string output;
    try {
        auto result = Command("./child2", std::chrono::seconds(5)).run(output);
        LOGT << output;
        LOGT << result;
    } catch (Command::Exception const &e) {
        LOGE << "error: " << e.what();
        LOGE << "state: " << e.getState().condition;
        LOGT << "output: " << output;
    }
}
