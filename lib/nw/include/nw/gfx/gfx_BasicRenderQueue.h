#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_BasicRenderElement.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/gfx/gfx_Material.h"
#include "nw/gfx/gfx_SceneHelper.h"
#include "nw/gfx/gfx_Skeleton.h"
#include "nw/gfx/gfx_SkeletalModel.h"
#include "nw/gfx/res/gfx_ResMaterial.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "nw/gfx/res/gfx_ResPrimitiveSet.h"
#include "nw/gfx/res/gfx_ResShape.h"

namespace nw {
namespace gfx {
class Camera;

// Instantiations found in the binary:
//   nw::gfx::BasicRenderQueue<nw::gfx::BasicRenderElement<unsigned long long>, nw::ut::MoveArray<nw::gfx::BasicRenderElement<unsigned long long> >, nw::gfx::BasicRenderKeyFactory<unsigned long long> >  typeinfo 0x008D0670  vtable 0x00902698
// The meshes to draw in one view, each with the key it is sorted by; a key factory per
// translucency kind of the material (the class name and the functor names are from the reference
// symbols, the rest is ours).
template <typename TElement, typename TContainer, typename TKeyFactory>
class BasicRenderQueue : public GfxObject
{
public:
    typedef TElement Element;

    // which meshes get a depth (the others sort at depth 0)
    struct AlwaysCalculatingFunctor
    {
        bool operator()(u8 /*translucencyKind*/) const { return true; }
    };
    // (name is ours) only the translucent meshes
    struct TranslucentCalculatingFunctor
    {
        bool operator()(u8 translucencyKind) const { return translucencyKind == res::ResMaterialData::TRANSLUCENCY_KIND_TRANSLUCENT; }
    };

    // The depth of a mesh of a model at the center of the bounding box of its shape: with the
    // matrix of the bone of a shape that has one primitive set (rigid), else the world matrix of the
    // model.
    struct CalculateDepthOfSkeletalModelFunctor
    {
        template <typename TCalculatingFunctor>
        f32 operator()(res::ResMesh mesh, const Camera& camera) const;

        SkeletalModel* m_pModel; // 0x0
        res::ResModel m_ResModel; // 0x4
    };

    // (inline) a queue with room for capacity elements (isFixedSize: no more)
    BasicRenderQueue(os::IAllocator* pAllocator, int capacity, bool isFixedSize)
        : GfxObject(pAllocator),
          m_Elements(isFixedSize ? TContainer(capacity != 0 ? pAllocator->Alloc(capacity * sizeof(Element), 32) : nullptr, capacity, pAllocator)
                                 : TContainer(pAllocator)),
          m_pOpaqueKeyFactory(nullptr), m_pTranslucentKeyFactory(nullptr), m_pAdditiveKeyFactory(nullptr), m_pSubtractiveKeyFactory(nullptr),
          m_IsKeyCachingEnabled(false)
    {
    }
    virtual ~BasicRenderQueue();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x00828048 slot 0x08

    // (inline) the queue emptied, the key factories that are given replaced
    void Reset(TKeyFactory* pOpaqueKeyFactory, TKeyFactory* pTranslucentKeyFactory, TKeyFactory* pAdditiveKeyFactory,
               TKeyFactory* pSubtractiveKeyFactory)
    {
        m_Elements.clear();
        m_IsKeyCachingEnabled = false;
        if (pOpaqueKeyFactory != nullptr) {
            m_pOpaqueKeyFactory = pOpaqueKeyFactory;
        }
        if (pTranslucentKeyFactory != nullptr) {
            m_pTranslucentKeyFactory = pTranslucentKeyFactory;
        }
        if (pAdditiveKeyFactory != nullptr) {
            m_pAdditiveKeyFactory = pAdditiveKeyFactory;
        }
        if (pSubtractiveKeyFactory != nullptr) {
            m_pSubtractiveKeyFactory = pSubtractiveKeyFactory;
        }
    }

    // the element added with its key (NULL: the queue is full); with key caching the keys of the
    // meshes that are not translucent are kept in the meshes
    Element* Enqueue(const Element& element, f32 depth, u8 translucencyKind, u8 layer);

    static const TypeInfo s_TypeInfo;

