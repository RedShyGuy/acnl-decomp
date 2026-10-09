#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/fnd/fnd_TimeSpan.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_Event.h"
#include "nn/snd/CTR/snd_Types.h"
#include "nn/snd/CTR/snd_VoiceImpl.h"

namespace nn {
namespace snd {
namespace CTR {
class OutputCapture;

// The structures in the memory of the DSP (double buffered: the ARM writes one set while the
// DSP reads the other). A changed value is marked in m_DirtyFlags. Names are ours.
struct DspMasterParam
{
    u32 m_DirtyFlags;                       // 0x00
    f32 m_MasterVolume;                     // 0x04
    f32 m_AuxReturnVolume[AUX_BUS_COUNT];   // 0x08
    u16 m_OutputBufferCount;                // 0x10
    u16 m_Unknown12[2];                     // 0x12
    u16 m_OutputMode;                       // 0x16
    u16 m_ClippingMode;                     // 0x18
    u16 m_IsHeadsetConnected;               // 0x1A
    u16 m_SurroundDepth;                    // 0x1C
    u16 m_SurroundSpeakerPosition;          // 0x1E
    u16 m_Unknown20;                        // 0x20
    u16 m_RearRatio;                        // 0x22
    u16 m_AuxFrontBypass[AUX_BUS_COUNT];    // 0x24
    u16 m_AuxBusEnable[AUX_BUS_COUNT];      // 0x28
    DspFxDelayParams m_Delay[AUX_BUS_COUNT];    // 0x2C
    DspFxReverbParams m_Reverb[AUX_BUS_COUNT];  // 0x54
    u16 m_SyncMode;                         // 0xBC
    u16 m_PaddingBE;                        // 0xBE
    u32 m_DirtyFlags2;                      // 0xC0
};
ASSERT_SIZE(DspMasterParam, 0xC4);

struct DspMasterStatus
{
    u16 m_Unknown0;             // 0x0
    u16 m_DroppedFrameCount;    // 0x2
};

// a wave buffer that follows the playing one
struct DspNextBuffer
{
    u32 m_Address;              // 0x00, halves swapped
    u32 m_SampleLength;         // 0x04, halves swapped
    AdpcmContext m_AdpcmContext;    // 0x08
    u8 m_HasAdpcmContext;       // 0x0E
    u8 m_IsLoop;                // 0x0F
    u16 m_BufferId;             // 0x10
    u16 m_Padding;              // 0x12
};
ASSERT_SIZE(DspNextBuffer, 0x14);

struct DspChannelParam
{
    static const s32 NEXT_BUFFER_COUNT = 4;

    u32 m_DirtyFlags;                       // 0x00
    u16 m_Mix[3][8];                        // 0x04, the f32 of MixParam in 16 bit halves
    f32 m_TimerRate;                        // 0x34
    u8 m_Interpolation[2];                  // 0x38
    u16 m_FilterType;                       // 0x3A
    s16 m_MonoFilter[2];                    // 0x3C
    s16 m_BiquadFilter[5];                  // 0x40
    u16 m_NextBufferMask;                   // 0x4A
    DspNextBuffer m_NextBuffers[NEXT_BUFFER_COUNT]; // 0x4C
    u32 m_Unknown9C;                        // 0x9C
    u16 m_PlayState;                        // 0xA0
    s16 m_SyncCount;                        // 0xA2
    u32 m_PlayPosition;                     // 0xA4
    u32 m_UnknownA8;                        // 0xA8
    u32 m_Address;                          // 0xAC, halves swapped
    u32 m_SampleLength;                     // 0xB0, halves swapped
    DspsndAudioInfo m_AudioInfo;            // 0xB4
    u16 m_AdpcmContext[3];                  // 0xB6
    u16 m_BufferFlags;                      // 0xBC, 1: has an ADPCM context, 2: loop
    u16 m_BufferId;                         // 0xBE
};
ASSERT_SIZE(DspChannelParam, 0xC0);

// The driver of the DSP sound component: the parameters of the master output, the aux buses and
// the 24 channels, and the frame synchronization. Member names are ours.
class Dspsnd
{
public:
    static const s32 CHANNEL_COUNT = 24;
    static const s32 SAVED_MEMORY_SIZE = 4224;
    static const s32 STATUS_SIZE = 608;
    // in the status: the DSP cycles of the last frame (u32; the name is ours)
    static const s32 STATUS_DSP_CYCLES = 8;
    // the samples of a frame per channel
    static const s32 FRAME_SAMPLES = 160;

