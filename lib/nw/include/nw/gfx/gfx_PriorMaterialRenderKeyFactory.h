#pragma once

#include <stdint.h>
#include "decomp.h"
#include "nw/gfx/gfx_BasicRenderKeyFactory.h"
#include "nw/gfx/gfx_Material.h"
#include "nw/gfx/gfx_Model.h"

namespace nw {
namespace gfx {
// Instantiations found in the binary:
//   nw::gfx::PriorMaterialRenderKeyFactory<unsigned long long, nw::gfx::BasicRenderElement<unsigned long long>::Depth24bitQuantizer>  typeinfo 0x008D07B4  vtable 0x00902970
// Render keys sorted by the material before the depth: bits 24-52 the material (with three bits of
// the address of its model), 0-23 the quantized depth (the class name is from the reference symbols,
// the rest is ours).
template <typename TKey, typename TQuantizer>
class PriorMaterialRenderKeyFactory : public BasicRenderKeyFactory<TKey>
{
public:
    typedef BasicRenderKeyFactory<TKey> Base;
    typedef typename Base::Element Element;

    // (inline)
    explicit PriorMaterialRenderKeyFactory(os::IAllocator* pAllocator) : Base(pAllocator) {}
    virtual ~PriorMaterialRenderKeyFactory() {}

    virtual TKey CreateRenderKey(const Element& element, f32 depth, u8 layer)
    {
        TKey key = TQuantizer()(depth);
        const res::ResMeshData* pMesh = element.m_Mesh.ptr();
        Model* pModel = element.m_pModel;
        if (pMesh != nullptr && pModel != nullptr) {
            const Material* pMaterial = pModel->GetMaterial(pMesh->materialIndex);
            const u32 id = pMaterial->GetOriginal().ptr()->renderKeyId;
            const u32 owner = static_cast<u32>((static_cast<u64>(reinterpret_cast<uintptr_t>(pMaterial->GetOwner())) >> 2) & 7);
            const u32 material = (((id & ~0x03C00000u) << 3) | owner) & 0x1FFFFFFF;
            key |= static_cast<TKey>(material) << 24;
            const u32 translucencyKind = pMaterial->GetResource(Material::RESOURCE_SHADING).ptr()->translucencyKind;
            key |= static_cast<TKey>(translucencyKind & 3) << Base::TRANSLUCENCY_KIND_SHIFT;
            key |= static_cast<TKey>(pMesh->renderPriority & 0xF) << 49;
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
