#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_DisposeCallback.h"
#include "nw/snd/internal/snd_SoundArchiveLoader.h"

namespace nw {
namespace snd {
namespace internal {
template <typename T>
struct BinaryTable;
} // namespace internal

// The loader of the game: a table of the addresses of all files of the archive in memory supplied by the
// game (the class and function names are from the reference symbols, the members are ours).
// RTTI N2nw3snd16SoundDataManagerE @ 0x008D0908
// vtable 0x00902F3C (vptr 0x00902F44), offset_to_top 0, 6 entries
// vtable 0x00902F5C (vptr 0x00902F64), offset_to_top -12, 5 entries
class SoundDataManager : public ::nw::snd::internal::driver::DisposeCallback, public ::nw::snd::internal::SoundArchiveLoader
{
public:
    // asked first for the address of a file (name is ours; nothing in the game sets it)
    class FileManager
    {
    public:
        virtual ~FileManager() {}
        virtual const void* GetFileAddress(u32 fileId) = 0; // slot 0x08
    };

    SoundDataManager(); // 0x00137C74 | nintendogs:bytes [tier A]
    virtual ~SoundDataManager(); // 0x004C089C slot 0x00 | fefates:bytes

    // the files in the memory go away
    virtual void InvalidateData(const void* pStart, const void* pEnd); // 0x004C06B8 slot 0x08 | nintendogs:callseq
    virtual const void* SetFileAddressToTable(u32 fileId, const void* pAddress); // 0x004C0800 slot 0x0C | mk7dlp:bytes
    virtual const void* GetFileAddressFromTable(u32 fileId) const; // 0x0073EE84 slot 0x10 | nintendogs:bytes
    virtual const void* GetFileAddressImpl(u32 fileId) const; // 0x0073EDF4 slot 0x14 | fefates:bytes

    // the table in the buffer (GetRequiredMemSize bytes); false if it is too small
    bool Initialize(const SoundArchive* pSoundArchive, void* pBuffer, u32 bufferSize); // 0x004C05AC | nintendogs:bytes-fuzzy [tier A]
    void Finalize(); // 0x004C0824 | nintendogs:callseq [tier A]
    u32 GetRequiredMemSize(const SoundArchive* pSoundArchive) const; // 0x0073EE58 | nintendogs:bytes [tier A]
    const void* detail_GetFileAddress(u32 fileId) const; // 0x0073EE78 | nintendogs:callseq

    internal::BinaryTable<const void*>* m_pFileTable; // 0x218
    FileManager* m_pFileManager;                      // 0x21C
};
ASSERT_SIZE(SoundDataManager, 0x220);
} // namespace snd
} // namespace nw
