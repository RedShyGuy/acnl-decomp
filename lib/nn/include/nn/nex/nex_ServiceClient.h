#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
class ClientProtocol;
class Credentials;
class EndPoint;
class ProtocolCallContext;
class ProtocolRegistry;

// The base of the clients of a back end: the default credentials (with the connections) of its
// protocols. The names of the members and of the functions without a reference name are ours.
// RTTI N2nn3nex13ServiceClientE @ 0x008CE290
// vtable 0x008FC990 (vptr 0x008FC998), offset_to_top 0, 10 entries
class ServiceClient : public ::nn::nex::RootObject
{
public:
    // id: the id of the client
    explicit ServiceClient(unsigned int id); // 0x0036D600
    virtual ~ServiceClient(); // 0x0036D6F4 slot 0x00 | fefates:bytes
    // 0x0036D6E4 slot 0x04 (deleting dtor)
    virtual void vf_0x08(); // 0x0036D484 slot 0x08
    virtual bool Bind(nn::nex::Credentials* pCredentials); // 0x0036D528 slot 0x0C
    virtual bool Unbind(); // 0x0036D5A4 slot 0x10
    virtual bool IsConnected() const; // 0x0072AA08 slot 0x14
    virtual bool IsFaulty() const; // 0x0072AB28 slot 0x18 | fefates:bytes
    virtual void ConnectionStateHasChanged(); // 0x0036D524 slot 0x1C
    virtual void UpdateProtocolsDefaultCredentials(nn::nex::Credentials* pCredentials) = 0; // slot 0x20
    // the call goes to the connection of its credentials (or of the default ones); false if there
    // is none
    virtual bool SetCallOwner(nn::nex::ProtocolCallContext* pContext); // 0x0072AAEC slot 0x24
    void SetDefaultCredentials(nn::nex::Credentials* pCredentials); // 0x0036D488 | fefates:bytes [tier B]
    // false if the registry does not take the responder (only a nop in the original at the end that
    // falls into ProtocolRegistry::RegisterProtocol)
    bool RegisterProtocol(nn::nex::ClientProtocol* pProtocol); // 0x00383520 | fefates:bytes [tier B]
    // (inline; nothing is left of it in this build)
    bool UnregisterProtocol(nn::nex::ClientProtocol* pProtocol) { return true; }
    // the connection of the stream type (10: the secure one)
    nn::nex::EndPoint* GetConnection(unsigned short streamType) const; // 0x0072AAA4 | fefates:bytes [tier B]
    nn::nex::ProtocolRegistry* GetProtocolRegistry() const; // 0x0072AADC

    u32 m_Id;                             // 0x04
    Credentials* m_pDefaultCredentials;   // 0x08
    bool m_IsLocal;                       // 0x0C (no connections)
};
ASSERT_SIZE(ServiceClient, 0x10);
} // namespace nex
} // namespace nn
