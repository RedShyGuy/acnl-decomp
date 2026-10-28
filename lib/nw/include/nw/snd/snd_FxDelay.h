#pragma once

#include "decomp.h"
#include "nw/snd/snd_FxBase.h"

namespace nw {
namespace snd {
// A delay effect computed in the aux callback; the algorithm of nn::snd::CTR::FxDelay for any number of
// samples (the class and function names are from the reference symbols, the members are ours).
// RTTI N2nw3snd7FxDelayE @ 0x008D0964
// vtable 0x00902FC4 (vptr 0x00902FCC), offset_to_top 0, 6 entries
class FxDelay : public ::nw::snd::FxBase
{
public:
    struct Param
    {
        u32 delayTime;          // 0x0 milliseconds
        f32 feedbackGain;       // 0x4
        f32 damping;            // 0x8
        bool isEnableSurround;  // 0xC
    };
    static const int CHANNEL_MAX = 4;

    FxDelay(); // 0x004C3DB4 | nintendogs:bytes [tier B]
    // 0x004C3E5C (deleting dtor)
    virtual ~FxDelay(); // 0x004C3ECC slot 0x00

    virtual bool Initialize(); // 0x004C3A38 slot 0x08 | nintendogs:bytes
    virtual void Finalize(); // 0x004C3C38 slot 0x0C | nintendogs:bytes
    virtual void UpdateBuffer(int channelCount, nn::snd::CTR::AuxBusData* pData, int sampleLength, SampleFormat format, f32 sampleRate,
                              OutputMode outputMode); // 0x004C3AE0 slot 0x10 | nintendogs:bytes

    bool AssignWorkBuffer(uptr buffer, u32 size); // 0x004C3BE8 | nintendogs:bytes [tier B]
    void ReleaseWorkBuffer(); // 0x004C3C08 (name after nn::snd::CTR::FxDelay)
    u32 GetRequiredMemSize(); // 0x004C3C1C | nintendogs:bytes [tier B]
    bool SetParam(const Param& param); // 0x004C3C8C | nintendogs:bytes [tier B]

    Param m_Param;                      // 0x0C
    void* m_WorkBuffer;                 // 0x1C
    u32 m_WorkSize;                     // 0x20
    s32* m_DelayLine[CHANNEL_MAX];      // 0x24
    s32 m_LpfState[CHANNEL_MAX];        // 0x34
    u32 m_DelayFrames;                  // 0x44
    u32 m_Position;                     // 0x48 in frames
    s32 m_Feedback;                     // 0x4C 1.7 fixed point
    s32 m_LpfB;                         // 0x50
    s32 m_LpfA;                         // 0x54
    u32 m_MaxDelayTime;                 // 0x58 of the work buffer (set by Initialize)
    bool m_MaxIsEnableSurround;         // 0x5C
    u8 m_ChannelCount;                  // 0x5D
    bool m_IsActive;                    // 0x5E
};
ASSERT_SIZE(FxDelay, 0x60);
} // namespace snd
} // namespace nw
