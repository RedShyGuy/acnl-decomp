#pragma once

#include "imgdb/imgdb_Types.h"

namespace imgdb {

// an image file: DCF names (upper case, without terminator when full), dates and the attributes of the database
class ImageInfo
{
public:
    void Initialize();
    void ResetBodyId(BodyIdType type);
    void SetFileName(const IndexInfo& index);
    void SetFileName(const char* name);
    void SetFileName(const wchar_t* name);
    static void CopyStrToBuf(char* buffer, int size, const char* string);
    static void CopyStrToBuf(char* buffer, int size, const wchar_t* string);
    static ImageKind GetImageKindFromExt(const wchar_t* ext);
    void SetDirectoryNo(int no);
    void SetShootingType(ShootingType type);
    void SetValidityFlag(bool isValid);
    void SetDirectoryName(const wchar_t* name);
    void SetDirectoryName(const IndexInfo& index, bool isLegacy);
    void SetHandleTypeBit(HandleTypeBit bit);
    void SetSaveProcessType(SaveProcessType type);
    void SetDistinctionTypeBit(DistinctionTypeBit bit);
    void SetExt(ImageKind kind);
    void SetExt(const char* ext);
    void SetExt(const wchar_t* ext);
    void SetBodyId(BodyIdType type, u32 id);

    int GetDirectoryNo() const;
    bool ValidateFileName() const;
    bool ValidateDirectoryName() const;
    int GetFileName(wchar_t* buffer, int size) const;
    bool ValidateExt() const;
    ImageKind GetImageKind() const;
    IndexInfo GetIndexInfo() const;
    bool IsValidBodyId(BodyIdType type) const;
    ShootingType GetShootingType() const;
    bool GetValidityFlag() const;
    int GetDirectoryName(wchar_t* buffer, int size) const;
    HandleTypeBit GetHandleTypeBit() const;
    SaveProcessType GetSaveProcessType() const;
    bool ValidateWithoutPath() const;
    DistinctionTypeBit GetDistinctionTypeBit() const;
    int GetExt(wchar_t* buffer, int size) const;
    bool Validate() const;
    u32 GetBodyId(BodyIdType type) const;

    char m_Ext[4];                    // +0x00 "JPG"
    char m_FileName[8];               // +0x04 "HNI_nnnn"
    char m_DirectoryName[8];          // +0x0C "nnnNIN03"
    DateTimeSeconds m_DateTime[2];    // +0x14
    u32 m_TitleUniqueId;              // +0x24
    u32 m_IsValid : 1;                // +0x28
    u32 m_HandleTypeBit : 8;
    u32 m_BodyIdValidBits : 4;
    u32 : 19;
    u32 m_ShootingType : 5;           // +0x2C
    u32 m_DistinctionTypeBit : 16;
    u32 m_SaveProcessType : 4;
    u32 : 7;
    u32 m_BodyIds[BODY_ID_TYPE_NUM];  // +0x30
    u32 m_FileSize;                   // +0x40
    u8 _44[12];                       // +0x44
};
ASSERT_SIZE(ImageInfo, 0x50);

} // namespace imgdb
