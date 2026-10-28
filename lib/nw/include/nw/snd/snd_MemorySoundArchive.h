#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/os/os_CriticalSection.h"
#include "nw/snd/internal/snd_SoundArchiveFileReader.h"
#include "nw/snd/snd_SoundArchive.h"

namespace nw {
namespace snd {
// A sound archive in memory; its external files come from the file system (the class and function names
// are from the reference symbols, the members are ours).
// RTTI N2nw3snd18MemorySoundArchiveE @ 0x008D0928
// vtable 0x00902F78 (vptr 0x00902F80), offset_to_top 0, 6 entries
class MemorySoundArchive : public ::nw::snd::SoundArchive
{
public:
    explicit MemorySoundArchive(bool isTry); // 0x004C0A74 (symbols.json: [ctor?] without the parameter)
    // 0x004C0AC8 (deleting dtor)
    virtual ~MemorySoundArchive(); // 0x004C0AEC slot 0x00

    virtual const void* detail_GetFileAddress(u32 fileId) const; // 0x0073EFC0 slot 0x08 | nintendogs:callseq
    virtual size_t detail_GetRequiredStreamBufferSize() const; // 0x0073F0FC slot 0x0C
    virtual io::FileStream* OpenStream(void* pBuffer, int bufferSize, u32 begin, u32 length); // 0x004C0A00 slot 0x10
    virtual io::FileStream* OpenExtStream(void* pBuffer, int bufferSize, const char* pExtFilePath, void* pCacheBuffer, u32 cacheSize,
                                          FileStreamPriority priority) const; // 0x0073EEB0 slot 0x14

    bool Initialize(const void* pSoundArchiveFile); // 0x004C0988 | nintendogs:callgraph [tier A]
    // the archive is no longer available (name is ours)
    void Finalize(); // 0x004C02C8
    // the file streams report their errors (name after FsSoundArchiveBase)
    static void FsErrorCallbackHandler(nn::Result result, void* pArg); // 0x004C0A44

    const void* m_pData;                                  // 0x11C
    internal::SoundArchiveFileReader m_ArchiveFileReader; // 0x120
    nn::Result m_LastFsResult;                            // 0x164
    nn::os::CriticalSection m_ResultLock;                 // 0x168
    bool m_IsTry;                                         // 0x174
    u8 padding0x175[3];                                   // 0x175
};
ASSERT_SIZE(MemorySoundArchive, 0x178);
} // namespace snd
} // namespace nw
