#pragma once

#include "decomp.h"
#include "Search/dSearchCandCore.h"

// Instantiations found in the binary:
//   SearchCand<102>  typeinfo 0x008CB134  vtable 0x008EC320
//   SearchCand<10>  typeinfo 0x008CB140  vtable 0x008EC32C
//   SearchCand<11>  typeinfo 0x008CB14C  vtable 0x008EC338
//   SearchCand<16>  typeinfo 0x008CB158  vtable 0x008EC344
//   SearchCand<256>  typeinfo 0x008CB164  vtable 0x008EC350
//   SearchCand<2>  typeinfo 0x008CB170  vtable 0x008EC35C
//   SearchCand<48>  typeinfo 0x008CB17C  vtable 0x008EC368
//   SearchCand<4>  typeinfo 0x008CB188  vtable 0x008EC374
//   SearchCand<50>  typeinfo 0x008CB194  vtable 0x008EC380
//   SearchCand<5>  typeinfo 0x008CB1A0  vtable 0x008EC38C
//   SearchCand<7>  typeinfo 0x008CB1AC  vtable 0x008EC398
//   SearchCand<8>  typeinfo 0x008CB1B8  vtable 0x008EC3A4
// the candidates 0..N-1 with their bits (names are ours)
template <int N>
class SearchCand : public SearchCandCore
{
public:
    // (inline)
    SearchCand() : SearchCandCore(N, m_Bits) {}

    u32 m_Bits[(N + 31) / 32]; // 0x14
};
