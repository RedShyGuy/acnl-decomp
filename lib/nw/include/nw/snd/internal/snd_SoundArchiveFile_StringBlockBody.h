#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"

class nw::snd::internal::SoundArchiveFile::StringBlockBody
{
public:
    struct Sections { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void GetItemIdImpl(nw::snd::internal::SoundArchiveFile::StringBlockBody::Sections, const char*) const; // 0x007401D4 | fefates:bytes [tier B]
    void GetString(unsigned) const; // 0x007402BC | nintendogs:bytes [tier A]
};
