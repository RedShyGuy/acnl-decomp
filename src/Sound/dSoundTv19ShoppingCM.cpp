#include "Sound/dSoundTv19ShoppingCM.h"
#include "Sound/dSoundLabel.h"

// 0x002FFF64
// 0x002FFF54 (deleting dtor)
SoundTv19ShoppingCM::~SoundTv19ShoppingCM()
{
    // (only the base is destroyed)
}

// 0x002FF22C
void SoundTv19ShoppingCM::Start()
{
    m_ItemId = GROUP_TV_19;
    SoundTvBase::Start();
    m_Volume = 1.0f;
    m_Scene = -1;
}

// 0x002FFEBC
void SoundTv19ShoppingCM::SetScene(s8 scene, s8 variant)
{
    if (m_Scene == scene) {
        return;
    }
    PlaySound(1, SE_TV_19);
    if (GetHandle(1).m_pSound != NULL) {
        WriteVariable(1, 0, scene);
        WriteVariable(1, 1, variant);
    }
    m_Scene = scene;
}

// 0x002FFA60
void SoundTv19ShoppingCM::UpdateProgram(s32 frame)
{
    if (static_cast<u32>(frame) < 217) {
        if (GetHandle(0).m_pSound == NULL) {
            PlaySound(0, SE_TV_19_BGM_1);
            m_Obj.SetVolume(0, 1.0f, 0);
            m_Volume = 1.0f;
        }
        if (static_cast<u32>(frame - 55) < 162 && m_Volume != 0.5f) {
            m_Obj.SetVolume(0, 0.5f, 10);
            m_Volume = 0.5f;
        }
    } else if (static_cast<u32>(frame - 217) < 118) {
        if (GetHandle(0).m_pSound != NULL) {
            GetHandle(0).Stop(120);
            m_Volume = 0.0f;
        }
    } else if (static_cast<u32>(frame - 335) < 135) {
        if (GetHandle(0).m_pSound == NULL) {
            PlaySound(0, SE_TV_19_ANMA);
        }
    } else if (static_cast<u32>(frame - 470) < 100) {
        if (GetHandle(0).m_pSound != NULL) {
            GetHandle(0).Stop(15);
            m_Volume = 0.0f;
        }
    } else if (static_cast<u32>(frame - 570) < 330) {
        if (GetHandle(0).m_pSound == NULL) {
            PlaySound(0, SE_TV_19_BGM_2);
            m_Obj.SetVolume(0, 0.0f, 0);
            m_Obj.SetVolume(0, 0.5f, 60);
            m_Volume = 0.5f;
        }
        if (frame > 800 && m_Volume != 1.0f) {
            m_Obj.SetVolume(0, 1.0f, 15);
            m_Volume = 1.0f;
        }
    }
    switch (frame - 60) {
        case 0:
            PlaySound(1, SE_TV_19_TITLE);
            m_Scene = 0;
            break;
        case 45:
            SetScene(1, 0);
            break;
        case 75:
            SetScene(2, 0);
            break;
        case 112:
            SetScene(3, 0);
            break;
        case 157:
            SetScene(4, 0);
            break;
        case 206:
            SetScene(5, 0);
            break;
        case 243:
            SetScene(6, 0);
            break;
        case 281:
            SetScene(7, 0);
            break;
        case 291:
            SetScene(8, 0);
            break;
        case 322:
            SetScene(9, 0);
            break;
        case 341:
            SetScene(10, 0);
            break;
        case 390:
            SetScene(11, 0);
            break;
        case 435:
            SetScene(12, 0);
            break;
        case 472:
            SetScene(13, 0);
            break;
        case 540:
            SetScene(14, 0);
            break;
        case 597:
            SetScene(15, 0);
            break;
        case 652:
            SetScene(16, 0);
            break;
        case 679:
            SetScene(17, 0);
            break;
        case 743:
            SetScene(18, 0);
            break;
    }
}
