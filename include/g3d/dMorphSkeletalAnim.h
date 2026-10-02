#pragma once

#include "decomp.h"
#include "g3d/dSkeletalAnim.h"

namespace g3d {
// RTTI N3g3d17MorphSkeletalAnimE @ 0x008D0D0C
// vtable 0x00903A98 (vptr 0x00903AA0), offset_to_top 0, 8 entries
class MorphSkeletalAnim : public ::g3d::SkeletalAnim
{
public:
    MorphSkeletalAnim(); // ctor candidate(s) 0x004F003C (unverified)
    virtual void vf_0x00(); // 0x004F0094 slot 0x00 | virtual slot, introduced by g3d::BaseAnim
    virtual void vf_0x04(); // 0x004F0074 slot 0x04 | virtual slot, introduced by g3d::BaseAnim
    virtual void vf_0x10(); // 0x004F002C slot 0x10 | virtual slot, introduced by g3d::BaseAnim
    virtual void vf_0x18(); // 0x004EFF40 slot 0x18 | virtual slot, introduced by g3d::BaseAnim
};
} // namespace g3d
