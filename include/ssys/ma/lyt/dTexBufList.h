#pragma once

#include "decomp.h"
#include "ssys/st/dList.h"
#include "ssys/ma/lyt/dTexBufListNode.h"
#include "nw/lyt/lyt_Layout.h"

namespace ssys {
namespace ma {
namespace lyt {
// the VRAM images of an ArcResourceAccessorVRAM (the nodes are freed with the layout allocator; function name is ours)
// RTTI N4ssys2ma3lyt10TexBufListE @ 0x008D244C
// vtable 0x00907270 (vptr 0x00907278), offset_to_top 0, 2 entries
class TexBufList : public ::ssys::st::List
{
public:
    TexBufList() {} // (inline)
    virtual ~TexBufList(); // 0x0056860C slot 0x00

    // (inline)
    void DeleteAll()
    {
        while (m_pFirst != NULL) {
            TexBufListNode* node = static_cast<TexBufListNode*>(m_pFirst);
            Erase(node);
            if (node != NULL) {
                node->~TexBufListNode();
                nw::lyt::Layout::FreeMemory(node);
            }
        }
    }
};
ASSERT_SIZE(TexBufList, 0xC);
} // namespace lyt
} // namespace ma
} // namespace ssys
