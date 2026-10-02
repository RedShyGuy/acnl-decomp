#pragma once

#include "decomp.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
class JobBackEndServicesLoginWithData
{
public:
    void CompleteJob(nn::nex::qResult); // 0x003C5C18 | fefates:bytes [tier B]
    void Authenticate(); // 0x003C5D68 | fefates:bytes [tier B]
    void RegisterURLs(); // 0x003C5EAC | fefates:bytes [tier B]
    void CompleteLogout(); // 0x003C6194 | fefates:bytes [tier B]
    void ValidateArguments(); // 0x003C61B8 | fefates:bytes [tier B]
    void ProcessAuthConnectionResult(); // 0x003C6248 | fefates:bytes [tier B]
    void ProcessAuthenticationResult(); // 0x003C6348 | fefates:bytes [tier B]
    void ConnectToAuthenticationService(); // 0x003C6490 | fefates:bytes [tier B]
    void ProcessSecConnConnectionResult(); // 0x003C66B4 | fefates:bytes [tier B]
    void ConnectToSecureConnectionService(); // 0x003C67BC | fefates:bytes [tier B]
    void DisconnectFromAuthenticationService(); // 0x003C6A04 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
