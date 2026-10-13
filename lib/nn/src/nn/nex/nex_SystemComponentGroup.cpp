#include "nn/nex/nex_SystemComponentGroup.h"
#include <cwchar>

namespace nn {
namespace nex {
// 0x00397D40 (slot name from the reference symbols)
void nn::nex::SystemComponentGroup::OnTerminate()
{
    // nothing to do in this build
}

// 0x00397D44 (slot name from the reference symbols)
void nn::nex::SystemComponentGroup::OnInitialize()
{
    // nothing to do in this build
}

// 0x00397D48 (slot name from the reference symbols)
bool nn::nex::SystemComponentGroup::EndTermination()
{
    DoWork();
    return GetComponentsState() == STATE_TERMINATED;
}

// 0x00397D78 (slot name from the reference symbols)
bool nn::nex::SystemComponentGroup::BeginTermination()
{
    DoWork();
    return GetComponentsState() == STATE_TERMINATED;
}

// 0x00397DA8 (slot name from the reference symbols)
bool nn::nex::SystemComponentGroup::EndInitialization()
{
    DoWork();
    return GetComponentsState() == STATE_READY;
}

// 0x00397DD8 | fefates:bytes [tier B]
bool nn::nex::SystemComponentGroup::RegisterComponent(nn::nex::SystemComponent* pComponent)
{
    m_Components.push_back(pComponent);
    if (pComponent->m_pGroup != nullptr) {
        pComponent->m_pGroup = nullptr;
    }
    pComponent->m_pGroup = this;
    TestState();
    return true;
}

// 0x00397EC8 (slot name from the reference symbols)
bool nn::nex::SystemComponentGroup::BeginInitialization()
{
    DoWork();
    return GetComponentsState() == STATE_READY;
}

// 0x00397EF8 | fefates:bytes [tier B]
bool nn::nex::SystemComponentGroup::UnregisterComponent(nn::nex::SystemComponent* pComponent)
{
    qList<SystemComponent*>::Node* pNode = m_Components.GetFirst();
    while (!m_Components.IsEnd(pNode) && pNode->m_Value != pComponent) {
        pNode = pNode->m_pNext;
    }
    if (m_Components.IsEnd(pNode)) {
        return false;
    }
    m_Components.erase(pNode);
    pComponent->ReleaseRef();
    return true;
}

// 0x00397FA0 | fefates:bytes [tier B]
bool nn::nex::SystemComponentGroup::UnregisterAllComponents()
{
    while (m_Components.GetSize() != 0) {
        SystemComponent* pComponent = m_Components.front();
        qList<SystemComponent*>::Node* pNode = m_Components.GetFirst();
        while (!m_Components.IsEnd(pNode) && pNode->m_Value != pComponent) {
            pNode = pNode->m_pNext;
        }
        if (m_Components.IsEnd(pNode)) {
            continue;
        }
        m_Components.erase(pNode);
        pComponent->ReleaseRef();
    }
    return true;
}

// 0x00398058
void nn::nex::SystemComponentGroup::vf_0x10()
{
    // nothing to do in this build
}

// 0x0039805C | fefates:bytes
void nn::nex::SystemComponentGroup::DoWork()
{
    if (m_State == STATE_TERMINATING) {
        if (GetComponentsState() == STATE_TERMINATED || m_Components.GetSize() == 0) {
            TryTransition(STATE_TERMINATED);
            return;
        }
        for (qList<SystemComponent*>::Node* pNode = m_Components.GetFirst(); !m_Components.IsEnd(pNode); pNode = pNode->m_pNext) {
            if (pNode->m_Value->m_State != STATE_TERMINATED) {
                pNode->m_Value->Terminate();
            }
        }
    }
    if (m_State == STATE_INITIALIZING) {
        if (GetComponentsState() == STATE_READY || m_Components.GetSize() == 0) {
            TryTransition(STATE_READY);
            return;
        }
        for (qList<SystemComponent*>::Node* pNode = m_Components.GetFirst(); !m_Components.IsEnd(pNode); pNode = pNode->m_pNext) {
            if (pNode->m_Value->m_State != STATE_READY) {
                pNode->m_Value->Initialize();
            }
        }
    }
}

// 0x0039816C | fefates:bytes
nn::nex::SystemComponent::_State nn::nex::SystemComponentGroup::TestState()
{
    DoWork();
    return m_State;
}

// 0x0039818C | fefates:bytes [tier B]
nn::nex::SystemComponentGroup::SystemComponentGroup(const nn::nex::String& name) : SystemComponent(name)
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x00398290 | fefates:bytes
// 0x0039825C (deleting dtor)
nn::nex::SystemComponentGroup::~SystemComponentGroup()
{
    UnregisterAllComponents();
}

// 0x0072CC58 | fefates:bytes [tier B]
nn::nex::SystemComponent::_State nn::nex::SystemComponentGroup::GetComponentsState() const
{
    u32 states = 0;
    for (qList<SystemComponent*>::Node* pNode = m_Components.GetFirst(); !m_Components.IsEnd(pNode); pNode = pNode->m_pNext) {
        states |= pNode->m_Value->TestState();
    }
    if (m_UseCount != 0) {
        if ((states & STATE_FAULTY) || (states & STATE_UNKNOWN)) {
            return STATE_UNKNOWN;
        }
        if (states == STATE_CREATED || states == STATE_TERMINATED) {
            return STATE_UNKNOWN;
        }
        if ((states & ~(STATE_READY | STATE_IN_USE)) == 0) {
            return STATE_IN_USE;
        }
        if ((states & ~(STATE_CREATED | STATE_INITIALIZING | STATE_READY | STATE_IN_USE)) == 0) {
            return STATE_UNKNOWN;
        }
        if ((states & ~(STATE_READY | STATE_IN_USE | STATE_TERMINATING | STATE_TERMINATING_WHILE_IN_USE | STATE_TERMINATED)) == 0) {
            return STATE_TERMINATING_WHILE_IN_USE;
        }
        return STATE_UNKNOWN;
    }
    if (states & STATE_FAULTY) {
        return STATE_FAULTY;
    }
    if (states & STATE_UNKNOWN) {
        return STATE_UNKNOWN;
    }
    if (states == STATE_CREATED) {
        return STATE_CREATED;
    }
    if (states == STATE_TERMINATED) {
        return STATE_TERMINATED;
    }
    if ((states & ~(STATE_READY | STATE_IN_USE)) == 0) {
        return STATE_READY;
    }
    if ((states & ~(STATE_CREATED | STATE_INITIALIZING | STATE_READY | STATE_IN_USE)) == 0) {
        return STATE_INITIALIZING;
    }
    if ((states & ~(STATE_READY | STATE_IN_USE | STATE_TERMINATING | STATE_TERMINATING_WHILE_IN_USE | STATE_TERMINATED)) == 0) {
        return STATE_TERMINATING;
    }
    return STATE_UNKNOWN;
}

// 0x0072CD4C
const wchar_t* nn::nex::SystemComponentGroup::GetType() const
{
    return L"SystemComponentGroup";
}

// 0x0072CD80
bool nn::nex::SystemComponentGroup::IsAKindOf(const wchar_t* pType) const
{
    if (wcscmp(L"SystemComponentGroup", pType) == 0) {
        return true;
    }
    return wcscmp(L"SystemComponent", pType) == 0;
}

} // namespace nex
} // namespace nn
