#include "nn/nex/nex_KerberosEncryption.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_KeyCache.h"

namespace nn {
namespace nex {
namespace {
const u32 DEFAULT_BASE_ITERATIONS = 65000;
const u32 DEFAULT_MODULUS = 1024;
} // namespace

// 0x009712D8 (name is ours)
KeyCache* KerberosEncryption::s_pKeyCache;
// 0x009712DC (name is ours)
KeyDerivation* KerberosEncryption::s_pKeyDerivation;
// 0x00AE05C4 (name is ours)
MD5KeyDerivation KerberosEncryption::s_DefaultKeyDerivation(DEFAULT_BASE_ITERATIONS, DEFAULT_MODULUS);

// 0x0038BFCC | fefates:bytes [tier B]
bool nn::nex::KerberosEncryption::InitializeKey(nn::nex::CallContext* pContext, const char* pPassword, nn::nex::Key* pKey)
{
    return GetKeyDerivation()->InitializeKey(pContext, pPassword, pKey);
}

// 0x0038C014 | mk7dlp:callseq [tier A]
bool nn::nex::KerberosEncryption::Decrypt(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput)
{
    Buffer buffer(input);
    if (!m_Checksum.RemoveChecksum(&buffer)) {
        return false;
    }
    return m_Encryption.Decrypt(buffer, pOutput) == 0;
}

// 0x0038C084 | mk7dlp:callseq [tier A]
bool nn::nex::KerberosEncryption::Decrypt(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput, const nn::nex::Key& key)
{
    HMACChecksum checksum;
    if (!checksum.SetKey(key)) {
        return false;
    }
    Buffer buffer(input);
    if (!checksum.RemoveChecksum(&buffer)) {
        return false;
    }
    RC4Encryption encryption;
    if (!encryption.SetKey(key)) {
        return false;
    }
    return encryption.Decrypt(buffer, pOutput) == 0;
}

// 0x0038C16C | mk7dlp:callseq [tier A]
bool nn::nex::KerberosEncryption::Encrypt(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput, const nn::nex::Key& key)
{
    RC4Encryption encryption;
    HMACChecksum checksum;
    if (!checksum.SetKey(key) || !encryption.SetKey(key) || encryption.Encrypt(input, pOutput) != 0) {
        return false;
    }
    return checksum.AppendChecksum(pOutput);
}

// 0x0038C220 | fefates:bytes [tier B]
bool nn::nex::KerberosEncryption::CreateKey(nn::nex::CallContext* pContext, unsigned int id, const char* pPassword, nn::nex::Key* pKey)
{
    return s_pKeyDerivation->CreateKey(pContext, id, pPassword, pKey, s_pKeyCache);
}

// 0x0038C268 | fefates:bytes [tier B]
nn::nex::Key nn::nex::KerberosEncryption::CreateKey(unsigned int id, const char* pPassword)
{
    KeyDerivation* pDerivation = GetKeyDerivation();
    if (s_pKeyCache == nullptr) {
        return pDerivation->CreateKey(id, pPassword);
    }
    Key key;
    if (!s_pKeyCache->RetrieveKey(id, pPassword, &key)) {
        key = s_pKeyDerivation->CreateKey(id, pPassword);
        s_pKeyCache->AddKey(id, pPassword, key);
    }
    return key;
}

// 0x0038C350 | mk7dlp:callseq [tier A]
nn::nex::KerberosEncryption::KerberosEncryption()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0038C374 | mk7dlp:callseq [tier A]
nn::nex::KerberosEncryption::~KerberosEncryption()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003CCAE4 | fefates:bytes [tier B]
void nn::nex::KerberosEncryption::SetKey(const nn::nex::Key& key)
{
    m_Encryption.SetKey(key);
    m_Checksum.SetKey(key);
    m_Key = key;
}

} // namespace nex
} // namespace nn
