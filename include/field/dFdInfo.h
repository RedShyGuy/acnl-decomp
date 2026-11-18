#pragma once

#include "decomp.h"
#include "field/dFieldAttribute.h"
#include "field/dFieldTypes.h"
#include "nn/math/math_Vector3.h"

class Field;

// the attributes of the units at the shore whose water part depends on the wave (BsWaveMgr)
extern const FieldAttribute s_ShoreAttributes[13];
// sets of attributes of the FdInfo tests (meaning not known yet)
extern const u32 s_Attribute04;
extern const FieldAttribute s_Attributes891666[6];
extern const FieldAttribute s_Attributes89166C[6];
extern const FieldAttribute s_Attributes891672[8];
extern const FieldAttribute s_Attributes89167A[16];
extern const FieldAttribute s_Attributes891697[90];

// Information about the field: the conversions between world positions, blocks and units, the heights (class name,
// GetCurrentField, GetHeight, GetHeightDefault, ToUnit and ToBlockAndLocalUnit after libgarden field/FdInfo.hpp; the
// other names are ours).
//
// Negative positions round down (world -1 is unit -1, unit -1 is block -1 and local unit 15).
class FdInfo
{
public:
    // the size of a unit and a block in world units (libgarden: unitBaseSize)
    static const s32 UNIT_BASE_SIZE = 32;
    static const s32 BLOCK_BASE_SIZE = UNIT_BASE_SIZE * UNIT_BASE_NUM;
    // the same sizes as variables (most code reads these from .rodata; libgarden: unitWorldSizeX etc.)
    static const s32 BLOCK_WORLD_SIZE_X;      // 512
    static const s32 BLOCK_WORLD_SIZE_Z;      // 512
    static const f32 UNIT_WORLD_SIZE_XF;      // 32.0
    static const f32 UNIT_WORLD_SIZE_ZF;      // 32.0
    static const f32 UNIT_WORLD_SIZE_HALF_XF; // 16.0
    static const f32 UNIT_WORLD_SIZE_HALF_ZF; // 16.0
    static const s32 UNIT_WORLD_SIZE_X;       // 32
    static const s32 UNIT_WORLD_SIZE_Z;       // 32
    static const s32 UNIT_WORLD_SIZE_HALF_X;  // 16
    static const s32 UNIT_WORLD_SIZE_HALF_Z;  // 16

    // the field of the current stage (NULL if there is none)
    static Field* GetCurrentField(); // 0x006A690C | libgarden [tier A]

