///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Test_H
#define SCY_Test_H


#include "scy/interface.h"
#include "scy/mutex.h"
#include <iostream>
#include <list>
#include <map>


namespace scy {

  
/// Modern unit testing classes
namespace test {


class Test;
class TestRunner;

typedef std::function<void()> voidfunc_t;
typedef std::list<Test*> test_list_t;
typedef std::list<std::string> error_list_t;
typedef std::map<Test*, error_list_t> error_map_t;

/// Initialize the test environment.
void initialize();

/// Finalize the test environment.
///
/// Destroy the TestRunner singleton instance and return the exit code.
int finalize();

/// Run all tests.
void runAll();

/// Describe a test environment implemented by the given lambda function.
void describe(const std::string& name, voidfunc_t target);

/// Describe a test environment implemented by the given test instance.
void describe(const std::string& name, Test* test);

/// Expect asserts that a condition is true (use expect() as defined below).
void expectImpl(bool passed, const char* assert, const char* file, long line);

// Shamelessly define macros to aesthetic name :)
#ifdef NDEBUG
#define expect(x) test::expectImpl(true, "", "", 0)
#else
#define expect(x) test::expectImpl(x, #x , __FILE__, __LINE__)
#endif


//
// Test
//

/// This class is for implementing any kind
/// async test that is compatible with a TestRunner.
class Test
{
public:
    Test(const std::string& name = "Unnamed Test");

    /// Should remain protected.
    virtual ~Test();

    /// Called by the TestRunner to run the test.
    virtual void run() = 0;

    /// Return true when the test passed without errors.
    bool passed();

    /// The name of the test.
    std::string name;

    /// A list of test errors.
    error_list_t errors;

    /// The test run duration for benchmarking.
    double duration;

protected:
    Test(const Test& test);
    Test& operator=(Test const&);

    /// Tests belong to a TestRunner instance.
    friend class TestRunner;
};


class FunctionTest: public Test
{
public:
    voidfunc_t target;

    FunctionTest(voidfunc_t target, const std::string& name = "Unnamed Test") :
        Test(name), target(target)
    {
    }

protected:
    virtual ~FunctionTest()
    {
    }

    void run()
    {
        target();
    }
};


//
// Test Runner
//


/// The TestRunner is a queue in charge of running one or many tests.
///
/// The TestRunner continually loops through each test in
/// the test list calling the test's run() method.
class TestRunner
{
public:
    TestRunner();
    virtual ~TestRunner();

    /// Add a test to the runner.
    void add(Test* test);

    /// Return a pointer to the test matching the given name,
    /// or nullptr if no matching test exists.
    Test* get(const std::string& name) const;

    /// Called by the async context to run the next test.
    void run();

    /// Destroy and clears all managed tests.
    void clear();

    /// Return the currently active Test or nullptr.
    Test* current() const;

    /// Return the list of tests.
    test_list_t tests() const;

    /// Return a map of tests and errors.
    error_map_t errors() const;

    /// Return true if all tests passed.
    bool passed() const;

    /// Return the default TestRunner singleton, although
    /// TestRunner instances may be initialized individually.
    static TestRunner& getDefault();

protected:
    mutable Mutex    _mutex;
    std::list<Test*> _tests;
    Test* _current;
};


} } // namespace scy::test


#endif // SCY_Test_H

/// @\}
