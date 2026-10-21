#include "nn/nex/nex_RC4Encryption.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_CriticalSection.h"

namespace nn {
namespace nex {
namespace {
const u32 MIN_KEY_LENGTH = 5;
const u32 MAX_KEY_LENGTH = 64;
} // namespace

// 0x0036AB0C | fefates:bytes-fuzzy
void nn::nex::RC4Encryption::KeyHasChanged()
{
    for (u32 i = 0; i < 256; i++) {
        m_InitialState[i] = i;
    }
    m_J = 0;
    m_I = 0;
    const u8* pKey = m_Key.GetContentPtr();
    u32 keyLength = m_Key.GetLength();
    u32 keyIndex = 0;
    u8 j = 0;
    for (u32 i = 0; i < 256; i++) {
        j = pKey[keyIndex] + m_InitialState[i] + j;
        u8 value = m_InitialState[i];
        m_InitialState[i] = m_InitialState[j];
        m_InitialState[j] = value;
        keyIndex = (keyIndex + 1) % keyLength;
    }
    ResetState();
}

// 0x0036ABE0 (name is ours)
void nn::nex::RC4Encryption::Process(u8* pData, u32 size)
{
    ScopedCS lock(*m_pLock);
    if (m_Key.GetLength() == 0) {
        return;
    }
    if (m_IsResetPerMessage) {
        ResetState();
    }
    u8 i = m_I;
    u8 j = m_J;
    for (; size != 0; size--) {
        i++;
        u8 value = m_State[i];
        j += value;
        m_State[i] = m_State[j];
        m_State[j] = value;
        *pData++ ^= m_State[static_cast<u8>(m_State[i] + value)];
    }
    m_I = i;
    m_J = j;
}

// 0x0036ACB8 slot 0x14 | slot vf_0x14 of nn::nex::EncryptionAlgorithm
int nn::nex::RC4Encryption::Decrypt(nn::nex::Buffer* pBuffer)
{
    Process(pBuffer->m_pData + pBuffer->m_HeaderSize, pBuffer->m_ContentSize);
    return 0;
}

// 0x0036ACD8 slot 0x10 | mk7dlp:bytes
int nn::nex::RC4Encryption::Decrypt(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput)
{
    *pOutput = input;
    Decrypt(pOutput);
    return 0;
}

// 0x0036AD08 slot 0x0C | slot vf_0x0C of nn::nex::EncryptionAlgorithm
int nn::nex::RC4Encryption::Encrypt(nn::nex::Buffer* pBuffer)
{
    Process(pBuffer->m_pData + pBuffer->m_HeaderSize, pBuffer->m_ContentSize);
    return 0;
}

// 0x0036AD28 slot 0x08 | mk7dlp:bytes
int nn::nex::RC4Encryption::Encrypt(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput)
{
    *pOutput = input;
    Encrypt(pOutput);
    return 0;
}

// 0x0036AD58 | mk7dlp:callseq-callee [tier A]
nn::nex::RC4Encryption::RC4Encryption() : m_IsResetPerMessage(true)
{
    m_MinKeyLength = MIN_KEY_LENGTH;
    m_MaxKeyLength = MAX_KEY_LENGTH;
    m_pLock = new CriticalSection(0x20000000);
    // the key until SetKey
    u8 key[] = {'C', 'D', '&', 'M', 'L'};
    SetKey(Key(key, sizeof(key)));
}

// 0x0036AE8C slot 0x00 | mk7dlp:callseq-callee
// 0x0036AE28 (deleting dtor)
nn::nex::RC4Encryption::~RC4Encryption()
{
    delete m_pLock;
}

} // namespace nex
} // namespace nn
