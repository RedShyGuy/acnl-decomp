#pragma once

#include "decomp.h"
#include "nn/nex/nex_HMACChecksum.h"
#include "nn/nex/nex_Key.h"
#include "nn/nex/nex_MD5KeyDerivation.h"
#include "nn/nex/nex_RC4Encryption.h"

namespace nn {
namespace nex {
class Buffer;
class CallContext;
class KeyCache;

// The encryption of Kerberos tickets and messages: RC4 with an HMAC-MD5 checksum appended, both with
// the same key. The keys of passwords come from a KeyDerivation (by default MD5 with 65000 + id %
// 1024 iterations) and a KeyCache. The member names and the names of the statics are ours.
class KerberosEncryption
{
public:
    KerberosEncryption(); // 0x0038C350 | mk7dlp:callseq [tier A]
    ~KerberosEncryption(); // 0x0038C374 | mk7dlp:callseq [tier A]

    void SetKey(const nn::nex::Key& key); // 0x003CCAE4 | fefates:bytes [tier B]
    // false if the checksum does not match
    bool Decrypt(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput); // 0x0038C014 | mk7dlp:callseq [tier A]
    bool Decrypt(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput, const nn::nex::Key& key); // 0x0038C084 | mk7dlp:callseq [tier A]
    bool Encrypt(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput, const nn::nex::Key& key); // 0x0038C16C | mk7dlp:callseq [tier A]

    // the key of the password (in a job)
    static bool InitializeKey(nn::nex::CallContext* pContext, const char* pPassword, nn::nex::Key* pKey); // 0x0038BFCC | fefates:bytes [tier B]
    // the key of the id and password (in a job, with the cache)
    static bool CreateKey(nn::nex::CallContext* pContext, unsigned int id, const char* pPassword, nn::nex::Key* pKey); // 0x0038C220 | fefates:bytes [tier B]
    static nn::nex::Key CreateKey(unsigned int id, const char* pPassword); // 0x0038C268 | fefates:bytes [tier B]

    // (inline) the derivation, by default s_DefaultKeyDerivation
    static KeyDerivation* GetKeyDerivation()
    {
        if (s_pKeyDerivation == nullptr) {
            s_pKeyDerivation = &s_DefaultKeyDerivation;
        }
        return s_pKeyDerivation;
    }

    u32 m_Unknown0x00;            // 0x000
    RC4Encryption m_Encryption;   // 0x004
    HMACChecksum m_Checksum;      // 0x234
    Key m_Key;                    // 0x2E4

    static KeyCache* s_pKeyCache;
    static KeyDerivation* s_pKeyDerivation;
    static MD5KeyDerivation s_DefaultKeyDerivation;
};
ASSERT_OFFSET(KerberosEncryption, m_Checksum, 0x234);
ASSERT_OFFSET(KerberosEncryption, m_Key, 0x2E4);
} // namespace nex
} // namespace nn
