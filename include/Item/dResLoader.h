#pragma once

#include "decomp.h"
#include "g3d/dResourceLoader.h"

namespace item {
// RTTI N4item9ResLoaderE @ 0x008D1128
// vtable 0x009049A0 (vptr 0x009049A8), offset_to_top 0, 2 entries
class ResLoader : public ::g3d::ResourceLoader
{
public:
    ResLoader(); // ctor candidate(s) 0x0053788C (unverified)
    virtual void vf_0x00(); // 0x005378C8 slot 0x00 | virtual slot, introduced by g3d::ResourceSet
    virtual void vf_0x04(); // 0x005378B4 slot 0x04 | virtual slot, introduced by g3d::ResourceSet
};
} // namespace item
