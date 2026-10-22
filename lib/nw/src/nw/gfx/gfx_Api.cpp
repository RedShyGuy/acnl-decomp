#include "nw/gfx/gfx_Api.h"

#include "nw/gfx/res/gfx_ResGraphicsFile.h"

namespace nw {
namespace gfx {
// 0x004A7240 | libgarden [tier A]
const void* GetModel(const void* const* ppGraphicsFile, const char* name)
{
    const res::ResGraphicsFileData* pData = static_cast<const res::ResGraphicsFileData*>(*ppGraphicsFile);
    return pData->models.GetData(name);
}

} // namespace gfx
} // namespace nw
