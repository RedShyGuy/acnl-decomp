#include "nn/pia/inet/inet_NatTraversalTimeList.h"

namespace nn {
namespace pia {
namespace inet {
// 0x003FFE2C | fefates:bytes-fuzzy [tier B]
void nn::pia::inet::NatTraversalTimeList::Add(const nn::pia::inet::NatTraversalTime& time)
{
    NatTraversalTime* pTime = Find(time.m_Location);
    if (pTime == nullptr) {
        pTime = PushBackNew();
        if (pTime == nullptr) {
            return;
        }
    }
    time.Trace(0x8000);
    *pTime = time;
}

// 0x003FFF40 | fefates:bytes [tier B]
nn::pia::inet::NatTraversalTime* nn::pia::inet::NatTraversalTimeList::Find(const nn::pia::transport::StationLocation& location)
{
    for (Node* node = Begin(); node != End(); node = Advance(node)) {
        if (IsSameLocation(node->m_Value, location)) {
            return &node->m_Value;
        }
    }
    return nullptr;
}

// 0x003FFFB0 | fefates:bytes [tier B]
void nn::pia::inet::NatTraversalTimeList::Remove(const nn::pia::transport::StationLocation& location)
{
    for (Node* node = Begin(); node != End();) {
        NatTraversalTime* pTime = &node->m_Value;
        if (IsSameLocation(*pTime, location)) {
            pTime->Trace(0x8000);
            node = Advance(node);
            pTime->~NatTraversalTime();
            Erase(pTime);
        } else {
            node = Advance(node);
        }
    }
}

// 0x004000D4
// 0x00400064 (deleting dtor)
nn::pia::inet::NatTraversalTimeList::~NatTraversalTimeList()
{
    ClearNodes();
}

} // namespace inet
} // namespace pia
} // namespace nn
