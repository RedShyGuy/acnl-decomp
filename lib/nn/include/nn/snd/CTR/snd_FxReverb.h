#pragma once

#include "decomp.h"
#include "nn/snd/CTR/snd_Types.h"

namespace nn {
namespace snd {
namespace CTR {
// RTTI N2nn3snd3CTR8FxReverbE @ 0x008D02E0
// vtable 0x009020B0 (vptr 0x009020B8), offset_to_top 0, 2 entries
// A reverb computed by the CPU on an aux bus: early reflections, a pre-delay, two comb filters
// and an all-pass filter. Member names are ours.
class FxReverb
{
public:
    struct Param
    {
        u32 m_EarlyReflectionTime;      // 0x00, milliseconds
        u32 m_FusedTime;                // 0x04
        u32 m_PreDelayTime;             // 0x08
        f32 m_Coloration;               // 0x0C
        f32 m_Damping;                  // 0x10
        const ReverbFilterSize* m_FilterSize;   // 0x14
        f32 m_EarlyGain;                // 0x18
        f32 m_FusedGain;                // 0x1C
        bool m_IsEnableSurround;        // 0x20
    };

    static const s32 CHANNEL_MAX = 4;

    FxReverb(); // 0x00466138 | stores vtable ptr
    virtual ~FxReverb(); // 0x004662E4 slot 0x00 | fefates:bytes
    // 0x0046628C slot 0x04 | nintendogs:bytes (deleting dtor)
    bool Initialize(); // 0x00465964 | nintendogs:bytes [confirmed by fefates] [tier A]
    void UpdateBuffer(uptr data); // 0x00465A70 | nintendogs:callgraph [confirmed by fefates] [tier A]
    void InitializeParam(); // 0x00465CA0 | fefates:bytes [tier B]
    bool AssignWorkBuffer(uptr buffer, size_t size); // 0x00465E9C | nintendogs:bytes [confirmed by fefates] [tier A]
    void ReleaseWorkBuffer(); // 0x00465EBC (name is ours)
    size_t GetRequiredMemSize(); // 0x00465ED0 | nintendogs:bytes [confirmed by fefates] [tier A]
    void Finalize(); // 0x00465F7C | nintendogs:bytes [confirmed by fefates] [tier A]
    bool SetParam(const Param& param); // 0x00465FC4 | nintendogs:bytes [confirmed by fefates] [tier A]

    Param m_Param;                          // 0x004
    void* m_WorkBuffer;                     // 0x028
    size_t m_WorkSize;                      // 0x02C
    ReverbFilterSize m_FilterSize;          // 0x030
    s32* m_EarlyBuffer[CHANNEL_MAX];        // 0x03C
    s32* m_PreDelayBuffer[CHANNEL_MAX];     // 0x04C
    s32* m_CombBuffer[CHANNEL_MAX][2];      // 0x05C
    s32* m_AllPassBuffer[CHANNEL_MAX];      // 0x07C
    s32* m_Unknown8C[CHANNEL_MAX];          // 0x08C
    u32 m_EarlyLength;                      // 0x09C, in samples
    u32 m_EarlyPosition;                    // 0x0A0
    u32 m_PreDelayLength;                   // 0x0A4
    u32 m_PreDelayPosition;                 // 0x0A8
    u32 m_CombLength[2];                    // 0x0AC
    u32 m_CombPosition[2];                  // 0x0B4
    s32 m_CombGain[2];                      // 0x0BC, 1.7 fixed point
    u32 m_AllPassLength;                    // 0x0C4
    u32 m_AllPassPosition;                  // 0x0C8
    s32 m_AllPassGain;                      // 0x0CC
    s32 m_LpfState[CHANNEL_MAX];            // 0x0D0
    s32 m_EarlyGain;                        // 0x0E0
    s32 m_FusedGain;                        // 0x0E4
    s32 m_LpfB;                             // 0x0E8
    s32 m_LpfA;                             // 0x0EC
    // the limits of the work buffer (set by Initialize)
    u32 m_MaxEarlyReflectionTime;           // 0x0F0
    u32 m_MaxPreDelayTime;                  // 0x0F4
    ReverbFilterSize m_MaxFilterSize;       // 0x0F8
    bool m_IsActive;                        // 0x104
};
ASSERT_SIZE(FxReverb, 0x108);
} // namespace CTR
} // namespace snd
} // namespace nn
