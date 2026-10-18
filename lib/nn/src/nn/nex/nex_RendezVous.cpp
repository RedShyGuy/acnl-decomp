#include "nn/nex/nex_RendezVous.h"

namespace nn {
namespace nex {
// 0x00355108 | fefates:bytes [tier B]
bool nn::nex::RendezVous::Login(nn::nex::CallContext* pContext, const wchar_t* pName, const char* pPassword, const wchar_t* pAddress, unsigned short port,
                                const nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>& data, nn::nex::Credentials** ppCredentials, nn::nex::String* pServerName)
{
    OnLoginStart(*pContext);
    return LoginImpl(pContext, nullptr, pName, pPassword, pAddress, port, ppCredentials, pServerName, const_cast<AnyObjectHolder<Data, String>*>(&data));
}

// 0x00355168
nn::nex::RendezVous::RendezVous()
{
    // nothing to do
}

// 0x00377284 slot 0x00 | fefates:callgraph
// 0x00355180 (deleting dtor)
nn::nex::RendezVous::~RendezVous()
{
    // nothing to do
}

// 0x00376C50 | fefates:bytes [tier B]
bool nn::nex::RendezVous::Logout(nn::nex::CallContext* pContext, nn::nex::Credentials* pCredentials)
{
    OnLogoutStart(*pContext);
    return Logout_(pContext, pCredentials);
}

} // namespace nex
} // namespace nn
