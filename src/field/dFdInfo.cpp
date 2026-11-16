#include "field/dFdInfo.h"

#include "field/dField.h"
#include "field/dFieldAttribute.h"
#include "field/dJmpBlock.h"
#include "stage/stage_Api.h"
#include "Bs/dBsWaveMgr.h"
#include "field/dFieldMath.h"
#include "Other/dSaveProcess.h"
#include "ssys/ma/ma_Math.h"
#include <algorithm>

namespace {
// world coordinate -> unit (rounds down)
DECOMP_ALWAYS_INLINE s32 ToUnitX(f32 x)
{
    s32 unit = static_cast<s32>(x) / FdInfo::UNIT_WORLD_SIZE_X;
    if (x < 0.0f) {
        unit--;
    }
    return unit;
}
DECOMP_ALWAYS_INLINE s32 ToUnitZ(f32 z)
{
    s32 unit = static_cast<s32>(z) / FdInfo::UNIT_WORLD_SIZE_Z;
    if (z < 0.0f) {
        unit--;
    }
    return unit;
}
// unit -> block (rounds down)
DECOMP_ALWAYS_INLINE s32 UnitToBlock(s32 x)
{
    return x >= 0 ? x / static_cast<s32>(UNIT_BASE_NUM) : (x + 1) / static_cast<s32>(UNIT_BASE_NUM) - 1;
}
// unit -> unit in its block
DECOMP_ALWAYS_INLINE s32 UnitToLocal(s32 x)
{
    return x >= 0 ? x % static_cast<s32>(UNIT_BASE_NUM)
                  : (x + 1) % static_cast<s32>(UNIT_BASE_NUM) + static_cast<s32>(UNIT_BASE_NUM) - 1;
}
// one of the two units to the north of a unit has the attribute UNKNOWN_03
DECOMP_ALWAYS_INLINE bool IsNorthAttribute03(s32 x, s32 z, FieldName field)
{
    FieldAttribute north1 = FdInfo::GetAttribute(x, z - 1, field);
    FieldAttribute north2 = FdInfo::GetAttribute(x, z - 2, field);
    return north1 == FIELD_ATTRIBUTE_UNKNOWN_03 || north2 == FIELD_ATTRIBUTE_UNKNOWN_03;
}

DECOMP_ALWAYS_INLINE bool IsAttribute02_03_04_A2_A5_F8(FieldAttribute attribute)
{
    return attribute == FIELD_ATTRIBUTE_FLOOR_02 || attribute == FIELD_ATTRIBUTE_FLOOR_F8 ||
           attribute == FIELD_ATTRIBUTE_UNKNOWN_04 || attribute == FIELD_ATTRIBUTE_UNKNOWN_03 ||
           attribute == FIELD_ATTRIBUTE_UNKNOWN_A2 || attribute == FIELD_ATTRIBUTE_UNKNOWN_A3 ||
           attribute == FIELD_ATTRIBUTE_UNKNOWN_A4 || attribute == FIELD_ATTRIBUTE_UNKNOWN_A5;
}

DECOMP_ALWAYS_INLINE bool IsAttributeAD_B1(FieldAttribute attribute)
{
    return attribute == FIELD_ATTRIBUTE_UNKNOWN_AD || attribute == FIELD_ATTRIBUTE_UNKNOWN_AE ||
           attribute == FIELD_ATTRIBUTE_UNKNOWN_AF || attribute == FIELD_ATTRIBUTE_UNKNOWN_B0 ||
           attribute == FIELD_ATTRIBUTE_UNKNOWN_B1;
}

template <u32 N>
DECOMP_ALWAYS_INLINE bool Contains(const FieldAttribute (&set)[N], FieldAttribute attribute)
{
    for (u32 i = 0; i < N; i++) {
        if (set[i] == attribute) {
            return true;
        }
    }
    return false;
}

} // namespace

// 0x00316FAC | libgarden [tier A]
DECOMP_NOIPA void FdInfo::ToUnit(s32& x, s32& z, const nn::math::VEC3& wpos)
{
    x = ToUnitX(wpos.x);
    z = ToUnitZ(wpos.z);
}

// 0x0052F500 (name is ours)
Vector2i FdInfo::ToUnit(const nn::math::VEC3& wpos)
{
    s32 x = ToUnitX(wpos.x);
    s32 z = ToUnitZ(wpos.z);
    return Vector2i(x, z);
}

// 0x006A7968 (name is ours)
void FdInfo::ToUnit(s32& x, s32& z, s32 blockX, s32 blockZ, s32 unitX, s32 unitZ)
{
    x = unitX + blockX * UNIT_BASE_NUM;
    z = unitZ + blockZ * UNIT_BASE_NUM;
}

// 0x0052F57C (name is ours)
Vector2i FdInfo::ToUnit(const Vector2i& block, const Vector2i& unit)
{
    return Vector2i(unit.x + block.x * UNIT_BASE_NUM, unit.y + block.y * UNIT_BASE_NUM);
}

// 0x006A7750 (name is ours)
void FdInfo::ToBlock(s32& blockX, s32& blockZ, s32 x, s32 z)
{
    blockX = UnitToBlock(x);
    blockZ = UnitToBlock(z);
}

// 0x0052F4A8 (name is ours)
Vector2i FdInfo::ToBlock(const Vector2i& unit)
{
    return Vector2i(UnitToBlock(unit.x), UnitToBlock(unit.y));
}

// 0x0052F42C (name is ours)
Vector2i FdInfo::ToBlock(const nn::math::VEC3& wpos)
{
    s32 x = static_cast<s32>(wpos.x) / BLOCK_WORLD_SIZE_X;
    s32 z = static_cast<s32>(wpos.z) / BLOCK_WORLD_SIZE_Z;
    if (wpos.x < 0.0f) {
        x--;
    }
    if (wpos.z < 0.0f) {
        z--;
    }
    return Vector2i(x, z);
}

