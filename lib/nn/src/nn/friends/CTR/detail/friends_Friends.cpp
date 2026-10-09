#include "nn/friends/CTR/detail/friends_Friends.h"
#include <string.h>
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace friends {
namespace CTR {
namespace detail {
namespace {
// command headers (3dbrew "Friend Services")
const bit32 COMMAND_HAS_LOGGED_IN = 0x00010000;
const bit32 COMMAND_LOGIN = 0x00030002;
const bit32 COMMAND_LOGOUT = 0x00040000;
const bit32 COMMAND_GET_MY_FRIEND_KEY = 0x00050000;
const bit32 COMMAND_GET_MY_PREFERENCE = 0x00060000;
const bit32 COMMAND_GET_MY_PRESENCE = 0x00080000;
const bit32 COMMAND_GET_MY_PASSWORD = 0x00100040;
const bit32 COMMAND_GET_FRIEND_KEY_LIST = 0x00110080;
const bit32 COMMAND_UNSCRAMBLE_LOCAL_FRIEND_CODE = 0x001C0042;
const bit32 COMMAND_SET_NOTIFICATION_MASK = 0x00210040;
const bit32 COMMAND_GET_EVENT_NOTIFICATION = 0x00220040;
const bit32 COMMAND_GET_LAST_RESPONSE_RESULT = 0x00230000;
const bit32 COMMAND_RESULT_TO_ERROR_CODE = 0x00270040;
const bit32 COMMAND_REQUEST_GAME_AUTHENTICATION = 0x00280244;
const bit32 COMMAND_GET_GAME_AUTHENTICATION_DATA = 0x00290000;
const bit32 COMMAND_REQUEST_SERVICE_LOCATOR = 0x002A0204;
const bit32 COMMAND_GET_SERVICE_LOCATOR_DATA = 0x002B0000;
const bit32 COMMAND_SET_CLIENT_SDK_VERSION = 0x00320042;
const bit32 COMMAND_GET_MY_APPROACH_CONTEXT = 0x00330000;
const bit32 COMMAND_ADD_FRIEND_WITH_APPROACH = 0x00340046;

// the translation descriptors (3dbrew "IPC")
const bit32 DESCRIPTOR_PROCESS_ID = 0x20;
const bit32 DESCRIPTOR_COPY_HANDLE = 0;

// the sizes of the strings of the requests
const size_t KEY_HASH_SIZE = 9;
const size_t SERVICE_SIZE = 5;
const size_t NAME_SIZE = 11 * sizeof(wchar_t);

inline bit32 StaticBufferDescriptor(size_t size, s32 index)
{
    return (size << 14) | (index << 10) | 2;
}

inline void SetByte(bit32* word, u8 value)
{
    *reinterpret_cast<u8*>(word) = value;
}

inline nn::Result Send(bit32* command)
{
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

// a request whose reply goes into a buffer of the caller
inline nn::Result SendWithReceiveBuffer(bit32* command, void* buffer, size_t size)
{
    bit32* staticBuffers = nn::os::detail::GetIpcStaticBuffers();
    bit32 saved0 = staticBuffers[0];
    bit32 saved1 = staticBuffers[1];
    staticBuffers[0] = StaticBufferDescriptor(size, 0);
    staticBuffers[1] = reinterpret_cast<uptr>(buffer);
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    staticBuffers[0] = saved0;
    staticBuffers[1] = saved1;
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}
} // namespace

// 0x0097E8BC
nn::Handle s_Session;
// 0x0097E8B8
nn::Handle s_SessionAdmin;

// 0x0012BDE8 | fefates:bytes [tier B]
nn::Result nn::friends::CTR::detail::Friends::GetMyPresence(nnfriendsMyPresence* pPresence)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_MY_PRESENCE;
    return SendWithReceiveBuffer(command, pPresence, sizeof(nnfriendsMyPresence));
}

// 0x0012BE38 | fefates:callgraph [tier C]
nn::Result nn::friends::CTR::detail::Friends::GetFriendKeyList(nnfriendsFriendKey* pKeys, unsigned int* pCount, unsigned int offset, unsigned int size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_FRIEND_KEY_LIST;
    command[1] = offset;
    command[2] = size;
    bit32* staticBuffers = nn::os::detail::GetIpcStaticBuffers();
    bit32 saved0 = staticBuffers[0];
    bit32 saved1 = staticBuffers[1];
    staticBuffers[0] = StaticBufferDescriptor(size * sizeof(nnfriendsFriendKey), 0);
    staticBuffers[1] = reinterpret_cast<uptr>(pKeys);
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    staticBuffers[0] = saved0;
    staticBuffers[1] = saved1;
    if (result.IsFailure()) {
        return result;
    }
    *pCount = command[2];
    return nn::Result(command[1]);
}

// 0x0012BEA0 | fefates:callgraph [tier C]
nn::Result nn::friends::CTR::detail::Friends::SetClientSdkVersion(u32 version)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_CLIENT_SDK_VERSION;
    command[1] = version;
    command[2] = DESCRIPTOR_PROCESS_ID;
    return Send(command);
}

// 0x0012BEE0 | fefates:callgraph [tier C]
nn::Result nn::friends::CTR::detail::Friends::SetNotificationMask(bit32 mask)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_NOTIFICATION_MASK;
    command[1] = mask;
    return Send(command);
}

// 0x0048B2A8 | fefates:bytes [tier B]
nn::Result nn::friends::CTR::detail::Friends::HasLoggedIn(bool* pHasLoggedIn)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_HAS_LOGGED_IN;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pHasLoggedIn = *reinterpret_cast<bool*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x0048B2E4 | fefates:callgraph [tier C]
nn::Result nn::friends::CTR::detail::Friends::GetMyPassword(char* pPassword, size_t size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_MY_PASSWORD;
    command[1] = size;
    return SendWithReceiveBuffer(command, pPassword, size);
}

// 0x0048B340 | fefates:bytes [tier B]
nn::Result nn::friends::CTR::detail::Friends::GetMyFriendKey(nnfriendsFriendKey* pKey)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_MY_FRIEND_KEY;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pKey = *reinterpret_cast<nnfriendsFriendKey*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x0048B388 (name after 3dbrew)
nn::Result nn::friends::CTR::detail::Friends::GetMyPreference(bool* pIsPublicMode, bool* pIsShowGameName, bool* pIsShowPlayedGame)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_MY_PREFERENCE;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pIsPublicMode = *reinterpret_cast<bool*>(&command[2]);
    *pIsShowGameName = *reinterpret_cast<bool*>(&command[3]);
    *pIsShowPlayedGame = *reinterpret_cast<bool*>(&command[4]);
    return nn::Result(command[1]);
}

