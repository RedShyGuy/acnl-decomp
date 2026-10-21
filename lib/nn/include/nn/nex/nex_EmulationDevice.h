#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// A device of the network emulation of a transport (one for the input and one for the output,
// RootTransport +0x5C/+0x78). The names of the members and functions are ours.
// RTTI N2nn3nex15EmulationDeviceE @ 0x008CE490
class EmulationDevice : public ::nn::nex::RootObject
{
public:
    // (inline)
    EmulationDevice() : m_IsEnabled(false), m_Unknown0x08(0), m_Bandwidth(-1), m_Latency(0), m_Jitter(0), m_LossRate(0.0f) {}
    // (inline in the derived destructors)
    virtual ~EmulationDevice() {}
    // (shared by the derived classes)
    virtual void Enable(); // 0x00377650 slot 0x08

    bool m_IsEnabled;    // 0x04
    u32 m_Unknown0x08;   // 0x08
    s32 m_Bandwidth;     // 0x0C (bits per second; -1: no limit)
    s32 m_Latency;       // 0x10 (milliseconds)
    s32 m_Jitter;        // 0x14 (milliseconds; a random delay below it is added)
    f32 m_LossRate;      // 0x18 (the part of the packets that are lost; StreamManager sets it for the input from a global)
};
ASSERT_SIZE(EmulationDevice, 0x1C);
} // namespace nex
} // namespace nn
