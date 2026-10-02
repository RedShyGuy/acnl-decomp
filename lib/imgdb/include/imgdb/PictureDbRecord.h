#pragma once

#include "decomp.h"
#include "imgdb/ImageDbRecord.h"

namespace imgdb {
// RTTI N5imgdb15PictureDbRecordE @ 0x008D2B38
// vtable 0x00908F58 (vptr 0x00908F60), offset_to_top 0, 4 entries
class PictureDbRecord : public ::imgdb::ImageDbRecord
{
public:
    PictureDbRecord(); // ctor candidate(s) 0x005A9D8C (unverified)
    virtual void vf_0x00(); // 0x00755A68 slot 0x00 | virtual slot, introduced by imgdb::PictureDbRecord
    virtual void vf_0x04(); // 0x005AAC80 slot 0x04 | virtual slot, introduced by imgdb::PictureDbRecord
    virtual void GetValidityStateTableRecord() const; // 0x00755ADC slot 0x08 | nintendogs:bytes
    virtual void vf_0x0C(); // 0x005AAC8C slot 0x0C | virtual slot, introduced by imgdb::PictureDbRecord
    void GetLegacyTableRecord() const; // 0x00755A8C | nintendogs:bytes [tier B]
};
} // namespace imgdb
