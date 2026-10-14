#include "nn/nex/nex_HMACChecksum.h"
#include "nn/nex/nex_MD5.h"
#include "nn/nex/nex_SignatureBytes.h"
#include <cstring>

namespace nn {
namespace nex {
namespace {
const u8 INNER_PAD_VALUE = 0x36;
const u8 OUTER_PAD_VALUE = 0x5C;
} // namespace

// 0x0035CA68 slot 0x20 | fefates:bytes
void nn::nex::HMACChecksum::KeyHasChanged()
{
    const u8* pKey = m_Key.GetContentPtr();
    u32 length = m_Key.GetLength();
    if (length > PAD_SIZE) {
        // (the digest stays on the stack after the block)
        MD5 md5;
        md5.Update(pKey, length);
        pKey = md5.GetDigest();
        length = MD5::DIGEST_SIZE;
    }
    memset(m_InnerPad, 0, sizeof(m_InnerPad));
    memset(m_OuterPad, 0, sizeof(m_OuterPad));
    memcpy(m_InnerPad, pKey, length);
    memcpy(m_OuterPad, pKey, length);
    for (u32 i = 0; i < PAD_SIZE; i++) {
        m_InnerPad[i] ^= INNER_PAD_VALUE;
        m_OuterPad[i] ^= OUTER_PAD_VALUE;
    }
}

// 0x0035CB5C slot 0x0C
bool nn::nex::HMACChecksum::ComputeChecksum(const unsigned char** ppData, const unsigned int* pSizes, int count, nn::nex::SignatureBytes& checksum)
{
    MD5 md5;
    ChecksumComputeHelper(ppData, pSizes, count, md5);
    memcpy(checksum.m_Data, md5.m_Digest, MD5::DIGEST_SIZE);
    checksum.m_Size = MD5::DIGEST_SIZE;
    return true;
}

// 0x0035CBCC slot 0x08 | fefates:callseq
bool nn::nex::HMACChecksum::ComputeChecksum(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput)
{
    MD5 md5;
    const unsigned char* pData = input.m_pData + input.m_HeaderSize;
    unsigned int size = input.m_ContentSize;
    ChecksumComputeHelper(&pData, &size, 1, md5);
    pOutput->AppendData(md5.m_Digest, MD5::DIGEST_SIZE, static_cast<unsigned>(-1));
    return true;
}

// 0x0035CC50 slot 0x1C
u32 nn::nex::HMACChecksum::GetChecksumLength()
{
    return MD5::DIGEST_SIZE;
}

// 0x0035CC58 | fefates:bytes [tier B]
void nn::nex::HMACChecksum::ChecksumComputeHelper(const unsigned char** ppData, const unsigned int* pSizes, int count, nn::nex::MD5& outer)
{
    MD5 inner;
    inner.Update(m_InnerPad, PAD_SIZE);
    for (int i = 0; i < count; i++) {
        if (ppData[i] != nullptr && pSizes[i] != 0) {
            inner.Update(ppData[i], pSizes[i]);
        }
    }
    inner.GetDigest();
    outer.Update(m_OuterPad, PAD_SIZE);
    outer.Update(inner.m_Digest, MD5::DIGEST_SIZE);
    outer.GetDigest();
}

// 0x0035CD5C slot 0x18
u32 nn::nex::HMACChecksum::ComputeChecksumForTransportArray(const unsigned char** ppData, const unsigned int* pSizes, int count)
{
    MD5 md5;
    ChecksumComputeHelper(ppData, pSizes, count, md5);
    return *reinterpret_cast<const u32*>(md5.m_Digest);
}

// 0x0035CDB8 | mk7dlp:callseq-callee [tier A]
nn::nex::HMACChecksum::HMACChecksum()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0035CE48 slot 0x00 | mk7dlp:callseq-callee
// 0x0035CE14 (deleting dtor)
nn::nex::HMACChecksum::~HMACChecksum()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
