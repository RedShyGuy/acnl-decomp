#pragma once

#include "decomp.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
void CopyString(wchar_t**, const wchar_t*); // 0x00354EE0 | fefates:bytes [tier B]
void InsertToHistoryPacketQueue(nn::nex::qChain<nn::nex::Packet*,nn::nex::ChainPolicyHistoryPacket<nn::nex::Packet*>>*, nn::nex::PacketQueue*); // 0x003B9818 | fefates:bytes-fuzzy [tier B]
void InvokeCallbackOnCompletion(nn::nex::CallContext*, const nn::nex::UserContext*); // 0x003B9A9C | mk7dlp:bytes [tier A]
void hex(nn::nex::StringStream&); // 0x003CD054 | mk7dlp:bytes [tier B]
void CreateBunldingStreamFromHistoryPacketQueue(nn::nex::qChain<nn::nex::Packet*,nn::nex::ChainPolicyHistoryPacket<nn::nex::Packet*>>*, nn::nex::PacketOut*, nn::nex::Time, unsigned int, nn::nex::ByteStream*); // 0x003CD224 | fefates:bytes-fuzzy [tier B]
void operator+(const wchar_t*, const nn::nex::String&); // 0x003DA184 | fefates:bytes [tier B]
void operator+(const nn::nex::String&, const wchar_t*); // 0x003DA284 | fefates:bytes [tier B]
void operator+(const nn::nex::String&, const nn::nex::String&); // 0x003DA388 | fefates:bytes [tier B]
} // namespace nex
} // namespace nn
