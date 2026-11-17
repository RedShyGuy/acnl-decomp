#pragma once

#include "decomp.h"
#include "Audio/dAudioObjDoor.h"
#include "g3d/dSkeletalAnim.h"
#include "g3d/dSkeletalModel.h"
#include "mutil/dResourceLoaderMgr.h"
#include "netgame/netgame_Api.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector3.h"
#include "ssys/ma/dHeapAllocator.h"

// The door of a house (class name after the heap name "hobj::Door::Allocator"; the rest is ours; no vtable).
// The model "Strc/hobj/door/hobj_door/hobj_door_<shape>" with the textures of the design; the animation of the door
// comes from one of two shared animation files (Func6DF40C/Func6DF244), the first one also shows the closed door.
// MyHouse (door at +0x4C8) and MyHouseMax (+0x3AC) forward their door slots to it.
namespace hobj {
class Door
{
public:
    // the design selects the sounds of the door (DESIGN_NUM designs)
    static const u32 DESIGN_NUM = 22;

    Door(); // 0x00531B48
    ~Door(); // 0x00531BD8

    // the heaps (in the structure heap of HeapInit), the shape (SvHouseExterior::houseDoorShape: 0 "a", 1 "b"), the
    // design (houseDoor) and the sound at world
    void Initialize(u8 shape, u8 design, const nn::math::VEC3* world); // 0x00531454
    // the files of the model and of its textures are loaded (true when both have a key)
    bool Load(); // 0x005312A4
    // the model and the closed door (the first animation at its end, stopped)
    void Build(); // 0x00530FA8
    // the animation: a door that has opened turns around (its frame 32 of 57 or 23 of 49) and closes with the close
    // sound of the design; then the position of the sound
    void Update(const nn::math::VEC3* world, bool isPaused); // 0x00530DC4
    void Finalize(); // 0x00531A60
    void AddToScene(); // 0x00531A38
    // the world matrix of the model
    void UpdateMatrix(const nn::math::MTX34& matrix); // 0x00531A44
    // the door opens with the animation of the second file (isSecondAnim) or of the first one; the sound ignores the
    // master volume for the own player; away from the tours the door chime of the home sounds too
    void Open(bool isSecondAnim, netgame::PlayerNo player); // 0x005315A8
    // the same with the door chime of a packed melody (the sound of the tour test is left out)
    void OpenWithChime(bool isSecondAnim, const u8* packedMelody, netgame::PlayerNo player); // 0x005318A8
    // the door stops once it is open (past frame 31 of the second animation, not at its end, or past frame 22 of the
    // first one); true when it stopped
    bool HoldOpen(bool isSecondAnim); // 0x005314F8
    // the door closes (the animation runs backwards) with the close sound, unless g_Unknown9516C0 keeps it open
    void Close(); // 0x00531758

private:
    // (inline) the sounds of the design
    u32 GetOpenSound() const;
    u32 GetCloseSound() const;

public:
    ssys::ma::HeapAllocator m_Allocator;                          // 0x000
    g3d::SkeletalModel m_Model;                                   // 0x008
    g3d::SkeletalAnim m_Anim;                                     // 0x014
    mutil::BaseResourceMgr<g3d::ResourceLoader, 2> m_ResourceMgr; // 0x02C
    u8 m_Shape;                                                   // 0x290
    u8 m_Design;                                                  // 0x291
    mutil::ResourceKey m_ModelKey;                                // 0x294
    mutil::ResourceKey m_TextureKey;                              // 0x29C
    AudioObjDoor m_Sound;                                         // 0x2A4
};
ASSERT_SIZE(Door, 0x384);
} // namespace hobj
