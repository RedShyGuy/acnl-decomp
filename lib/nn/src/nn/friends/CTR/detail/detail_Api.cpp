// friends_Api.cpp (static initializer __sti___15_friends_Api_cpp at 0x00792F84: the three locks)
#include "nn/friends/CTR/detail/detail_Api.h"
#include <string.h>
#include "nn/ac/CTR/CTR_Api.h"
#include "nn/cfg/CTR/CTR_Api.h"
#include "nn/friends/CTR/detail/friends_Friends.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_Event.h"
#include "nn/srv/srv_Api.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace friends {
namespace CTR {
namespace detail {
namespace {
// results (module 49; the names are ours)
const bit32 RESULT_ALREADY_INITIALIZED = 0xD8A0C7F9; // permanent, invalid state, 1017
const bit32 RESULT_NOT_INITIALIZED = 0xD8A0C7F8;     // permanent, invalid state, 1016
const bit32 RESULT_INVALID_POINTER = 0xE0E0C7F6;     // usage, invalid argument, 1014
const bit32 RESULT_INVALID_HANDLE = 0xE0E0C7F7;      // usage, invalid argument, 1015
const bit32 RESULT_OUT_OF_RANGE = 0xE0E0C7E9;        // usage, invalid argument, 1001
const bit32 RESULT_NOT_CONNECTED = 0xD8A0C405;       // permanent, invalid state, 5
const bit32 RESULT_RESTRICTED = 0xD8A0C4F9;          // permanent, invalid state, 249

const char SERVICE_NAME[] = "frd:u";
// what SetClientSdkVersion tells frd:u
const u32 CLIENT_SDK_VERSION = 0x0B0500C8;
// the most friends of a list
const u32 MAX_FRIEND_COUNT = 100;
const size_t PASSWORD_SIZE = 32;

// set: friends may be added whatever the parental controls say (nothing in ACNL sets it)
// 0x00975F08
bool s_IsFriendRegistrationAllowed;
// 0x00975F0C
s32 s_InitializeCount;
// GetMyApproachContext / AddFriendWithApproach
// 0x00AE1B44
nn::os::CriticalSection s_ApproachLock;
// (initialized and finalized with s_ApproachLock, not used otherwise)
// 0x00AE1B50
nn::os::CriticalSection s_Lock3;
// 0x00AE1B5C
nn::os::CriticalSection s_Lock((nn::os::CriticalSection::InitializeTag()));

// one of the sessions is open (inline everywhere)
inline bool IsAnyInitialized()
{
    return IsInitialized() || IsInitializedAdmin();
}

// (inline in Initialize)
inline nn::Result GetMyPresence(nnfriendsMyPresence* pPresence)
{
    if (!IsAnyInitialized()) {
        return RESULT_NOT_INITIALIZED;
    }
    return Friends::GetMyPresence(pPresence);
}
} // namespace

// word 8 of the presence read by Initialize (external: only written in ACNL; name is ours)
// 0x00975F10
u32 s_MyPresenceValue08;

// 0x00125524 | fefates:bytes [tier B]
nn::Result Initialize()
{
    s_Lock.Enter();
    nn::Result result;
    if (IsInitializedAdmin()) {
        result = RESULT_ALREADY_INITIALIZED;
    } else if (IsInitialized()) {
        s_InitializeCount++;
        result = nn::Result();
    } else {
        result = nn::srv::Initialize();
        if (result.IsSuccess()) {
            result = nn::srv::GetServiceHandle(&s_Session, SERVICE_NAME, strlen(SERVICE_NAME), 0);
            if (result.IsSuccess()) {
                s_InitializeCount++;
                Friends::SetClientSdkVersion(CLIENT_SDK_VERSION);
                s_ApproachLock.Initialize();
                s_IsFriendRegistrationAllowed = false;
                s_Lock3.Initialize();
                nnfriendsMyPresence presence;
                result = GetMyPresence(&presence);
                if (result.IsSuccess()) {
                    s_MyPresenceValue08 = reinterpret_cast<u32*>(presence.data)[2];
                }
            }
        }
    }
    s_Lock.Exit();
    return result;
}

// 0x001256A0 | fefates:bytes [tier B]
nn::Result GetFriendKeyList(nnfriendsFriendKey* pKeys, unsigned int* pCount, unsigned int offset, unsigned int size)
{
    if (!IsAnyInitialized()) {
        return RESULT_NOT_INITIALIZED;
    }
    if (pKeys == 0 || pCount == 0) {
        return RESULT_INVALID_POINTER;
    }
    if (size > MAX_FRIEND_COUNT) {
        return RESULT_OUT_OF_RANGE;
    }
    return Friends::GetFriendKeyList(pKeys, pCount, offset, size);
}

// 0x0012BDAC | fefates:callgraph [tier C]
bool IsInitializedAdmin()
{
    s_Lock.Enter();
    bool isInitialized = s_SessionAdmin.IsValid();
    s_Lock.Exit();
    return isInitialized;
}

// 0x001374C8 | fefates:bytes [tier B]
nn::Result Finalize()
{
    s_Lock.Enter();
    if (IsInitialized()) {
        if (--s_InitializeCount == 0) {
            s_ApproachLock.Finalize();
            s_Lock3.Finalize();
            s_MyPresenceValue08 = 0;
            nn::Result result = nn::svc::CloseHandle(s_Session);
            if (result.IsFailure()) {
                s_Lock.Exit();
                return result;
            }
        }
    }
    s_Lock.Exit();
    return nn::Result();
}

// 0x0048A904 | fefates:bytes [tier B]
bool HasLoggedIn()
{
    if (!IsAnyInitialized()) {
        return false;
    }
    bool hasLoggedIn;
    if (Friends::HasLoggedIn(&hasLoggedIn).IsFailure()) {
        return false;
    }
    return hasLoggedIn;
}

// 0x0048A968 | fefates:bytes [tier B]
nn::Result GetMyPassword(char* pPassword)
{
    if (!IsAnyInitialized()) {
        return RESULT_NOT_INITIALIZED;
    }
    if (pPassword == 0) {
        return RESULT_INVALID_POINTER;
    }
    return Friends::GetMyPassword(pPassword, PASSWORD_SIZE);
}

// 0x0048A9E0 (name is ours)
bool IsInitialized()
{
    s_Lock.Enter();
    bool isInitialized = s_InitializeCount != 0;
    s_Lock.Exit();
    return isInitialized;
}

// 0x0048AA1C | fefates:bytes [tier B]
u32 GetMyPrincipalId()
{
    if (!IsAnyInitialized()) {
        return 0;
    }
    nnfriendsFriendKey key;
    if (Friends::GetMyFriendKey(&key).IsFailure()) {
        return 0;
    }
    return key.principalId;
}

// 0x0048AA88 | fefates:bytes [tier B]
u32 ResultToErrorCode(const nn::Result& result)
{
    if (!IsAnyInitialized()) {
        return 0;
    }
    unsigned int code = 0;
    Friends::ResultToErrorCode(&code, result);
    return code;
}

// 0x0048AAF0 (name is ours)
bool IsPlayedGameHidden()
{
    if (!IsAnyInitialized()) {
        return false;
    }
    bool isPublicMode;
    bool isShowGameName;
    bool isShowPlayedGame;
    if (Friends::GetMyPreference(&isPublicMode, &isShowGameName, &isShowPlayedGame).IsFailure()) {
        return false;
    }
    return !isShowPlayedGame;
}

// 0x0048AB68 (name is ours, after the cfg function)
bool IsRestrictFriendRegistration()
{
    if (!IsAnyInitialized()) {
        return true;
    }
    s_ApproachLock.Enter();
    if (s_IsFriendRegistrationAllowed) {
        s_ApproachLock.Exit();
        return false;
    }
    bool isRestricted = nn::cfg::CTR::IsRestrictFriendRegistration();
    s_ApproachLock.Exit();
    return isRestricted;
}

// 0x0048ABFC (name after 3dbrew)
u32 GetEventNotification(nn::friends::CTR::EventNotification* pEvents, unsigned int count, bool* pIsOverflowed)
{
    if (!IsAnyInitialized()) {
        return 0;
    }
    if (pEvents == 0) {
        return 0;
    }
    bool isOverflowed;
    unsigned int readCount;
    Friends::GetEventNotification(pEvents, count, &isOverflowed, &readCount);
    if (pIsOverflowed != 0) {
        *pIsOverflowed = isOverflowed;
    }
    return readCount;
}

// 0x0048AC8C (name after 3dbrew)
nn::Result GetMyApproachContext(nn::friends::CTR::ApproachContext* pContext)
{
    if (!IsAnyInitialized()) {
        return RESULT_NOT_INITIALIZED;
    }
    if (pContext == 0) {
        return RESULT_INVALID_POINTER;
    }
    s_ApproachLock.Enter();
    if (IsRestrictFriendRegistration()) {
        s_ApproachLock.Exit();
        return RESULT_RESTRICTED;
    }
    nn::Result result = Friends::GetMyApproachContext(pContext);
    if (result.IsFailure()) {
        memset(pContext, 0, sizeof(*pContext));
    }
    s_ApproachLock.Exit();
    return result;
}

// 0x0048AD60 (name after 3dbrew)
nn::Result AddFriendWithApproach(nn::os::Event* pEvent, const nn::friends::CTR::ApproachContext* pContext)
{
    if (!IsAnyInitialized()) {
        return RESULT_NOT_INITIALIZED;
    }
    if (pEvent == 0) {
        return RESULT_INVALID_POINTER;
    }
    if (!pEvent->GetHandle().IsValid()) {
        return RESULT_INVALID_HANDLE;
    }
    s_ApproachLock.Enter();
    if (IsRestrictFriendRegistration()) {
        s_ApproachLock.Exit();
        return RESULT_RESTRICTED;
    }
    // an empty screen name
    u32 screenName = 0;
    nn::Result result = Friends::AddFriendWithApproach(pEvent->GetHandle(), pContext, reinterpret_cast<u16*>(&screenName), 1);
    s_ApproachLock.Exit();
    return result;
}

// 0x0048AE4C | fefates:callgraph [tier C]
nn::Result GetLastResponseResult()
{
    if (!IsAnyInitialized()) {
        return RESULT_NOT_INITIALIZED;
    }
    return Friends::GetLastResponseResult();
}

// 0x0048AEA8 | fefates:callgraph [tier C]
nn::Result GetServiceLocatorData(nnfriesndsServiceLocatorData* pData)
{
    if (!IsAnyInitialized()) {
        return RESULT_NOT_INITIALIZED;
    }
    if (pData == 0) {
        return RESULT_INVALID_POINTER;
    }
    return Friends::GetServiceLocatorData(pData);
}

// 0x0048AF1C | fefates:callgraph [tier C]
nn::Result GetGameAuthenticationData(nnfriesndsGameAuthenticationData* pData)
{
    if (!IsAnyInitialized()) {
        return RESULT_NOT_INITIALIZED;
    }
    if (pData == 0) {
        return RESULT_INVALID_POINTER;
    }
    return Friends::GetGameAuthenticationData(pData);
}

// 0x0048AF90 (name after 3dbrew)
nn::Result UnscrambleLocalFriendCode(u64* pFriendCodes, const nn::friends::CTR::ScrambledFriendCode* pScrambled, unsigned int count)
{
    if (!IsAnyInitialized()) {
        return RESULT_NOT_INITIALIZED;
    }
    if (pFriendCodes == 0 || pScrambled == 0) {
        return RESULT_INVALID_POINTER;
    }
    if (count > MAX_FRIEND_COUNT) {
        return RESULT_OUT_OF_RANGE;
    }
    return Friends::UnscrambleLocalFriendCode(pFriendCodes, pScrambled, count);
}

// 0x0048B028 | fefates:bytes [tier B]
nn::Result RequestServiceLocatorWithoutLogin(nn::os::Event* pEvent, unsigned int serverId, const char* pKeyHash, const char* pService, unsigned char a,
                                             unsigned char b)
{
    if (!IsAnyInitialized()) {
        return RESULT_NOT_INITIALIZED;
    }
    if (!nn::ac::CTR::IsConnected()) {
        return RESULT_NOT_CONNECTED;
    }
    if (pEvent == 0 || pKeyHash == 0 || pService == 0) {
        return RESULT_INVALID_POINTER;
    }
    if (!pEvent->GetHandle().IsValid()) {
        return RESULT_INVALID_HANDLE;
    }
    return Friends::RequestServiceLocator(pEvent->GetHandle(), serverId, pKeyHash, pService, a, b);
}

// 0x0048B0F4 | fefates:bytes [tier B]
nn::Result RequestGameAuthenticationWithoutLogin(nn::os::Event* pEvent, unsigned int serverId, const wchar_t* pName, unsigned char a, unsigned char b)
{
    if (!IsAnyInitialized()) {
        return RESULT_NOT_INITIALIZED;
    }
    if (!nn::ac::CTR::IsConnected()) {
        return RESULT_NOT_CONNECTED;
    }
    if (pEvent == 0 || pName == 0) {
        return RESULT_INVALID_POINTER;
    }
    if (!pEvent->GetHandle().IsValid()) {
        return RESULT_INVALID_HANDLE;
    }
    return Friends::RequestGameAuthentication(pEvent->GetHandle(), serverId, pName, a, b);
}

// 0x0048B1B0 | fefates:bytes [tier B]
nn::Result Login(nn::os::Event* pEvent)
{
    if (!IsAnyInitialized()) {
        return RESULT_NOT_INITIALIZED;
    }
    if (!nn::ac::CTR::IsConnected()) {
        return RESULT_NOT_CONNECTED;
    }
    if (pEvent == 0) {
        return RESULT_INVALID_POINTER;
    }
    if (!pEvent->GetHandle().IsValid()) {
        return RESULT_INVALID_HANDLE;
    }
    return Friends::Login(pEvent->GetHandle());
}

// 0x0048B24C | fefates:callgraph [tier C]
nn::Result Logout()
{
    if (!IsAnyInitialized()) {
        return RESULT_NOT_INITIALIZED;
    }
    return Friends::Logout();
}

} // namespace detail
} // namespace CTR
} // namespace friends
} // namespace nn
