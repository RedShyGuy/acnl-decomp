#pragma once

#include "decomp.h"

namespace bg {
// Instantiations found in the binary:
//   bg::ExchangeBank<BankVramConfig<66000u, 1u, 0u, 0u, false>, 66000u>  typeinfo 0x008CDB98  vtable 0x008FBBB4
//   bg::ExchangeBank<BankVramConfig<8320u, 1u, 0u, 0u, false>, 8600u>  typeinfo 0x008CDBA4  vtable 0x008FBBDC
template <typename T0, auto T1>
class ExchangeBank
{
public:
    // TODO: members unknown
};
} // namespace bg
