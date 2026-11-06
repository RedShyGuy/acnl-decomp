#include "Sound/dSoundKKInfo_SingletonDisposer_.h"

// 0x001D9AD4
// 0x001D9A90 (deleting dtor)
SoundKKInfo::SingletonDisposer_::~SingletonDisposer_()
{
    if (this == s_pSingletonDisposer) {
        s_pSingletonDisposer = NULL;
        s_pInstance = NULL;
    }
}
