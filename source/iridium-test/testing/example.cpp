#define DEFINE_MOCK_CREATE

#include <iridium/testing/tester.h>
#include <iridium/testing/mock.h>


#include <iridium/system/implementation/process.h>
#include <iridium/io/implementation/multiplexer.h>
#include <iridium/items.h>


TEST(bool_) {
    ASSERT(1 == 1);
    ASSERT(true);
}


TEST(exception) {
    ASSERT(throw std::runtime_error("exception"), std::exception);
}


TEST(comparing_equal) {
    ASSERT(5, equal, 5);
}


TEST(comparing_less) {
    ASSERT(5, less, 7);
}


TEST(comparing_less_equal) {
    ASSERT(5, lessEqual, 5);
}


TEST(comparing_greater) {
    ASSERT(7, greater, 5);
}


TEST(comparing_greater_equal) {
    ASSERT(5, greaterEqual, 5);
}


class IDatabase {
public:
    DEFINE_INTERFACE(IDatabase)
    virtual std::string getUserName(int const &group_id, int   const &user_id) = 0;
    virtual std::string getUserName(int const &group_id, float const &user_id) = 0;
};


DEFINE_MOCK_CLASS(IDatabase) {
    DEFINE_MOCK_METHOD(std::string, getUserName, int const &, int const &);
    DEFINE_MOCK_METHOD(std::string, getUserName, int const &, float const &);
};


namespace some_namespace {

class CDatabase: public IDatabase {
public:
    CDatabase(std::string const &db_name)
    :
        m_db_name(db_name)
    {}
    DEFINE_IMPLEMENTATION(CDatabase)

    std::string getUserName(int const &group_id, int const &user_id) override {
        return "CDatabase: " + m_db_name + "getUserName " +
            convert<std::string>(group_id) + " " +
            convert<std::string>(user_id);
    }
    std::string getUserName(int const &group_id, float const &user_id) override {
        return getUserName(group_id, static_cast<int>(user_id)) + " f";
    }
    virtual std::string doSomething() const {
        return m_db_name + "soSomething const";
    }
private:
    virtual std::string doSomething() {
        return m_db_name + "soSomething";
    }

    std::string m_db_name;
};

} // some_namespace


using CDatabase = some_namespace::CDatabase;


//class CDatabaseMock: public CDatabase, public ::iridium::testing::Mock<CDatabase> {
//public:
DEFINE_MOCK_CLASS(CDatabase) {
    DEFINE_MOCK_CONSTRUCTOR(CDatabase)
    DEFINE_MOCK_METHOD(std::string, getUserName, int const &, int const &);
    DEFINE_MOCK_METHOD(std::string, doSomething);
    DEFINE_MOCK_METHOD_CONST(std::string, doSomething);
};


class CDatabaseAdapter: public IDatabase {
public:
    DEFINE_IMPLEMENTATION(CDatabaseAdapter)
    CDatabaseAdapter(std::string const &name)
    :
        m_name      (name),
        m_database  (some_namespace::CDatabase::create("adapted_db_name"))
    {}
    std::string getUserName(int const &group_id, int const &user_id) override {
        return "adapted by '" + m_name + "': " + m_database->getUserName(group_id, user_id) + " ";// + getProperty();
    }

    std::string getUserName(int const &group_id, float const &user_id) override {
        return getUserName(group_id, static_cast<int>(user_id));
    }

private:
    std::string           m_name;
    IDatabase::TSharedPtr m_database;
};


DEFINE_MOCK_CLASS(CDatabaseAdapter) {
    DEFINE_MOCK_CONSTRUCTOR(CDatabaseAdapter)
    DEFINE_MOCK_METHOD     (std::string, getUserName, int const &, int const &);
};


