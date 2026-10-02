#pragma once

#include "decomp.h"
#include "nn/nex/nex_ByteStream.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex7MessageE @ 0x008CF674
// vtable 0x008FFB58 (vptr 0x008FFB60), offset_to_top 0, 2 entries
class Message : public ::nn::nex::ByteStream
{
public:
    Message(); // ctor candidate(s) 0x003D23CC, 0x003D2490 (unverified)
    virtual ~Message(); // 0x003D2600 slot 0x00 | fefates:bytes-fuzzy
    // 0x003D25AC slot 0x04 | fefates:bytes-fuzzy (deleting dtor)
    void AddDataFromMessage(const nn::nex::Message&); // 0x003D2324 | fefates:bytes [tier B]
    void SetSize(); // 0x003D2348 | fefates:bytes [tier B]
    void GetBuffer(); // 0x003D23B8 | fefates:bytes [tier B]
    Message(nn::nex::Buffer*); // 0x003D23CC | fefates:bytes-fuzzy [tier B]
};
} // namespace nex
} // namespace nn
