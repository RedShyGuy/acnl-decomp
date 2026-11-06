#include "Sound/dSoundRiverMgr_SingletonDisposer_.h"

// 0x00247364
// 0x00247310 (deleting dtor)
SoundRiverMgr::SingletonDisposer_::~SingletonDisposer_()
{
    if (this == s_pSingletonDisposer) {
        s_pSingletonDisposer = NULL;
        s_pInstance->~SoundRiverMgr();
        s_pInstance = NULL;
    }
}
