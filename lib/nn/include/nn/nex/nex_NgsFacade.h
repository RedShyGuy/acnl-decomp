#pragma once

#include "decomp.h"
#include "nn/nex/nex_RendezVous.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex9NgsFacadeE @ 0x008CF768
// vtable 0x008FFD40 (vptr 0x008FFD48), offset_to_top 0, 5 entries
class NgsFacade : public ::nn::nex::RendezVous
{
public:
    virtual ~NgsFacade(); // 0x003D73D0 slot 0x00 | fefates:bytes
    // 0x003D73C0 slot 0x04 | slot vf_0x04 of nn::nex::BackEndServices (deleting dtor)
    void LoginAndRequestAuthenticationTokenImpl(nn::nex::ProtocolCallContext*, unsigned int, const wchar_t*, const char*, char*, const void*, int, unsigned int); // 0x003D6CC4 | fefates:bytes [tier B]
    void Login(nn::nex::ProtocolCallContext*, unsigned int, const wchar_t*, int, unsigned int); // 0x003D7164 | fefates:bytes [tier B]
    void Logout(nn::nex::ProtocolCallContext*); // 0x003D719C | fefates:bytes [tier B]
    NgsFacade(); // 0x003D725C | fefates:bytes-fuzzy [tier B]
    void GetLastLoginErrorCode() const; // 0x0072EA08 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