    // the height of the ground at a world position: the unit height if it is flat, otherwise its triangle at the
    // position (0 outside of the field)
    static f32 GetHeight(const nn::math::VEC3& wpos, FieldName field); // 0x006C67B0 | libgarden [tier A]
    static f32 GetHeight(const nn::math::VEC3& wpos);                  // 0x006C67A8 (field 0)
    static f32 GetHeightDefault(const nn::math::VEC3& wpos, FieldName field); // 0x006C6E28 | libgarden [tier A]
    // the normal of the triangle at a world position ((0, 1, 0) outside of the field)
    static void GetNormal(nn::math::VEC3* out, const nn::math::VEC3& wpos, FieldName field); // 0x006C6880
    // the height of a unit (JmpBlock::GetHeight; 0 outside of the field)
    static f32 GetUnitHeight(const nn::math::VEC3& wpos, FieldName field);                    // 0x006C6AF0
    static f32 GetUnitHeight(s32 x, s32 z, FieldName field);                                  // 0x006C6B7C
    static f32 GetUnitHeight(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field);  // 0x006C6C08
    // the height of a unit after the default units of its ground (rooms: the units of the field)
    static f32 GetBgHeight(const nn::math::VEC3& wpos, FieldName field);                      // 0x006C8090
    static f32 GetBgHeight(s32 x, s32 z, FieldName field);                                    // 0x006C80E0
    static f32 GetBgHeight(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field);    // 0x006C8130
    // GetHeight, or GetBgHeight for the attributes of IsAttribute02_03_04_A2_A5_F8
    static f32 GetHeightOrBgHeight(const nn::math::VEC3& wpos, FieldName field); // 0x006C7190
    // GetUnitHeight for the attributes AD..B1 and the units with a part of s_Attributes891697, otherwise GetBgHeight
    static f32 GetUnitOrBgHeight(const nn::math::VEC3& wpos, FieldName field);                   // 0x006C83B8
    static f32 GetUnitOrBgHeight(s32 x, s32 z, FieldName field);                                 // 0x006C8408
    static f32 GetUnitOrBgHeight(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field); // 0x006C8458
    // GetHeight, at least the water surface; above it plus an offset after the table 95E93E of the part
    static f32 GetSurfaceHeight(const nn::math::VEC3& wpos, FieldName field); // 0x006C6A48
    // GetHeight, 16 lower where the table 95E93E of the part has 2
    static f32 GetHeight95E93E(const nn::math::VEC3& wpos, FieldName field); // 0x006C8374
    // like GetSurfaceHeight; the parts 17/18 and the attributes AD..B1 after GetHeightDefault
    static f32 GetSurfaceHeightEx(const nn::math::VEC3& wpos, FieldName field); // 0x006C8D48
    // (two heights of the auto camp stages; names after the address)
    static f32 Func6C8C4C(const nn::math::VEC3& wpos, FieldName field); // 0x006C8C4C
    static f32 Func6C880C(const nn::math::VEC3& wpos, FieldName field); // 0x006C880C
    // GetHeightDefault after the table 95E93E of the part (2: the water surface, 1/3: 32 higher) and 112 for the
    // parts of s_Attributes891697
    static f32 GetSurfaceHeightDefault(const nn::math::VEC3& wpos, FieldName field); // 0x006C863C
    // the four centers of the unit have the same height
    static bool HasEqualHeights(const nn::math::VEC3& wpos, FieldName field); // 0x006C8720
    static bool IsFlat(s32 x, s32 z, FieldName field);          // 0x006C8EB8
    static bool IsUnknownFlag80(s32 x, s32 z, FieldName field); // 0x006C8F40
    // the kind of a unit (JmpUnit::GetKind; 0 outside of the field)
    static u8 GetKind(s32 x, s32 z, FieldName field);                                 // 0x006C8564
    static u8 GetKind(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field); // 0x006C85EC

