#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace nex {
class NATProperties;
} // namespace nex
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet11NatPropertyE @ 0x008CF824
// vtable 0x008FFEA4 (vptr 0x008FFEAC), offset_to_top 0, 3 entries
//
// The NAT of the console as nex detected it (nex::NATProperties). The layout is from the
// constructor; the member names are ours.
class NatProperty : public ::nn::pia::common::RootObject
{
public:
    NatProperty(); // 0x003E3B08 | fefates:bytes [tier B]
    virtual ~NatProperty(); // 0x003E3B38 slot 0x00
    // 0x003E3B34 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x003E3B04 slot 0x08

    void SetNexNatProperties(const nn::nex::NATProperties& properties); // 0x003E3A4C | fefates:bytes [tier B]

    u16 m_PrivatePort;     // 0x04
    u16 m_PublicPort;      // 0x06
    u8 m_NatMapping;       // 0x08
    u8 m_NatFiltering;     // 0x09
    u32 m_PortIncrement;   // 0x0C
    u8 m_IsPortPreserved;  // 0x10
};
ASSERT_SIZE(NatProperty, 0x14);
} // namespace inet
} // namespace pia
} // namespace nn
