#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex6StringE @ 0x008CF5DC
// vtable 0x008FFA78 (vptr 0x008FFA80), offset_to_top 0, 2 entries
class String : public ::nn::nex::RootObject
{
public:
    String(); // ctor candidate(s) 0x0035524C, 0x00355534, 0x003558FC, 0x00361F24, 0x003917F8, 0x003938A4, 0x003D1250, 0x003D12AC, 0x003D12D8, 0x003D1300, 0x003D3AE4, 0x003DA184, 0x003DA284, 0x003DA388, 0x00729A88, 0x00729C88, 0x00729E88, 0x00729F6C, 0x0072A128, 0x0072E040, 0x0072E0A8, 0x0072E1C8, 0x0072E374, 0x0072E560, 0x007FF074, 0x007FF204, 0x007FFD1C, 0x007FFE9C, 0x0082E600 (unverified)
    virtual ~String(); // 0x003D135C slot 0x00 | fefates:bytes
    // 0x003D132C slot 0x04 | slot vf_0x04 of nn::nex::String (deleting dtor)
    void ReleaseCopy(char*); // 0x003D1140 | fefates:bytes [tier B]
    void Format(const wchar_t*, ...); // 0x003D1168 | fefates:bytes [tier B]
    void IsEqual(const wchar_t*, const wchar_t*); // 0x003D11A4 | fefates:bytes [tier B]
    void Reserve(int); // 0x003D11F4 | fefates:bytes [tier B]
    String(const char*); // 0x003D1250 | mk7dlp:bytes-fuzzy [tier A]
    String(const wchar_t*); // 0x003D12AC | mk7dlp:callgraph [tier A]
    String(const nn::nex::String&); // 0x003D12D8 | fefates:bytes [tier B]
    void operator =(const char*); // 0x003D13A4 | mk7dlp:callseq [tier A]
    void operator=(const wchar_t*); // 0x003D1424 | fefates:bytes [tier B]
    void operator=(const nn::nex::String&); // 0x003D1484 | fefates:bytes [tier B]
    void operator+=(const nn::nex::String&); // 0x003D14DC | fefates:bytes [tier B]
    void CreateCopy(char**) const; // 0x0072E178 | fefates:bytes [tier B]
    void FindSubstringNoCase(const wchar_t*) const; // 0x0072E1C8 | fefates:bytes [tier B]
    void ToUInt64() const; // 0x0072E2CC | fefates:bytes [tier B]
    void GetLength() const; // 0x0072E360 | mk7dlp:callgraph [tier A]
};
} // namespace nex
} // namespace nn
