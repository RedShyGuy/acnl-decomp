#include "Sound/dSoundTv13Cooking.h"
#include "Sound/dSoundLabel.h"

// 0x002BE804
// 0x002BE7F4 (deleting dtor)
SoundTv13Cooking::~SoundTv13Cooking()
{
    // (only the base is destroyed)
}

// 0x002BDB7C
void SoundTv13Cooking::Start()
{
    m_ItemId = GROUP_TV_13;
    SoundTvBase::Start();
    m_Volume = 1.0f;
    m_Scene = -1;
}

// 0x002BE75C
void SoundTv13Cooking::SetScene(s8 scene, s8 variant)
{
    if (m_Scene == scene) {
        return;
    }
    PlaySound(1, SE_TV_13);
    if (GetHandle(1).m_pSound != NULL) {
        WriteVariable(1, 0, scene);
        WriteVariable(1, 1, variant);
    }
    m_Scene = scene;
}

inline void SoundTv13Cooking::PlayEffect(u32 soundId)
{
    PlaySound(0, soundId);
    m_Obj.SetVolume(0, 1.0f, 0);
}

// 0x002BDBFC
void SoundTv13Cooking::UpdateProgram(s32 frame)
{
    if (static_cast<u32>(frame) < 360) {
        if (GetHandle(0).GetId() != SE_TV_13_BGM) {
            PlaySound(0, SE_TV_13_BGM);
            m_Volume = 1.0f;
            m_Obj.SetVolume(0, 1.0f, 0);
        }
    } else if (static_cast<u32>(frame - 360) < 210) {
        if (GetHandle(0).GetId() != SE_TV_13_BGM) {
            PlaySound(0, SE_TV_13_BGM);
            m_Volume = 0.5f;
            m_Obj.SetVolume(0, 0.5f, 0);
        }
        if (frame <= 460) {
            if (m_Volume != 0.5f) {
                m_Volume = 0.5f;
                m_Obj.SetVolume(0, 0.5f, 10);
            }
        } else if (frame > 470 && m_Volume != 0.3f) {
            m_Volume = 0.3f;
            m_Obj.SetVolume(0, 0.3f, 100);
        }
    } else if (static_cast<u32>(frame - 570) < 190) {
        if (GetHandle(0).GetId() == SE_TV_13_BGM) {
            GetHandle(0).Stop(60);
            m_Volume = 0.0f;
        }
        if (frame == 650) {
            PlayEffect(SE_TV_13_FIRE_ON);
        } else if (frame > 650 && GetHandle(0).m_pSound == NULL) {
            PlayEffect(SE_TV_13_YUDE);
        }
    } else if (static_cast<u32>(frame - 930) < 660) {
        if (GetHandle(0).GetId() == SE_TV_13_YUDE) {
            GetHandle(0).Stop(60);
        }
        if (frame == 1095) {
            PlayEffect(SE_TV_13_EGG_BREAK);
        } else if (frame == 1155 || frame == 1175 || frame == 1195) {
            PlayEffect(SE_TV_13_PEPPER_MILL);
        } else if (frame == 1225) {
            PlayEffect(SE_TV_13_PUT_BOWL);
        } else if (frame == 1270) {
            PlayEffect(SE_TV_13_PUT_SILVER);
        } else if (static_cast<u32>(frame - 1291) < 239 && GetHandle(0).GetId() != SE_TV_13_HAND_MIX) {
            PlayEffect(SE_TV_13_HAND_MIX);
        } else if (frame == 1560) {
            PlayEffect(SE_TV_13_PUT_SILVER);
        }
    } else if (static_cast<u32>(frame - 1590) < 400) {
        if (GetHandle(0).GetId() == SE_TV_13_HAND_MIX) {
            GetHandle(0).Stop(10);
        }
        if (frame < 1720 && GetHandle(0).GetId() != SE_TV_13_CUT) {
            PlayEffect(SE_TV_13_CUT);
        }
    } else if (static_cast<u32>(frame - 1990) < 423) {
        if (GetHandle(0).GetId() == SE_TV_13_CUT) {
            GetHandle(0).Stop(30);
        }
        if (frame == 2030) {
            PlayEffect(SE_TV_13_FIRE_ON);
        } else if (frame > 2030 && GetHandle(0).m_pSound == NULL) {
            PlayEffect(SE_TV_13_ITAME);
        }
    } else if (static_cast<u32>(frame - 2413) < 307) {
        if (frame == 2420) {
            PlayEffect(SE_TV_13_FIRE_OFF);
        } else if (frame == 2458) {
            PlayEffect(SE_TV_13_PUT_BOWL);
        } else if (frame == 2478) {
            PlayEffect(SE_TV_13_PUT_DISH);
        } else if (frame == 2510) {
            PlayEffect(SE_TV_13_PUT_SILVER);
        } else if (frame >= 2530) {
            if (GetHandle(0).GetId() != SE_TV_13_BGM) {
                PlaySound(0, SE_TV_13_BGM);
                m_Volume = 0.0f;
                m_Obj.SetVolume(0, 0.0f, 0);
            }
            if (m_Volume != 1.0f) {
                m_Volume = 1.0f;
                m_Obj.SetVolume(0, 1.0f, 150);
            }
        }
    }
    switch (frame - 0) {
        case 360:
            SetScene(0, 0);
            return;
        case 389:
            SetScene(1, 0);
            return;
        case 401:
            SetScene(2, 0);
            return;
        case 434:
            SetScene(3, 0);
            return;
        case 466:
            SetScene(4, 0);
            return;
        case 481:
            SetScene(5, 0);
            return;
        case 525:
            SetScene(6, 0);
            return;
        case 531:
            SetScene(7, 0);
            return;
        case 570:
            SetScene(8, 0);
            return;
        case 600:
            SetScene(6, 0);
            return;
        case 606:
            SetScene(9, 0);
            return;
        case 810:
            SetScene(10, 0);
            return;
        case 829:
            SetScene(11, 0);
            return;
        case 852:
            SetScene(37, 0);
            return;
        case 930:
            SetScene(12, 0);
            return;
        case 950:
            SetScene(13, 0);
            return;
        case 976:
            SetScene(6, 0);
            return;
        case 982:
            SetScene(14, 0);
            return;
        case 1028:
            SetScene(15, 0);
            return;
        case 1039:
            SetScene(16, 0);
            return;
        case 1048:
            SetScene(17, 0);
            return;
        case 1059:
            SetScene(18, 0);
            return;
        case 1065:
            SetScene(19, 0);
            return;
        case 1250:
            SetScene(20, 0);
            return;
        case 1590:
            SetScene(21, 0);
            return;
        case 1618:
            SetScene(22, 0);
            return;
        case 1657:
            SetScene(23, 0);
            return;
        case 1669:
            SetScene(24, 0);
            return;
        case 1679:
            SetScene(28, 0);
            return;
        case 1719:
            SetScene(25, 0);
            return;
        case 1741:
            SetScene(26, 0);
            return;
        case 1748:
            SetScene(27, 0);
            return;
        case 1758:
            SetScene(28, 0);
            return;
        case 1773:
            SetScene(29, 0);
            return;
        case 1821:
            SetScene(30, 0);
            return;
        case 1840:
            SetScene(31, 0);
            return;
        case 1861:
            SetScene(19, 0);
            return;
        case 1990:
            SetScene(11, 0);
            return;
        case 2027:
            SetScene(20, 0);
            return;
        case 2380:
            SetScene(32, 0);
            return;
        case 2413:
            SetScene(33, 0);
            return;
        case 2430:
            SetScene(34, 0);
            return;
        case 2570:
            SetScene(35, 0);
            return;
        case 2590:
            SetScene(36, 0);
            return;
        case 2596:
            SetScene(37, 0);
            return;
    }
}
