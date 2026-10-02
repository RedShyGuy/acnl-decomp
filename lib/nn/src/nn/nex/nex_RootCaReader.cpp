#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_RootCaReader.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::RootCaReader::RootCaReader()
{
}

// 0x00360378 slot 0x00 | virtual slot, introduced by nn::nex::RootCaReader
void nn::nex::RootCaReader::vf_0x00()
{
}

// 0x00360374 slot 0x04 | virtual slot, introduced by nn::nex::RootCaReader
void nn::nex::RootCaReader::vf_0x04()
{
}

// 0x0035FAA4 | fefates:bytes-fuzzy [tier B]
void nn::nex::RootCaReader::Read(nn::nex::qVector<unsigned char>*, const char**, unsigned int)
{
}

// 0x0035FD78 | fefates:bytes-fuzzy [tier B]
void nn::nex::RootCaReader::ReadImpl(nn::nex::qVector<unsigned char>*, const char**, unsigned int)
{
}

} // namespace nex
} // namespace nn
