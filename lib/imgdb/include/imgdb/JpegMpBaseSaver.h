#pragma once

#include "decomp.h"

namespace imgdb {
// RTTI N5imgdb15JpegMpBaseSaverE @ 0x008D2B24
// vtable 0x00908F30 (vptr 0x00908F38), offset_to_top 0, 2 entries
class JpegMpBaseSaver
{
public:
    JpegMpBaseSaver(); // ctor candidate(s) 0x005A9BC8 (unverified)
    virtual void vf_0x00(); // 0x005A9C94 slot 0x00 | virtual slot, introduced by imgdb::JpegMpBaseSaver
    virtual void vf_0x04(); // 0x005A9C5C slot 0x04 | virtual slot, introduced by imgdb::JpegMpBaseSaver
    void SetScreenshotFlag(bool); // 0x005A5BC8 | nintendogs:bytes [tier B]
    void SetTitleUniqueId(unsigned); // 0x005A9B98 | nintendogs:bytes [tier B]
};
} // namespace imgdb
