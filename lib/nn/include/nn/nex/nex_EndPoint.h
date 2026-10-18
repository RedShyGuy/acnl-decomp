#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_EndPointInfoInterface.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_UserContext.h"
#include "nn/nex/nex_qResult.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
class Buffer;
class ConnectionOrientedStream;
class EndPointEventHandler;
class Key;

// The object an end point reports its failure to (EndPoint::SetFaultResultHolder): the first failure
// is kept. Its class is not known (the name and the members are ours).
struct FaultResultHolder
{
    // (inline) no failure yet
    bool IsSuccess()
    {
        ScopedCS lock(m_Lock);
        return m_Result.m_Code >= 0;
    }
    // (inline) keeps the result if there is no failure yet
    void SetResult(const qResult& result)
    {
        ScopedCS lock(m_Lock);
        if (m_Result.m_Code >= 0) {
            m_Result = result;
        }
    }

    void* m_pVtable;          // 0x00
    CriticalSection m_Lock;   // 0x04
    qResult m_Result;         // 0x2C
};

// A connection of a ConnectionOrientedStream to a station. The names of the members and of the
// functions without a reference name are ours (the names of the states after the strings
// "IsConnecting()" etc. in the binary).
// RTTI N2nn3nex8EndPointE @ 0x008CF6B0
// vtable 0x008FFBB0 (vptr 0x008FFBB8), offset_to_top 0, 37 entries
class EndPoint : public ::nn::nex::EndPointInfoInterface, public ::nn::nex::RootObject
{
public:
    enum _ConnectionState : u8
    {
        STATE_NOT_CONNECTED = 0,
        STATE_CONNECTING = 1,
        STATE_CONNECTED = 2,
        STATE_DISCONNECTING = 3,
        STATE_FAULTY = 4
    };
    // the function called when a connection or disconnection completes
    typedef void (*CompletionCallback)(EndPoint* pEndPoint, qResult result, const UserContext* pContext);

    EndPoint(nn::nex::ConnectionOrientedStream* pStream, const nn::nex::StationURL* pURL); // 0x003D4DAC | fefates:bytes [tier B]
    virtual ~EndPoint(); // 0x003D4E24 slot 0x00 | mk7dlp:bytes-fuzzy
    // 0x003D4DF4 slot 0x04 (deleting dtor)
    virtual void ResetStatistics() = 0; // slot 0x08
    virtual bool GetStatistics(void* pStatistics) = 0; // slot 0x0C
    virtual u32 GetPendingReliablePacketCount() = 0; // slot 0x10
    virtual void GetPendingReliablePacketCounts(nn::nex::qVector<u32>* pCounts) = 0; // slot 0x14
    virtual u16 GetMaxSubstreamId() = 0; // slot 0x18
    virtual bool IsNotConnected() = 0; // slot 0x1C
    virtual bool IsConnecting() = 0; // slot 0x20
    virtual bool IsDisconnecting() = 0; // slot 0x24
    virtual bool IsFaulty() = 0; // slot 0x28
    virtual bool IsConnected() = 0; // slot 0x2C
    virtual bool PeerIsConnected() = 0; // slot 0x30
    virtual bool PeerIsNotConnected() = 0; // slot 0x34
    virtual void DisableKeepAlive(); // 0x003D4D28 slot 0x38 | slot vf_0x38 of nn::nex::EndPoint
    virtual void SetKeepAliveTimeout(unsigned int milliseconds) = 0; // slot 0x3C
    virtual void SetMaxSilenceTime(unsigned int milliseconds) = 0; // slot 0x40
    virtual u32 GetKeepAliveTimeout() = 0; // slot 0x44
    virtual u32 GetMaxSilenceTime() = 0; // slot 0x48
    virtual void SetPeerConnected() = 0; // slot 0x4C
    virtual void SetPeerDisconnected() = 0; // slot 0x50
    virtual u8 GetConnectionState() const = 0; // slot 0x54
    virtual bool SetConnectionState(nn::nex::EndPoint::_ConnectionState state) = 0; // slot 0x58
    virtual nn::nex::qResult RegisterEventHandler(nn::nex::EndPointEventHandler* pHandler); // 0x003D4D48 slot 0x5C | mk7dlp:callseq-callee
    virtual bool IsAnyPacketProcessed() = 0; // slot 0x60
    virtual u32 GetSignaledFaultError() const = 0; // slot 0x64
    virtual u32 GetLastRTT() = 0; // slot 0x68
    virtual u32 GetRTO() = 0; // slot 0x6C
    virtual nn::nex::qResult _Connect(nn::nex::Buffer* pConnectData, nn::nex::Buffer* pReplyData, CompletionCallback pCallback, const nn::nex::UserContext& context, unsigned int timeout) = 0; // slot 0x70
    virtual nn::nex::qResult _Disconnect(CompletionCallback pCallback, const nn::nex::UserContext& context, unsigned int timeout) = 0; // slot 0x74
    virtual nn::nex::qResult _Send(nn::nex::Buffer* pBuffer, unsigned int flags, bool copy, unsigned char substreamId, unsigned int id) = 0; // slot 0x78
    virtual u64 GetSupportedFunctionsFlags() const = 0; // slot 0x7C
    virtual void SetFaultResultHolder(nn::nex::FaultResultHolder* pHolder); // 0x003D4C68 slot 0x80
    virtual void ClearFaultResultHolder(); // 0x003D4D3C slot 0x84
    virtual void SignalFaultEvent(unsigned int error, bool isProcessed); // 0x003D4D38 slot 0x88 | slot vf_0x88 of nn::nex::EndPoint
    virtual void SetEncryptionKey(const nn::nex::Key& key) = 0; // slot 0x8C
    virtual const nn::nex::Key& GetEncryptionKey() = 0; // slot 0x90

    void SetPrincipalID(unsigned principalId); // 0x003D4C70 | mk7dlp:bytes-fuzzy [tier A]
    void SetConnectionID(unsigned connectionId); // 0x003D4CCC | mk7dlp:bytes-fuzzy [tier A]
    // its stream opens it (one more reference)
    void Open(); // 0x003D4D84 | mk7dlp:bytes [tier A]
    // its stream closes it
    void Close(); // 0x003D4D98 | fefates:callgraph [tier C]

    ConnectionOrientedStream* m_pStream;   // 0x04
    StationURL m_URL;                      // 0x08
    EndPointEventHandler* m_pEventHandler; // 0x70
    EndPoint* m_pOwner;                    // 0x74 (the SecureEndPoint above it)
    u32 m_ConnectionId;                    // 0x78
    u32 m_PrincipalId;                     // 0x7C
    u32 m_OpenCount;                       // 0x80 (Open / Close)
    u8 m_Unknown0x84;                      // 0x84
    FaultResultHolder* m_pFaultResultHolder; // 0x88 (not set by the constructor)
    u8 m_Unknown0x8C;                      // 0x8C
};
ASSERT_OFFSET(EndPoint, m_pEventHandler, 0x70);
ASSERT_SIZE(EndPoint, 0x90);
} // namespace nex
} // namespace nn
