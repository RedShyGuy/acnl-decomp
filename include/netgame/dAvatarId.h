#pragma once

#include "decomp.h"

struct SvPersonalId;

namespace netgame {
// the empty player id of the save data (0)
extern const u16 INVALID_PLAYER_ID;
// the player id of a session that has none
extern const u16 UNKNOWN_PLAYER_ID;

// A player of a save: the ids of the player and of the town (names after libgarden: AvatarId, playerId,
// townId; the others are ours)
struct AvatarId
{
    DECOMP_NOINLINE AvatarId() : playerId(INVALID_PLAYER_ID), townId(0) {} // 0x006261F8
    DECOMP_NOINLINE ~AvatarId(); // 0x00626214

    DECOMP_NOINLINE void Reset(); // 0x006261B4
    void Set(const SvPersonalId& id); // 0x006261D0
    void Set(const u16& newPlayerId, const u16& newTownId); // 0x006261E4
    bool IsValid() const { return playerId != INVALID_PLAYER_ID && townId != 0; }
    bool operator==(const AvatarId& other) const { return playerId == other.playerId && townId == other.townId; }

    u16 playerId; // 0x0
    u16 townId;   // 0x2
};
ASSERT_SIZE(AvatarId, 4);

// the principal id and the avatar of a friend (names after libgarden)
struct Principal
{
    Principal(); // 0x00624AE0
    ~Principal(); // 0x00624AFC
    Principal& Set(const SvPersonalId& id, const u32& newPrincipalId); // 0x00624AC8
    // the principal id of the console and the ids of the current player
    void SetFromMyself(); // 0x00624A0C
    void Reset(); // 0x006261A8

    u32 principalId;   // 0x0
    AvatarId avatarId; // 0x4
};
ASSERT_SIZE(Principal, 8);

// a friend as the network knows it (names are ours)
struct FriendId
{
    FriendId(); // 0x0051B7AC
    // the principal ids if both have one, else the friend codes
    bool IsDifferent(const FriendId& other) const; // 0x00748318

    u32 principalId;   // 0x0
    u32 friendCode[2]; // 0x4
};
ASSERT_SIZE(FriendId, 0xC);

// a principal as Base64 text (the name of a station)
void DecodePrincipal(Principal* principal, const char16* text, u32 length); // 0x00624A4C
void EncodePrincipal(char16* text, const Principal* principal); // 0x00624A90

// the best friends of the player as ids (names are ours)
class AvatarIdList
{
public:
    static const s32 NUM = 32;

    AvatarIdList(); // 0x00625820
    // the ids of the best friends of the save data
    void SetFromSave(); // 0x00625778
    void Assign(const AvatarIdList& other); // 0x006257F4
    void Reset(); // 0x006257FC
    // the index of the valid id, -1 if it is not there
    s32 Find(const AvatarId& id) const; // 0x0075F994

    AvatarId m_Ids[NUM]; // 0x00
};
ASSERT_SIZE(AvatarIdList, 0x80);
} // namespace netgame
