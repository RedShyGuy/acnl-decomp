#pragma once

#include "decomp.h"
#include "nn/snd/CTR/snd_Types.h"

namespace nn {
namespace snd {
namespace CTR {
// The reverb effect of the DSP on an aux bus; the buffers are in a work buffer of the
// application. Member names are ours.
class DspFxReverb
{
public:
    struct Param
    {
        u32 m_EarlyReflectionTime;      // 0x00, milliseconds
        u32 m_FusedTime;                // 0x04
        u32 m_PreDelayTime;             // 0x08
        f32 m_Coloration;               // 0x0C
        f32 m_Damping;                  // 0x10
        const ReverbFilterSize* m_FilterSize;   // 0x14, NULL: the default
        f32 m_EarlyGain;                // 0x18
        f32 m_FusedGain;                // 0x1C
        bool m_IsEnableSurround;        // 0x20
    };

    DspFxReverb(); // 0x00461414 (name is ours)
    bool AssignWorkBuffer(uptr buffer, size_t size); // 0x00460D18 (name is ours)
    bool IsBufferInUse(); // 0x00460D9C | fefates:callgraph [tier C]
    static size_t GetRequiredMemSize(const Param& param); // 0x00460DF0 (name is ours)
    bool Attach(AuxBusId bus); // 0x00460E64 | fefates:bytes [tier B]
    bool Enable(bool isEnabled); // 0x00460EBC | fefates:bytes [tier B]
    void Finalize(); // 0x00460F48 | fefates:bytes [tier B]
    bool SetParam(const Param& param); // 0x00461014 | fefates:bytes [tier B]

    void* m_Buffer;         // 0x0
    uptr m_DeviceAddress;   // 0x4
    size_t m_Size;          // 0x8
    bool m_IsInitialized;   // 0xC
    s8 m_AuxBus;            // 0xD
    bool m_IsEnabled;       // 0xE
    u8 m_DisabledFrame;     // 0xF
};
ASSERT_SIZE(DspFxReverb, 0x10);
} // namespace CTR
} // namespace snd
} // namespace nn
