#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_SequenceTrack.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
class MmlSequenceTrack;
class SequenceSoundPlayer;

// Reads the MML commands of a sequence track (the class and function names are from the reference
// symbols; the names of the commands are ours, after what CommandProc does with them).
// RTTI N2nw3snd8internal6driver9MmlParserE @ 0x008D0B58
// vtable 0x009034FC (vptr 0x00903504), offset_to_top 0, 4 entries
class MmlParser
{
public:
    // how an argument is stored (names are ours)
    enum SeqArgType
    {
        SEQ_ARG_NONE = 0,
        SEQ_ARG_U8 = 1,
        SEQ_ARG_S16 = 2,       // big endian
        SEQ_ARG_VMIDI = 3,     // 7 bits per byte, the top bit: more
        SEQ_ARG_RANDOM = 4,    // two s16: from, to
        SEQ_ARG_VARIABLE = 5,  // the number of a variable
    };
    // the commands (names are ours)
    enum MmlCommand
    {
        MML_WAIT = 0x80,
        MML_PRG = 0x81,
        MML_OPEN_TRACK = 0x88,
        MML_JUMP = 0x89,
        MML_CALL = 0x8A,
        MML_RANDOM = 0xA0,          // the next argument is random
        MML_VARIABLE = 0xA1,        // the next argument is a variable
        MML_IF = 0xA2,              // the next command only if the compare flag is set
        MML_TIME = 0xA3,            // the next command has a second argument (s16)
        MML_TIME_RANDOM = 0xA4,
        MML_TIME_VARIABLE = 0xA5,
        MML_TIMEBASE = 0xB0,
        MML_ENV_HOLD = 0xB1,
        MML_MONOPHONIC = 0xB2,
        MML_VELOCITY_RANGE = 0xB3,
        MML_BIQUAD_TYPE = 0xB4,
        MML_BIQUAD_VALUE = 0xB5,
        MML_BANK_SELECT = 0xB6,
        MML_MOD_PHASE = 0xBD,
        MML_MOD_CURVE = 0xBE,
        MML_FRONT_BYPASS = 0xBF,
        MML_PAN = 0xC0,
        MML_VOLUME = 0xC1,
        MML_MAIN_VOLUME = 0xC2,
        MML_TRANSPOSE = 0xC3,
        MML_PITCH_BEND = 0xC4,
        MML_BEND_RANGE = 0xC5,
        MML_PRIO = 0xC6,
        MML_NOTE_WAIT = 0xC7,
        MML_TIE = 0xC8,
        MML_PORTA = 0xC9,
        MML_MOD_DEPTH = 0xCA,
        MML_MOD_SPEED = 0xCB,
        MML_MOD_TYPE = 0xCC,
        MML_MOD_RANGE = 0xCD,
        MML_PORTA_SW = 0xCE,
        MML_PORTA_TIME = 0xCF,
        MML_ATTACK = 0xD0,
        MML_DECAY = 0xD1,
        MML_SUSTAIN = 0xD2,
        MML_RELEASE = 0xD3,
        MML_LOOP_START = 0xD4,
        MML_VOLUME2 = 0xD5,
        MML_PRINTVAR = 0xD6,
        MML_SURROUND_PAN = 0xD7,
        MML_LPF_CUTOFF = 0xD8,
        MML_FXSEND_A = 0xD9,
        MML_FXSEND_B = 0xDA,
        MML_MAINSEND = 0xDB,
        MML_INIT_PAN = 0xDC,
        MML_MUTE = 0xDD,
        MML_DAMPER = 0xDF,
        MML_MOD_DELAY = 0xE0,
        MML_TEMPO = 0xE1,
        MML_SWEEP_PITCH = 0xE3,
        MML_MOD_PERIOD = 0xE4,
        MML_EX_COMMAND = 0xF0,
        MML_ENV_RESET = 0xFB,
        MML_LOOP_END = 0xFC,
        MML_RET = 0xFD,
        MML_ALLOC_TRACK = 0xFE,
        MML_FIN = 0xFF,
    };
    // the extended commands (the byte after MML_EX_COMMAND; CommandProc gets 0xF000 + it; names are ours)
    enum MmlExCommand
    {
        MML_EX_SETVAR = 0x80,
        MML_EX_ADDVAR = 0x81,
        MML_EX_SUBVAR = 0x82,
        MML_EX_MULVAR = 0x83,
        MML_EX_DIVVAR = 0x84,
        MML_EX_SHIFTVAR = 0x85,
        MML_EX_RANDVAR = 0x86,
        MML_EX_ANDVAR = 0x87,
        MML_EX_ORVAR = 0x88,
        MML_EX_XORVAR = 0x89,
        MML_EX_NOTVAR = 0x8A,
        MML_EX_MODVAR = 0x8B,
        MML_EX_CMP_EQ = 0x90,
        MML_EX_CMP_GE = 0x91,
        MML_EX_CMP_GT = 0x92,
        MML_EX_CMP_LE = 0x93,
        MML_EX_CMP_LT = 0x94,
        MML_EX_CMP_NE = 0x95,
        MML_EX_MOD2_CURVE = 0xA0,
        MML_EX_MOD2_PHASE = 0xA1,
        MML_EX_MOD2_DEPTH = 0xA2,
        MML_EX_MOD2_SPEED = 0xA3,
        MML_EX_MOD2_TYPE = 0xA4,
        MML_EX_MOD2_RANGE = 0xA5,
        MML_EX_MOD3_CURVE = 0xA6,
        MML_EX_MOD3_PHASE = 0xA7,
        MML_EX_MOD3_DEPTH = 0xA8,
        MML_EX_MOD3_SPEED = 0xA9,
        MML_EX_MOD3_TYPE = 0xAA,
        MML_EX_MOD3_RANGE = 0xAB,
        MML_EX_MOD4_CURVE = 0xAC,
        MML_EX_MOD4_PHASE = 0xAD,
        MML_EX_MOD4_DEPTH = 0xAE,
        MML_EX_MOD4_SPEED = 0xAF,
        MML_EX_MOD4_TYPE = 0xB0,
        MML_EX_MOD4_RANGE = 0xB1,
        MML_EX_USERPROC = 0xE0,
        MML_EX_MOD2_DELAY = 0xE1,
        MML_EX_MOD2_PERIOD = 0xE2,
        MML_EX_MOD3_DELAY = 0xE3,
        MML_EX_MOD3_PERIOD = 0xE4,
        MML_EX_MOD4_DELAY = 0xE5,
        MML_EX_MOD4_PERIOD = 0xE6,
    };

