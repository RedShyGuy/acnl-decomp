#pragma once

#include "decomp.h"
#include "nn/snd/CTR/snd_DspFxDelay.h"
#include "nn/snd/CTR/snd_DspFxReverb.h"
#include "nw/snd/snd_Global.h"

namespace nn {
namespace snd {
namespace CTR {
class FxDelay;
class FxReverb;
} // namespace CTR
} // namespace snd
} // namespace nn

namespace nw {
namespace snd {
class FxBase;

namespace internal {
namespace driver {
class BasicSoundPlayer;
class DisposeCallback;
} // namespace driver

// A command from the game threads to the sound thread (the class and ProcessCommandList are from the
// reference symbols, the members and the names of the commands are ours). The commands of a flush
// are a list in the memory ring of the DriverCommandManager; the derived commands follow at 0x10.
struct DriverCommand
{
    // the commands (by what ProcessCommandList does with them; only those used so far are named)
    enum Id : u8
    {
        ID_DUMMY = 0,            // nothing (a flush without commands)
        ID_REPLY = 2,            // the flag of the game is set: the sound thread got the list (name is ours)
        ID_PLAYER_INIT = 3,      // BasicSoundPlayer::Initialize, then the flag of the sound is set
        ID_PLAYER_FINALIZE = 4,  // BasicSoundPlayer::Finalize
        ID_PLAYER_START = 5,     // BasicSoundPlayer::Start
        ID_PLAYER_STOP = 6,      // BasicSoundPlayer::Stop (fade out: volume 0 first)
        ID_PLAYER_PAUSE = 7,     // BasicSoundPlayer::Pause
        ID_PLAYER_PARAM = 8,     // the parameters of a BasicSoundPlayer
        ID_PLAYER_PANMODE = 9,
        ID_PLAYER_PANCURVE = 10,
        ID_PLAYER_FRONTBYPASS = 11,
        ID_PLAYER_CLEAR_UNKNOWN0x10 = 12, // BasicSoundPlayer::m_Unknown0x10 = false
        ID_SEQSOUND_SETUP = 13,           // SequenceSoundPlayer::Setup
        ID_SEQSOUND_LOAD = 14,            // SequenceSoundPlayer::RequestLoad
        ID_SEQSOUND_PREPARE = 15,         // SequenceSoundPlayer::Prepare
        ID_SEQSOUND_SKIP = 16,            // SequenceSoundPlayer::Skip
        ID_SEQSOUND_TEMPORATIO = 17,
        ID_SEQSOUND_CHANNELPRIO = 18,
        ID_SEQSOUND_SETVAR = 19,          // SequenceSoundPlayer::SetLocalVariable
        ID_SEQSOUND_SETGVAR = 20,         // SequenceSoundPlayer::SetGlobalVariable
        ID_SEQSOUND_SETTVAR = 21,         // SequenceTrack::SetTrackVariable
        ID_SEQTRACK_MUTE = 22,            // SequenceSoundPlayer::SetTrackMute
        ID_SEQTRACK_SILENCE = 23,         // SequenceSoundPlayer::SetTrackSilence
        ID_SEQTRACK_VOLUME = 24,          // SequenceSoundPlayer::SetTrackVolume
        ID_SEQTRACK_PITCH = 25,           // SequenceSoundPlayer::SetTrackPitch
        ID_SEQTRACK_PAN = 26,             // SequenceSoundPlayer::SetTrackPan
        ID_SEQTRACK_SURROUND_PAN = 27,    // SequenceSoundPlayer::SetTrackSurroundPan
        ID_SEQTRACK_LPF = 28,             // SequenceSoundPlayer::SetTrackLpfFreq
        ID_SEQTRACK_PAN_RANGE = 29,       // SequenceSoundPlayer::SetTrackPanRange
        ID_SEQTRACK_BIQUAD = 30,          // SequenceSoundPlayer::SetTrackBiquadFilter
        ID_SEQTRACK_BANK_INDEX = 31,      // SequenceSoundPlayer::SetTrackBankIndex
        ID_SEQTRACK_TRANSPOSE = 32,       // SequenceSoundPlayer::SetTrackTranspose
        ID_SEQTRACK_VELOCITY_RANGE = 33,  // SequenceSoundPlayer::SetTrackVelocityRange
        ID_WAVESOUND_PREPARE = 34,        // WaveSoundPlayer::Prepare
        ID_WAVESOUND_LOAD = 35,           // WaveSoundPlayer::RequestLoad
        ID_WAVESOUND_CHANNEL_PRIO = 36,   // the channel priority of a WaveSoundPlayer
        ID_WAVESOUND_CHANNEL_PARAM = 37,  // the channel priority and the release priority fix of a WaveSoundPlayer
        ID_WAVESOUND_LOOP = 38,           // WaveSoundPlayer::m_IsLoop
        ID_STRMSOUND_SETUP = 39,          // StreamSoundPlayer::Setup
        ID_STRMSOUND_PREPARE = 40,        // StreamSoundPlayer::Prepare
        ID_STRMSOUND_PREPARE_PREFETCH = 41, // StreamSoundPlayer::PreparePrefetch
        ID_STRMSOUND_LOAD_HEADER = 42,    // StreamSoundPlayer::LoadHeader (from the task thread)
        ID_STRMSOUND_LOAD_DATA = 43,      // StreamSoundPlayer::LoadStreamData (from the task thread)
        ID_STRMSOUND_LOOP_FROM_START = 44, // StreamSoundPlayer::m_IsLoopFromStart
        ID_STRMTRACK_VOLUME = 45,         // StreamSoundPlayer::SetTrackVolume
        ID_STRMTRACK_INITIAL_VOLUME = 46, // StreamSoundPlayer::SetTrackInitialVolume
        ID_STRMTRACK_PAN = 47,            // StreamSoundPlayer::SetTrackPan
        ID_STRMTRACK_SURROUND_PAN = 48,   // StreamSoundPlayer::SetTrackSurroundPan
        ID_DISPOSE = 49,        // DisposeCallbackManager::Dispose
        ID_REGISTER_DISPOSE_CALLBACK = 50,   // DisposeCallbackManager::RegisterDisposeCallback
        ID_UNREGISTER_DISPOSE_CALLBACK = 51, // DisposeCallbackManager::UnregisterDisposeCallback
        ID_APPEND_EFFECT = 52,              // HardwareManager::AppendEffect(FxBase*)
        ID_APPEND_EFFECT_FX_DELAY = 53,     // HardwareManager::AppendEffect(nn::snd::CTR::FxDelay*)
        ID_APPEND_EFFECT_FX_REVERB = 54,    // HardwareManager::AppendEffect(nn::snd::CTR::FxReverb*)
        ID_APPEND_EFFECT_DSP_DELAY = 55,    // HardwareManager::AppendEffect(nn::snd::CTR::DspFxDelay*)
        ID_APPEND_EFFECT_DSP_REVERB = 56,   // HardwareManager::AppendEffect(nn::snd::CTR::DspFxReverb*)
        ID_CLEAR_EFFECT = 57,               // HardwareManager::ClearEffect
        ID_VOICE_SYNC = 58,      // VoiceManager::UpdateAllVoicesSync
    };

