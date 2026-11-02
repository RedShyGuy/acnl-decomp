#pragma once

#include "decomp.h"
#include "ssys/st/dListNode.h"

namespace ssys {
namespace ma {
namespace lyt {
// a texture image an ArcResourceAccessorVRAM copied to VRAM (member name is ours)
// RTTI N4ssys2ma3lyt14TexBufListNodeE @ 0x008D2464
// vtable 0x009072CC (vptr 0x009072D4), offset_to_top 0, 2 entries
class TexBufListNode : public ::ssys::st::ListNode
{
public:
    // (inline in ArcResourceAccessorVRAM::LoadTextureImage)
    explicit TexBufListNode(void* address) : m_pAddress(address) {}
    virtual ~TexBufListNode(); // 0x005687AC slot 0x00

    void* m_pAddress; // 0xC
};
ASSERT_SIZE(TexBufListNode, 0x10);
} // namespace lyt
} // namespace ma
} // namespace ssys
