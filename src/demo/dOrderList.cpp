#include "demo/dOrderList.h"

namespace demo {
// the orders (with the state of demo_Api.cpp in an initializer that also builds globals of other files)
// 0x009AC170
OrderList s_OrderList;

// 0x0052C984 (name is ours)
demo::Order* demo::OrderList::Alloc()
{
    for (s32 i = 0; i < ORDER_NUM; i++) {
        if (m_Orders[i].m_State == ORDER_STATE_FREE) {
            Order* order = &m_Orders[i];
            order->m_PartnerId = 0;
            order->m_ActorId = 0;
            order->m_Id = REVOKED_PLAYER_CONTROL;
            order->m_Result = 0;
            order->m_Param = 0;
            order->m_1C = 0;
            return order;
        }
    }
    return NULL;
}

// 0x0052C9D8 (name is ours)
void demo::OrderList::Reset()
{
    ssys::st::List::Clear();
    for (s32 i = 0; i < ORDER_NUM; i++) {
        m_Orders[i].Reset();
    }
}

// 0x0052CA20 (name is ours)
void demo::OrderList::Clear()
{
    for (Order* order = static_cast<Order*>(m_pFirst); order != NULL; order = static_cast<Order*>(order->m_pNext)) {
        order->Reset();
    }
    ssys::st::List::Clear();
}

// 0x0052E1B4 slot 0x00 (name after the RTTI)
// 0x0052E1B0 (deleting dtor)
// 0x0052CA64 (copy)
demo::OrderList::~OrderList()
{
    // (only the members are destroyed)
}
} // namespace demo
