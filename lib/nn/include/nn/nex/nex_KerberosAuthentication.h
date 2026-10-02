#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class KerberosAuthentication
{
public:
    void ValidateConnectionRequest(nn::nex::BitStream*, nn::nex::BitStream*, nn::nex::AuthenticationClient*, unsigned int*, nn::nex::Ticket**); // 0x0039D5A0 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
