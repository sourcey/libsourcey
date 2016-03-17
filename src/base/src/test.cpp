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


#include "scy/test.h"
#include "scy/logger.h"
#include "scy/memory.h"
#include "scy/singleton.h"
#include "scy/util.h"

#include <iostream>
#include <assert.h>


using std::cout;
using std::cerr;
using std::endl;


namespace scy {
namespace test {


static Singleton<TestRunner> singleton;


void describe(const std::string& name, std::function<void()> target)
{
    auto test = new FunctionTest(target, name);
    TestRunner::getDefault().add(test);
}


void describe(const std::string& name, Test* test)
{
    test->name = name;
    TestRunner::getDefault().add(test);
}


int finalize()
{
    bool passed = TestRunner::getDefault().passed();
    singleton.destroy();
    return passed ? 0 : 1;
}


void _expect(bool passed, const char* assert, const char* file, long line)
{
    if (passed)
        return;

    std::stringstream ss;
    ss << "failed on " << assert << " in " << file << " at " << line;

    auto test = TestRunner::getDefault().current();
    if (test) {
        test->errors.push_back(ss.str());
    }

    std::cout << ss.str() << std::endl;
}


//
// Test Runner
//


TestRunner::TestRunner() :
    _current(nullptr)
{
}


TestRunner::~TestRunner()
{
    clear();
}


void TestRunner::add(Test* test)
{
    cout << test->name << " added" << endl;
    Mutex::ScopedLock lock(_mutex);
    _tests.push_back(test);
}


Test* TestRunner::get(const std::string& name) const
{
    Mutex::ScopedLock lock(_mutex);
    for (auto it = _tests.begin(); it != _tests.end(); ++it) {
        if ((*it)->name == name)
            return *it;
    }
    return nullptr;
}


void TestRunner::clear()
{
    Mutex::ScopedLock lock(_mutex);
    util::clearList<Test>(_tests);
}


test_list_t TestRunner::tests() const
{
    Mutex::ScopedLock lock(_mutex);
    return _tests;
}


error_map_t TestRunner::errors() const
{
    error_map_t errors;
    test_list_t tests = this->tests();
    for (auto it = tests.begin(); it != tests.end(); ++it) {
        if (!(*it)->passed()) {
            errors[(*it)] = (*it)->errors;
        }
    }
    return errors;
}


bool TestRunner::passed() const
{
    return errors().empty();
}


Test* TestRunner::current() const
{
    Mutex::ScopedLock lock(_mutex);
    return _current;
}


void TestRunner::run()
{
    test_list_t tests = this->tests();
    for (auto it = tests.begin(); it != tests.end(); ++it) {
        cout << "---------------------------------------------------------------" << endl;
        cout << (*it)->name << " starting" << endl;
        (*it)->run();
        cout << (*it)->name << " ended" << endl;
    }
}


TestRunner& TestRunner::getDefault()
{
    return *singleton.get();
}


//
// Test
//


Test::Test(const std::string& name) :
    name(name)
{
}


Test::~Test()
{
}


bool Test::passed()
{
    return errors.empty();
}


} } // namespace scy::test
