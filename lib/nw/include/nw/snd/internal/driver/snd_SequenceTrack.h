#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_Channel.h"
#include "nw/snd/internal/snd_CurveLfoParam.h"
#include "nw/snd/internal/snd_MoveValue.h"
#include "nw/snd/snd_Global.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
class SequenceSoundPlayer;

// A track of a sequence: its position in the sequence data, the parameters the commands set and the
// channels of its notes (the class and function names are from the reference symbols, the members and
// the names of the setters are ours; the setters are named after the SetTrack* of SequenceSoundPlayer,
// whose member pointer table 0x008B61B4 points to them).
// RTTI N2nw3snd8internal6driver13SequenceTrackE @ 0x008D0A6C
// vtable 0x0090330C (vptr 0x00903314), offset_to_top 0, 3 entries
class SequenceTrack
{
public:
    // what Parse did (names are ours)
    enum ParseResult
    {
        PARSE_RESULT_CONTINUE = 0,
        PARSE_RESULT_FINISH = 1,
    };
    static const int VARIABLE_COUNT = 16;
    static const int LFO_COUNT = 4;
    // the most commands parsed in one tick
    static const int PARSE_COUNT_MAX = 10000; // (name is ours)
    static const int CALL_STACK_DEPTH = 10; // (name is ours)

    // a call or a loop of the parser (names are ours)
    struct CallStack
    {
        bool loopFlag;          // 0x0 a loop (not a call)
        u8 loopCount;           // 0x1 (0: endless)
        u8 padding[2];          // 0x2
        const u8* address;      // 0x4 where it goes back to
    };

    SequenceTrack(); // 0x004CC96C | fefates:bytes [tier B]
    virtual ~SequenceTrack(); // 0x004CCA8C slot 0x00 | fefates:bytes

    // the next command of the track
    virtual ParseResult Parse(bool doNoteOn) = 0; // slot 0x08

    void InitParam(); // 0x004CC7EC | fefates:bytes [tier B]
    void SetSeqData(const void* pBase, s32 offset); // 0x004CBBF0 | nintendogs:callseq-callee [tier A]
    void Open(); // 0x004CC388 | nintendogs:callseq-callee [tier A]
    void Close(); // 0x004CC3A4 | nintendogs:callseq-callee [tier A]
    // the commands of one tick; -1: the track ends
    int ParseNextTick(bool doNoteOn); // 0x004CBCAC | fefates:bytes [tier B]

    void UpdateChannelLength(); // 0x004CC30C | fefates:bytes [tier B]
    void UpdateChannelParam(); // 0x004CBEF8 | fefates:bytes [tier B]
    // release: the release of the envelope (-1: as it is)
    void ReleaseAllChannel(int release); // 0x004CBEA4 | nintendogs:callseq-callee [tier A]
    void FreeAllChannel(); // 0x004CBDDC | nintendogs:callseq-callee [tier A]
    void PauseAllChannel(bool isPause); // 0x004CBE2C | fefates:bytes [tier B]
    // a note; tieFlag: the channel of the last note goes on (NULL: no channel)
    Channel* NoteOn(int key, int velocity, s32 length, bool tieFlag); // 0x004CC408 | nintendogs:callgraph [tier A]
    static void ChannelCallbackFunc(Channel* pChannel, Channel::ChannelCallbackStatus status, u32 userData); // 0x004CC290 | fefates:bytes [tier B]
    s16* GetVariablePtr(int index); // 0x004CBE10 | nintendogs:callseq-callee [tier A]

    void SetMute(SeqMute mute); // 0x004CC720 | nintendogs:callseq-callee [tier A]
    void SetSilence(bool isSilence, int fadeTimes); // 0x004CBC00 | fefates:bytes [tier B]
    void SetVolume(f32 volume); // 0x004CC964 (name is ours)
    void SetPitch(f32 pitch); // 0x004CC7E4 (name is ours)
    void SetPan(f32 pan); // 0x004CC718 (name is ours)
    void SetSurroundPan(f32 surroundPan); // 0x004CBE24 (name is ours)
    void SetLpfFreq(f32 lpfFreq); // 0x004CBBE8 (name is ours)
    void SetBiquadFilter(int type, f32 value); // 0x004CBE80 (name is ours)
    void SetPanRange(f32 panRange); // 0x004CBC94 (name is ours)
    void SetBankIndex(int bankIndex); // 0x004CBC9C (name is ours)
    void SetTranspose(s8 transpose); // 0x004CBCA4 (name is ours)
    void SetVelocityRange(u8 velocityRange); // 0x004CBE9C (name is ours)
    void SetTrackVariable(int index, s16 value); // 0x004CBE8C (name is ours)

