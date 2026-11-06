#include "Sound/dSoundLivePlayer_SingletonDisposer_.h"

// 0x002A45C4
// 0x002A452C (deleting dtor)
SoundLivePlayer::SingletonDisposer_::~SingletonDisposer_()
{
    if (this == s_pSingletonDisposer) {
        s_pSingletonDisposer = NULL;
        s_pInstance->~SoundLivePlayer();
        s_pInstance = NULL;
    }
}
