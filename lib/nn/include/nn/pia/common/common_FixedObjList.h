#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
// Instantiations found in the binary:
//   nn::pia::common::FixedObjList<nn::pia::inet::NatDetecter::SendNatCheckMessage, 20u>  typeinfo 0x008CFE10
//   nn::pia::common::FixedObjList<nn::pia::inet::NatTraversalTime, 12u>  typeinfo 0x008CFE1C
template <typename T0, auto T1>
class FixedObjList
{
public:
    // TODO: members unknown
};
} // namespace common
} // namespace pia
} // namespace nn
