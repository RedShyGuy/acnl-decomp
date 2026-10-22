#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResPrimitive.h"

namespace nw {
namespace gfx {
namespace res {
// a primitive set of a shape (only the fields known so far; names are ours)
struct ResPrimitiveSetData
{
    u32 m_Unknown0x0[3];     // 0x0
    ResArrayInfo primitives; // 0xC ResPrimitiveData
};
ASSERT_SIZE(ResPrimitiveSetData, 0x14);

// the binary resource of a primitive set (the class name and Cleanup are from the reference symbols)
class ResPrimitiveSet
{
public:
    // (inline)
    explicit ResPrimitiveSet(const void* pData = nullptr) : m_pData(static_cast<const ResPrimitiveSetData*>(pData)) {}
    const ResPrimitiveSetData* ptr() const { return m_pData; }
    ResPrimitiveSetData* ptr() { return const_cast<ResPrimitiveSetData*>(m_pData); }

    // the copies and command buffers of the index streams freed
    void Cleanup(); // 0x004A75CC | nintendogs:bytes [tier A]

    const ResPrimitiveSetData* m_pData; // 0x0
};
ASSERT_SIZE(ResPrimitiveSet, 0x4);
} // namespace res
} // namespace gfx
} // namespace nw
