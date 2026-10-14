#pragma once

#include "decomp.h"
#include "nn/nex/nex_PRUDPMessageInterface.h"
#include "nn/nex/nex_Packet.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Stream.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
class RelayMessage;

// Passes the calls on to the message class of a packet: the relay for stream type 11 and relay
// messages, otherwise V0 or V1 by the message version of the packet (Packet +0x24) or by the magic
// of the received bytes. The names of the members are ours.
// RTTI N2nn3nex20PRUDPMessageSelectorE @ 0x008CE8D0
// vtable 0x008FD884 (vptr 0x008FD88C), offset_to_top 0, 9 entries
class PRUDPMessageSelector : public ::nn::nex::PRUDPMessageInterface, public ::nn::nex::RootObject
{
public:
    // the stream type of the relay
    static const u32 STREAM_TYPE_RELAY = 11;
    // a message must have this many bytes to be told apart
    static const u32 MIN_MESSAGE_SIZE = 10;

    PRUDPMessageSelector(); // 0x003976B4 (name is ours)
    virtual ~PRUDPMessageSelector(); // 0x003977F4 slot 0x00 | fefates:bytes
    // 0x003977C4 slot 0x04 (deleting dtor)
    virtual bool CalcSignature(nn::nex::PacketOut* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature); // 0x00397554 slot 0x08 | fefates:bytes
    virtual void CalcSignature(nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::Stream::Type type, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature); // 0x00397520 slot 0x0C | fefates:bytes
    virtual nn::nex::PRUDPMessageInterface::SignatureMethod DecideSignatureMethod(const nn::nex::Packet* pPacket, nn::nex::Stream::Type type, bool isOutgoing); // 0x003975BC slot 0x10 | fefates:bytes
    virtual int CalcExpectedSignature(const nn::nex::Packet* pPacket, nn::nex::PRUDPMessageInterface::SignatureMethod method, const nn::nex::Key* pKey, nn::nex::TransportSignatureGenerator* pGenerator, const nn::nex::SignatureBytes* pReceived, nn::nex::SignatureBytes& signature); // 0x00397588 slot 0x14 | fefates:bytes
    virtual bool Unpack(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int* pSize); // 0x0039760C slot 0x18 | fefates:bytes
    virtual bool FastUnpack(nn::nex::ByteStream* pStream, nn::nex::PacketIn* pPacket, unsigned int* pSize); // 0x00397478 slot 0x1C | fefates:bytes
    virtual bool Pack(nn::nex::PacketOut* pPacket, nn::nex::ByteStream* pStream); // 0x003975D4 slot 0x20 | fefates:bytes

    // (inline; the name is ours) the message class of the version of a packet
    PRUDPMessageInterface* GetMessage(const Packet* pPacket) { return m_Messages[pPacket->m_MessageVersion]; }

    qVector<PRUDPMessageInterface*> m_Messages; // 0x04 (V0, V1)
    RelayMessage* m_pRelayMessage;              // 0x10
};
ASSERT_OFFSET(PRUDPMessageSelector, m_pRelayMessage, 0x10);
ASSERT_SIZE(PRUDPMessageSelector, 0x14);
} // namespace nex
} // namespace nn
