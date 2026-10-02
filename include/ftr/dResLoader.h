#pragma once

#include "decomp.h"
#include "g3d/dResourceLoader.h"

namespace ftr {
// RTTI N3ftr9ResLoaderE @ 0x008D0C70
// vtable 0x00903934 (vptr 0x0090393C), offset_to_top 0, 2 entries
class ResLoader : public ::g3d::ResourceLoader
{
public:
    ResLoader(); // ctor candidate(s) 0x004EAB00 (unverified)
    virtual void vf_0x00(); // 0x00318010 slot 0x00 | virtual slot, introduced by g3d::ResourceSet
    virtual void vf_0x04(); // 0x004EAB18 slot 0x04 | virtual slot, introduced by g3d::ResourceSet
};
} // namespace ftr
