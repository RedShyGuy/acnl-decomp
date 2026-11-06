#include "Sound/dSoundTv08Variety.h"
#include "Sound/dSoundLabel.h"
#include "netgame/dGameFunctions.h"

// 0x002BDB78
// 0x002BDB68 (deleting dtor)
SoundTv08Variety::~SoundTv08Variety()
{
    // (only the base is destroyed)
}

// 0x002BD6C0
void SoundTv08Variety::Start()
{
    m_ItemId = GROUP_TV_08;
    m_Reaction = 0;
    m_MusicFrame = 0;
    SoundTvBase::Start();
}

// 0x002BDB58
void SoundTv08Variety::OnLoaded()
{
    PlaySound(0, SE_TV_08_BGM1);
}

// 0x002BD738
void SoundTv08Variety::UpdateProgram(s32 frame)
{
    s32 t = frame % 450;
    if (m_PlayArg == 1) {
        m_PlayArg = 0;
        GetHandle(0).Stop(0);
        PlaySound(0, SE_TV_08_BGM0);
        m_MusicFrame = 0;
    } else if (t == 0) {
        if (netgame::Func2FF63C(100) < 70) {
            WriteVariable(0, 0, 1);
        }
        GetHandle(1).Stop(0);
        PlaySound(1, SE_TV_08_VOICE0);
    } else if (t == 135 || t == 217) {
        if (t == 135 && netgame::Func2FF63C(100) < 60) {
            WriteVariable(0, 1, 1);
            m_Reaction = 1;
        }
        u32 random = netgame::Func2FF63C(100);
        GetHandle(1).Stop(0);
        if (random < 33) {
            PlaySound(1, SE_TV_08_VOICE10);
        } else if (random < 66) {
            PlaySound(1, SE_TV_08_VOICE11);
        } else {
            PlaySound(1, SE_TV_08_VOICE12);
        }
    } else if (t == 270) {
        if (netgame::Func2FF63C(100) < 70 && m_Reaction != 1) {
            WriteVariable(0, 1, 1);
            m_Reaction = 2;
        }
        if (netgame::Func2FF63C(100) < 60 && m_Reaction != 1) {
            WriteVariable(0, 0, 1);
            m_Reaction = 2;
        }
    } else if (t == 375) {
        u32 random = netgame::Func2FF63C(100);
        if (m_Reaction != 2) {
            if (random < 70) {
                WriteVariable(0, 2, 1);
                m_Reaction = 3;
            } else {
                WriteVariable(0, 0, 1);
                m_Reaction = 4;
            }
        }
        GetHandle(1).Stop(0);
        PlaySound(1, SE_TV_08_VOICE2);
    } else if (m_MusicFrame == 154) {
        GetHandle(0).Stop(0);
        PlaySound(0, SE_TV_08_BGM1);
    } else if (m_MusicFrame == 1050) {
        GetHandle(0).Stop(0);
        PlaySound(0, SE_TV_08_BGM2);
    } else if (m_MusicFrame == 1350) {
        GetHandle(0).Stop(0);
        PlaySound(0, SE_TV_08_BGM0);
        m_MusicFrame = 0;
    }
    m_MusicFrame++;
}
