#pragma once

#include "decomp.h"
#include "nn/nex/nex_ProtocolRequestBrokerInterface.h"
#include "nn/nex/nex_SingleThreadCallPolicy.h"
#include "nn/nex/nex_qMap.h"

namespace nn {
namespace nex {
class ServerProtocol;

// (out of line in the original)
template <>
DECOMP_NOINLINE qMap<u16, ServerProtocol*>::qMap();
template <>
DECOMP_NOINLINE qMap<u16, ServerProtocol*>::~qMap();

// The broker of the client: it processes the messages of the end points (call requests to the
// server protocols of this station, responses to the client protocols) and sends the responses.
// The names of the members and of the functions without a reference name are ours.
// RTTI N2nn3nex27ClientProtocolRequestBrokerE @ 0x008CEFF4
// vtable 0x008FEED0 (vptr 0x008FEED8), offset_to_top 0, 17 entries
class ClientProtocolRequestBroker : public ::nn::nex::ProtocolRequestBrokerInterface
{
public:
    // (inline, e.g. in JobBackEndServicesLogin::CreateStreamManager 0x0072D108)
    ClientProtocolRequestBroker() : m_pCurrentEndPoint(nullptr) { m_CallPolicy.m_Unknown0x0C = 3; }
    virtual ~ClientProtocolRequestBroker(); // 0x003BCBD0 slot 0x00 | slot vf_0x00 of nn::nex::ProtocolRequestBrokerInterface
    // 0x003BCBAC slot 0x04 (deleting dtor)
    virtual void ProcessMessage(nn::nex::CallProtocolMethodOperation*, nn::nex::EndPoint*, nn::nex::Buffer*); // 0x003BC368 slot 0x08 | fefates:bytes
    virtual void ProcessCallRequest(unsigned short, nn::nex::Message*, nn::nex::Message*, bool*, nn::nex::EndPoint*); // 0x003BC4B8 slot 0x0C | slot vf_0x0C of nn::nex::ProtocolRequestBrokerInterface
    virtual void ProcessCallResponse(unsigned short, nn::nex::Message*, nn::nex::EndPoint*); // 0x003BC9B4 slot 0x10 | fefates:bytes
    // (not supported by the client broker)
    virtual bool RegisterProtocol(nn::nex::Protocol*); // 0x003BC3F0 slot 0x14 | slot vf_0x14 of nn::nex::ProtocolRequestBrokerInterface
    virtual void UnregisterProtocol(nn::nex::Protocol*); // 0x003BC9B0 slot 0x18 | slot vf_0x18 of nn::nex::ProtocolRequestBrokerInterface
    virtual bool vf_0x1C(); // 0x003BC308 slot 0x1C | virtual slot, introduced by nn::nex::ProtocolRequestBrokerInterface
    virtual bool vf_0x20(); // 0x003BC310 slot 0x20 | virtual slot, introduced by nn::nex::ProtocolRequestBrokerInterface
    virtual bool RegisterServerProtocol(unsigned short id, nn::nex::ServerProtocol* pProtocol); // 0x003BC9F4 slot 0x24 (name is ours)
    virtual bool UnregisterServerProtocol(unsigned short id); // 0x003BCADC slot 0x28 (name is ours)
    virtual nn::nex::ServerProtocol* FindProtocol(unsigned short id); // 0x003BC3F8 slot 0x2C (name is ours)
    // (not supported by the client broker)
    virtual nn::nex::qResult ProcessLocalCall(nn::nex::Message* pMessage, nn::nex::Protocol* pClient, nn::nex::Protocol* pServer, u32 unknown); // 0x003BC384 slot 0x30 (name is ours)
    virtual nn::nex::qResult vf_0x34(); // 0x003BCAC0 slot 0x34 | virtual slot, introduced by nn::nex::ProtocolRequestBrokerInterface
    virtual nn::nex::qResult vf_0x38(); // 0x003BCB90 slot 0x38 | virtual slot, introduced by nn::nex::ProtocolRequestBrokerInterface
    // the calls of the end point are cancelled (the two slots do the same)
    virtual void PeerDisconnected(nn::nex::EndPoint* pEndPoint); // 0x003BC3A0 slot 0x3C (name is ours; the calls of the end point are cancelled)
    virtual void FaultDetection(nn::nex::EndPoint* pEndPoint, unsigned int error); // 0x003BC318 slot 0x40 (name is ours)

    // a request is dispatched and its response sent; a response goes to its client protocol
    void ProcessMessageCore(nn::nex::CallProtocolMethodOperation* pOperation, nn::nex::EndPoint* pEndPoint, nn::nex::Buffer* pBuffer); // 0x003BC634 | fefates:callgraph [tier C]

    qMap<u16, ServerProtocol*> m_ServerProtocols; // 0x60
    EndPoint* m_pCurrentEndPoint;                // 0x7C (of the message being processed)
    SingleThreadCallPolicy m_CallPolicy;         // 0x80
};
ASSERT_OFFSET(ClientProtocolRequestBroker, m_ServerProtocols, 0x60);
ASSERT_OFFSET(ClientProtocolRequestBroker, m_CallPolicy, 0x80);
ASSERT_SIZE(ClientProtocolRequestBroker, 0x94);

} // namespace nex
} // namespace nn

namespace std {
// (out of line in the original, with the insert inline)
template <>
DECOMP_NOINLINE nn::nex::ServerProtocol*& map<u16, nn::nex::ServerProtocol*, less<u16>, nn::nex::MemAllocator<pair<const u16, nn::nex::ServerProtocol*> > >::operator[](const u16& key);
} // namespace std
