#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_BasicRenderKeyFactory.h"
#include "nw/gfx/gfx_Material.h"
#include "nw/gfx/gfx_Model.h"

namespace nw {
namespace gfx {
// Instantiations found in the binary:
//   nw::gfx::PriorDepthRenderKeyFactory<unsigned long long, nw::gfx::BasicRenderElement<unsigned long long>::ReverseDepth24bitQuantizer>  typeinfo 0x008D0784  vtable 0x009028EC
// Render keys sorted by the depth before the material: bits 21-44 the quantized depth, 0-20 the
// material (the class name is from the reference symbols, the rest is ours).
template <typename TKey, typename TQuantizer>
class PriorDepthRenderKeyFactory : public BasicRenderKeyFactory<TKey>
{
public:
    typedef BasicRenderKeyFactory<TKey> Base;
    typedef typename Base::Element Element;

    // (inline)
    explicit PriorDepthRenderKeyFactory(os::IAllocator* pAllocator) : Base(pAllocator) {}
    virtual ~PriorDepthRenderKeyFactory() {}

    virtual TKey CreateRenderKey(const Element& element, f32 depth, u8 layer)
    {
        TKey key = TQuantizer()(depth) << 21;
        const res::ResMeshData* pMesh = element.m_Mesh.ptr();
        Model* pModel = element.m_pModel;
        if (pMesh != nullptr && pModel != nullptr) {
            const Material* pMaterial = pModel->GetMaterial(pMesh->materialIndex);
            key |= pMaterial->GetOriginal().ptr()->renderKeyId & 0x1FFFFF;
            const u32 translucencyKind = pMaterial->GetResource(Material::RESOURCE_SHADING).ptr()->translucencyKind;
            key |= static_cast<TKey>(translucencyKind & 3) << Base::TRANSLUCENCY_KIND_SHIFT;
            key |= static_cast<TKey>(pMesh->renderPriority) << Base::PRIORITY_SHIFT;
        }
        return key | static_cast<TKey>(layer) << Base::LAYER_SHIFT;
    }
    virtual TKey CreateCommandRenderKey(u32 command, u8 translucencyKind, u8 priority, u8 layer)
    {
        return static_cast<TKey>(command) | static_cast<TKey>(layer) << Base::LAYER_SHIFT | static_cast<TKey>(priority) << Base::PRIORITY_SHIFT |
               static_cast<TKey>(translucencyKind & 3) << Base::TRANSLUCENCY_KIND_SHIFT | static_cast<TKey>(1) << Base::COMMAND_SHIFT;
    }
};
} // namespace gfx
} // namespace nw
