#include "nn/nex/nex_ByteStream.h"
#include "nn/nex/nex_Message.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x003D23CC, 0x003D2490 (unverified)
nn::nex::Message::Message()
{
}

// 0x003D2600 slot 0x00 | fefates:bytes-fuzzy
nn::nex::Message::~Message()
{
}

// 0x003D2324 | fefates:bytes [tier B]
void nn::nex::Message::AddDataFromMessage(const nn::nex::Message&)
{
}

// 0x003D2348 | fefates:bytes [tier B]
void nn::nex::Message::SetSize()
{
}

// 0x003D23B8 | fefates:bytes [tier B]
void nn::nex::Message::GetBuffer()
{
}

// 0x003D23CC | fefates:bytes-fuzzy [tier B]
nn::nex::Message::Message(nn::nex::Buffer*)
{
}

} // namespace nex
} // namespace nn
