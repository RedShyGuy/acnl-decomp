#pragma once

#include "decomp.h"
#include "nn/nex/nex_AccountManagementProtocolClient.h"
#include "nn/nex/nex_ServiceClient.h"

namespace nn {
namespace nex {
// The service client of the account management protocol, as JobCreateAccount uses it. It has no
// RTTI and no functions in this build (the name and the layout beyond the protocol are ours).
class AccountManagementClient : public ::nn::nex::ServiceClient
{
public:
    // (the protocol client has no ExtractCallSpecificResults in this build, so it cannot be a member
    // object here: its storage)
    AccountManagementProtocolClient* GetProtocol() { return reinterpret_cast<AccountManagementProtocolClient*>(m_ProtocolStorage); }

    u32 m_ProtocolStorage[0x44 / sizeof(u32)]; // 0x10 (AccountManagementProtocolClient)
};
} // namespace nex
} // namespace nn