    // Tests of the attribute of a unit (names are ours; "north 03": one of the two units to the north has the
    // attribute UNKNOWN_03, which hides the unit)
    // flag 2 of the attribute, false north 03
    static bool HasFlag02(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field); // 0x006C610C
    static bool HasFlag08(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field); // 0x006C6294
    static bool IsAttribute04(s32 x, s32 z, FieldName field); // 0x006C61B4
    static bool IsAttribute10(s32 x, s32 z, FieldName field); // 0x006C6A24
    static bool IsStoneF9(s32 x, s32 z, FieldName field);                                 // 0x006C6218
    static bool IsStoneF9(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field); // 0x006C6274
    // the attribute AD, or the table 95E93E of the part has 2
    static bool IsAttributeADOrPart95E93E2(const nn::math::VEC3& wpos, FieldName field); // 0x006C61DC
    static bool IsAttributeAD_B1(const nn::math::VEC3& wpos, FieldName field); // 0x006C6C5C
    static bool IsAttribute02_03_04_A2_A5_F8(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field); // 0x006C658C
    DECOMP_EMIT_COPY DECOMP_NOINLINE static bool IsAttribute02_03_04_A2_A5_F8(s32 x, s32 z, FieldName field) // 0x002FCAFC
    {
        s32 blockX = 0;
        s32 blockZ = 0;
        s32 unitX = 0;
        s32 unitZ = 0;
        ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, x, z);
        FieldAttribute attribute = GetAttribute(blockX, blockZ, unitX, unitZ, field);
        return attribute == FIELD_ATTRIBUTE_FLOOR_02 || attribute == FIELD_ATTRIBUTE_FLOOR_F8 ||
               attribute == FIELD_ATTRIBUTE_UNKNOWN_04 || attribute == FIELD_ATTRIBUTE_UNKNOWN_03 ||
               attribute == FIELD_ATTRIBUTE_UNKNOWN_A2 || attribute == FIELD_ATTRIBUTE_UNKNOWN_A3 ||
               attribute == FIELD_ATTRIBUTE_UNKNOWN_A4 || attribute == FIELD_ATTRIBUTE_UNKNOWN_A5;
    }
    // the table 95EB3C, 2 north 03
    static u8 Get95EB3C(const nn::math::VEC3& wpos, FieldName field);                         // 0x006C62AC
    static u8 Get95EB3C(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field);       // 0x006C6334
    // the table 95EB3C is 0, false north 03
    static bool Is95EB3CZero(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field);  // 0x006C66A4
    static u8 Get95E93E(s32 x, s32 z, FieldName field);                                       // 0x006C667C
    static u8 Get95E93E(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field);       // 0x006C668C
    // the table 95EA3D, 0 north 03
    static u8 Get95EA3D(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field);       // 0x006C697C
    // the table 95E93E of the north or south part is not 0
    static bool HasPart95E93E(const nn::math::VEC3& wpos, FieldName field); // 0x006C7030
    static bool HasPart95E93E(s32 x, s32 z, FieldName field);               // 0x006C70E0
    // a part of the unit is in s_Attributes89166C / s_Attributes891697
    static bool HasPartIn89166C(s32 x, s32 z, FieldName field);                                 // 0x006C6CD8
    static bool HasPartIn89166C(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field); // 0x006C6D28
    static bool HasPartIn891697(const nn::math::VEC3& wpos, FieldName field);                   // 0x006C7534
    static bool HasPartIn891697(s32 x, s32 z, FieldName field);                                 // 0x006C7584
    static bool HasPartIn891697(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field); // 0x006C75D4
    // the attribute is in s_Attributes891666 / s_Attributes891672 / s_Attributes89167A
    static bool IsAttributeIn891666(s32 x, s32 z, FieldName field); // 0x006C82DC
    static bool IsAttributeIn891672(s32 x, s32 z, FieldName field); // 0x006C8328
    static bool IsAttributeIn89167A(s32 x, s32 z, FieldName field); // 0x006C8518
    static bool IsAttribute11(s32 x, s32 z, FieldName field); // 0x006C9248
    static bool IsAttributeIn89166C(s32 x, s32 z, FieldName field);                                 // 0x006C8FC8
    static bool IsAttributeIn89166C(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field); // 0x006C905C
    // the north part of the attribute is UNKNOWN_0F
    static bool IsNorthPart0F(s32 x, s32 z, FieldName field); // 0x006C90B0
    // the table 95E93E of the part is 2 / not 0
    static bool IsPart95E93E2(const nn::math::VEC3& wpos, FieldName field);       // 0x006C90E0
    static bool IsPart95E93ENonZero(const nn::math::VEC3& wpos, FieldName field); // 0x006C92B0
    static bool HasFlag01(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field); // 0x006C9460
    static s8 Get95E740(s32 x, s32 z, FieldName field); // 0x006C9478
    // the table 95EC3B is not 0 and the attribute is not one of 7C..7F, FA, FB
    static bool Is95EC3BWithout7C_FB(s32 x, s32 z, FieldName field); // 0x006C7AA4

    // the attribute of a unit (0 outside of the field)
    DECOMP_NOINLINE static FieldAttribute GetAttribute(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, FieldName field); // 0x006C6628
    DECOMP_NOINLINE static FieldAttribute GetAttribute(s32 x, s32 z, FieldName field); // 0x00315660
    DECOMP_NOINLINE static FieldAttribute GetAttribute(const nn::math::VEC3& wpos, FieldName field); // 0x006C65D8
    // the attribute of the part of the unit at a world position (FieldAttributeParts: the triangle of the position); on
    // the shore BsWaveMgr decides whether the water part is wet; 0 outside of the field, SOIL_14 on an auto camp stage
    // without the block
    static FieldAttribute GetPartAttribute(const nn::math::VEC3& wpos, FieldName field); // 0x006C63DC

    // world position -> unit
    DECOMP_NOINLINE static void ToUnit(s32& x, s32& z, const nn::math::VEC3& wpos); // 0x00316FAC | libgarden [tier A]
    static Vector2i ToUnit(const nn::math::VEC3& wpos);            // 0x0052F500
    // block and local unit -> unit
    static void ToUnit(s32& x, s32& z, s32 blockX, s32 blockZ, s32 unitX, s32 unitZ); // 0x006A7968
    static Vector2i ToUnit(const Vector2i& block, const Vector2i& unit);              // 0x0052F57C
    // unit -> block
    static void ToBlock(s32& blockX, s32& blockZ, s32 x, s32 z); // 0x006A7750
    static Vector2i ToBlock(const Vector2i& unit);              // 0x0052F4A8
    // world position -> block
    DECOMP_NOINLINE static Vector2i ToBlock(const nn::math::VEC3& wpos); // 0x0052F42C
    // unit -> unit in its block
    static Vector2i ToLocalUnit(const Vector2i& unit); // 0x0052F3B0
    // world position -> unit in its block
    static Vector2i ToLocalUnit(const nn::math::VEC3& wpos); // 0x0052F2CC
    // unit -> block and unit in it
    DECOMP_NOINLINE static void ToBlockAndLocalUnit(s32& blockX, s32& blockZ, s32& unitX, s32& unitZ, s32 x, s32 z); // 0x006A7998 | libgarden [tier A]
    // world position -> block and unit in it
    DECOMP_NOINLINE static void ToBlockAndLocalUnit(s32& blockX, s32& blockZ, s32& unitX, s32& unitZ, const nn::math::VEC3& wpos); // 0x006A74C4

    // unit -> world position of its corner (y 0)
    static void ToWorld(nn::math::VEC3* out, s32 x, s32 z);          // 0x006A68AC
    static void ToWorld(nn::math::VEC3* out, const Vector2i& unit); // 0x0052F5A8
    // block -> world position of its corner (y 0)
    static void ToWorldOfBlock(nn::math::VEC3* out, s32 blockX, s32 blockZ);   // 0x006A68DC
    static void ToWorldOfBlock(nn::math::VEC3* out, const Vector2i& block); // 0x0052F5F0
    // unit -> world position of its center (y 0)
    static void ToCenterWorld(nn::math::VEC3* out, s32 x, s32 z);                                // 0x006A69E4
    static void ToCenterWorld(nn::math::VEC3* out, const Vector2i& unit);                       // 0x0052F638
    static void ToCenterWorld(nn::math::VEC3* out, const Vector2i& block, const Vector2i& unit); // 0x0052F6A4
    DECOMP_NOINLINE static void ToCenterWorld(nn::math::VEC3* out, s32 blockX, s32 blockZ, s32 unitX, s32 unitZ); // 0x00307158
    // the center of the unit of a world position (y 0)
    static void ToCenterWorld(nn::math::VEC3* out, const nn::math::VEC3& wpos); // 0x006A691C
    // the center of a unit with the height of the unit (GetUnitHeight in field 0)
    static void ToCenterWorldWithUnitHeight(nn::math::VEC3* out, const nn::math::VEC3& wpos);       // 0x006A77BC
    static void ToCenterWorldWithUnitHeight(nn::math::VEC3* out, s32 x, s32 z);                     // 0x006A7894
    static void ToCenterWorldWithUnitHeight(nn::math::VEC3* out, s32 blockX, s32 blockZ, s32 unitX,
                                            s32 unitZ);                                             // 0x006A78F4
    // ... with GetUnitOrBgHeight
    static void ToCenterWorldWithUnitOrBgHeight(nn::math::VEC3* out, const nn::math::VEC3& wpos);   // 0x006A7D3C
    static void ToCenterWorldWithUnitOrBgHeight(nn::math::VEC3* out, s32 x, s32 z);                 // 0x006A7E14
    static void ToCenterWorldWithUnitOrBgHeight(nn::math::VEC3* out, s32 blockX, s32 blockZ, s32 unitX,
                                                s32 unitZ);                                         // 0x006A7E74

private:
    // the height offset of a part after the table 95E93E (shared by GetSurfaceHeight and GetSurfaceHeightEx)
    static f32 GetPartHeightOffset(FieldAttribute part);
};
