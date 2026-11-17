#pragma once

#include "decomp.h"
#include "g3d/dModel.h"
#include "mutil/dResourceLoaderMgr.h"
#include "nn/math/math_MTX34.h"
#include "ssys/ma/dHeapAllocator.h"

// The fence of a house (class name after the heap name "hobj::Fence::Allocator"; the rest is ours; no vtable).
// The model "Strc/hobj/fence/hobj_fence_<kind>/hobj_fence_<kind>_n" or "..._p" (the meaning of the two variants is
// unknown) with the textures of the season.
namespace hobj {
class Fence
{
public:
    Fence(); // 0x00533BDC
    ~Fence(); // 0x00533C5C

    // the heaps (in the structure heap of HeapInit), the variant (true: "_n", false: "_p") and the kind
    void Initialize(bool isVariantN, u8 kind); // 0x00533A58
    // the files of the model and of its textures are loaded (true when both have a key)
    bool Load(); // 0x0053379C
    // the model from the loaded files
    void Build(); // 0x0053368C
    void Finalize(); // 0x00533B0C
    void AddToScene(); // 0x00533AE4
    // the local matrix and then the world matrix
    void UpdateMatrix(const nn::math::MTX34& matrix); // 0x00533AF0

    ssys::ma::HeapAllocator m_Allocator;                          // 0x000
    mutil::BaseResourceMgr<g3d::ResourceLoader, 2> m_ResourceMgr; // 0x008
    g3d::Model m_Model;                                           // 0x26C
    bool m_IsVariantN;                                            // 0x278
    u8 m_Kind;                                                    // 0x279
    mutil::ResourceKey m_ModelKey;                                // 0x27C
    mutil::ResourceKey m_TextureKey;                              // 0x284
};
ASSERT_SIZE(Fence, 0x28C);
} // namespace hobj
