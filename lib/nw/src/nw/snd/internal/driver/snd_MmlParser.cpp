#include "nw/snd/internal/driver/snd_MmlParser.h"

#include "nw/snd/internal/Util/Util_Api.h"
#include "nw/snd/internal/driver/snd_MmlSequenceTrack.h"
#include "nw/snd/internal/driver/snd_SequenceSoundPlayer.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
namespace {
// the parameter steps of the commands
const f32 MOD_DEPTH_STEP = 1.0f / 128.0f;
const f32 MOD_SPEED_STEP = 0.390625f;
const f32 MOD_PERIOD_TO_SPEED = 100.0f;
const int MOD_DELAY_STEP = 5;
const f32 SWEEP_PITCH_STEP = 1.0f / 64.0f;
const f32 LPF_STEP = 1.0f / 64.0f;
const f32 BIQUAD_VALUE_STEP = 1.0f / 127.0f;
const int TEMPO_MAX = 0x3FF;
// the variables: 0-15 local, 16-31 global (player), 32-47 of the track
const int PLAYER_VARIABLE_COUNT = 32;
const int TRACK_VARIABLE_END = 48;

// (inline) the next byte
u8 ReadByte(const u8** ppData)
{
    return *(*ppData)++;
}
// (inline) the next 24 bits (big endian)
u32 Read24(const u8** ppData)
{
    u32 value = ReadByte(ppData);
    value = (value << 8) | ReadByte(ppData);
    value = (value << 8) | ReadByte(ppData);
    return value;
}
// (inline) a variable of the player or the track (NULL: none)
s16* GetVariablePtr(SequenceSoundPlayer* pPlayer, SequenceTrack* pTrack, int varNo)
{
    if (varNo < PLAYER_VARIABLE_COUNT) {
        return pPlayer->GetVariablePtr(varNo);
    }
    if (varNo < TRACK_VARIABLE_END) {
        return pTrack->GetVariablePtr(varNo - PLAYER_VARIABLE_COUNT);
    }
    return nullptr;
}
} // namespace

// 0x0097F674 (name is ours)
bool nw::snd::internal::driver::MmlParser::s_IsPrintVarEnabled = false;

