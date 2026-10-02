#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_qBuffer.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x003D3DAC, 0x003D3E60 (unverified)
nn::nex::qBuffer::qBuffer()
{
}

// 0x003D3F20 slot 0x00 | fefates:bytes
nn::nex::qBuffer::~qBuffer()
{
}

// 0x003D3BD0 | fefates:bytes [tier B]
void nn::nex::qBuffer::initialize(unsigned int)
{
}

// 0x003D3C38 | fefates:bytes [tier B]
void nn::nex::qBuffer::clear()
{
}

// 0x003D3C64 | fefates:bytes [tier B]
void nn::nex::qBuffer::resize(unsigned int, const unsigned char*, unsigned int)
{
}

// 0x003D3D2C | fefates:bytes [tier B]
void nn::nex::qBuffer::push_back(const unsigned char*, unsigned int)
{
}

// 0x003D3DAC | fefates:bytes [tier B]
nn::nex::qBuffer::qBuffer(const nn::nex::qBuffer&)
{
}

// 0x003D3E60 | fefates:bytes [tier B]
nn::nex::qBuffer::qBuffer(unsigned int, unsigned int)
{
}

// 0x003D3F78 | fefates:bytes [tier B]
void nn::nex::qBuffer::operator=(const nn::nex::qBuffer&)
{
}

} // namespace nex
} // namespace nn
