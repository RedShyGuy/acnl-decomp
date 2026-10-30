#pragma once

#include "imgdb/ImageDbRecord.h"

namespace imgdb {

class LegacyTableRecord;
class PictureDatabase;
class PictureTableRecord;

// a record of a PictureDatabase: its entries in the tables (names are ours)
// RTTI N5imgdb15PictureDbRecordE @ 0x008D2B38
// vtable 0x00908F58 (vptr 0x00908F60), offset_to_top 0, 4 entries
class PictureDbRecord : public ImageDbRecord
{
public:
    PictureDbRecord() : m_pDatabase(NULL), m_Index(-1) {}

    virtual const ImageInfo* GetImageInfo() const;
    virtual ImageInfo* GetImageInfo();
    virtual const ValidityStateTableRecord* GetValidityStateTableRecord() const;
    virtual ValidityStateTableRecord* GetValidityStateTableRecord();

    // NULL without the table of the DSi camera
    LegacyTableRecord* GetLegacyTableRecord() const;
    PictureTableRecord* GetPictureTableRecord() const;

    PictureDatabase* m_pDatabase;  // +0x04
    int m_Index;                   // +0x08
};
ASSERT_SIZE(PictureDbRecord, 0xC);

} // namespace imgdb
