#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex10ByteStreamE @ 0x008CDF30
// vtable 0x008FC130 (vptr 0x008FC138), offset_to_top 0, 2 entries
class ByteStream : public ::nn::nex::RootObject
{
public:
    ByteStream(); // ctor candidate(s) 0x0035EE40, 0x0035F7F8, 0x00363A90, 0x003641EC, 0x00369DB4, 0x0036C124, 0x0038E420, 0x0038FD30, 0x003CD514 (unverified)
    virtual ~ByteStream(); // 0x00354DF0 slot 0x00 | fefates:callgraph
    // 0x00354DC0 slot 0x04 | fefates:callseq (deleting dtor)
    void ExtractRaw(unsigned char*, unsigned int); // 0x00354C70 | fefates:bytes-fuzzy [tier B]
    void Append(const nn::nex::Time*); // 0x00354D20 | fefates:bytes [tier B]
    void SetLength(unsigned int); // 0x00354DA0 | fefates:bytes [tier B]
    void AppendRaw(const unsigned char*, unsigned); // 0x003CF8EC | mk7dlp:callgraph [tier A]
};
} // namespace nex
} // namespace nn
