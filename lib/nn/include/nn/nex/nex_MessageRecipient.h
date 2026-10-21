#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_MessageRecipient.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex16MessageRecipientE @ 0x008CE5D8
// vtable 0x008FD1E0 (vptr 0x008FD1E8), offset_to_top 0, 2 entries
class MessageRecipient : public ::nn::nex::_DDL_MessageRecipient
{
public:
    // (inline)
    MessageRecipient() {}
    virtual ~MessageRecipient(); // 0x00382714 slot 0x00
    // 0x00382710 slot 0x04 (deleting dtor)
};
ASSERT_SIZE(MessageRecipient, 0x10);
} // namespace nex
} // namespace nn
