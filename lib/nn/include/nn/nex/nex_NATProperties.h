#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// the transport adapter of the platform factory (PRUDPEndPoint.cpp; the name is ours)
extern void* g_pUnknown0x96CA28;

// RTTI N2nn3nex13NATPropertiesE @ 0x008CE23C
// vtable 0x008FC818 (vptr 0x008FC820), offset_to_top 0, 2 entries
class NATProperties : public ::nn::nex::RootObject
{
public:
    virtual ~NATProperties(); // 0x0036208C slot 0x00 | mk7dlp:bytes
    // 0x00362050 slot 0x04 (deleting dtor)
    NATProperties(); // 0x00361FF0 | fefates:bytes [tier B]
    // the mapping and the filtering of the NAT (3 while a global flag of nex is set)
    // (out of line copies of the inline functions; inline where the original inlines them:
    // GetNATMappingInline / GetNATFilteringInline)
    u8 GetNATMapping() const; // 0x0072A618
    u8 GetNATFiltering() const; // 0x0072A634
    // (inline) the mapping / filtering of the NAT (unknown while a custom transport adapter is set)
    u8 GetNATMappingInline() const { return g_pUnknown0x96CA28 != nullptr ? NAT_UNKNOWN : m_NatMapping; }
    u8 GetNATFilteringInline() const { return g_pUnknown0x96CA28 != nullptr ? NAT_UNKNOWN : m_NatFiltering; }

    // the value of an unknown mapping or filtering (the name is ours)
    static const u8 NAT_UNKNOWN = 3;

    // (names are ours; armlink placed them in front of String::operator=)
    void SetPrivateAddress(const nn::nex::String& address); // 0x00361FE8
    void SetPublicAddress(const nn::nex::String& address); // 0x003D147C

    // the layout is from the constructor; the member names are ours (the address and port of the
    // console, the ones the NAT check servers saw, and how the NAT maps the ports, after
    // pia::inet::NatPropertyDetecter)
    String m_PrivateAddress;  // 0x04
    u16 m_PrivatePort;        // 0x0C
    String m_PublicAddress;   // 0x10
    u16 m_PublicPort;         // 0x18
    u8 m_NatMapping;          // 0x1A
    u8 m_NatFiltering;        // 0x1B
    u32 m_PortIncrement;      // 0x1C
    u8 m_IsPortPreserved;     // 0x20
};
ASSERT_SIZE(NATProperties, 0x24);
} // namespace nex
} // namespace nn
