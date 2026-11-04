#pragma once

// The best friends of the player (a save file of its own; layout after libgarden, the fields are ours).
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvPersonalId.h"

#pragma pack(push, 1)

struct SvBestFriend {
    /* 0x0000 */ u8 unk_0x0[0x1420];
    /* 0x1420 */ SvPersonalId personalId; // the friend (empty: no friend in the entry)
    /* 0x144E */ u8 unk_0x144E[0x62];
};
ASSERT_SIZE(SvBestFriend, 0x14B0);

struct SvBestFriendList {
    // whether one of the entries holds a friend
    bool HasFriend() const; // 0x0071E09C | libgarden [tier A]

    /* 0x0000 */ u64 header;
    /* 0x0008 */ SvBestFriend entries[32];
};
ASSERT_SIZE(SvBestFriendList, 0x29608);

#pragma pack(pop)
