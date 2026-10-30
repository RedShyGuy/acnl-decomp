#include <cwchar>
#include <new>

#include "imgdb/ImageSearcher.h"
#include "imgdb/imgdb_Constants.h"
#include "imgdb/util/util_Api.h"

namespace imgdb {

namespace {

const int ROOT_PATH_SIZE = 21;
const int DIRECTORY_PATH_SIZE = 30;
// "nnnNNNNN" and "XXXXnnnn"
const int DCF_NAME_LENGTH = 8;
const int EXT_LENGTH_MAX = 4;

} // namespace

// 0x005A7354 | nintendogs:bytes [tier B]
void imgdb::ImageSearcher::Initialize(Allocator& allocator, StorageType storage)
{
    m_pAllocator = &allocator;
    m_Storage = storage;
    wchar_t rootPath[ROOT_PATH_SIZE];
    util::CreateImageRootDirectoryPath(rootPath, ROOT_PATH_SIZE, storage, false);
    m_pRootDirectory = CreateDirectory(rootPath, 1000);
    m_ImageInfo.Initialize();
}

// 0x005A73D0 | nintendogs:bytes-fuzzy [tier B]
imgdb::ImageSearcher::SearchResult imgdb::ImageSearcher::SearchFile()
{
    if (m_pDirectory == NULL || !m_pDirectory->Read())
    {
        return SEARCH_RESULT_END;
    }
    if (!m_pDirectory->IsFile())
    {
        return SEARCH_RESULT_SKIP;
    }
    const wchar_t* name = m_pDirectory->GetEntry()->name;
    int length = std::wcslen(name);
    if (length < DCF_NAME_LENGTH)
    {
        return SEARCH_RESULT_SKIP;
    }
    m_ImageInfo.SetFileName("");
    m_ImageInfo.SetExt("");
    if (length > DCF_NAME_LENGTH)
    {
        if (name[DCF_NAME_LENGTH] != L'.')
        {
            return SEARCH_RESULT_SKIP;
        }
        int extLength = length - (DCF_NAME_LENGTH + 1);
        if (extLength < 1 || extLength > EXT_LENGTH_MAX)
        {
            return SEARCH_RESULT_SKIP;
        }
        wchar_t fileName[DCF_NAME_LENGTH + 1];
        std::wmemcpy(fileName, name, DCF_NAME_LENGTH);
        fileName[DCF_NAME_LENGTH] = L'\0';
        m_ImageInfo.SetFileName(fileName);
        m_ImageInfo.SetExt(name + DCF_NAME_LENGTH + 1);
    }
    else
    {
        m_ImageInfo.SetFileName(name);
        m_ImageInfo.SetExt("");
    }
    if (m_ImageInfo.ValidateFileName() && m_ImageInfo.ValidateExt() && m_ImageInfo.GetImageKind() != IMAGE_KIND_NONE)
    {
        return SEARCH_RESULT_FOUND;
    }
    m_ImageInfo.SetFileName("");
    m_ImageInfo.SetExt("");
    return SEARCH_RESULT_SKIP;
}

// 0x005A7524 | nintendogs:bytes-fuzzy [tier B]
imgdb::ImageSearcher::SearchResult imgdb::ImageSearcher::SearchDirectory()
{
    if (!m_pRootDirectory->Read())
    {
        return SEARCH_RESULT_END;
    }
    if (!m_pRootDirectory->IsDirectory())
    {
        return SEARCH_RESULT_SKIP;
    }
    const wchar_t* name = m_pRootDirectory->GetEntry()->name;
    if (std::wcslen(name) != DCF_NAME_LENGTH)
    {
        return SEARCH_RESULT_SKIP;
    }
    m_ImageInfo.SetDirectoryName(name);
    if (!m_ImageInfo.ValidateDirectoryName())
    {
        return SEARCH_RESULT_SKIP;
    }
    DeleteDirectory(m_pDirectory);
    m_ImageInfo.SetExt("");
    m_ImageInfo.SetFileName("");
    wchar_t path[DIRECTORY_PATH_SIZE];
    int length = util::CreateImageRootDirectoryPath(path, DIRECTORY_PATH_SIZE, m_Storage, false);
    std::wcscpy(path + length, L"/");
    m_ImageInfo.GetDirectoryName(path + length + 1, DIRECTORY_PATH_SIZE - (length + 1));
    m_pDirectory = CreateDirectory(path, 6000);
    return SEARCH_RESULT_FOUND;
}

// 0x005A7674 | nintendogs:bytes [tier B]
imgdb::ImageSearcher::ImageSearcher()
    : m_pAllocator(NULL), m_Storage(STORAGE_TYPE_TWL_NAND), m_pRootDirectory(NULL), m_pDirectory(NULL)
{
    m_ImageInfo.Initialize();
}

// 0x005A76A4 | nintendogs:bytes [tier B]
imgdb::ImageSearcher::~ImageSearcher()
{
    if (m_pAllocator == NULL)
    {
        return;
    }
    DeleteDirectory(m_pDirectory);
    DeleteDirectory(m_pRootDirectory);
    m_ImageInfo.Initialize();
    m_Storage = STORAGE_TYPE_TWL_NAND;
    m_pAllocator = NULL;
}

// 0x007554C0 | nintendogs:bytes-fuzzy [tier B]
bool imgdb::ImageSearcher::IsNintendoDirectoryName(int* no) const
{
    wchar_t name[DCF_NAME_LENGTH + 1];
    m_ImageInfo.GetDirectoryName(name, DCF_NAME_LENGTH + 1);
    if (std::wcslen(name) != DCF_NAME_LENGTH)
    {
        return false;
    }
    for (int i = 0; i < 3; i++)
    {
        if (s_DirectoryMaker[i] != name[3 + i])
        {
            return false;
        }
    }
    int value;
    if (!util::AToI(value, name + 6, 2))
    {
        return false;
    }
    if (no != NULL)
    {
        *no = value;
    }
    return true;
}

} // namespace imgdb
