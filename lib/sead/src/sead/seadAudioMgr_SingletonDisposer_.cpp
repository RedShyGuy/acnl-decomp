#include "sead/seadAudioMgr_SingletonDisposer_.h"

// 0x00560A98 slot 0x00
// 0x00560A40 (deleting dtor)
sead::AudioMgr::SingletonDisposer_::~SingletonDisposer_()
{
    if (this != AudioMgr::s_pSingletonDisposer) {
        return;
    }
    AudioMgr::s_pSingletonDisposer = nullptr;
    AudioMgr::s_pInstance->~AudioMgr();
    AudioMgr::s_pInstance = nullptr;
}
