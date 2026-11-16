#pragma once

#include "decomp.h"
#include "field/dFieldAttribute.h"
#include "field/dFdInfo.h"
#include "field/dFieldTypes.h"
#include "ssys/ma/dVec3i.h"
#include "nn/math/math_Vector3.h"

// The height and the attribute of one unit (name and layout after libgarden field/JmpUnit.hpp; the function names are
// ours).
//
// The unit is split into four triangles from its center to its sides; each height word holds the heights of one
// triangle in steps of 8 world units: bits 0-4 the center, bits 5-9 and 10-14 the two corners.
class JmpUnit
{
public:
    // the triangles of m_Heights
    enum Side : u8
    {
        SIDE_NORTH = 0, // corners north-east, north-west
        SIDE_WEST = 1,  // corners north-west, south-west
        SIDE_SOUTH = 2, // corners south-west, south-east
        SIDE_EAST = 3,  // corners south-east, north-east
        SIDE_NUM = 4,
    };
    // the bits of m_Flags
    enum Flag : u8
    {
        FLAG_KIND_MASK = 0x03,
        FLAG_FLAT = 0x40, // the height of the north center is the height of the whole unit
        FLAG_UNKNOWN_80 = 0x80,
    };

    DECOMP_EMIT_COPY DECOMP_NOINLINE JmpUnit() // 0x0060DD18
    {
        m_Heights[SIDE_NORTH] = 0;
        m_Heights[SIDE_WEST] = 0;
        m_Heights[SIDE_SOUTH] = 0;
        m_Heights[SIDE_EAST] = 0;
        m_Attribute = FIELD_ATTRIBUTE_UNKNOWN_00;
        m_Flags = 0;
    }
    DECOMP_EMIT_COPY ~JmpUnit() {} // 0x0060DD38

    // 0x0060DD60 (copy)
    DECOMP_EMIT_COPY DECOMP_NOINLINE JmpUnit& operator=(const JmpUnit& other) // 0x0060DCE4
    {
        m_Heights[SIDE_NORTH] = other.m_Heights[SIDE_NORTH];
        m_Heights[SIDE_WEST] = other.m_Heights[SIDE_WEST];
        m_Heights[SIDE_SOUTH] = other.m_Heights[SIDE_SOUTH];
        m_Heights[SIDE_EAST] = other.m_Heights[SIDE_EAST];
        m_Attribute = other.m_Attribute;
        m_Flags = other.m_Flags;
        return *this;
    }

    // the height of the center (of the triangle of a side)
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetHeightNorth() const { return (m_Heights[SIDE_NORTH] & 0x1F) << 3; } // 0x0075E844
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetHeightSouth() const { return (m_Heights[SIDE_SOUTH] & 0x1F) << 3; } // 0x0075E854
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetHeightWest() const { return (m_Heights[SIDE_WEST] & 0x1F) << 3; }   // 0x0075E864
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetHeightEast() const { return (m_Heights[SIDE_EAST] & 0x1F) << 3; }   // 0x0075E874

    DECOMP_EMIT_COPY DECOMP_NOINLINE FieldAttribute GetAttribute() const { return m_Attribute; } // 0x0075E644
    // (its calls were replaced by its instruction)
    void SetAttribute(FieldAttribute attribute) { m_Attribute = attribute; }
    DECOMP_EMIT_COPY DECOMP_NOINLINE u8 GetKind() const { return m_Flags & FLAG_KIND_MASK; } // 0x0075E838
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsFlat() const { return (m_Flags & FLAG_FLAT) >> 6; } // 0x0075E8A4
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsUnknownFlag80() const { return (m_Flags & FLAG_UNKNOWN_80) >> 7; } // 0x0075E8D4

    // the triangle of a side relative to the unit center: the center and the two corners of the side (y = height)
    DECOMP_EMIT_COPY DECOMP_NOINLINE void GetTriangleNorth(ssys::ma::Vec3i* points) const; // 0x0075E64C
    DECOMP_EMIT_COPY DECOMP_NOINLINE void GetTriangleSouth(ssys::ma::Vec3i* points) const; // 0x0075E6C0
    DECOMP_EMIT_COPY DECOMP_NOINLINE void GetTriangleWest(ssys::ma::Vec3i* points) const;  // 0x0075E734
    DECOMP_EMIT_COPY DECOMP_NOINLINE void GetTriangleEast(ssys::ma::Vec3i* points) const;  // 0x0075E7A4

private:
    /* 0x0 */ u16 m_Heights[SIDE_NUM];
    /* 0x8 */ FieldAttribute m_Attribute;
    /* 0x9 */ u8 m_Flags;
};
ASSERT_SIZE(JmpUnit, 0xA);

// The heights and attributes of the units of one block of a field (name and layout after libgarden field/JmpBlock.hpp;
// the function names are ours).
class JmpBlock
{
public:
    class Visitor;

    // the size of the units of a block for SetUnits (name is ours)
    static const u32 DATA_SIZE = sizeof(JmpUnit) * UNIT_BASE_NUM * UNIT_BASE_NUM;

    JmpBlock();  // 0x006AA30C
    ~JmpBlock(); // 0x006AA354

    bool Initialize(s32 blockX, s32 blockZ); // 0x006AA278
    // (empty; the calls were removed by armlink)
    void Finalize() {}
    // calls the visitor for the units until it returns true
    void Accept(Visitor* visitor); // 0x006AA2B8

