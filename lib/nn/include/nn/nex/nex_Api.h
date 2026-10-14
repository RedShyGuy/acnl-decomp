#pragma once

#include "decomp.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
void InsertToHistoryPacketQueue(nn::nex::qChain<nn::nex::Packet*,nn::nex::ChainPolicyHistoryPacket<nn::nex::Packet*>>*, nn::nex::PacketQueue*); // 0x003B9818 | fefates:bytes-fuzzy [tier B]
void InvokeCallbackOnCompletion(nn::nex::CallContext*, const nn::nex::UserContext*); // 0x003B9A9C | mk7dlp:bytes [tier A]
// switches the stream to hexadecimal numbers without padding
nn::nex::StringStream& hex(nn::nex::StringStream& stream); // 0x003CD054 | mk7dlp:bytes [tier B]
// hexadecimal numbers without "0x"
nn::nex::StringStream& noshowbase(nn::nex::StringStream& stream); // 0x0035697C | fefates:callgraph [tier C]
// switches the stream to zero padded hexadecimal numbers
nn::nex::StringStream& zerohex(nn::nex::StringStream& stream); // 0x003D47E8 | fefates:callgraph [tier C]
void CreateBunldingStreamFromHistoryPacketQueue(nn::nex::qChain<nn::nex::Packet*,nn::nex::ChainPolicyHistoryPacket<nn::nex::Packet*>>*, nn::nex::PacketOut*, nn::nex::Time, unsigned int, nn::nex::ByteStream*); // 0x003CD224 | fefates:bytes-fuzzy [tier B]
} // namespace nex
} // namespace nn
