#pragma once

#include "decomp.h"
#include "nn/fnd/fnd_DateTime.h"

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
    s32 result;                       // 0x000 (read by nex JobCTRLogin)
    s32 httpStatusCode;               // 0x004
    u8 unknown0x008[0x80];            // 0x008
    char serviceToken[0x100];         // 0x088
    u8 unknown0x188[0x8];             // 0x188
    nn::fnd::DateTime serverTime;     // 0x190 (nex zeroes it)
};
ASSERT_SIZE(nnfriesndsServiceLocatorData, 0x198);

struct nnfriesndsGameAuthenticationData
{
    s32 result;                       // 0x000 (read by nex JobCTRLogin)
    s32 httpStatusCode;               // 0x004
    char serverAddress[0x20];         // 0x008
    u16 serverPort;                   // 0x028
    u8 unknown0x02A[0x6];             // 0x02A
    char token[0x100];                // 0x030
    nn::fnd::DateTime serverTime;     // 0x130 (nex zeroes it)
};
ASSERT_SIZE(nnfriesndsGameAuthenticationData, 0x138);

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
