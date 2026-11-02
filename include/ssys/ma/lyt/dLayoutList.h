#pragma once

#include "decomp.h"
#include "ssys/st/dList.h"

namespace ssys {
namespace ma {
namespace lyt {
class Base2D;

// a draw list of the LayoutMgr: Base2Ds sorted by their layer (function names are ours)
// RTTI N4ssys2ma3lyt10LayoutListE @ 0x008D2440
// vtable 0x0090725C (vptr 0x00907264), offset_to_top 0, 3 entries
class LayoutList : public ::ssys::st::List
{
public:
    // (inline)
    LayoutList() {}
    virtual ~LayoutList(); // 0x0056859C slot 0x00
    // 0x00568598 (deleting dtor)
    // DrawSave of all nodes
    virtual void DrawSave(); // 0x00568568 slot 0x08

    // sets the draw mode of the nodes (they are Layouts) and draws them
    void Draw(u32 drawMode); // 0x005684F8
    // inserts in front of the first node with a larger layer
    void Add(Base2D* node); // 0x00568530
};
ASSERT_SIZE(LayoutList, 0xC);
} // namespace lyt
} // namespace ma
} // namespace ssys
