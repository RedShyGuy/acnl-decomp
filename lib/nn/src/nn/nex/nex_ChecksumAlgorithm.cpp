#include "nn/nex/nex_ChecksumAlgorithm.h"
#include "nn/nex/nex_JobDeriveKey.h"
#include "nn/nex/nex_Scheduler.h"
#include <cstring>

namespace nn {
namespace nex {
// 0x00385BB8 | mk7dlp:callseq [tier A]
bool nn::nex::ChecksumAlgorithm::AppendChecksum(nn::nex::Buffer* pBuffer)
{
    if (!IsReady()) {
        return false;
    }
    if (m_ChecksumLength == 0) {
        m_ChecksumLength = GetChecksumLength();
    }
    Buffer checksum(GetChecksumLength());
    ComputeChecksum(*pBuffer, &checksum);
    FoldChecksum(checksum);
    pBuffer->AppendData(checksum.m_pData + checksum.m_HeaderSize, checksum.m_ContentSize, static_cast<unsigned>(-1));
    return true;
}

// 0x00385CE8 | mk7dlp:callseq [tier A]
bool nn::nex::ChecksumAlgorithm::RemoveChecksum(nn::nex::Buffer* pBuffer)
{
    if (!IsReady()) {
        return false;
    }
    if (m_ChecksumLength == 0) {
        m_ChecksumLength = GetChecksumLength();
    }
    if (pBuffer->m_ContentSize < m_ChecksumLength) {
        return false;
    }
    Buffer received;
    pBuffer->m_ContentSize -= m_ChecksumLength;
    received.AppendData(pBuffer->m_pData + pBuffer->m_HeaderSize + pBuffer->m_ContentSize, m_ChecksumLength, static_cast<unsigned>(-1));
    Buffer computed(GetChecksumLength());
    ComputeChecksum(*pBuffer, &computed);
    FoldChecksum(computed);
    if (received.m_ContentSize != computed.m_ContentSize
        || memcmp(computed.m_pData + computed.m_HeaderSize, received.m_pData + received.m_HeaderSize, received.m_ContentSize) != 0) {
        return false;
    }
    return true;
}

// 0x00385EB0 slot 0x14 | fefates:bytes
u32 nn::nex::ChecksumAlgorithm::ComputeChecksumForTransport(const unsigned char* pData, unsigned int size)
{
    return ComputeChecksumForTransportArray(&pData, &size, 1);
}

// 0x00385ED4 | mk7dlp:callseq [tier A]
nn::nex::Key nn::nex::ChecksumAlgorithm::DeriveKey(const char* pPassword, unsigned iterations)
{
    Buffer password;
    password.AppendData(pPassword, strlen(pPassword), static_cast<unsigned>(-1));
    return DeriveKeyInline(password, iterations);
}

// 0x00386198 (name is ours)
nn::nex::Key nn::nex::ChecksumAlgorithm::DeriveKey(const nn::nex::Buffer& password, unsigned iterations)
{
    return DeriveKeyInline(password, iterations);
}

// 0x00385FC8 | fefates:bytes [tier B]
bool nn::nex::ChecksumAlgorithm::DeriveKey(nn::nex::CallContext* pContext, nn::nex::ChecksumAlgorithm* pAlgorithm, const char* pPassword, unsigned int iterations,
                                           nn::nex::Key* pKey, nn::nex::KeyCache* pKeyCache, nn::nex::Job::JobType jobType)
{
    Buffer password;
    password.AppendData(pPassword, strlen(pPassword), static_cast<unsigned>(-1));
    return DeriveKey(pContext, pAlgorithm, password, iterations, pKey, pKeyCache, jobType);
}

// 0x00386044 | fefates:bytes [tier B]
bool nn::nex::ChecksumAlgorithm::DeriveKey(nn::nex::CallContext* pContext, nn::nex::ChecksumAlgorithm* pAlgorithm, const nn::nex::Buffer& password, unsigned int iterations,
                                           nn::nex::Key* pKey, nn::nex::KeyCache* pKeyCache, nn::nex::Job::JobType jobType)
{
    ScopedCS lock(Scheduler::GetInstanceInline()->m_Lock);
    if (!pContext->InitiateCall()) {
        return false;
    }
    JobDeriveKey* pJob = new JobDeriveKey(pContext, pAlgorithm, password, iterations, pKey, jobType);
    pJob->m_pKeyCache = pKeyCache;
    Scheduler::GetInstanceInline()->Queue(pJob, false);
    return true;
}

} // namespace nex
} // namespace nn
