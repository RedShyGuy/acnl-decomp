#pragma once

#include "decomp.h"
#include "nw/ut/internal/ut_LinkListImpl.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// Told when sound data in memory goes away (the class and InvalidateData are from the reference symbols,
// the member is ours).
// RTTI N2nw3snd8internal6driver15DisposeCallbackE
class DisposeCallback
{
public:
    // (inline)
    DisposeCallback() {}
    virtual ~DisposeCallback(); // 0x004CCD88 slot 0x00

    // the data from pStart up to pEnd goes away
    virtual void InvalidateData(const void* pStart, const void* pEnd) = 0; // slot 0x08

    ut::LinkListNode m_Link; // 0x4 (DisposeCallbackManager)
};
ASSERT_SIZE(DisposeCallback, 0xC);
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