    u8 m_PlayerTrackNo;                 // 0x004
    bool m_OpenFlag;                    // 0x005
    u8 padding0x06[2];                  // 0x006
    f32 m_ExtVolume;                    // 0x008
    f32 m_ExtPitch;                     // 0x00C
    f32 m_ExtPan;                       // 0x010
    f32 m_ExtSurroundPan;               // 0x014
    f32 m_PanRange;                     // 0x018
    const u8* m_pSeqBase;               // 0x01C
    const u8* m_pCurrentAddr;           // 0x020
    bool m_CmpFlag;                     // 0x024 the result of the compare commands
    bool m_WaitByNoteLengthFlag;        // 0x025 a note waits for its length
    bool m_TieFlag;                     // 0x026
    bool m_MonophonicFlag;              // 0x027 a new note takes the channel of the last one
    CallStack m_CallStack[CALL_STACK_DEPTH]; // 0x028 the calls and loops of the parser
    u8 m_CallStackDepth;                // 0x078
    bool m_FrontBypassFlag;             // 0x079
    bool m_MuteFlag;                    // 0x07A
    bool m_SilenceFlag;                 // 0x07B
    s32 m_Wait;                         // 0x07C ticks until the next command
    bool m_NoteWaitFlag;                // 0x080 the next command waits for the end of the notes
    bool m_PortaFlag;                   // 0x081
    bool m_DamperFlag;                  // 0x082 the notes ignore the end of their length
    u8 m_BankIndex;                     // 0x083
    int m_PrgNo;                        // 0x084
    f32 m_SweepPitch;                   // 0x088
    MoveValue<u8, s16> m_Volume;        // 0x08C
    MoveValue<u8, s16> m_Volume2;       // 0x092
    MoveValue<s8, s16> m_Pan;           // 0x098
    MoveValue<s8, s16> m_SurroundPan;   // 0x09E
    MoveValue<s8, s16> m_PitchBend;     // 0x0A4
    u8 padding0xAA[2];                  // 0x0AA
    CurveLfoParam m_LfoParam[LFO_COUNT]; // 0x0AC
    u8 m_LfoTarget[LFO_COUNT];          // 0x0EC Channel::LfoTarget
    u8 m_VelocityRange;                 // 0x0F0
    u8 m_BendRange;                     // 0x0F1 semitones
    s8 m_InitPan;                       // 0x0F2
    u8 padding0xF3;                     // 0x0F3
    s8 m_Transpose;                     // 0x0F4
    u8 m_Priority;                      // 0x0F5
    u8 m_PortaKey;                      // 0x0F6
    u8 m_PortaTime;                     // 0x0F7
    u8 m_Attack;                        // 0x0F8 (above 127: from the bank)
    u8 m_Decay;                         // 0x0F9
    u8 m_Sustain;                       // 0x0FA
    u8 m_Release;                       // 0x0FB
    s16 m_Hold;                         // 0x0FC
    s8 m_BiquadType;                    // 0x0FE
    u8 m_MainSend;                      // 0x0FF
    u8 m_FxSend[2];                     // 0x100
    u8 padding0x102[2];                 // 0x102
    f32 m_LpfFreq;                      // 0x104
    f32 m_BiquadValue;                  // 0x108
    s16 m_TrackVariable[VARIABLE_COUNT]; // 0x10C
    SequenceSoundPlayer* m_pSequenceSoundPlayer; // 0x12C
    Channel* m_pChannelList;            // 0x130 linked over Channel::m_pNextLink
};
ASSERT_SIZE(SequenceTrack, 0x134);
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
