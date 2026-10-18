#pragma once

#include "decomp.h"
#include "nn/nex/nex_EmulationDevice.h"
#include "nn/nex/nex_TimedQueue.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
// The internet of the network emulation: values are lost at the loss rate of the device and are due
// after its latency and a random jitter. The names of the members are ours.
// Instantiations found in the binary:
//   nn::nex::VirtualInternet<nn::nex::NetworkEmulator::NetworkPacket>  typeinfo 0x008CE52C  vtable 0x008FD04C
template <typename T>
class VirtualInternet : public ::nn::nex::TimedQueue<T>
{
public:
    // (inline)
    explicit VirtualInternet(EmulationDevice* pDevice) : m_pDevice(pDevice) {}
    // (the members are defined in nex_NetworkEmulator.cpp, the only instance)
    virtual ~VirtualInternet();
    virtual void Queue(const T& value, nn::nex::Time time);

    EmulationDevice* m_pDevice; // 0x1C
};
} // namespace nex
} // namespace nn
