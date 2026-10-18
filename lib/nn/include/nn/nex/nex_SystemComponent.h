#pragma once

#include "decomp.h"
#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
class SystemComponentGroup;

// A part of nex with a state machine (initialize, use, terminate). The names of the states, of the
// members and of the functions without a reference name are ours.
// RTTI N2nn3nex15SystemComponentE @ 0x008CE520
// vtable 0x008FD004 (vptr 0x008FD00C), offset_to_top 0, 16 entries
class SystemComponent : public ::nn::nex::RefCountedObject
{
public:
    class Use;

    enum _State : u16
    {
        STATE_CREATED = 1 << 0,
        STATE_INITIALIZING = 1 << 1,
        STATE_READY = 1 << 2,
        STATE_IN_USE = 1 << 3,
        STATE_TERMINATING = 1 << 4,
        STATE_TERMINATING_WHILE_IN_USE = 1 << 5,
        STATE_TERMINATED = 1 << 6,
        STATE_FAULTY = 1 << 7,
        STATE_UNKNOWN = 1 << 8
    };

    explicit SystemComponent(const nn::nex::String& name); // 0x0037E3C8 | fefates:bytes [tier B]
    SystemComponent(); // TODO: default ctor added so derived stubs compile - may not exist
    virtual ~SystemComponent(); // 0x0037E47C slot 0x00 | fefates:callgraph
    // 0x0037E420 slot 0x04 (deleting dtor)
    // the name of the class (inline in the original, with a copy out of line)
    virtual const wchar_t* GetType() const; // 0x0072B6EC slot 0x08
    // whether the class is (derived from) the named one (inline, with a copy out of line)
    virtual bool IsAKindOf(const wchar_t* pType) const; // 0x0072B714 slot 0x0C
    virtual void vf_0x10() = 0; // slot 0x10
    virtual void StateTransition(_State state); // 0x0037DF18 slot 0x14 (slot name from the reference symbols)
    virtual void OnInitialize(); // 0x0037DEEC slot 0x18 (slot name from the reference symbols)
    virtual void OnTerminate(); // 0x0037DEE0 slot 0x1C (slot name from the reference symbols)
    virtual bool BeginInitialization(); // 0x0037E018 slot 0x20 (slot name from the reference symbols)
    virtual bool EndInitialization(); // 0x0037E010 slot 0x24 (slot name from the reference symbols)
    virtual bool BeginTermination(); // 0x0037E008 slot 0x28 (slot name from the reference symbols)
    virtual bool EndTermination(); // 0x0037DF10 slot 0x2C (slot name from the reference symbols)
    virtual bool ValidTransition(_State state); // 0x0037DF1C slot 0x30 | fefates:bytes
    virtual bool UseIsAllowed(); // 0x0037DEF8 slot 0x34 | fefates:bytes
    virtual _State TestState(); // 0x0037E3C0 slot 0x38 (slot name from the reference symbols)
    virtual void DoWork(); // 0x0037E204 slot 0x3C (slot name from the reference symbols)

    _State Initialize(); // 0x0037DDCC | fefates:bytes [tier B]
    // terminates and waits (with the scheduler) until the component is terminated
    nn::nex::qResult WaitForTerminatedState(unsigned int timeout); // 0x0037E020 | fefates:bytes-fuzzy [tier B]
    _State Terminate(); // 0x0037E210 | fefates:bytes [tier B]
    // WaitForTerminatedState for the scheduler (armlink placed it far from the others)
    static bool WaitForTerminatedStateCallback(nn::nex::RootObject* pObject, unsigned int timeout); // 0x00186BD0 (name is ours)
    void SetName(const nn::nex::String& name); // 0x0037E208 | fefates:callgraph [tier C]

    // (inline) changes to state if the transition is valid
    void TryTransition(_State state)
    {
        if (ValidTransition(state)) {
            StateTransition(state);
            m_State = state;
        }
    }

    String m_Name;                   // 0x0C
    _State m_State;                  // 0x14
    u32 m_UseCount;                  // 0x18
    SystemComponentGroup* m_pGroup;  // 0x1C
};
ASSERT_SIZE(SystemComponent, 0x20);

// A use of a SystemComponent (counted while it exists). The member names are ours.
// RTTI N2nn3nex15SystemComponent3UseE @ 0x008CE514
// vtable 0x008FCFF4 (vptr 0x008FCFFC), offset_to_top 0, 2 entries
class SystemComponent::Use : public ::nn::nex::RootObject
{
public:
    // (inline) the component gets in use if it allows it (m_Unknown0x08 is not set)
    explicit Use(SystemComponent* pComponent) : m_pComponent(pComponent)
    {
        if (pComponent->UseIsAllowed()) {
            if (pComponent->m_State == STATE_READY) {
                pComponent->TryTransition(STATE_IN_USE);
            }
            pComponent->m_UseCount++;
            m_IsValid = true;
        } else {
            m_IsValid = false;
        }
    }
    virtual ~Use(); // 0x0037E16C slot 0x00 | mk7dlp:bytes-fuzzy
    // 0x0037E13C slot 0x04 (deleting dtor)

    SystemComponent* m_pComponent; // 0x04
    u32 m_Unknown0x08;             // 0x08
    bool m_IsValid;                // 0x0C
};
} // namespace nex
} // namespace nn