// 0x0052F3B0 (name is ours)
Vector2i FdInfo::ToLocalUnit(const Vector2i& unit)
{
    return Vector2i(UnitToLocal(unit.x), UnitToLocal(unit.y));
}

// 0x0052F2CC (name is ours)
Vector2i FdInfo::ToLocalUnit(const nn::math::VEC3& wpos)
{
    s32 x = ToUnitX(wpos.x);
    s32 z = ToUnitZ(wpos.z);
    return Vector2i(UnitToLocal(x), UnitToLocal(z));
}

// 0x006A7998 | libgarden [tier A]
void FdInfo::ToBlockAndLocalUnit(s32& blockX, s32& blockZ, s32& unitX, s32& unitZ, s32 x, s32 z)
{
    blockX = UnitToBlock(x);
    blockZ = UnitToBlock(z);
    unitX = UnitToLocal(x);
    unitZ = UnitToLocal(z);
}

// 0x006A74C4 (name is ours)
void FdInfo::ToBlockAndLocalUnit(s32& blockX, s32& blockZ, s32& unitX, s32& unitZ, const nn::math::VEC3& wpos)
{
    s32 x = static_cast<s32>(wpos.x) / BLOCK_WORLD_SIZE_X;
    s32 z = static_cast<s32>(wpos.z) / BLOCK_WORLD_SIZE_Z;
    if (wpos.x < 0.0f) {
        x--;
    }
    blockX = x;
    if (wpos.z < 0.0f) {
        z--;
    }
    blockZ = z;
    x = ToUnitX(wpos.x);
    z = ToUnitZ(wpos.z);
    unitX = UnitToLocal(x);
    unitZ = UnitToLocal(z);
}

// 0x006A68AC (name is ours)
void FdInfo::ToWorld(nn::math::VEC3* out, s32 x, s32 z)
{
    out->y = 0.0f;
    out->x = static_cast<f32>(x * UNIT_BASE_SIZE);
    out->z = static_cast<f32>(z * UNIT_BASE_SIZE);
}

// 0x0052F5A8 (name is ours)
void FdInfo::ToWorld(nn::math::VEC3* out, const Vector2i& unit)
{
    *out = nn::math::VEC3(static_cast<f32>(unit.x * UNIT_WORLD_SIZE_X), 0.0f,
                         static_cast<f32>(unit.y * UNIT_WORLD_SIZE_Z));
}

// 0x006A68DC (name is ours)
void FdInfo::ToWorldOfBlock(nn::math::VEC3* out, s32 blockX, s32 blockZ)
{
    out->y = 0.0f;
    out->x = static_cast<f32>(blockX * BLOCK_BASE_SIZE);
    out->z = static_cast<f32>(blockZ * BLOCK_BASE_SIZE);
}

// 0x0052F5F0 (name is ours)
void FdInfo::ToWorldOfBlock(nn::math::VEC3* out, const Vector2i& block)
{
    *out = nn::math::VEC3(static_cast<f32>(block.x * BLOCK_WORLD_SIZE_X), 0.0f,
                         static_cast<f32>(block.y * BLOCK_WORLD_SIZE_Z));
}

// 0x006A69E4 (name is ours)
void FdInfo::ToCenterWorld(nn::math::VEC3* out, s32 x, s32 z)
{
    *out = nn::math::VEC3(static_cast<f32>(x * UNIT_BASE_SIZE + UNIT_WORLD_SIZE_HALF_X), 0.0f,
                         static_cast<f32>(z * UNIT_BASE_SIZE + UNIT_WORLD_SIZE_HALF_Z));
}

// 0x0052F638 (name is ours)
void FdInfo::ToCenterWorld(nn::math::VEC3* out, const Vector2i& unit)
{
    *out = nn::math::VEC3(static_cast<f32>(unit.x * UNIT_WORLD_SIZE_X + UNIT_WORLD_SIZE_HALF_X), 0.0f,
                         static_cast<f32>(unit.y * UNIT_WORLD_SIZE_Z + UNIT_WORLD_SIZE_HALF_Z));
}

// 0x0052F6A4 (name is ours)
void FdInfo::ToCenterWorld(nn::math::VEC3* out, const Vector2i& block, const Vector2i& unit)
{
    s32 x = unit.x + block.x * UNIT_BASE_NUM;
    s32 z = unit.y + block.y * UNIT_BASE_NUM;
    *out = nn::math::VEC3(static_cast<f32>(x * UNIT_WORLD_SIZE_X + UNIT_WORLD_SIZE_HALF_X), 0.0f,
                         static_cast<f32>(z * UNIT_WORLD_SIZE_Z + UNIT_WORLD_SIZE_HALF_Z));
}

// 0x00307158 (name is ours)
void FdInfo::ToCenterWorld(nn::math::VEC3* out, s32 blockX, s32 blockZ, s32 unitX, s32 unitZ)
{
    s32 x = unitX + blockX * UNIT_BASE_NUM;
    s32 z = unitZ + blockZ * UNIT_BASE_NUM;
    *out = nn::math::VEC3(static_cast<f32>(x * UNIT_BASE_SIZE + UNIT_WORLD_SIZE_HALF_X), 0.0f,
                         static_cast<f32>(z * UNIT_BASE_SIZE + UNIT_WORLD_SIZE_HALF_Z));
}

