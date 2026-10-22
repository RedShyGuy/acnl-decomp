#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResLookupTable.h"

namespace nw {
namespace gfx {
namespace res {
// The lookup tables of the fragment lighting: six samplers (names are ours; which PICA table each one
// feeds is not known yet).
struct ResFragmentLightingTableData
{
    ut::Offset samplers[6]; // 0x0 ResLightingLookupTableData
};
ASSERT_SIZE(ResFragmentLightingTableData, 0x18);

// the binary resource of the lighting tables (the class name is from the reference symbols)
class ResFragmentLightingTable
{
public:
    // (inline)
    explicit ResFragmentLightingTable(const void* pData = nullptr)
        : m_pData(static_cast<const ResFragmentLightingTableData*>(pData))
    {
    }
    const ResFragmentLightingTableData* ptr() const { return m_pData; }
    ResLightingLookupTable GetSampler(int index) const { return ResLightingLookupTable(m_pData->samplers[index].ToPtr()); }

    void Cleanup(); // 0x00140F80 | nintendogs:bytes-fuzzy [tier A]
    u32 Setup(os::IAllocator* pAllocator, ResGraphicsFile graphicsFile); // 0x004A8874 | nintendogs:bytes [tier A]

    const ResFragmentLightingTableData* m_pData; // 0x0
};
ASSERT_SIZE(ResFragmentLightingTable, 0x4);

// (inline) the lookup table of a lighting sampler: a reference is resolved, then the table set up
inline u32 SetupLightingLookupTable(ResLightingLookupTable table, ResGraphicsFile graphicsFile)
{
    u32 result = RESULT_SUCCESS;
    const u32 referenceResult = SetupReferenceLut(
        ResReferenceLookupTable(DynamicCastData<ResLookupTableData>(table.GetSampler().ptr(), ResLookupTable::TYPE_REFERENCE)), graphicsFile);
    if (!(referenceResult & RESULT_FAILURE_MASK)) {
        result |= table.GetSampler().Setup();
    }
    result |= referenceResult;
    return result;
}
} // namespace res
} // namespace gfx
} // namespace nw
