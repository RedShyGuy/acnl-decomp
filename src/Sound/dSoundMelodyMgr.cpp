#include "Sound/dSoundMelodyMgr.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "netgame/dGameFunctions.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include "nw/snd/snd_SequenceSoundHandle.h"
#include "nw/snd/snd_SoundArchive.h"
#include "sead/seadFormatFixedSafeString.h"
#include "stage/stage_Api.h"

// 0x0083C146
const SoundMelody SoundMelodyMgr::s_DefaultMelodies[5] = {
    {{10, 15, 7, 5, 6, 6, 6, 15, 7, 7, 6, 13, 3, 14, 14, 15}},
    {{5, 5, 7, 10, 12, 15, 12, 15, 10, 15, 13, 15, 3, 14, 14, 15}},
    {{7, 8, 15, 7, 6, 8, 15, 10, 12, 15, 13, 14, 10, 15, 15, 15}},
    {{3, 3, 5, 7, 6, 6, 8, 10, 12, 15, 13, 14, 10, 15, 15, 15}},
    {{9, 14, 7, 4, 5, 7, 10, 12, 11, 15, 13, 15, 7, 14, 14, 15}},
};
// 0x0083C196
const s16 SoundMelodyMgr::s_EditorPitches[14] = {-5, -3, -1, 0, 2, 4, 5, 7, 9, 11, 12, 14, 16, -5};

// 0x001289F0
SoundMelodyMgr::SoundMelodyMgr() : m_Melody(s_DefaultMelodies[0]), m_Delay(0), m_pDelayedNotes(NULL)
{
}

// 0x002771F0
// 0x0027718C (deleting dtor)
SoundMelodyMgr::~SoundMelodyMgr()
{
    // (only the members are destroyed)
}

// 0x004DBB90
void SoundMelodyMgr::initialize(sead::AudioMgr*, sead::Heap*)
{
    m_TimeChime.Initialize();
}

// 0x00276DC8
void SoundMelodyMgr::Play(u32 soundId, const u8* notes)
{
    if (soundId == SE_MELODY_LETTER) {
        m_Delay = 12;
        m_pDelayedNotes = notes;
        return;
    }
    if (m_OneSeqPlayer.m_State != SoundMelodyPlayerOneSeqBase::STATE_NONE) {
        return;
    }
    m_Player.m_IsPan = 0;
    if (soundId == SE_MELODY_NPCSP_ALW && stage::GetCurrent() == stage::NAME_RECYCLE_SHOP) {
        m_OneSeqPlayer.Start(SE_MELODY_ONESEQ_NPCSP_ALW, notes);
    } else {
        m_Player.Start(soundId, notes);
    }
}

// 0x00276E58
void SoundMelodyMgr::PlayEditorNote(u32 soundId, const u8* note)
{
    if (*note > SoundMelodyPlayerBase::NOTE_RANDOM) {
        return;
    }
    m_EditorHandle.Stop(0);
    if (*note == SoundMelodyPlayerBase::NOTE_RANDOM) {
        SoundMgr::s_pInstance->StartSound(&m_EditorHandle, SE_SYS_MELODY_PREVIEW_MO);
        return;
    }
    if (soundId != SE_SYS_MELODY_PREVIEW_NOTE) {
        soundId = SE_SYS_MELODY_PREVIEW_NOTE;
    }
    SoundMgr::s_pInstance->StartSound(&m_EditorHandle, soundId);
    nw::snd::SequenceSoundHandle sequence(&m_EditorHandle);
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->WriteVariable(0, s_EditorPitches[*note]);
    }
}

// 0x00276F0C
void SoundMelodyMgr::PlayTimeChime()
{
    m_TimeChime.Start(SE_MELODY_ONESEQ_CHIME_TIME, NULL);
}

// 0x00276F20
void SoundMelodyMgr::PlayCharacter(const sead::SafeStringBase<char>& name, const u8* notes)
{
    sead::FormatFixedSafeString<6> upperName("%s", name.getCString());
    ToUpper(&upperName);
    sead::FormatFixedSafeString<32> label("SE_MELODY_NPCNRM_%s", upperName.getCString());
    nw::snd::SoundArchive* archive = SoundMgr::s_pInstance->m_pArchive;
    if (archive->GetItemId(label.getCString()) == 0xFFFFFFFF) {
        label.format("SE_MELODY_NPCSP_%s", upperName.getCString());
        if (archive->GetItemId(label.getCString()) == 0xFFFFFFFF) {
            return;
        }
    }
    u32 soundId = archive->GetItemId(label.getCString());
    if (soundId == SE_MELODY_NPCSP_SZA0) {
        if (m_OneSeqPlayer.m_State == SoundMelodyPlayerOneSeqBase::STATE_NONE) {
            m_Player.m_IsPan = 0;
            m_Player.Start(soundId, notes);
        }
        return;
    }
    if (m_OneSeqPlayer.m_State != SoundMelodyPlayerOneSeqBase::STATE_NONE) {
        return;
    }
    m_Player.m_IsPan = 1;
    if (soundId == SE_MELODY_NPCSP_ALW && stage::GetCurrent() == stage::NAME_RECYCLE_SHOP) {
        m_OneSeqPlayer.Start(SE_MELODY_ONESEQ_NPCSP_ALW, notes);
    } else {
        m_Player.Start(soundId, notes);
    }
}

