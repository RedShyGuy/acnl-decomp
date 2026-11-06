#include "Sound/dSoundMuInfo_SingletonDisposer_.h"

// 0x001DA100
// 0x001DA0BC (deleting dtor)
SoundMuInfo::SingletonDisposer_::~SingletonDisposer_()
{
    if (this == s_pSingletonDisposer) {
        s_pSingletonDisposer = NULL;
        s_pInstance = NULL;
    }
}
