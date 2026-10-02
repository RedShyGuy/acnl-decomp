#pragma once

#include "decomp.h"
#include "imgdb/JpegMpBaseSaver.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace imgdb {
// RTTI N5imgdb7MpSaverE @ 0x008D2C08
// vtable 0x00909088 (vptr 0x00909090), offset_to_top 0, 2 entries
class MpSaver : public ::imgdb::JpegMpBaseSaver, public ::nn::util::ADLFireWall::NonCopyable<imgdb::MpSaver>
{
public:
    virtual void vf_0x00(); // 0x005B0920 slot 0x00 | virtual slot, introduced by imgdb::JpegMpBaseSaver
    virtual void vf_0x04(); // 0x005B08C4 slot 0x04 | virtual slot, introduced by imgdb::JpegMpBaseSaver
    void Save(); // 0x005B04B8 | nintendogs:bytes-fuzzy [tier B]
    void SetRaw(const void*, const void*, int, int, nn::jpeg::CTR::PixelFormat); // 0x005B0808 | nintendogs:bytes [tier B]
    MpSaver(); // 0x005B0824 | nintendogs:bytes [tier B]
    void ValidateParam() const; // 0x0075612C | nintendogs:bytes [tier B]
};
} // namespace imgdb
