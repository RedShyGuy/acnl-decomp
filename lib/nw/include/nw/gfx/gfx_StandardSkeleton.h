#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_CalculatedTransform.h"
#include "nw/gfx/gfx_Skeleton.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx16StandardSkeletonE @ 0x008D0688
// vtable 0x009026BC (vptr 0x009026C4), offset_to_top 0, 13 entries
class StandardSkeleton : public ::nw::gfx::Skeleton
{
public:
    StandardSkeleton(); // ctor candidate(s) 0x0049C520 (unverified)
    virtual ~StandardSkeleton(); // 0x0049CCA0 slot 0x00 | slot vf_0x00 of nw::gfx::Skeleton
    virtual void vf_0x04(); // 0x0049CBA4 slot 0x04 | virtual slot, introduced by nw::gfx::Skeleton
    virtual void vf_0x08(); // 0x0073A840 slot 0x08 | virtual slot, introduced by nw::gfx::Skeleton
    virtual void vf_0x0C(); // 0x0049C510 slot 0x0C | virtual slot, introduced by nw::gfx::Skeleton
    virtual void vf_0x10(); // 0x0073A84C slot 0x10 | virtual slot, introduced by nw::gfx::Skeleton
    virtual void vf_0x14(); // 0x0049C518 slot 0x14 | virtual slot, introduced by nw::gfx::Skeleton
    virtual void vf_0x18(); // 0x0073A854 slot 0x18 | virtual slot, introduced by nw::gfx::Skeleton
    virtual void vf_0x1C(); // 0x0049C4F8 slot 0x1C | virtual slot, introduced by nw::gfx::Skeleton
    virtual void vf_0x20(); // 0x0073A828 slot 0x20 | virtual slot, introduced by nw::gfx::Skeleton
    virtual void vf_0x24(); // 0x0049C508 slot 0x24 | virtual slot, introduced by nw::gfx::Skeleton
    virtual void vf_0x28(); // 0x0073A838 slot 0x28 | virtual slot, introduced by nw::gfx::Skeleton
    virtual void vf_0x2C(); // 0x0049C500 slot 0x2C | virtual slot, introduced by nw::gfx::Skeleton
    virtual void vf_0x30(); // 0x0073A830 slot 0x30 | virtual slot, introduced by nw::gfx::Skeleton
    void Create(nw::gfx::res::ResSkeleton, int, bool, nw::ut::MoveArray<nw::gfx::CalculatedTransform>, nw::os::IAllocator*); // 0x0049C520 | nintendogs:callseq [tier A]
};
} // namespace gfx
} // namespace nw
