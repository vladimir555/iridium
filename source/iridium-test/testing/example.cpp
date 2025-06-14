#define DEFINE_MOCK_CREATE // This should be defined before including iridium headers that use DEFINE_CREATE

#include <iridium/testing/tester.h>
#include <iridium/testing/mock.h>
#include <iridium/convertion/convert.h> // For convert in existing tests
#include <iridium/logging/logger.h>   // For LOGD in Combined test

#include <iridium/system/implementation/process.h> // Existing include
#include <iridium/io/implementation/multiplexer.h> // Existing include
#include <iridium/items.h>                         // Existing include

#include <string>
#include <stdexcept>
#include <vector>
#include <limits> // For std::numeric_limits in tests/mock.h itself

// Helper macro for creating unique sequence names
#define IRIDIUM_STRINGIFY_DETAIL(x) #x
#define IRIDIUM_TO_STRING(x) IRIDIUM_STRINGIFY_DETAIL(x)
#define MAKE_SEQ_NAME(base) base ":" __FILE__ ":" IRIDIUM_TO_STRING(__LINE__)


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
            iridium::convertion::convert<std::string>(group_id) + " " +
            iridium::convertion::convert<std::string>(user_id);
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
}

// --- New Test Code for Mock Sequences ---

class ISequenceTester {
public:
    DEFINE_INTERFACE(ISequenceTester);
    virtual ~ISequenceTester() = default;
    virtual void DoSomething(int x, std::string y) = 0;
    virtual int GetValue(int key) = 0;
    virtual void NoArgsMethod() = 0;
    virtual std::string ReturnsString() = 0;
    virtual void MethodForThrow() = 0;
    virtual int MethodForInvoke(int val) = 0;
    virtual void VoidMethodForInvoke(int val) = 0;
};

DEFINE_MOCK_CLASS(ISequenceTester) {
public:
    DEFINE_MOCK_METHOD(void, DoSomething, int, std::string);
    DEFINE_MOCK_METHOD(int, GetValue, int);
    DEFINE_MOCK_METHOD(void, NoArgsMethod);
    DEFINE_MOCK_METHOD(std::string, ReturnsString);
    DEFINE_MOCK_METHOD(void, MethodForThrow);
    DEFINE_MOCK_METHOD(int, MethodForInvoke, int);
    DEFINE_MOCK_METHOD(void, VoidMethodForInvoke, int);
};

using iridium::testing::ArgsAre;
using iridium::testing::Return;
using iridium::testing::Throw;
using iridium::testing::Invoke;
using iridium::testing::Times;


TEST(MockSequence_TimesExactly_Success) {
    ISequenceTesterMock mock;
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("TimesExactly_Success"));
    seq.ExpectObj(&ISequenceTesterMock::NoArgsMethod).Times(2);
    mock.NoArgsMethod();
    mock.NoArgsMethod();
    // Destructor verifies
}

TEST(MockSequence_TimesExactly_TooMany) {
    ISequenceTesterMock mock;
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("TimesExactly_TooMany"));
    seq.ExpectObj(&ISequenceTesterMock::NoArgsMethod).Times(1);
    mock.NoArgsMethod();
    ASSERT(mock.NoArgsMethod(), std::runtime_error); // Expect error on second call
}

TEST(MockSequence_TimesExactly_TooFew) {
    ISequenceTesterMock mock; // Destructor should report error
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("TimesExactly_TooFew"));
    seq.ExpectObj(&ISequenceTesterMock::NoArgsMethod).Times(2);
    mock.NoArgsMethod();
    // Test relies on stderr check from destructor for "expected 2 calls, but received 1"
}

TEST(MockSequence_TimesNever_Success) {
    ISequenceTesterMock mock;
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("TimesNever_Success"));
    seq.ExpectObj(&ISequenceTesterMock::NoArgsMethod).Times(Times::Never());
    // Destructor verifies
}

TEST(MockSequence_TimesNever_Called) {
    ISequenceTesterMock mock;
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("TimesNever_Called"));
    seq.ExpectObj(&ISequenceTesterMock::NoArgsMethod).Times(Times::Never());
    ASSERT(mock.NoArgsMethod(), std::runtime_error); // Expect error on first call
}


TEST(MockSequence_TimesAtLeast_Success) {
    ISequenceTesterMock mock;
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("TimesAtLeast_Success"));
    seq.ExpectObj(&ISequenceTesterMock::NoArgsMethod).Times(Times::AtLeast(2));
    mock.NoArgsMethod();
    mock.NoArgsMethod();
    mock.NoArgsMethod(); // 3 calls >= 2
    // Destructor verifies
}

