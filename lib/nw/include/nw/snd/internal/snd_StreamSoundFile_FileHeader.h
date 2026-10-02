#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_StreamSoundFile.h"

class nw::snd::internal::StreamSoundFile::FileHeader
{
public:
    void GetReferenceBy(unsigned short) const; // 0x0073FA54 | fefates:bytes [tier B]
    void GetInfoBlockSize() const; // 0x0073FB30 | fefates:bytes [tier B]
    void GetInfoBlockOffset() const; // 0x0073FB5C | fefates:bytes [tier B]
};
