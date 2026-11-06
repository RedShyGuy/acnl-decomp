#include "Sound/dSoundMgr_SingletonDisposer_.h"

// 0x006B3C0C
// 0x006B3BBC (deleting dtor)
SoundMgr::SingletonDisposer_::~SingletonDisposer_()
{
    if (this == s_pSingletonDisposer) {
        s_pSingletonDisposer = NULL;
        s_pInstance->~SoundMgr();
        s_pInstance = NULL;
    }
}
