#include "Sound/dSoundTv20JuiceCM.h"
#include "Sound/dSoundLabel.h"

// 0x002BEA6C
// 0x002BEA5C (deleting dtor)
SoundTv20JuiceCM::~SoundTv20JuiceCM()
{
    // (only the base is destroyed)
}

// 0x002BE898
void SoundTv20JuiceCM::Start()
{
    m_ItemId = GROUP_TV_20;
    SoundTvBase::Start();
}

// 0x002BE910
void SoundTv20JuiceCM::UpdateProgram(s32 frame)
{
    if (frame == 0 || GetHandle(0).m_pSound == NULL) {
        PlaySound(0, SE_TV_20_BGM);
        m_Obj.SetVolume(0, 1.0f, 0);
        m_Volume = 1.0f;
    }
    if (frame >= 360 && m_Volume != 0.0f) {
        m_Obj.SetVolume(0, 0.0f, 90);
        m_Volume = 0.0f;
    }
    u32 current = GetHandle(1).GetId();
    u32 sound;
    if (static_cast<u32>(frame) < 110) {
        sound = SE_TV_20;
    } else if (static_cast<u32>(frame - 110) < 110) {
        sound = SE_TV_20_2;
    } else if (static_cast<u32>(frame - 220) < 10) {
        sound = SE_TV_20_3;
    } else if (static_cast<u32>(frame - 230) < 110) {
        sound = SE_TV_20_4;
    } else {
        sound = SE_TV_20_5;
    }
    if (current != sound) {
        PlaySound(1, sound);
    }
}
