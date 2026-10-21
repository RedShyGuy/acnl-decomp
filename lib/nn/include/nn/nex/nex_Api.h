#pragma once

#include "decomp.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_Packet.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_qChain.h"

namespace nn {
namespace nex {
// the packets of the queue are removed; the ones for the bundling history go to the front of the
// history (due again after the retransmission delay; the oldest leave a full history)
void InsertToHistoryPacketQueue(nn::nex::qChain<nn::nex::Packet*, nn::nex::ChainPolicyHistoryPacket<nn::nex::Packet*> >* pHistory, nn::nex::PacketQueue* pQueue); // 0x003B9818 | fefates:bytes-fuzzy [tier B]
// the packet leaves the history (its reference is released); the next one
nn::nex::qChain<nn::nex::Packet*, nn::nex::ChainPolicyHistoryPacket<nn::nex::Packet*> >::iterator PacketHistoryChainDequeue(nn::nex::qChain<nn::nex::Packet*, nn::nex::ChainPolicyHistoryPacket<nn::nex::Packet*> >* pHistory, nn::nex::qChain<nn::nex::Packet*, nn::nex::ChainPolicyHistoryPacket<nn::nex::Packet*> >::iterator it); // 0x003B8554 | fefates:bytes [tier B]
void InvokeCallbackOnCompletion(nn::nex::CallContext*, const nn::nex::UserContext*); // 0x003B9A9C | mk7dlp:bytes [tier A]
// switches the stream to hexadecimal numbers without padding
nn::nex::StringStream& hex(nn::nex::StringStream& stream); // 0x003CD054 | mk7dlp:bytes [tier B]
// hexadecimal numbers without "0x"
nn::nex::StringStream& noshowbase(nn::nex::StringStream& stream); // 0x0035697C | fefates:callgraph [tier C]
// switches the stream to zero padded hexadecimal numbers
nn::nex::StringStream& zerohex(nn::nex::StringStream& stream); // 0x003D47E8 | fefates:callgraph [tier C]
// the packets of the history (to the address of the packet, sent before the time) are written into
// the stream while they fit into size bytes; the number of packets (not decompiled yet)
u32 CreateBunldingStreamFromHistoryPacketQueue(nn::nex::qChain<nn::nex::Packet*, nn::nex::ChainPolicyHistoryPacket<nn::nex::Packet*> >* pHistory, nn::nex::PacketOut* pPacket, nn::nex::Time time, unsigned int size, nn::nex::ByteStream* pStream); // 0x003CD224 | fefates:bytes-fuzzy [tier B]
} // namespace nex
} // namespace nn
