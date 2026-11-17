#pragma once

#include "decomp.h"
#include "Audio/dAudioObj1.h"
#include "g3d/dSkeletalAnim.h"
#include "g3d/dSkeletalModel.h"
#include "hobj/dHouseBase.h"
#include "mutil/dResourceLoaderMgr.h"
#include "sead/seadHeap.h"
#include "ssys/ma/dHeapAllocator.h"

// The tent (the first house of a player; class name from the RTTI and the heap name "hobj::Tent::Allocator", the rest
// is ours). The model "Strc/hobj/tent/hobj_tent_01" with the textures "hobj_tent_<kind>" of the season; the door is the
// skeletal animation of the model: it stops in the middle (open) for 11 frames and then closes.
namespace hobj {
// RTTI N4hobj4TentE @ 0x008D1108
// vtable 0x00904908 (vptr 0x00904910), offset_to_top 0, 17 entries
class Tent : public ::hobj::HouseBase
{
public:
    Tent(); // 0x00533510
    virtual ~Tent(); // 0x00533624 slot 0x00

    // the heaps (in the structure heap of HeapInit), the kind of the textures and the sound at world
    void Initialize(u8 kind, const nn::math::VEC3* world); // 0x0053313C

    virtual bool Load() override; // 0x00532F80 slot 0x08
    virtual void Build(const void* houseData) override; // 0x00532D5C slot 0x0C
    virtual void Finalize() override; // 0x00533428 slot 0x10
    virtual void Update(const nn::math::VEC3* world, bool isPaused) override; // 0x00532C58 slot 0x14
    virtual void UpdateMatrix(const nn::math::MTX34& matrix) override; // 0x0053340C slot 0x18
    virtual void AddToScene() override; // 0x005333F0 slot 0x1C
    virtual void OpenDoor(bool isSecondAnim, netgame::PlayerNo player) override; // 0x005332B4 slot 0x20
    virtual void OpenDoorWithChime(bool isSecondAnim, netgame::PlayerNo player,
                                   const u8* packedMelody) override; // 0x005333E4 slot 0x24
    virtual bool HoldDoorOpen(bool isSecondAnim) override; // 0x00533324 slot 0x28
    virtual void CloseDoor() override; // 0x0053338C slot 0x2C
    virtual void UpdateColor(const void* houseData) override; // 0x005333E0 slot 0x30
    virtual void vf_0x34() override; // 0x005333D4 slot 0x34
    virtual void vf_0x38() override; // 0x005333DC slot 0x38
    virtual void vf_0x3C() override; // 0x005333D8 slot 0x3C
    DECOMP_EMIT_COPY virtual u32 GetResourceFreeSize() override; // 0x0074915C slot 0x40

    // the frames the open door waits before it closes
    static const u8 OPEN_WAIT_FRAMES = 11;

    ssys::ma::HeapAllocator m_Allocator;                          // 0x004
    mutil::BaseResourceMgr<g3d::ResourceLoader, 2> m_ResourceMgr; // 0x00C
    g3d::SkeletalModel m_Model;                                   // 0x270
    g3d::SkeletalAnim m_Anim;                                     // 0x27C
    u8 m_Kind;                                                    // 0x294
    AudioObj1 m_Sound;                                            // 0x298
    mutil::ResourceKey m_ModelKey;                                // 0x2F8
    mutil::ResourceKey m_TextureKey;                              // 0x300
    bool m_IsVisible;                                             // 0x308
    u8 m_OpenTimer;                                               // 0x309
};
ASSERT_SIZE(Tent, 0x30C);

inline u32 Tent::GetResourceFreeSize()
{
    sead::Heap* heap = m_ResourceMgr.m_Allocator.m_pHeap;
    if (heap == NULL) {
        return 0;
    }
    return heap->getFreeSize();
}
} // namespace hobj
