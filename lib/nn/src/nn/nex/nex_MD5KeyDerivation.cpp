#include "nn/nex/nex_KeyDerivation.h"
#include "nn/nex/nex_MD5KeyDerivation.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::MD5KeyDerivation::MD5KeyDerivation()
{
}

// 0x00382518 slot 0x00 | slot vf_0x00 of nn::nex::MD5KeyDerivation
nn::nex::MD5KeyDerivation::~MD5KeyDerivation()
{
}

// 0x00382514 slot 0x04 | virtual slot, introduced by nn::nex::MD5KeyDerivation
void nn::nex::MD5KeyDerivation::vf_0x04()
{
}

// 0x003824C0 slot 0x08 | mk7dlp:bytes-fuzzy
void nn::nex::MD5KeyDerivation::CreateKey(unsigned, const char*)
{
}

// 0x003823C0 slot 0x0C | virtual slot, introduced by nn::nex::MD5KeyDerivation
void nn::nex::MD5KeyDerivation::vf_0x0C()
{
}

// 0x0038230C slot 0x10 | virtual slot, introduced by nn::nex::MD5KeyDerivation
void nn::nex::MD5KeyDerivation::vf_0x10()
{
}

// 0x00382450 slot 0x14 | fefates:bytes
void nn::nex::MD5KeyDerivation::CreateKey(nn::nex::CallContext*, unsigned int, const char*, nn::nex::Key*, nn::nex::KeyCache*)
{
}

// 0x00382400 slot 0x18 | fefates:bytes
void nn::nex::MD5KeyDerivation::InitializeKey(nn::nex::CallContext*, const char*, nn::nex::Key*)
{
}

// 0x00382230 slot 0x1C | virtual slot, introduced by nn::nex::MD5KeyDerivation
void nn::nex::MD5KeyDerivation::vf_0x1C()
{
}

} // namespace nex
} // namespace nn
