#pragma once

#include "decomp.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex23JobBackEndServicesLoginE @ 0x008CECE8
// vtable 0x008FE534 (vptr 0x008FE53C), offset_to_top 0, 13 entries
class JobBackEndServicesLogin : public ::nn::nex::StepSequenceJob
{
public:
    JobBackEndServicesLogin(); // TODO: default ctor added so derived stubs compile - may not exist
    virtual ~JobBackEndServicesLogin(); // 0x003B0C9C slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003B0BB8 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void CancelJob(); // 0x003B0940 slot 0x24 | fefates:bytes
    void CompleteJob(nn::nex::qResult); // 0x003AF97C | fefates:bytes [tier B]
    void RegisterURLs(); // 0x003AFC10 | fefates:bytes [tier B]
    void CompleteLogout(); // 0x003AFFA0 | fefates:bytes [tier B]
    void ValidateArguments(); // 0x003AFFC4 | fefates:bytes [tier B]
    void ProcessAuthConnectionResult(); // 0x003B0050 | fefates:bytes [tier B]
    void ProcessAuthenticationResult(); // 0x003B0150 | fefates:bytes [tier B]
    void ConnectToAuthenticationService(); // 0x003B0298 | fefates:bytes [tier B]
    void ProcessSecConnConnectionResult(); // 0x003B0530 | fefates:bytes [tier B]
    void ConnectToSecureConnectionService(); // 0x003B0638 | fefates:bytes [tier B]
    void DisconnectFromAuthenticationService(); // 0x003B083C | fefates:bytes [tier B]
    JobBackEndServicesLogin(unsigned int, nn::nex::BackEndServices*, nn::nex::qResult*, const nn::nex::String&, const char*, const wchar_t*, unsigned short, nn::nex::Credentials**, nn::nex::String*, nn::nex::AnyObjectHolder<nn::nex::Data,nn::nex::String>*, int, nn::nex::StreamManager* (*)()); // 0x003B0A04 | fefates:bytes-fuzzy [tier B]
    void CreateStreamManager() const; // 0x0072D108 | fefates:bytes-fuzzy [tier B]
};
} // namespace nex
} // namespace nn