// 0x006A691C (name is ours)
void FdInfo::ToCenterWorld(nn::math::VEC3* out, const nn::math::VEC3& wpos)
{
    s32 x = ToUnitX(wpos.x);
    s32 z = ToUnitZ(wpos.z);
    *out = nn::math::VEC3(static_cast<f32>(x * UNIT_BASE_SIZE + UNIT_WORLD_SIZE_HALF_X), 0.0f,
                         static_cast<f32>(z * UNIT_BASE_SIZE + UNIT_WORLD_SIZE_HALF_Z));
}

// 0x006A77BC (name is ours)
void FdInfo::ToCenterWorldWithUnitHeight(nn::math::VEC3* out, const nn::math::VEC3& wpos)
{
    s32 x = ToUnitX(wpos.x);
    s32 z = ToUnitZ(wpos.z);
    *out = nn::math::VEC3(static_cast<f32>(x * UNIT_BASE_SIZE + UNIT_WORLD_SIZE_HALF_X), 0.0f,
                         static_cast<f32>(z * UNIT_BASE_SIZE + UNIT_WORLD_SIZE_HALF_Z));
    out->y = GetUnitHeight(wpos, FIELD_NAME_CURRENT);
}

// 0x006A7894 (name is ours)
void FdInfo::ToCenterWorldWithUnitHeight(nn::math::VEC3* out, s32 x, s32 z)
{
    *out = nn::math::VEC3(static_cast<f32>(x * UNIT_BASE_SIZE + UNIT_WORLD_SIZE_HALF_X), 0.0f,
                         static_cast<f32>(z * UNIT_BASE_SIZE + UNIT_WORLD_SIZE_HALF_Z));
    out->y = GetUnitHeight(x, z, FIELD_NAME_CURRENT);
}

// 0x006A78F4 (name is ours)
void FdInfo::ToCenterWorldWithUnitHeight(nn::math::VEC3* out, s32 blockX, s32 blockZ, s32 unitX, s32 unitZ)
{
    s32 x = unitX + blockX * UNIT_BASE_NUM;
    s32 z = unitZ + blockZ * UNIT_BASE_NUM;
    *out = nn::math::VEC3(static_cast<f32>(x * UNIT_BASE_SIZE + UNIT_WORLD_SIZE_HALF_X), 0.0f,
                         static_cast<f32>(z * UNIT_BASE_SIZE + UNIT_WORLD_SIZE_HALF_Z));
    out->y = GetUnitHeight(blockX, blockZ, unitX, unitZ, FIELD_NAME_CURRENT);
}

// 0x006A7D3C (name is ours)
void FdInfo::ToCenterWorldWithUnitOrBgHeight(nn::math::VEC3* out, const nn::math::VEC3& wpos)
{
    s32 x = ToUnitX(wpos.x);
    s32 z = ToUnitZ(wpos.z);
    *out = nn::math::VEC3(static_cast<f32>(x * UNIT_BASE_SIZE + UNIT_WORLD_SIZE_HALF_X), 0.0f,
                         static_cast<f32>(z * UNIT_BASE_SIZE + UNIT_WORLD_SIZE_HALF_Z));
    out->y = GetUnitOrBgHeight(wpos, FIELD_NAME_CURRENT);
}

// 0x006A7E14 (name is ours)
void FdInfo::ToCenterWorldWithUnitOrBgHeight(nn::math::VEC3* out, s32 x, s32 z)
{
    *out = nn::math::VEC3(static_cast<f32>(x * UNIT_BASE_SIZE + UNIT_WORLD_SIZE_HALF_X), 0.0f,
                         static_cast<f32>(z * UNIT_BASE_SIZE + UNIT_WORLD_SIZE_HALF_Z));
    out->y = GetUnitOrBgHeight(x, z, FIELD_NAME_CURRENT);
}

// 0x006A7E74 (name is ours)
void FdInfo::ToCenterWorldWithUnitOrBgHeight(nn::math::VEC3* out, s32 blockX, s32 blockZ, s32 unitX, s32 unitZ)
{
    s32 x = unitX + blockX * UNIT_BASE_NUM;
    s32 z = unitZ + blockZ * UNIT_BASE_NUM;
    *out = nn::math::VEC3(static_cast<f32>(x * UNIT_BASE_SIZE + UNIT_WORLD_SIZE_HALF_X), 0.0f,
                         static_cast<f32>(z * UNIT_BASE_SIZE + UNIT_WORLD_SIZE_HALF_Z));
    out->y = GetUnitOrBgHeight(blockX, blockZ, unitX, unitZ, FIELD_NAME_CURRENT);
}

// 0x006A690C | libgarden [tier A]
Field* FdInfo::GetCurrentField()
{
    return Field::s_pFields[FIELD_NAME_CURRENT];
}

// 0x006C67A8 (name is ours)
f32 FdInfo::GetHeight(const nn::math::VEC3& wpos)
{
    return GetHeight(wpos, FIELD_NAME_CURRENT);
}

// 0x006C67B0 | libgarden [tier A]
f32 FdInfo::GetHeight(const nn::math::VEC3& wpos, FieldName field)
{
    Field* fieldObj = stage::GetField(field);
    if (fieldObj != NULL) {
        s32 blockX = 0;
        s32 blockZ = 0;
        s32 unitX = 0;
        s32 unitZ = 0;
        ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, wpos);
        JmpBlock* block = fieldObj->GetJmpBlock(blockX, blockZ);
        if (block != NULL) {
            if (block->IsFlat(unitX, unitZ)) {
                return block->GetHeight(unitX, unitZ);
            }
            nn::math::VEC3 center;
            nn::math::VEC3 corner1;
            nn::math::VEC3 corner2;
            block->GetTriangle(&center, &corner1, &corner2, wpos);
            return GetTriangleHeight(wpos, center, corner1, corner2);
        }
    }
    return 0.0f;
}

