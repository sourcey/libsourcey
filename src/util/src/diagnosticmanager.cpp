///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup util
/// @{


#include "scy/util/diagnosticmanager.h"


using std::endl;


namespace scy {


IDiagnostic::IDiagnostic()
{
}


IDiagnostic::~IDiagnostic()
{
}


void IDiagnostic::reset()
{
    summary.clear();
    setState(this, DiagnosticState::None);
}


void IDiagnostic::check()
{
    reset();
    run();
}


void IDiagnostic::addSummary(const std::string& text)
{
    summary.push_back(text);
    SummaryUpdated.emit(/*this, */ text);
}


bool IDiagnostic::pass()
{
    return setState(this, DiagnosticState::Passed);
}


bool IDiagnostic::fail()
{
    return setState(this, DiagnosticState::Failed);
}


bool IDiagnostic::complete() const
{
    return stateEquals(DiagnosticState::Passed) ||
           stateEquals(DiagnosticState::Failed);
}


bool IDiagnostic::passed() const
{
    return stateEquals(DiagnosticState::Passed) ||
           stateEquals(DiagnosticState::Failed);
}


bool IDiagnostic::failed() const
{
    return stateEquals(DiagnosticState::Failed);
}


// ---------------------------------------------------------------------
//
DiagnosticManager::DiagnosticManager()
{
    TraceL << "Create" << endl;
}


DiagnosticManager::~DiagnosticManager()
{
    TraceL << "Destroy" << endl;
}

void DiagnosticManager::resetAll()
{
    Map tests= map();
    for (auto& test : tests) {
        test.second->reset();
    }
}


void DiagnosticManager::checkAll()
{
    Map tests= map();
    for (auto& test : tests) {
        test.second->check();
    }
}


bool DiagnosticManager::allComplete()
{
    Map tests= map();
    for (auto& test : tests) {
        if (!test.second->complete())
            return false;
    }
    return true;
}


bool DiagnosticManager::addDiagnostic(IDiagnostic* test)
{
    assert(test);
    assert(!test->name.empty());

    TraceL << "Adding Diagnostic: " << test->name << endl;
    // test->StateChange += sdelegate(this,
    // &DiagnosticManager::onDiagnosticStateChange);
    return DiagnosticStore::add(test->name, test);
}


bool DiagnosticManager::freeDiagnostic(const std::string& name)
{
    assert(!name.empty());

    TraceL << "Removing Diagnostic: " << name << endl;
    IDiagnostic* test= DiagnosticStore::remove(name);
    if (test) {
        // TODO:
        // test->StateChange -= sdelegate(this,
        // &DiagnosticManager::onDiagnosticStateChange);
        delete test;
        return true;
    }
    return false;
}


IDiagnostic* DiagnosticManager::getDiagnostic(const std::string& name)
{
    return DiagnosticStore::get(name, true);
}


void DiagnosticManager::onDiagnosticStateChange(void* sender,
                                                DiagnosticState& state,
                                                const DiagnosticState&)
{
    auto test= reinterpret_cast<IDiagnostic*>(sender);
    TraceL << "Diagnostic state change: " << test->name << ": " << state
           << endl;

    if (test->complete() && allComplete())
        DiagnosticsComplete.emit(/*this*/);
}


} // namespace scy
