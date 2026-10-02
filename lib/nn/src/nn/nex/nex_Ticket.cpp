#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_DateTime.h"
#include "nn/nex/nex_Ticket.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x003D1764, 0x003D17B0 (unverified)
nn::nex::Ticket::Ticket()
{
}

// 0x003D1838 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
nn::nex::Ticket::~Ticket()
{
}

// 0x003D1518 | fefates:bytes [tier B]
void nn::nex::Ticket::Decrypt(nn::nex::KerberosEncryption*, const nn::nex::Key&)
{
}

// 0x003D1764 | fefates:bytes [tier B]
nn::nex::Ticket::Ticket(const nn::nex::Buffer&, unsigned int)
{
}

// 0x003D17B0 | fefates:bytes [tier B]
nn::nex::Ticket::Ticket(const nn::nex::Buffer&, unsigned int, unsigned int, const nn::nex::Key&, nn::nex::DateTime)
{
}

} // namespace nex
} // namespace nn
