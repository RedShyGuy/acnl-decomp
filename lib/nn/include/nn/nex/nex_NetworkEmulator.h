#pragma once

#include "decomp.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_EmulationDevice.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_VirtualInternet.h"
#include "nn/nex/nex_VirtualModem.h"

namespace nn {
namespace nex {
// The emulation of a network for the packets of a socket (one for the sent and one for the received
// packets): the packets go through the modem (bandwidth) and then the internet (loss, latency,
// jitter) of the device. The names of the members are ours.
class NetworkEmulator
{
public:
    // a packet and its address (the name is from the RTTI of the templates)
    struct NetworkPacket
    {
        // (inline)
        NetworkPacket(Buffer* pBuffer, const InetAddress& address) : m_pBuffer(pBuffer)
        {
            m_pBuffer->AcquireRef();
            m_Address = address;
        }
        NetworkPacket(const NetworkPacket& other) : m_pBuffer(other.m_pBuffer)
        {
            m_pBuffer->AcquireRef();
            m_Address = other.m_Address;
        }
        ~NetworkPacket() { m_pBuffer->ReleaseRef(); }
        u32 GetSize() const { return m_pBuffer->m_ContentSize; }

        Buffer* m_pBuffer;     // 0x00
        InetAddress m_Address; // 0x04
    };

    NetworkEmulator(nn::nex::EmulationDevice* pDevice); // 0x0037AD74 | mk7dlp:callseq [tier A]
    // the packet is sent to the address through the emulation
    void Queue(nn::nex::Buffer* pBuffer, nn::nex::InetAddress& address); // 0x0037A6AC | fefates:bytes [tier B]
    // the packet that came through is removed (the buffer is not used)
    void Dequeue(nn::nex::Buffer* pBuffer); // 0x0037A7C4 | fefates:bytes [tier B]
    // the first packet that came through (false if none is due)
    bool GetHead(nn::nex::Buffer** ppBuffer, nn::nex::InetAddress* pAddress); // 0x0037A8AC | fefates:callgraph [tier C]

    u64 m_Unknown0x00;                       // 0x00 (not set)
    VirtualModem<NetworkPacket> m_Modem;     // 0x08
    VirtualInternet<NetworkPacket> m_Internet; // 0x30
    EmulationDevice* m_pDevice;              // 0x50
};
ASSERT_OFFSET(NetworkEmulator, m_Modem, 0x08);
ASSERT_OFFSET(NetworkEmulator, m_Internet, 0x30);
ASSERT_OFFSET(NetworkEmulator, m_pDevice, 0x50);
ASSERT_SIZE(NetworkEmulator, 0x58);
} // namespace nex
} // namespace nn
