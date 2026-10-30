#pragma once

// The streams of the C library (names are ours, from the code that uses them). A stream has a buffer that holds the
// bytes of the file from m_BufferPosition on; m_pCurrent is the next byte, m_pEnd the end of the valid bytes.

#include "decomp.h"
#include "types.h"

namespace clib {
// the flags of a stream
const u32 FILE_READ = 0x1;
const u32 FILE_WRITE = 0x2;
const u32 FILE_SEEKED = 0x10;               // m_BufferPosition was moved: the next write seeks first
const u32 FILE_SEEK_PENDING = 0x20;         // fseek only noted m_SeekPosition
const u32 FILE_ERROR = 0x80;
const u32 FILE_FULL_BUFFERING = 0x100;      // written when the buffer is full
const u32 FILE_LINE_BUFFERING = 0x200;      // ... also at '\n'
const u32 FILE_UNBUFFERED = 0x400;          // ... after every byte
const u32 FILE_BUFFER_ALLOCATED = 0x800;    // the buffer came from malloc
const u32 FILE_WRITING = 0x2000;            // the last operation was a write
const u32 FILE_APPEND = 0x8000;
const u32 FILE_DIRTY = 0x10000;             // the buffer holds bytes that are not written yet
const u32 FILE_SEEK_BEFORE_WRITE = 0x20000;
const u32 FILE_TEXT_POSITION = 0x80000;     // ftell counts one byte less
const u32 FILE_WRITTEN = 0x400000;          // (set by __flsbuf)
const u32 FILE_ABORT_WRITE = 0x800000;      // __flsbuf drops the buffer and returns -2

struct File
{
    s32 m_ReadAvailable;    // 0x0 (bytes left to read in the buffer)
    u8* m_pCurrent;         // 0x4 (the next byte of the buffer)
    s32 m_Available;        // 0x8 (bytes left to write in the buffer; negative: ~room after a seek into it)
    u32 m_Flags;            // 0xC
    u8* m_pBuffer;          // 0x10
    int m_Handle;           // 0x14
    s32 m_BufferPosition;   // 0x18 (the position of the buffer in the file)
    s32 m_BufferSize;       // 0x1C
    u32 _unknown0x20;
    u8 m_SingleByte[4];     // 0x24 (the buffer of an unbuffered stream)
    s32 m_SeekPosition;     // 0x28 (FILE_SEEK_PENDING)
    u8* m_pEnd;             // 0x2C (the end of the valid bytes of the buffer, if behind m_pCurrent)
    u8 _unknown0x30[0x14];
    u8 m_HasPushback;       // 0x44 (ungetc)
    u8 m_PushbackCount;     // 0x45
    u8 _unknown0x46[6];
    uptr m_Next;            // 0x4C (the next stream; bit 0: a static stream)
};
ASSERT_SIZE(File, 0x50);
} // namespace clib

extern "C" {
int fputc(int c, clib::File* pStream);
int ferror(clib::File* pStream);
long ftell(clib::File* pStream);
int __flsbuf(int c, clib::File* pStream);
}