// 0x006C6880 (name is ours)
void FdInfo::GetNormal(nn::math::VEC3* out, const nn::math::VEC3& wpos, FieldName field)
{
    Field* fieldObj = stage::GetField(field);
    if (fieldObj != NULL) {
        s32 blockX;
        s32 blockZ;
        s32 unitX;
        s32 unitZ;
        ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, wpos);
        JmpBlock* block = fieldObj->GetJmpBlock(blockX, blockZ);
        if (block != NULL) {
            nn::math::VEC3 a;
            nn::math::VEC3 b;
            nn::math::VEC3 c;
            block->GetTriangle(&a, &b, &c, wpos);
            nn::math::VEC3 ab(b.x - a.x, b.y - a.y, b.z - a.z);
            nn::math::VEC3 bc(c.x - b.x, c.y - b.y, c.z - b.z);
            nn::math::VEC3 normal(ab.y * bc.z - ab.z * bc.y, ab.z * bc.x - ab.x * bc.z, ab.x * bc.y - ab.y * bc.x);
            ssys::ma::NormalizeIfPossible(&normal);
            *out = normal;
            return;
        }
    }
    *out = nn::math::VEC3(0.0f, 1.0f, 0.0f);
}

// 0x006C6AF0 (name is ours)
f32 FdInfo::GetUnitHeight(const nn::math::VEC3& wpos, FieldName field)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, wpos);
    Field* fieldObj = stage::GetField(field);
    if (fieldObj != NULL) {
        JmpBlock* block = fieldObj->GetJmpBlock(blockX, blockZ);
        if (block != NULL) {
            return block->GetHeight(unitX, unitZ);
        }
    }
    return 0.0f;
}

// 0x006C6B7C (name is ours)
f32 FdInfo::GetUnitHeight(s32 x, s32 z, FieldName field)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, x, z);
    Field* fieldObj = stage::GetField(field);
    if (fieldObj != NULL) {
        JmpBlock* block = fieldObj->GetJmpBlock(blockX, blockZ);
        if (block != NULL) {
            return block->GetHeight(unitX, unitZ);
        }
    }
    return 0.0f;
}

// 0x006C6C08 (name is ours)
f32 FdInfo::GetUnitHeight(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field)
{
    Field* fieldObj = stage::GetField(field);
    if (fieldObj != NULL) {
        JmpBlock* block = fieldObj->GetJmpBlock(blockX, blockZ);
        if (block != NULL) {
            return block->GetHeight(unitX, unitZ);
        }
    }
    return 0.0f;
}

// 0x006C8090 (name is ours)
f32 FdInfo::GetBgHeight(const nn::math::VEC3& wpos, FieldName field)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, wpos);
    return GetBgHeight(blockX, blockZ, unitX, unitZ, field);
}

// 0x006C80E0 (name is ours)
f32 FdInfo::GetBgHeight(s32 x, s32 z, FieldName field)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, x, z);
    return GetBgHeight(blockX, blockZ, unitX, unitZ, field);
}

// 0x006C8130 (name is ours)
f32 FdInfo::GetBgHeight(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field)
{
    Field* fieldObj = stage::GetField(field);
    if (fieldObj != NULL) {
        BgInfo* bg = fieldObj->GetBg(blockX, blockZ);
        if (bg != NULL) {
            SvBgName name = bg->m_Name;
            if (IsIndoorBgName(name)) {
                Field* fieldObj2 = stage::GetField(field);
                if (fieldObj2 != NULL) {
                    JmpBlock* block = fieldObj2->GetJmpBlock(blockX, blockZ);
                    if (block != NULL) {
                        return block->GetHeight(unitX, unitZ);
                    }
                }
            } else {
                const JmpUnit* units = Func1C2588(Func3044DC(), name);
                if (units != NULL) {
                    const JmpUnit* unit = &units[unitZ * UNIT_BASE_NUM + unitX];
                    if (unit->IsFlat()) {
                        return static_cast<f32>(unit->GetHeightNorth());
                    }
                    s32 heights[5];
                    heights[0] = 0;
                    heights[1] = unit->GetHeightNorth();
                    heights[2] = unit->GetHeightSouth();
                    heights[3] = unit->GetHeightWest();
                    heights[4] = unit->GetHeightEast();
                    heights[0] = std::max(heights[0], heights[1]);
                    heights[0] = std::max(heights[0], heights[2]);
                    heights[0] = std::max(heights[0], heights[3]);
                    heights[0] = std::max(heights[0], heights[4]);
                    return static_cast<f32>(heights[0]);
                }
            }
        }
    }
    return 0.0f;
}

// 0x006C7190 (name is ours)
f32 FdInfo::GetHeightOrBgHeight(const nn::math::VEC3& wpos, FieldName field)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, wpos);
    if (!::IsAttribute02_03_04_A2_A5_F8(GetAttribute(blockX, blockZ, unitX, unitZ, field))) {
        return GetHeight(wpos, field);
    }
    s32 blockX2 = 0;
    s32 blockZ2 = 0;
    s32 unitX2 = 0;
    s32 unitZ2 = 0;
    ToBlockAndLocalUnit(blockX2, blockZ2, unitX2, unitZ2, wpos);
    return GetBgHeight(blockX2, blockZ2, unitX2, unitZ2, field);
}

// 0x006C83B8 (name is ours)
f32 FdInfo::GetUnitOrBgHeight(const nn::math::VEC3& wpos, FieldName field)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, wpos);
    return GetUnitOrBgHeight(blockX, blockZ, unitX, unitZ, field);
}

// 0x006C8408 (name is ours)
f32 FdInfo::GetUnitOrBgHeight(s32 x, s32 z, FieldName field)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, x, z);
    return GetUnitOrBgHeight(blockX, blockZ, unitX, unitZ, field);
}

