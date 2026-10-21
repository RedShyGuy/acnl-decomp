#include "nn/nex/nex_SystemComponent.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_SpinTest.h"
#include <cwchar>

namespace nn {
namespace nex {
// 0x00186BD0 (name is ours)
bool nn::nex::SystemComponent::WaitForTerminatedStateCallback(nn::nex::RootObject* pObject, unsigned int timeout)
{
    return static_cast<SystemComponent*>(pObject)->WaitForTerminatedState(timeout);
}

// 0x0037DDCC | fefates:bytes [tier B]
nn::nex::SystemComponent::_State nn::nex::SystemComponent::Initialize()
{
    OnInitialize();
    switch (m_State) {
    case STATE_CREATED:
    case STATE_TERMINATED:
        TryTransition(STATE_INITIALIZING);
        if (BeginInitialization()) {
            TryTransition(STATE_READY);
        }
        return m_State;
    case STATE_INITIALIZING:
        if (EndInitialization()) {
            TryTransition(STATE_READY);
        }
        return m_State;
    default:
        return m_State;
    }
}

// 0x0037DEE0 (slot name from the reference symbols)
void nn::nex::SystemComponent::OnTerminate()
{
    TestState();
}

// 0x0037DEEC (slot name from the reference symbols)
void nn::nex::SystemComponent::OnInitialize()
{
    TestState();
}

// 0x0037DEF8 | fefates:bytes
bool nn::nex::SystemComponent::UseIsAllowed()
{
    return m_State == STATE_READY || m_State == STATE_IN_USE;
}

// 0x0037DF10 (slot name from the reference symbols)
bool nn::nex::SystemComponent::EndTermination()
{
    return true;
}

// 0x0037DF18 (slot name from the reference symbols)
void nn::nex::SystemComponent::StateTransition(_State state)
{
    // nothing to do in this build
}

// 0x0037DF1C | fefates:bytes
bool nn::nex::SystemComponent::ValidTransition(_State state)
{
    if (state == STATE_FAULTY || state == STATE_UNKNOWN) {
        return true;
    }
    switch (m_State) {
    case STATE_CREATED:
        return state == STATE_INITIALIZING || state == STATE_TERMINATING || state == STATE_TERMINATED;
    case STATE_INITIALIZING:
        return state == STATE_READY;
    case STATE_READY:
        return state == STATE_IN_USE || state == STATE_TERMINATING;
    case STATE_IN_USE:
        return state == STATE_READY || state == STATE_TERMINATING_WHILE_IN_USE;
    case STATE_TERMINATING:
        return state == STATE_TERMINATED;
    case STATE_TERMINATING_WHILE_IN_USE:
        return m_UseCount == 0 && state == STATE_TERMINATING;
    case STATE_TERMINATED:
        return state == STATE_CREATED || state == STATE_INITIALIZING;
    case STATE_FAULTY:
    case STATE_UNKNOWN:
        return true;
    default:
        return false;
    }
}

// 0x0037E008 (slot name from the reference symbols)
bool nn::nex::SystemComponent::BeginTermination()
{
    return true;
}

// 0x0037E010 (slot name from the reference symbols)
bool nn::nex::SystemComponent::EndInitialization()
{
    return true;
}

// 0x0037E018 (slot name from the reference symbols)
bool nn::nex::SystemComponent::BeginInitialization()
{
    return true;
}

// 0x0037E020 | fefates:bytes-fuzzy [tier B]
nn::nex::qResult nn::nex::SystemComponent::WaitForTerminatedState(unsigned int timeout)
{
    Terminate();
    qResult result;
    if (Scheduler::PreventRegularBlockCall(timeout != 0, timeout, WaitForTerminatedStateCallback, this, &result)) {
        return result;
    }
    SpinTest spinTest(10, timeout);
    spinTest.m_Mode = SpinTest::MODE_STOP;
    while (TestState() != STATE_TERMINATED) {
        if (!spinTest.SpinOnce(const_cast<wchar_t*>(L"D:/home/jenkins/workspace/Manual-Pack/5129d543/OnlineCore/src/Core/SystemComponent.cpp"),
                               339, const_cast<wchar_t*>(L"TestState()!=Terminated"))) {
            break;
        }
    }
    return qResult(TestState() == STATE_TERMINATED ? qResult::SUCCESS : static_cast<int>(0x8001000B));
}

// 0x0037E16C | mk7dlp:bytes-fuzzy
// 0x0037E13C (deleting dtor)
nn::nex::SystemComponent::Use::~Use()
{
    if (!m_IsValid) {
        return;
    }
    SystemComponent* pComponent = m_pComponent;
    if (--pComponent->m_UseCount != 0) {
        return;
    }
    pComponent->TryTransition(pComponent->m_State == STATE_IN_USE ? STATE_READY : STATE_TERMINATING);
}

// 0x0037E204 (slot name from the reference symbols)
void nn::nex::SystemComponent::DoWork()
{
    // nothing to do in this build
}

// 0x0037E208 | fefates:callgraph [tier C]
void nn::nex::SystemComponent::SetName(const nn::nex::String& name)
{
    m_Name = name;
}

// 0x0037E210 | fefates:bytes [tier B]
nn::nex::SystemComponent::_State nn::nex::SystemComponent::Terminate()
{
    OnTerminate();
    switch (m_State) {
    case STATE_TERMINATING:
        if (EndTermination()) {
            TryTransition(STATE_TERMINATED);
        }
        return m_State;
    case STATE_CREATED:
    case STATE_INITIALIZING:
    case STATE_READY:
    case STATE_FAULTY:
    case STATE_UNKNOWN:
        StateTransition(STATE_TERMINATING);
        m_State = STATE_TERMINATING;
        if (BeginTermination()) {
            TryTransition(STATE_TERMINATED);
        }
        return m_State;
    case STATE_IN_USE:
        TryTransition(STATE_TERMINATING_WHILE_IN_USE);
        return m_State;
    case STATE_TERMINATING_WHILE_IN_USE:
        if (m_UseCount == 0) {
            TryTransition(STATE_TERMINATING);
            if (BeginTermination()) {
                TryTransition(STATE_TERMINATED);
            }
        }
        return m_State;
    default:
        return m_State;
    }
}

// 0x0037E3C0 (slot name from the reference symbols)
nn::nex::SystemComponent::_State nn::nex::SystemComponent::TestState()
{
    return m_State;
}

// 0x0037E3C8 | fefates:bytes [tier B]
nn::nex::SystemComponent::SystemComponent(const nn::nex::String& name) : m_Name(name), m_State(STATE_CREATED), m_UseCount(0), m_pGroup(nullptr)
{
}

// 0x0037E47C | fefates:callgraph
// 0x0037E420 (deleting dtor)
nn::nex::SystemComponent::~SystemComponent()
{
    if (m_pGroup != nullptr) {
        m_pGroup = nullptr;
    }
}

// 0x0072B6EC
const wchar_t* nn::nex::SystemComponent::GetType() const
{
    return L"SystemComponent";
}

// 0x0072B714
bool nn::nex::SystemComponent::IsAKindOf(const wchar_t* pType) const
{
    return wcscmp(L"SystemComponent", pType) == 0;
}

} // namespace nex
} // namespace nn
