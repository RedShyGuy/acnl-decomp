#pragma once

#include "imgdb/imgdb_Types.h"

namespace imgdb {

class ImageInfo;
class ValidityStateTableRecord;

// a record of the database (names are ours; the non-const getters call the const ones)
// RTTI N5imgdb13ImageDbRecordE @ 0x008D2AFC (its vtable is not in the binary)
class ImageDbRecord
{
public:
    virtual const ImageInfo* GetImageInfo() const = 0;                               // slot 0x00
    virtual ImageInfo* GetImageInfo() = 0;                                           // slot 0x04
    virtual const ValidityStateTableRecord* GetValidityStateTableRecord() const = 0;  // slot 0x08
    virtual ValidityStateTableRecord* GetValidityStateTableRecord() = 0;              // slot 0x0C
};

} // namespace imgdb
