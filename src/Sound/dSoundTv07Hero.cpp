#include "Sound/dSoundTv07Hero.h"
#include "Sound/dSoundLabel.h"
#include "netgame/dGameFunctions.h"

// 0x00248744
// 0x00248734 (deleting dtor)
SoundTv07Hero::~SoundTv07Hero()
{
    // (only the base is destroyed)
}

// 0x0024837C
void SoundTv07Hero::Start()
{
    m_ItemId = GROUP_TV_07;
    m_IsTrackMuted = 0;
    m_ChangeCount = 0;
    m_LastScene = 3;
    m_Scene = 3;
    SoundTvBase::Start();
}

// 0x00248724
void SoundTv07Hero::OnLoaded()
{
    PlaySound(1, SE_TV_07_BGM1);
}

// 0x00248400
void SoundTv07Hero::UpdateProgram(s32 frame)
{
    s32 t = frame % 357;
    if (m_PlayArg == 1) {
        m_PlayArg = 0;
        GetHandle(1).Stop(0);
        PlaySound(1, SE_TV_07_BGM0);
        GetHandle(0).Stop(0);
        PlaySound(0, SE_TV_07_SE_LAUGH);
        m_Scene = 0;
        m_LastScene = 0;
        m_IsTrackMuted = 0;
        m_ChangeCount = 0;
        return;
    }
    if (t == 0) {
        GetHandle(1).Stop(0);
        GetHandle(0).Stop(0);
        if (netgame::Func2FF63C(100) < 40) {
            PlaySound(1, SE_TV_07_BGM0s);
            PlaySound(0, SE_TV_07_SE_LAUGH);
            m_Scene = 1;
        } else {
            PlaySound(1, SE_TV_07_BGM2);
            PlaySound(0, SE_TV_07_SE_POWERUP);
            m_Scene = 2;
        }
    } else if (t == 135) {
        if (m_Scene != 0) {
            u32 random = netgame::Func2FF63C(3);
            GetHandle(1).Stop(0);
            GetHandle(0).Stop(0);
            if (random == 0) {
                PlaySound(1, SE_TV_07_BGM1);
                PlaySound(0, SE_TV_07_SE_RUNBOMB);
                m_Scene = 3;
            } else if (random == 1) {
                PlaySound(1, SE_TV_07_BGM1);
                PlaySound(0, SE_TV_07_SE_RUNWIND);
                m_Scene = 4;
            } else {
                PlaySound(1, SE_TV_07_BGM3);
                PlaySound(0, SE_TV_07_SE_RUNBOMB);
                m_Scene = 5;
            }
        }
    } else if (t == 180) {
        if (m_Scene == 0) {
            u32 random = netgame::Func2FF63C(2);
            GetHandle(1).Stop(0);
            GetHandle(0).Stop(0);
            if (random == 0) {
                PlaySound(1, SE_TV_07_BGM1);
                PlaySound(0, SE_TV_07_SE_RUNBOMB);
                m_Scene = 3;
            } else {
                PlaySound(1, SE_TV_07_BGM1);
                PlaySound(0, SE_TV_07_SE_RUNWIND);
                m_Scene = 4;
            }
        }
    }
    if (m_LastScene == m_Scene) {
        return;
    }
    if (m_ChangeCount == 0) {
        m_ChangeCount = 1;
    } else if (m_ChangeCount == 1 && netgame::Func2FF63C(2) == 0) {
        m_ChangeCount++;
    } else if (m_ChangeCount >= 1) {
        m_ChangeCount = 0;
        m_IsTrackMuted ^= 1;
    }
    if (m_IsTrackMuted % 2 != 0) {
        SetTrackMute(1, 63, true);
    }
    m_LastScene = m_Scene;
}
