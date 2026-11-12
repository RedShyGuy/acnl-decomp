#include "netgame/dAvatarId.h"

#include "Other/dFriendList.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvPlayer.h"
#include "net/dNetBase64.h"
#include <cstring>

namespace netgame {
// 0x00625778 (name is ours)
void netgame::AvatarIdList::SetFromSave()
{
    s32 i = 0;
    GetGardenPlus();
    SvBestFriendList* list = SvGetBestFriendList();
    if (list != NULL) {
        u32 num = list->GetFriendNum();
        for (u32 j = 0; j < num; j++) {
            m_Ids[i].Set(list->GetFriend(j)->personalId);
            i++;
        }
    }
    for (; i < NUM; i++) {
        m_Ids[i].Reset();
    }
}

// 0x006257F4 (name is ours)
void netgame::AvatarIdList::Assign(const AvatarIdList& other)
{
    std::memcpy(static_cast<void*>(this), &other, sizeof(AvatarIdList));
}

// 0x006257FC (name is ours)
void netgame::AvatarIdList::Reset()
{
    for (s32 i = 0; i < NUM; i++) {
        m_Ids[i].Reset();
    }
}

// 0x00625820 (name is ours)
netgame::AvatarIdList::AvatarIdList()
{
    for (s32 i = 0; i < NUM; i++) {
        m_Ids[i].Reset();
    }
}

// 0x00624A4C (name is ours)
void DecodePrincipal(Principal* principal, const char16* text, u32 length)
{
    if (principal == NULL || text == NULL) {
        return;
    }
    char work[16];
    net::DecodeBase64(text, length > 16 ? 16 : length, reinterpret_cast<u8*>(principal), sizeof(Principal), work, sizeof(work));
}

// 0x00624A90 (name is ours)
void EncodePrincipal(char16* text, const Principal* principal)
{
    if (text == NULL) {
        return;
    }
    char work[16];
    net::EncodeBase64(reinterpret_cast<const u8*>(principal), sizeof(Principal), text, 16, work, sizeof(work));
}

// 0x00624AC8 (name is ours)
netgame::Principal& netgame::Principal::Set(const SvPersonalId& id, const u32& newPrincipalId)
{
    principalId = newPrincipalId;
    avatarId.Set(id);
    return *this;
}

// 0x00624A0C (name is ours)
void netgame::Principal::SetFromMyself()
{
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL) {
        principalId = FriendList::s_pInstance->GetMyPrincipalId();
        avatarId.Set(player->playerInfo);
    }
}

// 0x00624AE0 (name is ours)
netgame::Principal::Principal() : principalId(0)
{
}

// 0x00624AFC (name is ours)
netgame::Principal::~Principal()
{
    // (only the members are destroyed)
}

// 0x0051B7AC (name is ours)
netgame::FriendId::FriendId()
{
    localFriendCode[0] = 0;
    localFriendCode[1] = 0;
}

// 0x00748318 (name is ours)
bool netgame::FriendId::IsDifferent(const FriendId& other) const
{
    if (principalId != 0 && other.principalId != 0) {
        return other.principalId != principalId;
    }
    if ((other.localFriendCode[0] | other.localFriendCode[1]) == 0) {
        return true;
    }
    return localFriendCode[0] != other.localFriendCode[0] || localFriendCode[1] != other.localFriendCode[1];
}

// 0x00626214 (name is ours)
DECOMP_NOIPA netgame::AvatarId::~AvatarId()
{
    // empty (in the original too)
}

// 0x006261A8 (name is ours)
void netgame::Principal::Reset()
{
    principalId = 0;
    avatarId.Reset();
}

// 0x006261B4 (name is ours)
void netgame::AvatarId::Reset()
{
    playerId = INVALID_PLAYER_ID;
    townId = 0;
}

// 0x006261D0 (name is ours)
void netgame::AvatarId::Set(const SvPersonalId& id)
{
    playerId = id.playerData.pid;
    townId = id.townData.tid;
}

// 0x006261E4 (name is ours)
void netgame::AvatarId::Set(const u16& newPlayerId, const u16& newTownId)
{
    playerId = newPlayerId;
    townId = newTownId;
}

// 0x0075F994 (name is ours)
s32 netgame::AvatarIdList::Find(const AvatarId& id) const
{
    if (!id.IsValid()) {
        return -1;
    }
    for (s32 i = 0; i < NUM; i++) {
        if (m_Ids[i].IsValid() && m_Ids[i] == id) {
            return i;
        }
    }
    return -1;
}
} // namespace netgame
