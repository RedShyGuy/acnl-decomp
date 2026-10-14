#pragma once

#include "decomp.h"
#include "nn/nex/nex_EncryptionAlgorithm.h"

namespace nn {
namespace nex {
class CriticalSection;

// RC4 with a key of 5 to 64 bytes ("CD&ML" by default). Unless m_IsResetPerMessage is cleared every
// message starts with the state after the key schedule. The member names and the names of the
// functions without a reference name are ours.
// RTTI N2nn3nex13RC4EncryptionE @ 0x008CE26C
// vtable 0x008FC900 (vptr 0x008FC908), offset_to_top 0, 8 entries
class RC4Encryption : public ::nn::nex::EncryptionAlgorithm
{
public:
    RC4Encryption(); // 0x0036AD58 | mk7dlp:callseq-callee [tier A]
    virtual ~RC4Encryption(); // 0x0036AE8C slot 0x00 | mk7dlp:callseq-callee
    // 0x0036AE28 slot 0x04 | fefates:bytes (deleting dtor)
    virtual int Encrypt(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput); // 0x0036AD28 slot 0x08 | mk7dlp:bytes
    virtual int Encrypt(nn::nex::Buffer* pBuffer); // 0x0036AD08 slot 0x0C | slot vf_0x0C of nn::nex::EncryptionAlgorithm
    virtual int Decrypt(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput); // 0x0036ACD8 slot 0x10 | mk7dlp:bytes
    virtual int Decrypt(nn::nex::Buffer* pBuffer); // 0x0036ACB8 slot 0x14 | slot vf_0x14 of nn::nex::EncryptionAlgorithm
    // the key schedule
    virtual void KeyHasChanged(); // 0x0036AB0C slot 0x1C | fefates:bytes-fuzzy

    // (inline) the state after the key schedule
    void ResetState()
    {
        m_J = 0;
        m_I = 0;
        memcpy(m_State, m_InitialState, sizeof(m_State));
    }
    // XORs the key stream into the data (nothing without a key)
    void Process(u8* pData, u32 size); // 0x0036ABE0 (name is ours)

    u8 m_InitialState[256];       // 0x028 (after the key schedule)
    u8 m_State[256];              // 0x128
    u8 m_I;                       // 0x228
    u8 m_J;                       // 0x229
    bool m_IsResetPerMessage;     // 0x22A
    CriticalSection* m_pLock;     // 0x22C
};
ASSERT_OFFSET(RC4Encryption, m_InitialState, 0x28);
ASSERT_OFFSET(RC4Encryption, m_pLock, 0x22C);
ASSERT_SIZE(RC4Encryption, 0x230);
} // namespace nex
} // namespace nn
