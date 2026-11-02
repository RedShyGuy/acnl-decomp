#include "ssys/ma/dVramMemBlock.h"

namespace ssys {
namespace ma {
// 0x00567D7C slot 0x08
bool ssys::ma::VramMemBlock::IsBefore(const ::ssys::st::ListNode* other) const
{
    return m_Address < static_cast<const VramMemBlock*>(other)->m_Address;
}

// the first block from start on behind this one
// 0x00567D94 slot 0x0C
::ssys::st::ListNode* ssys::ma::VramMemBlock::FindInsertPosition(::ssys::st::ListNode* start) const
{
    ::ssys::st::ListNode* node = start;
    while (node != NULL) {
        if (static_cast<VramMemBlock*>(node)->m_Address > m_Address) {
            break;
        }
        node = node->m_pNext;
    }
    return node;
}

// 0x00567DC4 slot 0x00
// 0x00567DC0 (deleting dtor)
ssys::ma::VramMemBlock::~VramMemBlock()
{
    // empty (in the original too)
}

} // namespace ma
} // namespace ssys
