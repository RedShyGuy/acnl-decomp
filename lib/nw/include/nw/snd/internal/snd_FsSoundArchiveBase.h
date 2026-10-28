#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/os/os_CriticalSection.h"
#include "nw/snd/internal/snd_FileReader.h"
#include "nw/snd/internal/snd_SoundArchiveFileReader.h"
#include "nw/snd/snd_SoundArchive.h"

namespace nw {
namespace snd {
namespace internal {
// A sound archive in the file system: the header in memory, the rest read through file streams (the class
// and function names are from the reference symbols, the members are ours).
// RTTI N2nw3snd8internal18FsSoundArchiveBaseE @ 0x008D0A08
// vtable 0x00903258 (vptr 0x00903260), offset_to_top 0, 6 entries
class FsSoundArchiveBase : public ::nw::snd::SoundArchive
{
public:
    // the bytes read for the header of the file
    static const int HEADER_SIZE = 64; // (name is ours)

    explicit FsSoundArchiveBase(bool isTry); // 0x004C8948 | fefates:bytes [tier B]
    // 0x004C89D8 (deleting dtor)
    virtual ~FsSoundArchiveBase(); // 0x004C8A60 slot 0x00 | fefates:bytes

    virtual const void* detail_GetFileAddress(u32 fileId) const; // 0x007406E8 slot 0x08
    virtual size_t detail_GetRequiredStreamBufferSize() const; // 0x007406F0 slot 0x0C
    virtual io::FileStream* OpenStream(void* pBuffer, int bufferSize, u32 begin, u32 length); // 0x004C867C slot 0x10 | fefates:bytes
    virtual io::FileStream* OpenExtStream(void* pBuffer, int bufferSize, const char* pExtFilePath, void* pCacheBuffer, u32 cacheSize,
                                          FileStreamPriority priority) const; // 0x007405CC slot 0x14 | fefates:bytes

    // opens the archive and reads its header; the directory of the archive is the root of the external files
    bool Open(const char* pPath); // 0x004C87C4 | fefates:bytes [tier B]
    void Close(); // 0x004C8910 | fefates:bytes-fuzzy [tier B]
    // the file streams report their errors
    static void FsErrorCallbackHandler(nn::Result result, void* pArg); // 0x004C8794 | fefates:bytes [tier B]

    FileReader m_FileReader;                    // 0x11C
    SoundArchiveFileReader m_ArchiveFileReader; // 0x13C
    nn::Result m_LastFsResult;                  // 0x180
    nn::os::CriticalSection m_ResultLock;       // 0x184
    bool m_IsOpened;                            // 0x190
    bool m_IsTry;                               // 0x191 the file system errors are not fatal
    u8 padding0x192[2];                         // 0x192
};
ASSERT_SIZE(FsSoundArchiveBase, 0x194);
} // namespace internal
} // namespace snd
} // namespace nw
