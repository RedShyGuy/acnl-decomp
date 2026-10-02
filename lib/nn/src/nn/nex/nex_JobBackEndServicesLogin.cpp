#include "nn/nex/nex_qResult.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_JobBackEndServicesLogin.h"

namespace nn {
namespace nex {
// TODO: default ctor added so derived stubs compile - may not exist
nn::nex::JobBackEndServicesLogin::JobBackEndServicesLogin()
{
}

// 0x003B0C9C slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
nn::nex::JobBackEndServicesLogin::~JobBackEndServicesLogin()
{
}

// 0x003B0940 slot 0x24 | fefates:bytes
void nn::nex::JobBackEndServicesLogin::CancelJob()
{
}

// 0x003AF97C | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogin::CompleteJob(nn::nex::qResult)
{
}

// 0x003AFC10 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogin::RegisterURLs()
{
}

// 0x003AFFA0 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogin::CompleteLogout()
{
}

// 0x003AFFC4 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogin::ValidateArguments()
{
}

// 0x003B0050 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogin::ProcessAuthConnectionResult()
{
}

// 0x003B0150 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogin::ProcessAuthenticationResult()
{
}

// 0x003B0298 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogin::ConnectToAuthenticationService()
{
}

// 0x003B0530 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogin::ProcessSecConnConnectionResult()
{
}

// 0x003B0638 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogin::ConnectToSecureConnectionService()
{
}

// 0x003B083C | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogin::DisconnectFromAuthenticationService()
{
}

// 0x003B0A04 | fefates:bytes-fuzzy [tier B]
nn::nex::JobBackEndServicesLogin::JobBackEndServicesLogin(unsigned int, nn::nex::BackEndServices*, nn::nex::qResult*, const nn::nex::String&, const char*, const wchar_t*, unsigned short, nn::nex::Credentials**, nn::nex::String*, nn::nex::AnyObjectHolder<nn::nex::Data,nn::nex::String>*, int, nn::nex::StreamManager* (*)())
{
}

// 0x0072D108 | fefates:bytes-fuzzy [tier B]
void nn::nex::JobBackEndServicesLogin::CreateStreamManager() const
{
}

} // namespace nex
} // namespace nn
