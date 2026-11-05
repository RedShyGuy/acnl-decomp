#include "Sound/dSoundMelodyPlayerOneSeqTimeTone.h"
#include "nw/snd/internal/snd_BasicSound.h"

// 0x00132874
SoundMelodyPlayerOneSeqTimeTone::SoundMelodyPlayerOneSeqTimeTone()
    : m_IsMuffled(false), m_IsDistant(false), m_IsDisabled(0)
{
}

// 0x004DBDD4
// 0x004DBD98 (deleting dtor)
SoundMelodyPlayerOneSeqTimeTone::~SoundMelodyPlayerOneSeqTimeTone()
{
    StopSound();
}

// 0x004DBB98
void SoundMelodyPlayerOneSeqTimeTone::Initialize()
{
    m_FilterFader.SoundFader::Reset(0.0f);
    m_VolumeFader.SoundFader::Reset(1.0f);
}

inline void SoundMelodyPlayerOneSeqTimeTone::ApplyFaders()
{
    m_FilterFader.SoundFader::Update();
    m_VolumeFader.SoundFader::Update();
    if (m_Handle.m_pSound != NULL) {
        m_Handle.m_pSound->SetBiquadFilter(1, m_FilterFader.GetValue());
    }
    if (m_Handle.m_pSound != NULL) {
        m_Handle.m_pSound->SetVolume(m_VolumeFader.GetValue(), 0);
    }
}

// 0x004DBBC8
void SoundMelodyPlayerOneSeqTimeTone::Start(u32 soundId, const u8* notes)
{
    if (m_IsDisabled == 1) {
        return;
    }
    SoundMelodyPlayerOneSeqBase::Start(soundId, notes);
    ApplyFaders();
}

// 0x001B0A54
void SoundMelodyPlayerOneSeqTimeTone::Update()
{
    SoundMelodyPlayerOneSeqBase::Update();
    if (m_State == STATE_PLAY || m_State == STATE_STOP) {
        ApplyFaders();
    }
    m_FilterFader.SoundFader::Update();
    m_VolumeFader.SoundFader::Update();
}

// 0x004DBC2C
void SoundMelodyPlayerOneSeqTimeTone::SetDistant(bool isDistant)
{
    if (m_IsDistant == isDistant) {
        return;
    }
    if (isDistant) {
        f32 volume = 0.4f;
        if (m_Handle.m_pSound != NULL) {
            m_FilterFader.SetTarget(0.5f, 90);
            m_VolumeFader.SetTarget(volume, 90);
        } else {
            m_FilterFader.SetTarget(0.5f, 0);
            m_VolumeFader.SetTarget(volume, 0);
        }
    } else {
        f32 volume = 1.0f;
        if (m_Handle.m_pSound != NULL) {
            m_FilterFader.SetTarget(0.0f, 150);
            m_VolumeFader.SetTarget(volume, 150);
        } else {
            m_FilterFader.SetTarget(0.0f, 0);
            m_VolumeFader.SetTarget(volume, 0);
        }
    }
    m_IsDistant = isDistant;
}

// 0x004DBCF4
void SoundMelodyPlayerOneSeqTimeTone::SetMuffled(bool isMuffled)
{
    if (m_IsMuffled == isMuffled) {
        return;
    }
    f32 filter;
    if (isMuffled) {
        filter = 0.8f;
    } else {
        filter = 0.0f;
    }
    if (m_Handle.m_pSound != NULL) {
        m_FilterFader.SetTarget(filter, 20);
        m_VolumeFader.SetTarget(1.0f, 20);
    } else {
        m_FilterFader.SetTarget(filter, 0);
        m_VolumeFader.SetTarget(1.0f, 0);
    }
    m_IsMuffled = isMuffled;
}
