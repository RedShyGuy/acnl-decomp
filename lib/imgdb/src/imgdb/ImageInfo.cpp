#include <cstdio>
#include <cstring>
#include <cwchar>

#include "imgdb/ImageInfo.h"
#include "imgdb/imgdb_Constants.h"
#include "imgdb/imgdb_Internal.h"
#include "imgdb/util/util_Api.h"

namespace imgdb {

namespace {

using detail::CopyBufToStr;
using detail::IsLower;

// the characters a name may have
inline bool IsNameChar(int c)
{
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

} // namespace

// (falls into DateTimeSeconds::Initialize for the second date)
// 0x005A95D8 | nintendogs:bytes [tier B]
void imgdb::ImageInfo::Initialize()
{
    std::memset(this, 0, sizeof(*this));
    m_DateTime[0].Initialize();
    m_DateTime[1].Initialize();
}

// 0x005B0C08 | nintendogs:bytes [tier B]
void imgdb::ImageInfo::ResetBodyId(BodyIdType type)
{
    m_BodyIdValidBits &= ~(1 << type);
    m_BodyIds[type] = 0;
}

// "HNI_" and the number of the file
// 0x005B0C4C | nintendogs:bytes [tier B]
void imgdb::ImageInfo::SetFileName(const IndexInfo& index)
{
    char name[16];
    std::sprintf(name, "%s%04d", s_FileNamePrefix, index.m_FileIndex + 1);
    CopyStrToBuf(m_FileName, sizeof(m_FileName), name);
}

// 0x005B0C98 (name is ours)
void imgdb::ImageInfo::SetFileName(const char* name)
{
    CopyStrToBuf(m_FileName, sizeof(m_FileName), name);
}

// (in upper case; the original measures the string first and does not use the length)
// 0x005B0CA8 | nintendogs:bytes [tier B]
DECOMP_NOINLINE void imgdb::ImageInfo::CopyStrToBuf(char* buffer, int size, const char* string)
{
    for (int i = 0; i < size; i++)
    {
        s8 c = string[i];
        buffer[i] = static_cast<s8>(IsLower(c) ? c - 0x20 : c);
        if (buffer[i] == '\0')
        {
            break;
        }
    }
}

// 0x005B0D64 | nintendogs:callgraph [tier C]
void imgdb::ImageInfo::SetFileName(const wchar_t* name)
{
    CopyStrToBuf(m_FileName, sizeof(m_FileName), name);
}

// 0x005B0D74 | nintendogs:callgraph [tier C]
DECOMP_NOINLINE void imgdb::ImageInfo::CopyStrToBuf(char* buffer, int size, const wchar_t* string)
{
    for (int i = 0; i < size; i++)
    {
        u16 c = string[i];
        buffer[i] = static_cast<s8>(IsLower(c) ? c - 0x20 : c);
        if (buffer[i] == '\0')
        {
            break;
        }
    }
}

// 0x005B0E34 (name is ours)
imgdb::ImageKind imgdb::ImageInfo::GetImageKindFromExt(const wchar_t* ext)
{
    for (int i = 0; i < IMAGE_KIND_NUM; i++)
    {
        const wchar_t* extension = s_ImageExtensions[static_cast<u8>(i)];
        if (*extension == L'.')
        {
            extension++;
        }
        if (std::wcscmp(ext, extension) == 0)
        {
            return static_cast<ImageKind>(i);
        }
    }
    return IMAGE_KIND_NONE;
}

// the number of the directory (its suffix stays)
// 0x005B0EE4 | nintendogs:callgraph [tier C]
DECOMP_NOINLINE void imgdb::ImageInfo::SetDirectoryNo(int no)
{
    char number[4];
    std::sprintf(number, "%03d", no);
    m_DirectoryName[0] = number[0];
    m_DirectoryName[1] = number[1];
    m_DirectoryName[2] = number[2];
}

// 0x005B0F20 | nintendogs:bytes [tier B]
void imgdb::ImageInfo::SetShootingType(ShootingType type)
{
    m_ShootingType = type;
}

// 0x005B0F38 | nintendogs:bytes [tier B]
void imgdb::ImageInfo::SetValidityFlag(bool isValid)
{
    m_IsValid = isValid;
}

// 0x005B0F50 | nintendogs:callgraph [tier C]
void imgdb::ImageInfo::SetDirectoryName(const wchar_t* name)
{
    CopyStrToBuf(m_DirectoryName, sizeof(m_DirectoryName), name);
}

// "nnnNIN03", or "nnnNIN02" for the directories of the DSi
// 0x005B0F60 | nintendogs:bytes [tier B]
DECOMP_NOINLINE void imgdb::ImageInfo::SetDirectoryName(const IndexInfo& index, bool isLegacy)
{
    const char* suffix = isLegacy ? s_LegacyDirectorySuffix : s_DirectorySuffix;
    char name[16];
    std::sprintf(name, "%03d%s", index.m_DirectoryIndex + 100, suffix);
    CopyStrToBuf(m_DirectoryName, sizeof(m_DirectoryName), name);
}

// 0x005B0FB8 | nintendogs:bytes [tier B]
void imgdb::ImageInfo::SetHandleTypeBit(HandleTypeBit bit)
{
    m_HandleTypeBit = bit;
}

// 0x005B0FD0 | nintendogs:bytes [tier B]
void imgdb::ImageInfo::SetSaveProcessType(SaveProcessType type)
{
    m_SaveProcessType = type;
}

// 0x005B0FEC | nintendogs:bytes [tier B]
void imgdb::ImageInfo::SetDistinctionTypeBit(DistinctionTypeBit bit)
{
    m_DistinctionTypeBit = bit;
}

// 0x005B1070 | nintendogs:bytes [tier B]
DECOMP_NOINLINE void imgdb::ImageInfo::SetExt(ImageKind kind)
{
    const wchar_t* extension = s_ImageExtensions[kind];
    if (*extension == L'.')
    {
        extension++;
    }
    CopyStrToBuf(m_Ext, sizeof(m_Ext), extension);
}

// 0x005B1090 (name is ours)
void imgdb::ImageInfo::SetExt(const char* ext)
{
    CopyStrToBuf(m_Ext, sizeof(m_Ext), ext);
}

// 0x005B109C | nintendogs:callgraph [tier C]
void imgdb::ImageInfo::SetExt(const wchar_t* ext)
{
    CopyStrToBuf(m_Ext, sizeof(m_Ext), ext);
}

// 0x005B10A8 | nintendogs:bytes [tier B]
void imgdb::ImageInfo::SetBodyId(BodyIdType type, u32 id)
{
    m_BodyIdValidBits = static_cast<u8>(m_BodyIdValidBits | (1 << type));
    m_BodyIds[type] = id;
}

// 0x005AFAD0 | nintendogs:callgraph [tier C]
int imgdb::ImageInfo::GetDirectoryNo() const
{
    return util::GetDirectoryNoFromDcfDirectoryName(m_DirectoryName);
}

// 0x005AF820 (name is ours)
bool imgdb::ImageInfo::ValidateFileName() const
{
    return util::ValidateDcfFileNameWithoutLength(m_FileName, NULL);
}

// 0x005AFC18 (name is ours)
bool imgdb::ImageInfo::ValidateDirectoryName() const
{
    return util::ValidateDcfDirectoryNameWithoutLength(m_DirectoryName, NULL);
}

// (the size of the buffer is not checked)
// 0x0075634C | nintendogs:bytes [tier B]
int imgdb::ImageInfo::GetFileName(wchar_t* buffer, int size) const
{
    return CopyBufToStr(buffer, m_FileName, sizeof(m_FileName));
}

// 0x007563A0 | nintendogs:bytes [tier B]
bool imgdb::ImageInfo::ValidateExt() const
{
    for (int i = 0; i < static_cast<int>(sizeof(m_Ext)); i++)
    {
        s8 c = m_Ext[i];
        if (c == '\0')
        {
            break;
        }
        if (!IsNameChar(c))
        {
            return false;
        }
    }
    return true;
}

// 0x007563F4 | nintendogs:bytes [tier B]
imgdb::ImageKind imgdb::ImageInfo::GetImageKind() const
{
    wchar_t ext[6];
    CopyBufToStr(ext, m_Ext, sizeof(m_Ext));
    return GetImageKindFromExt(ext);
}

// 0x007564A0 | nintendogs:bytes [tier B]
DECOMP_NOINLINE imgdb::IndexInfo imgdb::ImageInfo::GetIndexInfo() const
{
    IndexInfo index;
    index.SetFileIndex(util::GetFileNoFromDcfFileName(m_FileName) - 1);
    index.SetDirectoryIndex(util::GetDirectoryNoFromDcfDirectoryName(m_DirectoryName) - 100);
    return index;
}

// 0x007564D8 | nintendogs:bytes [tier B]
bool imgdb::ImageInfo::IsValidBodyId(BodyIdType type) const
{
    return (m_BodyIdValidBits & (1 << type)) != 0;
}

// 0x007564F4 | nintendogs:callgraph [tier C]
imgdb::ShootingType imgdb::ImageInfo::GetShootingType() const
{
    return static_cast<ShootingType>(m_ShootingType);
}

// 0x00756500 | nintendogs:callgraph [tier C]
bool imgdb::ImageInfo::GetValidityFlag() const
{
    return m_IsValid;
}

// 0x00756520 | nintendogs:bytes [tier B]
DECOMP_NOINLINE int imgdb::ImageInfo::GetDirectoryName(wchar_t* buffer, int size) const
{
    return CopyBufToStr(buffer, m_DirectoryName, sizeof(m_DirectoryName));
}

// 0x00756574 | nintendogs:callgraph [tier C]
imgdb::HandleTypeBit imgdb::ImageInfo::GetHandleTypeBit() const
{
    return static_cast<HandleTypeBit>(m_HandleTypeBit);
}

// 0x00756584 | nintendogs:callgraph [tier C]
imgdb::SaveProcessType imgdb::ImageInfo::GetSaveProcessType() const
{
    return static_cast<SaveProcessType>(m_SaveProcessType);
}

// 0x00756594 | nintendogs:bytes [tier B]
bool imgdb::ImageInfo::ValidateWithoutPath() const
{
    if (m_IsValid && (m_DateTime[0].IsInvalid() || m_DateTime[1].IsInvalid()))
    {
        return false;
    }
    return true;
}

// 0x007565D8 | nintendogs:callgraph [tier C]
imgdb::DistinctionTypeBit imgdb::ImageInfo::GetDistinctionTypeBit() const
{
    return static_cast<DistinctionTypeBit>(m_DistinctionTypeBit);
}

// 0x007565E8 | nintendogs:bytes [tier B]
int imgdb::ImageInfo::GetExt(wchar_t* buffer, int size) const
{
    return CopyBufToStr(buffer, m_Ext, sizeof(m_Ext));
}

// 0x0075663C | nintendogs:bytes [tier B]
bool imgdb::ImageInfo::Validate() const
{
    if (m_IsValid)
    {
        if (!ValidateExt() || !ValidateFileName() || !ValidateDirectoryName())
        {
            return false;
        }
    }
    return ValidateWithoutPath();
}

// 0x0075670C | nintendogs:callgraph [tier C]
u32 imgdb::ImageInfo::GetBodyId(BodyIdType type) const
{
    return m_BodyIds[type];
}

namespace util {

// the copy of an image info, in a directory of the DSi or with the number of its directory
// 0x005B0E8C | nintendogs:bytes [tier B]
void GenerateImageInfoFromImageInfo(ImageInfo& info, const ImageInfo& source, bool isLegacy)
{
    info = source;
    if (isLegacy)
    {
        info.SetDirectoryName(info.GetIndexInfo(), isLegacy);
        return;
    }
    info.SetDirectoryNo(info.GetIndexInfo().m_DirectoryIndex + 100);
}

// 0x005B1008 | nintendogs:bytes [tier B]
void GenerateImageInfoFromImageInfo(ImageInfo& info, const ImageInfo& source, ImageKind kind, bool isLegacy)
{
    info = source;
    if (isLegacy)
    {
        info.SetDirectoryName(info.GetIndexInfo(), isLegacy);
    }
    else
    {
        info.SetDirectoryNo(info.GetIndexInfo().m_DirectoryIndex + 100);
    }
    info.SetExt(kind);
}

// 0x0075650C | nintendogs:bytes [tier B]
int CreateImageDirectoryName(wchar_t* buffer, int size, const ImageInfo& info)
{
    return info.GetDirectoryName(buffer, size);
}

} // namespace util

} // namespace imgdb
