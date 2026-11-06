#include "Sound/dSoundChordTracker.h"
#include "Sound/dSoundHaniwaMgr.h"
#include "Sound/dSoundInstrument.h"
#include "Sound/dSoundMuInfo.h"
#include "Sound/dSoundRoomMusicPlayer.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include "nw/snd/snd_SequenceSoundHandle.h"

namespace {
const u32 DEFAULT_VALUE = 1000000;
const u32 END_TICK = 0x7FFFFFFF;
const u32 CONTROL_TRACK = 15;
const u8 VARIABLE_CHORD_KIND = 32;
const u8 VARIABLE_CHORD_ROOT = 33;
const u32 TICK_OFFSET = 12;
const u32 MAX_FRAMES = 120;
const u32 ACTIVE_FRAMES = 30;
const u32 QUIET_FRAMES = 90;

// the notes of the chords above the root
// 0x0083EDC8
const u16 s_ChordMajor[] = {0, 4, 7, 12};
// 0x0083EDD0
const u16 s_ChordSus4[] = {0, 5, 7, 12};
// 0x0083EDD8
const u16 s_ChordAugmented[] = {0, 4, 8, 12};
// 0x0083EDE0
const u16 s_ChordSixth[] = {0, 4, 9, 12};
// 0x0083EDE8
const u16 s_ChordSeventh[] = {0, 4, 7, 10, 12};
// 0x0083EDF2
const u16 s_ChordAugmentedSeventh[] = {0, 4, 8, 10, 12};
// 0x0083EDFC
const u16 s_ChordMajorSeventh[] = {0, 4, 7, 11, 12};
// 0x0083EE06
const u16 s_ChordAugmentedMajorSeventh[] = {0, 4, 8, 11, 12};
// 0x0083EE10
const u16 s_ChordMinor[] = {0, 3, 7, 12};
// 0x0083EE18
const u16 s_ChordDiminished[] = {0, 3, 6, 12};
// 0x0083EE20
const u16 s_ChordMinorFlatSixth[] = {0, 3, 8, 12};
// 0x0083EE28
const u16 s_ChordMinorSixth[] = {0, 3, 9, 12};
// 0x0083EE30
const u16 s_ChordMinorSeventh[] = {0, 3, 7, 10, 12};
// 0x0083EE3A
const u16 s_ChordHalfDiminished[] = {0, 3, 6, 10, 12};
// 0x0083EE44
const u16 s_ChordMinorMajorSeventh[] = {0, 3, 7, 11, 12};
// 0x0083EE4E
const u16 s_ChordDiminishedMajorSeventh[] = {0, 3, 6, 11, 12};
// 0x0083EE58
const u16 s_ChordDiminishedSeventh[] = {0, 3, 6, 9, 12};
} // namespace

