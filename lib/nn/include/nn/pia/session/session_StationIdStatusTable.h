#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_ObjList.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/transport/transport_StationIdTable.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session20StationIdStatusTableE @ 0x008D0080
// vtable 0x00901A54 (vptr 0x00901A5C), offset_to_top 0, 2 entries
//
// The status of the stations of the session by their station id (Session keeps one when it uses
// the station id table): the session they are in and the flags of the joint session. The layout
// is from the constructor and Add; all member and function names are ours.
class StationIdStatusTable : public ::nn::pia::common::RootObject
{
public:
    struct Entry
    {
        u8 m_Status;            // 0x00, 1, 2 (SetStatus); 0 when cleared
        u8 m_Unknown0x1;        // 0x01, 1, 2 (SetUnknown0x1)
        bool m_IsValid;         // 0x02, true from Add
        bool m_IsNotified;      // 0x03, Session was told of the station
        StationId m_StationId;  // 0x04
        u32 m_SessionId;        // 0x0C, of the session (gathering) the station is in
        u8 m_Unknown0x10;       // 0x10
        StationIndex m_StationIndex; // 0x11
        bool m_Unknown0x12;     // 0x12
        bool m_Unknown0x13;     // 0x13
    };

    StationIdStatusTable(); // 0x0043ED60
    virtual ~StationIdStatusTable(); // 0x0043EE4C slot 0x00
    // 0x0043EDB4 slot 0x04 (deleting dtor)

    // stationNum entries (1..12)
    nn::Result Initialize(u32 stationNum); // 0x0043E3B4
    nn::Result Add(const StationId& stationId, u32 sessionId, u8 status, bool isNotified); // 0x0043E2EC
    nn::Result Remove(const StationId& stationId); // 0x0043E560
    void Clear(); // 0x0043E6FC
    Entry* Find(const StationId& stationId); // 0x0043E500

    // the fields of one entry (false / 0 if there is none)
    void SetValid(const StationId& stationId, bool isValid); // 0x0043E4B0
    void SetUnknown0x10(const StationId& stationId, u8 value); // 0x0043E644
    void SetStationIndex(const StationId& stationId, StationIndex stationIndex); // 0x0043E6A0
    void SetStatus(const StationId& stationId, bool isTwo); // 0x0043E754
    void SetNotified(const StationId& stationId, bool isNotified); // 0x0043E8A4
    bool SetUnknown0x12(const StationId& stationId, bool value); // 0x0043E8F4
    bool SetUnknown0x13(const StationId& stationId, bool value); // 0x0043EAD8
    bool SetUnknown0x1(const StationId& stationId, bool isOne); // 0x0043ECEC
    u8 GetUnknown0x1(const StationId& stationId); // 0x0043E7C0
    bool GetSessionId(const StationId& stationId, u32* pSessionId) const; // 0x007339EC
    bool GetUnknown0x10(const StationId& stationId, u8* pValue) const; // 0x00733A54
    bool GetStationIndex(const StationId& stationId, StationIndex* pStationIndex) const; // 0x00733AF8
    bool IsValid(const StationId& stationId) const; // 0x00733C70
    u8 GetStatus(const StationId& stationId) const; // 0x00733CD0
    bool IsNotified(const StationId& stationId) const; // 0x00733E80
    bool GetUnknown0x12(const StationId& stationId) const; // 0x00733EE0
    bool GetUnknown0x13(const StationId& stationId) const; // 0x00733F40

    // the same for all entries
    void ClearUnknown0x1(); // 0x0043E820
    void ClearValid(); // 0x0043E84C
    void ClearStatus(); // 0x0043E878
    void ClearUnknown0x12(); // 0x0043EC94
    void ClearUnknown0x13(); // 0x0043ECC0

    // the stations with the session id
    u16 GetStationNum(u32 sessionId) const; // 0x00733ABC
    // 1 + the number of session ids below sessionId
    u8 GetSessionRank(u32 sessionId) const; // 0x00733B60
    // the different session ids; their number
    u8 GetSessionIds(u32* pSessionIds) const; // 0x00733C00
    // false if one of the stations is not valid
    bool AreValid(const StationId* pStationIds, u32 num) const; // 0x00733D30
    // 2 if one of the stations (all without a list) has status 1, 1 if one has status 0
    u8 CheckStatus(const StationId* pStationIds, u32 num) const; // 0x00733DBC
    // the station is in the joint session (with Session)
    bool IsJointSessionStation(const transport::StationIdTable::Entry* pEntry) const; // 0x00733FA0

    // with Session: the stations that are gone leave, the others are announced
    void RemoveLostStations(); // 0x0043E958
    void NotifyStations(); // 0x0043EB3C

    common::ObjList<Entry> m_List; // 0x04
    void* m_pBuffer;               // 0x30
};
ASSERT_SIZE(StationIdStatusTable::Entry, 0x14);
ASSERT_OFFSET(StationIdStatusTable, m_pBuffer, 0x30);
ASSERT_SIZE(StationIdStatusTable, 0x34);
} // namespace session
} // namespace pia
} // namespace nn
