#pragma once

#include "decomp.h"
#include "nn/nex/nex_DateTime.h"
#include "nn/nex/nex_RefCountedObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex6TicketE @ 0x008CF5E8
// vtable 0x008FFA88 (vptr 0x008FFA90), offset_to_top 0, 2 entries
class Ticket : public ::nn::nex::RefCountedObject
{
public:
    Ticket(); // ctor candidate(s) 0x003D1764, 0x003D17B0 (unverified)
    virtual ~Ticket(); // 0x003D1838 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003D180C slot 0x04 | fefates:bytes (deleting dtor)
    void Decrypt(nn::nex::KerberosEncryption*, const nn::nex::Key&); // 0x003D1518 | fefates:bytes [tier B]
    Ticket(const nn::nex::Buffer&, unsigned int); // 0x003D1764 | fefates:bytes [tier B]
    Ticket(const nn::nex::Buffer&, unsigned int, unsigned int, const nn::nex::Key&, nn::nex::DateTime); // 0x003D17B0 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
