#pragma once

// Results of nn::uds (module 68). The names are ours, after 3dbrew's tables of result summaries and
// descriptions.

#include "decomp.h"

namespace nn {
namespace uds {
namespace CTR {

const bit32 RESULT_TOO_LARGE = 0xE10113E9;                  // usage, wrong argument, 1001
const bit32 RESULT_NOT_AUTHORIZED = 0xE10113EA;             // usage, wrong argument, 1002
const bit32 RESULT_MISALIGNED_ADDRESS = 0xE10113F1;         // usage, wrong argument, 1009
const bit32 RESULT_MISALIGNED_SIZE = 0xE10113F2;            // usage, wrong argument, 1010
const bit32 RESULT_OUT_OF_RANGE = 0xE10113FD;               // usage, wrong argument, 1021
const bit32 RESULT_INVALID_POINTER = 0xE0E113F6;            // usage, invalid argument, 1014
const bit32 RESULT_NOT_INITIALIZED = 0xE0A113F8;            // usage, invalid state, 1016
const bit32 RESULT_ALREADY_INITIALIZED = 0xE06113F9;        // usage, out of resource, 1017
const bit32 RESULT_OUT_OF_MEMORY = 0xC86113F3;              // status, out of resource, 1011
const bit32 RESULT_NOT_AUTHORIZED_STATE = 0xC8A113EA;       // status, invalid state, 1002 (sleep)
const bit32 RESULT_BEACON_WITHOUT_NETWORK = 0xE1211005;     // usage, canceled, 5
const bit32 RESULT_BEACON_WITHOUT_NODES = 0xE1211008;       // usage, canceled, 8
// the results that pia local tells apart (meaning unknown, names after level/summary/description)
const bit32 RESULT_NOT_FOUND_1018 = 0xC88113FA;             // status, not found, 1018
const bit32 RESULT_OUT_OF_RESOURCE_1 = 0xC8611001;          // status, out of resource, 1
const bit32 RESULT_CANCELED_1019 = 0xC92113FB;              // status, canceled, 1019
const bit32 RESULT_CANCELED_1022 = 0xC92113FE;              // status, canceled, 1022
const bit32 RESULT_STATUS_CHANGED_2 = 0xC9411002;           // status, status changed, 2

// the kernel's "session closed by the other side" (module os, 26): the uds service is gone
const bit32 RESULT_SESSION_CLOSED = 0xC920181A;

} // namespace CTR
} // namespace uds
} // namespace nn
