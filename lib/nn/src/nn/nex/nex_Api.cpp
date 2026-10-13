#include "nn/nex/nex_Api.h"
#include "nn/nex/nex_StringStream.h"

namespace nn {
namespace nex {
// 0x003B9818 | fefates:bytes-fuzzy [tier B]
void InsertToHistoryPacketQueue(nn::nex::qChain<nn::nex::Packet*,nn::nex::ChainPolicyHistoryPacket<nn::nex::Packet*>>*, nn::nex::PacketQueue*)
{
}

// 0x003B9A9C | mk7dlp:bytes [tier A]
void InvokeCallbackOnCompletion(nn::nex::CallContext*, const nn::nex::UserContext*)
{
}

// 0x003CD054 | mk7dlp:bytes [tier B]
nn::nex::StringStream& hex(nn::nex::StringStream& stream)
{
    stream.m_IsHex = true;
    stream.m_IsZeroPadded = false;
    return stream;
}

// 0x003CD224 | fefates:bytes-fuzzy [tier B]
void CreateBunldingStreamFromHistoryPacketQueue(nn::nex::qChain<nn::nex::Packet*,nn::nex::ChainPolicyHistoryPacket<nn::nex::Packet*>>*, nn::nex::PacketOut*, nn::nex::Time, unsigned int, nn::nex::ByteStream*)
{
}

} // namespace nex
} // namespace nn
