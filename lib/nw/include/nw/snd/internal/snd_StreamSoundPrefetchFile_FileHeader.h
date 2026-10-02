#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_StreamSoundPrefetchFile.h"

class nw::snd::internal::StreamSoundPrefetchFile::FileHeader
{
public:
    void GetInfoBlock() const; // 0x0074127C | fefates:bytes [tier B]
};
