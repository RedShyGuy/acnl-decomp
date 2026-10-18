#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// A network interface of the console (the addresses in network byte order). The names of the
// members are ours.
class InterfaceInfo : public ::nn::nex::RootObject
{
public:
    // the flags (names after the text of Trace)
    static const u32 FLAG_POINT_TO_POINT = 1;
    static const u32 FLAG_BROADCAST = 2;
    static const u32 FLAG_LOOPBACK = 4;

    InterfaceInfo(); // 0x00361F24 | fefates:bytes [tier B]
    ~InterfaceInfo(); // 0x00361F64
    // the text of the interface for a trace (the trace itself is not in this build)
    void Trace(unsigned long long traceFlags); // 0x00361DDC | mk7dlp:bytes-fuzzy [tier A]

    u32 m_Address;          // 0x00
    u32 m_BroadcastAddress; // 0x04
    u32 m_Netmask;          // 0x08
    u32 m_Flags;            // 0x0C
    String m_Name;          // 0x10
};
ASSERT_SIZE(InterfaceInfo, 0x18);
} // namespace nex
} // namespace nn