// 0x0048B3DC | fefates:bytes [tier B]
nn::Result nn::friends::CTR::detail::Friends::ResultToErrorCode(unsigned int* pCode, nn::Result result)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_RESULT_TO_ERROR_CODE;
    command[1] = result.GetPrintableBits();
    nn::Result ipcResult = nn::svc::SendSyncRequest(s_Session);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    *pCode = command[2];
    return nn::Result(command[1]);
}

// 0x0048B420 | fefates:callgraph [tier C]
nn::Result nn::friends::CTR::detail::Friends::GetEventNotification(nn::friends::CTR::EventNotification* pEvents, unsigned int count, bool* pIsOverflowed,
                                                                    unsigned int* pCount)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_EVENT_NOTIFICATION;
    command[1] = count;
    bit32* staticBuffers = nn::os::detail::GetIpcStaticBuffers();
    bit32 saved0 = staticBuffers[0];
    bit32 saved1 = staticBuffers[1];
    staticBuffers[0] = StaticBufferDescriptor(count * sizeof(nn::friends::CTR::EventNotification), 0);
    staticBuffers[1] = reinterpret_cast<uptr>(pEvents);
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    staticBuffers[0] = saved0;
    staticBuffers[1] = saved1;
    if (result.IsFailure()) {
        return result;
    }
    *pIsOverflowed = *reinterpret_cast<bool*>(&command[2]);
    *pCount = command[3];
    return nn::Result(command[1]);
}

// 0x0048B498 (name after 3dbrew)
nn::Result nn::friends::CTR::detail::Friends::GetMyApproachContext(nn::friends::CTR::ApproachContext* pContext)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_MY_APPROACH_CONTEXT;
    return SendWithReceiveBuffer(command, pContext, sizeof(nn::friends::CTR::ApproachContext));
}

