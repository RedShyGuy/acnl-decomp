#include "imgdb/ImageCollectionTable.h"

namespace imgdb {

// 0x00755D14 | nintendogs:bytes [tier B]
int imgdb::ImageCollectionTable::FindRecord(ImageCollectionTableRecord::State state, int start) const
{
    return FindRecordByStateBits(static_cast<u8>(1 << state), start);
}

} // namespace imgdb
