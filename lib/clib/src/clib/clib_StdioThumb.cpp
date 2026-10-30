// The stream functions of the ARM C library (Thumb code in the original): the buffer handling of the streams
// (fputc/__flsbuf, flush, seek, ftell). The file operations below them are functions of the program that only
// return -1 (clib_Retarget.cpp). See clib_Stdio.h; all names except the C library functions are ours.

#include "decomp.h"
#include "clib/clib_Stdio.h"
#include "clib/clib_Thumb.h"

using namespace clib;

namespace {
// the values of errno (names are ours)
const int ERROR_DOMAIN = 1;     // (ftell of a stream that is not open)

// the results of __fseek
const int SEEK_DONE = 0;
const int SEEK_FAILED = 1;
const int SEEK_INVALID = 2;

inline u8* EndOfData(File* pStream)
{
    return pStream->m_pEnd > pStream->m_pCurrent ? pStream->m_pEnd : pStream->m_pCurrent;
}
} // namespace

extern "C" {

int* __aeabi_errno_addr();
void* malloc(size_t size);
int __stdio_is_terminal(int handle);
int __stdio_file_length(int handle);
int __stdio_seek(int handle, s32 position);
int __stdio_write_file(int handle, const u8* pData, int size, u32 flags);

// into the buffer while there is room, else through __flsbuf
// 0x0011C1AC (name is ours, after the C standard)
int fputc(int c, File* pStream)
{
    if (pStream->m_Available <= 0) {
        return __flsbuf(c, pStream);
    }
    pStream->m_Available--;
    u8 byte = c;
    *pStream->m_pCurrent++ = byte;
    return byte;
}

// 0x001032EA (name is ours, after the C standard)
int ferror(File* pStream)
{
    return pStream->m_Flags & FILE_ERROR;
}

// the stream is unusable until it is reopened
// 0x002F99D0 (name is ours)
void __stdio_set_error(File* pStream)
{
    pStream->m_Flags = (pStream->m_Flags | FILE_ERROR) & ~0x200000;
    pStream->m_Available = 0;
    pStream->m_ReadAvailable = 0;
}

// writes size bytes at the position of the buffer (seeks first when it was moved); 0 or -1
// 0x002F99E4 (name is ours)
int __stdio_write(const u8* pData, int size, File* pStream)
{
    u32 flags = pStream->m_Flags;
    int handle = pStream->m_Handle;
    if ((flags & (FILE_SEEKED | FILE_SEEK_BEFORE_WRITE)) != 0) {
        if (__stdio_seek(handle, pStream->m_BufferPosition) < 0) {
            __stdio_set_error(pStream);
            return -1;
        }
        flags &= ~(FILE_SEEKED | FILE_SEEK_BEFORE_WRITE);
        pStream->m_Flags = flags;
    }
    // (the result: the bytes not written, bit 31 set on an error)
    int notWritten = __stdio_write_file(handle, pData, size, flags);
    pStream->m_BufferPosition += size - (notWritten & 0x7FFFFFFF);
    if (notWritten != 0) {
        __stdio_set_error(pStream);
        return -1;
    }
    return notWritten;
}

// writes the bytes of the buffer that are not written yet; 0 or -1
// 0x002F9A30 (name is ours)
int __stdio_flush(File* pStream)
{
    u8* pBuffer = pStream->m_pBuffer;
    u8* pEnd = EndOfData(pStream);
    pStream->m_Flags &= ~(FILE_ABORT_WRITE | 0x80000);
    if ((pStream->m_Flags & FILE_DIRTY) != 0) {
        if (pEnd != pBuffer && __stdio_write(pBuffer, pEnd - pBuffer, pStream) != 0) {
            return -1;
        }
        pStream->m_pEnd = pBuffer;
        pStream->m_pCurrent = pBuffer;
        pStream->m_Available = 0;
        pStream->m_Flags &= ~FILE_DIRTY;
    }
    return 0;
}

// carries out a seek that fseek only noted: the buffer moves to the new position
// 0x002F9A7C (name is ours)
void __stdio_apply_seek(File* pStream)
{
    pStream->m_Flags &= ~FILE_SEEK_PENDING;
    if (pStream->m_BufferPosition != pStream->m_SeekPosition) {
        __stdio_flush(pStream);
        pStream->m_Flags = (pStream->m_Flags & ~(0x1000 | FILE_WRITING)) | FILE_SEEKED;
        pStream->m_BufferPosition = pStream->m_SeekPosition;
        pStream->m_pEnd = pStream->m_pBuffer;
        pStream->m_pCurrent = pStream->m_pBuffer;
    }
    pStream->m_Flags &= ~(0x40 | 0x4000 | FILE_ABORT_WRITE | 0x80000);
}

// the position of the next byte; -1 when the stream is not open
// 0x0010143C (name is ours, after the C standard)
long ftell(File* pStream)
{
    u32 flags = pStream->m_Flags;
    if ((flags & (FILE_READ | FILE_WRITE)) == 0) {
        *__aeabi_errno_addr() = ERROR_DOMAIN;
        return -1;
    }
    long position;
    if ((flags & FILE_SEEK_PENDING) != 0) {
        position = pStream->m_SeekPosition;
    } else {
        position = reinterpret_cast<s32>(pStream->m_pCurrent) + pStream->m_BufferPosition -
                   reinterpret_cast<s32>(pStream->m_pBuffer);
    }
    if (pStream->m_HasPushback != 0) {
        return position - pStream->m_PushbackCount;
    }
    if ((flags & FILE_TEXT_POSITION) != 0 && position > 0) {
        position--;
    }
    return position;
}

// the work of fseek (whence 0 start, 1 current position, 2 end): a position inside the valid bytes of the buffer
// only moves m_pCurrent, any other is noted for the next access
// 0x001022AC (name is ours)
int __fseek(File* pStream, long offset, int whence)
{
    int handle = pStream->m_Handle;
    if ((pStream->m_Flags & (FILE_READ | FILE_WRITE)) == 0 || __stdio_is_terminal(handle) != 0) {
        return SEEK_INVALID;
    }
    switch (whence) {
    case 0:
        break;
    case 1:
        offset += ftell(pStream);
        break;
    case 2: {
        int length = __stdio_file_length(handle);
        pStream->m_Flags |= FILE_SEEKED;
        if (length < 0) {
            __stdio_set_error(pStream);
            return SEEK_FAILED;
        }
        if ((pStream->m_Flags & FILE_DIRTY) != 0) {
            // the bytes in the buffer may make the file longer
            int written = reinterpret_cast<s32>(EndOfData(pStream)) + pStream->m_BufferPosition -
                          reinterpret_cast<s32>(pStream->m_pBuffer);
            if (written > length) {
                length = written;
            }
        }
        offset += length;
        break;
    }
    default:
        return SEEK_INVALID;
    }
    if (offset < 0) {
        return SEEK_INVALID;
    }
    u32 flags = pStream->m_Flags;
    if (pStream->m_pEnd < pStream->m_pCurrent) {
        pStream->m_pEnd = pStream->m_pCurrent;
        if ((flags & FILE_SEEK_BEFORE_WRITE) != 0) {
            flags = (flags & ~FILE_SEEK_BEFORE_WRITE) | FILE_SEEKED;
        }
    }
    s32 bufferPosition = pStream->m_BufferPosition;
    u8* pBuffer = pStream->m_pBuffer;
    if (bufferPosition <= offset &&
        reinterpret_cast<s32>(EndOfData(pStream)) + bufferPosition - reinterpret_cast<s32>(pBuffer) > offset &&
        EndOfData(pStream) != pBuffer) {
        s32 inBuffer = offset - bufferPosition;
        pStream->m_Available = inBuffer - pStream->m_BufferSize;
        pStream->m_ReadAvailable = inBuffer - (EndOfData(pStream) - pBuffer);
        pStream->m_pCurrent = pBuffer + inBuffer;
        flags &= ~FILE_SEEK_PENDING;
    } else {
        flags |= FILE_SEEK_PENDING;
        pStream->m_Available = 0;
        pStream->m_SeekPosition = offset;
        pStream->m_ReadAvailable = 0;
    }
    pStream->m_Flags = flags & ~(0x40 | 0x1000 | FILE_WRITING | FILE_ABORT_WRITE | 0x80000);
    pStream->m_HasPushback = 0;
    return SEEK_DONE;
}

// fputc when the buffer has no room: gets a buffer (malloc, or the single byte of the stream), stores c and writes
// the buffer as the buffering mode asks; c, -1 on an error, -2 when the write is abandoned
// 0x001037E8 | manual:ours [tier B]
int __flsbuf(int c, File* pStream)
{
    pStream->m_Flags = (pStream->m_Flags & ~(FILE_ABORT_WRITE | 0x80000)) | FILE_WRITTEN;
    if ((pStream->m_Flags & FILE_SEEK_PENDING) != 0) {
        __stdio_apply_seek(pStream);
    }
    u32 flags = pStream->m_Flags;
    if ((flags & (FILE_WRITE | FILE_ERROR | 0x1000)) != FILE_WRITE) {
        __stdio_set_error(pStream);
        return -1;
    }
    if ((flags & (FILE_APPEND | FILE_WRITING)) == FILE_APPEND) {
        // the first write of an append stream goes to the end of the file
        if ((flags & FILE_DIRTY) != 0) {
            pStream->m_pCurrent = EndOfData(pStream);
        } else {
            pStream->m_pEnd = pStream->m_pBuffer;
            pStream->m_pCurrent = pStream->m_pBuffer;
            pStream->m_BufferPosition = __stdio_file_length(pStream->m_Handle);
        }
    }
    s32 available = pStream->m_Available;
    u8 byte = c;
    if (available < 0 && (pStream->m_Flags & FILE_LINE_BUFFERING) == 0) {
        // after a seek into the buffer: the room behind the position
        pStream->m_Available = ~available;
        pStream->m_ReadAvailable = 0;
        pStream->m_Flags |= FILE_WRITING | FILE_DIRTY;
        *pStream->m_pCurrent++ = byte;
        return byte;
    }
    pStream->m_Flags |= FILE_WRITING;
    pStream->m_ReadAvailable = 0;
    if (pStream->m_pBuffer == NULL) {
        bool isUnbuffered;
        if (__stdio_is_terminal(pStream->m_Handle) != 0) {
            // a terminal gets a buffer only when a buffering mode was set
            isUnbuffered = true;
            if ((pStream->m_Flags & (FILE_FULL_BUFFERING | FILE_LINE_BUFFERING)) != 0) {
                pStream->m_pBuffer = static_cast<u8*>(malloc(pStream->m_BufferSize));
                if (pStream->m_pBuffer != NULL) {
                    pStream->m_Flags |= FILE_BUFFER_ALLOCATED;
                    isUnbuffered = false;
                }
            }
        } else {
            pStream->m_pBuffer = static_cast<u8*>(malloc(pStream->m_BufferSize));
            isUnbuffered = pStream->m_pBuffer == NULL;
            if (!isUnbuffered) {
                pStream->m_Flags |= FILE_BUFFER_ALLOCATED;
                if ((pStream->m_Flags & (FILE_FULL_BUFFERING | FILE_LINE_BUFFERING)) == 0) {
                    pStream->m_Flags |= FILE_FULL_BUFFERING;
                }
            }
        }
        if (isUnbuffered) {
            pStream->m_BufferSize = 1;
            pStream->m_pBuffer = pStream->m_SingleByte;
            pStream->m_Flags = (pStream->m_Flags & ~(FILE_FULL_BUFFERING | FILE_LINE_BUFFERING)) | FILE_UNBUFFERED;
        }
        pStream->m_pCurrent = pStream->m_pBuffer;
    }

    flags = pStream->m_Flags;
    u8* pBuffer = pStream->m_pBuffer;
    if ((flags & FILE_FULL_BUFFERING) != 0) {
        // write the full buffer, then start it again with c
        u8* pEnd = EndOfData(pStream);
        if (pEnd - pBuffer != 0 && __stdio_write(pBuffer, pEnd - pBuffer, pStream) != 0) {
            return -1;
        }
        if ((pStream->m_Flags & FILE_ABORT_WRITE) != 0) {
            pStream->m_pEnd = pBuffer;
            pStream->m_pCurrent = pBuffer;
            pStream->m_Available = 0;
            return -2;
        }
        pStream->m_pEnd = pBuffer + 1;
        pStream->m_pCurrent = pBuffer + 1;
        pStream->m_Available = pStream->m_BufferSize - 1;
        pStream->m_Flags |= FILE_DIRTY;
        *pBuffer = byte;
        return byte;
    }

    // line buffered or unbuffered: store c, write the buffer at a new line, when it is full or after every byte
    u32 isNewLine;
    if ((flags & FILE_ABORT_WRITE) != 0) {
        isNewLine = flags & FILE_DIRTY;
    } else {
        *pStream->m_pCurrent++ = byte;
        pStream->m_Flags |= FILE_DIRTY;
        isNewLine = byte == 10;
    }
    int length = EndOfData(pStream) - pBuffer;
    if ((pStream->m_Flags & FILE_UNBUFFERED) != 0 || pStream->m_BufferSize <= length || isNewLine != 0) {
        int ahead = EndOfData(pStream) - pStream->m_pCurrent;
        pStream->m_pEnd = pBuffer;
        pStream->m_pCurrent = pBuffer;
        pStream->m_Available = 0;
        if (length > 0 && __stdio_write(pBuffer, length, pStream) != 0) {
            return -1;
        }
        if (ahead != 0) {
            // the position was before the end of the bytes: it moves back there
            pStream->m_pEnd = pBuffer;
            pStream->m_pCurrent = pBuffer;
            pStream->m_BufferPosition -= ahead;
            pStream->m_Flags |= FILE_SEEKED;
        }
    }
    if ((pStream->m_Flags & FILE_ABORT_WRITE) != 0) {
        pStream->m_BufferPosition += pStream->m_pCurrent - pBuffer;
        pStream->m_pEnd = pBuffer;
        pStream->m_pCurrent = pBuffer;
        return -2;
    }
    return byte;
}

} // extern "C"
