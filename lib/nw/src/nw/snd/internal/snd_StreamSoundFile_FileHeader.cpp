#include "nw/snd/internal/snd_StreamSoundFile_FileHeader.h"

namespace nw {
namespace snd {
namespace internal {
// 0x0073FA38 (name after HasRegionBlock)
bool nw::snd::internal::StreamSoundFile::FileHeader::HasSeekBlock() const
{
    return GetReferenceBy(TYPE_SEEK_BLOCK) != nullptr;
}

// 0x0073FA54 | fefates:bytes [tier B]
const nw::snd::internal::BinaryReferenceWithSize* nw::snd::internal::StreamSoundFile::FileHeader::GetReferenceBy(u16 typeId) const
{
    for (int i = 0; i < dataBlocks; ++i) {
        if (blocks[i].typeId == typeId) {
            return &blocks[i];
        }
    }
    return nullptr;
}

// 0x0073FB14 (name after the reference symbols of the region functions)
bool nw::snd::internal::StreamSoundFile::FileHeader::HasRegionBlock() const
{
    return GetReferenceBy(TYPE_REGION_BLOCK) != nullptr;
}

// 0x0073FB30 | fefates:bytes [tier B]
u32 nw::snd::internal::StreamSoundFile::FileHeader::GetInfoBlockSize() const
{
    return GetReferenceBy(TYPE_INFO_BLOCK)->size;
}

// 0x0073FB44 (name after GetInfoBlockOffset)
u32 nw::snd::internal::StreamSoundFile::FileHeader::GetDataBlockOffset() const
{
    return GetReferenceBy(TYPE_DATA_BLOCK)->offset;
}

// 0x0073FB5C | fefates:bytes [tier B]
u32 nw::snd::internal::StreamSoundFile::FileHeader::GetInfoBlockOffset() const
{
    return GetReferenceBy(TYPE_INFO_BLOCK)->offset;
}

// 0x0073FB70 (name after GetInfoBlockOffset)
u32 nw::snd::internal::StreamSoundFile::FileHeader::GetSeekBlockOffset() const
{
    return GetReferenceBy(TYPE_SEEK_BLOCK)->offset;
}

// 0x0073FB88 (name after GetInfoBlockOffset)
u32 nw::snd::internal::StreamSoundFile::FileHeader::GetRegionBlockOffset() const
{
    return GetReferenceBy(TYPE_REGION_BLOCK)->offset;
}

} // namespace internal
} // namespace snd
} // namespace nw
