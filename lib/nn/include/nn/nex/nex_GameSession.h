#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_GameSession.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex11GameSessionE @ 0x008CE00C
// vtable 0x008FC280 (vptr 0x008FC288), offset_to_top 0, 9 entries
class GameSession : public ::nn::nex::_DDL_GameSession
{
public:
    GameSession(); // ctor address unknown
    virtual ~GameSession(); // 0x003576DC slot 0x00 | slot vf_0x00 of nn::nex::_DDL_Gathering
    // 0x003576BC slot 0x04 | slot vf_0x04 of nn::nex::_DDL_Gathering (deleting dtor)
    virtual void Clone() const; // 0x0072B814 slot 0x08 | fefates:bytes
    virtual void GetGatheringType() const; // 0x0072B7CC slot 0x0C | mk7dlp:bytes
    virtual void vf_0x10(); // 0x0072B7EC slot 0x10 | virtual slot, introduced by nn::nex::_DDL_Gathering
    virtual void vf_0x14(); // 0x0072B8B4 slot 0x14 | virtual slot, introduced by nn::nex::_DDL_Gathering
    virtual void StreamIn(nn::nex::Message*) const; // 0x0072B844 slot 0x18 | fefates:bytes
    virtual void StreamOut(nn::nex::Message*); // 0x00383F88 slot 0x1C | fefates:bytes
};
} // namespace nex
} // namespace nn
