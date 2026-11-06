#include "Sound/dSoundTv12Anime.h"
#include "Sound/dSoundLabel.h"

// 0x002796EC
// 0x002796DC (deleting dtor)
SoundTv12Anime::~SoundTv12Anime()
{
    // (only the base is destroyed)
}

// 0x00279328
void SoundTv12Anime::Start()
{
    m_ItemId = GROUP_TV_12;
    m_Phase = 0;
    m_IsMuted = 0;
    m_MuteFrames = 0;
    m_IsCrash1 = 0;
    SoundTvBase::Start();
}

// 0x002793A8
void SoundTv12Anime::UpdateProgram(s32 frame)
{
    s32 t = frame % 483;
    s32 rest;
    if (t < 290) {
        if (GetHandle(1).GetId() != SE_TV_12_RUN) {
            PlaySound(1, SE_TV_12_RUN);
        }
        m_Phase = 0;
        rest = 290 - t;
    } else if (t < 342) {
        if (GetHandle(1).GetId() != SE_TV_12_JUMP_START) {
            PlaySound(1, SE_TV_12_JUMP_START);
            m_IsCrash1 = !m_IsCrash1;
        }
        m_Phase = 1;
        rest = 342 - t;
    } else {
        u32 crash = m_IsCrash1 ? SE_TV_12_CRASH_1 : SE_TV_12_CRASH_2;
        if (GetHandle(1).GetId() != crash) {
            PlaySound(1, crash);
        }
        m_Phase = 2;
        rest = 483 - t;
    }
    // the remaining time of the phase for the sequence (in 1/32 seconds)
    if (GetHandle(1).m_pSound != NULL) {
        WriteVariable(1, 0, static_cast<s8>(rest * 96 / 30));
    }
    if (GetHandle(0).m_pSound != NULL) {
        if (m_Phase != 0) {
            if (m_IsMuted == 0) {
                if (m_MuteFrames == 0) {
                    m_MuteFrames = 10;
                    SetTrackSilence(0, 6, true, m_MuteFrames);
                } else if (--m_MuteFrames == 0) {
                    SetTrackMute(0, 6, true);
                    m_IsMuted = 1;
                }
            }
        } else if (m_IsMuted == 1) {
            SetTrackMute(0, 6, false);
            SetTrackSilence(0, 6, false, 10);
            m_IsMuted = 0;
            m_MuteFrames = 0;
        }
        return;
    }
    if (m_PlayArg != 0) {
        m_IsMuted = 0;
        m_PlayArg = 0;
    } else {
        m_IsMuted = m_Phase != 0;
    }
    PlaySound(0, SE_TV_12_BGM);
    SetTrackSilence(0, 6, static_cast<s8>(m_IsMuted), 0);
    SetTrackMute(0, 6, static_cast<s8>(m_IsMuted));
    m_MuteFrames = 0;
}
