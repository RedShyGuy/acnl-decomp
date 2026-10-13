#include "nn/nex/nex__DDL_BinaryMessage.h"
#include "nn/nex/nex_BinaryMessage.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::BinaryMessage::BinaryMessage()
{
}

// 0x00361C40 slot 0x00
// 0x00361C00 (deleting dtor)
nn::nex::BinaryMessage::~BinaryMessage()
{
}

// 0x00361B44 slot 0x20 | virtual slot, introduced by nn::nex::UserMessage
void nn::nex::BinaryMessage::vf_0x20()
{
}

} // namespace nex
} // namespace nn
