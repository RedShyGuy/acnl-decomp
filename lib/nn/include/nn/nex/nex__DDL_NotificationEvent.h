#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex22_DDL_NotificationEventE @ 0x008CECA4
// vtable 0x008FE21C (vptr 0x008FE224), offset_to_top 0, 2 entries
class _DDL_NotificationEvent : public ::nn::nex::RootObject
{
public:
    _DDL_NotificationEvent(); // ctor address unknown
    virtual void vf_0x00(); // 0x0039F438 slot 0x00 | virtual slot, introduced by nn::nex::_DDL_NotificationEvent
    virtual void vf_0x04(); // 0x0039F418 slot 0x04 | virtual slot, introduced by nn::nex::_DDL_NotificationEvent
    void Extract(nn::nex::Message*, nn::nex::_DDL_NotificationEvent*); // 0x0039F2EC | mk7dlp:callseq-callee [tier A]

    // (the layout is from the handlers of pia; the member names are ours) the type is
    // 1000 * type + subtype
    u32 m_SourcePid;        // 0x04
    u32 m_Unknown0x8;       // 0x08
    u32 m_Type;             // 0x0C
    u32 m_Param1;           // 0x10
    u32 m_Param2;           // 0x14
    nn::nex::String m_StringParam; // 0x18
};
ASSERT_OFFSET(_DDL_NotificationEvent, m_Type, 0xC);
ASSERT_OFFSET(_DDL_NotificationEvent, m_StringParam, 0x18);
} // namespace nex
} // namespace nn
