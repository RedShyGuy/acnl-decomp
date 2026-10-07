#pragma once

// Types shared by all modules of pia (the file name is ours).

#include "types.h"

namespace nn {
namespace pia {

// The modules of pia. The values are the ones the modules pass to common::HeapManager
// (common = 2 in common::BeginSetup, local = 3, transport = 4, inet = 6, session = 7); the
// enumerator names are ours, the meaning of the other values is not known.
enum ModuleType : u8
{
    MODULE_TYPE_COMMON = 2,
    MODULE_TYPE_LOCAL = 3,
    MODULE_TYPE_TRANSPORT = 4,
    MODULE_TYPE_INET = 6,
    MODULE_TYPE_SESSION = 7,
};

// Index of a station in the session: 0..STATION_INDEX_MAX. 253 marks a station that has no index
// yet (common::Packet::Reset sets it as the source); the enumerator names are ours.
enum StationIndex : u8
{
    STATION_INDEX_MAX = 11,
    STATION_INDEX_UNIDENTIFIED = 253,
};

// The id of a station in the whole network: 64 bits, but only word aligned (passed in r1:r2 after
// this). The special indices 253 / 254 / 255 map to the ids -3 / -2 / -1 (Transport::
// ConvertToStationId). It has a constructor: locals start as 0 and the static ids have guards.
// The member names are ours.
struct StationId
{
    StationId() : m_Low(0), m_High(0) {}
    StationId(u32 low, u32 high) : m_Low(low), m_High(high) {}

    bool operator==(const StationId& rhs) const { return m_Low == rhs.m_Low && m_High == rhs.m_High; }
    bool operator!=(const StationId& rhs) const { return !(*this == rhs); }

    // as one 64 bit value (the messages carry it so; names are ours)
    u64 ToU64() const { return (static_cast<u64>(m_High) << 32) | m_Low; }
    static StationId FromU64(u64 value) { return StationId(static_cast<u32>(value), static_cast<u32>(value >> 32)); }

    u32 m_Low;  // 0x0
    u32 m_High; // 0x4
};

// The station ids of the special indices 253 / 254 / 255, each a function-local static
// (names are ours)
const StationId& GetStationIdOfIndex253(); // 0x003E254C (name is ours)
const StationId& GetStationIdOfIndex254(); // 0x003E24FC (name is ours)
const StationId& GetStationIdOfIndex255(); // 0x003E24AC (name is ours)

} // namespace pia
} // namespace nn