    Dspsnd();
    ~Dspsnd(); // 0x00465344 | fefates:bytes [tier B]

    nn::Result Initialize(bool isWakeUp); // 0x00463850 | nintendogs:callgraph [confirmed by mk7dlp] [tier A]
    void SetSyncMode(SyncMode mode); // 0x00463A44 | fefates:bytes [tier B]
    bool EnableAuxBus(AuxBusId bus, bool isEnabled); // 0x00463A98 | nintendogs:callgraph [confirmed by fefates] [tier A]
    bool SetRearRatio(u16 ratio); // 0x00463B24 | fefates:bytes [tier B]
    void SendParameter(); // 0x00463B8C | fefates:callgraph [tier C]
    bool SetChannelMix(u8 channel, const MixParam* mixParam); // 0x00463CF0 | nintendogs:callgraph [confirmed by fefates] [tier A]
    bool SetChannelRIM(u8 channel, u16 interpolation, u16 interpolation2); // 0x00463DCC | fefates:bytes [tier B]
    void SyncFrameData(); // 0x00463E4C | fefates:callgraph [tier C]
    bool SetChannelTimer(u8 channel, f32 rate); // 0x00463F70 | fefates:bytes [tier B]
    bool SetClippingMode(ClippingMode mode); // 0x00463FEC | fefates:bytes [tier B]
    void SetMasterVolume(f32 volume); // 0x0046404C | nintendogs:callgraph [confirmed by fefates] [tier A]
    bool SetSurroundDepth(u16 depth); // 0x004640AC | fefates:bytes [tier B]
    bool SetAuxFrontBypass(AuxBusId bus, bool isBypass); // 0x00464118 | nintendogs:callgraph [confirmed by fefates] [tier A]
    bool SetDspDelayEffect(AuxBusId bus, DspFxDelayParams& params); // 0x004641A4 | nintendogs:callgraph [confirmed by fefates] [tier A]
    void SetAuxReturnVolume(AuxBusId bus, f32 volume); // 0x004642A8 | nintendogs:callgraph [confirmed by fefates] [tier A]
    bool SetChannelPlayStop(u8 channel); // 0x00464320 | fefates:bytes [tier B]
    bool SetDspReverbEffect(AuxBusId bus, DspFxReverbParams& params); // 0x00464394 | nintendogs:callgraph [confirmed by fefates] [tier A]
    bool SetSoundOutputMode(OutputMode mode); // 0x00464500 | fefates:bytes [tier B]
    void InitializeVariables(bool isWakeUp); // 0x00464560 | fefates:bytes [tier B]
    bool SetChannelPlayStart(u8 channel); // 0x00464810 | fefates:bytes [tier B]
    bool SetChannelSyncCount(u8 channel, s16 syncCount); // 0x00464880 | nintendogs:callgraph [confirmed by fefates] [tier A]
    s32 GetDroppedFrameCount(); // 0x004648F0 | fefates:bytes [tier B]
    bool SetChannelAdpcmParam(u8 channel, const AdpcmParam* param); // 0x00464940 | nintendogs:callseq [confirmed by fefates] [tier A]
    void SetOutputBufferCount(int count); // 0x004649CC | fefates:bytes [tier B]
    bool SetIsHeadsetConnected(bool isConnected); // 0x00464A20 | fefates:bytes [tier B]
    bool ResetChannelNextBuffer(u8 channel); // 0x00464A80 | fefates:bytes [tier B]
    bool AppendChannelNextBuffer(u8 channel, const WaveBuffer* buffer, int slot); // 0x00464AE8 | nintendogs:callgraph [confirmed by fefates] [tier A]
    bool SetChannelIirFilterType(u8 channel, FilterType type); // 0x00464C64 | fefates:bytes [tier B]
    bool UpdateChannelNextBuffer(u8 channel, const WaveBuffer* buffer); // 0x00464CD4 | fefates:bytes [tier B]
    bool SetChannelIIRFilter_Mono(u8 channel, s16 a0, s16 b0); // 0x00464D94 | fefates:bytes [tier B]
    bool SetChannelIIRFilter_Biquad(u8 channel, s16 b2, s16 b1, s16 b0, s16 a2, s16 a1); // 0x00464E0C | fefates:bytes [tier B]
    bool SetSurroundSpeakerPosition(SurroundSpeakerPosition position); // 0x00464E98 | fefates:bytes [tier B]
    bool InitializeChannelParameters(u8 channel); // 0x00464F04 | fefates:bytes [tier B]
    void Finalize(bool isSleep); // 0x00464F6C | nintendogs:callgraph [confirmed by fefates, mk7dlp] [tier A]
    bool WaitPipe(nn::fnd::TimeSpan timeout); // 0x004650B0 | fefates:bytes [tier B]
    void WaitPipe(); // 0x00465120 | fefates:bytes [tier B]
    bool AssignPCM(u8 channel, const WaveBuffer* buffer, DspsndAudioInfo audioInfo); // 0x00465178 | nintendogs:callgraph [confirmed by fefates] [tier A]

