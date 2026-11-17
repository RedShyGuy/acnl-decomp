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

// The fully extended house of a player (class name from the RTTI and the heap name "hobj::MyHouseMax", the rest is
// ours). Like MyHouse, but the model "Strc/hobj/my_house/hobj_myhouse_max_<style>/hobj_myhouse_max_<style><door shape>"
// depends on the style of the exterior, the walls use "wallmax_<brick>" and there is no wall mask.
namespace hobj {
// RTTI N4hobj10MyHouseMaxE @ 0x008D10FC
// vtable 0x009048BC (vptr 0x009048C4), offset_to_top 0, 17 entries
class MyHouseMax : public ::hobj::HouseBase
{
public:
    MyHouseMax(); // 0x00530588
    virtual ~MyHouseMax(); // 0x005306A0 slot 0x00

    // the heaps (in the structure heap of HeapInit), the exterior and the door with its sound at world
    void Initialize(const SvHouseExterior* exterior, const nn::math::VEC3* world); // 0x005301A4

    virtual bool Load() override; // 0x0052FE3C slot 0x08
    virtual void Build(const void* houseData) override; // 0x0052F998 slot 0x0C
    virtual void Finalize() override; // 0x005304B0 slot 0x10
    virtual void Update(const nn::math::VEC3* world, bool isPaused) override; // 0x00530DBC slot 0x14
    virtual void UpdateMatrix(const nn::math::MTX34& matrix) override; // 0x00530438 slot 0x18
    virtual void AddToScene() override; // 0x00530418 slot 0x1C
    virtual void OpenDoor(bool isSecondAnim, netgame::PlayerNo player) override; // 0x005315A0 slot 0x20
    virtual void OpenDoorWithChime(bool isSecondAnim, netgame::PlayerNo player,
                                   const u8* packedMelody) override; // 0x00531894 slot 0x24
    virtual bool HoldDoorOpen(bool isSecondAnim) override; // 0x005314F0 slot 0x28
    virtual void CloseDoor() override; // 0x00531750 slot 0x2C
    virtual void UpdateColor(const void* houseData) override; // 0x00530340 slot 0x30
    virtual void vf_0x34() override; // 0x00530334 slot 0x34
    virtual void vf_0x38() override; // 0x0053033C slot 0x38
    virtual void vf_0x3C() override; // 0x00530338 slot 0x3C
    DECOMP_EMIT_COPY virtual u32 GetResourceFreeSize() override; // 0x0074906C slot 0x40

private:
    // (inline) the color of the walls: constant color 2 of "m_wall" from m_Color and the brightness of the light
    void ApplyWallColor();

public:
    ssys::ma::HeapAllocator m_Allocator;                          // 0x004
    mutil::BaseResourceMgr<g3d::ResourceLoader, 3> m_ResourceMgr; // 0x00C
    g3d::SkeletalModel m_Model;                                   // 0x38C
    nw::gfx::res::ResMaterialData* m_pWallMaterial;               // 0x398 "m_wall"
    nw::gfx::res::ResMaterialData* m_pLampMaterial;               // 0x39C "m_lamp" (NULL: none)
    SvHouseExterior m_Exterior;                                   // 0x3A0
    Door m_Door;                                                  // 0x3AC
    mutil::ResourceKey m_ModelKey;                                // 0x730
    mutil::ResourceKey m_RoofKey;                                 // 0x738
    mutil::ResourceKey m_WallKey;                                 // 0x740
    u8 m_Color[4];                                                // 0x748 from Func720F9C (1..3: blue, green, red)
};
ASSERT_SIZE(MyHouseMax, 0x74C);

inline u32 MyHouseMax::GetResourceFreeSize()
{
    sead::Heap* heap = m_ResourceMgr.m_Allocator.m_pHeap;
    if (heap == NULL) {
        return 0;
    }
    return heap->getFreeSize();
}
} // namespace hobj
