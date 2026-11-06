#include "Sound/dSoundHaniwaMgr_SingletonDisposer_.h"

// 0x00276B3C
// 0x00276AF8 (deleting dtor)
SoundHaniwaMgr::SingletonDisposer_::~SingletonDisposer_()
{
    if (this == s_pSingletonDisposer) {
        s_pSingletonDisposer = NULL;
        s_pInstance = NULL;
    }
}
