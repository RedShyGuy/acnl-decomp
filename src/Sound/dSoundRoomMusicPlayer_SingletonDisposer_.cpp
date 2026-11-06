#include "Sound/dSoundRoomMusicPlayer_SingletonDisposer_.h"

// 0x00322BB8
// 0x00322AE0 (deleting dtor)
SoundRoomMusicPlayer::SingletonDisposer_::~SingletonDisposer_()
{
    if (this == s_pSingletonDisposer) {
        s_pSingletonDisposer = NULL;
        s_pInstance->~SoundRoomMusicPlayer();
        s_pInstance = NULL;
    }
}
