#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/friends/CTR/friends_Types.h"

namespace nn {
namespace friends {
namespace CTR {
namespace detail {
// The commands of the service frd:u (3dbrew "Friend Services"); all static, on s_Session. The
// function names are from the symbols or after 3dbrew (the symbols mix up some of them).
class Friends
{
public:
    static nn::Result HasLoggedIn(bool* pHasLoggedIn); // 0x0048B2A8 | fefates:bytes [tier B]
    static nn::Result Login(nn::Handle event); // 0x0048B7A4 | fefates:bytes [tier B]
    static nn::Result Logout(); // 0x0048B7E4 | fefates:bytes [tier B]
    static nn::Result GetMyFriendKey(nnfriendsFriendKey* pKey); // 0x0048B340 | fefates:bytes [tier B]
    static nn::Result GetMyPreference(bool* pIsPublicMode, bool* pIsShowGameName, bool* pIsShowPlayedGame); // 0x0048B388 (name after 3dbrew)
    static nn::Result GetMyPresence(nnfriendsMyPresence* pPresence); // 0x0012BDE8 | fefates:bytes [tier B]
    static nn::Result GetMyPassword(char* pPassword, size_t size); // 0x0048B2E4 | fefates:callgraph [tier C]
    static nn::Result GetFriendKeyList(nnfriendsFriendKey* pKeys, unsigned int* pCount, unsigned int offset, unsigned int size); // 0x0012BE38 | fefates:callgraph [tier C]
    static nn::Result UnscrambleLocalFriendCode(u64* pFriendCodes, const nn::friends::CTR::ScrambledFriendCode* pScrambled, unsigned int count); // 0x0048B730 | fefates:callgraph [tier C]
    static nn::Result SetNotificationMask(bit32 mask); // 0x0012BEE0 | fefates:callgraph [tier C]
    static nn::Result GetEventNotification(nn::friends::CTR::EventNotification* pEvents, unsigned int count, bool* pIsOverflowed, unsigned int* pCount); // 0x0048B420 | fefates:callgraph [tier C]
    static nn::Result GetLastResponseResult(); // 0x0048B54C | fefates:bytes [tier B]
    static nn::Result ResultToErrorCode(unsigned int* pCode, nn::Result result); // 0x0048B3DC | fefates:bytes [tier B]
    static nn::Result RequestGameAuthentication(nn::Handle event, unsigned int serverId, const wchar_t* pName, unsigned char a, unsigned char b); // 0x0048B6A0 | fefates:bytes [tier B]
    static nn::Result GetGameAuthenticationData(nnfriesndsGameAuthenticationData* pData); // 0x0048B650 | fefates:bytes [tier B]
    static nn::Result RequestServiceLocator(nn::Handle event, unsigned int serverId, const char* pKeyHash, const char* pService, unsigned char a, unsigned char b); // 0x0048B5CC | fefates:bytes [tier B]
    static nn::Result GetServiceLocatorData(nnfriesndsServiceLocatorData* pData); // 0x0048B57C | fefates:bytes [tier B]
    static nn::Result SetClientSdkVersion(u32 version); // 0x0012BEA0 | fefates:callgraph [tier C]
    static nn::Result GetMyApproachContext(nn::friends::CTR::ApproachContext* pContext); // 0x0048B498 (name after 3dbrew)
    static nn::Result AddFriendWithApproach(nn::Handle event, const nn::friends::CTR::ApproachContext* pContext, const u16* pScreenName, u32 screenNameLength); // 0x0048B4E8 (name after 3dbrew)
};

// the sessions of frd:u and frd:a (frd:a is never opened by ACNL; names are ours)
extern nn::Handle s_Session; // 0x0097E8BC
extern nn::Handle s_SessionAdmin; // 0x0097E8B8
} // namespace detail
} // namespace CTR
} // namespace friends
} // namespace nn
