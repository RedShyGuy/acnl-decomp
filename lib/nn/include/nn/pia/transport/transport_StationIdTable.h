#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_ObjList.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/pia_Types.h"

namespace nn {
namespace pia {
namespace transport {
// The station ids and indices of the stations in the session, with a key per station (Transport
// holds one at 0x7C). The layout is from the constructor and Initialize; the member names and
// the names of the unnamed functions are ours.
class StationIdTable : public common::RootObject
{
public:
    // the type name is from the signatures of Find, its members are ours
    struct Entry
    {
        StationId m_StationId;       // 0x0
        StationIndex m_StationIndex; // 0x8
        u32 m_Key;                   // 0xC
    };

    StationIdTable(); // 0x0044F5A8 | fefates:bytes [tier B]
    ~StationIdTable(); // 0x0044F5F0 | fefates:bytes [tier B]

    // room for entryNum entries on the pia heap
    nn::Result Initialize(u32 entryNum); // 0x0044F23C (name is ours)
    void Finalize(); // 0x0044F510 | fefates:bytes [tier B]

    nn::Result Add(StationId stationId, StationIndex stationIndex, u32 key); // 0x0044F3EC (name is ours)
    nn::Result Remove(StationIndex stationIndex); // 0x0044F354 (name is ours)
    nn::Result Remove(StationId stationId); // 0x0044F4F4 (name is ours)
    nn::Result RemoveCore(Entry* pEntry); // 0x0044F370 (name is ours)
    void Clear(); // 0x0044F484 (name is ours)
    // a lower limit of entries (not above the capacity)
    void SetEntryNumMax(u32 num); // 0x0044F3DC (name is ours)

    s32 GetEntryNum() const; // 0x00734EE0 | fefates:bytes [tier B]
    nn::Result Find(nn::pia::transport::StationIdTable::Entry* pEntry, nn::pia::StationIndex stationIndex) const; // 0x00734F10 | fefates:bytes [tier B]
    nn::Result Find(nn::pia::transport::StationIdTable::Entry* pEntry, nn::pia::StationId stationId) const; // 0x00734F74 | fefates:bytes [tier B]
    nn::Result Find(nn::pia::transport::StationIdTable::Entry* pEntry, unsigned int key) const; // 0x00734FE0 | fefates:bytes [tier B]
    Entry* FindCore(nn::pia::StationIndex stationIndex) const; // 0x00735074 | fefates:bytes-fuzzy [tier B]
    Entry* FindCore(nn::pia::StationId stationId) const; // 0x007350B0 | fefates:bytes [tier B]
    Entry* FindCore(unsigned int key) const; // 0x00735104 | fefates:bytes [tier B]

    common::ObjList<Entry> m_List; // 0x04
    void* m_pBuffer;               // 0x30
    u32 m_EntryNumMax;             // 0x34
};
ASSERT_OFFSET(StationIdTable, m_List, 0x4);
ASSERT_OFFSET(StationIdTable, m_pBuffer, 0x30);
ASSERT_SIZE(StationIdTable, 0x38);
} // namespace transport
} // namespace pia
} // namespace nn
