#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class AccountManagementProtocolClient
{
public:
    void ProtoReturn_CreateAccount(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003C5560 | fefates:bytes [tier B]
    void ProtoReturn_CustomCreateAccount(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003C57A0 | fefates:bytes [tier B]
    void ProtoReturn_NintendoCreateAccount(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003C5840 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
