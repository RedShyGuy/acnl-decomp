#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_Pane.h"

namespace nw {
namespace lyt {
namespace res {
struct Bounding;
} // namespace res

// RTTI N2nw3lyt8BoundingE @ 0x008D08B4
// vtable 0x00902E20 (vptr 0x00902E28), offset_to_top 0, 28 entries
//
// A pane that is not drawn (an area, e.g. for touch tests).
class Bounding : public ::nw::lyt::Pane
{
public:
    Bounding(const res::Bounding* pBlock, const ResBlockSet& resBlockSet); // 0x004BC8D0 | nintendogs:callseq-callee [tier A]
    virtual ~Bounding(); // 0x004B7FC0 slot 0x00 (name is ours)
    // 0x004BC8E8 (deleting dtor)
    virtual const PaneTypeInfo* GetRuntimeTypeInfo() const; // 0x0073E84C slot 0x08 (name is ours)
    virtual void DrawSelf(DrawInfo& drawInfo); // 0x004BC8CC slot 0x64 (name is ours)

    static const PaneTypeInfo s_TypeInfo; // (name is ours)
};
ASSERT_SIZE(Bounding, 0xD4);
} // namespace lyt
} // namespace nw
