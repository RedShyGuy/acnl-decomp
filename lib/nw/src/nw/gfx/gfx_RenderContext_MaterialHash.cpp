#include "nw/gfx/gfx_RenderContext_MaterialHash.h"

#include "nw/gfx/gfx_Material.h"

namespace nw {
namespace gfx {
// 0x00495DB0 | nintendogs:bytes [tier A]
void nw::gfx::RenderContext::MaterialHash::ResetMaterialHash(int flags)
{
    if (flags & Material::BUFFER_USER_UNIFORMS) {
        m_UserUniformsHash = 0xFFFFFFFF;
    }
    if (flags & Material::BUFFER_SHADING) {
        m_ShadingHash = 0xFFFFFFFF;
    }
    if (flags & Material::BUFFER_MATERIAL_COLOR) {
        m_MaterialColorHash = 0xFFFFFFFF;
    }
    if (flags & Material::BUFFER_RASTERIZATION) {
        m_RasterizationHash = 0xFFFFFFFF;
    }
    if (flags & Material::BUFFER_TEXTURE_COORDINATORS) {
        m_TextureCoordinatorsHash = 0xFFFFFFFF;
    }
    if (flags & Material::BUFFER_TEXTURE_MAPPERS) {
        m_TextureMappersHash = 0xFFFFFFFF;
    }
    if (flags & Material::BUFFER_FRAGMENT_LIGHTING) {
        m_FragmentLightingHash = 0xFFFFFFFF;
    }
    if (flags & Material::BUFFER_FRAGMENT_LIGHTING_TABLE) {
        m_FragmentLightingTableHash = 0xFFFFFFFF;
    }
    if (flags & Material::BUFFER_TEXTURE_COMBINERS) {
        m_TextureCombinersHash = 0xFFFFFFFF;
    }
    if (flags & Material::BUFFER_FRAGMENT_OPERATION) {
        m_FragmentOperationHash = 0xFFFFFFFF;
    }
    if (flags & Material::BUFFER_COMBINER_BUFFER) {
        m_CombinerBufferHash = 0xFFFFFFFF;
    }
}

} // namespace gfx
} // namespace nw
