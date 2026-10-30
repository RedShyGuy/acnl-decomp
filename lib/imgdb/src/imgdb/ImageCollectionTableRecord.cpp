#include <cwchar>

#include "imgdb/ImageCollectionTableRecord.h"
#include "imgdb/imgdb_Constants.h"
#include "imgdb/util/util_Api.h"

namespace imgdb {

// 0x005AB864 | nintendogs:bytes [tier B]
void imgdb::ImageCollectionTableRecord::CopyStrToBuf(char* buffer, int size, const wchar_t* string)
{
    std::wcslen(string);
    for (int i = 0; i < size; i++)
    {
        u16 c = string[i];
        buffer[i] = static_cast<s8>(detail::IsLower(c) ? c - 0x20 : c);
        if (buffer[i] == '\0')
        {
            break;
        }
    }
}

// 0x00755ED4 | nintendogs:bytes-fuzzy [tier B]
int imgdb::ImageCollectionTableRecord::CreateImagePath(wchar_t* buffer, int size, StorageType storage,
                                                      bool isLegacy) const
{
    int length = util::CreateImageRootDirectoryPath(buffer, size, storage, isLegacy);
    std::wcscpy(buffer + length, L"/");
    length++;
    length += detail::CopyBufToStr(buffer + length, m_DirectoryName, sizeof(m_DirectoryName));
    std::wcscpy(buffer + length, L"/");
    length++;
    length += detail::CopyBufToStr(buffer + length, m_FileName, sizeof(m_FileName));
    std::wcscpy(buffer + length, L".");
    length++;
    return length + detail::CopyBufToStr(buffer + length, m_Ext, sizeof(m_Ext));
}

// 0x0075601C | nintendogs:bytes-fuzzy [tier B]
int imgdb::ImageCollectionTableRecord::CreateImagePathWithChangeExt(wchar_t* buffer, int size, StorageType storage,
                                                                   ImageKind kind, bool isLegacy) const
{
    int length = util::CreateImageRootDirectoryPath(buffer, size, storage, isLegacy);
    std::wcscpy(buffer + length, L"/");
    length++;
    length += detail::CopyBufToStr(buffer + length, m_DirectoryName, sizeof(m_DirectoryName));
    std::wcscpy(buffer + length, L"/");
    length++;
    length += detail::CopyBufToStr(buffer + length, m_FileName, sizeof(m_FileName));
    int extLength = s_ImageExtensionLengths[kind];
    std::wcscpy(buffer + length, s_ImageExtensions[kind]);
    return length + extLength;
}

} // namespace imgdb
