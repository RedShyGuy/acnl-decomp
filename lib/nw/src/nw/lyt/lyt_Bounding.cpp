#include "nw/lyt/lyt_Bounding.h"

#include "nw/lyt/lyt_ResourceFormat.h"

namespace nw {
namespace lyt {
// 0x00976154 (name is ours)
const PaneTypeInfo nw::lyt::Bounding::s_TypeInfo(&Pane::s_TypeInfo);

// sets s_TypeInfo
// 0x007967B8 (static initializer)

// 0x004BC8D0 | nintendogs:callseq-callee [tier A]
nw::lyt::Bounding::Bounding(const res::Bounding* pBlock, const ResBlockSet& /*resBlockSet*/) : Pane(pBlock)
{
}

// 0x004B7FC0 slot 0x00 (name is ours)
// 0x004BC8E8 (deleting dtor)
nw::lyt::Bounding::~Bounding()
{
    // empty (in the original too: only the Pane destructor)
}

// 0x0073E84C slot 0x08 (name is ours)
const PaneTypeInfo* nw::lyt::Bounding::GetRuntimeTypeInfo() const
{
    return &s_TypeInfo;
}

// 0x004BC8CC slot 0x64 (name is ours)
void nw::lyt::Bounding::DrawSelf(DrawInfo& /*drawInfo*/)
{
    // empty (in the original too: a bounding pane is not drawn)
}

} // namespace lyt
} // namespace nw
