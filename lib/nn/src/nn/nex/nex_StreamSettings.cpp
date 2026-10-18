#include "nn/nex/nex_StreamSettings.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_Key.h"
#include "nn/nex/nex_MD5.h"

namespace nn {
namespace nex {
// 0x00374868 (name is ours)
nn::nex::StreamSettings::StreamSettings()
{
    m_StreamType = static_cast<u32>(-1);
    m_KeyByteSum = 0;
    m_KeySum = 0;
    m_pCompressionAlgorithm = nullptr;
    m_Unknown0x60 = 20;
    m_KeepAliveTimeout = 1000;
    m_pEncryptionAlgorithm = nullptr;
    m_MaxSilenceTime = 10000;
    m_Unknown0x78 = 1.25f;
    m_Unknown0x7C = 1.0f;
    m_Unknown0x80 = 50;
    m_Unknown0x90 = 2000;
    m_Unknown0x8C = 2;
    m_Unknown0x70 = 32;
    m_Unknown0x84 = -(2000 / 8);
    m_Unknown0x88 = 250;
    m_Unknown0x95 = 1;
    m_Unknown0x96 = 0;
    m_UsesKey = true;
    m_Unknown0x68 = 1;
    m_Unknown0x98 = 0;
    m_Unknown0x99 = 2;
    m_pChecksumAlgorithm = nullptr;
    m_Unknown0x74 = 128;
    m_Unknown0xA0 = 0;
}

// 0x003749DC slot 0x00 | fefates:bytes
// 0x0037498C (deleting dtor)
nn::nex::StreamSettings::~StreamSettings()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003745CC | fefates:callgraph [tier C]
nn::nex::qResult nn::nex::StreamSettings::SetMaxSilenceTime(unsigned int milliseconds)
{
    m_MaxSilenceTime = milliseconds;
    return qResult(qResult::SUCCESS);
}

// 0x003745F0 | fefates:callgraph [tier C]
nn::nex::qResult nn::nex::StreamSettings::SetKeepAliveTimeout(unsigned int milliseconds)
{
    m_KeepAliveTimeout = milliseconds;
    return qResult(qResult::SUCCESS);
}

// 0x00374614 | fefates:callgraph [tier C]
void nn::nex::StreamSettings::SetKey(const nn::nex::String& key)
{
    if (!m_UsesKey) {
        return;
    }
    char* pKey;
    key.CreateCopy(&pKey);
    u8 byteSum = 0;
    s32 sum = 0;
    for (const char* p = pKey; *p != '\0'; p++) {
        byteSum += *p;
        sum += static_cast<signed char>(*p);
    }
    String::ReleaseBuffer(pKey);
    m_KeyByteSum = byteSum;
    m_KeySum = sum;
    if (m_pChecksumAlgorithm == nullptr) {
        return;
    }
    // the key of the checksum is the MD5 of the key
    Buffer keyBuffer;
    Buffer digest(MD5::DIGEST_SIZE);
    char* pCopy;
    key.CreateCopy(&pCopy);
    keyBuffer.AppendData(pCopy, key.GetLengthInline(), static_cast<unsigned>(-1));
    String::ReleaseBuffer(pCopy);
    MD5 md5;
    md5.Update(keyBuffer.m_pData + keyBuffer.m_HeaderSize, keyBuffer.m_ContentSize);
    digest.AppendData(md5.GetDigest(), MD5::DIGEST_SIZE, static_cast<unsigned>(-1));
    m_pChecksumAlgorithm->SetKey(Key(digest.m_pData + digest.m_HeaderSize, digest.m_ContentSize));
}

} // namespace nex
} // namespace nn
