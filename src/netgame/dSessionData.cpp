#include "Sv/dSvPersonalId.h"
#include "net/dNetBase64.h"
#include "netgame/dGameFunctions.h"
#include "netgame/dNetGameMgr.h"
#include "script/dWordCPtrSv.h"
#include "script/dWordPtrSv.h"
#include "sead/seadWFixedSafeString.h"
#include <cstring>

namespace netgame {

// the town id of a session that has none (name is ours)
// 0x0088F7B4
extern const u16 UNKNOWN_TOWN_ID = 0xFFFF;

// 0x00A22D68 (name is ours)
SessionData s_SessionData;

namespace {
const u32 HEADER_SIZE = 6;
const u32 HEADER_TEXT_LENGTH = 8;
const u32 TEXT_LENGTH = SessionData::TEXT_LENGTH;

// 0x0087CD8C
const wchar_t s_Separator[] = L"@";

struct SessionHeader
{
    u16 townId;   // 0x0
    u16 playerId; // 0x2
    u8 region;    // 0x4
    u8 gender;    // 0x5
};
ASSERT_SIZE(SessionHeader, HEADER_SIZE);

// the text from *pos up to the next separator, false if none is left
DECOMP_ALWAYS_INLINE bool GetToken(const sead::SafeStringBase<wchar_t>& text, s32* pos, wchar_t* token, s32 tokenSize)
{
    if (*pos > text.getLength()) {
        return false;
    }
    s32 length = 0;
    const wchar_t* src = text.getCString() + *pos;
    while (length < tokenSize - 1) {
        wchar_t c = *src;
        if (c == sead::SafeStringBase<wchar_t>::s_NullChar || c == s_Separator[0]) {
            break;
        }
        token[length] = c;
        length++;
        src++;
    }
    token[length] = sead::SafeStringBase<wchar_t>::s_NullChar;
    *pos += length + 1;
    return length > 0;
}
} // namespace

// 0x00629024 (name is ours)
void ReadSessionPlayers(const void* data, SvPersonalId* players)
{
    SessionHeader header;
    if (net::DecodeBase64(static_cast<const char16*>(data), HEADER_TEXT_LENGTH, reinterpret_cast<u8*>(&header), HEADER_SIZE,
                          static_cast<sead::Heap*>(NULL)) != HEADER_SIZE) {
        header.townId = UNKNOWN_TOWN_ID;
        header.playerId = UNKNOWN_PLAYER_ID;
        header.region = GetDefaultRegion();
        header.gender = 0;
    }
    sead::WFixedSafeString<256> text;
    text.copy(sead::SafeStringBase<wchar_t>(static_cast<const wchar_t*>(data) + HEADER_TEXT_LENGTH));
    s32 pos = 0;
    wchar_t townName[9];
    wchar_t playerName[9];
    s32 i = 0;
    for (; i < 4; i++) {
        if (!GetToken(text, &pos, townName, 9) || !GetToken(text, &pos, playerName, 9)) {
            break;
        }
        SvTownId town;
        SvPersonalId personalId;
        if (i != 0) {
            MakeTownId(&town, reinterpret_cast<const char16*>(townName), GetDefaultRegion());
            MakePersonalId(&personalId, reinterpret_cast<const char16*>(playerName), UNKNOWN_PLAYER_ID, 0, &town);
        } else {
            MakeTownId(&town, reinterpret_cast<const char16*>(townName), header.townId, header.region);
            MakePersonalId(&personalId, reinterpret_cast<const char16*>(playerName), header.playerId, header.gender, &town);
        }
        AssignPersonalId(&players[i], &personalId);
    }
    for (; i < 4; i++) {
        if (i != 0) {
            ClearPersonalId(&players[i]);
        }
    }
}

// 0x006295D4 (name is ours)
void SetSessionData(const SvPersonalId* players, u16 param)
{
    s_GateParam = param;
    BuildSessionData(players);
}

// 0x006295E4 (name is ours)
void BuildSessionData(const SvPersonalId* players)
{
    SessionHeader header;
    header.townId = players[0].townData.tid;
    header.playerId = players[0].playerData.pid;
    header.region = players[0].townData.unk_0x14;
    header.gender = players[0].playerData.gender & 1;
    if (net::EncodeBase64(reinterpret_cast<const u8*>(&header), HEADER_SIZE, s_SessionData.text, TEXT_LENGTH,
                          static_cast<sead::Heap*>(NULL)) != HEADER_TEXT_LENGTH) {
        for (u32 i = 0; i < HEADER_TEXT_LENGTH; i++) {
            s_SessionData.text[i] = s_Separator[0];
        }
    }
    script::WordCPtrSv separator(reinterpret_cast<const char16*>(s_Separator), 2);
    script::WordPtrSv text(s_SessionData.text + HEADER_TEXT_LENGTH, TEXT_LENGTH - HEADER_TEXT_LENGTH);
    text.Clear();
    for (s32 i = 0; i < 4; i++) {
        const SvPersonalId& player = players[i];
        if (!player.IsValid()) {
            continue;
        }
        text.Append(script::WordCPtrSv(player.townData.dataTownName, 9));
        text.Append(separator);
        text.Append(script::WordCPtrSv(player.playerData.playerName, 9));
        text.Append(separator);
    }
    s_SessionData.length = text.GetLength() + HEADER_TEXT_LENGTH;
}

// 0x00629788 (name is ours)
void SetGateParam(u16 param)
{
    NetGameMgr::s_pInstance->m_GateMgr.IsClosed(GateMgr::FLAG_TRAIN_STATION);
    s_GateParam = param;
}
} // namespace netgame