// 0x0094E578
const u16* SoundChordTracker::s_Chords[3][10] = {
    {NULL, s_ChordMajor, s_ChordSus4, s_ChordAugmented, s_ChordSixth, s_ChordSeventh, s_ChordAugmentedSeventh,
     s_ChordMajorSeventh, s_ChordAugmentedMajorSeventh, NULL},
    {NULL, s_ChordMinor, s_ChordDiminished, s_ChordMinorFlatSixth, s_ChordMinorSixth, s_ChordMinorSeventh,
     s_ChordHalfDiminished, s_ChordMinorMajorSeventh, s_ChordDiminishedMajorSeventh, NULL},
    {NULL, s_ChordDiminishedSeventh, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
};
// 0x0083EE62
const SoundChordTracker::ChordKind SoundChordTracker::s_ProgressionKinds[PROGRESSION_NUM] = {
    {0, 1}, {0, 1}, {1, 1}, {1, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1},
};
// 0x0083EE82
const u16 SoundChordTracker::s_ProgressionRoots[PROGRESSION_NUM] = {2, 9, 11, 6, 7, 2, 7, 9};

// 0x002A564C
SoundChordTracker::SoundChordTracker()
    : m_pPlayer(NULL), m_UserNum(0), m_MinValue(0), m_ChordGroup(0), m_ChordEntry(1), m_ChordRoot(0), m_IsChordSet(false),
      m_IsParsed(false), m_Frames(0), m_PlayCount(0), m_FramesSincePlay(0), m_IsCounting(false), m_IsActive(false),
      m_ActiveFrames(0), m_LastBeat(0), m_BeatCount(0)
{
    for (u32 i = 0; i < USER_NUM; i++) {
        m_pUsers[i] = NULL;
    }
}

// 0x002A5150
void SoundChordTracker::Initialize()
{
    m_pPlayer = SoundRoomMusicPlayer::s_pInstance;
}

// 0x002A5164
void SoundChordTracker::Start(u32 soundId)
{
    SoundSeqParser* parser = SoundSeqParser::s_pInstance;
    if (parser == NULL) {
        return;
    }
    parser->Load(soundId);
    parser->ParseVariable(m_KindEntries, CONTROL_TRACK, VARIABLE_CHORD_KIND, ENTRY_NUM);
    parser->ParseVariable(m_RootEntries, CONTROL_TRACK, VARIABLE_CHORD_ROOT, ENTRY_NUM);
    m_IsParsed = true;
}

// 0x002A51C8
void SoundChordTracker::OnUserPlayed()
{
    m_FramesSincePlay = 0;
    m_PlayCount++;
    if (m_IsActive == 1 && m_ActiveFrames == 0) {
        m_ActiveFrames = MAX_FRAMES;
    }
}

// 0x002A51FC
void SoundChordTracker::Unregister(u32 index)
{
    if (index < USER_NUM && m_pUsers[index] != NULL) {
        m_pUsers[index] = NULL;
        m_UserNum--;
    }
}

// 0x002A522C
void SoundChordTracker::UpdateCounters()
{
    u32 framesSincePlay = ++m_FramesSincePlay;
    u32 frames = ++m_Frames;
    if (m_IsCounting) {
        if (framesSincePlay >= MAX_FRAMES) {
            m_IsCounting = false;
            m_IsActive = false;
            m_ActiveFrames = 0;
            m_Frames = 0;
            m_PlayCount = 0;
        } else if (framesSincePlay >= ACTIVE_FRAMES) {
            m_IsActive = true;
        }
    } else if (framesSincePlay >= QUIET_FRAMES) {
        m_Frames = 0;
        m_PlayCount = 0;
    } else if (frames >= MAX_FRAMES && framesSincePlay < ACTIVE_FRAMES) {
        m_IsCounting = true;
        m_FramesSincePlay = 0;
    }
    if (m_IsActive && m_ActiveFrames != 0) {
        m_ActiveFrames--;
        if (m_ActiveFrames == 0) {
            m_IsActive = false;
        }
    }
}

// 0x002A52E0
void SoundChordTracker::Stop()
{
    m_LastBeat = m_pPlayer->m_Conductor.GetBeat();
    m_BeatCount = 0;
    m_IsParsed = false;
}

// 0x002A5314
void SoundChordTracker::Update()
{
    m_IsChordSet = false;
    m_MinValue = 0;
    u32 seen = 0;
    for (u32 i = 0; i < USER_NUM; i++) {
        if (m_pUsers[i] == NULL) {
            continue;
        }
        s32 value = m_pUsers[i]->m_PhraseNotes;
        if (value != 0 && (m_MinValue == 0 || m_MinValue > value)) {
            m_MinValue = value;
        }
        seen++;
        if (m_UserNum <= seen) {
            break;
        }
    }
    UpdateCounters();
    if (!m_IsParsed && m_pPlayer != NULL) {
        s16 beat = m_pPlayer->m_Conductor.GetBeat();
        if (m_LastBeat != beat) {
            m_LastBeat = beat;
            m_BeatCount++;
        }
    }
}

// 0x002A53CC
void SoundChordTracker::GetChord(const u16** notes, u16* root)
{
    if (!m_IsChordSet) {
        if (m_pPlayer != NULL) {
            if (m_IsParsed) {
                nw::snd::SoundHandle* handle = &m_pPlayer->m_Handle;
                if (handle != NULL && handle->m_pSound != NULL) {
                    nw::snd::SequenceSoundHandle sequence(handle);
                    u32 tick = sequence.m_pSound != NULL ? sequence.m_pSound->GetTick() : 0;
                    u32 position = SoundMuInfo::s_pInstance->WrapPosition(m_pPlayer->m_Song, tick) + TICK_OFFSET;

                    s32 kind = DEFAULT_VALUE;
                    for (s32 i = 0; i < ENTRY_NUM - 1; i++) {
                        u32 next = m_KindEntries[i + 1].tick;
                        if (next == END_TICK || next > position) {
                            kind = m_KindEntries[i].tempo;
                            break;
                        }
                    }
                    if (kind != static_cast<s32>(DEFAULT_VALUE)) {
                        s32 group = kind / 10;
                        s32 entry = kind % 10;
                        if (group < 3 && entry < 10 && entry != 0 && s_Chords[group][entry] != NULL) {
                            m_ChordGroup = group;
                            m_ChordEntry = entry;
                        }
                    }

                    s32 chordRoot = DEFAULT_VALUE;
                    for (s32 i = 0; i < ENTRY_NUM - 1; i++) {
                        u32 next = m_RootEntries[i + 1].tick;
                        if (next == END_TICK || next > position) {
                            chordRoot = m_RootEntries[i].tempo;
                            break;
                        }
                    }
                    if (chordRoot != static_cast<s32>(DEFAULT_VALUE)) {
                        m_ChordRoot = chordRoot % 12;
                    }
                }
            } else {
                s32 index = static_cast<s16>((m_BeatCount % 32) / 4);
                if (index < 0 || index >= PROGRESSION_NUM) {
                    index = 0;
                }
                m_ChordRoot = s_ProgressionRoots[index];
                m_ChordGroup = s_ProgressionKinds[index].group;
                m_ChordEntry = s_ProgressionKinds[index].entry;
            }
        }
        m_IsChordSet = true;
    }
    *notes = s_Chords[m_ChordGroup][m_ChordEntry];
    *root = m_ChordRoot;
}

// 0x002A55F8
u32 SoundChordTracker::Register(SoundInstrument* user)
{
    if (m_UserNum > USER_NUM) {
        return USER_NUM + 1;
    }
    for (u32 i = 0; i < USER_NUM; i++) {
        if (m_pUsers[i] == NULL) {
            m_pUsers[i] = user;
            m_UserNum++;
            return i;
        }
    }
    return 0;
}

// 0x0071CD1C
bool SoundChordTracker::IsSessionActive() const
{
    SoundHaniwaMgr* mgr = SoundHaniwaMgr::s_pInstance;
    if (mgr == NULL) {
        return m_IsActive;
    }
    if (m_IsActive) {
        return true;
    }
    return m_IsCounting && mgr->m_Level >= 4;
}
