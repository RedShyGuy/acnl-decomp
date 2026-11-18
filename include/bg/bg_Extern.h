#pragma once

#include "decomp.h"
#include "Sv/dSvFgName.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector2.h"
#include "nn/math/math_Vector3.h"

struct SvRoom;
namespace camera {
class Mgr;
} // namespace camera
namespace g3d {
class Model;
} // namespace g3d
namespace item {
class ResLoader;
} // namespace item

// A wallpaper or a flooring of a room with its value (SvRoomState::unk_0x20 / unk_0x21; meaning not shown by code yet;
// names are ours).
struct RoomCovering
{
    SvFgName item; // 0x0
    u8 param;      // 0x4
};
ASSERT_SIZE(RoomCovering, 0x6);

// (game code, not decompiled yet; names after the addresses) what the room view and its walls, landings and floor call
// the wallpaper / flooring of the current room (NAME_0x234C / 0x23EB without one)
RoomCovering Func529BE8(); // 0x00529BE8
RoomCovering Func5299E0(); // 0x005299E0
// sets the texture of the loaded file of a covering on the material of a model (with its texture translation)
void Func52985C(g3d::Model* model, item::ResLoader* loader, const RoomCovering* covering, const char* material,
                const nn::math::VEC2* translate); // 0x0052985C
// applies the value of a covering to the material of a model
void Func529EF8(g3d::Model* model, u8 param, const char* material, const RoomCovering* covering); // 0x00529EF8
// applies the value of the flooring to the floor model
void Func529DBC(g3d::Model* model, u8 param); // 0x00529DBC
// the fade of a wall seen from the camera (0..1; the first argument is not used)
f32 Func766E80(const camera::Mgr* camera, const nn::math::VEC3* position, s16 angle); // 0x00766E80
// the rotation of a matrix as angles x/y/z
void Func750BD8(const nn::math::MTX34* matrix, s16* angles); // 0x00750BD8
// the angle of a direction (direction * 0x4000)
s16 Func6F9090(const u32* direction); // 0x006F9090
// a height offset of the room models (always -1.0)
f32 Func6FA938(); // 0x006FA938
void Func1E6844(u32 value, u32 stage); // 0x001E6844
u8 Func1E839C(); // 0x001E839C
// the room of a house stage (the rooms of the player houses and of the museum exhibitions; NULL for other stages)
SvRoom* Func6FA758(u32 stage); // 0x006FA758
// the index of the house of a field name (table 0x0095377C)
u8 Func6FB278(u32 fieldName); // 0x006FB278
