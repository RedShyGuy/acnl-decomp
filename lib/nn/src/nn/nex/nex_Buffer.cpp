#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_Buffer.h"

namespace nn {
namespace nex {
// 0x003D0078 slot 0x00 | mk7dlp:callseq-callee
nn::nex::Buffer::~Buffer()
{
}

// 0x003CF8F8 | mk7dlp:callgraph [tier A]
void nn::nex::Buffer::AppendData(const void*, unsigned, unsigned)
{
}

// 0x003CF97C | fefates:bytes [tier B]
void nn::nex::Buffer::Initialize(unsigned int, unsigned char)
{
}

// 0x003CFA38 | fefates:bytes [tier B]
void nn::nex::Buffer::ComputeCheckSum(unsigned int, unsigned char)
{
}

// 0x003CFC30 | fefates:bytes [tier B]
void nn::nex::Buffer::ResizeByRealSize(unsigned int)
{
}

// 0x003CFF38 | mk7dlp:callseq-callee [tier A]
nn::nex::Buffer::Buffer(const nn::nex::Buffer&)
{
}

// 0x003CFFBC | fefates:bytes [tier B]
nn::nex::Buffer::Buffer(unsigned int)
{
}

// 0x003D0010 | mk7dlp:callseq-callee [tier A]
nn::nex::Buffer::Buffer()
{
}

// 0x003D00D0 | mk7dlp:callgraph [tier A]
void nn::nex::Buffer::operator =(const nn::nex::Buffer&)
{
}

// 0x003D016C | mk7dlp:callseq-callee [tier A]
void nn::nex::Buffer::operator [](unsigned)
{
}

// 0x0072E0A8 | fefates:bytes [tier B]
void nn::nex::Buffer::ToString() const
{
}

} // namespace nex
} // namespace nn
