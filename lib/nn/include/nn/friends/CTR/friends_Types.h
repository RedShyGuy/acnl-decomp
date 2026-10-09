#pragma once

#include "decomp.h"

// The C types of the friends library (names from the symbols, including the typo "friesnds"; the
// layouts after the IPC buffers of frd:u, 3dbrew "Friend Services"; member names are ours).

// a friend: principal id and local friend code
struct nnfriendsFriendKey
{
    u32 principalId;      // 0x0
    u32 padding;          // 0x4
    u64 localFriendCode;  // 0x8
};
ASSERT_SIZE(nnfriendsFriendKey, 0x10);

struct nnfriendsMyPresence
{
    u8 data[0x12C];
};

struct nnfriesndsServiceLocatorData
{
    u8 data[0x198];
};

struct nnfriesndsGameAuthenticationData
{
    u8 data[0x138];
};

namespace nn {
namespace friends {
namespace CTR {
// what GetMyApproachContext / AddFriendWithApproach exchange (name is ours)
struct ApproachContext
{
    u8 data[0x200];
};

// an entry of GetEventNotification (name is ours)
struct EventNotification
{
    u8 data[0x18];
};

// a local friend code as scrambled by StreetPass (name is ours)
struct ScrambledFriendCode
{
    u8 data[0xC];
};
} // namespace CTR
} // namespace friends
} // namespace nn
