#pragma once

#include "imgdb/Directory.h"
#include "imgdb/ImageInfo.h"

namespace imgdb {

// walks the DCF directories of a storage and their image files, one entry per call (member names are ours)
class ImageSearcher
{
public:
    enum SearchResult
    {
        SEARCH_RESULT_END,    // no more entries
        SEARCH_RESULT_FOUND,  // m_ImageInfo has the entry
        SEARCH_RESULT_SKIP,   // an entry that is no DCF directory or image
    };

    ImageSearcher();
    ~ImageSearcher();

    void Initialize(Allocator& allocator, StorageType storage);
    // the next image file of the current directory
    SearchResult SearchFile();
    // the next DCF directory, its files are searched next
    SearchResult SearchDirectory();
    // a directory "nnnNINxx" of a Nintendo console; xx
    bool IsNintendoDirectoryName(int* no) const;

private:
    void DeleteDirectory(Directory*& directory)
    {
        if (directory != NULL)
        {
            Allocator* allocator = m_pAllocator;
            directory->~Directory();
            allocator->Free(directory);
            directory = NULL;
        }
    }
    Directory* CreateDirectory(const wchar_t* path, int maxEntryNum)
    {
        void* buffer = m_pAllocator->Allocate(sizeof(Directory), 4);
        Directory* directory = buffer != NULL ? new (buffer) Directory() : NULL;
        directory->Initialize(*m_pAllocator, m_Storage, path, maxEntryNum);
        return directory;
    }

    Allocator* m_pAllocator;        // +0x00
    StorageType m_Storage;          // +0x04
    Directory* m_pRootDirectory;    // +0x08
    Directory* m_pDirectory;        // +0x0C, the current DCF directory

public:
    ImageInfo m_ImageInfo;          // +0x10
};
ASSERT_SIZE(ImageSearcher, 0x60);

} // namespace imgdb
