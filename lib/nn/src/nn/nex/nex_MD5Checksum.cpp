#include "nn/nex/nex_MD5Checksum.h"
#include "nn/nex/nex_MD5.h"
#include "nn/nex/nex_SignatureBytes.h"
#include <cstring>

namespace nn {
namespace nex {
// 0x00358750 slot 0x0C | fefates:bytes
bool nn::nex::MD5Checksum::ComputeChecksum(const unsigned char** ppData, const unsigned int* pSizes, int count, nn::nex::SignatureBytes& checksum)
{
    MD5 md5;
    for (int i = 0; i < count; i++) {
        if (ppData[i] != nullptr && pSizes[i] != 0) {
            md5.Update(ppData[i], pSizes[i]);
        }
    }
    const u8* pDigest = md5.GetDigest();
    checksum.m_Size = MD5::DIGEST_SIZE;
    memcpy(checksum.m_Data, pDigest, MD5::DIGEST_SIZE);
    return true;
}

// 0x00358804 slot 0x08 | fefates:bytes
bool nn::nex::MD5Checksum::ComputeChecksum(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput)
{
    MD5 md5;
    md5.Update(input.m_pData + input.m_HeaderSize, input.m_ContentSize);
    pOutput->AppendData(md5.GetDigest(), MD5::DIGEST_SIZE, static_cast<unsigned>(-1));
    return true;
}

// 0x00358890 slot 0x1C
u32 nn::nex::MD5Checksum::GetChecksumLength()
{
    return MD5::DIGEST_SIZE;
}

// 0x00358898 slot 0x18 | fefates:bytes
u32 nn::nex::MD5Checksum::ComputeChecksumForTransportArray(const unsigned char** ppData, const unsigned int* pSizes, int count)
{
    MD5 md5;
    for (int i = 0; i < count; i++) {
        if (ppData[i] != nullptr && pSizes[i] != 0) {
            md5.Update(ppData[i], pSizes[i]);
        }
    }
    return *reinterpret_cast<const u32*>(md5.GetDigest());
}

// 0x00358934 | fefates:bytes [tier B]
nn::nex::MD5Checksum::MD5Checksum()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x00358968 slot 0x00
// 0x00358950 (deleting dtor)
nn::nex::MD5Checksum::~MD5Checksum()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
