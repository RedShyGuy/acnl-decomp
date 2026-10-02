#pragma once

#include "decomp.h"
#include "Bs/dBsFtrMgr.h"
#include "g3d/dResourceLoader.h"

// RTTI N8BsFtrMgr15CommonResLoaderE @ 0x008D3FC4
// vtable 0x0090BF58 (vptr 0x0090BF60), offset_to_top 0, 2 entries
class BsFtrMgr::CommonResLoader : public ::g3d::ResourceLoader
{
public:
    CommonResLoader(); // ctor candidate(s) 0x0078656C (unverified)
    virtual void vf_0x00(); // 0x00692290 slot 0x00 | virtual slot, introduced by g3d::ResourceSet
    virtual void vf_0x04(); // 0x00692280 slot 0x04 | virtual slot, introduced by g3d::ResourceSet
};
