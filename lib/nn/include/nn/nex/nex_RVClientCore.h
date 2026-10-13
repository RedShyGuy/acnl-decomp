#pragma once

#include "decomp.h"
#include "nn/nex/nex_RefCountedObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex12RVClientCoreE @ 0x008CE168
// vtable 0x008FC624 (vptr 0x008FC62C), offset_to_top 0, 2 entries
class RVClientCore : public ::nn::nex::RefCountedObject
{
public:
    RVClientCore(); // ctor address unknown
    virtual ~RVClientCore(); // 0x0035EDC8 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x0035ED48 slot 0x04 | fefates:bytes (deleting dtor)
    static RVClientCore* GetInstance(); // 0x0035E8F4 | fefates:callgraph [tier C]
    // the server protocol with the id (not decompiled yet)
    nn::nex::ServerProtocol* FindProtocol(unsigned short id); // 0x0035EA4C (name is ours)

    // (only the members known so far; the names are ours)
    u8 m_Unknown0x9[3];   // 0x09
    ProtocolRegistry* m_pProtocolRegistry; // 0x0C
};
} // namespace nex
} // namespace nn