// 0x006C8458 (name is ours)
f32 FdInfo::GetUnitOrBgHeight(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field)
{
    if (!::IsAttributeAD_B1(GetAttribute(blockX, blockZ, unitX, unitZ, field)) &&
        !HasPartIn891697(blockX, blockZ, unitX, unitZ, field)) {
        return GetBgHeight(blockX, blockZ, unitX, unitZ, field);
    }
    Field* fieldObj = stage::GetField(field);
    if (fieldObj != NULL) {
        JmpBlock* block = fieldObj->GetJmpBlock(blockX, blockZ);
        if (block != NULL) {
            return block->GetHeight(unitX, unitZ);
        }
    }
    return 0.0f;
}

DECOMP_ALWAYS_INLINE f32 FdInfo::GetPartHeightOffset(FieldAttribute part)
{
    // 0x0095EEBC (guard 0x0095EE3C)
    static f32 s_Offsets[4] = {0.0f, 16.0f, GetWaterSurfaceHeight(), 16.0f};
    u32 index = GetAttribute95E93E(part);
    if (index >= 4) {
        index = 0;
    }
    return s_Offsets[index];
}

// 0x006C6A48 (name is ours)
f32 FdInfo::GetSurfaceHeight(const nn::math::VEC3& wpos, FieldName field)
{
    f32 height = GetHeight(wpos, field);
    if (GetWaterSurfaceHeight() >= height) {
        return GetWaterSurfaceHeight();
    }
    FieldAttribute part = GetPartAttribute(wpos, field);
    return GetPartHeightOffset(part) + height;
}

// 0x006C8374 (name is ours)
f32 FdInfo::GetHeight95E93E(const nn::math::VEC3& wpos, FieldName field)
{
    f32 offset = 0.0f;
    if (GetAttribute95E93E(GetPartAttribute(wpos, field)) == 2) {
        offset = -16.0f;
    }
    return GetHeight(wpos, field) + offset;
}

// 0x006C8564 (name is ours)
u8 FdInfo::GetKind(s32 x, s32 z, FieldName field)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, x, z);
    Field* fieldObj = stage::GetField(field);
    if (fieldObj != NULL) {
        JmpBlock* block = fieldObj->GetJmpBlock(blockX, blockZ);
        if (block != NULL) {
            return block->GetKind(unitX, unitZ);
        }
    }
    return 0;
}

// 0x006C85EC (name is ours)
u8 FdInfo::GetKind(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field)
{
    Field* fieldObj = stage::GetField(field);
    if (fieldObj != NULL) {
        JmpBlock* block = fieldObj->GetJmpBlock(blockX, blockZ);
        if (block != NULL) {
            return block->GetKind(unitX, unitZ);
        }
    }
    return 0;
}

// 0x006C610C (name is ours)
bool FdInfo::HasFlag02(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field)
{
    s32 x = 0;
    s32 z = 0;
    ToUnit(x, z, blockX, blockZ, unitX, unitZ);
    if (IsNorthAttribute03(x, z, field)) {
        return false;
    }
    return HasAttributeFlag02(GetAttribute(blockX, blockZ, unitX, unitZ, field));
}

// 0x006C61B4 (name is ours)
bool FdInfo::IsAttribute04(s32 x, s32 z, FieldName field)
{
    u32 attribute = s_Attribute04;
    return static_cast<FieldAttribute>(attribute) == GetAttribute(x, z, field);
}

// 0x006C61DC (name is ours)
bool FdInfo::IsAttributeADOrPart95E93E2(const nn::math::VEC3& wpos, FieldName field)
{
    if (GetAttribute(wpos, field) == FIELD_ATTRIBUTE_UNKNOWN_AD) {
        return true;
    }
    return GetAttribute95E93E(GetPartAttribute(wpos, field)) == 2;
}

// 0x006C6218 (name is ours)
bool FdInfo::IsStoneF9(s32 x, s32 z, FieldName field)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, x, z);
    return GetAttribute(blockX, blockZ, unitX, unitZ, field) == FIELD_ATTRIBUTE_STONE_F9;
}

// 0x006C6274 (name is ours)
bool FdInfo::IsStoneF9(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field)
{
    return GetAttribute(blockX, blockZ, unitX, unitZ, field) == FIELD_ATTRIBUTE_STONE_F9;
}

// 0x006C6294 (name is ours)
bool FdInfo::HasFlag08(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field)
{
    return HasAttributeFlag08(GetAttribute(blockX, blockZ, unitX, unitZ, field));
}

// 0x006C62AC (name is ours)
u8 FdInfo::Get95EB3C(const nn::math::VEC3& wpos, FieldName field)
{
    s32 x = 0;
    s32 z = 0;
    ToUnit(x, z, wpos);
    if (IsNorthAttribute03(x, z, field)) {
        return 2;
    }
    return GetAttribute95EB3C(GetAttribute(wpos, field));
}

// 0x006C6334 (name is ours)
u8 FdInfo::Get95EB3C(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field)
{
    s32 x = 0;
    s32 z = 0;
    ToUnit(x, z, blockX, blockZ, unitX, unitZ);
    if (IsNorthAttribute03(x, z, field)) {
        return 2;
    }
    return GetAttribute95EB3C(GetAttribute(blockX, blockZ, unitX, unitZ, field));
}

// 0x006C658C (name is ours)
bool FdInfo::IsAttribute02_03_04_A2_A5_F8(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field)
{
    return ::IsAttribute02_03_04_A2_A5_F8(GetAttribute(blockX, blockZ, unitX, unitZ, field));
}

// 0x006C667C (name is ours)
u8 FdInfo::Get95E93E(s32 x, s32 z, FieldName field)
{
    return GetAttribute95E93E(GetAttribute(x, z, field));
}

