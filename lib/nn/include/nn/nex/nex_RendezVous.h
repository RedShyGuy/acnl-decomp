#pragma once

#include "decomp.h"
#include "nn/nex/nex_BackEndServices.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex10RendezVousE @ 0x008CDFBC
// vtable 0x008FC1C0 (vptr 0x008FC1C8), offset_to_top 0, 5 entries
class RendezVous : public ::nn::nex::BackEndServices
{
public:
    RendezVous(); // ctor candidate(s) 0x00355168 (unverified)
    virtual ~RendezVous(); // 0x00377284 slot 0x00 | fefates:callgraph
    // 0x00355180 slot 0x04 | slot vf_0x04 of nn::nex::BackEndServices (deleting dtor)
    void Login(nn::nex::CallContext*, const wchar_t*, const char*, const wchar_t*, unsigned short, const nn::nex::AnyObjectHolder<nn::nex::Data,nn::nex::String>&, nn::nex::Credentials**, nn::nex::String*); // 0x00355108 | fefates:bytes [tier B]
    void Logout(nn::nex::CallContext*, nn::nex::Credentials*); // 0x00376C50 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
