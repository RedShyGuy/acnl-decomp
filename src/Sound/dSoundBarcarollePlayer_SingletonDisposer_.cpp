#include "Sound/dSoundBarcarollePlayer_SingletonDisposer_.h"

// 0x0032B704
// 0x0032B698 (deleting dtor)
SoundBarcarollePlayer::SingletonDisposer_::~SingletonDisposer_()
{
    if (this == s_pSingletonDisposer) {
        s_pSingletonDisposer = NULL;
        s_pInstance->~SoundBarcarollePlayer();
        s_pInstance = NULL;
    }
}
