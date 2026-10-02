#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex12StringStreamE @ 0x008CE1D0
// vtable 0x008FC718 (vptr 0x008FC720), offset_to_top 0, 2 entries
class StringStream : public ::nn::nex::RootObject
{
public:
    virtual ~StringStream(); // 0x00361478 slot 0x00 | fefates:bytes
    virtual void vf_0x04(); // 0x00361448 slot 0x04 | fefates:callseq
    void StreamNumber(unsigned char); // 0x00361144 | fefates:bytes [tier B]
    void StreamNumber(int); // 0x003611E4 | fefates:bytes [tier B]
    void StreamNumber(unsigned int); // 0x00361284 | fefates:bytes [tier B]
    void TestFreeRoom(unsigned); // 0x00361314 | mk7dlp:bytes-fuzzy [tier A]
    void Clear(); // 0x003613B4 | fefates:bytes [tier B]
    StringStream(); // 0x00361408 | mk7dlp:bytes [tier A]
    void GetLength() const; // 0x0072A604 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
