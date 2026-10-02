#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"

class nw::snd::internal::SoundArchiveFile::StreamSoundInfoLegacy
{
public:
    void GetLoopEndFrame() const; // 0x007402E8 | fefates:bytes [tier B]
    void GetLoopStartFrame() const; // 0x00740308 | fefates:bytes [tier B]
    void GetStreamFileType() const; // 0x00740328 | fefates:bytes [tier B]
    void IsLoop() const; // 0x0074036C | fefates:bytes [tier B]
};
