#include "Sound/dSoundMicMgr_SingletonDisposer_.h"

// 0x001D9CB8
// 0x001D9C68 (deleting dtor)
SoundMicMgr::SingletonDisposer_::~SingletonDisposer_()
{
    if (this == s_pSingletonDisposer) {
        s_pSingletonDisposer = NULL;
        s_pInstance->~SoundMicMgr();
        s_pInstance = NULL;
    }
}
