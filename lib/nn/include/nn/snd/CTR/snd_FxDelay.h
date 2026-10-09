#pragma once

#include "decomp.h"
#include "nn/snd/CTR/snd_Types.h"

namespace nn {
namespace snd {
namespace CTR {
// RTTI N2nn3snd3CTR7FxDelayE @ 0x008D02D8
// vtable 0x009020A0 (vptr 0x009020A8), offset_to_top 0, 2 entries
// A delay effect computed by the CPU on an aux bus (MasterManager::ExecuteEffect). Member names
// are ours.
class FxDelay
{
public:
    struct Param
    {
        u32 m_DelayTime;            // 0x0, milliseconds
        f32 m_FeedbackGain;         // 0x4
        f32 m_Damping;              // 0x8
        bool m_IsEnableSurround;    // 0xC
    };

    static const s32 CHANNEL_MAX = 4;

    FxDelay(); // 0x00465768 | nintendogs:bytes [confirmed by fefates] [tier A]
    virtual ~FxDelay(); // 0x00465874 slot 0x00 | fefates:bytes
    // 0x00465804 slot 0x04 | nintendogs:bytes (deleting dtor)
    bool Initialize(); // 0x004653F4 | nintendogs:bytes [confirmed by fefates] [tier A]
    void UpdateBuffer(uptr data); // 0x0046549C | nintendogs:bytes [confirmed by fefates] [tier A]
    bool AssignWorkBuffer(uptr buffer, size_t size); // 0x00465594 | nintendogs:bytes [confirmed by fefates] [tier A]
    void ReleaseWorkBuffer(); // 0x004655B4 (name is ours)
    size_t GetRequiredMemSize(); // 0x004655C8 | nintendogs:bytes [confirmed by fefates] [tier A]
    void Finalize(); // 0x004655E4 | nintendogs:bytes [confirmed by fefates] [tier A]
    bool SetParam(const Param& param); // 0x00465638 | nintendogs:bytes [confirmed by fefates] [tier A]

    Param m_Param;                      // 0x04
    void* m_WorkBuffer;                 // 0x14
    size_t m_WorkSize;                  // 0x18
    s32* m_DelayLine[CHANNEL_MAX];      // 0x1C
    s32 m_LpfState[CHANNEL_MAX];        // 0x2C
    u32 m_DelayFrames;                  // 0x3C
    u32 m_Position;                     // 0x40, in frames
    s32 m_Feedback;                     // 0x44, 1.7 fixed point
    s32 m_LpfB;                         // 0x48
    s32 m_LpfA;                         // 0x4C
    u32 m_MaxDelayTime;                 // 0x50, of the work buffer (set by Initialize)
    bool m_MaxIsEnableSurround;         // 0x54
    u8 m_ChannelCount;                  // 0x55
    bool m_IsActive;                    // 0x56
};
ASSERT_SIZE(FxDelay, 0x58);
} // namespace CTR
} // namespace snd
} // namespace nn
