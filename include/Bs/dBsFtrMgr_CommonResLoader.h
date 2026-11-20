#pragma once

#include "decomp.h"
#include "Bs/dBsFtrMgr.h"
#include "g3d/dResourceLoader.h"

// the loader of the common resources of the furniture ("Ftr/Common/FtrCommon.bcres"; one global object)
// RTTI N8BsFtrMgr15CommonResLoaderE @ 0x008D3FC4
// vtable 0x0090BF58 (vptr 0x0090BF60), offset_to_top 0, 2 entries
class BsFtrMgr::CommonResLoader : public ::g3d::ResourceLoader
{
public:
    // (inline: the static initializer 0x0078656C builds the global CommonResLoader 0x0098BF78)
    CommonResLoader() {}
    virtual ~CommonResLoader(); // 0x00692290 slot 0x00
    // 0x00692280 (deleting dtor)
};
