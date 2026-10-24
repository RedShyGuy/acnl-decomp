#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_StreamSoundPrefetchFile.h"
#include "nw/ut/ut_Api.h"

class nw::snd::internal::StreamSoundPrefetchFile::FileHeader : public ::nw::ut::BinaryFileHeader
{
public:
    // the blocks (NULL: none)
    const void* GetInfoBlock() const; // 0x0074127C | fefates:bytes [tier B]
    const void* GetPrefetchDataBlock() const; // 0x007412E8 (name after GetInfoBlock)

    BinaryReferenceWithSize blocks[1]; // 0x14
};
