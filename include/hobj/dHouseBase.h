#pragma once

#include "decomp.h"
#include "g3d/dSkeletalModel.h"
#include "netgame/netgame_Api.h"
#include "nw/gfx/gfx_Model.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector3.h"

namespace hobj {
namespace detail {
// (inline; names are ours) the position of the material of the name in the model (-1: none)
inline int GetMaterialIndex(const g3d::SkeletalModel& model, const char* name)
{
    return model.GetModel()->GetResModel().ptr()->materials.GetDic().GetIndex(name);
}

// the material of the name of the model
inline nw::gfx::res::ResMaterialData* FindMaterialData(const g3d::SkeletalModel& model, const char* name)
{
    const nw::gfx::res::ResModelData* data = model.GetModel()->GetResModel().ptr();
    const int index = GetMaterialIndex(model, name);
    return static_cast<nw::gfx::res::ResMaterialData*>(data->materials.GetData(index));
}
} // namespace detail

// the suffixes of the door shapes (SvHouseExterior::houseDoorShape; MyHouse and MyHouseMax add them to the model name)
extern const char* g_DoorShapeSuffixes[2];

// The interface of the house models in front of which the player stands (Tent, MyHouse, MyHouseMax; class name from
// the RTTI, the slot names are ours). Only the CRO modules ModuleOutdoor/ModuleExhibition use the houses.
// RTTI N4hobj9HouseBaseE @ 0x008D1120
class HouseBase
{
public:
    // (inline; the binary has no constructor, destructor or vtable of HouseBase)
    HouseBase() {}
    virtual ~HouseBase() {} // slot 0x00 (slot 0x04: deleting)

    // the files of the models and of the textures are loaded (true when all have a key)
    virtual bool Load() = 0; // slot 0x08
    // the models from the loaded files (with the colors of the house data)
    virtual void Build(const void* houseData) = 0; // slot 0x0C
    virtual void Finalize() = 0; // slot 0x10
    // the door animation and the position of the sounds
    virtual void Update(const nn::math::VEC3* world, bool isPaused) = 0; // slot 0x14
    // the world matrices of the models
    virtual void UpdateMatrix(const nn::math::MTX34& matrix) = 0; // slot 0x18
    virtual void AddToScene() = 0; // slot 0x1C
    // the door opens (the house with a Door: with the animation of the second file, isSecondAnim); the sound ignores
    // the master volume for the own player
    virtual void OpenDoor(bool isSecondAnim, netgame::PlayerNo player) = 0; // slot 0x20
    // the same with the door chime of a packed melody (a tent has no door chime)
    virtual void OpenDoorWithChime(bool isSecondAnim, netgame::PlayerNo player, const u8* packedMelody) = 0; // slot 0x24
    // the door stops once it is open; true when it stopped
    virtual bool HoldDoorOpen(bool isSecondAnim) = 0; // slot 0x28
    virtual void CloseDoor() = 0; // slot 0x2C
    // the colors of the model from the data of the house (a tent does nothing)
    virtual void UpdateColor(const void* houseData) = 0; // slot 0x30
    // (empty in all houses)
    virtual void vf_0x34() = 0; // slot 0x34
    virtual void vf_0x38() = 0; // slot 0x38
    virtual void vf_0x3C() = 0; // slot 0x3C
    // the free size of the heap of the resource manager (each house has an inline copy)
    virtual u32 GetResourceFreeSize() = 0; // slot 0x40
};
} // namespace hobj
