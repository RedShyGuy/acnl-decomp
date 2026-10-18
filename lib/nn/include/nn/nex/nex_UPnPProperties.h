#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The UPnP properties of a transport (RootTransport +0xF0). The layout is from the constructor of
// RootTransport; the member names are ours.
// RTTI N2nn3nex14UPnPPropertiesE @ 0x008CE418
// vtable 0x008FCDDC (vptr 0x008FCDE4), offset_to_top 0, 2 entries
class UPnPProperties : public ::nn::nex::RootObject
{
public:
    // (inline in the constructor of RootTransport)
    UPnPProperties()
        : m_Unknown0x04(0), m_Unknown0x06(0), m_Unknown0x08(1), m_Unknown0x09(0), m_Unknown0x0A(0), m_Unknown0x0B(0)
    {
    }
    virtual ~UPnPProperties(); // 0x00375BA8 slot 0x00
    // 0x00375B90 slot 0x04 (deleting dtor)

    u8 m_Unknown0x04;  // 0x04
    u16 m_Unknown0x06; // 0x06
    u8 m_Unknown0x08;  // 0x08
    u8 m_Unknown0x09;  // 0x09 (NgsFacade clears it)
    u8 m_Unknown0x0A;  // 0x0A
    u8 m_Unknown0x0B;  // 0x0B
};
ASSERT_SIZE(UPnPProperties, 0xC);
} // namespace nex
} // namespace nn
