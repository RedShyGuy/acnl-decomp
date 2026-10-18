#include "nn/nex/nex_PacketEncDec.h"
#include "nn/nex/nex_MD5.h"
#include "nn/nex/nex_Packet.h"
#include "nn/nex/nex_RC4Encryption.h"
#include "nn/nex/nex_StreamSettings.h"

#include <cstring>

namespace nn {
namespace nex {
// 0x00899A40
extern const u8 s_UnreliableKeySalts[];

namespace {
// the flags of the packets with the second unreliable key (the names are ours)
const u16 FLAG_UNRELIABLE_HIGH = 0x600;
// the flag of a reliable packet
const u16 FLAG_RELIABLE = 0x20;
// the types whose payload is encrypted
const u32 TYPE_DATA = 2;
const u32 TYPE_UNKNOWN_5 = 5;
// the size of the unreliable keys
const u32 UNRELIABLE_KEY_SIZE = 32;

// a new RC4 whose state continues over the messages
inline EncryptionAlgorithm* CreateStreamEncryption()
{
    RC4Encryption* pEncryption = new RC4Encryption();
    pEncryption->DisableResetPerMessage();
    return pEncryption;
}

// the payload of the packet is encrypted
inline bool IsEncrypted(const Packet* pPacket, Stream::Type type)
{
    if (Stream::GetSettings(type)->m_Unknown0xA0 == 0 || pPacket->m_Unknown0x88 != 0 || pPacket->m_pPayload == nullptr) {
        return false;
    }
    u32 packetType = pPacket->GetType();
    return packetType == TYPE_DATA || packetType == TYPE_UNKNOWN_5;
}
} // namespace

// (inline; the name is ours)
void nn::nex::PacketEncDec::GetUnreliableKey(const nn::nex::Packet* pPacket, nn::nex::Key* pKey) const
{
    const Key& base = (pPacket->m_TypeFlags & FLAG_UNRELIABLE_HIGH) ? m_UnreliableKeyHigh : m_UnreliableKey;
    memcpy(const_cast<u8*>(pKey->GetContentPtr()), base.GetContentPtr(), UNRELIABLE_KEY_SIZE);
    u16 sequenceId = pPacket->m_SequenceId.m_Value;
    u8* pData = const_cast<u8*>(pKey->GetContentPtr());
    pData[0] += static_cast<u8>(sequenceId);
    pData = const_cast<u8*>(pKey->GetContentPtr());
    pData[1] += static_cast<u8>(sequenceId >> 8);
    pData = const_cast<u8*>(pKey->GetContentPtr());
    pData[pKey->GetLength() - 1] += pPacket->m_SessionId;
}

// 0x0035E4F4
nn::nex::PacketEncDec::PacketEncDec()
    : m_Encryptions(1), m_Decryptions(1), m_ReliableKey(), m_UnreliableKey(UNRELIABLE_KEY_SIZE), m_UnreliableKeyHigh(UNRELIABLE_KEY_SIZE),
      m_IsKeySet(false), m_MaxSubstreamId(1)
{
    m_Encryptions[0] = CreateStreamEncryption();
    m_Decryptions[0] = CreateStreamEncryption();
    m_pUnreliableEncryption = CreateStreamEncryption();
}

// 0x0035E774 | fefates:bytes [tier B]
nn::nex::PacketEncDec::~PacketEncDec()
{
    for (u32 i = 0; i < m_Encryptions.size(); i++) {
        if (m_Encryptions[i] != nullptr) {
            delete m_Encryptions[i];
        }
        if (m_Decryptions[i] != nullptr) {
            delete m_Decryptions[i];
        }
    }
    if (m_pUnreliableEncryption != nullptr) {
        delete m_pUnreliableEncryption;
    }
}

// 0x0035D6D4 | fefates:bytes [tier B]
void nn::nex::PacketEncDec::GenerateKey(const nn::nex::Key& key, const unsigned char* pA, const unsigned char* pB, nn::nex::Key* pKey)
{
    MD5 md5A;
    MD5 md5B;
    md5A.Update(key.GetContentPtr(), key.GetLength());
    md5B.Update(key.GetContentPtr(), key.GetLength());
    md5A.Update(pA, 8);
    md5B.Update(pB, 8);
    const u8* pDigestA = md5A.GetDigest();
    const u8* pDigestB = md5B.GetDigest();
    memcpy(const_cast<u8*>(pKey->GetContentPtr()), pDigestA, MD5::DIGEST_SIZE);
    memcpy(const_cast<u8*>(pKey->GetContentPtr()) + MD5::DIGEST_SIZE, pDigestB, MD5::DIGEST_SIZE);
}

// 0x0035D844 | fefates:bytes [tier B]
void nn::nex::PacketEncDec::SetReliableEncryptionKey(const nn::nex::Key& key, unsigned char a, unsigned char b)
{
    m_ReliableKey = key;
    Key encryptionKey(key);
    const_cast<u8*>(encryptionKey.GetContentPtr())[key.GetLength() / 2] += a;
    Key decryptionKey(key);
    const_cast<u8*>(decryptionKey.GetContentPtr())[key.GetLength() / 2] += b;
    m_Encryptions[0]->SetKeyInline(encryptionKey);
    m_Decryptions[0]->SetKeyInline(decryptionKey);
    m_IsKeySet = true;
    if (m_MaxSubstreamId > 1) {
        DecideMaxUserReliableSubStreams(m_MaxSubstreamId);
    }
}

// 0x0035DA2C | fefates:bytes [tier B]
void nn::nex::PacketEncDec::ReinitUserReliableSubStreams()
{
    if (m_Encryptions.size() <= 1) {
        return;
    }
    Key encryptionKey(m_Encryptions[0]->m_Key);
    Key decryptionKey(m_Decryptions[0]->m_Key);
    for (u32 i = 1; i < m_Encryptions.size(); i++) {
        for (u32 j = 0; j < encryptionKey.GetLength() / 2; j++) {
            const_cast<u8*>(encryptionKey.GetContentPtr())[j] += encryptionKey.GetLength() / 2 - j + 1;
            const_cast<u8*>(decryptionKey.GetContentPtr())[j] += encryptionKey.GetLength() / 2 - j + 1;
        }
        m_Encryptions[i]->SetKeyInline(encryptionKey);
        m_Decryptions[i]->SetKeyInline(decryptionKey);
    }
}

// 0x0035DBCC | fefates:bytes [tier B]
void nn::nex::PacketEncDec::DecideMaxUserReliableSubStreams(unsigned short maxSubstreamId)
{
    m_MaxSubstreamId = maxSubstreamId;
    if (!m_IsKeySet) {
        return;
    }
    u32 count = maxSubstreamId + 1;
    while (count < m_Encryptions.size()) {
        m_Encryptions.m_pEnd--;
        m_Decryptions.m_pEnd--;
    }
    m_Encryptions.reserve(maxSubstreamId + 1);
    m_Decryptions.reserve(maxSubstreamId + 1);
    for (int i = m_Encryptions.size(); i < static_cast<int>(count); i++) {
        m_Encryptions.push_back(CreateStreamEncryption());
        m_Decryptions.push_back(CreateStreamEncryption());
    }
    ReinitUserReliableSubStreams();
}

// 0x0035E054 | fefates:bytes [tier B]
void nn::nex::PacketEncDec::Decode(nn::nex::Packet* pPacket)
{
    if (!IsEncrypted(pPacket, static_cast<Stream::Type>(pPacket->m_DestinationPort >> 4))) {
        return;
    }
    if (pPacket->m_TypeFlags & FLAG_RELIABLE) {
        m_Decryptions[pPacket->m_SubstreamId]->Decrypt(pPacket->m_pPayload);
        return;
    }
    Key key(UNRELIABLE_KEY_SIZE);
    GetUnreliableKey(pPacket, &key);
    m_pUnreliableEncryption->SetKeyInline(key);
    m_pUnreliableEncryption->Decrypt(pPacket->m_pPayload);
}

// 0x0035E250 | fefates:bytes [tier B]
void nn::nex::PacketEncDec::Encode(nn::nex::Packet* pPacket, nn::nex::Stream::Type type, bool copy)
{
    if (!IsEncrypted(pPacket, type)) {
        return;
    }
    Buffer* pPayload = pPacket->m_pPayload;
    if (copy) {
        // the encrypted payload gets the data, the packet keeps a copy of it
        pPayload = new Buffer(*pPacket->m_pPayload);
        Buffer* pOriginal = pPacket->m_pPayload;
        u8 flags = pPayload->m_Flags;
        pPayload->m_Flags = pOriginal->m_Flags;
        pOriginal->m_Flags = flags;
        u8* pData = pPayload->m_pData;
        pPayload->m_pData = pOriginal->m_pData;
        pOriginal->m_pData = pData;
    }
    if (pPacket->m_TypeFlags & FLAG_RELIABLE) {
        m_Encryptions[pPacket->m_SubstreamId]->Encrypt(pPayload);
    } else {
        Key key(UNRELIABLE_KEY_SIZE);
        GetUnreliableKey(pPacket, &key);
        m_pUnreliableEncryption->SetKeyInline(key);
        m_pUnreliableEncryption->Encrypt(pPayload);
    }
    if (copy) {
        if (pPacket->m_pPayload != nullptr) {
            pPacket->m_pPayload->ReleaseRef();
        }
        pPacket->m_pPayload = pPayload;
    }
}

} // namespace nex
} // namespace nn
