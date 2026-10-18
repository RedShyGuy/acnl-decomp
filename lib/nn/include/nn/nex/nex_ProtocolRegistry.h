#pragma once

#include "decomp.h"
#include "nn/nex/nex_Protocol.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qMap.h"

namespace nn {
namespace nex {
// (out of line in the original; defined in nex_ProtocolRegistry.cpp)
template <>
DECOMP_NOINLINE qMap<u16, Protocol*>::qMap();
template <>
DECOMP_NOINLINE qMap<u16, Protocol*>::~qMap();

// The client and server protocols of the station by id (RVClientCore). The member names are ours.
// RTTI N2nn3nex16ProtocolRegistryE @ 0x008CE5FC
// vtable 0x008FD214 (vptr 0x008FD21C), offset_to_top 0, 2 entries
class ProtocolRegistry : public ::nn::nex::RootObject
{
public:
    // (inline in the constructor of RVClientCore)
    ProtocolRegistry() {}
    virtual ~ProtocolRegistry(); // 0x003837C0 slot 0x00 | fefates:bytes
    // 0x003837B0 slot 0x04 (deleting dtor)
    // the protocol with the id and type (null if there is none)
    nn::nex::Protocol* GetProtocol(unsigned short id, nn::nex::Protocol::_Type type); // 0x00383454 | fefates:callgraph [tier C]
    // the registry owns the protocol (a second one with the id is deleted)
    bool RegisterProtocol(nn::nex::Protocol* pProtocol); // 0x00383580
    bool RegisterProtocol(unsigned short id, nn::nex::Protocol* pProtocol); // 0x0038378C | fefates:bytes [tier B]

    qMap<u16, Protocol*> m_ClientProtocols; // 0x04
    qMap<u16, Protocol*> m_ServerProtocols; // 0x20
};
ASSERT_SIZE(ProtocolRegistry, 0x3C);
} // namespace nex
} // namespace nn
