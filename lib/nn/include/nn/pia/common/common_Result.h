#pragma once

// Result codes of pia and the pointer check its functions use (the file name is ours).
//
// All pia results are in module 82. Level and summary after 3dbrew ("Error codes"); the meaning
// of the descriptions is ours, taken from the places that return them.

#include "types.h"

namespace nn {
namespace pia {
namespace common {

// usage, invalid state, 33: Initialize called again
const bit32 RESULT_ALREADY_INITIALIZED = 0xE0A14821;
// usage, out of resource, 32: zlib could not get memory
const bit32 RESULT_OUT_OF_MEMORY = 0xE0614820;
// permanent, internal, 34: a packet or message is not in the expected format
const bit32 RESULT_INVALID_FORMAT = 0xD9614822;
// usage, invalid argument, 35: the output buffer is too small
const bit32 RESULT_BUFFER_SHORTAGE = 0xE0E14823;
// permanent, canceled, 36: the call was canceled (CallContext::SignalCancel)
const bit32 RESULT_CANCELED = 0xD9214824;
// usage, invalid argument, 38
const bit32 RESULT_INVALID_ARGUMENT = 0xE0E14826;
// usage, invalid state, 39: e.g. not in setup mode
const bit32 RESULT_INVALID_STATE = 0xE0A14827;
// usage, internal, 42: an error of a library pia uses (zlib) or an unsupported mode
const bit32 RESULT_INTERNAL_ERROR = 0xE161482A;
// usage, invalid state, 43: the module is not initialized
const bit32 RESULT_NOT_INITIALIZED = 0xE0A1482B;
// permanent, invalid state, 46: the instance is already created
const bit32 RESULT_ALREADY_EXISTS = 0xD8A1482E;

// pia accepts only pointers into the application's address range [0x00100000, 0x40000000)
// (inline everywhere: "sub rX, p, #0x100000; cmp rX, #0x3FF00000"; the name is ours)
inline bool IsValidPointer(const void* p)
{
    return reinterpret_cast<uptr>(p) - 0x00100000 < 0x3FF00000;
}

} // namespace common
} // namespace pia
} // namespace nn
