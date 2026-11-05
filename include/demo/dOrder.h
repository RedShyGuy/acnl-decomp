#pragma once

#include "decomp.h"
#include "Other/dListNodePriority.h"
#include "demo/demo_Api.h"
#include "sead/seadSafeStringBase.h"

namespace demo {
// an order in the list of the orders (names are ours)
// RTTI N4demo5OrderE @ 0x008D10CC
// vtable 0x00904838 (vptr 0x00904840), offset_to_top 0, 5 entries
class Order : public ::ListNodePriority
{
public:
    DECOMP_EMIT_COPY Order() : ListNodePriority(), m_Priority(0xFF) {} // 0x0052DC24 (name after the RTTI)
    virtual ~Order(); // 0x0052DC58 slot 0x00 (name after the RTTI)
    // 0x0052DC54 slot 0x04 (deleting dtor)
    // a lower priority goes first, then the name
    bool IsBefore(const ::ssys::st::ListNode* other) const override; // 0x002BB2B8 slot 0x08
    // (empty for all orders)
    virtual const sead::SafeStringBase<char>& GetName() const; // 0x0071DEAC slot 0x10

    // makes the order free
    void Reset(); // 0x0052DBFC

    u8 m_Priority;     // 0x0C
    u32 m_PartnerId;   // 0x10 the other actor (DemoActor::GetId)
    u32 m_ActorId;     // 0x14 the actor of the order
    u8 m_State;        // 0x18 OrderState
    u8 m_Id;           // 0x19 OrderID
    u8 m_Result;       // 0x1A
    u8 m_Param;        // 0x1B
    u8 m_1C;           // 0x1C
};
ASSERT_SIZE(Order, 0x20);
} // namespace demo
