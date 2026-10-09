#pragma once

#include "decomp.h"

namespace nn {
namespace ir {
namespace CTR {
// the state of the extra pad (values 0-3 are not used in this program; names are ours)
enum CepdStatus : u8 {
    CEPD_STATUS_SAMPLING = 4,
};

// the state of the extra pad (only read here; the library that sets it is not in this program)
CepdStatus CepdGetStatus(); // 0x0034B710 | nintendogs:callgraph [tier A]
} // namespace CTR
} // namespace ir
} // namespace nn
