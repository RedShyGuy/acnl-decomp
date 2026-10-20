#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_ConnectionData.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex14ConnectionDataE @ 0x008CE2F0
// vtable 0x008FCB00 (vptr 0x008FCB08), offset_to_top 0, 2 entries
class ConnectionData : public ::nn::nex::_DDL_ConnectionData
{
public:
    // (inline)
    ConnectionData() {}
    virtual ~ConnectionData(); // 0x00371020 slot 0x00
    // 0x00371000 slot 0x04 (deleting dtor)
};
ASSERT_SIZE(ConnectionData, 0x74);

// (out of line in the original; nex_ConnectionData.cpp)
template <>
DECOMP_NOINLINE qList<ConnectionData>::qList();
} // namespace nex
} // namespace nn

namespace std {
template <>
DECOMP_NOINLINE list<nn::nex::ConnectionData, nn::nex::MemAllocator<nn::nex::ConnectionData> >::~list();
} // namespace std