// 0x00741818 slot 0x08 | nintendogs:callseq
void nw::snd::internal::driver::MmlParser::CommandProc(MmlSequenceTrack* pTrack, u32 command, s32 commandArg1, s32 commandArg2) const
{
    SequenceSoundPlayer* pPlayer = pTrack->m_pSequenceSoundPlayer;
    const u8 arg1U8 = static_cast<u8>(commandArg1);

    if (command <= 0xFF) {
        switch (command) {
        case MML_PRG:
            if (commandArg1 < 0x10000) {
                pTrack->m_PrgNo = static_cast<u16>(commandArg1);
            }
            break;
        case MML_OPEN_TRACK: {
            SequenceTrack* pNewTrack = pPlayer->GetPlayerTrack(commandArg1);
            if (pNewTrack != nullptr && pNewTrack != pTrack) {
                pNewTrack->Close();
                pNewTrack->SetSeqData(pTrack->m_pSeqBase, commandArg2);
                pNewTrack->Open();
            }
            break;
        }
        case MML_JUMP:
            pTrack->m_pCurrentAddr = pTrack->m_pSeqBase + commandArg1;
            break;
        case MML_CALL:
            if (pTrack->m_CallStackDepth < SequenceTrack::CALL_STACK_DEPTH) {
                SequenceTrack::CallStack& callStack = pTrack->m_CallStack[pTrack->m_CallStackDepth];
                callStack.address = pTrack->m_pCurrentAddr;
                callStack.loopFlag = false;
                ++pTrack->m_CallStackDepth;
                pTrack->m_pCurrentAddr = pTrack->m_pSeqBase + commandArg1;
            }
            break;
        case MML_TIMEBASE:
            pPlayer->m_Timebase = arg1U8;
            break;
        case MML_ENV_HOLD:
            pTrack->m_Hold = arg1U8;
            break;
        case MML_MONOPHONIC:
            pTrack->m_MonophonicFlag = commandArg1 != 0;
            if (pTrack->m_MonophonicFlag) {
                pTrack->ReleaseAllChannel(-1);
                pTrack->FreeAllChannel();
            }
            break;
        case MML_VELOCITY_RANGE:
            pTrack->m_VelocityRange = arg1U8;
            break;
        case MML_BIQUAD_TYPE:
            pTrack->m_BiquadType = static_cast<s8>(commandArg1);
            break;
        case MML_BIQUAD_VALUE:
            pTrack->m_BiquadValue = commandArg1 * BIQUAD_VALUE_STEP;
            break;
        case MML_BANK_SELECT:
            pTrack->m_BankIndex = arg1U8;
            break;
        case MML_MOD_PHASE:
            pTrack->m_LfoParam[0].phase = arg1U8;
            break;
        case MML_MOD_CURVE:
            pTrack->m_LfoParam[0].curve = arg1U8;
            break;
        case MML_FRONT_BYPASS:
            pTrack->m_FrontBypassFlag = commandArg1 != 0;
            break;
        case MML_PAN:
            pTrack->m_Pan.SetTarget(static_cast<s8>(commandArg1 - 64), static_cast<s16>(commandArg2));
            break;
        case MML_VOLUME:
            pTrack->m_Volume.SetTarget(arg1U8, static_cast<s16>(commandArg2));
            break;
        case MML_MAIN_VOLUME:
            pPlayer->m_SequenceVolume.SetTarget(arg1U8, static_cast<s16>(commandArg2));
            break;
        case MML_TRANSPOSE:
            pTrack->m_Transpose = static_cast<s8>(commandArg1);
            break;
        case MML_PITCH_BEND:
            pTrack->m_PitchBend.SetTarget(static_cast<s8>(commandArg1), static_cast<s16>(commandArg2));
            break;
        case MML_BEND_RANGE:
            pTrack->m_BendRange = arg1U8;
            break;
        case MML_PRIO:
            pTrack->m_Priority = arg1U8;
            break;
        case MML_NOTE_WAIT:
            pTrack->m_WaitByNoteLengthFlag = commandArg1 != 0;
            break;
        case MML_TIE:
            pTrack->m_TieFlag = commandArg1 != 0;
            pTrack->ReleaseAllChannel(-1);
            pTrack->FreeAllChannel();
            break;
        case MML_PORTA:
            pTrack->m_PortaKey = pTrack->m_Transpose + commandArg1;
            pTrack->m_PortaFlag = true;
            break;
        case MML_MOD_DEPTH:
            pTrack->m_LfoParam[0].depth = arg1U8 * MOD_DEPTH_STEP;
            break;
        case MML_MOD_SPEED:
            pTrack->m_LfoParam[0].speed = arg1U8 * MOD_SPEED_STEP;
            break;
        case MML_MOD_TYPE:
            pTrack->m_LfoTarget[0] = arg1U8;
            break;
        case MML_MOD_RANGE:
            pTrack->m_LfoParam[0].range = arg1U8;
            break;
        case MML_PORTA_SW:
            pTrack->m_PortaFlag = commandArg1 != 0;
            break;
        case MML_PORTA_TIME:
            pTrack->m_PortaTime = arg1U8;
            break;
        case MML_ATTACK:
            pTrack->m_Attack = arg1U8;
            break;
        case MML_DECAY:
            pTrack->m_Decay = arg1U8;
            break;
        case MML_SUSTAIN:
            pTrack->m_Sustain = arg1U8;
            break;
        case MML_RELEASE:
            pTrack->m_Release = arg1U8;
            break;
        case MML_LOOP_START:
            if (pTrack->m_CallStackDepth < SequenceTrack::CALL_STACK_DEPTH) {
                SequenceTrack::CallStack& callStack = pTrack->m_CallStack[pTrack->m_CallStackDepth];
                callStack.address = pTrack->m_pCurrentAddr;
                callStack.loopCount = arg1U8;
                callStack.loopFlag = true;
                ++pTrack->m_CallStackDepth;
            }
            break;
        case MML_VOLUME2:
            pTrack->m_Volume2.SetTarget(arg1U8, static_cast<s16>(commandArg2));
            break;
        case MML_PRINTVAR:
            if (s_IsPrintVarEnabled) {
                GetVariablePtr(pPlayer, pTrack, commandArg1);
            }
            break;
        case MML_SURROUND_PAN:
            pTrack->m_SurroundPan.SetTarget(static_cast<s8>(commandArg1), static_cast<s16>(commandArg2));
            break;
        case MML_LPF_CUTOFF:
            pTrack->m_LpfFreq = (commandArg1 - 64) * LPF_STEP;
            break;
        case MML_FXSEND_A:
            pTrack->m_FxSend[0] = arg1U8;
            break;
        case MML_FXSEND_B:
            pTrack->m_FxSend[1] = arg1U8;
            break;
        case MML_MAINSEND:
            pTrack->m_MainSend = arg1U8;
            break;
        case MML_INIT_PAN:
            pTrack->m_InitPan = commandArg1 - 64;
            break;
        case MML_MUTE:
            pTrack->SetMute(static_cast<SeqMute>(arg1U8));
            break;
        case MML_DAMPER:
            pTrack->m_DamperFlag = arg1U8 >= 64;
            break;
        case MML_MOD_DELAY:
            pTrack->m_LfoParam[0].delay = commandArg1 * MOD_DELAY_STEP;
            break;
        case MML_TEMPO: {
            int tempo = commandArg1;
            if (tempo > TEMPO_MAX) {
                tempo = TEMPO_MAX;
            } else if (tempo < 0) {
                tempo = 0;
            }
            pPlayer->m_Tempo = tempo;
            break;
        }
        case MML_SWEEP_PITCH:
            pTrack->m_SweepPitch = commandArg1 * SWEEP_PITCH_STEP;
            break;
        case MML_MOD_PERIOD: {
            const s16 period = static_cast<s16>(commandArg1);
            pTrack->m_LfoParam[0].speed = period == 0 ? 0.0f : MOD_PERIOD_TO_SPEED / period;
            break;
        }
        case MML_ENV_RESET:
            pTrack->m_Attack = 255;
            pTrack->m_Decay = 255;
            pTrack->m_Sustain = 255;
            pTrack->m_Release = 255;
            pTrack->m_Hold = 255;
            break;
        case MML_LOOP_END: {
            if (pTrack->m_CallStackDepth == 0) {
                break;
            }
            SequenceTrack::CallStack& callStack = pTrack->m_CallStack[pTrack->m_CallStackDepth - 1];
            if (!callStack.loopFlag) {
                break;
            }
            u8 loopCount = callStack.loopCount;
            if (loopCount != 0) {
                --loopCount;
                if (loopCount == 0) {
                    --pTrack->m_CallStackDepth;
                    break;
                }
            }
            callStack.loopCount = loopCount;
            pTrack->m_pCurrentAddr = callStack.address;
            break;
        }
        case MML_RET:
            // the loops inside the call are left
            while (pTrack->m_CallStackDepth != 0) {
                --pTrack->m_CallStackDepth;
                SequenceTrack::CallStack& callStack = pTrack->m_CallStack[pTrack->m_CallStackDepth];
                if (!callStack.loopFlag) {
                    pTrack->m_pCurrentAddr = callStack.address;
                    break;
                }
            }
            break;
        default:
            break;
        }
        return;
    }

    if (command >= 0x10000) {
        return;
    }
    // the extended commands
    s16* pVariable = nullptr;
    const u8 exCommand = command & 0xFF;
    if ((exCommand & 0xF0) == 0x80 || (exCommand & 0xF0) == 0x90) {
        pVariable = GetVariablePtr(pPlayer, pTrack, commandArg1);
        if (pVariable == nullptr) {
            return;
        }
    }
    switch (exCommand) {
    case MML_EX_SETVAR:
        *pVariable = commandArg2;
        break;
    case MML_EX_ADDVAR:
        *pVariable += commandArg2;
        break;
    case MML_EX_SUBVAR:
        *pVariable -= commandArg2;
        break;
    case MML_EX_MULVAR:
        *pVariable *= commandArg2;
        break;
    case MML_EX_DIVVAR:
        if (commandArg2 != 0) {
            *pVariable /= commandArg2;
        }
        break;
    case MML_EX_SHIFTVAR:
        if (commandArg2 >= 0) {
            *pVariable <<= commandArg2;
        } else {
            *pVariable >>= -commandArg2;
        }
        break;
    case MML_EX_RANDVAR: {
        bool isMinus = false;
        s16 range = commandArg2;
        if (range < 0) {
            isMinus = true;
            range = -range;
        }
        s32 random = (Util::CalcRandom() * (range + 1)) >> 16;
        if (isMinus) {
            random = -random;
        }
        *pVariable = random;
        break;
    }
    case MML_EX_ANDVAR:
        *pVariable &= commandArg2;
        break;
    case MML_EX_ORVAR:
        *pVariable |= commandArg2;
        break;
    case MML_EX_XORVAR:
        *pVariable ^= commandArg2;
        break;
    case MML_EX_NOTVAR:
        *pVariable = ~commandArg2;
        break;
    case MML_EX_MODVAR:
        if (commandArg2 != 0) {
            *pVariable %= commandArg2;
        }
        break;
    case MML_EX_CMP_EQ:
        pTrack->m_CmpFlag = *pVariable == commandArg2;
        break;
    case MML_EX_CMP_GE:
        pTrack->m_CmpFlag = *pVariable >= commandArg2;
        break;
    case MML_EX_CMP_GT:
        pTrack->m_CmpFlag = *pVariable > commandArg2;
        break;
    case MML_EX_CMP_LE:
        pTrack->m_CmpFlag = *pVariable <= commandArg2;
        break;
    case MML_EX_CMP_LT:
        pTrack->m_CmpFlag = *pVariable < commandArg2;
        break;
    case MML_EX_CMP_NE:
        pTrack->m_CmpFlag = *pVariable != commandArg2;
        break;
    case MML_EX_MOD2_CURVE:
        pTrack->m_LfoParam[1].curve = arg1U8;
        break;
    case MML_EX_MOD2_PHASE:
        pTrack->m_LfoParam[1].phase = arg1U8;
        break;
    case MML_EX_MOD2_DEPTH:
        pTrack->m_LfoParam[1].depth = arg1U8 * MOD_DEPTH_STEP;
        break;
    case MML_EX_MOD2_SPEED:
        pTrack->m_LfoParam[1].speed = arg1U8 * MOD_SPEED_STEP;
        break;
    case MML_EX_MOD2_TYPE:
        pTrack->m_LfoTarget[1] = arg1U8;
        break;
    case MML_EX_MOD2_RANGE:
        pTrack->m_LfoParam[1].range = arg1U8;
        break;
    case MML_EX_MOD3_CURVE:
        pTrack->m_LfoParam[2].curve = arg1U8;
        break;
    case MML_EX_MOD3_PHASE:
        pTrack->m_LfoParam[2].phase = arg1U8;
        break;
    case MML_EX_MOD3_DEPTH:
        pTrack->m_LfoParam[2].depth = arg1U8 * MOD_DEPTH_STEP;
        break;
    case MML_EX_MOD3_SPEED:
        pTrack->m_LfoParam[2].speed = arg1U8 * MOD_SPEED_STEP;
        break;
    case MML_EX_MOD3_TYPE:
        pTrack->m_LfoTarget[2] = arg1U8;
        break;
    case MML_EX_MOD3_RANGE:
        pTrack->m_LfoParam[2].range = arg1U8;
        break;
    case MML_EX_MOD4_CURVE:
        pTrack->m_LfoParam[3].curve = arg1U8;
        break;
    case MML_EX_MOD4_PHASE:
        pTrack->m_LfoParam[3].phase = arg1U8;
        break;
    case MML_EX_MOD4_DEPTH:
        pTrack->m_LfoParam[3].depth = arg1U8 * MOD_DEPTH_STEP;
        break;
    case MML_EX_MOD4_SPEED:
        pTrack->m_LfoParam[3].speed = arg1U8 * MOD_SPEED_STEP;
        break;
    case MML_EX_MOD4_TYPE:
        pTrack->m_LfoTarget[3] = arg1U8;
        break;
    case MML_EX_MOD4_RANGE:
        pTrack->m_LfoParam[3].range = arg1U8;
        break;
    case MML_EX_USERPROC:
        pPlayer->CallSequenceUserprocCallback(static_cast<u16>(commandArg1), pTrack);
        break;
    case MML_EX_MOD2_DELAY:
        pTrack->m_LfoParam[1].delay = commandArg1 * MOD_DELAY_STEP;
        break;
    case MML_EX_MOD2_PERIOD: {
        const s16 period = static_cast<s16>(commandArg1);
        pTrack->m_LfoParam[1].speed = period == 0 ? 0.0f : MOD_PERIOD_TO_SPEED / period;
        break;
    }
    case MML_EX_MOD3_DELAY:
        pTrack->m_LfoParam[2].delay = commandArg1 * MOD_DELAY_STEP;
        break;
    case MML_EX_MOD3_PERIOD: {
        const s16 period = static_cast<s16>(commandArg1);
        pTrack->m_LfoParam[2].speed = period == 0 ? 0.0f : MOD_PERIOD_TO_SPEED / period;
        break;
    }
    case MML_EX_MOD4_DELAY:
        pTrack->m_LfoParam[3].delay = commandArg1 * MOD_DELAY_STEP;
        break;
    case MML_EX_MOD4_PERIOD: {
        const s16 period = static_cast<s16>(commandArg1);
        pTrack->m_LfoParam[3].speed = period == 0 ? 0.0f : MOD_PERIOD_TO_SPEED / period;
        break;
    }
    default:
        break;
    }
}

