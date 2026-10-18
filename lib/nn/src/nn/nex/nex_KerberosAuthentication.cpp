#include "nn/nex/nex_KerberosAuthentication.h"
#include "nn/nex/nex_AuthenticationClient.h"
#include "nn/nex/nex_BitStream.h"
#include "nn/nex/nex_DateTime.h"
#include "nn/nex/nex_Key.h"
#include "nn/nex/nex_KerberosEncryption.h"
#include "nn/nex/nex_Ticket.h"

namespace nn {
namespace nex {
namespace {
// the maximum age of a ticket in a connection request (ms; the name is ours)
const s64 MAX_TICKET_AGE = 120000;
} // namespace

// 0x0039D5A0 | fefates:bytes [tier B]
bool nn::nex::KerberosAuthentication::ValidateConnectionRequest(nn::nex::BitStream* pRequest, nn::nex::BitStream* pReply, nn::nex::AuthenticationClient* pClient, unsigned int* pConnectionId, nn::nex::Ticket** ppTicket)
{
    // the request: the ticket (the session key and the encrypted ticket) and the authenticator
    Buffer ticketData;
    *pRequest >> ticketData;
    BitStream ticketStream(&ticketData);
    Buffer keyData;
    ticketStream >> keyData;
    Key key(keyData.m_pData + keyData.m_HeaderSize, keyData.m_ContentSize);
    Buffer encryptedTicket;
    ticketStream >> encryptedTicket;
    u32 failure = 0;
    if (pRequest->m_IsError || ticketStream.m_IsError) {
        BitStream reply;
        reply.Append(reinterpret_cast<const u8*>(&failure), sizeof(failure), 1);
        *pReply << *reply.m_pBuffer;
        return false;
    }

    Ticket* pTicket = new Ticket(encryptedTicket, pClient->m_PrincipalId);
    pTicket->Decrypt(pClient->m_pKerberos, key);
    DateTime now;
    DateTime::GetSystemTime(now);
    DateTime ticketTime(pTicket->m_Time);
    if (now - ticketTime > MAX_TICKET_AGE || !pTicket->m_IsDecrypted) {
        if (pTicket != nullptr) {
            delete pTicket;
        }
        BitStream reply;
        reply.Append(reinterpret_cast<const u8*>(&failure), sizeof(failure), 1);
        *pReply << *reply.m_pBuffer;
        return false;
    }

    // the authenticator: the principal, the connection id and a check value (replied plus one)
    Buffer encryptedAuthenticator;
    *pRequest >> encryptedAuthenticator;
    Buffer authenticator;
    pClient->m_pKerberos->Decrypt(encryptedAuthenticator, &authenticator, pTicket->m_SessionKey);
    BitStream authenticatorStream(&authenticator);
    u32 principalId;
    authenticatorStream.Extract(reinterpret_cast<u8*>(&principalId), sizeof(principalId), 1);
    u32 connectionId;
    authenticatorStream.Extract(reinterpret_cast<u8*>(&connectionId), sizeof(connectionId), 1);
    u32 check;
    authenticatorStream.Extract(reinterpret_cast<u8*>(&check), sizeof(check), 1);
    if (authenticatorStream.m_IsError || pTicket->m_PrincipalId != principalId) {
        if (pTicket != nullptr) {
            delete pTicket;
        }
        BitStream reply;
        reply.Append(reinterpret_cast<const u8*>(&failure), sizeof(failure), 1);
        *pReply << *reply.m_pBuffer;
        return false;
    }

    *pConnectionId = connectionId;
    BitStream reply;
    u32 response = check + 1;
    reply.Append(reinterpret_cast<const u8*>(&response), sizeof(response), 1);
    *pReply << *reply.m_pBuffer;
    *ppTicket = pTicket;
    return true;
}

} // namespace nex
} // namespace nn
