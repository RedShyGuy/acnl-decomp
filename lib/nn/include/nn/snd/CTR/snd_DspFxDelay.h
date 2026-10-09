#pragma once

#include "decomp.h"
#include "nn/snd/CTR/snd_Types.h"

namespace nn {
namespace snd {
namespace CTR {
// The delay effect of the DSP on an aux bus; the delay line is in a work buffer of the
// application. Member names are ours.
class DspFxDelay
{
public:
    struct Param
    {
        u32 m_DelayTime;            // 0x0, milliseconds
        f32 m_FeedbackGain;         // 0x4
        f32 m_Damping;              // 0x8
        bool m_IsEnableSurround;    // 0xC
    };

    DspFxDelay(); // 0x00460C1C (name is ours)
    bool AssignWorkBuffer(uptr buffer, size_t size); // 0x004607B4 (name is ours)
    bool IsBufferInUse(); // 0x00460838 | fefates:callgraph [tier C]
    static size_t GetRequiredMemSize(const Param& param); // 0x0046088C (name is ours)
    bool Attach(AuxBusId bus); // 0x004608CC | fefates:bytes [tier B]
    bool Enable(bool isEnabled); // 0x0046095C | fefates:bytes [tier B]
    void Finalize(); // 0x004609E8 | fefates:bytes [tier B]
    bool SetParam(const Param& param); // 0x00460AB4 | fefates:bytes [tier B]

    void* m_Buffer;         // 0x0
    uptr m_DeviceAddress;   // 0x4
    size_t m_Size;          // 0x8
    bool m_IsInitialized;   // 0xC
    s8 m_AuxBus;            // 0xD, AUX_BUS_NONE when not attached
    bool m_IsEnabled;       // 0xE
    u8 m_DisabledFrame;     // 0xF, the frame it was disabled in (the DSP still reads the buffer)
};
ASSERT_SIZE(DspFxDelay, 0x10);
} // namespace CTR
} // namespace snd
} // namespace nn
