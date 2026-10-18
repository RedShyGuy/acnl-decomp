#include "nn/nex/nex_Ticket.h"
#include "nn/nex/nex_BitStream.h"
#include "nn/nex/nex_DateTime.h"
#include "nn/nex/nex_HMACChecksum.h"
#include "nn/nex/nex_KerberosEncryption.h"
#include "nn/nex/nex_MD5Checksum.h"
#include "nn/nex/nex_RC4Encryption.h"
#include "nn/nex/nex_RefCountedObject.h"

namespace nn {
namespace nex {
namespace {
// (inline) the key of the ticket: the MD5 of the key of the encryption and the key
inline Key DeriveTicketKey(const Key& encryptionKey, const Key& key)
{
    BitStream keyData;
    keyData.AppendRaw(encryptionKey.GetContentPtr(), encryptionKey.GetLength() * 8);
    keyData.AppendRaw(key.GetContentPtr(), key.GetLength() * 8);
    MD5Checksum md5;
    Buffer checksum;
    md5.ComputeChecksum(*keyData.m_pBuffer, &checksum);
    return Key(checksum.m_pData + checksum.m_HeaderSize, checksum.m_ContentSize);
}
} // namespace

// 0x003D1838 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
// 0x003D180C (deleting dtor)
nn::nex::Ticket::~Ticket()
{
    // nothing to do: the members and bases are destroyed by the compiler
}

// 0x003D1518 | fefates:bytes [tier B]
void nn::nex::Ticket::Decrypt(nn::nex::KerberosEncryption* pEncryption, const nn::nex::Key& key)
{
    Key ticketKey = DeriveTicketKey(pEncryption->m_Key, key);
    BitStream ticket;
    bool isDecrypted = false;
    {
        // the data is checked with the HMAC and decrypted with RC4
        HMACChecksum checksum;
        if (!checksum.SetKey(ticketKey)) {
            return;
        }
        Buffer data(m_Data);
        if (!checksum.RemoveChecksum(&data)) {
            return;
        }
        RC4Encryption encryption;
        if (!encryption.SetKey(ticketKey)) {
            return;
        }
        isDecrypted = encryption.Decrypt(data, ticket.m_pBuffer) == 0;
    }
    if (!isDecrypted) {
        return;
    }
    m_IsDecrypted = true;
    ticket.AdjustLength();
    DateTime time;
    ticket.Extract(reinterpret_cast<unsigned char*>(&time.m_Value), 8, 1);
    m_Time = time;
    ticket.Extract(reinterpret_cast<unsigned char*>(&m_PrincipalId), 4, 1);
    ticket.ExtractRaw(m_SessionKey.PrepareContentPtr(SESSION_KEY_SIZE), SESSION_KEY_SIZE * 8);
}

// 0x003D1764 | fefates:bytes [tier B]
nn::nex::Ticket::Ticket(const nn::nex::Buffer& data, unsigned int unknown0x30) : m_Data(data), m_Unknown0x30(unknown0x30)
{
    m_IsDecrypted = false;
}

// 0x003D17B0 | fefates:bytes [tier B]
nn::nex::Ticket::Ticket(const nn::nex::Buffer& data, unsigned int principalId, unsigned int unknown0x30, const nn::nex::Key& sessionKey, nn::nex::DateTime time)
    : m_Data(data), m_PrincipalId(principalId), m_Unknown0x30(unknown0x30), m_SessionKey(sessionKey), m_Time(time)
{
    m_IsDecrypted = true;
}

} // namespace nex
} // namespace nn