// 0x006C668C (name is ours)
u8 FdInfo::Get95E93E(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field)
{
    return GetAttribute95E93E(GetAttribute(blockX, blockZ, unitX, unitZ, field));
}

// 0x006C66A4 (name is ours)
bool FdInfo::Is95EB3CZero(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field)
{
    s32 x = 0;
    s32 z = 0;
    ToUnit(x, z, blockX, blockZ, unitX, unitZ);
    if (IsNorthAttribute03(x, z, field)) {
        return false;
    }
    return Get95EB3C(blockX, blockZ, unitX, unitZ, field) == 0;
}

// 0x006C697C (name is ours)
u8 FdInfo::Get95EA3D(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field)
{
    s32 x = 0;
    s32 z = 0;
    ToUnit(x, z, blockX, blockZ, unitX, unitZ);
    if (IsNorthAttribute03(x, z, field)) {
        return 0;
    }
    return GetAttribute95EA3D(GetAttribute(blockX, blockZ, unitX, unitZ, field));
}

// 0x006C6A24 (name is ours)
bool FdInfo::IsAttribute10(s32 x, s32 z, FieldName field)
{
    return GetAttribute(x, z, field) == FIELD_ATTRIBUTE_UNKNOWN_10;
}

// 0x006C6C5C (name is ours)
bool FdInfo::IsAttributeAD_B1(const nn::math::VEC3& wpos, FieldName field)
{
    return ::IsAttributeAD_B1(GetAttribute(wpos, field));
}

// 0x006C6CD8 (name is ours)
bool FdInfo::HasPartIn89166C(s32 x, s32 z, FieldName field)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, x, z);
    return HasPartIn89166C(blockX, blockZ, unitX, unitZ, field);
}

// 0x006C6D28 (name is ours)
bool FdInfo::HasPartIn89166C(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field)
{
    FieldAttributeParts parts = GetAttributeParts(GetAttribute(blockX, blockZ, unitX, unitZ, field));
    return Contains(s_Attributes89166C, parts.parts[JmpUnit::SIDE_SOUTH]) ||
           Contains(s_Attributes89166C, parts.parts[JmpUnit::SIDE_NORTH]) ||
           Contains(s_Attributes89166C, parts.parts[JmpUnit::SIDE_WEST]) ||
           Contains(s_Attributes89166C, parts.parts[JmpUnit::SIDE_EAST]);
}

// 0x006C7030 (name is ours)
bool FdInfo::HasPart95E93E(const nn::math::VEC3& wpos, FieldName field)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, wpos);
    Field* fieldObj = stage::GetField(field);
    if (fieldObj != NULL) {
        JmpBlock* block = fieldObj->GetJmpBlock(blockX, blockZ);
        if (block != NULL) {
            FieldAttributeParts parts = GetAttributeParts(block->GetAttribute(unitX, unitZ));
            u8 north = GetAttribute95E93E(parts.parts[JmpUnit::SIDE_NORTH]);
            u8 south = GetAttribute95E93E(parts.parts[JmpUnit::SIDE_SOUTH]);
            return (south | north) != 0;
        }
    }
    return false;
}

// 0x006C70E0 (name is ours)
bool FdInfo::HasPart95E93E(s32 x, s32 z, FieldName field)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, x, z);
    Field* fieldObj = stage::GetField(field);
    if (fieldObj != NULL) {
        JmpBlock* block = fieldObj->GetJmpBlock(blockX, blockZ);
        if (block != NULL) {
            FieldAttributeParts parts = GetAttributeParts(block->GetAttribute(unitX, unitZ));
            u8 north = GetAttribute95E93E(parts.parts[JmpUnit::SIDE_NORTH]);
            u8 south = GetAttribute95E93E(parts.parts[JmpUnit::SIDE_SOUTH]);
            return (south | north) != 0;
        }
    }
    return false;
}

// 0x006C7534 (name is ours)
bool FdInfo::HasPartIn891697(const nn::math::VEC3& wpos, FieldName field)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, wpos);
    return HasPartIn891697(blockX, blockZ, unitX, unitZ, field);
}

// 0x006C7584 (name is ours)
bool FdInfo::HasPartIn891697(s32 x, s32 z, FieldName field)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, x, z);
    return HasPartIn891697(blockX, blockZ, unitX, unitZ, field);
}

// 0x006C75D4 (name is ours)
bool FdInfo::HasPartIn891697(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field)
{
    FieldAttributeParts parts = GetAttributeParts(GetAttribute(blockX, blockZ, unitX, unitZ, field));
    return Contains(s_Attributes891697, parts.parts[JmpUnit::SIDE_SOUTH]) ||
           Contains(s_Attributes891697, parts.parts[JmpUnit::SIDE_NORTH]) ||
           Contains(s_Attributes891697, parts.parts[JmpUnit::SIDE_WEST]) ||
           Contains(s_Attributes891697, parts.parts[JmpUnit::SIDE_EAST]);
}

// 0x006C7AA4 (name is ours)
bool FdInfo::Is95EC3BWithout7C_FB(s32 x, s32 z, FieldName field)
{
    // (a local copy of 0x00891660)
    const FieldAttribute excluded[] = {
        FIELD_ATTRIBUTE_UNKNOWN_7C, FIELD_ATTRIBUTE_UNKNOWN_7D, FIELD_ATTRIBUTE_UNKNOWN_7E,
        FIELD_ATTRIBUTE_UNKNOWN_7F, FIELD_ATTRIBUTE_CARPET_FA,  FIELD_ATTRIBUTE_CARPET_FB,
    };
    if (GetAttribute95EC3B(GetAttribute(x, z, field)) == 0) {
        return false;
    }
    if (Contains(excluded, GetAttribute(x, z, field))) {
        return false;
    }
    return true;
}

