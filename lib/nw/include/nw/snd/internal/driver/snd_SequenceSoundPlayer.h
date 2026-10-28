#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_BasicSoundPlayer.h"
#include "nw/snd/internal/driver/snd_DisposeCallback.h"
#include "nw/snd/internal/driver/snd_NoteOnCallback.h"
#include "nw/snd/internal/driver/snd_SequenceSoundLoader.h"
#include "nw/snd/internal/driver/snd_SequenceTrack.h"
#include "nw/snd/internal/driver/snd_SequenceTrackAllocator.h"
#include "nw/snd/internal/driver/snd_SoundThread.h"
#include "nw/snd/internal/driver/snd_SoundThread_PlayerCallback.h"
#include "nw/snd/internal/snd_BankFileReader.h"
#include "nw/snd/internal/snd_LoaderManager.h"
#include "nw/snd/internal/snd_MoveValue.h"
#include "nw/snd/internal/snd_WaveArchiveFileReader.h"

namespace nw {
namespace snd {
// what the user procedure command of a sequence gets (the struct name is from the reference symbols, the
// members are ours)
struct SequenceUserprocCallbackParam
{
    s16* localVariable;   // 0x0 of the player
    s16* globalVariable;  // 0x4
    s16* trackVariable;   // 0x8
    bool cmpFlag;          // 0xC the compare flag of the track (in and out)
};
ASSERT_SIZE(SequenceUserprocCallbackParam, 0x10);
typedef void (*SequenceUserprocCallback)(u16 procId, SequenceUserprocCallbackParam* pParam, void* pArg); // (name is ours)

namespace internal {
namespace driver {
// The player of a sequence sound in the sound thread: up to 16 tracks of a sequence with the banks and
// wave archives they play; the data is prepared by the sound or loaded by a SequenceSoundLoader (the class
// and function names are from the reference symbols, the members and the names of the unnamed functions
// are ours).
// RTTI N2nw3snd8internal6driver19SequenceSoundPlayerE @ 0x008D0B10
// vtable 0x00903468 (vptr 0x00903470), offset_to_top 0, 11 entries
// vtable 0x0090349C (vptr 0x009034A4), offset_to_top -68, 3 entries
// vtable 0x009034B0 (vptr 0x009034B8), offset_to_top -80, 5 entries
class SequenceSoundPlayer : public ::nw::snd::internal::driver::BasicSoundPlayer,
                            public ::nw::snd::internal::driver::DisposeCallback,
                            public ::nw::snd::internal::driver::SoundThread::PlayerCallback
{
public:
    static const int TRACK_COUNT = 16;
    static const int VARIABLE_COUNT = 16;
    static const int BANK_COUNT = SequenceSoundLoader::BANK_COUNT;

    // (names are ours)
    enum ResState : u8
    {
        RES_STATE_NONE = 0,
        RES_STATE_LOAD_REQUESTED = 1,
        RES_STATE_LOADING = 2,
        RES_STATE_PREPARED = 3,
    };
    // the unit of a skip (names are ours)
    enum OffsetType
    {
        OFFSET_TYPE_TICK = 0,
        OFFSET_TYPE_MILLISEC = 1,
    };

    // where to start (names are ours)
    struct StartInfo
    {
        s32 seqOffset;                // 0x0 in the sequence data
        u8 startOffsetType;           // 0x4 OffsetType
        s32 startOffset;              // 0x8
    };
    ASSERT_SIZE(StartInfo, 0xC);
    // the tracks and the note-on callback (names are ours)
    struct SetupArg
    {
        SequenceTrackAllocator* trackAllocator; // 0x0
        u32 allocTracks;                        // 0x4 a bit per track
        NoteOnCallback* callback;               // 0x8
    };
    ASSERT_SIZE(SetupArg, 0xC);
    // the prepared data (names are ours)
    struct PrepareArg
    {
        const void* seqFile;                    // 0x00
        const void* bankFiles[BANK_COUNT];      // 0x04
        const void* warcFiles[BANK_COUNT];      // 0x14
        bool warcIsIndividual[BANK_COUNT];      // 0x24
        s32 seqOffset;                          // 0x28
    };
    ASSERT_SIZE(PrepareArg, 0x2C);

    SequenceSoundPlayer(); // 0x004D38E8 | fefates:bytes [tier B]
    virtual ~SequenceSoundPlayer(); // 0x004D3A20 slot 0x00 | fefates:bytes

    virtual void Initialize(); // 0x004D26C0 slot 0x08 | fefates:bytes
    virtual void Finalize(); // 0x004D3614 slot 0x0C | fefates:bytes
    virtual void Start(); // 0x004D3148 slot 0x10
    virtual void Stop(); // 0x004D29E0 slot 0x14
    virtual void Pause(bool isPause); // 0x004D303C slot 0x18 | nintendogs:bytes-fuzzy

    // the sequence data or a bank goes away
    virtual void InvalidateData(const void* pStart, const void* pEnd); // 0x004D2B3C slot 0x1C
    // a channel of a track ends
    virtual void ChannelCallback(Channel* pChannel); // 0x004D2BF4 slot 0x20
    virtual void OnUpdateFrame(); // 0x004D3184 slot 0x24
    virtual void OnShutdown(); // 0x004D2EC0 slot 0x28

    // the global variables of all sequences to -1
    static void InitSequenceSoundPlayer(); // 0x004D2F50 | nintendogs:bytes [tier A]
    static s16 GetGlobalVariable(int index); // 0x004D2CBC (name is ours)
    static void SetGlobalVariable(int index, s16 value); // 0x004D2CD0 (name is ours)
    // the most ticks a skip parses in one frame
    static void SetSkipIntervalTick(int tick); // 0x004D2E08 (name is ours)
    // (a nop that falls into SetSkipIntervalTick; the game calls it; name is ours)
    static void SetSkipIntervalTickEntry(int tick); // 0x004D2E04

    void Setup(const SetupArg& arg); // 0x004D307C | fefates:bytes [tier B]
    void Prepare(const PrepareArg& arg); // 0x004D3520 | fefates:bytes [tier B]
    void RequestLoad(const StartInfo& startInfo, const SequenceSoundLoader::Arg& arg); // 0x004D2974 | fefates:bytes [tier B]
    void Update(); // 0x004D3188 | fefates:bytes [tier B]
    void Skip(OffsetType offsetType, int offset); // 0x004D2FF8 | nintendogs:bytes [tier A]
    // closes the tracks
    void FinishPlayer(); // 0x004D29E4 | nintendogs:callseq [tier A]

    SequenceTrack* GetPlayerTrack(int trackNo); // 0x004D2AFC | nintendogs:callseq-callee [tier A]
    // 0-15: the local variables, 16-31: the global variables
    s16* GetVariablePtr(int index); // 0x004D2B10 | nintendogs:callseq-callee [tier A]
    void SetLocalVariable(int index, s16 value); // 0x004D2C98 (name is ours)
    s16 GetLocalVariable(int index) const; // 0x00741680 (name is ours)
    void SetSeqUserprocCallback(SequenceUserprocCallback callback, void* pArg); // 0x004D2F7C (name is ours)
    void CallSequenceUserprocCallback(unsigned short procId, SequenceTrack* pTrack); // 0x004D2F88 | fefates:bytes [tier B]
    Channel* NoteOn(unsigned char bankIndex, const NoteOnInfo& info); // 0x004D3154 | fefates:bytes [tier B]

    void SetTrackMute(unsigned trackBitFlag, SeqMute mute); // 0x004D2A60 | nintendogs:bytes-fuzzy [tier A]
    void SetTrackSilence(unsigned long trackBitFlag, bool isSilence, int fadeTimes); // 0x004D2C10 | nintendogs:bytes-fuzzy [tier A]
    void SetTrackVolume(unsigned trackBitFlag, f32 volume); // 0x004D2BDC | nintendogs:bytes [tier A]
    void SetTrackPitch(unsigned trackBitFlag, f32 pitch); // 0x004D2AE4 | nintendogs:bytes [tier A]
    void SetTrackPan(unsigned trackBitFlag, f32 pan); // 0x004D29C8 | nintendogs:bytes [tier A]
    void SetTrackSurroundPan(unsigned trackBitFlag, f32 surroundPan); // 0x004D2E18 | nintendogs:bytes [tier A]
    void SetTrackLpfFreq(unsigned trackBitFlag, f32 lpfFreq); // 0x004D2BF8 | nintendogs:bytes [tier A]
    void SetTrackBiquadFilter(unsigned trackBitFlag, int type, f32 value); // 0x004D2E30 | nintendogs:bytes-fuzzy [tier A]
    void SetTrackPanRange(unsigned trackBitFlag, f32 panRange); // 0x004D2CA4 | nintendogs:bytes [tier A]
    // false if the bank is not there
    bool SetTrackBankIndex(unsigned int trackBitFlag, int bankIndex); // 0x004D2CE4 | fefates:bytes [tier B]
    void SetTrackTranspose(unsigned int trackBitFlag, signed char transpose); // 0x004D2D80 | fefates:bytes [tier B]
    void SetTrackVelocityRange(unsigned int trackBitFlag, unsigned char velocityRange); // 0x004D2ECC | fefates:bytes [tier B]
    // a parameter of the tracks of the bits
    // (ARMCC calls the f32 copy 0x007E792C and inlines the others, see the specialization below)
    template <typename T>
    void SetTrackParam(unsigned trackBitFlag, void (SequenceTrack::*pFunc)(T), T param)
    {
        for (int i = 0; i < TRACK_COUNT; ++i, trackBitFlag >>= 1) {
            if (trackBitFlag == 0) {
                break;
            }
            if (trackBitFlag & 1) {
                SequenceTrack* pTrack = GetPlayerTrack(i);
                if (pTrack != nullptr) {
                    (pTrack->*pFunc)(param);
                }
            }
        }
    }

    // (inline; name is ours) the track is closed and goes back to the allocator
    void CloseTrack(int trackNo)
    {
        SequenceTrack* pTrack = GetPlayerTrack(trackNo);
        if (pTrack == nullptr) {
            return;
        }
        pTrack->Close();
        m_pTrackAllocator->FreeTrack(m_pTracks[trackNo]);
        m_pTracks[trackNo] = nullptr;
    }
    // (inline; name is ours) ticks per millisecond
    f32 CalcTickPerMsec() const { return m_Timebase * m_Tempo * m_TempoRatio * (1.0f / 60000.0f); }

    void UpdateTick(); // 0x004D279C | mk7dlp:callseq [tier A]
    void SkipTick(); // 0x004D36F8 | fefates:bytes [tier B]

    bool m_IsReleasePriorityFix;                       // 0x05C
    u8 padding0x5D[3];                                 // 0x05D
    f32 m_PanRange;                                    // 0x060
    f32 m_TempoRatio;                                  // 0x064
    f32 m_TickFraction;                                // 0x068
    u32 m_SkipTickCounter;                             // 0x06C
    f32 m_SkipTimeCounter;                             // 0x070
    u8 m_Priority;                                     // 0x074
    u8 m_Timebase;                                     // 0x075 ticks per quarter note
    u16 m_Tempo;                                       // 0x076 quarter notes per minute
    MoveValue<u8, s16> m_SequenceVolume;               // 0x078 the volume command of the sequence
    u8 padding0x7E[2];                                 // 0x07E
    NoteOnCallback* m_pNoteOnCallback;                 // 0x080
    SequenceTrackAllocator* m_pTrackAllocator;         // 0x084
    SequenceUserprocCallback m_SeqUserprocCallback;    // 0x088
    void* m_pSeqUserprocCallbackArg;                   // 0x08C
    SequenceTrack* m_pTracks[TRACK_COUNT];             // 0x090
    s16 m_LocalVariable[VARIABLE_COUNT];              // 0x0D0
    u32 m_TickCounter;                                 // 0x0F0
    WaveArchiveFileReader m_WarcReaders[BANK_COUNT];   // 0x0F4
    BankFileReader m_BankReaders[BANK_COUNT];          // 0x134
    ResState m_ResState;                               // 0x164
    bool m_IsInitialized;                              // 0x165
    bool m_IsRegisteredPlayerCallback;                 // 0x166
    u8 padding0x167;                                   // 0x167
    StartInfo m_StartInfo;                             // 0x168
    LoaderManager<SequenceSoundLoader>* m_pLoaderManager; // 0x174
    SequenceSoundLoader* m_pLoader;                    // 0x178
    SequenceSoundLoader::Arg m_LoaderArg;              // 0x17C
};
ASSERT_SIZE(SequenceSoundPlayer, 0x1B0);

// the f32 copy is called (GCC would inline it and call the member function directly)
template <>
DECOMP_NOIPA void SequenceSoundPlayer::SetTrackParam<f32>(unsigned trackBitFlag, void (SequenceTrack::*pFunc)(f32), f32 param);
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
