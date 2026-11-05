#include "Sound/dSoundMelodyPlayerOneSeqBase.h"
#include "Sound/dSoundDataLoadMgr.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMelodyPlayerBase.h"
#include "Sound/dSoundMgr.h"
#include "netgame/dGameFunctions.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include "nw/snd/snd_SequenceSoundHandle.h"

// 0x00136128
SoundMelodyPlayerOneSeqBase::SoundMelodyPlayerOneSeqBase()
    : m_pNotes(NULL), m_SoundId(0), m_State(STATE_NONE), m_CanRestart(false)
{
}

// 0x0034350C
// 0x003434E8 (deleting dtor)
SoundMelodyPlayerOneSeqBase::~SoundMelodyPlayerOneSeqBase()
{
    // (only the members are destroyed)
}

// 0x003432A4
void SoundMelodyPlayerOneSeqBase::Stop(int fadeFrames)
{
    if (m_State != STATE_NONE) {
        m_Handle.Stop(fadeFrames);
        m_State = STATE_STOP;
    }
}

// 0x003432D0
void SoundMelodyPlayerOneSeqBase::Start(u32 soundId, const u8* notes)
{
    if (m_State != STATE_NONE) {
        if (!m_CanRestart) {
            return;
        }
        m_Handle.Stop(0);
        m_State = STATE_NONE;
    }
    if (!SoundDataLoadMgr::s_pInstance->IsLoaded(soundId)) {
        return;
    }
    if (notes != NULL) {
        m_pNotes = notes;
    } else {
        m_pNotes = SoundMgr::s_pInstance->m_MelodyMgr.m_Melody.notes;
    }
    m_SoundId = soundId;
    Setup();
    m_State = STATE_PLAY;
}

// 0x0034337C
void SoundMelodyPlayerOneSeqBase::Setup()
{
    SoundMgr::s_pInstance->StartSound(&m_Handle, m_SoundId);
    nw::snd::SequenceSoundHandle sequence(&m_Handle);
    for (s32 i = 0; i < SoundMelodyPlayerBase::NOTE_NUM; i++) {
        u8 note = m_pNotes[i];
        s16 value;
        if (note == SoundMelodyPlayerBase::NOTE_RANDOM) {
            value = SoundMelodyPlayerBase::GetPitch(0, netgame::Func2FF63C(6) + SoundMelodyPlayerBase::NOTE_RANDOM);
        } else if (note == SoundMelodyPlayerBase::NOTE_HOLD) {
            value = 500;
        } else if (note == SoundMelodyPlayerBase::NOTE_REST) {
            value = 1000;
        } else {
            value = SoundMelodyPlayerBase::GetPitch(0, note);
        }
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->WriteVariable(i, value);
        }
    }
}

// 0x00343498
void SoundMelodyPlayerOneSeqBase::Update()
{
    if ((m_State == STATE_PLAY || m_State == STATE_STOP) && m_Handle.m_pSound == NULL) {
        m_State = STATE_NONE;
    }
}

// 0x003434BC
void SoundMelodyPlayerOneSeqBase::StopDoorChime()
{
    if (m_SoundId == SE_MELODY_ONESEQ_DOOR_CONVENI) {
        m_Handle.Stop(0);
    }
}
