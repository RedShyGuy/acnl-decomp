#pragma once

#include "decomp.h"
#include "nw/io/io_FileStream.h"

class nw::io::FileStream::FilePosition
{
public:
    void Seek(int, unsigned); // 0x0048B81C | nintendogs:bytes [tier A]
    void Skip(int); // 0x0048B87C | nintendogs:bytes [tier A]
};
