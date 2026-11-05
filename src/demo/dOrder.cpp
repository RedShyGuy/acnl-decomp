#include "demo/dOrder.h"
#include "sead/seadFixedSafeString.h"

namespace demo {
// 0x0052DBFC (name is ours)
void demo::Order::Reset()
{
    m_PartnerId = 0;
    m_ActorId = 0;
    m_State = ORDER_STATE_FREE;
    m_Id = REVOKED_PLAYER_CONTROL;
    m_Result = 0;
    m_Param = 0;
    m_1C = 0;
}

// 0x0052DC58 slot 0x00 (name after the RTTI)
// 0x0052DC54 (deleting dtor)
demo::Order::~Order()
{
    // empty (in the original too)
}

// 0x002BB2B8 slot 0x08 (name is ours)
bool demo::Order::IsBefore(const ::ssys::st::ListNode* other) const
{
    const Order* order = static_cast<const Order*>(other);
    if (m_Priority < order->m_Priority) {
        return true;
    }
    if (m_Priority > order->m_Priority) {
        return false;
    }
    return GetName().compare(order->GetName()) < 0;
}

// 0x0071DEAC slot 0x10 (name is ours)
const sead::SafeStringBase<char>& demo::Order::GetName() const
{
    // 0x00A1961C (guard 0x00954354)
    static sead::FixedSafeString<48> s_Name;
    s_Name.clear();
    return s_Name;
}
} // namespace demo
