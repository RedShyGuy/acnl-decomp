#include "ssys/ma/dFlushCache_SingletonDisposer_.h"

// 0x00567218 slot 0x00 (name after sead)
// 0x005671C4 (deleting dtor)
ssys::ma::FlushCache::SingletonDisposer_::~SingletonDisposer_()
{
    if (this != FlushCache::s_pSingletonDisposer) {
        return;
    }
    FlushCache::s_pSingletonDisposer = NULL;
    FlushCache::s_pInstance->~FlushCache();
    FlushCache::s_pInstance = NULL;
}
