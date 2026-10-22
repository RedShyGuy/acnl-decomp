#include "nw/gfx/res/gfx_ResFog.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResLookupTable.h"

namespace nw {
namespace gfx {
namespace res {
// 0x001376C8 | nintendogs:bytes [tier A]
void nw::gfx::res::ResFog::Cleanup()
{
    ResLookupTable table = GetSampler();
    if (table.IsValid()) {
        table.Cleanup();
    }
}

// 0x004A9DEC | nintendogs:bytes [tier A]
u32 nw::gfx::res::ResFog::Setup(os::IAllocator* /*pAllocator*/, ResGraphicsFile graphicsFile)
{
    // a reference is resolved first, then the table set up
    u32 result = RESULT_SUCCESS;
    const u32 referenceResult = SetupReferenceLut(
        ResReferenceLookupTable(DynamicCastData<ResLookupTableData>(GetSampler().ptr(), ResLookupTable::TYPE_REFERENCE)), graphicsFile);
    if (!(referenceResult & RESULT_FAILURE_MASK)) {
        result |= GetSampler().Setup();
    }
    return referenceResult | result;
}

} // namespace res
} // namespace gfx
} // namespace nw
