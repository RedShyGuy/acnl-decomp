#pragma once

#include "imgdb/ImageCollectionTableRecord.h"

namespace imgdb {

// the records of ImageCollector in a buffer of the caller (names are ours)
class ImageCollectionTable
{
public:
    // the first record after start in the state, -1 without one
    int FindRecord(ImageCollectionTableRecord::State state, int start) const;

    // the first record after start in one of the states (a bit per state)
    int FindRecordByStateBits(u8 stateBits, int start) const
    {
        for (int i = start + 1; i >= 0 && i < m_RecordNum; i++)
        {
            if (stateBits & (1 << m_pRecords[i].m_State))
            {
                return i;
            }
        }
        return -1;
    }

    u32 _00;                                // +0x00
    ImageCollectionTableRecord* m_pRecords;  // +0x04
    int m_RecordNum;                        // +0x08
};
ASSERT_SIZE(ImageCollectionTable, 0xC);

} // namespace imgdb
