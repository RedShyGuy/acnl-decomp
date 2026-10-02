#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
// Instantiations found in the binary:
//   nn::nex::LiteBuffer<16>  typeinfo 0x008CDF68  vtable 0x008FC150
//   nn::nex::LiteBuffer<20>  typeinfo 0x008CDF74  vtable 0x008FC160
//   nn::nex::LiteBuffer<32>  typeinfo 0x008CDF80  vtable 0x008FC170
//   nn::nex::LiteBuffer<52>  typeinfo 0x008CDF8C  vtable 0x008FC180
//   nn::nex::LiteBuffer<6>  typeinfo 0x008CDF98  vtable 0x008FC190
//   nn::nex::LiteBuffer<8>  typeinfo 0x008CDFA4  vtable 0x008FC1A0
template <auto T0>
class LiteBuffer
{
public:
    // TODO: members unknown
};
} // namespace nex
} // namespace nn
