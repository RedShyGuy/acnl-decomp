#pragma once

#include "decomp.h"
#include "imgdb/JpegMpBaseSaver.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace imgdb {
// RTTI N5imgdb9JpegSaverE @ 0x008D2C28
// vtable 0x00909098 (vptr 0x009090A0), offset_to_top 0, 2 entries
class JpegSaver : public ::imgdb::JpegMpBaseSaver, public ::nn::util::ADLFireWall::NonCopyable<imgdb::JpegSaver>
{
public:
    virtual void vf_0x00(); // 0x005B15FC slot 0x00 | virtual slot, introduced by imgdb::JpegMpBaseSaver
    virtual void vf_0x04(); // 0x005B15A0 slot 0x04 | virtual slot, introduced by imgdb::JpegMpBaseSaver
    void Save(); // 0x005B11AC | nintendogs:bytes-fuzzy [tier B]
    void SetRaw(const void*, int, int, nn::jpeg::CTR::PixelFormat); // 0x005B14FC | nintendogs:bytes [tier B]
    JpegSaver(); // 0x005B1510 | nintendogs:bytes [tier B]
    void ValidateParam() const; // 0x007567A4 | nintendogs:bytes [tier B]
};
} // namespace imgdb
