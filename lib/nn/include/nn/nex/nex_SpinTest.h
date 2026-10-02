#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex8SpinTestE @ 0x008CF714
// vtable 0x008FFCB8 (vptr 0x008FFCC0), offset_to_top 0, 2 entries
class SpinTest : public ::nn::nex::RootObject
{
public:
    SpinTest(); // ctor candidate(s) 0x0017F290, 0x00356F58, 0x00362A00, 0x00367E38, 0x00382AA8, 0x003D5EAC (unverified)
    virtual void vf_0x00(); // 0x003D5F48 slot 0x00 | virtual slot, introduced by nn::nex::SpinTest
    virtual void vf_0x04(); // 0x003D5F24 slot 0x04 | virtual slot, introduced by nn::nex::SpinTest
    void SpinOnce(wchar_t*, unsigned int, wchar_t*); // 0x003D5E20 | fefates:bytes-fuzzy [tier B]
    SpinTest(unsigned int, unsigned int); // 0x003D5EAC | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
