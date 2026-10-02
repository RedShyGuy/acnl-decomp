#pragma once

#include "decomp.h"
#include "nn/nex/nex_Gathering.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex21_DDL_MatchmakeSessionE @ 0x008CEB80
// vtable 0x008FDFC8 (vptr 0x008FDFD0), offset_to_top 0, 9 entries
class _DDL_MatchmakeSession : public ::nn::nex::Gathering
{
public:
    _DDL_MatchmakeSession(); // ctor address unknown
    virtual ~_DDL_MatchmakeSession(); // 0x0039BC30 slot 0x00 | fefates:bytes
    // 0x0039BC20 slot 0x04 | slot vf_0x04 of nn::nex::_DDL_Gathering (deleting dtor)
    virtual void Clone() const; // 0x0072D008 slot 0x08 | fefates:callseq
    virtual void GetGatheringType() const; // 0x0072CFA8 slot 0x0C | mk7dlp:bytes
    virtual void vf_0x10(); // 0x0072CFD4 slot 0x10 | virtual slot, introduced by nn::nex::_DDL_Gathering
    virtual void vf_0x14(); // 0x0072D028 slot 0x14 | virtual slot, introduced by nn::nex::_DDL_Gathering
    virtual void StreamIn(nn::nex::Message*) const; // 0x0039B71C slot 0x18 | slot vf_0x18 of nn::nex::_DDL_Gathering
    virtual void StreamOut(nn::nex::Message*); // 0x0039B9B8 slot 0x1C | slot vf_0x1C of nn::nex::_DDL_Gathering
    void Extract(nn::nex::Message*, nn::nex::_DDL_MatchmakeSession*); // 0x0039B9C8 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
