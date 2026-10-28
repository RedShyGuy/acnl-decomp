#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_DisposeCallback.h"
#include "nw/snd/internal/snd_SoundArchiveLoader.h"

namespace nw {
namespace snd {
namespace internal {
// The loader of the data of one sound into a PlayerHeap: a small table of the loaded files (the class and
// function names are from the reference symbols, the members are ours).
// RTTI N2nw3snd8internal21PlayerHeapDataManagerE @ 0x008D0A34
// vtable 0x009032BC (vptr 0x009032C4), offset_to_top 0, 6 entries
// vtable 0x009032DC (vptr 0x009032E4), offset_to_top -12, 5 entries
class PlayerHeapDataManager : public ::nw::snd::internal::driver::DisposeCallback, public ::nw::snd::internal::SoundArchiveLoader
{
public:
    // a loaded file (names are ours)
    struct FileAddress
    {
        u32 fileId;           // 0x0 (-1: free)
        const void* address;  // 0x4
    };
    // the files of one sound
    static const int FILE_ADDRESS_COUNT = 9; // (name is ours)

    PlayerHeapDataManager(); // 0x004C9444 | nintendogs:bytes [tier A]
    virtual ~PlayerHeapDataManager(); // 0x004C94E0 slot 0x00 | fefates:bytes

    // the heap is cleared: all files go away
    virtual void InvalidateData(const void* pStart, const void* pEnd); // 0x004C9378 slot 0x08 | nintendogs:bytes
    virtual const void* SetFileAddressToTable(u32 fileId, const void* pAddress); // 0x004C93AC slot 0x0C
    virtual const void* GetFileAddressFromTable(u32 fileId) const; // 0x0074098C slot 0x10
    virtual const void* GetFileAddressImpl(u32 fileId) const; // 0x00740980 slot 0x14

    void Initialize(const SoundArchive* pSoundArchive); // 0x004C8ADC | nintendogs:bytes [tier A]
    void Finalize(); // 0x004C9418 | nintendogs:bytes [tier A]

    FileAddress m_FileAddressTable[FILE_ADDRESS_COUNT]; // 0x218
    bool m_IsInitialized;                               // 0x260
    bool m_IsFinalized;                                 // 0x261
};
ASSERT_SIZE(PlayerHeapDataManager, 0x264);
} // namespace internal
} // namespace snd
} // namespace nw
