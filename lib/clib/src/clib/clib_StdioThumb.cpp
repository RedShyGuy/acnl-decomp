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
int __stdio_open(const char* pName, u32 mode);
int __stdio_close(int handle);
void __raise_signal(int signal, const char* pMessage);
void free(void* pMemory);
void __rt_memclr_w(void* destination, size_t size);
int __fseek(File* pStream, long offset, int whence);
int __stdio_flush(File* pStream);

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
DECOMP_NOINLINE void __stdio_set_error(File* pStream)
{
    pStream->m_Flags = (pStream->m_Flags | FILE_ERROR) & ~0x200000;
    pStream->m_Available = 0;
    pStream->m_ReadAvailable = 0;
}

// writes size bytes at the position of the buffer (seeks first when it was moved); 0 or -1
// 0x002F99E4 (name is ours)
DECOMP_NOINLINE int __stdio_write(const u8* pData, int size, File* pStream)
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
DECOMP_NOINLINE int __stdio_flush(File* pStream)
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
DECOMP_NOINLINE void __stdio_apply_seek(File* pStream)
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
    pStream->m_Flags = (pStream->m_Flags & ~(FILE_ABORT_WRITE | 0x80000)) | FILE_ACCESSED;
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

// ---- opening and closing

// the standard streams; further streams are allocated and linked behind them
// 0x00AF86DC
File __stdin;
// 0x00AF872C
File __stdout;
// 0x00AF877C
File __stderr;

// 0x002F8224 (name is ours, after the C standard)
int fclose(File* pStream)
{
    int handle = pStream->m_Handle;
    u8* pBuffer = pStream->m_pBuffer;
    u32 flags = pStream->m_Flags;
    int result = 0;
    if ((flags & (FILE_READ | FILE_WRITE)) == 0) {
        return -1;
    }
    __stdio_flush(pStream);
    if (__stdio_close(handle) < 0) {
        return -1;
    }
    if ((flags & FILE_BUFFER_ALLOCATED) != 0) {
        free(pBuffer);
    }
    // (all but the link to the next stream)
    __rt_memclr_w(pStream, 0x48);
    pStream->m_Next &= ~1;
    return result;
}

// mode: "r", "w" or "a", then '+' (read and write), 'b' (binary) or 't'
// 0x002F8418 (name is ours, after the C standard)
File* freopen(const char* pName, const char* pMode, File* pStream)
{
    fclose(pStream);
    u32 flags;
    u32 openMode;   // (for __stdio_open: 1 binary, 2 update, 4 write, 8 append, 16 text)
    switch (*pMode++) {
    case 'r':
        flags = FILE_READ;
        openMode = 0;
        break;
    case 'w':
        flags = FILE_WRITE;
        openMode = 4;
        break;
    case 'a':
        flags = FILE_APPEND | FILE_WRITE;
        openMode = 8;
        break;
    default:
        return NULL;
    }
    for (;;) {
        char c = *pMode++;
        if (c == '+') {
            flags |= FILE_READ | FILE_WRITE;
            openMode |= 2;
        } else if (c == 'b') {
            flags |= FILE_BINARY;
            openMode |= 1;
        } else {
            if (c == 't') {
                openMode |= 16;
            }
            break;
        }
    }
    int handle = __stdio_open(pName, openMode);
    if (handle == -1) {
        return NULL;
    }
    pStream->m_pBuffer = NULL;
    pStream->m_pCurrent = NULL;
    pStream->m_Flags = flags;
    pStream->m_BufferSize = 0x200;
    pStream->m_Handle = handle;
    if ((openMode & 8) != 0) {
        __fseek(pStream, 0, 2);
    }
    pStream->m_Next |= 1;
    return pStream;
}

// the first stream of the list that is not open, else a new one at the end of the list
// 0x002F84AC (name is ours, after the C standard)
File* fopen(const char* pName, const char* pMode)
{
    File* pStream = &__stdin;
    for (;;) {
        uptr next = pStream->m_Next;
        if ((next & 1) == 0) {
            break;
        }
        if ((next >> 1) == 0) {
            File* pNew = static_cast<File*>(malloc(sizeof(File)));
            if (pNew == NULL) {
                return NULL;
            }
            pStream->m_Next |= (reinterpret_cast<uptr>(pNew) & ~1) | 1;
            __rt_memclr_w(pNew, sizeof(File));
            pStream = pNew;
            break;
        }
        pStream = reinterpret_cast<File*>(next & ~1);
    }
    return freopen(pName, pMode, pStream);
}

