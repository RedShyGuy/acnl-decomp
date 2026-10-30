#pragma once

#include "imgdb/FaceInfo.h"
#include "imgdb/imgdb_Types.h"

namespace imgdb {

// what the caller says about an image it registers (copied into the ImageInfo of the record)
class ImageDbRecordParam
{
public:
    static const int FACE_NUM = 4;

    void Initialize(ImageKind kind);
    void Initialize();
    void ResetBodyId(BodyIdType type);
    void SetBodyId(BodyIdType type, u32 id);
    bool IsValidBodyId(BodyIdType type) const;
    u32 GetBodyId(BodyIdType type) const;

    DateTimeSeconds m_DateTime;          // +0x00
    u32 m_TitleUniqueId;                 // +0x08
    u8 m_SaveProcessType;                // +0x0C SaveProcessType
    u8 m_HandleTypeBit;                  // +0x0D HandleTypeBit
    u8 m_DistinctionTypeBit;             // +0x0E DistinctionTypeBit
    u8 m_BodyIdValidBits;                // +0x0F
    u32 m_BodyIds[BODY_ID_TYPE_NUM];     // +0x10
    u8 m_ShootingType;                   // +0x20 ShootingType
    u8 _21[3];                           // +0x21
    FaceInfo m_Faces[FACE_NUM];          // +0x24
};
ASSERT_SIZE(ImageDbRecordParam, 0x6C);

} // namespace imgdb
