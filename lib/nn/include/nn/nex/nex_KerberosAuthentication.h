#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class KerberosAuthentication
{
public:
    // checks the connection request (a ticket); the reply, the connection id and the ticket
    static bool ValidateConnectionRequest(nn::nex::BitStream* pRequest, nn::nex::BitStream* pReply, nn::nex::AuthenticationClient* pClient, unsigned int* pConnectionId, nn::nex::Ticket** ppTicket); // 0x0039D5A0 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
