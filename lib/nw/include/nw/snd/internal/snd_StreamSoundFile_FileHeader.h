#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_StreamSoundFile.h"
#include "nw/ut/ut_Api.h"

class nw::snd::internal::StreamSoundFile::FileHeader : public ::nw::ut::BinaryFileHeader
{
public:
    bool HasSeekBlock() const; // 0x0073FA38 (name after HasRegionBlock)
    // the block of a type (NULL: none)
    DECOMP_NOINLINE const BinaryReferenceWithSize* GetReferenceBy(u16 typeId) const; // 0x0073FA54 | fefates:bytes [tier B]
    bool HasRegionBlock() const; // 0x0073FB14 (name after the reference symbols of the region functions)
    u32 GetInfoBlockSize() const; // 0x0073FB30 | fefates:bytes [tier B]
    u32 GetDataBlockOffset() const; // 0x0073FB44 (name after GetInfoBlockOffset)
    u32 GetInfoBlockOffset() const; // 0x0073FB5C | fefates:bytes [tier B]
    u32 GetSeekBlockOffset() const; // 0x0073FB70 (name after GetInfoBlockOffset)
    u32 GetRegionBlockOffset() const; // 0x0073FB88 (name after GetInfoBlockOffset)

    BinaryReferenceWithSize blocks[1]; // 0x14
};
