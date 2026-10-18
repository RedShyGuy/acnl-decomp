#pragma once

#include "decomp.h"
#include "nn/nex/nex_SystemComponent.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
class CallContext;
class CallProtocolMethodOperation;
class EndPoint;
class Message;
class ProtocolCallContext;
class ProtocolRequestBrokerInterface;

// The base of the client and server protocols (RMC): a protocol has an id, its messages start with
// a protocol key (the id and whether it is a request) and the calls carry a call id. The static
// functions read and write the parts of the messages. The names of the members, of the enum values
// and of the functions marked so are ours.
// RTTI N2nn3nex8ProtocolE @ 0x008CF708
class Protocol : public ::nn::nex::SystemComponent
{
public:
    // the kind of a message (the request bit of the protocol key)
    enum _Command : u8
    {
        COMMAND_RESPONSE = 0,
        COMMAND_REQUEST = 1
    };
    enum _Type : u8
    {
        TYPE_CLIENT = 0,
        TYPE_SERVER = 1
    };
    // the flags (m_Flags)
    // the protocol registers with its broker when it is initialized
    static const u32 FLAG_REGISTER = 1 << 0;
    // the server protocol locks the scheduler while it dispatches (when not set)
    static const u32 FLAG_NO_LOCK = 1 << 1;
    static const u32 FLAG_UNKNOWN_8 = 1 << 3;
    // the highest protocol id that fits in the key byte
    static const u16 SHORT_PROTOCOL_ID = 0x7F;

    // (inline in the original; Protocol.cpp)
    explicit Protocol(u32 flags)
        : SystemComponent(String(L"<unknown>")), m_ProtocolId(0), m_pConnection(nullptr), m_pBroker(nullptr), m_Flags(flags), m_pEndPoint(nullptr),
          m_IsEndPointKept(false), m_PrincipalId(0), m_ConnectionId(0)
    {
    }

    virtual bool BeginInitialization(); // 0x003D5D8C slot 0x20 | fefates:bytes [tier B]
    virtual bool BeginTermination(); // 0x003D5BD0 slot 0x28 | fefates:bytes [tier B]
    virtual _Type GetProtocolType() const = 0; // slot 0x40
    virtual void vf_0x44(); // 0x003D5DF8 slot 0x44
    virtual void vf_0x48(); // 0x003D5A48 slot 0x48
    // (inline in the original, with a copy out of line)
    virtual nn::nex::Protocol* Clone(); // 0x0072E9E8 slot 0x4C

    // (armlink placed it far from the others)
    void SetFlag(u32 flag); // 0x003D5E10 (name is ours)
    // the call context of a response (by the call id in the message)
    static nn::nex::CallContext* RetrieveCallContext(nn::nex::Message* pMessage); // 0x00391B54 | fefates:bytes [tier B]
    static void AddMethodID(nn::nex::Message* pMessage, unsigned int methodId); // 0x003D5A30 | mk7dlp:bytes [tier A]
    static void AddProtocolKey(nn::nex::Message* pMessage, nn::nex::Protocol::_Command command, unsigned short protocolId); // 0x003D5A5C | fefates:bytes [tier B]
    // the outcome, the call id and (on success) the results of a call
    static void AddCallResponse(nn::nex::Message* pMessage, nn::nex::CallProtocolMethodOperation* pOperation, unsigned int callId, const nn::nex::Message& results); // 0x003D5AE8 | fefates:bytes [tier B]
    static u32 ExtractMethodID(nn::nex::Message* pMessage); // 0x003D5BBC | mk7dlp:callseq-callee [tier A]
    static void ExtractCallOutcome(nn::nex::Message* pMessage, nn::nex::qResult* pResult); // 0x003D5C00 | fefates:bytes [tier B]
    static void ExtractProtocolKey(nn::nex::Message* pMessage, nn::nex::Protocol::_Command& command, unsigned short& protocolId); // 0x003D5D08 | fefates:bytes [tier B]
    // the call starts and its id goes into the message
    // the id of the call (after RegisterCallContext added it)
    static u32 ExtractCallID(nn::nex::Message* pMessage); // 0x003D5DFC (name is ours)
    static bool RegisterCallContext(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x003D5DBC | mk7dlp:callseq-callee [tier A]
    // true if all the flags are set (inline in the original, with a copy out of line)
    bool FlagIsSet(unsigned int flags) const; // 0x0072E9F0 | fefates:bytes [tier B]

    u16 m_ProtocolId;                          // 0x20
    EndPoint* m_pConnection;                   // 0x24 (of the calls without credentials)
    ProtocolRequestBrokerInterface* m_pBroker; // 0x28
    u32 m_Flags;                               // 0x2C
    // the end point of the current call and the ids of its station
    EndPoint* m_pEndPoint;                     // 0x30
    bool m_IsEndPointKept;                     // 0x34
    u32 m_PrincipalId;                         // 0x38
    u32 m_ConnectionId;                        // 0x3C
};
ASSERT_OFFSET(Protocol, m_pEndPoint, 0x30);
ASSERT_SIZE(Protocol, 0x40);
} // namespace nex
} // namespace nn
