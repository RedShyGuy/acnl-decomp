#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
class CurveLfo
{
public:
    void InitializeCurveTable(); // 0x004D48F8 | fefates:bytes [tier B]
    void Reset(); // 0x004D4950 | fefates:bytes [tier B]
    void Update(int); // 0x004D497C | nintendogs:callseq-callee [tier A]
    CurveLfo(); // 0x004D4A38 | fefates:bytes [tier B]
    void GetValue() const; // 0x00743000 | nintendogs:callseq-callee [tier A]
};
} // namespace internal
} // namespace snd
} // namespace nw
