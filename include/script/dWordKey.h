#pragma once

#include "decomp.h"

namespace script {
// Instantiations found in the binary:
//   script::WordKey<29u, int>  typeinfo 0x008D3D94  vtable 0x0090B544
//   script::WordKey<45u, int>  typeinfo 0x008D3DA0  vtable 0x0090B57C
//   script::WordKey<57u, int>  typeinfo 0x008D3DAC  vtable 0x0090B5B4
//   script::WordKey<85u, int>  typeinfo 0x008D3DB8  vtable 0x0090B5EC
//   script::WordKey<9u, int>  typeinfo 0x008D3DC4  vtable 0x0090B624
template <auto T0, typename T1>
class WordKey
{
public:
    // TODO: members unknown
};
} // namespace script
