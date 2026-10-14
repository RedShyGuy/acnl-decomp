#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_TextMessage.h"

namespace nn {
namespace nex {
// (0x0051353C, named TextMessage::TextMessage() [ctor?] in symbols.json, takes three arguments and
// is no constructor of this class)
// RTTI N2nn3nex11TextMessageE @ 0x008CE098
// vtable 0x008FC464 (vptr 0x008FC46C), offset_to_top 0, 9 entries
class TextMessage : public ::nn::nex::_DDL_TextMessage
{
public:
    // (inline)
    TextMessage() {}
    virtual ~TextMessage(); // 0x0035C380 slot 0x00
    // 0x0035C340 slot 0x04 (deleting dtor)
    // (only the part of UserMessage; a nop in the original that falls into it)
    virtual void Trace() const; // 0x0035C3BC slot 0x20
};
ASSERT_SIZE(TextMessage, 0x50);
} // namespace nex
} // namespace nn
