#pragma once

#include "decomp.h"

// The basic types of the field (names after libgarden field/types.hpp, FieldName.hpp).
//
// A field consists of blocks (the acres of the town) of 16*16 units; a unit is 32*32 world units. Positions in units
// are absolute (block * 16 + local unit).

// the units per block in each direction
const u32 UNIT_BASE_NUM = 16;
// the layers of items of a field (a field has one or two)
const u32 MAX_LAYER_NUM = 2;

// the blocks of the town (the outer ring has no items)
const u32 TOWN_BLOCK_X_NUM = 7;
const u32 TOWN_BLOCK_Z_NUM = 6;
const u32 TOWN_FG_BLOCK_X_NUM = TOWN_BLOCK_X_NUM - 2;
const u32 TOWN_FG_BLOCK_Z_NUM = TOWN_BLOCK_Z_NUM - 2;
const u32 TOWN_FG_BLOCK_TOTAL_NUM = TOWN_FG_BLOCK_X_NUM * TOWN_FG_BLOCK_Z_NUM;

// the blocks of the island
const u32 ISLAND_BLOCK_X_NUM = 4;
const u32 ISLAND_BLOCK_Z_NUM = 4;
const u32 ISLAND_FG_BLOCK_X_NUM = ISLAND_BLOCK_X_NUM - 2;
const u32 ISLAND_FG_BLOCK_Z_NUM = ISLAND_BLOCK_Z_NUM - 2;

// a field of the field table (Field::s_pFields); 0 is the field of the current stage
enum FieldName : u8
{
    FIELD_NAME_CURRENT = 0,
    // the fields of the island huts Island_Hut0..9 (the table 0x00953728 of the stage of a field)
    FIELD_NAME_ISLAND_HUT_0 = 32,
    FIELD_NAME_ISLAND_HUT_9 = 41,
};

// a position in blocks or units (x, z); the original passes it by reference and calls its copy constructor and
// destructor (so it has them; the destructor calls were removed by armlink)
struct Vector2i {
    DECOMP_EMIT_COPY DECOMP_NOINLINE Vector2i() : x(0), y(0) {} // 0x0052F788
    DECOMP_EMIT_COPY Vector2i(s32 x_, s32 y_) : x(x_), y(y_) {} // 0x0052F780
    DECOMP_EMIT_COPY DECOMP_NOINLINE Vector2i(const Vector2i& other) : x(other.x), y(other.y) {} // 0x0052F76C
    DECOMP_EMIT_COPY ~Vector2i() {} // 0x0052F798

    DECOMP_EMIT_COPY DECOMP_NOINLINE Vector2i& operator=(const Vector2i& other) // 0x0052F79C
    {
        x = other.x;
        y = other.y;
        return *this;
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE Vector2i& operator+=(const Vector2i& other) // 0x0052F7B0
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    // (a copy in the shared area of the inline functions)
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool operator!=(const Vector2i& other) const // 0x00748E44
    {
        return !(other.x == x && other.y == y);
    }

    // (0, 0) (name is ours)
    static const Vector2i& Zero(); // 0x0052F720

    s32 x;
    s32 y; // the z coordinate
};
ASSERT_SIZE(Vector2i, 0x8);
