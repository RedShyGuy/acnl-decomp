#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"

class nw::snd::internal::SoundArchiveFile::StreamSoundInfo
{
public:
    void GetStreamSoundExtension() const; // 0x007401AC | fefates:bytes [tier B]
};
