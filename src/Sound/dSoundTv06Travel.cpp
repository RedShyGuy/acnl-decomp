#include "Sound/dSoundTv06Travel.h"
#include "Sound/dSoundLabel.h"

// 0x002A5CD8
// 0x002A5CC8 (deleting dtor)
SoundTv06Travel::~SoundTv06Travel()
{
    // (only the base is destroyed)
}

// 0x002A56D0
void SoundTv06Travel::Start()
{
    m_ItemId = GROUP_TV_06;
    m_HasVoice = 1;
    SoundTvBase::Start();
}

// 0x002A5C98
void SoundTv06Travel::OnLoaded()
{
    PlaySound(1, SE_TV_06_BGM0);
    PlaySound(0, SE_TV_06_VOICE0);
}

// 0x002A5740
void SoundTv06Travel::UpdateProgram(s32 frame)
{
    switch (frame % 780) {
        case 0:
            SetTrackVolume(1, 1, 70.0f / 127.0f);
            SetTrackVolume(1, 2, 80.0f / 127.0f);
            SetTrackVolume(1, 12, 120.0f / 127.0f);
            SetTrackVolume(1, 16, 110.0f / 127.0f);
            if (m_HasVoice != 0) {
                GetHandle(0).Stop(0);
                PlaySound(0, SE_TV_06_VOICE0);
            }
            break;
        case 135:
            SetTrackVolume(1, 1, 75.0f / 127.0f);
            SetTrackVolume(1, 14, 110.0f / 127.0f);
            SetTrackVolume(1, 16, 110.0f / 127.0f);
            if (m_HasVoice != 0) {
                GetHandle(0).Stop(0);
                PlaySound(0, SE_TV_06_VOICE1);
            }
            break;
        case 240:
            SetTrackVolume(1, 1, 80.0f / 127.0f);
            SetTrackVolume(1, 14, 110.0f / 127.0f);
            SetTrackVolume(1, 16, 110.0f / 127.0f);
            if (m_HasVoice != 0) {
                GetHandle(0).Stop(0);
                PlaySound(0, SE_TV_06_VOICE2);
            }
            break;
        case 390:
            SetTrackVolume(1, 1, 80.0f / 127.0f);
            SetTrackVolume(1, 14, 110.0f / 127.0f);
            SetTrackVolume(1, 16, 100.0f / 127.0f);
            if (m_HasVoice != 0) {
                GetHandle(0).Stop(0);
                PlaySound(0, SE_TV_06_VOICE3);
            }
            break;
        case 556:
            SetTrackVolume(1, 1, 75.0f / 127.0f);
            SetTrackVolume(1, 2, 0.0f);
            SetTrackVolume(1, 12, 9.0f / 127.0f);
            SetTrackVolume(1, 16, 75.0f / 127.0f);
            if (m_HasVoice != 0) {
                GetHandle(0).Stop(0);
                PlaySound(0, SE_TV_06_VOICE4);
            }
            break;
        case 676:
            SetTrackVolume(1, 1, 85.0f / 127.0f);
            SetTrackVolume(1, 14, 0.0f);
            SetTrackVolume(1, 16, 70.0f / 127.0f);
            m_HasVoice = m_HasVoice != 1;
            break;
    }
}
