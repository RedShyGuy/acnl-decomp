#pragma once

#include "decomp.h"
#include "nn/nex/nex_EndPoint.h"

namespace nn {
namespace nex {
// The end point of a SecureStream: it passes everything to the end point of the stream below it
// (PRUDP), the connection goes through a JobConnectSecureEndPoint. The member names are ours.
// RTTI N2nn3nex14SecureEndPointE @ 0x008CE3C4
// vtable 0x008FCC68 (vptr 0x008FCC70), offset_to_top 0, 37 entries
class SecureEndPoint : public ::nn::nex::EndPoint
{
public:
    // (inline in SecureStream::CreateEndPoint)
    SecureEndPoint(nn::nex::ConnectionOrientedStream* pStream, const nn::nex::StationURL* pURL) : EndPoint(pStream, pURL), m_pEndPoint(nullptr), m_pCallback(nullptr)
    {
        m_Context.m_Value = 0;
    }
    virtual ~SecureEndPoint(); // 0x00374220 slot 0x00 | slot vf_0x00 of nn::nex::EndPoint
    // 0x00374204 slot 0x04 | fefates:callseq (deleting dtor)
    virtual void ResetStatistics(); // 0x00373D8C slot 0x08 | virtual slot, introduced by nn::nex::EndPoint
    virtual bool GetStatistics(void* pStatistics); // 0x0072B2B8 slot 0x0C | virtual slot, introduced by nn::nex::EndPoint
    virtual u32 GetPendingReliablePacketCount(); // 0x0072B320 slot 0x10 | virtual slot, introduced by nn::nex::EndPoint
    virtual void GetPendingReliablePacketCounts(nn::nex::qVector<u32>* pCounts); // 0x0072B328 slot 0x14 | virtual slot, introduced by nn::nex::EndPoint
    virtual u16 GetMaxSubstreamId(); // 0x0072B2C0 slot 0x18 | virtual slot, introduced by nn::nex::EndPoint
    virtual bool IsNotConnected(); // 0x00373CA8 slot 0x1C | slot vf_0x1C of nn::nex::EndPoint
    virtual bool IsConnecting(); // 0x00373C8C slot 0x20 | fefates:bytes
    virtual bool IsDisconnecting(); // 0x00373CC8 slot 0x24 | slot vf_0x24 of nn::nex::EndPoint
    virtual bool IsFaulty(); // 0x00374010 slot 0x28 | fefates:bytes
    virtual bool IsConnected(); // 0x00373B64 slot 0x2C | fefates:bytes
    virtual bool PeerIsConnected(); // 0x00373CE4 slot 0x30 | fefates:bytes
    virtual bool PeerIsNotConnected(); // 0x00373E70 slot 0x34 | virtual slot, introduced by nn::nex::EndPoint
    virtual void SetKeepAliveTimeout(unsigned int milliseconds); // 0x00373EC8 slot 0x3C | fefates:bytes
    virtual void SetMaxSilenceTime(unsigned int milliseconds); // 0x00373D90 slot 0x40 | fefates:bytes
    virtual u32 GetKeepAliveTimeout(); // 0x00373EAC slot 0x44 | slot vf_0x44 of nn::nex::EndPoint
    virtual u32 GetMaxSilenceTime(); // 0x00373D70 slot 0x48 | slot vf_0x48 of nn::nex::EndPoint
    virtual void SetPeerConnected(); // 0x00373D38 slot 0x4C | slot vf_0x4C of nn::nex::EndPoint
    virtual void SetPeerDisconnected(); // 0x00373EE4 slot 0x50 | slot vf_0x50 of nn::nex::EndPoint
    virtual u8 GetConnectionState() const; // 0x0072B2C8 slot 0x54 | slot vf_0x54 of nn::nex::EndPoint
    virtual bool SetConnectionState(nn::nex::EndPoint::_ConnectionState state); // 0x00373E90 slot 0x58 | fefates:bytes
    virtual nn::nex::qResult RegisterEventHandler(nn::nex::EndPointEventHandler* pHandler); // 0x00373F1C slot 0x5C | mk7dlp:callseq
    virtual bool IsAnyPacketProcessed(); // 0x00373F00 slot 0x60 | fefates:bytes
    virtual u32 GetSignaledFaultError() const; // 0x0072B2E4 slot 0x64 | fefates:bytes
    virtual u32 GetLastRTT(); // 0x0072B32C slot 0x68 | virtual slot, introduced by nn::nex::EndPoint
    virtual u32 GetRTO(); // 0x0072B29C slot 0x6C | virtual slot, introduced by nn::nex::EndPoint
    virtual nn::nex::qResult _Connect(nn::nex::Buffer* pConnectData, nn::nex::Buffer* pReplyData, CompletionCallback pCallback, const nn::nex::UserContext& context, unsigned int timeout); // 0x0037402C slot 0x70 | fefates:bytes
    virtual nn::nex::qResult _Disconnect(CompletionCallback pCallback, const nn::nex::UserContext& context, unsigned int timeout); // 0x00373B80 slot 0x74 | fefates:bytes
    virtual nn::nex::qResult _Send(nn::nex::Buffer* pBuffer, unsigned int flags, bool copy, unsigned char substreamId, unsigned int id); // 0x00373F90 slot 0x78 | slot vf_0x78 of nn::nex::EndPoint
    virtual u64 GetSupportedFunctionsFlags() const; // 0x0072B300 slot 0x7C | fefates:bytes
    virtual void SignalFaultEvent(unsigned int error, bool isProcessed); // 0x00373D54 slot 0x88 | fefates:bytes
    virtual void SetEncryptionKey(const nn::nex::Key& key); // 0x00373D1C slot 0x8C | fefates:bytes
    virtual const nn::nex::Key& GetEncryptionKey(); // 0x00373D00 slot 0x90 | slot vf_0x90 of nn::nex::EndPoint

    // (inline) the end point below is released (its references to this one too)
    void ReleaseEndPoint()
    {
        m_pEndPoint->SetPrincipalID(0);
        m_pEndPoint->SetConnectionID(0);
        SetPrincipalID(0);
        SetConnectionID(0);
        m_pEndPoint->m_pOwner = nullptr;
        m_pEndPoint->Close();
        m_pEndPoint = nullptr;
    }
    // the disconnection of the end point below is complete (the context is the SecureEndPoint)
    static void DisconnectCallback(nn::nex::EndPoint* pEndPoint, nn::nex::qResult result, const nn::nex::UserContext* pContext); // 0x00373DAC (name is ours)

    EndPoint* m_pEndPoint;          // 0x90 (of the stream below; its m_pOwner is this one)
    CompletionCallback m_pCallback; // 0x94 (of the disconnection)
    UserContext m_Context;          // 0x98
};
ASSERT_SIZE(SecureEndPoint, 0x9C);
} // namespace nex
} // namespace nn
