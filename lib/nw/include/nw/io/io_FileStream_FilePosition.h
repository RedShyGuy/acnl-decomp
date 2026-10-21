#pragma once

#include "decomp.h"
#include "nw/io/io_FileStream.h"

// The read position of a file stream, kept within the file. Member names are ours.
class nw::io::FileStream::FilePosition
{
public:
    // the origins of Seek (names are ours)
    enum SeekOrigin
    {
        SEEK_ORIGIN_BEGIN,
        SEEK_ORIGIN_CURRENT,
        SEEK_ORIGIN_END
    };

    void Seek(s32 offset, u32 origin); // 0x0048B81C | nintendogs:bytes [tier A]
    // the new position
    u32 Skip(s32 offset); // 0x0048B87C | nintendogs:bytes [tier A]

    u32 m_FileSize; // 0x0
    u32 m_Position; // 0x4
};