    DspChannelParam* GetChannelParam(u8 channel) { return m_ChannelParam[m_FrameCount & 1][channel]; }
    DspMasterParam* GetMasterParam() { return m_MasterParam[m_ParamIndex]; }

    nn::os::Event m_Event;                              // 0x0000, the DSP interrupt
    nn::Handle m_SemaphoreEvent;                        // 0x0004
    nn::os::CriticalSection m_Lock;                     // 0x0008
    volatile s32 m_UseCount;                            // 0x0014, of the frame functions
    OutputCapture* m_OutputCapture;                     // 0x0018
    u8 m_SavedDspMemory[SAVED_MEMORY_SIZE];             // 0x001C, over a sleep
    u16* m_FrameCounter[2];                             // 0x109C
    DspMasterParam* m_MasterParam[2];                   // 0x10A4
    DspMasterStatus* m_MasterStatus[2];                 // 0x10AC
    DspChannelParam* m_ChannelParam[2][CHANNEL_COUNT];  // 0x10B4
    DspChannelStatus* m_ChannelStatus[2][CHANNEL_COUNT];    // 0x1174
    AdpcmParam* m_AdpcmParam[2][CHANNEL_COUNT];         // 0x1234
    s32* m_AuxBuffer[2][AUX_BUS_COUNT];                 // 0x12F4
    s16* m_OutputBuffer[2];                             // 0x1304
    void* m_Unknown130C[2];                             // 0x130C
    void* m_DspStatusBuffer[2];                         // 0x1314
    bool m_IsInitialized;                               // 0x131C
    u8 m_WaitCount;                                     // 0x131D
    u16 m_FrameCount;                                   // 0x131E
    u16 m_StatusIndex;                                  // 0x1320
    u16 m_ParamIndex;                                   // 0x1322
    s32 m_DspCycleLimit;                                // 0x1324
    u8 m_DspStatus[STATUS_SIZE];                        // 0x1328, a copy of the status of the DSP
    void* m_Unknown1588[5][2];                          // 0x1588
    bool m_IsAuxUserCallbackEnabled;                    // 0x15B0
};
ASSERT_SIZE(Dspsnd, 0x15B4);

// (defined in the .cpp, with its address)
extern Dspsnd g_Dspsnd;
} // namespace CTR
} // namespace snd
} // namespace nn
