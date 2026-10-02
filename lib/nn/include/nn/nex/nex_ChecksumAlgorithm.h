#pragma once

#include "decomp.h"
#include "nn/nex/nex_Job.h"
#include "nn/nex/nex_PluginObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex17ChecksumAlgorithmE @ 0x008CE674
class ChecksumAlgorithm : public ::nn::nex::PluginObject
{
public:
    ChecksumAlgorithm(); // ctor address unknown
    void AppendChecksum(nn::nex::Buffer*); // 0x00385BB8 | mk7dlp:callseq [tier A]
    void RemoveChecksum(nn::nex::Buffer*); // 0x00385CE8 | mk7dlp:callseq [tier A]
    void DeriveKey(const char*, unsigned); // 0x00385ED4 | mk7dlp:callseq [tier A]
    void DeriveKey(nn::nex::CallContext*, nn::nex::ChecksumAlgorithm*, const char*, unsigned int, nn::nex::Key*, nn::nex::KeyCache*, nn::nex::Job::JobType); // 0x00385FC8 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
