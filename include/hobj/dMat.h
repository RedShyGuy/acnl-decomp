#pragma once

#include "decomp.h"
#include "g3d/dModel.h"
#include "mutil/dResourceLoaderMgr.h"
#include "nn/math/math_MTX34.h"
#include "ssys/ma/dHeapAllocator.h"

// The doormat in front of a house (class name after the heap name "hobj::Mat::Allocator"; the rest is ours; no vtable).
// The model "Strc/hobj/mat/hobj_mat_<kind>" with the textures of the season.
namespace hobj {
class Mat
{
public:
    Mat(); // 0x00530CF4
    ~Mat(); // 0x00530D70

    // the heaps (in the structure heap of HeapInit) for the mat of kind
    void Initialize(u8 kind); // 0x00530B44
    // the files of the model and of its textures are loaded (true when both have a key)
    bool Load(); // 0x00530924
    // the model from the loaded files
    void Build(); // 0x00530820
    void Finalize(); // 0x00530C04
    void AddToScene(); // 0x00530BC8
    // the local matrix and then the world matrix
    void Update(const nn::math::MTX34& matrix); // 0x00530BD4
    // the resource of the model (the nw::gfx::res::ResModelData)
    const void* GetResource() const; // 0x00530BF0
    nn::math::MTX34 GetLocalMatrix() const; // 0x00530CD4
    // (inline; the modules call the copy) whether the model is built
    DECOMP_EMIT_COPY bool IsBuilt() const { return m_Model.GetNode() != NULL; } // 0x00749088

    ssys::ma::HeapAllocator m_Allocator;                          // 0x000
    mutil::BaseResourceMgr<g3d::ResourceLoader, 2> m_ResourceMgr; // 0x008
    g3d::Model m_Model;                                           // 0x26C
    u8 m_Kind;                                                    // 0x278
    mutil::ResourceKey m_ModelKey;                                // 0x27C
    mutil::ResourceKey m_TextureKey;                              // 0x284
};
ASSERT_SIZE(Mat, 0x28C);
} // namespace hobj
