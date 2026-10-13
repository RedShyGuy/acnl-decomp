#include "nn/nex/nex__DDL_UserMessage.h"
#include "nn/nex/nex_UserMessage.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::UserMessage::UserMessage()
{
}

// 0x0035C418 slot 0x00
// 0x0035C3F0 (deleting dtor)
nn::nex::UserMessage::~UserMessage()
{
}

// 0x0035C3C0 slot 0x20 | virtual slot, introduced by nn::nex::UserMessage
void nn::nex::UserMessage::vf_0x20()
{
}

} // namespace nex
} // namespace nn
