#include "Sound/dSoundDataLoadMgr_SingletonDisposer_.h"

// 0x002BD010
// 0x002BCFAC (deleting dtor)
SoundDataLoadMgr::SingletonDisposer_::~SingletonDisposer_()
{
    if (this == s_pSingletonDisposer) {
        s_pSingletonDisposer = NULL;
        s_pInstance->~SoundDataLoadMgr();
        s_pInstance = NULL;
    }
}
