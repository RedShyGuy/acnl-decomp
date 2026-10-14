#include "nn/nex/nex_MD5ChecksumWithKey.h"
#include "nn/nex/nex_MD5.h"
#include "nn/nex/nex_SignatureBytes.h"
#include <cstring>

namespace nn {
namespace nex {
// 0x0038C398 slot 0x0C
bool nn::nex::MD5ChecksumWithKey::ComputeChecksum(const unsigned char** ppData, const unsigned int* pSizes, int count, nn::nex::SignatureBytes& checksum)
{
    MD5 md5;
    ChecksumComputeHelper(ppData, pSizes, count, md5);
    memcpy(checksum.m_Data, md5.m_Digest, MD5::DIGEST_SIZE);
    checksum.m_Size = MD5::DIGEST_SIZE;
    return true;
}

// 0x0038C408 slot 0x08 | fefates:callseq
bool nn::nex::MD5ChecksumWithKey::ComputeChecksum(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput)
{
    MD5 md5;
    const unsigned char* pData = input.m_pData + input.m_HeaderSize;
    unsigned int size = input.m_ContentSize;
    ChecksumComputeHelper(&pData, &size, 1, md5);
    pOutput->AppendData(md5.m_Digest, MD5::DIGEST_SIZE, static_cast<unsigned>(-1));
    return true;
}

// 0x0038C48C slot 0x1C
u32 nn::nex::MD5ChecksumWithKey::GetChecksumLength()
{
    return MD5::DIGEST_SIZE;
}

// 0x0038C494 | fefates:bytes [tier B]
void nn::nex::MD5ChecksumWithKey::ChecksumComputeHelper(const unsigned char** ppData, const unsigned int* pSizes, int count, nn::nex::MD5& md5)
{
    u32 keyLength = m_Key.GetLength();
    for (int i = 0; i < count; i++) {
        if (ppData[i] != nullptr && pSizes[i] != 0) {
            md5.Update(ppData[i], pSizes[i]);
        }
    }
    if (keyLength != 0) {
        md5.Update(m_Key.GetContentPtr(), keyLength);
    }
    md5.GetDigest();
}

// 0x0038C53C slot 0x18
u32 nn::nex::MD5ChecksumWithKey::ComputeChecksumForTransportArray(const unsigned char** ppData, const unsigned int* pSizes, int count)
{
    MD5 md5;
    ChecksumComputeHelper(ppData, pSizes, count, md5);
    return *reinterpret_cast<const u32*>(md5.m_Digest);
}

// 0x0038C5CC slot 0x00
// 0x0038C598 (deleting dtor)
nn::nex::MD5ChecksumWithKey::~MD5ChecksumWithKey()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