// mode: FILE_FULL_BUFFERING or FILE_LINE_BUFFERING with a buffer of 1 to 2^24 - 1 bytes, or FILE_UNBUFFERED; only
// before the first access; 0 or 1
// 0x002F9B98 (name is ours, after the C standard)
int setvbuf(File* pStream, char* pBuffer, int mode, size_t size)
{
    u32 flags = pStream->m_Flags;
    if ((flags & (FILE_READ | FILE_WRITE)) == 0 || (flags & FILE_ACCESSED) != 0) {
        return 1;
    }
    switch (mode) {
    case FILE_FULL_BUFFERING:
    case FILE_LINE_BUFFERING:
        if (size - 1 >= 0xFFFFFF) {
            return 1;
        }
        break;
    case FILE_UNBUFFERED:
        pBuffer = reinterpret_cast<char*>(pStream->m_SingleByte);
        size = 1;
        break;
    default:
        return 1;
    }
    pStream->m_pBuffer = reinterpret_cast<u8*>(pBuffer);
    pStream->m_BufferSize = size;
    pStream->m_pCurrent = reinterpret_cast<u8*>(pBuffer);
    pStream->m_Flags = (flags & ~(FILE_FULL_BUFFERING | FILE_LINE_BUFFERING | FILE_UNBUFFERED | FILE_BUFFER_ALLOCATED)) | mode;
    return 0;
}

// (a failure while the standard streams are set up)
// 0x002F9B8C (name is ours)
void __stdio_raise_error(const char* pMessage)
{
    __raise_signal(8, pMessage);
}

// at the end of the program: closes the standard streams and closes and frees the others
// 0x00102478 (name is ours)
void __stdio_exit()
{
    File* pStream = reinterpret_cast<File*>(__stderr.m_Next & ~1);
    fclose(&__stdin);
    fclose(&__stdout);
    fclose(&__stderr);
    while (pStream != NULL) {
        File* pNext = reinterpret_cast<File*>(pStream->m_Next & ~1);
        fclose(pStream);
        free(pStream);
        pStream = pNext;
    }
}

// pointers to the standard streams
// 0x00982FAC (name is ours)
File* s_pStandardInput;
// 0x00982FB0 (name is ours)
File* s_pStandardOutput;
// 0x00982FB4 (name is ours)
File* s_pStandardError;

// opens the standard streams (on the terminal the program does not have, so __stdio_open fails) and makes them line
// buffered. The arguments of __stdio_raise_error point into the merged string pool right in front of the empty
// names; what the strings were meant to be is unknown, they are written as the bytes found there.
// 0x001023A0 (name is ours)
void __stdio_initialize()
{
    __rt_memclr_w(&__stdin, sizeof(File));
    __rt_memclr_w(&__stdout, sizeof(File));
    __rt_memclr_w(&__stderr, sizeof(File));
    s_pStandardInput = &__stdin;
    s_pStandardOutput = &__stdout;
    s_pStandardError = &__stderr;
    __stdin.m_Next = reinterpret_cast<uptr>(&__stdout) + 1;
    __stdout.m_Next = reinterpret_cast<uptr>(&__stderr) + 1;
    __stderr.m_Next = 1;
    if (freopen("", "r", &__stdin) == NULL) {
        __stdio_raise_error("s://");
    }
    if (freopen("", "w", &__stdout) == NULL) {
        __stdio_raise_error("://");
    }
    if (freopen("", "w", &__stderr) == NULL) {
        __stdio_raise_error("//");
    }
    if (setvbuf(&__stdin, NULL, FILE_LINE_BUFFERING, 64) != 0) {
        __stdio_raise_error("\xFF\xFF");
    }
    if (setvbuf(&__stdout, NULL, FILE_LINE_BUFFERING, 64) != 0) {
        __stdio_raise_error("\xFF");
    }
    if (setvbuf(&__stderr, NULL, FILE_LINE_BUFFERING, 16) != 0) {
        __stdio_raise_error("");
    }
}

} // extern "C"

