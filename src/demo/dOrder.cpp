#include "Other/dListNodePriority.h"
#include "demo/dOrder.h"

namespace demo {
// ctor candidate(s) 0x0052DC28 (unverified)
demo::Order::Order()
{
}

// 0x0052DC58 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
demo::Order::~Order()
{
}

// 0x002BB2B8 slot 0x08 | virtual slot, introduced by demo::Order
bool demo::Order::IsBefore(const ::ssys::st::ListNode*) const
{
}

// 0x0071DEAC slot 0x10 | virtual slot, introduced by demo::Order
void demo::Order::vf_0x10()
{
}

} // namespace demo
