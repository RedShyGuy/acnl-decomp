#pragma once

#include "decomp.h"
#include "nn/nex/nex_EmulationDevice.h"
#include "nn/nex/nex_TimedQueue.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
// The bandwidth of the network emulation: the values leave one after the other, each after the
// time its size takes at the bandwidth of the device. The names of the members are ours.
// Instantiations found in the binary:
//   nn::nex::VirtualModem<nn::nex::NetworkEmulator::NetworkPacket>  typeinfo 0x008CE200  vtable 0x008FC784
template <typename T>
class VirtualModem : public ::nn::nex::TimedQueue<T>
{
public:
    // (inline)
    explicit VirtualModem(EmulationDevice* pDevice) : m_pDevice(pDevice), m_NextTime(SystemClock::GetTime()) {}
    // (the members are defined in nex_NetworkEmulator.cpp, the only instance)
    virtual ~VirtualModem();
    virtual void Queue(const T& value, nn::nex::Time time);

    EmulationDevice* m_pDevice; // 0x1C
    Time m_NextTime;            // 0x20 (when the line is free)
};
} // namespace nex
} // namespace nn
