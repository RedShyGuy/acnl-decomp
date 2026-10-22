#include "nw/gfx/res/gfx_ResTextureMapper.h"

namespace nw {
namespace gfx {
namespace res {
// 0x004A7D08 | nintendogs:bytes [tier B]
void nw::gfx::res::ResTextureSampler::SetTextureMipmapCommand()
{
    ResPixelBasedTextureMapperData* pOwner = ptr()->owner.To<ResPixelBasedTextureMapperData>();
    const ResTextureData* pTexture = pOwner->texture.To<ResTextureData>();
    const u32 minFilter = m_pData->minFilter & 0xFF;
    u32 maxLevel = 0;
    if (pTexture != nullptr && minFilter != ResTextureSamplerData::MIN_FILTER_LINEAR && minFilter != ResTextureSamplerData::MIN_FILTER_NEAREST) {
        maxLevel = static_cast<const ResPixelBasedTextureData*>(internal::DereferenceTexture(pTexture))->mipmapLevels - 1;
    }
    u32& lod = pOwner->commands[ResPixelBasedTextureMapperData::COMMAND_LOD];
    lod = (lod & ~0xF0000) | ((maxLevel << 28) >> 12);
}

} // namespace res
} // namespace gfx
} // namespace nw
