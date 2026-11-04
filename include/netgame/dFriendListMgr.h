#pragma once

#include "Sv/dSvPersonalId.h"
#include "decomp.h"
#include "netgame/dAvatarId.h"
#include "netgame/netgame_Api.h"
#include "nn/os/os_CriticalSection.h"
#include "sead/seadObjArray.h"

namespace netgame {
// The friends online and the best friends of the save data. The stations tell each other their best friends as
// Base64 text with a notification. (Names after libgarden: FriendListMgr, FindBestFriend; the others are ours.)
class FriendListMgr
{
public:
    static const s32 BEST_FRIEND_MAX = 32;
    static const s32 ONLINE_FRIEND_MAX = 100;

    // the steps of a change of the best friends of the save data
    enum State : u8
    {
        STATE_NONE,
        STATE_FETCH_FRIEND_CODE,
        STATE_ADD_BEST_FRIEND,
        STATE_SEND_STATUS,
    };

    // a friend online with its best friends
    struct OnlineFriend
    {
        OnlineFriend() : param(0) {}
        ~OnlineFriend() {}

        u32 principalId;   // 0x00
        AvatarIdList ids;  // 0x04
        AvatarId avatarId; // 0x84
        union
        {
            u32 param;     // 0x88
            u8 flags[4];
        };
    };
    ASSERT_SIZE(OnlineFriend, 0x8C);

    FriendListMgr(); // 0x006237DC
    void Initialize(); // 0x00622C54
    void Finalize(); // 0x00623734
    // the best friends from the save data again
    void Refresh(); // 0x00622CBC
    // tells the other stations about the connection (sends the best friends along)
    bool SendStatus(bool isConnected, bool isFirst); // 0x00622EC8
    // updates the entry of the best friend at the player
    void UpdateBestFriend(PlayerNo player); // 0x00622F50
    void OnNotification(const u32* principalId, u32 param1, u32 param2, const wchar_t* text, u32 length,
                        bool isFromHandler); // 0x006231E4
    bool StartAddBestFriend(PlayerNo player); // 0x006234CC
    // the steps of StartAddBestFriend; true when they are done
    bool ProcessAddBestFriend(); // 0x0062333C
    bool ProcessSendStatus(); // 0x00623588
    void RemoveBestFriend(u32 index); // 0x00623664
    void GetBestFriend(Principal* principal, u32 index); // 0x006236C0
    bool IsBestFriend(PlayerNo player); // 0x0075F4BC
    const AvatarId& GetMyAvatarId() const; // 0x0075F660
    DECOMP_NOINLINE const AvatarIdList& GetBestFriendIds() const { return m_BestFriendIds; } // 0x0075F768
    // the first flag of the best friend online, if it has me as best friend too
    s8 GetBestFriendFlag(u32 index); // 0x0075F66C
    // whether the friend online has me as best friend
    bool HasMeAsBestFriend(const u32* principalId); // 0x0075F6CC
    s32 FindBestFriend(const u32& principal, const AvatarId& avatar); // 0x0075F598 | libgarden

private:
    void OnFriendOnline(const u32* principalId, u32 param, const wchar_t* text, u32 length, bool isFromHandler); // 0x00622FCC
    void OnFriendOffline(const u32* principalId);

    nn::os::CriticalSection m_CriticalSection;  // 0x0000
    sead::FixedObjArray<OnlineFriend, ONLINE_FRIEND_MAX> m_OnlineFriends; // 0x000C
    AvatarIdList m_BestFriendIds;               // 0x3860
    AvatarId m_MyAvatarId;                      // 0x38E0
    FriendId m_BestFriends[BEST_FRIEND_MAX];    // 0x38E4
    SvPersonalId m_PersonalIds[BEST_FRIEND_MAX]; // 0x3A64
    OnlineFriend* m_BestFriendsOnline[BEST_FRIEND_MAX]; // 0x4024
    char16 m_Text[256];                         // 0x40A4
    union
    {
        u32 m_StatusParam;                      // 0x42A4
        bool m_StatusFlags[2];
    };
    State m_State;                              // 0x42A8
    PlayerNo m_Player;                          // 0x42A9
    bool m_IsInitialized;                       // 0x42AA
    bool m_IsSending;                           // 0x42AB
};
ASSERT_SIZE(FriendListMgr, 0x42AC);
} // namespace netgame
