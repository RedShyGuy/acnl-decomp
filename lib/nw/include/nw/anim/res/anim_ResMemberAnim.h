#pragma once

#include "decomp.h"

namespace nw {
namespace anim {
namespace res {
class ResMemberAnim
{
public:
    void GetPrimitiveSize() const; // 0x007438D4 | nintendogs:callgraph [tier A]
    void ApplyCacheForType(void*, const void*) const; // 0x00743940 | nintendogs:callgraph [tier A]
    void EvaluateResultForType(void*, unsigned, float, const void*) const; // 0x00743A4C | nintendogs:bytes-fuzzy [tier A]
};
} // namespace res
} // namespace anim
} // namespace nw