    // runs the commands of a list (in the sound thread)
    static void ProcessCommandList(DriverCommand* pList); // 0x004C7300 | nintendogs:callgraph [tier A]

    DriverCommand* next; // 0x0
    Id id;               // 0x4
    u8 padding[3];       // 0x5
    u32 tag;             // 0x8 the tag of the flush (the first command of a list)
    u32 memoryNext;      // 0xC the write index of the memory ring behind the command
};
ASSERT_SIZE(DriverCommand, 0x10);

// the voices update their parameters (name is ours)
struct DriverCommandVoiceSync : DriverCommand
{
    u32 syncFlag; // 0x10 the parameters to update (Voice)
};
ASSERT_SIZE(DriverCommandVoiceSync, 0x14);

// memory of sound data goes away (name is ours)
struct DriverCommandDispose : DriverCommand
{
    const void* pMemory; // 0x10
    u32 size;            // 0x14
};
ASSERT_SIZE(DriverCommandDispose, 0x18);

// a dispose callback is registered or unregistered (name is ours)
struct DriverCommandDisposeCallback : DriverCommand
{
    driver::DisposeCallback* pCallback; // 0x10
};
ASSERT_SIZE(DriverCommandDisposeCallback, 0x14);

// a command for a player (names are ours)
struct DriverCommandPlayer : DriverCommand
{
    driver::BasicSoundPlayer* player; // 0x10
};
ASSERT_SIZE(DriverCommandPlayer, 0x14);

// a command for a player with a flag: finalize, stop (fade out), pause, front bypass (names are ours)
struct DriverCommandPlayerState : DriverCommand
{
    driver::BasicSoundPlayer* player; // 0x10
    bool flag;                        // 0x14
    u8 padding[3];                    // 0x15
};
ASSERT_SIZE(DriverCommandPlayerState, 0x18);

// the player is initialized; then *availableFlag is set (names are ours)
struct DriverCommandPlayerInit : DriverCommand
{
    driver::BasicSoundPlayer* player; // 0x10
    bool* availableFlag;              // 0x14
};
ASSERT_SIZE(DriverCommandPlayerInit, 0x18);

// the pan mode or pan curve of a player (names are ours)
struct DriverCommandPlayerPanParam : DriverCommand
{
    driver::BasicSoundPlayer* player; // 0x10
    PanMode panMode;                  // 0x14
    PanCurve panCurve;                // 0x15
    u8 padding[2];                    // 0x16
};
ASSERT_SIZE(DriverCommandPlayerPanParam, 0x18);

// the parameters of a player (names are ours)
struct DriverCommandPlayerParam : DriverCommand
{
    driver::BasicSoundPlayer* player; // 0x10
    f32 volume;                       // 0x14
    f32 pitch;                        // 0x18
    f32 pan;                          // 0x1C
    f32 surroundPan;                  // 0x20
    f32 lpfFreq;                      // 0x24
    int biquadType;                   // 0x28
    f32 biquadValue;                  // 0x2C
    f32 mainSend;                     // 0x30
    f32 fxSend[2];                    // 0x34
};
ASSERT_SIZE(DriverCommandPlayerParam, 0x3C);

// the effects of an aux bus (52-57; names are ours)
struct DriverCommandEffect : DriverCommand
{
    AuxBus bus;        // 0x10
    u8 padding[3];     // 0x11
    FxBase* effect;    // 0x14
    u32 unknown0x18;   // 0x18
};
ASSERT_SIZE(DriverCommandEffect, 0x1C);
struct DriverCommandEffectFxDelay : DriverCommand
{
    AuxBus bus;                      // 0x10
    u8 padding[3];                   // 0x11
    nn::snd::CTR::FxDelay* effect;   // 0x14
};
ASSERT_SIZE(DriverCommandEffectFxDelay, 0x18);
struct DriverCommandEffectFxReverb : DriverCommand
{
    AuxBus bus;                      // 0x10
    u8 padding[3];                   // 0x11
    nn::snd::CTR::FxReverb* effect;  // 0x14
};
ASSERT_SIZE(DriverCommandEffectFxReverb, 0x18);
struct DriverCommandEffectDspDelay : DriverCommand
{
    AuxBus bus;                             // 0x10
    u8 padding[3];                          // 0x11
    nn::snd::CTR::DspFxDelay* effect;       // 0x14
    nn::snd::CTR::DspFxDelay::Param param;  // 0x18
};
ASSERT_SIZE(DriverCommandEffectDspDelay, 0x28);
struct DriverCommandEffectDspReverb : DriverCommand
{
    AuxBus bus;                                 // 0x10
    u8 padding[3];                              // 0x11
    nn::snd::CTR::DspFxReverb* effect;          // 0x14
    nn::snd::CTR::DspFxReverb::Param param;     // 0x18
    nn::snd::CTR::ReverbFilterSize filterSize; // 0x3C param.m_FilterSize points here
};
ASSERT_SIZE(DriverCommandEffectDspReverb, 0x48);
struct DriverCommandEffectClear : DriverCommand
{
    AuxBus bus;        // 0x10
    u8 padding[3];     // 0x11
    u32 unknown0x14;   // 0x14
    int fadeTimes;     // 0x18
};
ASSERT_SIZE(DriverCommandEffectClear, 0x1C);

// a flag the sound thread sets (2; names are ours)
struct DriverCommandReply : DriverCommand
{
    bool* pFlag; // 0x10
};
ASSERT_SIZE(DriverCommandReply, 0x14);
} // namespace internal
} // namespace snd
} // namespace nw
