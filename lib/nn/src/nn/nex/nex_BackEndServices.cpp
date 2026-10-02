#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_BackEndServices.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x00377100 (unverified)
nn::nex::BackEndServices::BackEndServices()
{
}

// 0x00377288 slot 0x00 | fefates:callseq
nn::nex::BackEndServices::~BackEndServices()
{
}

// 0x003765D4 slot 0x08 | fefates:bytes
void nn::nex::BackEndServices::PostLogoutCleanup()
{
}

// 0x00376964 slot 0x0C | fefates:bytes
void nn::nex::BackEndServices::RegisterServerProtocols()
{
}

// 0x003769C0 slot 0x10 | fefates:bytes
void nn::nex::BackEndServices::UnregisterServerProtocols()
{
}

// 0x00376098 | fefates:bytes [tier B]
void nn::nex::BackEndServices::LogoutImpl(nn::nex::CallContext*, nn::nex::Credentials*)
{
}

// 0x00376C00 | mk7dlp:bytes [tier A]
void nn::nex::BackEndServices::RegisterPreTerminateCallback(void(*)(nn::nex::CallContext*, const nn::nex::UserContext*), const nn::nex::UserContext&)
{
}

// 0x0037708C | fefates:bytes [tier B]
void nn::nex::BackEndServices::Terminate(nn::nex::CallContext*)
{
}

// 0x0072B5F8 | fefates:bytes [tier B]
void nn::nex::BackEndServices::LoginJobIsInProgress() const
{
}

// 0x0072B614 | fefates:bytes [tier B]
void nn::nex::BackEndServices::GetAuthenticationClient() const
{
}

// 0x0072B660 | fefates:bytes [tier B]
void nn::nex::BackEndServices::TerminateJobIsInProgress() const
{
}

// 0x0072B67C | fefates:bytes [tier B]
void nn::nex::BackEndServices::GetSecureConnectionClient() const
{
}

} // namespace nex
} // namespace nn