    TContainer m_Elements;                 // 0x08
    TKeyFactory* m_pOpaqueKeyFactory;      // 0x18
    TKeyFactory* m_pTranslucentKeyFactory; // 0x1C
    TKeyFactory* m_pAdditiveKeyFactory;    // 0x20
    TKeyFactory* m_pSubtractiveKeyFactory; // 0x24
    bool m_IsKeyCachingEnabled;            // 0x28

private:
    // (inline) the key factory of a translucency kind
    TKeyFactory* GetKeyFactory(u8 translucencyKind) const
    {
        switch (translucencyKind) {
        case res::ResMaterialData::TRANSLUCENCY_KIND_OPAQUE:
            return m_pOpaqueKeyFactory;
        case res::ResMaterialData::TRANSLUCENCY_KIND_TRANSLUCENT:
            return m_pTranslucentKeyFactory;
        case res::ResMaterialData::TRANSLUCENCY_KIND_SUBTRACTIVE:
            return m_pSubtractiveKeyFactory;
        case res::ResMaterialData::TRANSLUCENCY_KIND_ADDITIVE:
            return m_pAdditiveKeyFactory;
        default:
            return nullptr;
        }
    }
};

template <typename TElement, typename TContainer, typename TKeyFactory>
BasicRenderQueue<TElement, TContainer, TKeyFactory>::~BasicRenderQueue()
{
}

template <typename TElement, typename TContainer, typename TKeyFactory>
typename BasicRenderQueue<TElement, TContainer, TKeyFactory>::Element*
BasicRenderQueue<TElement, TContainer, TKeyFactory>::Enqueue(const Element& element, f32 depth, u8 translucencyKind, u8 layer)
{
    if (!m_Elements.push_back(element)) {
        return nullptr;
    }
    Element* pElement = m_Elements.end() - 1;
    if (m_IsKeyCachingEnabled && translucencyKind != res::ResMaterialData::TRANSLUCENCY_KIND_TRANSLUCENT) {
        res::ResMeshData* pMesh = const_cast<res::ResMeshData*>(pElement->m_Mesh.ptr());
        if (!(pMesh->flags & res::ResMeshData::FLAG_RENDER_KEY_CACHED)) {
            pElement->m_Key = GetKeyFactory(translucencyKind)->CreateRenderKey(*pElement, depth, layer);
            pMesh->renderKeyCache = pElement->m_Key;
            pMesh->flags |= res::ResMeshData::FLAG_RENDER_KEY_CACHED;
            return pElement;
        }
        pElement->m_Key = pMesh->renderKeyCache;
    } else {
        pElement->m_Key = GetKeyFactory(translucencyKind)->CreateRenderKey(*pElement, depth, layer);
    }
    return pElement;
}

template <typename TElement, typename TContainer, typename TKeyFactory>
template <typename TCalculatingFunctor>
f32 BasicRenderQueue<TElement, TContainer, TKeyFactory>::CalculateDepthOfSkeletalModelFunctor::operator()(res::ResMesh mesh,
                                                                                                          const Camera& camera) const
{
    const u32 translucencyKind = m_pModel->GetMaterial(mesh.ptr()->materialIndex)->GetResource(Material::RESOURCE_SHADING).ptr()->translucencyKind;
    if (!TCalculatingFunctor()(static_cast<u8>(translucencyKind))) {
        return 0.0f;
    }
    const res::ResSeparateDataShapeData* pShape =
        static_cast<const res::ResSeparateDataShapeData*>(m_ResModel.GetShape(mesh.ptr()->shapeIndex));
    const nn::math::MTX34* pMatrix;
    if (pShape->primitiveSets.count == 1) {
        const res::ResPrimitiveSetData* pPrimitiveSet = static_cast<const res::ResPrimitiveSetData*>(pShape->primitiveSets.GetData(0));
        const s32 boneIdx = *pPrimitiveSet->boneIndices.To<s32>();
        pMatrix = &(*m_pModel->GetSkeleton()->GetWorldMatrices())[boneIdx];
    } else {
        pMatrix = &m_pModel->m_WorldMatrix;
    }
    return SceneHelper::CalculateDepth(pShape->boundingBox.To<res::ResOrientedBoundingBoxData>()->center, *pMatrix, camera);
}
} // namespace gfx
} // namespace nw
