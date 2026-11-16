#include "field/dJmpBlock.h"

#include <algorithm>

#include "field/dFdInfo.h"
#include "field/dField.h"
#include "nn/nstd/nstd_String.h"
#include "field/dJmpBlock_Visitor.h"

// 0x006AA1B0 (name is ours)
f32 JmpBlock::GetHeight(s32 unitX, s32 unitZ)
{
    JmpUnit* unit = GetUnit(unitX, unitZ);
    s32 height = unit->GetHeightNorth();
    if (!unit->IsFlat()) {
        height = std::max(height, unit->GetHeightSouth());
        height = std::max(height, unit->GetHeightWest());
        height = std::max(height, unit->GetHeightEast());
    }
    return static_cast<f32>(height);
}

// 0x006A9DCC (name is ours)
void JmpBlock::SetUnits(const void* data, u32 size)
{
    if (size == sizeof(m_Units)) {
        nnnstdMemCpy(m_Units, data, sizeof(m_Units));
    }
}

// 0x006A9DE4 (name is ours)
void JmpBlock::GetTriangle(nn::math::VEC3* center, nn::math::VEC3* corner1, nn::math::VEC3* corner2,
                           const nn::math::VEC3& wpos)
{
    s32 blockX = 0;
    s32 blockZ = 0;
    s32 unitX = 0;
    s32 unitZ = 0;
    FdInfo::ToBlockAndLocalUnit(blockX, blockZ, unitX, unitZ, wpos);
    if (!BgInfo::IsInside(unitX, unitZ)) {
        return;
    }
    nn::math::VEC3 unitCenter;
    FdInfo::ToCenterWorld(&unitCenter, blockX, blockZ, unitX, unitZ);
    f32 dx = wpos.x - unitCenter.x;
    f32 dz = wpos.z - unitCenter.z;
    ssys::ma::Vec3i points[3];
    JmpUnit* unit = GetUnit(unitX, unitZ);
    if (dx + dz > 0.0f) {
        if (dz - dx > 0.0f) {
            unit->GetTriangleSouth(points);
        } else {
            unit->GetTriangleEast(points);
        }
    } else {
        if (dz - dx > 0.0f) {
            unit->GetTriangleWest(points);
        } else {
            unit->GetTriangleNorth(points);
        }
    }
    s32 x = m_WorldPos.x + unitX * FdInfo::UNIT_WORLD_SIZE_X + FdInfo::UNIT_WORLD_SIZE_HALF_X;
    s32 z = m_WorldPos.y + unitZ * FdInfo::UNIT_WORLD_SIZE_Z + FdInfo::UNIT_WORLD_SIZE_HALF_Z;
    for (s32 i = 0; i < 3; i++) {
        points[i].x += x;
        points[i].z += z;
    }
    center->x = static_cast<f32>(points[0].x);
    center->y = static_cast<f32>(points[0].y);
    center->z = static_cast<f32>(points[0].z);
    corner1->x = static_cast<f32>(points[1].x);
    corner1->y = static_cast<f32>(points[1].y);
    corner1->z = static_cast<f32>(points[1].z);
    corner2->x = static_cast<f32>(points[2].x);
    corner2->y = static_cast<f32>(points[2].y);
    corner2->z = static_cast<f32>(points[2].z);
}

// 0x006AA278 (name is ours)
bool JmpBlock::Initialize(s32 blockX, s32 blockZ)
{
    m_BlockPos.x = blockX;
    m_BlockPos.y = blockZ;
    m_WorldPos.x = blockX * FdInfo::BLOCK_WORLD_SIZE_X;
    m_WorldPos.y = blockZ * FdInfo::BLOCK_WORLD_SIZE_Z;
    FdInfo::ToWorldOfBlock(&m_WPos, blockX, blockZ);
    return true;
}

// 0x006AA2B8 (name is ours)
void JmpBlock::Accept(Visitor* visitor)
{
    for (s32 z = 0; z < static_cast<s32>(UNIT_BASE_NUM); z++) {
        for (s32 x = 0; x < static_cast<s32>(UNIT_BASE_NUM); x++) {
            if (visitor->Visit(this, x, z) == true) {
                return;
            }
        }
    }
}

// 0x006AA30C (name is ours)
JmpBlock::JmpBlock() : m_BlockPos(0, 0), m_WorldPos(0, 0)
{
}

// 0x006AA354 (name is ours)
JmpBlock::~JmpBlock()
{
    // (only the members are destroyed)
}
