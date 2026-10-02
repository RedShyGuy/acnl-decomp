#pragma once

#include "decomp.h"
#include "nn/nex/nex_Gathering.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex24_DDL_PersistentGatheringE @ 0x008CEE5C
// vtable 0x008FE988 (vptr 0x008FE990), offset_to_top 0, 9 entries
class _DDL_PersistentGathering : public ::nn::nex::Gathering
{
public:
    _DDL_PersistentGathering(); // ctor address unknown
    virtual ~_DDL_PersistentGathering(); // 0x003B5F70 slot 0x00 | fefates:bytes
    // 0x003B5F60 slot 0x04 | slot vf_0x04 of nn::nex::_DDL_Gathering (deleting dtor)
    virtual void Clone() const; // 0x0072D608 slot 0x08 | fefates:callseq
    virtual void GetGatheringType() const; // 0x0072D5A0 slot 0x0C | fefates:bytes
    virtual void vf_0x10(); // 0x0072D5D0 slot 0x10 | virtual slot, introduced by nn::nex::_DDL_Gathering
    virtual void vf_0x14(); // 0x0072D628 slot 0x14 | virtual slot, introduced by nn::nex::_DDL_Gathering
    virtual void StreamIn(nn::nex::Message*) const; // 0x003B5BC4 slot 0x18 | slot vf_0x18 of nn::nex::_DDL_Gathering
    virtual void StreamOut(nn::nex::Message*); // 0x003B5DAC slot 0x1C | slot vf_0x1C of nn::nex::_DDL_Gathering
    void Extract(nn::nex::Message*, nn::nex::_DDL_PersistentGathering*); // 0x003B5DBC | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
