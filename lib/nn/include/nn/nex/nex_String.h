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
    String(); // 0x003D1300 (symbols.json names it nn::boss::TaskIdList::TaskIdList)
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
    u32 GetLength() const; // 0x0072E360 | mk7dlp:callgraph [tier A]

    // (the member name is ours)
    wchar_t* m_pString; // 0x04 (nex::CopyString)
};
ASSERT_SIZE(String, 0x8);
} // namespace nex
} // namespace nn