// 0x006C82DC (name is ours)
bool FdInfo::IsAttributeIn891666(s32 x, s32 z, FieldName field)
{
    return Contains(s_Attributes891666, GetAttribute(x, z, field));
}

// 0x006C8328 (name is ours)
bool FdInfo::IsAttributeIn891672(s32 x, s32 z, FieldName field)
{
    return Contains(s_Attributes891672, GetAttribute(x, z, field));
}

// 0x006C8518 (name is ours)
bool FdInfo::IsAttributeIn89167A(s32 x, s32 z, FieldName field)
{
    return Contains(s_Attributes89167A, GetAttribute(x, z, field));
}

// 0x006C6E28 | libgarden [tier A]
f32 FdInfo::GetHeightDefault(const nn::math::VEC3& wpos, FieldName field)
{
    Field* fieldObj = stage::GetField(field);
    if (fieldObj != NULL) {
        s32 blockX = 0;
        s32 blockZ = 0;
        s32 unitX = 0;
        s32 unitZ = 0;
        ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, wpos);
        if (!::IsAttribute02_03_04_A2_A5_F8(GetAttribute(blockX, blockZ, unitX, unitZ, field))) {
            BgInfo* bg = fieldObj->GetBg(blockX, blockZ);
            if (bg != NULL) {
                SvBgName name = bg->m_Name;
                const JmpUnit* units = Func1C2588(Func3044DC(), name);
                if (units != NULL) {
                    const JmpUnit* unit = &units[unitZ * UNIT_BASE_NUM + unitX];
                    if (unit->IsFlat()) {
                        return static_cast<f32>(unit->GetHeightNorth());
                    }
                    // the default units of the ground as a block
                    JmpBlock block;
                    block.Initialize(blockX, blockZ);
                    block.SetUnits(units, sizeof(JmpUnit) * UNIT_BASE_NUM * UNIT_BASE_NUM);
                    nn::math::VEC3 center;
                    nn::math::VEC3 corner1;
                    nn::math::VEC3 corner2;
                    block.GetTriangle(&center, &corner1, &corner2, wpos);
                    return GetTriangleHeight(wpos, center, corner1, corner2);
                }
            }
        } else {
            s32 blockX2 = 0;
            s32 blockZ2 = 0;
            s32 unitX2 = 0;
            s32 unitZ2 = 0;
            ToBlockAndLocalUnit(blockX2, blockZ2, unitX2, unitZ2, wpos);
            return GetBgHeight(blockX2, blockZ2, unitX2, unitZ2, field);
        }
    }
    return 0.0f;
}

// 0x006C8D48 (name is ours)
f32 FdInfo::GetSurfaceHeightEx(const nn::math::VEC3& wpos, FieldName field)
{
    FieldAttribute part = GetPartAttribute(wpos, field);
    if (part != FIELD_ATTRIBUTE_STONE_17 && part != FIELD_ATTRIBUTE_WOOD_18) {
        s32 blockX = 0;
        s32 blockZ = 0;
        s32 unitX = 0;
        s32 unitZ = 0;
        ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, wpos);
        if (!::IsAttributeAD_B1(GetAttribute(blockX, blockZ, unitX, unitZ, field))) {
            f32 height = GetHeight(wpos, field);
            if (height > GetWaterSurfaceHeight()) {
                FieldAttribute part2 = GetPartAttribute(wpos, field);
                return GetPartHeightOffset(part2) + height;
            }
            return GetWaterSurfaceHeight();
        }
        f32 height = GetHeightDefault(wpos, field);
        return height + GetWaterSurfaceHeight();
    }
    return GetHeightDefault(wpos, field) + 16.0f;
}

// 0x006C863C (name is ours)
f32 FdInfo::GetSurfaceHeightDefault(const nn::math::VEC3& wpos, FieldName field)
{
    u8 kind = GetAttribute95E93E(GetPartAttribute(wpos, field));
    if (kind == 2) {
        return GetWaterSurfaceHeight();
    }
    f32 height = GetHeightDefault(wpos, field);
    if (kind == 1 || kind == 3) {
        return height + 32.0f;
    }
    // (the unit is not used)
    s32 x = 0;
    s32 z = 0;
    ToUnit(x, z, wpos);
    if (Contains(s_Attributes891697, GetPartAttribute(wpos, field))) {
        return 112.0f;
    }
    return height;
}

// 0x006C8720 (name is ours)
bool FdInfo::HasEqualHeights(const nn::math::VEC3& wpos, FieldName field)
{
    Field* fieldObj = stage::GetField(field);
    if (fieldObj != NULL) {
        Vector2i blockPos = ToBlock(wpos);
        JmpBlock* block = fieldObj->GetJmpBlock(blockPos.x, blockPos.y);
        if (block != NULL) {
            Vector2i unitPos = ToLocalUnit(wpos);
            JmpUnit unit;
            block->CopyUnit(&unit, unitPos.x, unitPos.y);
            s32 north = unit.GetHeightNorth();
            s32 west = unit.GetHeightWest();
            s32 south = unit.GetHeightSouth();
            s32 east = unit.GetHeightEast();
            if (north == west && north == south && north == east) {
                return true;
            }
        }
    }
    return false;
}

// 0x006C8EB8 (name is ours)
bool FdInfo::IsFlat(s32 x, s32 z, FieldName field)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, x, z);
    Field* fieldObj = stage::GetField(field);
    if (fieldObj != NULL) {
        JmpBlock* block = fieldObj->GetJmpBlock(blockX, blockZ);
        if (block != NULL) {
            return block->IsFlat(unitX, unitZ);
        }
    }
    return false;
}

