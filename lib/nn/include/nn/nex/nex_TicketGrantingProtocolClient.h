#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectHolder.h"
#include "nn/nex/nex_ClientProtocol.h"

namespace nn {
namespace nex {
class Buffer;
class Data;
class ProtocolCallContext;
class RVConnectionData;
class String;

// The client of the ticket granting protocol (the authentication server): the login returns the
// principal id, the ticket of the secure server, its connection data and the server name. The
// names of the members and of the functions without a reference name are ours.
// RTTI N2nn3nex28TicketGrantingProtocolClientE @ 0x008CF0C0
// vtable 0x008FF0C4 (vptr 0x008FF0CC), offset_to_top 0, 23 entries
class TicketGrantingProtocolClient : public ::nn::nex::ClientProtocol
{
public:
    // the methods of the protocol
    enum Method
    {
        METHOD_LOGIN = 1,
        METHOD_LOGIN_EX = 2,
        METHOD_REQUEST_TICKET = 3,
        METHOD_LOGIN_WITH_CONTEXT = 6,
        METHOD_COUNT = 6
    };
    // the extraction of the results of a method
    typedef void (*ProtoReturnFunction)(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext);

    // id: the protocol id (0: the responder, not registered)
    explicit TicketGrantingProtocolClient(unsigned short id); // 0x003BF6DC
    virtual ~TicketGrantingProtocolClient(); // 0x003BF758 slot 0x00
    // 0x003BF748 (deleting dtor)
    virtual void ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003BF08C slot 0x50 | fefates:bytes
    virtual nn::nex::ClientProtocol* CreateResponder() const; // 0x0072DE60 slot 0x54

    bool CallLogin(nn::nex::ProtocolCallContext* pContext, nn::nex::qResult* pResult, const nn::nex::String& name, unsigned int* pPrincipalId, nn::nex::Buffer* pTicket,
                   nn::nex::RVConnectionData* pConnectionData, nn::nex::String* pServerName); // 0x003BF538
    bool CallLoginEx(nn::nex::ProtocolCallContext* pContext, nn::nex::qResult* pResult, const nn::nex::String& name, unsigned int* pPrincipalId, nn::nex::Buffer* pTicket,
                     nn::nex::RVConnectionData* pConnectionData, nn::nex::String* pServerName, const nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>& data); // 0x003BE3CC
    bool CallLoginWithContext(nn::nex::ProtocolCallContext* pContext, nn::nex::qResult* pResult, const nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>& data,
                              unsigned int* pPrincipalId, nn::nex::Buffer* pTicket, nn::nex::RVConnectionData* pConnectionData); // 0x003BECD4
    // the request of a ticket for the user (the caller sends the message)
    static bool RequestTicket_PrepareRequest(nn::nex::ProtocolCallContext* pContext, nn::nex::Message* pMessage, nn::nex::qResult* pResult, const unsigned int& sourceId,
                                      const unsigned int& targetId, nn::nex::Buffer* pTicket); // 0x003BF438 | fefates:bytes [tier B]

    static void ProtoReturn_Login(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003BE594
    static void ProtoReturn_LoginEx(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003BE934
    static void ProtoReturn_RequestTicket(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003BEE84 | fefates:bytes [tier B]
    static void ProtoReturn_LoginWithContext(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003BF118 | fefates:bytes [tier B]

    // the extraction of the results of the methods (by the method id - 1; set by the calls)
    static ProtoReturnFunction s_ProtoReturnFunctions[METHOD_COUNT];
};
} // namespace nex
} // namespace nn
