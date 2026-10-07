#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet12NatProbeDataE @ 0x008CF83C
// vtable 0x008FFEE0 (vptr 0x008FFEE8), offset_to_top 0, 2 entries
//
// The content of a NAT traversal probe (16 bytes when serialized). The layout is from the
// constructor and Deserialize; the member names are ours.
class NatProbeData : public ::nn::pia::common::RootObject
{
public:
    static const unsigned int SERIALIZED_SIZE = 16;
    // m_Type
    static const u8 TYPE_PROBE = 0;
    static const u8 TYPE_REPLY = 1;

    NatProbeData(); // 0x003E4528 | fefates:bytes [tier B]
    virtual ~NatProbeData(); // 0x003E4550 slot 0x00
    // 0x003E454C slot 0x04 (deleting dtor)

    nn::Result Serialize(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const; // 0x0072EF60 | fefates:bytes [tier B]
    nn::Result Deserialize(const unsigned char* pBuffer, unsigned int size); // 0x003E44B8 | fefates:bytes [tier B]

    // the station key of the sender (NexNatTraversalProtocol::m_LocalCid)
    u32 m_StationKey;   // 0x04
    u8 m_Type;    // 0x08
    u8 m_Unknown0x9;    // 0x09
    u8 m_Unknown0xA;    // 0x0A
    u8 m_Unknown0xB;    // 0x0B
    // the send time of the probe (a reply returns it)
    u64 m_SendTime;  // 0x10
};
ASSERT_SIZE(NatProbeData, 0x18);
} // namespace inet
} // namespace pia
} // namespace nn
