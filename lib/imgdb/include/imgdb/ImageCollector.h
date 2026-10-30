#pragma once

#include "imgdb/ImageCollectionTable.h"
#include "imgdb/ImageInfo.h"

namespace imgdb {

class ImageSearcher;

// collects the image files of a storage into an ImageCollectionTable step by step: checks the known records, then
// searches the directories of Nintendo consoles and the others (names are ours except the ones of the functions)
class ImageCollector
{
public:
    enum ProcessType : u8
    {
        PROCESS_TYPE_IDLE,
        PROCESS_TYPE_CLEANUP,
        PROCESS_TYPE_COLLECT_OFFICIAL,
        PROCESS_TYPE_COLLECT_UNOFFICIAL,
        PROCESS_TYPE_NUM,
    };

    typedef void (ImageCollector::*ProcessFunc)();

    // the exit function of the current process, then the enter function of the new one
    void ChangeProcess(ProcessType type);
    void UpdateProcessCleanup();
    void UpdateProcessCollectOfficial();
    void UpdateProcessCollectUnofficial();
    // the record of the same directory and file name that is known or new, -1 without one
    int FindSameInfoInTable(const ImageInfo& info) const;

private:
    void ExitProcessIdle();
    void EnterProcessIdle();
    void ExitProcessCleanup();
    void UpdateProcessIdle();
    void EnterProcessCleanup();
    void ExitProcessCollectOfficial();
    void EnterProcessCollectOfficial();
    void ExitProcessCollectUnofficial();
    void EnterProcessCollectUnofficial();
    // a new record for the image of the searcher (or the extension of the JPEG record it replaces)
    void CollectImage(bool isOfficial);

    static ProcessFunc s_EnterProcess[PROCESS_TYPE_NUM];
    static ProcessFunc s_ExitProcess[PROCESS_TYPE_NUM];
    static ProcessFunc s_UpdateProcess[PROCESS_TYPE_NUM];

    Allocator* m_pAllocator;      // +0x00
    StorageType m_Storage;        // +0x04
    ImageCollectionTable m_Table;  // +0x08
    bool m_IsChanged;             // +0x14
    ProcessType m_Process;        // +0x15
    int m_Index;                  // +0x18, the record of the cleanup
    ImageSearcher* m_pSearcher;   // +0x1C
    bool m_IsDirectoryOpen;       // +0x20, the searcher walks the files of a directory
};
ASSERT_SIZE(ImageCollector, 0x24);

} // namespace imgdb