TEST(mock) {
    CDatabaseMock db_mock("db_mock");

    DEFINE_MOCK_BEHAVIOR(std::string, getUserName, db_mock, int const &, int const &) {
        return "Alice int";
    };

    ASSERT("Alice int",   equal, db_mock.getUserName(1, 2));

    DEFINE_MOCK_BEHAVIOR(std::string, doSomething, db_mock) {
        return "doSomething";
    };

    ASSERT("doSomething", equal, db_mock.doSomething());

    DEFINE_MOCK_BEHAVIOR_CONST(std::string, doSomething, db_mock) {
        return "doSomething const";
    };

    const auto &db_mock_const = db_mock;
    ASSERT("doSomething const", equal, db_mock_const.doSomething());


//    Количество вызовов
//
//    Определяет, сколько раз метод должен (или может) быть вызван:
//        Times(n)              — ровно n раз.
//        Times(AtLeast(n))     — минимум n раз.
//        Times(AtMost(n))      — максимум n раз.
//        Times(AnyNumber())    — любое число раз.
//        Times(0)              — не должен быть вызван.
//
//    Сравнение аргументов
//
//    Определяет, с какими аргументами должен быть вызван метод:
//        With(ArgsAre(42, "test"))                 — точное совпадение по значению.
//        With(Truly(predicate))                    — кастомная функция для сравнения.
//        With(Field(&MyStruct::id, Eq(123)))       — сравнение определённого поля.
//        With(Pointwise(predicate, expected_args)) — сравнение нескольких аргументов с использованием предиката.
//
//    Порядок вызовов
//
//    Определяет в каком порядке должны вызываться методы:
//        InSequence(seq)   — вызовы идут строго в порядке.
//        After(other_call) — вызов происходит после другого.
//
//    Поведение/результат вызова
//
//    Определяет, что должно произойти при вызове:
//        WillOnce(Return(value))       — вернуть значение.
//        WillRepeatedly(Return(value)) — вернуть значение при каждом вызове.
//        WillOnce(Throw(exception))    — выбросить исключение.
//        WillOnce(Invoke(lambda))      — вызвать пользовательскую функцию.
//        WillOnce([](...) { ... })     — вернуть значение через лямбду.


//    auto sequence = ::iridium::testing::MockSequence<CDatabaseMock>(db_mock, "file_line_1")
//        .addExpectation("file_line_2", 1, 1, &CDatabaseMock::getUserName, 2, 2)
//        .addExpectation("file_line_3", 1, 1, &CDatabaseMock::getUserName, 3, 3);
//
//    sequence.step(&CDatabaseMock::getUserName, 2, 2);
//    sequence.step(&CDatabaseMock::getUserName, 3, 3);
//    ASSERT(sequence.step(&CDatabaseMock::getUserName, 3, 3), std::exception);
//
//    CDatabaseAdapter dba("CDatabaseAdapter");
//    ASSERT("adapted by 'CDatabaseAdapter': Alice int ", equal, dba.getUserName(1, 2));
//
//    ASSERT(CDatabase::create(), std::exception);
}


//TEST(uncached_throw) {
//    throw std::runtime_error("uncached_throw");
//}

//#include <set>

//#define MAX(a, b) (a > b ? a : b)


//struct Group {
//    std::string name;
//    int start;
//    int end;
//};


//std::unordered_map<std::string, std::shared_ptr<S> > m;
//std::list<S> l;
//bool has(std::string const &group_name, int id) {
//    // 1 fast
//    auto i = m.at(name);
//    return i != m.end() && id >= i->start && id <= i->stop;
//
//    // 2 slow
//    for (auto const &i: l)
//        if (i.name == group_name && id >= i.start && id <= i.stop)
//            return true;
//
//    // 3
//
//
//    return false;
//}


//std::map<int, std::shared_ptr<Group> > m;
//std::shared_ptr<Group> has(int id) {
//    auto i =  m.lower_bound(id);
//    if  (i != m.end() && i->second && i->second->start <= id && i->second->end >= id)
//        return i->second;
//
//    return {};
//}
//bool testHas(int id) {
//    auto g = has(id);
//    LOGT << id << " " << (g ? g->name : "none");
//    return static_cast<bool>(g);
//}
//TEST(OK) {
//    auto g1 = std::make_shared<Group>(Group{"name1", 5 , 10});
//    auto g2 = std::make_shared<Group>(Group{"name2", 12, 19});
//
//    m[g1->start]    = g1;
//    m[g1->end]      = g1;
//
//    m[g2->start]    = g2;
//    m[g2->end]      = g2;
//
//    ASSERT(!testHas(3));
//
//    ASSERT(testHas(5));
//    ASSERT(testHas(6));
//    ASSERT(testHas(9));
//
//    ASSERT(!testHas(11));
//
//    ASSERT(testHas(12));
//    ASSERT(testHas(15));
//    ASSERT(testHas(19));
//
//    ASSERT(!testHas(20));
//}


//TEST(OK) {
//  std::set<int> s;
//
//    s.insert(3);
//    s.insert(1);
//    s.insert(2);
//    s.insert(1);
//
//    for (auto const &i: s)
//        LOGT << i;

