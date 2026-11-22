#pragma once

#include "decomp.h"
#include "Other/dGmoLoader.h"
#include "g3d/dSkeletalModel.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector3.h"

namespace ssys {
namespace ma {
class Allocator;
} // namespace ma
} // namespace ssys

namespace stage {
struct NpcInfo;
} // namespace stage

// The accessory of a villager for the season or an event (the model of the accessory resources of BsNpcMgr at a bone
// of the NPC model; no RTTI, all names are ours): a member of NpcModel.
struct NpcAccessoryModel
{
    // m_State: no accessory
    static const u32 STATE_NONE = 3;

    // (inline in the constructor of NpcModel)
    NpcAccessoryModel() : m_State(STATE_NONE) {}
    // (in the area of g3d: the loader, then the model)
    ~NpcAccessoryModel(); // 0x004EE3F4

    // the model of the accessory of kind for info (false: the accessory resources are not loaded or not of kind)
    bool Setup(u16 kind, ssys::ma::Allocator* allocator, g3d::SkeletalModel* parent, const stage::NpcInfo* info); // 0x001CC2EC
    // the model follows its bone of parent
    void Update(g3d::SkeletalModel* parent); // 0x001CC0DC
    // the user data "<name>_t", "<name>_r" (degrees) and "<name>_s" of model (true: one of them)
    bool ReadTransform(g3d::Model* model, const char* name, nn::math::VEC3* translate, nn::math::VEC3* rotate,
                       f32* scale); // 0x001CC150

    g3d::SkeletalModel m_Model;  // 0x00
    u32 m_State;                 // 0x0C (STATE_NONE: none, else the kind)
    u32 m_Unknown10;             // 0x10
    u32 m_Unknown14;             // 0x14
    s32 m_Bone;                  // 0x18 the bone head of the parent
    nn::math::MTX34 m_OffsetMtx; // 0x1C from the bone
    f32 m_Scale;                 // 0x4C
    GmoLoader m_Loader;          // 0x50
};
ASSERT_OFFSET(NpcAccessoryModel, m_Loader, 0x50);
ASSERT_SIZE(NpcAccessoryModel, 0x144);

class SvFgName;
// (functions of the game that NpcAccessoryModel calls; names after the addresses while the purpose is unknown)
// the name of the accessory resources of kind (Thumb)
const char* Func117820(u16 kind); // 0x00117820
// the high four bits of byte 0x1C of the entry of a villager (the accessory of kind 1 by it; for kind 0 the values 0..3
// take the second hat)
u32 Func71C420(const u16* villagerId); // 0x0071C420
// the item index + 1 of a pair of items behind an item (the empty item for 2 or more)
SvFgName Func71CE28(const SvFgName* items, u32 index); // 0x0071CE28
// the transformation of an accessory in the loaded GMO data (meaning unknown)
void Func1AAFF8(void* data, const char* name, nn::math::VEC3* translate, nn::math::VEC3* rotate, f32* scale); // 0x001AAFF8

