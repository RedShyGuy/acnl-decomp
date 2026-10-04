#include "nn/pia/common/common_Md5Context.h"
#include "nn/nstd/nstd_String.h"
#include <string.h>

namespace nn {
namespace pia {
namespace common {
namespace {
// the sine constants of MD5 (RFC 1321)
// 0x008C1C60
const u32 s_SineTable[64] = {
    0xD76AA478, 0xE8C7B756, 0x242070DB, 0xC1BDCEEE, 0xF57C0FAF, 0x4787C62A, 0xA8304613, 0xFD469501,
    0x698098D8, 0x8B44F7AF, 0xFFFF5BB1, 0x895CD7BE, 0x6B901122, 0xFD987193, 0xA679438E, 0x49B40821,
    0xF61E2562, 0xC040B340, 0x265E5A51, 0xE9B6C7AA, 0xD62F105D, 0x02441453, 0xD8A1E681, 0xE7D3FBC8,
    0x21E1CDE6, 0xC33707D6, 0xF4D50D87, 0x455A14ED, 0xA9E3E905, 0xFCEFA3F8, 0x676F02D9, 0x8D2A4C8A,
    0xFFFA3942, 0x8771F681, 0x6D9D6122, 0xFDE5380C, 0xA4BEEA44, 0x4BDECFA9, 0xF6BB4B60, 0xBEBFBC70,
    0x289B7EC6, 0xEAA127FA, 0xD4EF3085, 0x04881D05, 0xD9D4D039, 0xE6DB99E5, 0x1FA27CF8, 0xC4AC5665,
    0xF4292244, 0x432AFF97, 0xAB9423A7, 0xFC93A039, 0x655B59C3, 0x8F0CCC92, 0xFFEFF47D, 0x85845DD1,
    0x6FA87E4F, 0xFE2CE6E0, 0xA3014314, 0x4E0811A1, 0xF7537E82, 0xBD3AF235, 0x2AD7D2BB, 0xEB86D391,
};

// the order of the message words in rounds 2 to 4
// 0x008C1D60
const u32 s_WordIndexTable[48] = {
    1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12,
    5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2,
    0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9,
};

// the first byte of the padding
// 0x0097F9EC
u8 s_PaddingByte = 0x80;

const u32 INITIAL_STATE_A = 0x67452301;
const u32 INITIAL_STATE_B = 0xEFCDAB89;

inline u32 RotateLeft(u32 value, int shift)
{
    return (value << shift) | (value >> (32 - shift));
}
} // namespace

// 0x004261BC | fefates:callgraph [tier C]
void nn::pia::common::Md5Context::Initialize()
{
    m_State[0] = INITIAL_STATE_A;
    m_State[1] = INITIAL_STATE_B;
    m_State[2] = ~INITIAL_STATE_A;
    m_State[3] = ~INITIAL_STATE_B;
    m_Size = 0;
}

// 0x004261EC | fefates:bytes [tier B]
void nn::pia::common::Md5Context::ProcessBlock()
{
    u32 a = m_State[0];
    u32 b = m_State[1];
    u32 c = m_State[2];
    u32 d = m_State[3];
    const u32* x = reinterpret_cast<const u32*>(m_Block);
    const u32* t = s_SineTable;
    const u32* px = x;
    for (int i = 0; i < 4; i++) {
        a = b + RotateLeft(a + ((b & c) | (~b & d)) + *px++ + *t++, 7);
        d = a + RotateLeft(d + ((a & b) | (~a & c)) + *px++ + *t++, 12);
        c = d + RotateLeft(c + ((d & a) | (~d & b)) + *px++ + *t++, 17);
        b = c + RotateLeft(b + ((c & d) | (~c & a)) + *px++ + *t++, 22);
    }
    const u32* index = s_WordIndexTable;
    for (int i = 0; i < 4; i++) {
        a = b + RotateLeft(a + ((b & d) | (c & ~d)) + x[*index++] + *t++, 5);
        d = a + RotateLeft(d + ((a & c) | (b & ~c)) + x[*index++] + *t++, 9);
        c = d + RotateLeft(c + ((d & b) | (a & ~b)) + x[*index++] + *t++, 14);
        b = c + RotateLeft(b + ((c & a) | (d & ~a)) + x[*index++] + *t++, 20);
    }
    for (int i = 0; i < 4; i++) {
        a = b + RotateLeft(a + (b ^ c ^ d) + x[*index++] + *t++, 4);
        d = a + RotateLeft(d + (a ^ b ^ c) + x[*index++] + *t++, 11);
        c = d + RotateLeft(c + (d ^ a ^ b) + x[*index++] + *t++, 16);
        b = c + RotateLeft(b + (c ^ d ^ a) + x[*index++] + *t++, 23);
    }
    for (int i = 0; i < 4; i++) {
        a = b + RotateLeft(a + (c ^ (b | ~d)) + x[*index++] + *t++, 6);
        d = a + RotateLeft(d + (b ^ (a | ~c)) + x[*index++] + *t++, 10);
        c = d + RotateLeft(c + (a ^ (d | ~b)) + x[*index++] + *t++, 15);
        b = c + RotateLeft(b + (d ^ (c | ~a)) + x[*index++] + *t++, 21);
    }
    m_State[0] += a;
    m_State[1] += b;
    m_State[2] += c;
    m_State[3] += d;
}

// 0x00426504 | fefates:bytes [tier B]
void nn::pia::common::Md5Context::Update(const void* pData, unsigned int size)
{
    const u8* p = static_cast<const u8*>(pData);
    u32 used = m_Size % BLOCK_SIZE;
    u32 rest = BLOCK_SIZE - used;
    m_Size += size;
    if (size < rest) {
        if (size != 0) {
            nnnstdMemCpy(m_Block + used, p, size);
        }
        return;
    }
    nnnstdMemCpy(m_Block + used, p, rest);
    ProcessBlock();
    u32 remain = size - rest;
    p += rest;
    for (int blocks = remain / BLOCK_SIZE; blocks > 0; blocks--) {
        nnnstdMemCpy(m_Block, p, BLOCK_SIZE);
        p += BLOCK_SIZE;
        ProcessBlock();
    }
    u32 tail = remain % BLOCK_SIZE;
    if (tail != 0) {
        nnnstdMemCpy(m_Block, p, tail);
    }
}

// 0x004265D0 | fefates:bytes [tier B]
void nn::pia::common::Md5Context::GetHash(void* pOutput)
{
    u64 bits = static_cast<u64>(m_Size) << 3;
    Update(&s_PaddingByte, 1);
    u32 used = m_Size % BLOCK_SIZE;
    u32 rest = BLOCK_SIZE - used;
    if (rest < sizeof(u64)) {
        memset(m_Block + used, 0, rest);
        ProcessBlock();
        used = 0;
        rest = BLOCK_SIZE;
    }
    if (rest > sizeof(u64)) {
        memset(m_Block + used, 0, rest - sizeof(u64));
    }
    *reinterpret_cast<u64*>(m_Block + BLOCK_SIZE - sizeof(u64)) = bits;
    ProcessBlock();
    u32* output = static_cast<u32*>(pOutput);
    output[0] = m_State[0];
    output[1] = m_State[1];
    output[2] = m_State[2];
    output[3] = m_State[3];
}

// 0x0073180C | slot vf_0x08 of nn::pia::common::Md5Context
unsigned int nn::pia::common::Md5Context::GetHashSize() const
{
    return HASH_SIZE;
}

// 0x00731814 (name is ours)
unsigned int nn::pia::common::Md5Context::GetBlockSize() const
{
    return BLOCK_SIZE;
}

} // namespace common
} // namespace pia
} // namespace nn