    // (inline)
    MmlParser() {}
    virtual ~MmlParser(); // 0x004D48F4 slot 0x00

    // a command with its arguments
    virtual void CommandProc(MmlSequenceTrack* pTrack, u32 command, s32 commandArg1, s32 commandArg2) const; // 0x00741818 slot 0x08 | nintendogs:callseq
    // a note
    virtual void NoteOnCommandProc(MmlSequenceTrack* pTrack, int key, int velocity, s32 length, bool tieFlag) const; // 0x0074257C slot 0x0C | nintendogs:bytes

    // the next command of the track
    SequenceTrack::ParseResult Parse(MmlSequenceTrack* pTrack, bool doNoteOn) const; // 0x007425B8 (name after MmlSequenceTrack::Parse)
    // the alloctrack command at the start of a sequence: the tracks to allocate; the offset behind it
    static u32 ParseAllocTrack(const void* pBase, u32 offset, u32* pAllocTracks); // 0x004D48B8 | nintendogs:bytes-fuzzy [tier A]
    s32 ReadArg(const u8** ppData, SequenceSoundPlayer* pPlayer, SequenceTrack* pTrack, SeqArgType argType) const; // 0x00742B34 | nintendogs:callseq [tier A]

    // whether printvar is on (the print is removed)
    static bool s_IsPrintVarEnabled; // (name is ours)
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
