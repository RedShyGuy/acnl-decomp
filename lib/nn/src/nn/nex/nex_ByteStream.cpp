#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_ByteStream.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x0035EE40, 0x0035F7F8, 0x00363A90, 0x003641EC, 0x00369DB4, 0x0036C124, 0x0038E420, 0x0038FD30, 0x003CD514 (unverified)
nn::nex::ByteStream::ByteStream()
{
}

// 0x00354DF0 slot 0x00 | fefates:callgraph
nn::nex::ByteStream::~ByteStream()
{
}

// 0x00354C70 | fefates:bytes-fuzzy [tier B]
void nn::nex::ByteStream::ExtractRaw(unsigned char*, unsigned int)
{
}

// 0x00354D20 | fefates:bytes [tier B]
void nn::nex::ByteStream::Append(const nn::nex::Time*)
{
}

// 0x00354DA0 | fefates:bytes [tier B]
void nn::nex::ByteStream::SetLength(unsigned int)
{
}

// 0x003CF8EC | mk7dlp:callgraph [tier A]
void nn::nex::ByteStream::AppendRaw(const unsigned char*, unsigned)
{
}

} // namespace nex
} // namespace nn
