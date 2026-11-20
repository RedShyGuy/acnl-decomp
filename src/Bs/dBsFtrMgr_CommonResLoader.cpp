#include "g3d/dResourceLoader.h"
#include "Bs/dBsFtrMgr_CommonResLoader.h"

// 0x00692290 slot 0x00
// 0x00692280 (deleting dtor)
BsFtrMgr::CommonResLoader::~CommonResLoader()
{
    // (only the base is destroyed)
}

