#pragma once

#include "decomp.h"
#include "Sv/dSvHouse.h"
#include "g3d/dSkeletalModel.h"
#include "hobj/dDoor.h"
#include "hobj/dHouseBase.h"
#include "mutil/dResourceLoaderMgr.h"
#include "sead/seadHeap.h"
#include "ssys/ma/dHeapAllocator.h"

namespace nw {
namespace gfx {
namespace res {
struct ResMaterialData;
} // namespace res
} // namespace gfx
} // namespace nw

// The house of a player (class name from the RTTI and the heap name "hobj::MyHouse::Allocator", the rest is ours). The
// model "Strc/hobj/my_house/hobj_myhouse_<size>/hobj_myhouse_<size><door shape>" of the exterior of the house
// (SvHouseExterior) with the textures of the walls ("wall_<brick>"), of the wall mask and of the roof
// ("<season>/roof_<roof>") in the materials "m_wall", "m_roof" and (when the model has one) "m_lamp", and the Door.
namespace hobj {
// RTTI N4hobj7MyHouseE @ 0x008D1114
// vtable 0x00904954 (vptr 0x0090495C), offset_to_top 0, 17 entries
class MyHouse : public ::hobj::HouseBase
{
public:
    MyHouse(); // 0x00534B38
    virtual ~MyHouse(); // 0x00534C70 slot 0x00

    // the heaps (in the structure heap of HeapInit), the exterior and the door with its sound at world
    void Initialize(const SvHouseExterior* exterior, const nn::math::VEC3* world); // 0x005346E4

    virtual bool Load() override; // 0x00534210 slot 0x08
    virtual void Build(const void* houseData) override; // 0x00533CB4 slot 0x0C
    virtual void Finalize() override; // 0x00534A50 slot 0x10
    virtual void Update(const nn::math::VEC3* world, bool isPaused) override; // 0x00533CA8 slot 0x14
    virtual void UpdateMatrix(const nn::math::MTX34& matrix) override; // 0x005349D0 slot 0x18
    virtual void AddToScene() override; // 0x005349A8 slot 0x1C
    virtual void OpenDoor(bool isSecondAnim, netgame::PlayerNo player) override; // 0x00534888 slot 0x20
    virtual void OpenDoorWithChime(bool isSecondAnim, netgame::PlayerNo player,
                                   const u8* packedMelody) override; // 0x00534990 slot 0x24
    virtual bool HoldDoorOpen(bool isSecondAnim) override; // 0x00534894 slot 0x28
    virtual void CloseDoor() override; // 0x005348A0 slot 0x2C
    virtual void UpdateColor(const void* houseData) override; // 0x005348B8 slot 0x30
    virtual void vf_0x34() override; // 0x005348AC slot 0x34
    virtual void vf_0x38() override; // 0x005348B4 slot 0x38
    virtual void vf_0x3C() override; // 0x005348B0 slot 0x3C
    DECOMP_EMIT_COPY virtual u32 GetResourceFreeSize() override; // 0x00749178 slot 0x40

private:
    // (inline) the color of the walls: constant color 2 of "m_wall" from m_Color and the brightness of the light
    void ApplyWallColor();

public:
    ssys::ma::HeapAllocator m_Allocator;                          // 0x004
    mutil::BaseResourceMgr<g3d::ResourceLoader, 4> m_ResourceMgr; // 0x00C
    g3d::SkeletalModel m_Model;                                   // 0x4A8
    nw::gfx::res::ResMaterialData* m_pWallMaterial;               // 0x4B4 "m_wall"
    nw::gfx::res::ResMaterialData* m_pLampMaterial;               // 0x4B8 "m_lamp" (NULL: none)
    SvHouseExterior m_Exterior;                                   // 0x4BC
    Door m_Door;                                                  // 0x4C8
    mutil::ResourceKey m_ModelKey;                                // 0x84C
    mutil::ResourceKey m_RoofKey;                                 // 0x854
    mutil::ResourceKey m_WallKey;                                 // 0x85C
    mutil::ResourceKey m_WallMaskKey;                             // 0x864
    u8 m_Color[4];                                                // 0x86C from Func720F9C (1..3: blue, green, red)
};
ASSERT_SIZE(MyHouse, 0x870);

inline u32 MyHouse::GetResourceFreeSize()
{
    sead::Heap* heap = m_ResourceMgr.m_Allocator.m_pHeap;
    if (heap == NULL) {
        return 0;
    }
    return heap->getFreeSize();
}
} // namespace hobj