// 0x0048B4E8 (name after 3dbrew)
nn::Result nn::friends::CTR::detail::Friends::AddFriendWithApproach(nn::Handle event, const nn::friends::CTR::ApproachContext* pContext, const u16* pScreenName,
                                                                     u32 screenNameLength)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_ADD_FRIEND_WITH_APPROACH;
    command[3] = event.GetPrintableBits();
    command[4] = StaticBufferDescriptor(sizeof(nn::friends::CTR::ApproachContext), 3);
    command[5] = reinterpret_cast<uptr>(pContext);
    command[1] = screenNameLength;
    command[2] = DESCRIPTOR_COPY_HANDLE;
    command[6] = StaticBufferDescriptor(screenNameLength * sizeof(u16), 4);
    command[7] = reinterpret_cast<uptr>(pScreenName);
    return Send(command);
}

// 0x0048B54C | fefates:bytes [tier B]
nn::Result nn::friends::CTR::detail::Friends::GetLastResponseResult()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_LAST_RESPONSE_RESULT;
    return Send(command);
}

// 0x0048B57C | fefates:bytes [tier B]
nn::Result nn::friends::CTR::detail::Friends::GetServiceLocatorData(nnfriesndsServiceLocatorData* pData)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_SERVICE_LOCATOR_DATA;
    return SendWithReceiveBuffer(command, pData, sizeof(nnfriesndsServiceLocatorData));
}

// 0x0048B5CC | fefates:bytes [tier B]
nn::Result nn::friends::CTR::detail::Friends::RequestServiceLocator(nn::Handle event, unsigned int serverId, const char* pKeyHash, const char* pService,
                                                                     unsigned char a, unsigned char b)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_REQUEST_SERVICE_LOCATOR;
    command[1] = serverId;
    memcpy(&command[2], pKeyHash, KEY_HASH_SIZE);
    memcpy(&command[5], pService, SERVICE_SIZE);
    SetByte(&command[7], a);
    SetByte(&command[8], b);
    command[9] = DESCRIPTOR_PROCESS_ID;
    command[12] = event.GetPrintableBits();
    command[11] = DESCRIPTOR_COPY_HANDLE;
    return Send(command);
}

// 0x0048B650 | fefates:bytes [tier B]
nn::Result nn::friends::CTR::detail::Friends::GetGameAuthenticationData(nnfriesndsGameAuthenticationData* pData)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_GAME_AUTHENTICATION_DATA;
    return SendWithReceiveBuffer(command, pData, sizeof(nnfriesndsGameAuthenticationData));
}

// 0x0048B6A0 | fefates:bytes [tier B]
nn::Result nn::friends::CTR::detail::Friends::RequestGameAuthentication(nn::Handle event, unsigned int serverId, const wchar_t* pName, unsigned char a,
                                                                         unsigned char b)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_REQUEST_GAME_AUTHENTICATION;
    command[1] = serverId;
    memcpy(&command[2], pName, NAME_SIZE);
    SetByte(&command[8], a);
    SetByte(&command[9], b);
    command[13] = event.GetPrintableBits();
    command[10] = DESCRIPTOR_PROCESS_ID;
    command[12] = DESCRIPTOR_COPY_HANDLE;
    return Send(command);
}

// 0x0048B730 | fefates:callgraph [tier C]
nn::Result nn::friends::CTR::detail::Friends::UnscrambleLocalFriendCode(u64* pFriendCodes, const nn::friends::CTR::ScrambledFriendCode* pScrambled,
                                                                         unsigned int count)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_UNSCRAMBLE_LOCAL_FRIEND_CODE;
    command[3] = reinterpret_cast<uptr>(pScrambled);
    command[1] = count;
    command[2] = StaticBufferDescriptor(count * sizeof(nn::friends::CTR::ScrambledFriendCode), 1);
    return SendWithReceiveBuffer(command, pFriendCodes, count * sizeof(u64));
}

// 0x0048B7A4 | fefates:bytes [tier B]
nn::Result nn::friends::CTR::detail::Friends::Login(nn::Handle event)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[2] = event.GetPrintableBits();
    command[0] = COMMAND_LOGIN;
    command[1] = DESCRIPTOR_COPY_HANDLE;
    return Send(command);
}

// 0x0048B7E4 | fefates:bytes [tier B]
nn::Result nn::friends::CTR::detail::Friends::Logout()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_LOGOUT;
    return Send(command);
}

} // namespace detail
} // namespace CTR
} // namespace friends
} // namespace nn
