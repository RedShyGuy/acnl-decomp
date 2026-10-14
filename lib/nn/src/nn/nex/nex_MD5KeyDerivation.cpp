#include "nn/nex/nex_MD5KeyDerivation.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_MD5Checksum.h"

namespace nn {
namespace nex {
namespace {
// the type of the jobs of the derivation (meaning unknown)
const Job::JobType JOB_TYPE = static_cast<Job::JobType>(2);
} // namespace

// 0x00382230 slot 0x1C (name is ours)
bool nn::nex::MD5KeyDerivation::CreateKey(nn::nex::CallContext* pContext, unsigned int id, const nn::nex::Key& key, nn::nex::Key* pKey)
{
    Buffer password;
    password.AppendData(key.GetContentPtr(), key.GetLength(), static_cast<unsigned>(-1));
    u32 iterations = id % m_Modulus;
    return ChecksumAlgorithm::DeriveKey(pContext, new MD5Checksum(), password, iterations, pKey, nullptr, JOB_TYPE);
}

// 0x0038230C slot 0x10 (name is ours)
nn::nex::Key nn::nex::MD5KeyDerivation::CreateKey(unsigned id, const nn::nex::Key& key)
{
    Buffer password;
    password.AppendData(key.GetContentPtr(), key.GetLength(), static_cast<unsigned>(-1));
    u32 iterations = id % m_Modulus;
    MD5Checksum checksum;
    return checksum.DeriveKey(password, iterations);
}

// 0x003823C0 slot 0x0C (name is ours)
nn::nex::Key nn::nex::MD5KeyDerivation::InitializeKey(const char* pPassword)
{
    MD5Checksum checksum;
    return checksum.DeriveKey(pPassword, m_BaseIterations);
}

// 0x00382400 slot 0x18 | fefates:bytes
bool nn::nex::MD5KeyDerivation::InitializeKey(nn::nex::CallContext* pContext, const char* pPassword, nn::nex::Key* pKey)
{
    return ChecksumAlgorithm::DeriveKey(pContext, new MD5Checksum(), pPassword, m_BaseIterations, pKey, nullptr, JOB_TYPE);
}

// 0x00382450 slot 0x14 | fefates:bytes
bool nn::nex::MD5KeyDerivation::CreateKey(nn::nex::CallContext* pContext, unsigned int id, const char* pPassword, nn::nex::Key* pKey, nn::nex::KeyCache* pKeyCache)
{
    u32 iterations = m_BaseIterations + id % m_Modulus;
    return ChecksumAlgorithm::DeriveKey(pContext, new MD5Checksum(), pPassword, iterations, pKey, pKeyCache, JOB_TYPE);
}

// 0x003824C0 slot 0x08 | mk7dlp:bytes-fuzzy
nn::nex::Key nn::nex::MD5KeyDerivation::CreateKey(unsigned id, const char* pPassword)
{
    u32 iterations = m_BaseIterations + id % m_Modulus;
    MD5Checksum checksum;
    return checksum.DeriveKey(pPassword, iterations);
}

// 0x00382518 slot 0x00
// 0x00382514 (deleting dtor)
nn::nex::MD5KeyDerivation::~MD5KeyDerivation()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
