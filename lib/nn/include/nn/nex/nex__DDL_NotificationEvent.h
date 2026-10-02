#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

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
};
} // namespace nex
} // namespace nn
