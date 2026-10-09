#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/friends/CTR/friends_Types.h"

namespace nn {
namespace os {
class Event;
}
} // namespace nn

namespace nn {
namespace friends {
namespace CTR {
namespace detail {
// counted: the session is made by the first call and closed by the last Finalize
nn::Result Initialize(); // 0x00125524 | fefates:bytes [tier B]
nn::Result Finalize(); // 0x001374C8 | fefates:bytes [tier B]
// (symbols.json names 0x0048A9E0 nn::ac::CTR::IsInitialized; it is this one, after its globals)
bool IsInitialized(); // 0x0048A9E0 (name is ours)
// the session of frd:a (never opened by ACNL)
DECOMP_NOINLINE bool IsInitializedAdmin(); // 0x0012BDAC | fefates:callgraph [tier C]

nn::Result GetFriendKeyList(nnfriendsFriendKey* pKeys, unsigned int* pCount, unsigned int offset, unsigned int size); // 0x001256A0 | fefates:bytes [tier B]
bool HasLoggedIn(); // 0x0048A904 | fefates:bytes [tier B]
nn::Result GetMyPassword(char* pPassword); // 0x0048A968 | fefates:bytes [tier B]
// 0 without a connection to the service
u32 GetMyPrincipalId(); // 0x0048AA1C | fefates:bytes [tier B]
u32 ResultToErrorCode(const nn::Result& result); // 0x0048AA88 | fefates:bytes [tier B]
// the preference "show the played game" turned off (false without the service)
bool IsPlayedGameHidden(); // 0x0048AAF0 (name is ours)
// adding friends is not allowed (parental controls); true without the service
bool IsRestrictFriendRegistration(); // 0x0048AB68 (name is ours, after the cfg function)
// the number of notifications read into pEvents
u32 GetEventNotification(nn::friends::CTR::EventNotification* pEvents, unsigned int count, bool* pIsOverflowed); // 0x0048ABFC (name after 3dbrew)
nn::Result GetMyApproachContext(nn::friends::CTR::ApproachContext* pContext); // 0x0048AC8C (name after 3dbrew)
nn::Result AddFriendWithApproach(nn::os::Event* pEvent, const nn::friends::CTR::ApproachContext* pContext); // 0x0048AD60 (name after 3dbrew)
nn::Result GetLastResponseResult(); // 0x0048AE4C | fefates:callgraph [tier C]
nn::Result GetServiceLocatorData(nnfriesndsServiceLocatorData* pData); // 0x0048AEA8 | fefates:callgraph [tier C]
nn::Result GetGameAuthenticationData(nnfriesndsGameAuthenticationData* pData); // 0x0048AF1C | fefates:callgraph [tier C]
// (symbols.json names 0x0048AF90 GetFriendAttributeFlags; it calls the command UnscrambleLocalFriendCode)
nn::Result UnscrambleLocalFriendCode(u64* pFriendCodes, const nn::friends::CTR::ScrambledFriendCode* pScrambled, unsigned int count); // 0x0048AF90 (name after 3dbrew)
nn::Result RequestServiceLocatorWithoutLogin(nn::os::Event* pEvent, unsigned int serverId, const char* pKeyHash, const char* pService, unsigned char a, unsigned char b); // 0x0048B028 | fefates:bytes [tier B]
nn::Result RequestGameAuthenticationWithoutLogin(nn::os::Event* pEvent, unsigned int serverId, const wchar_t* pName, unsigned char a, unsigned char b); // 0x0048B0F4 | fefates:bytes [tier B]
nn::Result Login(nn::os::Event* pEvent); // 0x0048B1B0 | fefates:bytes [tier B]
nn::Result Logout(); // 0x0048B24C | fefates:callgraph [tier C]
} // namespace detail
} // namespace CTR
} // namespace friends
} // namespace nn
