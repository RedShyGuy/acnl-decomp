#pragma once

#include "decomp.h"
#include "nn/nex/nex_RefCountedObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex7qBufferE @ 0x008CF698
// vtable 0x008FFB88 (vptr 0x008FFB90), offset_to_top 0, 2 entries
class qBuffer : public ::nn::nex::RefCountedObject
{
public:
    qBuffer(); // ctor candidate(s) 0x003D3DAC, 0x003D3E60 (unverified)
    virtual ~qBuffer(); // 0x003D3F20 slot 0x00 | fefates:bytes
    // 0x003D3EAC slot 0x04 | fefates:bytes (deleting dtor)
    void initialize(unsigned int); // 0x003D3BD0 | fefates:bytes [tier B]
    void clear(); // 0x003D3C38 | fefates:bytes [tier B]
    void resize(unsigned int, const unsigned char*, unsigned int); // 0x003D3C64 | fefates:bytes [tier B]
    void push_back(const unsigned char*, unsigned int); // 0x003D3D2C | fefates:bytes [tier B]
    qBuffer(const nn::nex::qBuffer&); // 0x003D3DAC | fefates:bytes [tier B]
    qBuffer(unsigned int, unsigned int); // 0x003D3E60 | fefates:bytes [tier B]
    void operator=(const nn::nex::qBuffer&); // 0x003D3F78 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
