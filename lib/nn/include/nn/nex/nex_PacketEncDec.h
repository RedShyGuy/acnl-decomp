#pragma once

#include "decomp.h"
#include "nn/nex/nex_Key.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Stream.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
class EncryptionAlgorithm;
class Packet;

// the salts of the unreliable keys (GenerateKey: 8 bytes for each half of the two keys)
extern const u8 s_UnreliableKeySalts[];

// The encryption of the payloads of an end point: RC4 per reliable substream (the keys of the
// substreams after the first are derived from the first), and one RC4 for the unreliable packets
// whose key depends on the sequence id and the session id. The names of the members are ours.
class PacketEncDec : public ::nn::nex::RootObject
{
public:
    PacketEncDec(); // 0x0035E4F4
    ~PacketEncDec(); // 0x0035E774 | fefates:bytes [tier B]

    // the MD5s of the key with the 8 bytes of pA / pB (16 bytes each) as the 32 bytes of the key
    void GenerateKey(const nn::nex::Key& key, const unsigned char* pA, const unsigned char* pB, nn::nex::Key* pKey); // 0x0035D6D4 | fefates:bytes [tier B]
    // the key of the reliable substreams; a and b are added to its middle byte (encryption /
    // decryption)
    void SetReliableEncryptionKey(const nn::nex::Key& key, unsigned char a, unsigned char b); // 0x0035D844 | fefates:bytes [tier B]
    void ReinitUserReliableSubStreams(); // 0x0035DA2C | fefates:bytes [tier B]
    void DecideMaxUserReliableSubStreams(unsigned short maxSubstreamId); // 0x0035DBCC | fefates:bytes [tier B]
    void Decode(nn::nex::Packet* pPacket); // 0x0035E054 | fefates:bytes [tier B]
    // copy: the payload is replaced by an encrypted copy (the packet keeps the original data)
    void Encode(nn::nex::Packet* pPacket, nn::nex::Stream::Type type, bool copy); // 0x0035E250 | fefates:bytes [tier B]

    // (inline; the name is ours) the key of an unreliable packet
    void GetUnreliableKey(const nn::nex::Packet* pPacket, nn::nex::Key* pKey) const;

    u32 m_Unknown0x00;                                // 0x00 (not set by the constructor)
    qVector<EncryptionAlgorithm*> m_Encryptions;      // 0x04 (per reliable substream)
    qVector<EncryptionAlgorithm*> m_Decryptions;      // 0x10
    EncryptionAlgorithm* m_pUnreliableEncryption;     // 0x1C
    Key m_ReliableKey;                                // 0x20
    Key m_UnreliableKey;                              // 0x38 (of the packets without the flags 0x600)
    Key m_UnreliableKeyHigh;                          // 0x50 (of the packets with the flags 0x600)
    bool m_IsKeySet;                                  // 0x68
    u16 m_MaxSubstreamId;                             // 0x6A
};
ASSERT_OFFSET(PacketEncDec, m_ReliableKey, 0x20);
ASSERT_OFFSET(PacketEncDec, m_IsKeySet, 0x68);
ASSERT_SIZE(PacketEncDec, 0x6C);
} // namespace nex
} // namespace nn
