#include "Sound/dSoundSeaMgr_SingletonDisposer_.h"

// 0x001DA8D0
// 0x001DA86C (deleting dtor)
SoundSeaMgr::SingletonDisposer_::~SingletonDisposer_()
{
    if (this == s_pSingletonDisposer) {
        s_pSingletonDisposer = NULL;
        s_pInstance->~SoundSeaMgr();
        s_pInstance = NULL;
    }
}
