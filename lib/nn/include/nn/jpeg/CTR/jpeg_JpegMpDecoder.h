#pragma once

#include "decomp.h"
#include "nn/jpeg/CTR/detail/jpeg_DecoderWork.h"
#include "nn/jpeg/CTR/jpeg_Types.h"

namespace nn {
namespace jpeg {
namespace CTR {
// Decodes JPEG and MP files and reads their Exif data (member names are ours). The "GetLast"
// functions return data of the last decode.
class JpegMpDecoder
{
public:
    static size_t GetWorkBufferSize(); // 0x00470374
    static size_t GetDstBufferSize(u32 width, u32 height, PixelFormat format); // 0x00470218

    bool Initialize(void* buffer, size_t size); // 0x00470148 | nintendogs:bytes [tier B]
    bool GetMpEntry(MpEntry* entry, const MpIndex* index, u32 number); // 0x0046FF70 | nintendogs:bytes [tier B]
    bool GetMpIndex(MpIndex* index, const u8* src, size_t size); // 0x00470080 | nintendogs:bytes [tier B]
    bool ExtractExif(const u8* src, size_t size, bool isThumbnail); // 0x0047019C | nintendogs:bytes [tier B]
    size_t StartJpegDecoder(void* dst, size_t dstSize, const u8* src, size_t srcSize, u32 maxWidth, u32 maxHeight, PixelFormat format, bool isThumbnail); // 0x004702D8 | nintendogs:bytes [tier B]
    size_t StartJpegDecoderShrink(void* dst, size_t dstSize, const u8* src, size_t srcSize, u32 maxWidth, u32 maxHeight, PixelFormat format, bool isThumbnail, u32 shrink); // 0x00470380 | nintendogs:bytes [tier B]
    bool GetMpRegionsToBuildJpegData(MpRegionsToBuildJpegData* regions, const u8* src, size_t size); // 0x00470490 | nintendogs:bytes [tier B]
    size_t GetLastDateTime(char* buffer) const; // 0x004731C0 | nintendogs:bytes [tier B]
    u32 GetLastWidth() const; // 0x007372F4 | nintendogs:bytes [tier B]
    u32 GetLastHeight() const; // 0x00737324 | nintendogs:bytes [tier B]
    size_t GetLastMakerNoteSize(u32 index) const; // 0x0073734C | nintendogs:bytes [tier B]
    const u8* GetLastSoftware() const; // 0x0073739C (name is ours)
    const u8* GetLastMakerNotePointer(u32 index) const; // 0x007373E0 | nintendogs:bytes [tier B]
    size_t GetLastUserMakerNoteSize() const; // 0x00737430 | nintendogs:bytes [tier B]
    const u8* GetLastUserMakerNotePointer() const; // 0x00737480 | nintendogs:bytes [tier B]

private:
    JpegMpDecoderContext* m_Work;                       // 0x0
    bool m_IsInitialized;                               // 0x4
    detail::JpegMpDecoderTemporarySettingObj m_Settings; // 0x8
};
ASSERT_SIZE(JpegMpDecoder, 0x10);
} // namespace CTR
} // namespace jpeg
} // namespace nn
