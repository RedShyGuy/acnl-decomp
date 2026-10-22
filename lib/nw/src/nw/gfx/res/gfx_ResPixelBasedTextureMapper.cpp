#include "nw/gfx/res/gfx_ResTextureMapper.h"

namespace nw {
namespace gfx {
namespace res {
// 0x004A9630 | mk7dlp:bytes [tier B]
void nw::gfx::res::ResPixelBasedTextureMapper::ForceSetupTexture(ResTexture texture)
{
    ResReferenceTextureData* pReference = ptr()->texture.To<ResReferenceTextureData>();
    pReference->target.Set(texture.ptr());
    internal::BuildTextureCommand(ptr(), internal::DereferenceTexture(pReference));
}

// 0x004A9CE0 | nintendogs:bytes [tier B]
void nw::gfx::res::ResPixelBasedTextureMapper::DestroyDynamic()
{
    internal::DestroyPixelBasedTextureMapper(ptr(), ptr()->pDynamicAllocator);
}

} // namespace res
} // namespace gfx
} // namespace nw
