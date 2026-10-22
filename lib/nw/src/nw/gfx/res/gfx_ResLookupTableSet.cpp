#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResLookupTableSet.h"

namespace nw {
namespace gfx {
namespace res {
// 0x00137638 | nintendogs:bytes [tier A]
void nw::gfx::res::ResLookupTableSet::Cleanup()
{
}

// 0x004A7C5C | nintendogs:bytes [tier A]
u32 nw::gfx::res::ResLookupTableSet::Setup(os::IAllocator* /*pAllocator*/, ResGraphicsFile /*graphicsFile*/)
{
    u32 result = RESULT_SUCCESS;
    const s32 tableCount = GetTableCount();
    for (int i = 0; i < tableCount; ++i) {
        result |= GetTable(i).Setup();
    }
    return result;
}

} // namespace res
} // namespace gfx
} // namespace nw
