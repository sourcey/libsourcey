//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#ifndef SCY_Test_H
#define SCY_Test_H


#include "scy/interface.h"
#include "scy/mutex.h"
#include <iostream>
#include <list>
#include <map>


namespace scy {
namespace test {


class Test;
class TestRunner;

typedef std::function<void()> voidfunc_t;
typedef std::list<Test*> test_list_t;
typedef std::list<std::string> error_list_t;
typedef std::map<Test*, error_list_t> error_map_t;

void initialize();
    // Initialize the test environment.

int finalize();
    // Finalize the test environment.
    //
    // Destroy the TestRunner singleton instance and return the exit code.

void runAll();
    // Run all tests.

void describe(const std::string& name, voidfunc_t target);
    // Describe a test environment implemented by the given lambda function.

void describe(const std::string& name, Test* test);
    // Describe a test environment implemented by the given test instance.

void expectImpl(bool passed, const char* assert, const char* file, long line);
    // Expect asserts that a condition is true (use expect() as defined below).

// Shamelessly define macros to aesthetic name :)
#ifdef NDEBUG
#define expect(x) test::expectImpl(true, "", "", 0)
#else
#define expect(x) test::expectImpl(x, #x , __FILE__, __LINE__)
#endif


//
// Test
//


class Test
    /// This class is for implementing any kind
    /// async test that is compatible with a TestRunner.
{
public:
    Test(const std::string& name = "Unnamed Test");

    virtual ~Test();
        // Should remain protected.

    virtual void run() = 0;
        // Called by the TestRunner to run the test.

    bool passed();
        // Return true when the test passed without errors.

    std::string name;
        // The name of the test.

    error_list_t errors;
        // A list of test errors.

    double duration;
        // The test run duration for benchmarking.

protected:
    Test(const Test& test);
    Test& operator=(Test const&);

    friend class TestRunner;
        // Tests belong to a TestRunner instance.
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


class TestRunner
    // The TestRunner is a queue in charge of running one or many tests.
    //
    // The TestRunner continually loops through each test in
    // the test list calling the test's run() method.
{
public:
    TestRunner();
    virtual ~TestRunner();

    void add(Test* test);
        // Add a test to the runner.

    Test* get(const std::string& name) const;
        // Return a pointer to the test matching the given name,
        // or nullptr if no matching test exists.

    void run();
        // Called by the async context to run the next test.

    void clear();
        // Destroy and clears all managed tests.

    Test* current() const;
        // Return the currently active Test or nullptr.

    test_list_t tests() const;
        // Return the list of tests.

    error_map_t errors() const;
        // Return a map of tests and errors.

    bool passed() const;
        // Return true if all tests passed.

    static TestRunner& getDefault();
        // Return the default TestRunner singleton, although
        // TestRunner instances may be initialized individually.

protected:
    mutable Mutex    _mutex;
    std::list<Test*> _tests;
    Test* _current;
};


} } // namespace scy::test


#endif // SCY_Test_H
