#pragma once

#include "imgdb/imgdb_Types.h"
#include "nn/jpeg/CTR/jpeg_JpegMpDecoder.h"

namespace imgdb {

class SysMakerNote;

// the Exif data of the last decode of JpegDecoder and MpDecoder (names are ours)
// RTTI N5imgdb17JpegMpBaseDecoderE @ 0x008D2B6C (its vtable is not in the binary)
class JpegMpBaseDecoder
{
public:
    explicit JpegMpBaseDecoder(nn::jpeg::CTR::JpegMpDecoder* decoder) : m_pDecoder(decoder) {}
    virtual ~JpegMpBaseDecoder() = 0;

    // the shrink level at which width x height fits into maxWidth x maxHeight; width and height become the
    // shrunk size
    static int ComputeShrinkLevel(int& width, int& height, int maxWidth, int maxHeight);
    bool GetDateTime(nn::fnd::DateTime& dateTime) const;
    // a copy of the maker note of the system ("3DS1")
    bool GetSysMakerNote(SysMakerNote& note) const;
    // the maker note of the system in the Exif data, NULL without one
    const SysMakerNote* GetSysMakerNote() const;

    DECOMP_EMIT_COPY const u8* GetSoftware() const { return m_pDecoder->GetLastSoftware(); } // 0x00737394
    DECOMP_EMIT_COPY size_t GetUserMakerNoteSize() const { return m_pDecoder->GetLastUserMakerNoteSize(); } // 0x00737428
    DECOMP_EMIT_COPY const u8* GetUserMakerNote() const { return m_pDecoder->GetLastUserMakerNotePointer(); } // 0x00737478

protected:
    nn::jpeg::CTR::JpegMpDecoder* m_pDecoder;  // +0x04, the one of the derived class
};
ASSERT_SIZE(JpegMpBaseDecoder, 0x8);

inline JpegMpBaseDecoder::~JpegMpBaseDecoder()
{
}

} // namespace imgdb
