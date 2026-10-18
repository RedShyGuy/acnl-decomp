#pragma once

#include "decomp.h"
#include "nn/nex/nex_StateMachine.h"
#include "nn/nex/nex_StateMachine_QEvent.h"

namespace nn {
namespace nex {
// An operation of the back end (a protocol call, a login, a logout) with its type number and name.
// The names of the members and of the slots are ours.
// RTTI N2nn3nex9OperationE @ 0x008CF774
// vtable 0x008FFD5C (vptr 0x008FFD64), offset_to_top 0, 6 entries
class Operation : public ::nn::nex::StateMachine::QEvent
{
public:
    explicit Operation(unsigned int id); // 0x003D753C
    virtual ~Operation(); // 0x003D7570 slot 0x00
    // 0x003D7560 (deleting dtor)
    virtual u32 GetType() const; // 0x0072EA80 slot 0x08
    virtual u32 GetTypeImpl() const = 0; // slot 0x0C
    virtual const wchar_t* GetName() const = 0; // slot 0x10
    virtual void vf_0x14() = 0; // slot 0x14

    bool m_Unknown0x04;  // 0x04
    bool m_Unknown0x05;  // 0x05
    u32 m_Unknown0x08;   // 0x08
    u32 m_Id;            // 0x0C (the constructor argument)
};
ASSERT_SIZE(Operation, 0x10);
} // namespace nex
} // namespace nn