// 0x0074257C slot 0x0C | nintendogs:bytes
void nw::snd::internal::driver::MmlParser::NoteOnCommandProc(MmlSequenceTrack* pTrack, int key, int velocity, s32 length, bool tieFlag) const
{
    pTrack->NoteOn(key, velocity, length, tieFlag);
}

// 0x007425B8 (name after MmlSequenceTrack::Parse)
nw::snd::internal::driver::SequenceTrack::ParseResult nw::snd::internal::driver::MmlParser::Parse(MmlSequenceTrack* pTrack, bool doNoteOn) const
{
    SequenceSoundPlayer* pPlayer = pTrack->m_pSequenceSoundPlayer;
    const u8** ppData = &pTrack->m_pCurrentAddr;
    SeqArgType argType = SEQ_ARG_NONE;
    SeqArgType argType2 = SEQ_ARG_NONE;
    bool useArgType = false;
    bool doExecCommand = true;

    u32 command = ReadByte(ppData);
    // the prefixes
    if (command == MML_IF) {
        command = ReadByte(ppData);
        doExecCommand = pTrack->m_CmpFlag != false;
    }
    if (command == MML_TIME) {
        argType2 = SEQ_ARG_S16;
        command = ReadByte(ppData);
    } else if (command == MML_TIME_RANDOM) {
        argType2 = SEQ_ARG_RANDOM;
        command = ReadByte(ppData);
    } else if (command == MML_TIME_VARIABLE) {
        argType2 = SEQ_ARG_VARIABLE;
        command = ReadByte(ppData);
    }
    if (command == MML_RANDOM) {
        command = ReadByte(ppData);
        argType = SEQ_ARG_RANDOM;
        useArgType = true;
    } else if (command == MML_VARIABLE) {
        command = ReadByte(ppData);
        argType = SEQ_ARG_VARIABLE;
        useArgType = true;
    }

    if (!(command & 0x80)) {
        // a note: key, velocity, length
        const u8 velocity = ReadByte(ppData);
        const s32 length = ReadArg(ppData, pPlayer, pTrack, useArgType ? argType : SEQ_ARG_VMIDI);
        int key = command + pTrack->m_Transpose;
        if (!doExecCommand) {
            return SequenceTrack::PARSE_RESULT_CONTINUE;
        }
        if (key > 127) {
            key = 127;
        } else if (key < 0) {
            key = 0;
        }
        if (!pTrack->m_MuteFlag && doNoteOn) {
            NoteOnCommandProc(pTrack, key, velocity, length > 0 ? length : -1, pTrack->m_TieFlag);
        }
        if (pTrack->m_WaitByNoteLengthFlag) {
            pTrack->m_Wait = length;
            if (length == 0) {
                pTrack->m_NoteWaitFlag = true;
            }
        }
        return SequenceTrack::PARSE_RESULT_CONTINUE;
    }

    s32 commandArg1 = 0;
    s32 commandArg2 = 0;
    switch (command & 0xF0) {
    case 0x80:
        switch (command) {
        case MML_WAIT: {
            const s32 wait = ReadArg(ppData, pPlayer, pTrack, useArgType ? argType : SEQ_ARG_VMIDI);
            if (doExecCommand) {
                pTrack->m_Wait = wait;
            }
            break;
        }
        case MML_PRG:
            commandArg1 = ReadArg(ppData, pPlayer, pTrack, useArgType ? argType : SEQ_ARG_VMIDI);
            if (doExecCommand) {
                CommandProc(pTrack, command, commandArg1, commandArg2);
            }
            break;
        case MML_OPEN_TRACK: {
            const u8 trackNo = ReadByte(ppData);
            const u32 offset = Read24(ppData);
            if (doExecCommand) {
                CommandProc(pTrack, command, trackNo, offset);
            }
            break;
        }
        case MML_JUMP:
        case MML_CALL:
            commandArg1 = Read24(ppData);
            if (doExecCommand) {
                CommandProc(pTrack, command, commandArg1, commandArg2);
            }
            break;
        default:
            break;
        }
        break;
    case 0x90:
        if (doExecCommand) {
            CommandProc(pTrack, command, commandArg1, commandArg2);
        }
        break;
    case 0xB0:
    case 0xC0:
    case 0xD0: {
        const u8 value = ReadArg(ppData, pPlayer, pTrack, useArgType ? argType : SEQ_ARG_U8);
        if (argType2 != SEQ_ARG_NONE) {
            commandArg2 = ReadArg(ppData, pPlayer, pTrack, argType2);
        }
        if (doExecCommand) {
            // these take a signed byte
            commandArg1 = command == MML_TRANSPOSE || command == MML_PITCH_BEND ? static_cast<s8>(value) : value;
            CommandProc(pTrack, command, commandArg1, commandArg2);
        }
        break;
    }
    case 0xE0:
        commandArg1 = static_cast<s16>(ReadArg(ppData, pPlayer, pTrack, useArgType ? argType : SEQ_ARG_S16));
        if (doExecCommand) {
            CommandProc(pTrack, command, commandArg1, commandArg2);
        }
        break;
    case 0xF0:
        switch (command) {
        case MML_EX_COMMAND: {
            const u32 exCommand = ReadByte(ppData);
            switch (exCommand & 0xF0) {
            case 0x80:
            case 0x90: {
                const u8 varNo = ReadByte(ppData);
                const s16 value = ReadArg(ppData, pPlayer, pTrack, useArgType ? argType : SEQ_ARG_S16);
                if (doExecCommand) {
                    CommandProc(pTrack, exCommand + 0xF000, varNo, value);
                }
                break;
            }
            case 0xA0:
            case 0xB0:
                commandArg1 = ReadByte(ppData);
                if (doExecCommand) {
                    CommandProc(pTrack, exCommand + 0xF000, commandArg1, commandArg2);
                }
                break;
            case 0xE0:
                commandArg1 = static_cast<u16>(ReadArg(ppData, pPlayer, pTrack, useArgType ? argType : SEQ_ARG_S16));
                if (doExecCommand) {
                    CommandProc(pTrack, exCommand + 0xF000, commandArg1, commandArg2);
                }
                break;
            default:
                break;
            }
            break;
        }
        case MML_ALLOC_TRACK:
            // read at the start (ParseAllocTrack)
            *ppData += 2;
            break;
        case MML_FIN:
            if (doExecCommand) {
                return SequenceTrack::PARSE_RESULT_FINISH;
            }
            break;
        default:
            if (doExecCommand) {
                CommandProc(pTrack, command, commandArg1, commandArg2);
            }
            break;
        }
        break;
    default:
        break;
    }
    return SequenceTrack::PARSE_RESULT_CONTINUE;
}

