#pragma once

#include "decomp.h"
#include "nn/nex/nex_RefCountedObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex11CredentialsE @ 0x008CE000
// vtable 0x008FC270 (vptr 0x008FC278), offset_to_top 0, 2 entries
class Credentials : public ::nn::nex::RefCountedObject
{
public:
    Credentials(); // ctor candidate(s) 0x00376F5C (unverified)
    virtual ~Credentials(); // 0x00357650 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003575E0 slot 0x04 | fefates:bytes (deleting dtor)
    void MarkInvalid(); // 0x00137B04 | mk7dlp:bytes [tier B]
    void GetConnection(unsigned short) const; // 0x0072A100 | fefates:bytes [tier B]

    // (only the member pia uses; the name is ours, after pia::inet::NatTraverser)
    u8 m_Unknown0x9[3];   // 0x9
    u32 m_PrincipalId;    // 0xC
};
} // namespace nex
} // namespace nn