// 0x002770E8
void SoundMelodyMgr::calc()
{
    if (SoundMgr::s_pInstance->IsPaused()) {
        return;
    }
    if (m_Delay > 0 && --m_Delay == 0 && m_OneSeqPlayer.m_State == SoundMelodyPlayerOneSeqBase::STATE_NONE) {
        m_Player.m_IsPan = 0;
        m_Player.Start(SE_MELODY_LETTER, m_pDelayedNotes);
    }
    m_Player.Calc();
    m_OneSeqPlayer.Update();
    m_TimeChime.Update();
}

// 0x00277168
void SoundMelodyMgr::SetMelody(const SoundMelody& melody)
{
    m_Melody = melody;
}

// 0x00343284
void SoundMelodyMgr::Stop()
{
    m_Player.Stop();
    m_TimeChime.Stop(0);
}

// 0x00719BE4
const SoundMelody* SoundMelodyMgr::GetRandomDefaultMelody() const
{
    return &s_DefaultMelodies[netgame::Func2FF63C(4)];
}

// 0x00719C00
void SoundMelodyMgr::ToUpper(sead::BufferedSafeStringBase<char>* str) const
{
    s32 length = str->getLength();
    for (s32 i = 0; i != length + 1; i++) {
        if (str->at(i) == 'a') {
            str->overwriteAt(i, sead::SafeStringBase<char>("A"), -1);
        } else if (str->at(i) == 'b') {
            str->overwriteAt(i, sead::SafeStringBase<char>("B"), -1);
        } else if (str->at(i) == 'c') {
            str->overwriteAt(i, sead::SafeStringBase<char>("C"), -1);
        } else if (str->at(i) == 'd') {
            str->overwriteAt(i, sead::SafeStringBase<char>("D"), -1);
        } else if (str->at(i) == 'e') {
            str->overwriteAt(i, sead::SafeStringBase<char>("E"), -1);
        } else if (str->at(i) == 'f') {
            str->overwriteAt(i, sead::SafeStringBase<char>("F"), -1);
        } else if (str->at(i) == 'g') {
            str->overwriteAt(i, sead::SafeStringBase<char>("G"), -1);
        } else if (str->at(i) == 'h') {
            str->overwriteAt(i, sead::SafeStringBase<char>("H"), -1);
        } else if (str->at(i) == 'i') {
            str->overwriteAt(i, sead::SafeStringBase<char>("I"), -1);
        } else if (str->at(i) == 'j') {
            str->overwriteAt(i, sead::SafeStringBase<char>("J"), -1);
        } else if (str->at(i) == 'k') {
            str->overwriteAt(i, sead::SafeStringBase<char>("K"), -1);
        } else if (str->at(i) == 'l') {
            str->overwriteAt(i, sead::SafeStringBase<char>("L"), -1);
        } else if (str->at(i) == 'm') {
            str->overwriteAt(i, sead::SafeStringBase<char>("M"), -1);
        } else if (str->at(i) == 'n') {
            str->overwriteAt(i, sead::SafeStringBase<char>("N"), -1);
        } else if (str->at(i) == 'o') {
            str->overwriteAt(i, sead::SafeStringBase<char>("O"), -1);
        } else if (str->at(i) == 'p') {
            str->overwriteAt(i, sead::SafeStringBase<char>("P"), -1);
        } else if (str->at(i) == 'q') {
            str->overwriteAt(i, sead::SafeStringBase<char>("Q"), -1);
        } else if (str->at(i) == 'r') {
            str->overwriteAt(i, sead::SafeStringBase<char>("R"), -1);
        } else if (str->at(i) == 's') {
            str->overwriteAt(i, sead::SafeStringBase<char>("S"), -1);
        } else if (str->at(i) == 't') {
            str->overwriteAt(i, sead::SafeStringBase<char>("T"), -1);
        } else if (str->at(i) == 'u') {
            str->overwriteAt(i, sead::SafeStringBase<char>("U"), -1);
        } else if (str->at(i) == 'v') {
            str->overwriteAt(i, sead::SafeStringBase<char>("V"), -1);
        } else if (str->at(i) == 'w') {
            str->overwriteAt(i, sead::SafeStringBase<char>("W"), -1);
        } else if (str->at(i) == 'x') {
            str->overwriteAt(i, sead::SafeStringBase<char>("X"), -1);
        } else if (str->at(i) == 'y') {
            str->overwriteAt(i, sead::SafeStringBase<char>("Y"), -1);
        } else if (str->at(i) == 'z') {
            str->overwriteAt(i, sead::SafeStringBase<char>("Z"), -1);
        }
    }
}