// 0x00742B34 | nintendogs:callseq [tier A]
s32 nw::snd::internal::driver::MmlParser::ReadArg(const u8** ppData, SequenceSoundPlayer* pPlayer, SequenceTrack* pTrack, SeqArgType argType) const
{
    s32 value = 0;
    switch (argType) {
    case SEQ_ARG_U8:
        value = ReadByte(ppData);
        break;
    case SEQ_ARG_S16:
        value = ReadByte(ppData) << 8;
        value = (value & 0xFFFF) | ReadByte(ppData);
        break;
    case SEQ_ARG_VMIDI: {
        value = 0;
        u8 data;
        do {
            data = ReadByte(ppData);
            value = (value << 7) | (data & 0x7F);
        } while (data & 0x80);
        break;
    }
    case SEQ_ARG_RANDOM: {
        s32 min = ReadByte(ppData) << 8;
        min = static_cast<s16>((min & 0xFFFF) | ReadByte(ppData));
        s32 max = ReadByte(ppData) << 8;
        max = static_cast<s16>((max & 0xFFFF) | ReadByte(ppData));
        value = min + ((Util::CalcRandom() * (max - min + 1)) >> 16);
        break;
    }
    case SEQ_ARG_VARIABLE: {
        const u8 varNo = ReadByte(ppData);
        const s16* pVariable = GetVariablePtr(pPlayer, pTrack, varNo);
        if (pVariable != nullptr) {
            value = *pVariable;
        }
        break;
    }
    default:
        break;
    }
    return value;
}

// 0x004D48B8 | nintendogs:bytes-fuzzy [tier A]
u32 nw::snd::internal::driver::MmlParser::ParseAllocTrack(const void* pBase, u32 offset, u32* pAllocTracks)
{
    const u8* pData = static_cast<const u8*>(pBase) + offset;
    if (*pData != MML_ALLOC_TRACK) {
        *pAllocTracks = 1;
        return offset;
    }
    *pAllocTracks = ((pData[1] << 8) | pData[2]) | 1;
    return offset + 3;
}

// 0x004D48F4 slot 0x00
// 0x004D48F0 (deleting dtor)
nw::snd::internal::driver::MmlParser::~MmlParser()
{
    // empty (in the original too)
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
