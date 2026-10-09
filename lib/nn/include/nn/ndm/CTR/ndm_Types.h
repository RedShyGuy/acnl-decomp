#pragma once

#include "decomp.h"

namespace nn {
namespace ndm {
namespace CTR {
// the background daemons of the system (type names from the symbols, values after 3dbrew "NDM
// Services")
enum DaemonName : u8
{
    DAEMON_NAME_CEC = 0,
    DAEMON_NAME_BOSS = 1,
    DAEMON_NAME_NIM = 2,
    DAEMON_NAME_FRIENDS = 3,
    DAEMON_NAME_MAX = 4,
};

// a mask of daemons (1 << DaemonName)
const bit32 DAEMON_MASK_CEC = 1 << DAEMON_NAME_CEC;
const bit32 DAEMON_MASK_BOSS = 1 << DAEMON_NAME_BOSS;
const bit32 DAEMON_MASK_NIM = 1 << DAEMON_NAME_NIM;
const bit32 DAEMON_MASK_FRIENDS = 1 << DAEMON_NAME_FRIENDS;
const bit32 DAEMON_MASK_ALL = DAEMON_MASK_CEC | DAEMON_MASK_BOSS | DAEMON_MASK_NIM | DAEMON_MASK_FRIENDS;

// what has the network exclusively
enum ExclusiveMode : u8
{
    EXCLUSIVE_MODE_NONE = 0,
    EXCLUSIVE_MODE_INFRASTRUCTURE = 1,
    EXCLUSIVE_MODE_LOCAL_COMMUNICATIONS = 2,
    EXCLUSIVE_MODE_STREETPASS = 3,
    EXCLUSIVE_MODE_STREETPASS_DATA = 4,
};
} // namespace CTR
} // namespace ndm
} // namespace nn
