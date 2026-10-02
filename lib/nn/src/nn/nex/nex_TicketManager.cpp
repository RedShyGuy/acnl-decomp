#include "nn/nex/nex_TicketManager.h"

namespace nn {
namespace nex {
// TODO: default ctor added so derived stubs compile - may not exist
nn::nex::TicketManager::TicketManager()
{
}

// 0x0036EC1C | mk7dlp:bytes [tier A]
void nn::nex::TicketManager::InsertTicket(unsigned, nn::nex::Ticket*)
{
}

// 0x0036F098 | fefates:bytes [tier B]
void nn::nex::TicketManager::ReleaseTicket(nn::nex::Ticket*)
{
}

// 0x0036F244 | mk7dlp:bytes [tier A]
nn::nex::TicketManager::TicketManager(nn::nex::AuthenticationClient*)
{
}

} // namespace nex
} // namespace nn
