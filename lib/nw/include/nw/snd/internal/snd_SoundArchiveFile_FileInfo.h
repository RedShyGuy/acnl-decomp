#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BinaryTypes.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"

// where a file is: in the file block, outside the archive or nowhere
class nw::snd::internal::SoundArchiveFile::FileInfo
{
public:
    enum FileLocationType
    {
        FILE_LOCATION_TYPE_INTERNAL = 0,
        FILE_LOCATION_TYPE_EXTERNAL = 1,
        FILE_LOCATION_TYPE_NONE = 2,
    };

    // the file path
    const char* GetExternalFileInfo() const; // 0x00143AF0 | nintendogs:bytes [tier A]
    FileLocationType GetFileLocationType() const; // 0x00143B1C | nintendogs:bytes [tier A]
    // the place in the file block
    const BinaryReferenceWithSize* GetInternalFileInfo() const; // 0x00143B40 | nintendogs:bytes [tier A]

    BinaryReference fileLocation; // 0x0
};
