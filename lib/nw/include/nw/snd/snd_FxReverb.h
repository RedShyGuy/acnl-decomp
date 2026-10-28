#pragma once

#include "decomp.h"
#include "nn/snd/CTR/snd_Types.h"
#include "nw/snd/snd_FxBase.h"

namespace nw {
namespace snd {
// A reverb computed in the aux callback; the algorithm of nn::snd::CTR::FxReverb for any number of samples
// and up to four channels (the class and function names are from the reference symbols, the members are ours).
// RTTI N2nw3snd8FxReverbE @ 0x008D0970
// vtable 0x00902FE4 (vptr 0x00902FEC), offset_to_top 0, 6 entries
class FxReverb : public ::nw::snd::FxBase
{
public:
    struct Param
    {
        u32 earlyReflectionTime;                    // 0x00 milliseconds
        u32 fusedTime;                              // 0x04
        u32 preDelayTime;                           // 0x08
        f32 coloration;                             // 0x0C
        f32 damping;                                // 0x10
        const nn::snd::CTR::ReverbFilterSize* filterSize; // 0x14
        f32 earlyGain;                              // 0x18
        f32 fusedGain;                              // 0x1C
        bool isEnableSurround;                      // 0x20
        bool isInvertDamping;                       // 0x21 the damping filter with negative coefficients
    };
    static const int CHANNEL_MAX = 4;

    FxReverb(); // 0x004C4758 | nintendogs:bytes [tier B]
    // 0x004C48BC (deleting dtor)
    virtual ~FxReverb(); // 0x004C4934 slot 0x00

    virtual bool Initialize(); // 0x004C3F3C slot 0x08 | nintendogs:bytes
    virtual void Finalize(); // 0x004C4570 slot 0x0C | nintendogs:bytes
    virtual void UpdateBuffer(int channelCount, nn::snd::CTR::AuxBusData* pData, int sampleLength, SampleFormat format, f32 sampleRate,
                              OutputMode outputMode); // 0x004C4054 slot 0x10

    void InitializeParam(); // 0x004C42C4 | fefates:bytes [tier B]
    bool AssignWorkBuffer(uptr buffer, u32 size); // 0x004C449C | nintendogs:bytes [tier B]
    void ReleaseWorkBuffer(); // 0x004C44BC (name after nn::snd::CTR::FxReverb)
    u32 GetRequiredMemSize(); // 0x004C44D0 | nintendogs:bytes [tier B]
    bool SetParam(const Param& param); // 0x004C45CC | nintendogs:bytes [tier B]

    Param m_Param;                                  // 0x00C
    void* m_WorkBuffer;                             // 0x030
    u32 m_WorkSize;                                 // 0x034
    nn::snd::CTR::ReverbFilterSize m_FilterSize;    // 0x038
    s32* m_EarlyBuffer[CHANNEL_MAX];                // 0x044
    s32* m_PreDelayBuffer[CHANNEL_MAX];             // 0x054
    s32* m_CombBuffer[CHANNEL_MAX][2];              // 0x064
    s32* m_AllPassBuffer[CHANNEL_MAX];              // 0x084
    s32* m_Unknown94[CHANNEL_MAX];                  // 0x094
    u32 m_EarlyLength;                              // 0x0A4 in samples
    u32 m_EarlyPosition;                            // 0x0A8
    u32 m_PreDelayLength;                           // 0x0AC
    u32 m_PreDelayPosition;                         // 0x0B0
    u32 m_CombLength[2];                            // 0x0B4
    u32 m_CombPosition[2];                          // 0x0BC
    s32 m_CombGain[2];                              // 0x0C4 1.7 fixed point
    u32 m_AllPassLength;                            // 0x0CC
    u32 m_AllPassPosition;                          // 0x0D0
    s32 m_AllPassGain;                              // 0x0D4
    s32 m_LpfState[CHANNEL_MAX];                    // 0x0D8
    s32 m_EarlyGain;                                // 0x0E8
    s32 m_FusedGain;                                // 0x0EC
    s32 m_LpfB;                                     // 0x0F0
    s32 m_LpfA;                                     // 0x0F4
    // the limits of the work buffer (set by Initialize)
    u32 m_MaxEarlyReflectionTime;                   // 0x0F8
    u32 m_MaxPreDelayTime;                          // 0x0FC
    nn::snd::CTR::ReverbFilterSize m_MaxFilterSize; // 0x100
    bool m_MaxIsEnableSurround;                     // 0x10C
    u8 m_ChannelCount;                              // 0x10D
    bool m_IsActive;                                // 0x10E
};
ASSERT_SIZE(FxReverb, 0x110);
} // namespace snd
} // namespace nw
