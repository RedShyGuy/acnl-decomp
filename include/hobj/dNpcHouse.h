#pragma once

#include "decomp.h"
#include "g3d/dSkeletalModel.h"
#include "mutil/dResourceLoaderMgr.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector3.h"
#include "ssys/ma/dHeapAllocator.h"

namespace nw {
namespace gfx {
namespace res {
struct ResMaterialData;
} // namespace res
} // namespace gfx
} // namespace nw
namespace sead {
template <typename T>
class BufferedSafeStringBase;
} // namespace sead

// The house of a villager (the class and all names are ours, after the path "Strc/hobj/npc_house"; no vtable). Only
// Load and Build are in the main program, the rest of the class (constructor, the other functions) is in the CRO module
// ModuleOutdoor. The model "hobj_npchouse_<kind>" with the textures of the walls, of the wall mask and of the roof of
// the variants of the house, and the position of its chimney smoke (the bone "smoke").
namespace hobj {
class NpcHouse
{
public:
    // the files of the model and of its textures are loaded (true when all have a key)
    bool Load(); // 0x005353B4
    // the model at world: the textures in the materials "m_roof" and "m_wall", the matrix and the position of the
    // smoke, the color of the walls (Func1E63B0 with 20)
    void Build(const nn::math::VEC3* world); // 0x00534CE8
    // the path of the roof texture of kind and roof in the texture folder of the season
    static void FormatRoofPath(sead::BufferedSafeStringBase<char>* path, s32 kind, s32 roof); // 0x00530708

    ssys::ma::HeapAllocator m_Allocator;                          // 0x000
    mutil::BaseResourceMgr<g3d::ResourceLoader, 4> m_ResourceMgr; // 0x008
    g3d::SkeletalModel m_Model;                                   // 0x4A4
    nw::gfx::res::ResMaterialData* m_pWallMaterial;               // 0x4B0 "m_wall"
    u8 m_Kind;                                                    // 0x4B4
    u8 m_Wall;                                                    // 0x4B5 the variant of the walls and the mask
    u8 m_Roof;                                                    // 0x4B6 the variant of the roof
    mutil::ResourceKey m_ModelKey;                                // 0x4B8
    mutil::ResourceKey m_WallKey;                                 // 0x4C0
    mutil::ResourceKey m_WallMaskKey;                             // 0x4C8
    mutil::ResourceKey m_RoofKey;                                 // 0x4D0
    nn::math::MTX34 m_SmokeMatrix;                                // 0x4D8 the bone "smoke" (local)
    nn::math::VEC3 m_SmokePosition;                               // 0x508 the bone "smoke" at world
    u8 m_Unknown514[0x548 - 0x514];                               // 0x514 (used by the module)
    u8 m_Color[4];                                                // 0x548 from Func1E63B0 (1..3: blue, green, red)
};
ASSERT_SIZE(NpcHouse, 0x54C);
} // namespace hobj
