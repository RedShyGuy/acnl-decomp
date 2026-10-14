#include "nn/nex/nex_TextMessage.h"

namespace nn {
namespace nex {
// 0x0035C380 slot 0x00
// 0x0035C340 (deleting dtor)
nn::nex::TextMessage::~TextMessage()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0035C3BC slot 0x20
void nn::nex::TextMessage::Trace() const
{
    UserMessage::Trace();
}

} // namespace nex
} // namespace nn
