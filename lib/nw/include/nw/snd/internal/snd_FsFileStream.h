#pragma once

#include "decomp.h"
#include "nw/io/io_FileStream.h"
#include "nw/io/io_FileStream_FilePosition.h"
#include "nw/snd/internal/snd_FileReader.h"

namespace nw {
namespace snd {
namespace internal {
// A file stream over a part of a file of the file system: an own FileReader for a path or a given one
// (member names are ours). Failures in try mode go to the callback.
// RTTI N2nw3snd8internal12FsFileStreamE @ 0x008D099C
// vtable 0x00903090 (vptr 0x00903098), offset_to_top 0, 22 entries
class FsFileStream : public ::nw::io::FileStream
{
public:
    // called with the result of a failed access
    typedef void (*ErrorCallback)(nn::Result result, void* pArg); // (name is ours)

    FsFileStream(const char* pPath, u32 offset, u32 size, bool isTry, ErrorCallback pCallback, void* pCallbackArg); // 0x004C6E64 | fefates:bytes [tier B]
    FsFileStream(FileReader* pFileReader, u32 offset, u32 size, ErrorCallback pCallback, void* pCallbackArg); // 0x004C6FEC | fefates:bytes [tier B]
    virtual ~FsFileStream(); // 0x004C721C slot 0x04

    virtual bool IsAvailable() const { return true; } // 0x0073F3D4 slot 0x0C
    virtual bool vf_0x10() const { return false; } // 0x0073F3F4 slot 0x10
    virtual bool vf_0x14() const { return false; } // 0x0073F3EC slot 0x14
    virtual s32 Read(void* pBuffer, u32 size); // 0x004C6C74 slot 0x24 | fefates:bytes
    virtual void Close(); // 0x004C6E14 slot 0x38 | fefates:bytes
    virtual u32 GetSize() const { return m_Size; } // 0x0073F3E4 slot 0x3C
    virtual void Seek(s32 offset, u32 origin); // 0x004C6DB0 slot 0x40 | fefates:bytes
    virtual bool CanSeek() const { return true; } // 0x0073F3DC slot 0x4C (name after the other streams)
    virtual bool vf_0x50() const { return false; } // 0x0073F3FC slot 0x50
    virtual u32 Tell() const { return m_FilePosition.m_Position - m_Offset; } // 0x0073F3C4 slot 0x54 (name after the other streams)

    bool SetPriority(FileReader::Priority priority); // 0x004C6BF4 | fefates:bytes [tier B]

    // (inline) the result of the last access to the callback
    void CallErrorCallback()
    {
        if (m_pCallback != nullptr) {
            m_pCallback(m_pFileReader->m_Result, m_pCallbackArg);
        }
    }

    FileReader m_FileReader;                     // 0x14 (with a path)
    FileReader* m_pFileReader;                   // 0x34
    nw::io::FileStream::FilePosition m_FilePosition; // 0x38 in the file
    u32 m_Offset;                                // 0x40 the start of the part
    u32 m_Size;                                  // 0x44 the size of the part
    ErrorCallback m_pCallback;                   // 0x48
    void* m_pCallbackArg;                        // 0x4C
    bool m_IsOpened;                             // 0x50
    bool m_IsOwnFileReader;                      // 0x51 m_FileReader is used (closed by Close)
    bool m_IsCloseOnDestroy;                     // 0x52
};
ASSERT_SIZE(FsFileStream, 0x54);
} // namespace internal
} // namespace snd
} // namespace nw
