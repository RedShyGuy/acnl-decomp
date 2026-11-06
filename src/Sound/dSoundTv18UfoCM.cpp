#include "Sound/dSoundTv18UfoCM.h"
#include "Sound/dSoundLabel.h"

// 0x00279C58
// 0x00279C48 (deleting dtor)
SoundTv18UfoCM::~SoundTv18UfoCM()
{
    // (only the base is destroyed)
}

// 0x002796F0
void SoundTv18UfoCM::Start()
{
    m_ItemId = GROUP_TV_18;
    SoundTvBase::Start();
    m_Volume = -1.0f;
    m_Scene = 0;
    m_Phrase = -1;
}

// 0x00279B9C
void SoundTv18UfoCM::Talk(s8 phrase, s8 variant)
{
    if (m_Phrase == phrase) {
        return;
    }
    PlaySound(0, SE_TV_18_ALIEN_TALK);
    if (GetHandle(0).m_pSound != NULL) {
        m_Obj.SetVolume(0, 1.0f, 0);
        WriteVariable(0, 0, phrase);
        WriteVariable(0, 1, variant);
    }
    m_Phrase = phrase;
}

// 0x00279B98
void SoundTv18UfoCM::OnLoaded()
{
    // empty (in the original too)
}

// 0x00279778
void SoundTv18UfoCM::UpdateProgram(s32 frame)
{
    if (static_cast<u32>(frame) < 226) {
        if (m_Scene != 1) {
            PlaySound(1, SE_TV_18_UFO_COME);
            m_Scene = 1;
        }
        if (GetHandle(0).GetId() != SE_TV_18) {
            PlaySound(0, SE_TV_18);
            m_Obj.SetVolume(0, 1.0f, 0);
        }
        if (m_Volume != 0.7f) {
            m_Volume = 0.7f;
            m_Obj.SetVolume(0, 0.7f, 90);
        }
    } else if (static_cast<u32>(frame - 226) < 99) {
        if (m_Scene != 2) {
            PlaySound(1, SE_TV_18_ALIEN_APPEAR);
            m_Scene = 2;
        }
        if (GetHandle(0).GetId() != SE_TV_18) {
            PlaySound(0, SE_TV_18);
            m_Obj.SetVolume(0, 0.5f, 0);
        }
        // (the original tests 0.3 but sets 0)
        if (m_Volume != 0.3f) {
            m_Volume = 0.0f;
            m_Obj.SetVolume(0, 0.0f, 99);
        }
    } else if (static_cast<u32>(frame - 325) <= 300) {
        if (GetHandle(0).GetId() == SE_TV_18) {
            GetHandle(0).Stop(0);
        }
        if (m_Scene != 3) {
            PlaySound(1, SE_TV_18_ALIEN_LOOP);
            m_Scene = 3;
        }
        goto talk;
    } else if (static_cast<u32>(frame - 626) < 93) {
        if (m_Scene != 4) {
            PlaySound(1, SE_TV_18_ALIEN_DISAPPEAR);
            m_Scene = 4;
        }
        if (GetHandle(0).GetId() != SE_TV_18) {
            PlaySound(0, SE_TV_18);
            m_Obj.SetVolume(0, 0.3f, 0);
        }
        if (m_Volume != 0.3f) {
            m_Volume = 0.3f;
            m_Obj.SetVolume(0, 0.3f, 60);
        }
    } else {
        if (m_Scene != 5) {
            PlaySound(1, SE_TV_18_UFO_LEAVE);
            m_Scene = 5;
        }
        if (GetHandle(0).GetId() != SE_TV_18) {
            PlaySound(0, SE_TV_18);
            m_Obj.SetVolume(0, 0.3f, 0);
        }
        if (frame < 750) {
            if (m_Volume != 0.5f) {
                m_Volume = 0.5f;
                m_Obj.SetVolume(0, 0.5f, 30);
            }
        } else if (m_Volume != 1.0f) {
            m_Volume = 1.0f;
            m_Obj.SetVolume(0, 1.0f, 90);
        }
    }
    if (m_Scene != 3) {
        return;
    }
talk:
    switch (frame - 431) {
        case 0:
            Talk(0, 0);
            break;
        case 20:
            Talk(1, 0);
            break;
        case 50:
            Talk(2, 0);
            break;
        case 89:
            Talk(3, 0);
            break;
        case 110:
            Talk(4, 0);
            break;
        case 152:
            Talk(5, 0);
            break;
    }
}
