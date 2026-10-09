#include "nn/crypto/crypto_Sha256Context.h"
#include "nn/crypto/crypto_Api.h"

namespace nn {
namespace crypto {
namespace {
// the round constants of SHA-256
// 0x00983630
u32 s_RoundConstants[64] = {
    0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
    0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
    0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
    0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
    0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
    0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
    0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
    0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2,
};

const u32 INITIAL_STATE[8] = {
    0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19,
};

inline u32 RotateRight(u32 value, int shift)
{
    return (value >> shift) | (value << (32 - shift));
}
} // namespace

// 0x0048331C
void nn::crypto::Sha256Context::Initialize()
{
    m_BlockCountLow = 0;
    m_BlockCountHigh = 0;
    m_BlockUsed = 0;
    m_State[0] = INITIAL_STATE[0];
    m_State[1] = INITIAL_STATE[1];
    m_State[2] = INITIAL_STATE[2];
    m_State[3] = INITIAL_STATE[3];
    m_State[4] = INITIAL_STATE[4];
    m_State[5] = INITIAL_STATE[5];
    m_State[6] = INITIAL_STATE[6];
    m_State[7] = INITIAL_STATE[7];
}

// 0x00483388 (name is ours)
size_t nn::crypto::Sha256Context::GetHashSize() const
{
    return HASH_SIZE;
}

// 0x00483390 (name is ours)
void nn::crypto::Sha256Context::InitializeWithState(const void* pState, u64 size)
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
    m_State[5] = __builtin_bswap32(state[5]);
    m_State[6] = __builtin_bswap32(state[6]);
    m_State[7] = __builtin_bswap32(state[7]);
}

// 0x00483418
void nn::crypto::Sha256Context::Update(const void* pData, size_t size)
{
    ShaBlock512BitContext::Update(pData, size);
}

// 0x0048341C
void nn::crypto::Sha256Context::GetHash(void* pOutput)
{
    AddPadding();
    u32* output = static_cast<u32*>(pOutput);
    output[0] = __builtin_bswap32(m_State[0]);
    output[1] = __builtin_bswap32(m_State[1]);
    output[2] = __builtin_bswap32(m_State[2]);
    output[3] = __builtin_bswap32(m_State[3]);
    output[4] = __builtin_bswap32(m_State[4]);
    output[5] = __builtin_bswap32(m_State[5]);
    output[6] = __builtin_bswap32(m_State[6]);
    output[7] = __builtin_bswap32(m_State[7]);
}

// 0x00483490 (name is ours)
void nn::crypto::Sha256Context::Finalize()
{
}

// 0x00483498
// 0x00483494 (deleting dtor)
nn::crypto::Sha256Context::~Sha256Context()
{
}

// 0x00148D70
void nn::crypto::Sha256Context::ProcessBlock()
{
    u32 w[64];
    const u32* block = reinterpret_cast<const u32*>(m_Block);
    for (int i = 0; i < 16; i++) {
        w[i] = __builtin_bswap32(block[i]);
    }
    for (int i = 16; i < 64; i++) {
        u32 s0 = RotateRight(w[i - 15], 7) ^ RotateRight(w[i - 15], 18) ^ (w[i - 15] >> 3);
        u32 s1 = RotateRight(w[i - 2], 17) ^ RotateRight(w[i - 2], 19) ^ (w[i - 2] >> 10);
        w[i] = w[i - 7] + s1 + (w[i - 16] + s0);
    }
    u32 a = m_State[0];
    u32 b = m_State[1];
    u32 c = m_State[2];
    u32 d = m_State[3];
    u32 e = m_State[4];
    u32 f = m_State[5];
    u32 g = m_State[6];
    u32 h = m_State[7];
    for (int i = 0; i < 64; i++) {
        u32 t1 = (RotateRight(e, 6) ^ RotateRight(e, 11) ^ RotateRight(e, 25)) + ((e & f) ^ (g & ~e)) + (s_RoundConstants[i] + h) + w[i];
        u32 t2 = (RotateRight(a, 2) ^ RotateRight(a, 13) ^ RotateRight(a, 22)) + (((b ^ c) & a) ^ (b & c));
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }
    m_State[0] += a;
    m_State[1] += b;
    m_State[2] += c;
    m_State[3] += d;
    m_State[4] += e;
    m_State[5] += f;
    m_State[6] += g;
    m_State[7] += h;
}

// 0x00140D00 (name is ours, after CalculateSha1)
void CalculateSha256(void* pOutput, const void* pData, size_t size)
{
    Sha256Context context;
    context.Initialize();
    context.Update(pData, size);
    context.GetHash(pOutput);
}

} // namespace crypto
} // namespace nn
