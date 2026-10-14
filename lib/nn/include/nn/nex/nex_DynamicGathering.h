#pragma once

#include "decomp.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex__DDL_DynamicGathering.h"

namespace nn {
namespace nex {
// A gathering of a type that the client does not know: the type name is kept in the object.
// (0x003870D0, named as a ctor candidate before, is MatchMakingClient::MatchMakingClient().)
// RTTI N2nn3nex16DynamicGatheringE @ 0x008CE578
// vtable 0x008FD0B4 (vptr 0x008FD0BC), offset_to_top 0, 9 entries
class DynamicGathering : public ::nn::nex::_DDL_DynamicGathering
{
public:
    // (inline)
    DynamicGathering() : m_GatheringType(L"DynamicGathering") {}
    virtual ~DynamicGathering(); // 0x0037F188 slot 0x00
    // 0x0037F140 slot 0x04 | fefates:bytes (deleting dtor)
    // the kept type name
    virtual nn::nex::String GetGatheringType() const; // 0x003D12D0 slot 0x0C

    String m_GatheringType; // 0x54 (name is ours)
};
ASSERT_OFFSET(DynamicGathering, m_GatheringType, 0x54);
ASSERT_SIZE(DynamicGathering, 0x5C);
} // namespace nex
} // namespace nn
