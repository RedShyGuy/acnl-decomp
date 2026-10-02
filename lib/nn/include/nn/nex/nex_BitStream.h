#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex9BitStreamE @ 0x008CF738
// vtable 0x008FFCF4 (vptr 0x008FFCFC), offset_to_top 0, 2 entries
class BitStream : public ::nn::nex::RootObject
{
public:
    virtual void vf_0x00(); // 0x003D6734 slot 0x00 | fefates:callgraph
    virtual ~BitStream(); // 0x003D66B4 slot 0x04 | fefates:bytes
    void ExtractRaw(unsigned char*, unsigned int); // 0x003D6218 | fefates:bytes-fuzzy [tier B]
    void AppendRaw(const unsigned char*, unsigned); // 0x003D641C | mk7dlp:callseq [tier A]
    BitStream(nn::nex::Buffer*); // 0x003D6618 | fefates:bytes [tier B]
    BitStream(); // 0x003D6670 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
