#pragma once

#include "decomp.h"
#include "nn/nex/nex_RefCountedObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex6BufferE @ 0x008CF570
// vtable 0x008FF9FC (vptr 0x008FFA04), offset_to_top 0, 2 entries
class Buffer : public ::nn::nex::RefCountedObject
{
public:
    virtual ~Buffer(); // 0x003D0078 slot 0x00 | mk7dlp:callseq-callee
    // 0x003D0044 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    void AppendData(const void*, unsigned, unsigned); // 0x003CF8F8 | mk7dlp:callgraph [tier A]
    void Initialize(unsigned int, unsigned char); // 0x003CF97C | fefates:bytes [tier B]
    void ComputeCheckSum(unsigned int, unsigned char); // 0x003CFA38 | fefates:bytes [tier B]
    void ResizeByRealSize(unsigned int); // 0x003CFC30 | fefates:bytes [tier B]
    Buffer(const nn::nex::Buffer&); // 0x003CFF38 | mk7dlp:callseq-callee [tier A]
    Buffer(unsigned int); // 0x003CFFBC | fefates:bytes [tier B]
    Buffer(); // 0x003D0010 | mk7dlp:callseq-callee [tier A]
    void operator =(const nn::nex::Buffer&); // 0x003D00D0 | mk7dlp:callgraph [tier A]
    void operator [](unsigned); // 0x003D016C | mk7dlp:callseq-callee [tier A]
    void ToString() const; // 0x0072E0A8 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
