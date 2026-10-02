#include "nn/nex/nex_KeyCache.h"

namespace nn {
namespace nex {
// 0x003D5194 | fefates:bytes [tier B]
void nn::nex::KeyCache::RetrieveKey(unsigned int, const char*, nn::nex::Key*)
{
}

// 0x003D5338 | mk7dlp:callseq [tier A]
void nn::nex::KeyCache::AddKey(unsigned, const char*, const nn::nex::Key&)
{
}

// 0x003D5398 | mk7dlp:callseq [tier A]
void nn::nex::KeyCache::AddKey(unsigned, const nn::nex::Buffer&, const nn::nex::Key&)
{
}

} // namespace nex
} // namespace nn