//    LOGT << "trace log";
//    LOGD << "debug log";
//    LOGI << "info  log";

//    //ASSERT(true);
//    //ASSERT(1, equal, 1);
//    //ASSERT("asd", equal, string("asd"));
//    //ASSERT(2, greater, 1);
//    double d = 5.5;
//    ASSERT(5.5, equal, d);
//}



//TEST(sleep) {
//    using iridium::threading::sleep;
//
//    for (uint32_t i = 0; i < 100; i++) {
//        LOGT << i;
//        sleep(1000);
//    }
//}


//template<typename TResult>
//TResult assertOK(TResult const &result, std::string const &method) {
//    if (result) {
//        return result;
//    } else {
//        DWORD error_message_code    = ::GetLastError();
//        LPSTR buffer                = nullptr;
//
//        size_t size = FormatMessageA(
//            FORMAT_MESSAGE_ALLOCATE_BUFFER  |
//            FORMAT_MESSAGE_FROM_SYSTEM      |
//            FORMAT_MESSAGE_IGNORE_INSERTS,
//            NULL, error_message_code,
//            MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
//            (LPSTR)&buffer, 0, NULL);
//
//        std::string message(buffer, size);
//        LocalFree(buffer);
//        throw std::runtime_error(method + " error: " + message); // ----->
//    }
//}



//// todo: move to /system/process
//TEST(process) {
//    using iridium::system::implementation::CProcessStream;
//    using iridium::io::implementation::CMultiplexer;
//    using iridium::io::Buffer;
//    using iridium::io::IEvent;
//    using iridium::convertion::convert;
//    using iridium::checkOneOf;
//
//    auto multiplexer = CMultiplexer::create();
//
//    multiplexer->initialize();
//
//    auto process = CProcessStream::create("iridium_test.exe", "run --mode=raw /testing/example.cpp/sleep");
//
//    multiplexer->subscribe(process);
//
//    std::list<Buffer::TSharedPtr> buffers;
//
//    for (int i = 0; i < 5; i++) {
//        LOGT << "wait events ...";
//        auto events = multiplexer->waitEvents();
//        LOGT << "wait events OK";
//
//        for (auto const& event_: events) {
//            LOGT << event_->getType();
//            if (checkOneOf(event_->getType(), IEvent::TType::READ, IEvent::TType::UNKNOWN)) {
//                auto buffer = process->read(1024);
//                buffers.push_back(buffer);
//                LOGT << "stdout:\n'" << buffer << "'";
//            }
//        }
//    }
//
//    LOGT << process->getState().condition << " " << process->getState().exit_code;
//    iridium::threading::sleep(100);
//    LOGT << process->getState().condition << " " << process->getState().exit_code;
//
//    multiplexer->unsubscribe(process);
//    multiplexer->finalize();
//
//    std::string output;
//    for (auto buffer: buffers)
//        output += convert<std::string>(*buffer);
//
//    LOGT << "\noutput:\n" << output;
//
//    //string output;
//    //try {
//    //    auto result = Command("./child2", std::chrono::seconds(5)).run(output);
//    //    LOGT << output;
//    //    LOGT << result;
//    //} catch (Command::Exception const &e) {
//    //    LOGE << "error: " << e.what();
//    //    LOGE << "state: " << e.getState().condition;
//    //    LOGT << "output: " << output;
//    //}
//}


//#include "iridium/threading/thread.h"
//#include <iridium/io/fs/implementation/file_stream_writer.h>
//TEST(timeout) {
//    LOGT << "sleep ...";
//    iridium::threading::sleep(30000);
//    LOGT << "sleep OK";
//
////    auto f = iridium::io::fs::implementation::CFileStreamWriter::create("test.file");
////    f->initialize();
////    for (int i = 0; i < 100; i++) {
////        f->CFileStream::write(iridium::io::Buffer::create(convert<std::string>(i) + "\n"));
////        iridium::threading::sleep(50);
////    }
////    f->finalize();
//}


//TEST(crash) {
//    int *a = nullptr;
//    *a = 5;
//}

//#include <simd/simd.h>
//#include <experimental/simd>
//TEST(simd_) {
//
//    auto simd_vector = std::experimental::simd<uint8_t>(1);
//
//    simd_vector++;
//
////    for (auto const &i: simd_vector)
//
//}
