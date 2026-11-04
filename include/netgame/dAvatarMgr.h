#pragma once

#include "decomp.h"
#include "net/dSender.h"
#include "net/dSystemCallback.h"
#include "netgame/dAvatarId.h"
#include "netgame/netgame_Api.h"
#include "nn/pia/pia_Types.h"

namespace netgame {
// The stations of the session and their players: the callback of the network layer (names after libgarden:
// AvatarMgr, Principal, AvatarInfoBase, AvatarInfo, GetPlayerNo, GetAvatarInfo, m_Infos, m_JoinInfos, m_UpdateTick,
// m_FriendCodes; the others are ours)
// RTTI N7netgame9AvatarMgrE @ 0x008D3F70
// vtable 0x0090BD4C (vptr 0x0090BD54), offset_to_top 0, 8 entries
class AvatarMgr : public ::net::SystemCallback
{
public:
    struct AvatarInfoBase : public Principal
    {
        DECOMP_NOINLINE AvatarInfoBase(); // 0x0061B2CC
        ~AvatarInfoBase(); // 0x00624AF8
        DECOMP_NOINLINE void Reset(); // 0x0061B288
        DECOMP_NOINLINE void Set(const nn::pia::StationId& newStation, const Principal& principal); // 0x0061B2A4
        bool IsValid() const { return station != nn::pia::StationId(0xFFFFFFFD, 0xFFFFFFFF); }

        nn::pia::StationId station; // 0x08
    };
    ASSERT_SIZE(AvatarInfoBase, 0x10);

    struct AvatarInfo : public AvatarInfoBase
    {
        AvatarInfo(); // 0x006278B0
        ~AvatarInfo(); // 0x006278C4
        // a new station without a principal
        DECOMP_NOINLINE void Set(const nn::pia::StationId& newStation); // 0x00627794
        DECOMP_NOINLINE void Assign(const AvatarInfoBase& info); // 0x006277B8
        DECOMP_NOINLINE void Clear(); // 0x006277EC
        DECOMP_NOINLINE bool Send(const void* data, u32 size); // 0x00627818
        // whether nothing is on its way to the station
        DECOMP_NOINLINE bool CanSend() const; // 0x0075FF84
        DECOMP_NOINLINE u32 GetLatency() const; // 0x0075FFC0

        net::Sender::Handle* senderHandle; // 0x10
        s32 unk;                           // 0x14
    };
    ASSERT_SIZE(AvatarInfo, 0x18);

    struct FriendCode
    {
        DECOMP_NOINLINE FriendCode(); // 0x0062AFD8

        void Clear()
        {
            station = nn::pia::StationId(0xFFFFFFFD, 0xFFFFFFFF);
            friendCode[0] = 0;
            friendCode[1] = 0;
        }

        nn::pia::StationId station; // 0x0
        u32 friendCode[2];          // 0x8
    };
    ASSERT_SIZE(FriendCode, 0x10);

    static const u32 JOIN_INFO_NOT_FOUND = 0xFFFFFFFF;

    AvatarMgr(); // 0x0062B728
    virtual ~AvatarMgr(); // 0x0062B7F8 slot 0x00
    virtual bool OnJoinApproval(const char16* name, u32 length); // 0x0062ABDC slot 0x08
    virtual void OnStationJoined(const nn::pia::StationId& stationId, const char16* name, u32 length, u32 frame); // 0x0062B2BC slot 0x0C
    virtual void OnStationLeft(const nn::pia::StationId& stationId, u32 frame); // 0x0062A3E8 slot 0x10
    virtual void OnReceive(const nn::pia::StationId& stationId, const void* data, u32 size); // 0x0062B4F4 slot 0x14
    virtual void OnMessage(const u32* principalId, const wchar_t* text, u32 length, u32 isNotification); // 0x0062AC3C slot 0x18
    virtual void OnNotification(const u32* principalId, u32 param1, u32 param2, const wchar_t* text, u32 length,
                                bool isFromHandler); // 0x0062AFF4 slot 0x1C

    void Start(); // 0x0062A3A4
    void Process(); // 0x0062A530 | libgarden
    void Reset(); // 0x0062B1FC
    void RemovePlayer(PlayerNo player); // 0x0062AB24
    // takes over the stations that the host sent; false if they do not fit yet
    bool SetStations(u32* newNum, const void* stations); // 0x0062AC64
    // asks the network for the friend code of the player
    bool FetchFriendCode(PlayerNo player); // 0x0062B170
    void GetFriendId(FriendId* id, PlayerNo player) const; // 0x00760214
    bool HasSameStations(const void* stations) const; // 0x00760368
    bool Send(PlayerNo player, const void* data, u32 size); // 0x0062B6B0
    DECOMP_NOINLINE PlayerNo GetPlayerNo(const nn::pia::StationId& id) const; // 0x007603E8 | libgarden
    DECOMP_NOINLINE const AvatarInfo* GetAvatarInfo(PlayerNo player) const; // 0x007604F0 | libgarden
    bool IsInSession(PlayerNo player, bool skipSelf) const; // 0x00760660
    PlayerNo FindMyPlayerNo() const; // 0x00760594
    bool CanSend(PlayerNo player) const; // 0x00760464
    u32 GetLatency(PlayerNo player) const; // 0x007604CC
    bool GetStations(nn::pia::StationId* stations) const; // 0x00760530

private:
    DECOMP_NOINLINE const FriendCode* FindFriendCode(const nn::pia::StationId& id) const; // 0x0062B028
    void RemoveJoinInfo(const nn::pia::StationId& id); // 0x0062B0A4
    DECOMP_NOINLINE u32 FindJoinInfo(const nn::pia::StationId& id) const; // 0x007605DC

public:
    AvatarInfo* GetInfo(PlayerNo player) { return player < PLAYER_NO_MAX ? &m_Infos[player] : NULL; }
    const AvatarInfo* GetInfo(PlayerNo player) const { return player < PLAYER_NO_MAX ? &m_Infos[player] : NULL; }

    s32 unk_0x4;                      // 0x004
    AvatarInfo m_Infos[4];            // 0x008
    AvatarInfoBase m_JoinInfos[4];    // 0x068
    s32 m_UpdateTick[4];              // 0x0A8
    FriendCode m_FriendCodes[4];      // 0x0B8
    nn::pia::StationId m_NewStation;  // 0x0F8 (a station of a random match that just joined)
    nn::pia::StationId m_JoiningStation; // 0x100 (the same while the gate is closed for it)
    bool m_HasJoiner;                 // 0x108
    s32 unk_0x10C;
};
ASSERT_SIZE(AvatarMgr, 0x110);

const AvatarMgr::AvatarInfo* GetAvatar(PlayerNo player); // 0x00626150 | libgarden
} // namespace netgame
