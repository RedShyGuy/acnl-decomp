#include "Sound/dSoundDJPlayer_SingletonDisposer_.h"

// 0x0024565C
// 0x00245604 (deleting dtor)
SoundDJPlayer::SingletonDisposer_::~SingletonDisposer_()
{
    if (this == s_pSingletonDisposer) {
        s_pSingletonDisposer = NULL;
        s_pInstance->~SoundDJPlayer();
        s_pInstance = NULL;
    }
}
