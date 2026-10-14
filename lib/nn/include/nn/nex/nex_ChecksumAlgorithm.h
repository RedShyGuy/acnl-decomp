#pragma once

#include "decomp.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_Job.h"
#include "nn/nex/nex_Key.h"
#include "nn/nex/nex_PluginObject.h"

namespace nn {
namespace nex {
class CallContext;
class KeyCache;
struct SignatureBytes;

// A checksum (of a length up to GetChecksumLength(); a longer one is folded with XOR into
// m_ChecksumLength bytes). The member names and the names of the slots without a reference name
// are ours.
// RTTI N2nn3nex17ChecksumAlgorithmE @ 0x008CE674
class ChecksumAlgorithm : public ::nn::nex::PluginObject
{
public:
    // (inline)
    ChecksumAlgorithm() : m_ChecksumLength(0) {}
    virtual ~ChecksumAlgorithm() {}
    // the checksum of the content is appended to pOutput
    virtual bool ComputeChecksum(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput) = 0; // slot 0x08
    // the checksum of the arrays (the empty ones are skipped)
    virtual bool ComputeChecksum(const unsigned char** ppData, const unsigned int* pSizes, int count, nn::nex::SignatureBytes& checksum) = 0; // slot 0x0C
    virtual bool IsReady() const = 0; // slot 0x10
    // the first word of the checksum
    virtual u32 ComputeChecksumForTransport(const unsigned char* pData, unsigned int size); // 0x00385EB0 slot 0x14 | fefates:bytes
    virtual u32 ComputeChecksumForTransportArray(const unsigned char** ppData, const unsigned int* pSizes, int count) = 0; // slot 0x18
    virtual u32 GetChecksumLength() = 0; // slot 0x1C

    bool AppendChecksum(nn::nex::Buffer* pBuffer); // 0x00385BB8 | mk7dlp:callseq [tier A]
    // false if the checksum at the end of the content does not match (it is removed)
    bool RemoveChecksum(nn::nex::Buffer* pBuffer); // 0x00385CE8 | mk7dlp:callseq [tier A]
    // the key: the checksum of the password, iterations times
    nn::nex::Key DeriveKey(const char* pPassword, unsigned iterations); // 0x00385ED4 | mk7dlp:callseq [tier A]
    nn::nex::Key DeriveKey(const nn::nex::Buffer& password, unsigned iterations); // 0x00386198 (name is ours)
    // (inline in both)
    nn::nex::Key DeriveKeyInline(const nn::nex::Buffer& password, unsigned iterations)
    {
        Buffer data(password);
        Buffer checksum(GetChecksumLength());
        for (u32 i = 0; i < iterations; i++) {
            checksum.m_ContentSize = 0;
            checksum.m_HeaderSize = 0;
            ComputeChecksum(data, &checksum);
            data = checksum;
        }
        return Key(data.m_pData + data.m_HeaderSize, data.m_ContentSize);
    }
    // the same in a job (the job owns pAlgorithm; the cache may have the key already)
    static bool DeriveKey(nn::nex::CallContext* pContext, nn::nex::ChecksumAlgorithm* pAlgorithm, const char* pPassword, unsigned int iterations, nn::nex::Key* pKey, nn::nex::KeyCache* pKeyCache, nn::nex::Job::JobType jobType); // 0x00385FC8 | fefates:bytes [tier B]
    static bool DeriveKey(nn::nex::CallContext* pContext, nn::nex::ChecksumAlgorithm* pAlgorithm, const nn::nex::Buffer& password, unsigned int iterations, nn::nex::Key* pKey, nn::nex::KeyCache* pKeyCache, nn::nex::Job::JobType jobType); // 0x00386044 | fefates:bytes [tier B]

    // (inline) a checksum longer than m_ChecksumLength is folded into its last byte
    void FoldChecksum(Buffer& checksum)
    {
        if (m_ChecksumLength < GetChecksumLength()) {
            u32 length = GetChecksumLength();
            for (u32 i = m_ChecksumLength; i < length; i++) {
                checksum[m_ChecksumLength - 1] ^= checksum[i];
            }
            checksum.m_ContentSize = m_ChecksumLength;
        }
    }

    u8 m_ChecksumLength; // 0x08 (0: GetChecksumLength())
};
ASSERT_OFFSET(ChecksumAlgorithm, m_ChecksumLength, 0x08);
} // namespace nex
} // namespace nn
