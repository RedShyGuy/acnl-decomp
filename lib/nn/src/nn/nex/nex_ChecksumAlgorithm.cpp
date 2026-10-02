#include "nn/nex/nex_PluginObject.h"
#include "nn/nex/nex_Job.h"
#include "nn/nex/nex_ChecksumAlgorithm.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::ChecksumAlgorithm::ChecksumAlgorithm()
{
}

// 0x00385BB8 | mk7dlp:callseq [tier A]
void nn::nex::ChecksumAlgorithm::AppendChecksum(nn::nex::Buffer*)
{
}

// 0x00385CE8 | mk7dlp:callseq [tier A]
void nn::nex::ChecksumAlgorithm::RemoveChecksum(nn::nex::Buffer*)
{
}

// 0x00385ED4 | mk7dlp:callseq [tier A]
void nn::nex::ChecksumAlgorithm::DeriveKey(const char*, unsigned)
{
}

// 0x00385FC8 | fefates:bytes [tier B]
void nn::nex::ChecksumAlgorithm::DeriveKey(nn::nex::CallContext*, nn::nex::ChecksumAlgorithm*, const char*, unsigned int, nn::nex::Key*, nn::nex::KeyCache*, nn::nex::Job::JobType)
{
}

} // namespace nex
} // namespace nn
