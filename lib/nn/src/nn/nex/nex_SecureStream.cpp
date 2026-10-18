#include "nn/nex/nex_SecureStream.h"
#include "nn/nex/nex_AuthenticationClient.h"
#include "nn/nex/nex_BitStream.h"
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_KerberosAuthentication.h"
#include "nn/nex/nex_SecureConnectionClient.h"
#include "nn/nex/nex_SecureEndPoint.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_Ticket.h"
#include "nn/nex/nex_UserContext.h"

namespace nn {
namespace nex {
namespace {
// the result of the functions that the stream does not support (the name is ours)
const int RESULT_NOT_SUPPORTED = static_cast<int>(0x80050001);
} // namespace

// 0x00360CCC | fefates:bytes [tier B]
nn::nex::SecureStream::SecureStream() : ConnectionOrientedStream(static_cast<Type>(TYPE), nullptr)
{
    m_pAuthenticationClient = new AuthenticationClient();
    m_pStream = nullptr;
    m_pSecureConnectionClient = new SecureConnectionClient();
    m_pCredentials = nullptr;
}

// 0x00360EF4 slot 0x00 | slot vf_0x00 of nn::nex::Stream
// 0x00360E98 (deleting dtor)
nn::nex::SecureStream::~SecureStream()
{
    if (m_pSecureConnectionClient != nullptr) {
        delete m_pSecureConnectionClient;
    }
    if (m_pAuthenticationClient != nullptr) {
        delete m_pAuthenticationClient;
    }
}

// 0x00360AA4 slot 0x08 | mk7dlp:callseq
bool nn::nex::SecureStream::ReceiveIncomingPacket(unsigned short port, unsigned char session, nn::nex::Packet* pPacket)
{
    return false;
}

// 0x00360C34 slot 0x0C | slot vf_0x0C of nn::nex::Stream
void nn::nex::SecureStream::DoWork()
{
    // nothing to do
}

// 0x00360A78 slot 0x1C | mk7dlp:bytes
bool nn::nex::SecureStream::ResponsibleForURL(const nn::nex::StationURL* pURL)
{
    return pURL->GetURLType() == StationURL::URL_TYPE_PRUDPS;
}

// 0x003608D8 slot 0x20 | mk7dlp:bytes
const wchar_t* nn::nex::SecureStream::GetURLType()
{
    return L"prudps";
}

// 0x00360934 slot 0x24 | slot vf_0x24 of nn::nex::ConnectionOrientedStream
nn::nex::EndPoint* nn::nex::SecureStream::OpenEndPoint(const nn::nex::StationURL* pURL)
{
    return CreateEndPoint(pURL);
}

// 0x00360940 slot 0x28 | slot vf_0x28 of nn::nex::ConnectionOrientedStream
nn::nex::EndPoint* nn::nex::SecureStream::OpenEndPoint(unsigned connectionId)
{
    return nullptr;
}

// 0x003B1C6C slot 0x2C
bool nn::nex::SecureStream::OpenEndPoint(nn::nex::EndPoint* pEndPoint)
{
    return ConnectionOrientedStream::OpenEndPoint(pEndPoint);
}

// 0x00360A58 slot 0x30
nn::nex::EndPoint* nn::nex::SecureStream::FindEndPointByPrincipalID(unsigned principalId)
{
    return m_pStream->FindEndPointByPrincipalID(principalId);
}

// 0x00360A68 slot 0x34
u32 nn::nex::SecureStream::GetNumberOfConnectionIDs()
{
    return m_pStream->GetNumberOfConnectionIDs();
}

// 0x00360A48 slot 0x38
nn::nex::EndPoint* nn::nex::SecureStream::FindEndPointByConnectionID(unsigned connectionId)
{
    return m_pStream->FindEndPointByConnectionID(connectionId);
}

// 0x0072A5F4 slot 0x3C
nn::nex::qVector<u32> nn::nex::SecureStream::GetConnectionIDs()
{
    return m_pStream->GetConnectionIDs();
}

// 0x00360948 slot 0x40 | fefates:bytes
void nn::nex::SecureStream::CloseEndPoint(nn::nex::EndPoint* pEndPoint)
{
    SecureEndPoint* pSecureEndPoint = static_cast<SecureEndPoint*>(pEndPoint);
    if (pSecureEndPoint->m_pEndPoint != nullptr) {
        pSecureEndPoint->ReleaseEndPoint();
    }
    if (pSecureEndPoint != nullptr) {
        delete pSecureEndPoint;
    }
}

// 0x00360A38 slot 0x44
void nn::nex::SecureStream::Lock()
{
    m_pStream->Lock();
}

// 0x00360A94 slot 0x48
void nn::nex::SecureStream::Unlock()
{
    m_pStream->Unlock();
}

// 0x00360BFC slot 0x5C
nn::nex::qResult nn::nex::SecureStream::SendTo(const nn::nex::StationURL* pURL, nn::nex::Buffer* pPayload)
{
    return qResult(RESULT_NOT_SUPPORTED);
}

// 0x00360CA8 slot 0x68
bool nn::nex::SecureStream::IsReliableType(unsigned int type)
{
    return false;
}

// 0x003609F0 slot 0x80 | fefates:bytes
nn::nex::EndPoint* nn::nex::SecureStream::CreateEndPoint(const nn::nex::StationURL* pURL)
{
    return new SecureEndPoint(this, pURL);
}

// 0x003609C4 slot 0x84 (name is ours)
bool nn::nex::SecureStream::HasCredentials()
{
    Credentials* pCredentials = m_pCredentials;
    if (pCredentials == nullptr || pCredentials->m_ConnectionId == 0) {
        return false;
    }
    return pCredentials->m_PrincipalId != 0;
}

// 0x00360908 slot 0x88 (name is ours)
nn::nex::StationURL* nn::nex::SecureStream::CreateCredentialsURL()
{
    Credentials* pCredentials = m_pCredentials;
    if (pCredentials == nullptr) {
        return CreateURL(0, 0);
    }
    return CreateURL(pCredentials->m_ConnectionId, pCredentials->m_PrincipalId);
}

// 0x003608F0 slot 0x8C (name is ours)
void nn::nex::SecureStream::DeleteURL(nn::nex::StationURL* pURL)
{
    if (pURL != nullptr) {
        delete pURL;
    }
}

// 0x00360C38 slot 0x90 (name is ours)
nn::nex::StationURL* nn::nex::SecureStream::CreateURL(unsigned int connectionId, unsigned int principalId)
{
    StationURL* pURL = new StationURL();
    pURL->SetURLType(StationURL::URL_TYPE_PRUDPS);
    pURL->SetConnectionID(connectionId);
    pURL->SetPrincipalID(principalId);
    return pURL;
}

// 0x00360C18 slot 0x94
nn::nex::qResult nn::nex::SecureStream::vf_0x94()
{
    return qResult(RESULT_NOT_SUPPORTED);
}

// 0x00360CB0 slot 0x98
nn::nex::qResult nn::nex::SecureStream::vf_0x98()
{
    return qResult(RESULT_NOT_SUPPORTED);
}

// 0x00360C8C slot 0x9C
nn::nex::qResult nn::nex::SecureStream::vf_0x9C()
{
    return qResult(RESULT_NOT_SUPPORTED);
}

// 0x00360AAC (name is ours)
bool nn::nex::SecureStream::FilterIncomingConnection(const nn::nex::UserContext& context, nn::nex::Buffer* pRequest, nn::nex::Buffer* pReply, nn::nex::EndPoint* pEndPoint)
{
    return static_cast<SecureStream*>(context.m_pPointer)->FilterIncomingConnection(pRequest, pReply, pEndPoint);
}

// 0x00360AB4 | fefates:bytes [tier B]
bool nn::nex::SecureStream::FilterIncomingConnection(nn::nex::Buffer* pRequest, nn::nex::Buffer* pReply, nn::nex::EndPoint* pEndPoint)
{
    if (pRequest == nullptr) {
        return false;
    }
    BitStream request(pRequest);
    BitStream reply(pReply);
    Ticket* pTicket;
    u32 connectionId = 0;
    if (!KerberosAuthentication::ValidateConnectionRequest(&request, &reply, m_pAuthenticationClient, &connectionId, &pTicket)) {
        return false;
    }
    if (!pEndPoint->PeerIsConnected() && !pEndPoint->IsAnyPacketProcessed()) {
        pEndPoint->SetPrincipalID(pTicket->m_PrincipalId);
        pEndPoint->SetConnectionID(connectionId);
        pEndPoint->SetEncryptionKey(pTicket->m_SessionKey);
    }
    pTicket->ReleaseRef();
    return true;
}

} // namespace nex
} // namespace nn
