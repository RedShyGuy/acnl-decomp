#pragma once

#include "decomp.h"

namespace nn {
namespace cec {
namespace CTR {
namespace detail {
// The results of the cec library (module 66; the names are ours, after the level, summary and
// description of 3dbrew "Error codes"; 101-109 are cec's own descriptions).

// usage, invalid argument, out of range
const bit32 RESULT_OUT_OF_RANGE = 0xE0E10BFD;
// usage, invalid argument, invalid combination (sent and forwarded more than once)
const bit32 RESULT_INVALID_COMBINATION = 0xE0E10BEE;
// status, invalid argument, too large
const bit32 RESULT_TOO_LARGE = 0xC8E10BE9;
// status, not found, no data
const bit32 RESULT_NO_DATA = 0xC8810BEF;
// status, invalid state, misaligned size
const bit32 RESULT_MISALIGNED_SIZE = 0xC8A10BF2;
// status, not found, not authorized (no message box open, message already sent, ...)
const bit32 RESULT_NOT_AUTHORIZED = 0xC8810BEA;
// status, invalid state, busy (cecd is scanning)
const bit32 RESULT_BUSY = 0xC8A10BF0;
// status, out of resource, out of memory
const bit32 RESULT_OUT_OF_MEMORY = 0xC8610BF3;
// usage, invalid state, 108: the EULA is not agreed (or a newer one is required)
const bit32 RESULT_EULA_NOT_AGREED = 0xE0A1086C;
// usage, invalid state, 109: restricted by the parental controls
const bit32 RESULT_PARENTAL_CONTROL = 0xE0A1086D;
// status, invalid argument, 101: the box is full (size)
const bit32 RESULT_BOX_SIZE_FULL = 0xC8E10865;
// status, invalid argument, 102: the box is full (number of messages)
const bit32 RESULT_BOX_MESSAGES_FULL = 0xC8E10866;
// status, invalid argument, 103: there are too many message boxes
const bit32 RESULT_BOX_NUM_FULL = 0xC8E10867;
// status, invalid argument, 104: the message box exists
const bit32 RESULT_BOX_ALREADY_EXISTS = 0xC8E10868;
// status, invalid argument, 105: the message is too large for the box
const bit32 RESULT_MESSAGE_TOO_LARGE = 0xC8E10869;
// status, invalid argument, 106: broken data (box information, name)
const bit32 RESULT_INVALID_DATA = 0xC8E1086A;
// status, invalid argument, 107: the message is for another title
const bit32 RESULT_INVALID_ID = 0xC8E1086B;
// usage, not found, not initialized (no session)
const bit32 RESULT_NOT_INITIALIZED = 0xE0810BF8;

// the descriptions some functions test (common descriptions, 3dbrew "Error codes")
const bit32 DESCRIPTION_INVALID_HANDLE = 1015;
const bit32 DESCRIPTION_TIMEOUT = 1022;
} // namespace detail
} // namespace CTR
} // namespace cec
} // namespace nn
