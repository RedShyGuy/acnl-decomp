#pragma once

#include "decomp.h"
#include "nn/nex/nex_BackEndServices.h"

namespace nn {
namespace nex {
// The login and logout of BackEndServices with the operations.
// RTTI N2nn3nex10RendezVousE @ 0x008CDFBC
// vtable 0x008FC1C0 (vptr 0x008FC1C8), offset_to_top 0, 5 entries
class RendezVous : public ::nn::nex::BackEndServices
{
public:
    RendezVous(); // 0x00355168
    // (the complete destructor is only a nop in the original that falls into the one of
    // BackEndServices)
    virtual ~RendezVous(); // 0x00377284 slot 0x00 | fefates:callgraph
    // 0x00355180 (deleting dtor)
    bool Login(nn::nex::CallContext* pContext, const wchar_t* pName, const char* pPassword, const wchar_t* pAddress, unsigned short port,
               const nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>& data, nn::nex::Credentials** ppCredentials, nn::nex::String* pServerName); // 0x00355108 | fefates:bytes [tier B]
    // (only a nop in the original after OnLogoutStart that falls into BackEndServices::Logout_)
    bool Logout(nn::nex::CallContext* pContext, nn::nex::Credentials* pCredentials); // 0x00376C50 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
