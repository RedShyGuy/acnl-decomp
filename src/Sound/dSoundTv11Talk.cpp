#include "Sound/dSoundTv11Talk.h"
#include "Sound/dSoundLabel.h"

// 0x0024928C
// 0x0024927C (deleting dtor)
SoundTv11Talk::~SoundTv11Talk()
{
    // (only the base is destroyed)
}

// 0x00248748
void SoundTv11Talk::Start()
{
    m_ItemId = GROUP_TV_11;
    SoundTvBase::Start();
    m_Volume = 1.0f;
    m_Scene = -1;
}

// 0x002491E4
void SoundTv11Talk::SetScene(s8 scene, s8 variant)
{
    if (m_Scene == scene) {
        return;
    }
    PlaySound(1, SE_TV_11);
    if (GetHandle(1).m_pSound != NULL) {
        WriteVariable(1, 0, scene);
        WriteVariable(1, 1, variant);
    }
    m_Scene = scene;
}

// 0x002487C8
void SoundTv11Talk::UpdateProgram(s32 frame)
{
    if (static_cast<u32>(frame) < 540) {
        if (GetHandle(0).m_pSound == NULL) {
            PlaySound(0, SE_TV_11_BGM);
            m_Obj.SetVolume(0, 1.0f, 0);
            m_Volume = 1.0f;
        }
        if (static_cast<u32>(frame - 180) < 30 && m_Volume != 0.5f) {
            m_Obj.SetVolume(0, 0.5f, 30);
            m_Volume = 0.5f;
        } else if (frame >= 210 && m_Volume != 0.0f) {
            m_Obj.SetVolume(0, 0.0f, 330);
            m_Volume = 0.0f;
        }
    } else if (static_cast<u32>(frame - 540) < 1840) {
        if (GetHandle(0).m_pSound != NULL) {
            GetHandle(0).Stop(0);
            m_Volume = 0.0f;
        }
    } else if (static_cast<u32>(frame - 2380) < 320) {
        if (GetHandle(0).m_pSound == NULL) {
            PlaySound(0, SE_TV_11_BGM);
            m_Obj.SetVolume(0, 0.5f, 0);
            m_Volume = 0.5f;
        }
        if (frame > 2580 && m_Volume != 1.0f) {
            m_Obj.SetVolume(0, 1.0f, 120);
            m_Volume = 1.0f;
        }
    }
    switch (frame - 180) {
        case 0:
            SetScene(0, 0);
            return;
        case 52:
            SetScene(1, 0);
            return;
        case 97:
            SetScene(2, 0);
            return;
        case 142:
            SetScene(3, 0);
            return;
        case 187:
            SetScene(4, 0);
            return;
        case 255:
            SetScene(5, 0);
            return;
        case 307:
            SetScene(6, 0);
            return;
        case 352:
            SetScene(7, 0);
            return;
        case 390:
            SetScene(8, 0);
            return;
        case 420:
            SetScene(9, 0);
            return;
        case 465:
            SetScene(10, 0);
            return;
        case 570:
            SetScene(11, 0);
            return;
        case 622:
            SetScene(12, 0);
            return;
        case 637:
            SetScene(13, 0);
            return;
        case 652:
            SetScene(14, 0);
            return;
        case 687:
            SetScene(15, 0);
            return;
        case 723:
            SetScene(16, 0);
            return;
        case 757:
            SetScene(17, 0);
            return;
        case 765:
            SetScene(18, 0);
            return;
        case 776:
            SetScene(19, 0);
            return;
        case 816:
            SetScene(20, 0);
            return;
        case 885:
            SetScene(21, 0);
            return;
        case 910:
            SetScene(30, 6);
            return;
        case 945:
            SetScene(22, 0);
            return;
        case 1020:
            SetScene(23, 0);
            return;
        case 1057:
            SetScene(24, 0);
            return;
        case 1072:
            SetScene(25, 0);
            return;
        case 1102:
            SetScene(26, 0);
            return;
        case 1147:
            SetScene(27, 0);
            return;
        case 1200:
            SetScene(28, 0);
            return;
        case 1245:
            SetScene(29, 0);
            return;
        case 1297:
            SetScene(30, 0);
            return;
        case 1312:
            SetScene(31, 0);
            return;
        case 1350:
            SetScene(32, 0);
            return;
        case 1357:
            SetScene(33, 0);
            return;
        case 1372:
            SetScene(34, 0);
            return;
        case 1387:
            SetScene(35, 0);
            return;
        case 1410:
            SetScene(36, 0);
            return;
        case 1440:
            SetScene(37, 0);
            return;
        case 1470:
            SetScene(29, 0);
            return;
        case 1500:
            SetScene(38, 0);
            return;
        case 1545:
            SetScene(39, 0);
            return;
        case 1560:
            SetScene(54, 34);
            return;
        case 1597:
            SetScene(40, 0);
            return;
        case 1635:
            SetScene(41, 0);
            return;
        case 1680:
            SetScene(42, 0);
            return;
        case 1715:
            SetScene(43, 0);
            return;
        case 1758:
            SetScene(44, 0);
            return;
        case 1822:
            SetScene(45, 0);
            return;
        case 1852:
            SetScene(46, 0);
            return;
        case 1912:
            SetScene(47, 0);
            return;
        case 1927:
            SetScene(48, 0);
            return;
        case 1957:
            SetScene(49, 0);
            return;
        case 1980:
            SetScene(50, 0);
            return;
        case 2002:
            SetScene(51, 0);
            return;
        case 2025:
            SetScene(52, 0);
            return;
        case 2055:
            SetScene(53, 0);
            return;
        case 2092:
            SetScene(54, 0);
            return;
        case 2115:
            SetScene(55, 0);
            return;
    }
    // the laughter at the end
    if (static_cast<u32>(frame - 180 - 2152) < 338) {
        if (m_Scene != 56 && m_Scene != -1) {
            PlaySound(1, SE_TV_11_DOUBLE_LAUGH);
            m_Scene = 56;
        }
        if (frame - 180 > 2400 && m_Scene != -1) {
            GetHandle(1).Stop(90);
            m_Scene = -1;
        }
    }
}
