#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
class CurveAdshr
{
public:
    void Initialize(float); // 0x004C58E0 | nintendogs:bytes [tier A]
    void SetRelease(int); // 0x004C5928 | nintendogs:bytes [tier A]
    void Reset(float); // 0x004C59A0 | nintendogs:bytes [tier A]
    void Update(int); // 0x004C59BC | nintendogs:callseq-callee [tier A]
    void SetHold(int); // 0x004C5ACC | nintendogs:bytes [tier A]
    void SetDecay(int); // 0x004C5AEC | nintendogs:bytes [tier A]
    CurveAdshr(); // 0x004C5B78 | nintendogs:bytes [tier A]
    void GetValue() const; // 0x0073F30C | nintendogs:bytes [tier A]
};
} // namespace internal
} // namespace snd
} // namespace nw
