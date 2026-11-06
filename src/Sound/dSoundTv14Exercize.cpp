#include "Sound/dSoundTv14Exercize.h"
#include "Sound/dSoundLabel.h"

// 0x002D3D74
// 0x002D3D64 (deleting dtor)
SoundTv14Exercize::~SoundTv14Exercize()
{
    // (only the base is destroyed)
}

// 0x002D3AF0
void SoundTv14Exercize::Start()
{
    m_ItemId = GROUP_TV_14;
    SoundTvBase::Start();
    m_Part = 0;
}

// 0x002D3B64
void SoundTv14Exercize::SetPart(u8 part, u32 index, u32 soundId)
{
    if (m_Part == part) {
        return;
    }
    u32 other = index == 0 ? 1 : 0;
    bool isCross = false;
    if (m_Obj.m_HandleSet.m_Handles[other].m_pSound != NULL) {
        m_Obj.m_HandleSet.m_Handles[other].Stop(15);
        isCross = true;
    }
    PlaySound(index, soundId);
    m_Part = part;
    if (isCross) {
        m_Obj.SetVolume(index, 0.0f, 0);
        m_Obj.SetVolume(index, 1.0f, 15);
    } else {
        m_Obj.SetVolume(index, 1.0f, 0);
    }
}

// 0x002D3C50
void SoundTv14Exercize::UpdateProgram(s32 frame)
{
    if (static_cast<u32>(frame) < 296) {
        SetPart(1, 0, SE_TV_14_BGM);
    } else if (static_cast<u32>(frame - 296) < 643) {
        SetPart(2, 1, SE_TV_14_2);
    } else if (static_cast<u32>(frame - 939) < 304) {
        SetPart(3, 0, SE_TV_14_3);
    } else if (static_cast<u32>(frame - 1243) <= 276) {
        SetPart(4, 1, SE_TV_14_4);
    } else if (static_cast<u32>(frame - 1520) < 342) {
        SetPart(5, 0, SE_TV_14_5);
    } else if (static_cast<u32>(frame - 1862) < 362) {
        SetPart(6, 1, SE_TV_14_6);
    } else if (static_cast<u32>(frame - 2224) < 236) {
        SetPart(7, 0, SE_TV_14_7);
    } else {
        SetPart(8, 1, SE_TV_14_8);
    }
}
