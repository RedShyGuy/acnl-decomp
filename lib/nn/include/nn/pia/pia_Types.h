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

} // namespace pia
} // namespace nn
