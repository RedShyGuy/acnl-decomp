#pragma once

#include "decomp.h"
#include "demo/dOrder.h"
#include "ssys/st/dListPriority.h"

namespace demo {
// the queued orders and the pool of all orders (names are ours)
// RTTI N4demo9OrderListE @ 0x008D10D8
// vtable 0x00904854 (vptr 0x0090485C), offset_to_top 0, 2 entries
class OrderList : public ::ssys::st::ListPriority
{
public:
    static const s32 ORDER_NUM = 32;

    OrderList() {}
    virtual ~OrderList(); // 0x0052E1B4 slot 0x00 (name after the RTTI)
    // 0x0052E1B0 slot 0x04 (deleting dtor)
    // 0x0052CA64 (copy)

    // a free order (NULL: none)
    Order* Alloc(); // 0x0052C984
    // empties the list and frees all orders
    void Reset(); // 0x0052C9D8
    // frees the queued orders and empties the list
    void Clear(); // 0x0052CA20

    Order m_Orders[ORDER_NUM]; // 0x0C
};
ASSERT_SIZE(OrderList, 0x40C);

extern OrderList s_OrderList;
} // namespace demo
