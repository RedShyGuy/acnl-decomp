#include "nn/ir/CTR/CTR_Api.h"

namespace nn {
namespace ir {
namespace CTR {
// (name is ours)
// 0x00982ED0
CepdStatus s_CepdStatus;

// 0x0034B710 | nintendogs:callgraph [tier A]
CepdStatus CepdGetStatus()
{
    return s_CepdStatus;
}

} // namespace CTR
} // namespace ir
} // namespace nn
