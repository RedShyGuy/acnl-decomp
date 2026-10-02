#pragma once

#include "decomp.h"
#include "nn/nex/nex_HighLevelStream.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex18NATTraversalStreamE @ 0x008CE730
// vtable 0x008FD544 (vptr 0x008FD54C), offset_to_top 0, 7 entries
class NATTraversalStream : public ::nn::nex::HighLevelStream
{
public:
    NATTraversalStream(); // ctor candidate(s) 0x0038D610 (unverified)
    virtual ~NATTraversalStream(); // 0x003902A8 slot 0x00 | slot vf_0x00 of nn::nex::Stream
    // 0x00390224 slot 0x04 | slot vf_0x04 of nn::nex::Stream (deleting dtor)
    virtual void DoWork(); // 0x00390210 slot 0x0C | slot vf_0x0C of nn::nex::Stream
    virtual void vf_0x14(); // 0x00390170 slot 0x14 | mk7dlp:callseq
};
} // namespace nex
} // namespace nn
