#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class TicketManager
{
public:
    TicketManager(); // TODO: default ctor added so derived stubs compile - may not exist
    void InsertTicket(unsigned, nn::nex::Ticket*); // 0x0036EC1C | mk7dlp:bytes [tier A]
    void ReleaseTicket(nn::nex::Ticket*); // 0x0036F098 | fefates:bytes [tier B]
    TicketManager(nn::nex::AuthenticationClient*); // 0x0036F244 | mk7dlp:bytes [tier A]
};
} // namespace nex
} // namespace nn
