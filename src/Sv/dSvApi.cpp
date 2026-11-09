#include "Sv/dSvApi.h"
#include "Bs/dBsPartsSaveMgr.h"

// the objects that the access functions read (they belong to other files of the game)
// 0x0095CF94
SvSaveMemory g_SvSaveMemory;
// 0x0095CFA0
u8 g_SvIsSaveMemoryValid;
// 0x0095CFA1
u8 g_SvIsSaveMemoryLocked;
// 0x00A85790
SvNetPlayerList g_SvNetPlayerList;
// 0x00AB014C
SvPlayer* g_pSvCurrentPlayer;

// 0x002FB344 (name is ours)
SvGardenPlus* GetGardenPlus()
{
    return g_SvSaveMemory.pData;
}

// 0x002FAEAC (name is ours)
DECOMP_NOINLINE SvGardenData* GetGardenData()
{
    IsSaveIoOnMainData();
    IsSaveIoOnWholeFile();
    return &g_SvSaveMemory.pData->data;
}

// 0x002FB32C (name is ours)
SvNpcData* SvGardenData::GetNpcData()
{
    IsSaveIoOnNpcData();
    return &npcData;
}

// 0x002FBA0C (name is ours)
SvSharedData* GetSharedData()
{
    IsSaveIoOnMainData();
    IsSaveIoOnWholeFile();
    return &g_SvSaveMemory.pData->data.sharedData;
}

// 0x002FD00C (name is ours)
SvCensus* GetCensus()
{
    return &GetSharedData()->playerStats;
}

// 0x002FBA40 | libgarden [tier A]
SvPlayer* SvGetCurrentPlayer()
{
    if (IsSaveIoOnMainData()) {
        return NULL;
    }
    return g_pSvCurrentPlayer;
}

// 0x002FBA60 | libgarden [tier A]
DECOMP_NOINLINE SvPlayer* SvGetPlayer(PlayerSaveNumber save)
{
    if (IsSaveIoOnMainData()) {
        return NULL;
    }
    if (SvIsValidPlayerSaveNo(save)) {
        return GetGardenData()->players.Get(save);
    }
    if (SvIsNetPlayerSaveNo(save)) {
        u32 slot = SvGetNetPlayerSlot(save);
        if (slot < 4) {
            return &g_SvNetPlayerList.players.players[slot];
        }
    }
    return NULL;
}

// 0x002FF6CC (name after libgarden's sv.hpp)
PlayerSaveNumber SvGetPlayerSaveNo(netgame::PlayerNo player)
{
    u32 no = player < 4 ? player & 3 : netgame::GetMyPlayerNo();
    PlayerSaveNumber save = g_SvNetPlayerList.saveNumbers[no & 3];
    return SvIsAnyPlayerSaveNo(save) ? save : PLAYER_SAVE_NUMBER_INVALID;
}

// 0x002FEB2C | libgarden [tier A]
SvPlayer* SvGetNetPlayer(netgame::PlayerNo player)
{
    if (IsSaveIoOnMainData()) {
        return NULL;
    }
    u32 no = player < 4 ? player & 3 : netgame::GetMyPlayerNo();
    if (netgame::GetMyPlayerNo() == no) {
        return g_pSvCurrentPlayer;
    }
    PlayerSaveNumber save = SvGetPlayerSaveNo(static_cast<netgame::PlayerNo>(no));
    if (save == PLAYER_SAVE_NUMBER_INVALID) {
        return NULL;
    }
    return SvGetPlayer(save);
}

// 0x001DF8B4 | libgarden [tier A]
SvBestFriendList* SvGetBestFriendList()
{
    if (BsPartsSaveMgr<SvBestFriendList>::s_HasData && !BsPartsSaveMgr<SvBestFriendList>::s_IsDataBlocked) {
        return BsPartsSaveMgr<SvBestFriendList>::s_pData;
    }
    return NULL;
}