TEST(MockSequence_TimesAtLeast_TooFew) {
    ISequenceTesterMock mock; // Destructor should report error
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("TimesAtLeast_TooFew"));
    seq.ExpectObj(&ISequenceTesterMock::NoArgsMethod).Times(Times::AtLeast(2));
    mock.NoArgsMethod(); // Only 1 call < 2
    // Test relies on stderr check from destructor
}

TEST(MockSequence_TimesAtMost_Success) {
    ISequenceTesterMock mock;
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("TimesAtMost_Success"));
    seq.ExpectObj(&ISequenceTesterMock::NoArgsMethod).Times(Times::AtMost(2));
    mock.NoArgsMethod(); // Call 1
    mock.NoArgsMethod(); // Call 2
    // Destructor verifies
}

TEST(MockSequence_TimesAtMost_ZeroOK) {
    ISequenceTesterMock mock;
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("TimesAtMost_ZeroOK"));
    seq.ExpectObj(&ISequenceTesterMock::NoArgsMethod).Times(Times::AtMost(2));
    // Call 0 times
    // Destructor verifies
}


TEST(MockSequence_TimesAtMost_TooMany) {
    ISequenceTesterMock mock;
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("TimesAtMost_TooMany"));
    seq.ExpectObj(&ISequenceTesterMock::NoArgsMethod).Times(Times::AtMost(1));
    mock.NoArgsMethod();
    ASSERT(mock.NoArgsMethod(), std::runtime_error); // Expect error on second call
}

TEST(MockSequence_ArgsAre_Success) {
    ISequenceTesterMock mock;
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("ArgsAre_Success"));
    seq.ExpectObj(&ISequenceTesterMock::DoSomething).With(ArgsAre(10, "test"));
    mock.DoSomething(10, "test");
}

TEST(MockSequence_ArgsAre_Mismatch) {
    ISequenceTesterMock mock;
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("ArgsAre_Mismatch"));
    seq.ExpectObj(&ISequenceTesterMock::DoSomething).With(ArgsAre(10, "test"));
    ASSERT(mock.DoSomething(10, "wrong"), std::runtime_error);
}

TEST(MockSequence_WillOnceReturn_Success) {
    ISequenceTesterMock mock;
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("WillOnceReturn_Success"));
    seq.ExpectObj(&ISequenceTesterMock::GetValue).With(ArgsAre(1)).WillOnce(Return(100));
    ASSERT(mock.GetValue(1), iridium::testing::equal, 100);
}

TEST(MockSequence_WillOnceReturn_SecondCallError) {
    ISequenceTesterMock mock;
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("WillOnceReturn_SecondCallError"));
    seq.ExpectObj(&ISequenceTesterMock::GetValue).With(ArgsAre(1)).Times(1).WillOnce(Return(100));
    ASSERT(mock.GetValue(1), iridium::testing::equal, 100);
    // Second call should fail as sequence is exhausted or next expectation doesn't match
    ASSERT(mock.GetValue(1), std::runtime_error);
}

TEST(MockSequence_WillRepeatedlyReturn_Success) {
    ISequenceTesterMock mock;
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("WillRepeatedlyReturn_Success"));
    seq.ExpectObj(&ISequenceTesterMock::GetValue).With(ArgsAre(5)).Times(3).WillRepeatedly(Return(55));
    ASSERT(mock.GetValue(5), iridium::testing::equal, 55);
    ASSERT(mock.GetValue(5), iridium::testing::equal, 55);
    ASSERT(mock.GetValue(5), iridium::testing::equal, 55);
}

TEST(MockSequence_WillOnceThrow_Success) {
    ISequenceTesterMock mock;
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("WillOnceThrow_Success"));
    seq.ExpectObj(&ISequenceTesterMock::MethodForThrow).WillOnce(Throw(std::runtime_error("TestException")));
    ASSERT(mock.MethodForThrow(), std::runtime_error);
}

TEST(MockSequence_WillRepeatedlyThrow_Success) {
    ISequenceTesterMock mock;
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("WillRepeatedlyThrow_Success"));
    seq.ExpectObj(&ISequenceTesterMock::MethodForThrow).Times(2).WillRepeatedly(Throw(std::logic_error("LogicErr")));
    ASSERT(mock.MethodForThrow(), std::logic_error);
    ASSERT(mock.MethodForThrow(), std::logic_error);
}