    // the height of a unit: the north center if it is flat, otherwise the highest of the four centers
    f32 GetHeight(s32 unitX, s32 unitZ); // 0x006AA1B0
    // the world positions of the triangle of the unit at a world position (unchanged outside of the block)
    void GetTriangle(nn::math::VEC3* center, nn::math::VEC3* corner1, nn::math::VEC3* corner2,
                     const nn::math::VEC3& wpos); // 0x006A9DE4
    // takes over the units from data of 0xA00 bytes (other sizes are ignored)
    void SetUnits(const void* data, u32 size); // 0x006A9DCC

    // (the position is taken modulo 16)
    JmpUnit* GetUnit(s32 unitX, s32 unitZ)
    {
        return &m_Units[unitZ & (UNIT_BASE_NUM - 1)][unitX & (UNIT_BASE_NUM - 1)];
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE FieldAttribute GetAttribute(s32 unitX, s32 unitZ) { return GetUnit(unitX, unitZ)->GetAttribute(); } // 0x0075E624
    DECOMP_EMIT_COPY DECOMP_NOINLINE u8 GetKind(s32 unitX, s32 unitZ) { return GetUnit(unitX, unitZ)->GetKind(); }           // 0x0075E818
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsFlat(s32 unitX, s32 unitZ) { return GetUnit(unitX, unitZ)->IsFlat(); }          // 0x0075E884
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsUnknownFlag80(s32 unitX, s32 unitZ) { return GetUnit(unitX, unitZ)->IsUnknownFlag80(); } // 0x0075E8B4
    DECOMP_EMIT_COPY DECOMP_NOINLINE void SetAttribute(s32 unitX, s32 unitZ, FieldAttribute attribute) { GetUnit(unitX, unitZ)->SetAttribute(attribute); } // 0x0060DB7C
    DECOMP_EMIT_COPY DECOMP_NOINLINE void SetUnit(s32 unitX, s32 unitZ, const JmpUnit& unit) { *GetUnit(unitX, unitZ) = unit; } // 0x0060DD3C
    DECOMP_EMIT_COPY DECOMP_NOINLINE void CopyUnit(JmpUnit* out, s32 unitX, s32 unitZ) { *out = *GetUnit(unitX, unitZ); } // 0x007677C4

private:
    /* 0x000 */ Vector2i m_BlockPos;
    /* 0x008 */ Vector2i m_WorldPos; // (libgarden: m_UnitPos) the block in world units
    /* 0x010 */ nn::math::VEC3 m_WPos;
    /* 0x01C */ JmpUnit m_Units[UNIT_BASE_NUM][UNIT_BASE_NUM];
};
ASSERT_SIZE(JmpBlock, 0xA1C);

inline void JmpUnit::GetTriangleNorth(ssys::ma::Vec3i* points) const
{
    points[0].x = 0;
    points[0].y = (m_Heights[SIDE_NORTH] & 0x1F) << 3;
    points[0].z = 0;
    points[1].x = FdInfo::UNIT_WORLD_SIZE_HALF_X;
    points[1].y = (m_Heights[SIDE_NORTH] & 0x3E0) >> 2;
    points[1].z = -FdInfo::UNIT_WORLD_SIZE_HALF_Z;
    points[2].x = -FdInfo::UNIT_WORLD_SIZE_HALF_X;
    points[2].y = (m_Heights[SIDE_NORTH] & 0x7C00) >> 7;
    points[2].z = -FdInfo::UNIT_WORLD_SIZE_HALF_Z;
}

inline void JmpUnit::GetTriangleSouth(ssys::ma::Vec3i* points) const
{
    points[0].x = 0;
    points[0].y = (m_Heights[SIDE_SOUTH] & 0x1F) << 3;
    points[0].z = 0;
    points[1].x = -FdInfo::UNIT_WORLD_SIZE_HALF_X;
    points[1].y = (m_Heights[SIDE_SOUTH] & 0x3E0) >> 2;
    points[1].z = FdInfo::UNIT_WORLD_SIZE_HALF_Z;
    points[2].x = FdInfo::UNIT_WORLD_SIZE_HALF_X;
    points[2].y = (m_Heights[SIDE_SOUTH] & 0x7C00) >> 7;
    points[2].z = FdInfo::UNIT_WORLD_SIZE_HALF_Z;
}

inline void JmpUnit::GetTriangleWest(ssys::ma::Vec3i* points) const
{
    points[0].x = 0;
    points[0].y = (m_Heights[SIDE_WEST] & 0x1F) << 3;
    points[0].z = 0;
    points[1].x = -FdInfo::UNIT_WORLD_SIZE_HALF_X;
    points[1].y = (m_Heights[SIDE_WEST] & 0x3E0) >> 2;
    points[1].z = -FdInfo::UNIT_WORLD_SIZE_HALF_Z;
    points[2].x = -FdInfo::UNIT_WORLD_SIZE_HALF_X;
    points[2].y = (m_Heights[SIDE_WEST] & 0x7C00) >> 7;
    points[2].z = FdInfo::UNIT_WORLD_SIZE_HALF_Z;
}

inline void JmpUnit::GetTriangleEast(ssys::ma::Vec3i* points) const
{
    points[0].x = 0;
    points[0].y = (m_Heights[SIDE_EAST] & 0x1F) << 3;
    points[0].z = 0;
    points[1].x = FdInfo::UNIT_WORLD_SIZE_HALF_X;
    points[1].y = (m_Heights[SIDE_EAST] & 0x3E0) >> 2;
    points[1].z = FdInfo::UNIT_WORLD_SIZE_HALF_Z;
    points[2].x = FdInfo::UNIT_WORLD_SIZE_HALF_X;
    points[2].y = (m_Heights[SIDE_EAST] & 0x7C00) >> 7;
    points[2].z = -FdInfo::UNIT_WORLD_SIZE_HALF_Z;
}
