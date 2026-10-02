#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex11StreamTableE @ 0x008CE08C
// vtable 0x008FC454 (vptr 0x008FC45C), offset_to_top 0, 2 entries
class StreamTable : public ::nn::nex::RootObject
{
public:
    virtual ~StreamTable(); // 0x0035BB98 slot 0x00 | fefates:bytes
    // 0x0035BB68 slot 0x04 | slot vf_0x04 of nn::nex::StreamTable (deleting dtor)
    void FindStream(wchar_t*); // 0x0035BA80 | mk7dlp:bytes-fuzzy [tier A]
    StreamTable(); // 0x0035BAE0 | mk7dlp:bytes [tier A]
};
} // namespace nex
} // namespace nn
