#pragma once

#include "decomp.h"
#include "nn/nex/nex_ConnectionOrientedStream.h"

namespace nn {
namespace nex {
class AuthenticationClient;
class Credentials;
class SecureConnectionClient;
class UserContext;

// The PRUDPS stream: its end points (SecureEndPoint) are end points of the stream below it with a
// Kerberos ticket. The names of the members and of the functions without a reference name are
// ours.
// RTTI N2nn3nex12SecureStreamE @ 0x008CE1A0
// vtable 0x008FC670 (vptr 0x008FC678), offset_to_top 0, 40 entries
class SecureStream : public ::nn::nex::ConnectionOrientedStream
{
public:
    // the stream type of the secure streams
    static const u32 TYPE = 10;

    SecureStream(); // 0x00360CCC | fefates:bytes [tier B]
    virtual ~SecureStream(); // 0x00360EF4 slot 0x00 | slot vf_0x00 of nn::nex::Stream
    // 0x00360E98 slot 0x04 | mk7dlp:bytes (deleting dtor)
    virtual bool ReceiveIncomingPacket(unsigned short port, unsigned char session, nn::nex::Packet* pPacket); // 0x00360AA4 slot 0x08 | mk7dlp:callseq
    virtual void DoWork(); // 0x00360C34 slot 0x0C | slot vf_0x0C of nn::nex::Stream
    virtual bool ResponsibleForURL(const nn::nex::StationURL* pURL); // 0x00360A78 slot 0x1C | mk7dlp:bytes
    virtual const wchar_t* GetURLType(); // 0x003608D8 slot 0x20 | mk7dlp:bytes
    virtual nn::nex::EndPoint* OpenEndPoint(const nn::nex::StationURL* pURL); // 0x00360934 slot 0x24 | slot vf_0x24 of nn::nex::ConnectionOrientedStream
    virtual nn::nex::EndPoint* OpenEndPoint(unsigned connectionId); // 0x00360940 slot 0x28 | slot vf_0x28 of nn::nex::ConnectionOrientedStream
    // (only a nop in the original that falls into ConnectionOrientedStream::OpenEndPoint)
    virtual bool OpenEndPoint(nn::nex::EndPoint* pEndPoint); // 0x003B1C6C slot 0x2C
    virtual nn::nex::EndPoint* FindEndPointByPrincipalID(unsigned principalId); // 0x00360A58 slot 0x30
    virtual u32 GetNumberOfConnectionIDs(); // 0x00360A68 slot 0x34
    virtual nn::nex::EndPoint* FindEndPointByConnectionID(unsigned connectionId); // 0x00360A48 slot 0x38
    virtual nn::nex::qVector<u32> GetConnectionIDs(); // 0x0072A5F4 slot 0x3C
    virtual void CloseEndPoint(nn::nex::EndPoint* pEndPoint); // 0x00360948 slot 0x40 | fefates:bytes
    virtual void Lock(); // 0x00360A38 slot 0x44
    virtual void Unlock(); // 0x00360A94 slot 0x48
    virtual nn::nex::qResult SendTo(const nn::nex::StationURL* pURL, nn::nex::Buffer* pPayload); // 0x00360BFC slot 0x5C
    virtual bool IsReliableType(unsigned int type); // 0x00360CA8 slot 0x68
    virtual nn::nex::EndPoint* CreateEndPoint(const nn::nex::StationURL* pURL); // 0x003609F0 slot 0x80 | fefates:bytes
    // the credentials have a principal and a connection id
    virtual bool HasCredentials(); // 0x003609C4 slot 0x84 (name is ours)
    // the URL of the credentials
    virtual nn::nex::StationURL* CreateCredentialsURL(); // 0x00360908 slot 0x88 (name is ours)
    virtual void DeleteURL(nn::nex::StationURL* pURL); // 0x003608F0 slot 0x8C (name is ours)
    virtual nn::nex::StationURL* CreateURL(unsigned int connectionId, unsigned int principalId); // 0x00360C38 slot 0x90 (name is ours)
    virtual nn::nex::qResult vf_0x94(); // 0x00360C18 slot 0x94
    virtual nn::nex::qResult vf_0x98(); // 0x00360CB0 slot 0x98
    virtual nn::nex::qResult vf_0x9C(); // 0x00360C8C slot 0x9C

    // checks the ticket of a connection request; the end point gets its principal, connection id
    // and key
    bool FilterIncomingConnection(nn::nex::Buffer* pRequest, nn::nex::Buffer* pReply, nn::nex::EndPoint* pEndPoint); // 0x00360AB4 | fefates:bytes [tier B]
    // (the filter of StreamManager::AssociateSecureStream; the context is the stream)
    static bool FilterIncomingConnection(const nn::nex::UserContext& context, nn::nex::Buffer* pRequest, nn::nex::Buffer* pReply, nn::nex::EndPoint* pEndPoint); // 0x00360AAC (name is ours)

    ConnectionOrientedStream* m_pStream;                 // 0x14 (the stream below)
    AuthenticationClient* m_pAuthenticationClient;       // 0x18
    SecureConnectionClient* m_pSecureConnectionClient;   // 0x1C
    Credentials* m_pCredentials;                         // 0x20
};
ASSERT_SIZE(SecureStream, 0x24);
} // namespace nex
} // namespace nn
