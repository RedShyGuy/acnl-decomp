#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"

class nw::snd::internal::SoundArchiveFile::FileInfo
{
public:
    void GetExternalFileInfo() const; // 0x00143AF0 | nintendogs:bytes [tier A]
    void GetFileLocationType() const; // 0x00143B1C | nintendogs:bytes [tier A]
    void GetInternalFileInfo() const; // 0x00143B40 | nintendogs:bytes [tier A]
};
