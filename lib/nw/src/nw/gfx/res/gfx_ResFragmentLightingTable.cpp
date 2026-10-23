#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResFragmentLightingTable.h"

namespace nw {
namespace gfx {
namespace res {
namespace {
// (inline) a sampler that is there
inline u32 SetupSampler(ResLightingLookupTable sampler, ResGraphicsFile graphicsFile)
{
    if (!sampler.IsValid()) {
        return RESULT_SUCCESS;
    }
    return SetupLightingLookupTable(sampler, graphicsFile);
}

// (inline) the table of a sampler that is there
inline void CleanupSampler(ResLightingLookupTable sampler)
{
    if (sampler.IsValid()) {
        ResLookupTable table = sampler.GetSampler();
        if (table.IsValid()) {
            table.Cleanup();
        }
    }
}
} // namespace

// 0x00140F80 | nintendogs:bytes-fuzzy [tier A]
void nw::gfx::res::ResFragmentLightingTable::Cleanup()
{
    CleanupSampler(GetSampler(0));
    CleanupSampler(GetSampler(1));
    CleanupSampler(GetSampler(2));
    CleanupSampler(GetSampler(3));
    CleanupSampler(GetSampler(4));
    CleanupSampler(GetSampler(5));
}

// 0x004A8874 | nintendogs:bytes [tier A]
u32 nw::gfx::res::ResFragmentLightingTable::Setup(os::IAllocator* /*pAllocator*/, ResGraphicsFile graphicsFile)
{
    u32 result = RESULT_SUCCESS;
    result |= SetupSampler(GetSampler(3), graphicsFile);
    result |= SetupSampler(GetSampler(4), graphicsFile);
    result |= SetupSampler(GetSampler(0), graphicsFile);
    result |= SetupSampler(GetSampler(1), graphicsFile);
    result |= SetupSampler(GetSampler(2), graphicsFile);
    result |= SetupSampler(GetSampler(5), graphicsFile);
    return result;
}

} // namespace res
} // namespace gfx
} // namespace nw
