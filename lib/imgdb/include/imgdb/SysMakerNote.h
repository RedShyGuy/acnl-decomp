#pragma once

#include "imgdb/imgdb_Types.h"

namespace imgdb {

// the maker note of the system in the Exif data of a photo, the copy of the database values of the image (names
// are ours)
class SysMakerNote
{
public:
    static const u32 MAGIC = 0x31534433;  // "3DS1"

    void Initialize();
    void SetShootingType(ShootingType type);
    void SetDistinctionTypeBit(DistinctionTypeBit bit);

    DECOMP_EMIT_COPY ShootingType GetShootingType() const { return static_cast<ShootingType>(m_ShootingType); } // 0x0075509C
    DECOMP_EMIT_COPY DistinctionTypeBit GetDistinctionTypeBit() const { return static_cast<DistinctionTypeBit>(m_DistinctionTypeBit); } // 0x007550AC

    u32 m_Magic;                        // +0x00
    u32 m_SaveProcessType;              // +0x04 SaveProcessType
    DateTimeSeconds m_CreateSecond;     // +0x08
    u32 m_TitleUniqueId;                // +0x10
    u16 m_HandleTypeBit;                // +0x14 HandleTypeBit
    u16 m_BodyIdValidBits;              // +0x16
    u32 m_BodyIds[BODY_ID_TYPE_NUM];    // +0x18
    f32 _28;                            // +0x28
    u32 _2C;                            // +0x2C
    u32 : 7;                            // +0x30
    u32 m_ShootingType : 5;
    u32 m_DistinctionTypeBit : 16;
    u32 : 4;
    u32 _34[3];                         // +0x34
};
ASSERT_SIZE(SysMakerNote, 0x40);

} // namespace imgdb
