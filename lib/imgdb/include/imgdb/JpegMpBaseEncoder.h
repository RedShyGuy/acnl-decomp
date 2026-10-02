#pragma once

#include "decomp.h"

namespace imgdb {
// RTTI N5imgdb17JpegMpBaseEncoderE @ 0x008D2B74
// vtable 0x00908FB0 (vptr 0x00908FB8), offset_to_top 0, 2 entries
class JpegMpBaseEncoder
{
public:
    JpegMpBaseEncoder(); // ctor address unknown
    virtual ~JpegMpBaseEncoder(); // 0x0011C12F slot 0x00 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x04(); // 0x0011C12F slot 0x04 | slot vf_0x00 of ChangeRentalBase
    void SetSysMakerNoteBodyId(imgdb::BodyIdType, unsigned); // 0x005AB120 | nintendogs:bytes [tier B]
    void ConvTitleUniqueIdToString(char*, int, unsigned); // 0x005AB144 | nintendogs:bytes [tier B]
};
} // namespace imgdb
