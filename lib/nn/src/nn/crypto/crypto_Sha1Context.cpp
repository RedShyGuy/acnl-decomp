#include "nn/crypto/crypto_Sha1Context.h"
#include "nn/crypto/crypto_Api.h"

namespace nn {
namespace crypto {
namespace {
// the round constants of SHA-1
const u32 K0 = 0x5A827999;
const u32 K1 = 0x6ED9EBA1;
const u32 K2 = 0x8F1BBCDC;
const u32 K3 = 0xCA62C1D6;

const u32 INITIAL_STATE_A = 0x67452301;
const u32 INITIAL_STATE_B = 0xEFCDAB89;
const u32 INITIAL_STATE_E = 0xC3D2E1F0;

inline u32 RotateLeft(u32 value, int shift)
{
    return (value << shift) | (value >> (32 - shift));
}

// the word t of the message schedule, kept in 16 words
inline u32 GetScheduleWord(u32* w, int t)
{
    if (t >= 16) {
        w[t & 15] = RotateLeft(w[(t - 16) & 15] ^ w[(t - 14) & 15] ^ w[(t - 8) & 15] ^ w[(t - 3) & 15], 1);
    }
    return w[t & 15];
}
} // namespace

// 0x00482DF8 | nintendogs:bytes [tier A]
void nn::crypto::Sha1Context::Initialize()
{
    m_BlockCountLow = 0;
    m_BlockCountHigh = 0;
    m_BlockUsed = 0;
    m_State[0] = INITIAL_STATE_A;
    m_State[1] = INITIAL_STATE_B;
    m_State[2] = ~INITIAL_STATE_A;
    m_State[3] = ~INITIAL_STATE_B;
    m_State[4] = INITIAL_STATE_E;
}

// 0x00482E44 (name is ours)
size_t nn::crypto::Sha1Context::GetHashSize() const
{
    return HASH_SIZE;
}

// 0x00482E4C | nintendogs:bytes [tier A]
void nn::crypto::Sha1Context::ProcessBlock()
{
    u32 w[16];
    u32 a = m_State[0];
    u32 b = m_State[1];
    u32 c = m_State[2];
    u32 d = m_State[3];
    u32 e = m_State[4];
    const u32* block = reinterpret_cast<const u32*>(m_Block);
    for (int i = 0; i < 16; i++) {
        w[i] = __builtin_bswap32(block[i]);
    }
    int t = 0;
    for (; t < 20; t++) {
        u32 temp = GetScheduleWord(w, t) + (((b & c) | (d & ~b)) + K0) + (e + RotateLeft(a, 5));
        e = d;
        d = c;
        c = RotateLeft(b, 30);
        b = a;
        a = temp;
    }
    for (; t < 40; t++) {
        u32 temp = GetScheduleWord(w, t) + ((b ^ c ^ d) + K1) + (e + RotateLeft(a, 5));
        e = d;
        d = c;
        c = RotateLeft(b, 30);
        b = a;
        a = temp;
    }
    for (; t < 60; t++) {
        u32 temp = GetScheduleWord(w, t) + ((((c | d) & b) | (c & d)) + K2) + (e + RotateLeft(a, 5));
        e = d;
        d = c;
        c = RotateLeft(b, 30);
        b = a;
        a = temp;
    }
    for (; t < 80; t++) {
        u32 temp = GetScheduleWord(w, t) + ((b ^ c ^ d) + K3) + (e + RotateLeft(a, 5));
        e = d;
        d = c;
        c = RotateLeft(b, 30);
        b = a;
        a = temp;
    }
    m_State[0] += a;
    m_State[1] += b;
    m_State[2] += c;
    m_State[3] += d;
    m_State[4] += e;
}

// 0x00483158 (name is ours)
void nn::crypto::Sha1Context::InitializeWithState(const void* pState, u64 size)
{
    u64 blockCount = size / BLOCK_SIZE;
    m_BlockUsed = 0;
    m_BlockCountLow = blockCount;
    m_BlockCountHigh = blockCount >> 32;
    const u32* state = static_cast<const u32*>(pState);
    m_State[0] = __builtin_bswap32(state[0]);
    m_State[1] = __builtin_bswap32(state[1]);
    m_State[2] = __builtin_bswap32(state[2]);
    m_State[3] = __builtin_bswap32(state[3]);
    m_State[4] = __builtin_bswap32(state[4]);
}

// 0x004831BC | nintendogs:bytes [tier A]
void nn::crypto::Sha1Context::GetHash(void* pOutput)
{
    AddPadding();
    u32* output = static_cast<u32*>(pOutput);
    output[0] = __builtin_bswap32(m_State[0]);
    output[1] = __builtin_bswap32(m_State[1]);
    output[2] = __builtin_bswap32(m_State[2]);
    output[3] = __builtin_bswap32(m_State[3]);
    output[4] = __builtin_bswap32(m_State[4]);
}

// 0x0048320C (name is ours)
void nn::crypto::Sha1Context::Finalize()
{
}

// 0x00483214
// 0x00483210 (deleting dtor)
nn::crypto::Sha1Context::~Sha1Context()
{
}

// 0x001437E0
void nn::crypto::Sha1Context::Update(const void* pData, size_t size)
{
    ShaBlock512BitContext::Update(pData, size);
}

// 0x00483278 | fefates:bytes [tier B]
void CalculateSha1(void* pOutput, const void* pData, size_t size)
{
    Sha1Context context;
    context.Initialize();
    context.Update(pData, size);
    context.GetHash(pOutput);
}

} // namespace crypto
} // namespace nn