// 0x006C8F40 (name is ours)
bool FdInfo::IsUnknownFlag80(s32 x, s32 z, FieldName field)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, x, z);
    Field* fieldObj = stage::GetField(field);
    if (fieldObj != NULL) {
        JmpBlock* block = fieldObj->GetJmpBlock(blockX, blockZ);
        if (block != NULL) {
            return block->IsUnknownFlag80(unitX, unitZ);
        }
    }
    return false;
}

// 0x006C9248 (name is ours)
bool FdInfo::IsAttribute11(s32 x, s32 z, FieldName field)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, x, z);
    return GetAttribute(blockX, blockZ, unitX, unitZ, field) == FIELD_ATTRIBUTE_STONE_11;
}

// 0x006C8FC8 (name is ours)
bool FdInfo::IsAttributeIn89166C(s32 x, s32 z, FieldName field)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, x, z);
    return Contains(s_Attributes89166C, GetAttribute(blockX, blockZ, unitX, unitZ, field));
}

// 0x006C905C (name is ours)
bool FdInfo::IsAttributeIn89166C(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field)
{
    return Contains(s_Attributes89166C, GetAttribute(blockX, blockZ, unitX, unitZ, field));
}

// 0x006C90B0 (name is ours)
bool FdInfo::IsNorthPart0F(s32 x, s32 z, FieldName field)
{
    FieldAttributeParts parts = GetAttributeParts(GetAttribute(x, z, field));
    return parts.parts[JmpUnit::SIDE_NORTH] == FIELD_ATTRIBUTE_UNKNOWN_0F;
}

// 0x006C90E0 (name is ours)
bool FdInfo::IsPart95E93E2(const nn::math::VEC3& wpos, FieldName field)
{
    return GetAttribute95E93E(GetPartAttribute(wpos, field)) == 2;
}

// 0x006C92B0 (name is ours)
bool FdInfo::IsPart95E93ENonZero(const nn::math::VEC3& wpos, FieldName field)
{
    return GetAttribute95E93E(GetPartAttribute(wpos, field)) != 0;
}

// 0x006C9460 (name is ours)
bool FdInfo::HasFlag01(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field)
{
    return HasAttributeFlag01(GetAttribute(blockX, blockZ, unitX, unitZ, field));
}

// 0x006C9478 (name is ours)
s8 FdInfo::Get95E740(s32 x, s32 z, FieldName field)
{
    return GetAttribute95E740(GetAttribute(x, z, field));
}

// 0x006C6628 (name is ours)
FieldAttribute FdInfo::GetAttribute(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field)
{
    Field* fieldObj = stage::GetField(field);
    if (fieldObj != NULL) {
        JmpBlock* block = fieldObj->GetJmpBlock(blockX, blockZ);
        if (block != NULL) {
            FieldAttribute attribute = block->GetAttribute(unitX, unitZ);
            if (attribute < FIELD_ATTRIBUTE_NUM) {
                return attribute;
            }
        }
    }
    return FIELD_ATTRIBUTE_UNKNOWN_00;
}

// 0x00315660 (name is ours)
FieldAttribute FdInfo::GetAttribute(s32 x, s32 z, FieldName field)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, x, z);
    return GetAttribute(blockX, blockZ, unitX, unitZ, field);
}

// 0x006C65D8 (name is ours)
FieldAttribute FdInfo::GetAttribute(const nn::math::VEC3& wpos, FieldName field)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, wpos);
    return GetAttribute(blockX, blockZ, unitX, unitZ, field);
}

// 0x006C63DC (name is ours)
FieldAttribute FdInfo::GetPartAttribute(const nn::math::VEC3& wpos, FieldName field)
{
    Field* fieldObj = stage::GetField(field);
    if (fieldObj != NULL) {
        s32 blockX = 0;
        s32 blockZ = 0;
        s32 unitX = 0;
        s32 unitZ = 0;
        ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, wpos);
        JmpBlock* block = fieldObj->GetJmpBlock(blockX, blockZ);
        if (block != NULL) {
            FieldAttribute attribute = block->GetAttribute(unitX, unitZ);
            FieldAttributeParts parts = GetAttributeParts(attribute);
            nn::math::VEC3 center;
            ToCenterWorld(&center, blockX, blockZ, unitX, unitZ);
            // the triangle of the position: the diagonals of the unit separate them
            nn::math::VEC3 offset(wpos.x - center.x, 0.0f, wpos.z - center.z);
            f32 sum = offset.x + offset.z;
            f32 difference = offset.z - offset.x;
            FieldAttribute part;
            if (sum > 0.0f) {
                part = difference > 0.0f ? parts.parts[JmpUnit::SIDE_SOUTH] : parts.parts[JmpUnit::SIDE_EAST];
            } else {
                part = difference > 0.0f ? parts.parts[JmpUnit::SIDE_WEST] : parts.parts[JmpUnit::SIDE_NORTH];
            }

            if (part == FIELD_ATTRIBUTE_WATER) {
                BsWaveMgr* waveMgr = BsWaveMgr::GetInstance();
                for (u32 i = 0; i < sizeof(s_ShoreAttributes); i++) {
                    if (s_ShoreAttributes[i] == attribute) {
                        if (waveMgr != NULL) {
                            return waveMgr->GetShoreAttribute(attribute, offset);
                        }
                        break;
                    }
                }
                return part;
            }
            if (part < FIELD_ATTRIBUTE_NUM) {
                return part;
            }
        } else if (stage::IsKind(stage::KIND_AUTO_CAMP, stage::GetCurrent())) {
            return FIELD_ATTRIBUTE_SOIL_14;
        }
    }
    return FIELD_ATTRIBUTE_UNKNOWN_00;
}