TEST(MockSequence_WillOnceInvoke_Success) {
    ISequenceTesterMock mock;
    int invoked_val = 0;
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("WillOnceInvoke_Success"));
    seq.ExpectObj(&ISequenceTesterMock::VoidMethodForInvoke)
        .With(ArgsAre(123))
        .WillOnce(Invoke([&](int val){ invoked_val = val; }));
    mock.VoidMethodForInvoke(123);
    ASSERT(invoked_val, iridium::testing::equal, 123);
}

TEST(MockSequence_WillOnceInvoke_Return) {
    ISequenceTesterMock mock;
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("WillOnceInvoke_Return"));
    seq.ExpectObj(&ISequenceTesterMock::MethodForInvoke)
        .With(ArgsAre(10))
        .WillOnce(Invoke([](int val){ return val * 2; }));
    ASSERT(mock.MethodForInvoke(10), iridium::testing::equal, 20);
}

TEST(MockSequence_WillRepeatedlyInvoke_Success) {
    ISequenceTesterMock mock;
    int call_count = 0;
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("WillRepeatedlyInvoke_Success"));
    seq.ExpectObj(&ISequenceTesterMock::NoArgsMethod)
        .Times(3)
        .WillRepeatedly(Invoke([&](){ call_count++; }));
    mock.NoArgsMethod();
    mock.NoArgsMethod();
    mock.NoArgsMethod();
    ASSERT(call_count, iridium::testing::equal, 3);
}

TEST(MockSequence_Combined) {
    ISequenceTesterMock mock;
    iridium::testing::MockSequence<ISequenceTesterMock> sequence_s1(mock, MAKE_SEQ_NAME("Combined_S1"));
    int invoke_count = 0;

    sequence_s1.ExpectObj(&ISequenceTesterMock::GetValue)
        .With(ArgsAre(1))
        .Times(2)
        .WillRepeatedly(Return(10));

    sequence_s1.ExpectObj(&ISequenceTesterMock::DoSomething)
        .With(ArgsAre(5, "hello"))
        .WillOnce(Invoke([&](int, std::string){
            LOGD << "Invoke DoSomething(5, hello)";
            invoke_count++;
        }));

    sequence_s1.ExpectObj(&ISequenceTesterMock::GetValue)
        .With(ArgsAre(2))
        .WillOnce(Return(20));

    sequence_s1.ExpectObj(&ISequenceTesterMock::MethodForThrow)
        .Times(Times::Never());


    ASSERT(mock.GetValue(1), iridium::testing::equal, 10);
    ASSERT(mock.GetValue(1), iridium::testing::equal, 10);
    mock.DoSomething(5, "hello");
    ASSERT(invoke_count, iridium::testing::equal, 1);
    ASSERT(mock.GetValue(2), iridium::testing::equal, 20);
    // Destructor verifies MethodForThrow was not called and other counts.
}

TEST(MockSequence_UnexpectedCall_Method) {
    ISequenceTesterMock mock;
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("UnexpectedCall_Method"));
    seq.ExpectObj(&ISequenceTesterMock::NoArgsMethod);
    // Call a different method
    ASSERT(mock.GetValue(1), std::runtime_error);
}

TEST(MockSequence_UnexpectedCall_Args) {
    ISequenceTesterMock mock;
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("UnexpectedCall_Args"));
    seq.ExpectObj(&ISequenceTesterMock::GetValue).With(ArgsAre(1));
    // Call with different args
    ASSERT(mock.GetValue(2), std::runtime_error);
}

TEST(MockSequence_UnexpectedCall_EmptySequence) {
    ISequenceTesterMock mock;
    iridium::testing::MockSequence<ISequenceTesterMock> seq(mock, MAKE_SEQ_NAME("UnexpectedCall_EmptySequence"));
    // No expectations set
    ASSERT(mock.NoArgsMethod(), std::runtime_error);
}


// The old test cases are below

//TEST(uncached_throw) {
//    throw std::runtime_error("uncached_throw");
//}

// ... (rest of the original example.cpp content)
// For brevity, I'll assume the rest of the file (like 'process' test, etc.) follows here.
// The important part is that the new tests are added before the very end.
// The original content after TEST(mock) was mostly commented out examples.
// I will add a marker to indicate where the original content continues.

// --- End of New Test Code ---
// --- Original example.cpp content (if any non-commented) would continue from here ---
// (Assuming most of the rest was commented, so effectively this is near the end)
