#include "netgame/dGameGlobals.h"
#include "netgame/dGameFunctions.h"
#include "netgame/dNetGameMgr.h"

#include "Sv/dSvApi.h"
#include "Sv/dSvBestFriendList.h"
#include "Sv/dSvPlayer.h"
#include "net/dNet.h"
#include "net/dNetBase64.h"
#include "script/dStrMgr.h"
#include "script/dWordCPtrSv.h"
#include "time/time_Extern.h"
#include "script/dWordPtrSv.h"
#include "sead/seadFixedSafeStringBase.h"
#include "sead/seadNew.h"
#include "sead/seadWFixedSafeString.h"
#include <cstring>

namespace netgame {


namespace {
const s32 MESSAGE_NUM = 100;
const u8 HEADER_VERSION = 3;
// the header of a text as Base64 characters
const u32 HEADER_TEXT_LENGTH = 212;
const s32 CONNECT_WAIT_FRAMES = 60;

// 0x0087CD90 (name is ours)
extern const u32 s_NoPrincipal = 0;
// 0x0087CD94
extern const wchar_t s_Separator[] = L"@";

const u32 BIT_ON_NEW_FRIEND = 1 << 6;

DECOMP_ALWAYS_INLINE void CopyWord(char16* dst, u32 dstSize, const char16* src, u32 srcSize)
{
    script::WordCPtrSv source(src, srcSize);
    script::WordPtrSv target(dst, dstSize);
    target.Copy(source);
}

DECOMP_ALWAYS_INLINE void CopyMessage(NetGameMgr::BestFriendMgr::Message* dst, const NetGameMgr::BestFriendMgr::Message* src)
{
    std::memcpy(dst, src, sizeof(NetGameMgr::BestFriendMgr::Message));
}

DECOMP_ALWAYS_INLINE NetGameMgr::BestFriendMgr::Message* AllocMessages(sead::Heap* heap)
{
    typedef NetGameMgr::BestFriendMgr::Message Message;
    Message* buffer = static_cast<Message*>(operator new(sizeof(Message) * MESSAGE_NUM, heap, 4, std::nothrow));
    if (buffer != NULL) {
        for (s32 i = 0; i < MESSAGE_NUM; i++) {
            new (&buffer[i]) Message();
        }
    }
    return buffer;
}

DECOMP_ALWAYS_INLINE void AppendString(sead::BufferedSafeStringBase<wchar_t>* buffer, const sead::SafeStringBase<wchar_t>& str)
{
    s32 length = buffer->getLength();
    s32 num = str.getLength();
    s32 room = buffer->mBufferSize - length;
    if (room <= num) {
        num = room - 1;
    }
    if (num > 0) {
        std::memcpy(buffer->getMutableStringTop() + length, str.getCString(), num * sizeof(wchar_t));
        buffer->getMutableStringTop()[length + num] = sead::SafeStringBase<wchar_t>::s_NullChar;
    }
}

// the header in front of a text between best friends
struct TextHeader
{
    TextHeader() {}

    u8 version;       // 0x00
    u8 pad;
    u16 targetId;     // 0x02
    SvPlayerId player; // 0x04
    u16 townId;       // 0x1A
    AvatarIdList ids; // 0x1C
};
ASSERT_SIZE(TextHeader, 0x9C);
} // namespace

// 0x0062BB64 (name is ours)
netgame::NetGameMgr::BestFriendMgr::Message::Message() : principalId(0), townId(0)
{
    isNotification = false;
    isOwn = false;
    ClearPlayerId(&player);
    townName[0] = 0;
    text[0] = 0;
    time::SvTimeClear(&time);
}

// 0x0062BBB4 (name is ours)
void netgame::NetGameMgr::BestFriendMgr::ProcessConnect()
{
    ConnectMgr& connect = NetGameMgr::s_pInstance->m_ConnectMgr;
    if (m_IsDisconnecting) {
        if (m_RetryCount >= 2) {
            Stop();
        } else if (!connect.IsConnectedOrConnecting(ConnectMgr::CONNECTION_BEST_FRIEND)) {
            m_IsConnectRequested = true;
            m_ConnectTimer = 0;
            m_IsDisconnecting = false;
            m_RetryCount = 0;
        } else if (!connect.IsCurrentCmd(ConnectMgr::CONNECTION_BEST_FRIEND, false)) {
            m_RetryCount++;
        }
    }
    if (!m_IsConnectRequested) {
        m_ConnectTimer = 0;
        return;
    }
    if (!netgame::IsMultiPlayer() && !connect.IsConnectingOrDisconnecting(ConnectMgr::CONNECTION_LOCAL)) {
        if (m_ConnectTimer < CONNECT_WAIT_FRAMES) {
            m_ConnectTimer++;
            return;
        }
        if (!connect.Connect(ConnectMgr::CONNECTION_BEST_FRIEND)) {
            return;
        }
    }
    Stop();
}

// 0x0062BCD8 (name is ours)
void netgame::NetGameMgr::BestFriendMgr::AllocBuffers(sead::Heap* heap)
{
    if (heap == NULL) {
        return;
    }
    if (m_History.m_pBuffer == NULL) {
        Message* buffer = AllocMessages(heap);
        if (buffer != NULL) {
            m_History.Set(buffer, MESSAGE_NUM);
        }
    }
    if (m_Received.m_pBuffer == NULL) {
        Message* buffer = AllocMessages(heap);
        if (buffer != NULL) {
            m_Received.Set(buffer, MESSAGE_NUM);
        }
    }
    ClearAll();
    m_Text[0] = 0;
    m_TargetPrincipalId = 0;
    m_HasNewMessage = false;
    m_IsHistoryCut = false;
    m_SendState = SEND_STATE_IDLE;
    Stop();
    m_IsPaused = false;
}

// 0x0062BE08 (name is ours)
void netgame::NetGameMgr::BestFriendMgr::Process()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    ConnectMgr& connect = mgr->m_ConnectMgr;
    net::Net& net = net::Net::s_Instance;
    if (connect.IsConnectedOrConnecting(ConnectMgr::CONNECTION_BEST_FRIEND) &&
        !connect.IsCurrentCmd(ConnectMgr::CONNECTION_BEST_FRIEND, false) && net.IsError() && net.GetErrorLevel() >= 2) {
        if (!m_IsDisconnecting) {
            m_IsConnectRequested = false;
            m_ConnectTimer = 0;
            m_RetryCount = 0;
        }
        connect.Disconnect(ConnectMgr::CONNECTION_BEST_FRIEND);
    }
    if (m_SendState == SEND_STATE_SEND) {
        if (net.IsError()) {
            m_SendState = SEND_STATE_IDLE;
        } else {
            const wchar_t* text = reinterpret_cast<const wchar_t*>(m_Text);
            bool isSent = m_TargetPrincipalId != 0 ? net.SendTextMessage(&m_TargetPrincipalId, text) : net.SendNotification102(text);
            if (isSent) {
                m_SendState = SEND_STATE_IDLE;
            }
        }
    }
    if (m_SendState != SEND_STATE_NONE) {
        m_CriticalSection.Enter();
        while (m_Received.m_Count != 0 && m_Received.Peek()->principalId == 0) {
            Message message;
            m_Received.Pop(&message);
            m_History.PushFront(&message);
            m_HasNewMessage = true;
        }
        m_CriticalSection.Exit();
    }
    if (!m_IsPaused) {
        ProcessConnect();
    }
}

// 0x0062C0E8 (name is ours)
void netgame::NetGameMgr::BestFriendMgr::OnMessage(const u32* principalId, const wchar_t* text, u32 /* length */, u32 isNotification)
{
    if (*principalId == 0 || m_Received.m_pBuffer == NULL) {
        return;
    }
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (!mgr->m_ConnectMgr.IsConnectedOrConnecting(ConnectMgr::CONNECTION_BEST_FRIEND)) {
        return;
    }
    Message message;
    message.principalId = *principalId;
    message.isNotification = isNotification;
    message.isOwn = false;
    TextHeader header;
    char work[215];
    u32 decoded = net::DecodeBase64(reinterpret_cast<const char16*>(text), HEADER_TEXT_LENGTH, reinterpret_cast<u8*>(&header), sizeof(header),
                                    work, sizeof(work));
    if (decoded != sizeof(header) || header.version != HEADER_VERSION) {
        return;
    }
    FriendListMgr& friends = mgr->m_FriendListMgr;
    if (!isNotification && header.targetId != INVALID_PLAYER_ID && header.targetId != friends.GetMyAvatarId().playerId) {
        return;
    }
    if (header.ids.Find(friends.GetMyAvatarId()) < 0) {
        return;
    }
    AvatarId avatarId;
    avatarId.Set(header.player.pid, header.townId);
    if (friends.FindBestFriend(*principalId, avatarId) < 0) {
        return;
    }
    AssignPlayerId(&message.player, &header.player);
    message.townId = header.townId;

    sead::SafeStringBase<wchar_t> all(text);
    const wchar_t* body = all.getLength() >= static_cast<s32>(HEADER_TEXT_LENGTH) ? text + HEADER_TEXT_LENGTH
                                                                                  : &sead::SafeStringBase<wchar_t>::s_NullChar;
    sead::WFixedSafeString<44> content;
    content.copy(sead::SafeStringBase<wchar_t>(body));
    sead::SafeStringBase<wchar_t> separator(s_Separator);
    s32 index = content.findIndex(separator);
    if (index < 0) {
        return;
    }
    const wchar_t* townName = content.getCString() + index + 1;
    sead::SafeStringBase<wchar_t> town(townName);
    CopyWord(message.townName, 9, reinterpret_cast<const char16*>(townName), town.getLength() + 1);
    content.getMutableStringTop()[index] = sead::SafeStringBase<wchar_t>::s_NullChar;
    CopyWord(message.text, 33, reinterpret_cast<const char16*>(content.getCString()), content.getLength() + 1);
    Enqueue(&message);
}

// 0x0062C5D8 (name is ours)
void netgame::NetGameMgr::OnConnectBestFriend()
{
    m_BestFriendMgr.ClearReceived();
}

// 0x0062C5E4 (name is ours)
void netgame::NetGameMgr::BestFriendMgr::ClearReceived()
{
    m_CriticalSection.Enter();
    m_Received.Clear();
    m_CriticalSection.Exit();
}

// 0x0062C610 (name is ours)
bool netgame::NetGameMgr::PopBestFriendMessage(BestFriendMgr::Message* message)
{
    return m_BestFriendMgr.PopMessage(message);
}

// 0x0062C61C (name is ours)
bool netgame::NetGameMgr::BestFriendMgr::PopMessage(Message* message)
{
    m_CriticalSection.Enter();
    while (m_Received.m_Count != 0) {
        m_Received.Pop(message);
        if (message->principalId != 0 && !IsFromBestFriend(message)) {
            continue;
        }
        m_History.PushFront(message);
        if (message->principalId != 0) {
            m_CriticalSection.Exit();
            return true;
        }
    }
    m_CriticalSection.Exit();
    return false;
}

// 0x0062C740 (name is ours)
void netgame::NetGameMgr::BestFriendMgr::Reset(bool isForced)
{
    if (isForced) {
        m_IsPaused = false;
    }
    if (m_IsDisconnecting &&
        (netgame::IsMultiPlayer() || NetGameMgr::s_pInstance->m_ConnectMgr.IsConnectingOrDisconnecting(ConnectMgr::CONNECTION_LOCAL))) {
        m_IsConnectRequested = false;
        m_ConnectTimer = 0;
        m_IsDisconnecting = false;
    } else {
        if (!m_IsDisconnecting) {
            m_IsConnectRequested = false;
        }
        m_ConnectTimer = 0;
    }
    m_RetryCount = 0;
}

// 0x0062C7B8 (name is ours)
void netgame::NetGameMgr::BestFriendMgr::Stop()
{
    m_IsConnectRequested = false;
    m_ConnectTimer = 0;
    m_IsDisconnecting = false;
    m_RetryCount = 0;
}

DECOMP_ALWAYS_INLINE bool netgame::NetGameMgr::BestFriendMgr::IsFromBestFriend(const Message* message)
{
    AvatarId avatarId;
    avatarId.Set(message->player.pid, message->townId);
    return NetGameMgr::s_pInstance->m_FriendListMgr.FindBestFriend(message->principalId, avatarId) >= 0;
}

// 0x0062C7D0 (name is ours)
bool netgame::NetGameMgr::BestFriendMgr::PopToHistory(Message* message)
{
    m_Received.Pop(message);
    if (message->principalId != 0 && !IsFromBestFriend(message)) {
        return false;
    }
    m_History.PushFront(message);
    return true;
}

// 0x0062C8B8 (name is ours)
void netgame::NetGameMgr::BestFriendMgr::AddToHistory(const Message* message)
{
    m_History.PushFront(message);
    m_HasNewMessage = true;
}

// 0x0062C964 (name after FriendListMgr::OnNotification)
void netgame::NetGameMgr::BestFriendMgr::OnBestFriendOnline(const u32* principalId, const SvPersonalId* personalId)
{
    Message message;
    message.principalId = *principalId;
    script::WordFix<64> word;
    if (script::StrMgr::s_pInstance != NULL) {
        script::StrMgr::s_pInstance->GetString(&word, "SYS_2D_UI", 0x16E);
    }
    script::WordPtrSv(message.text, 33).Copy(word);
    CopyWord(message.townName, 9, personalId->townData.dataTownName, 9);
    AssignPlayerId(&message.player, &personalId->playerData);
    message.townId = personalId->townData.tid;
    message.isNotification = true;
    message.isOwn = false;
    Enqueue(&message);
}

// 0x0062CAF0 (name is ours)
void netgame::NetGameMgr::BestFriendMgr::Enqueue(Message* message)
{
    m_CriticalSection.Enter();
    if (m_Received.m_Count >= m_Received.m_Capacity) {
        Message oldest;
        if (PopToHistory(&oldest)) {
            m_IsHistoryCut = true;
        }
    }
    message->time = SvTime(GetCurrentTime());
    m_Received.PushBack(message);
    m_CriticalSection.Exit();
}

// 0x0062CBA4 (name is ours)
void netgame::NetGameMgr::BestFriendMgr::RequestDisconnect(bool disconnects)
{
    if (netgame::IsMultiPlayer()) {
        return;
    }
    ConnectMgr& connect = NetGameMgr::s_pInstance->m_ConnectMgr;
    if (!connect.IsConnectedOrConnecting(ConnectMgr::CONNECTION_BEST_FRIEND) && !m_IsConnectRequested) {
        return;
    }
    m_IsDisconnecting = true;
    m_RetryCount = 0;
    m_IsConnectRequested = false;
    m_ConnectTimer = 0;
    if (disconnects) {
        connect.Disconnect(ConnectMgr::CONNECTION_BEST_FRIEND);
        m_IsPaused = true;
    }
}

// 0x0062CC24 (name is ours)
bool ConnectBestFriendIfAllowed()
{
    SvPlayer* player = SvGetCurrentPlayer();
    if (player == NULL) {
        return false;
    }
    if ((reinterpret_cast<const u8*>(player)[0x571F] & BIT_ON_NEW_FRIEND) == 0) {
        return false;
    }
    GetSharedData();
    SvBestFriendList* list = SvGetBestFriendList();
    if (list == NULL || !list->HasFriend()) {
        return false;
    }
    if (Func350D40() && !Func23CBF0() && (!Func23CBC0() || !Func710100() || !Func23CB00())) {
        return false;
    }
    if (!Func23C4CC() || !Func23CC18() || !Func350BA8()) {
        return false;
    }
    return StartConnectBestFriend();
}

// 0x0062CCE4 (name is ours)
bool netgame::NetGameMgr::SendBestFriendText(const sead::SafeStringBase<wchar_t>& text, const Principal* principal)
{
    return m_BestFriendMgr.SendText(text, principal);
}

// 0x0062CCF0 (name is ours)
bool netgame::NetGameMgr::BestFriendMgr::SendText(const sead::SafeStringBase<wchar_t>& text, const Principal* principal)
{
    u32 principalId = principal->principalId;
    if (principalId == 0 || !principal->avatarId.IsValid()) {
        return false;
    }
    u16 playerId = principal->avatarId.playerId;
    return SendText(text, &principalId, &playerId);
}

// 0x0062CD4C (name is ours)
void netgame::NetGameMgr::ClearBestFriendMessages()
{
    m_BestFriendMgr.ClearAll();
}

// 0x0062CD58 (name is ours)
void netgame::NetGameMgr::BestFriendMgr::ClearAll()
{
    m_CriticalSection.Enter();
    m_Received.Clear();
    m_History.Clear();
    m_HasNewMessage = false;
    m_IsHistoryCut = false;
    m_CriticalSection.Exit();
}

// 0x0062CD94 (name is ours)
bool netgame::NetGameMgr::BestFriendMgr::SendText(const sead::SafeStringBase<wchar_t>& text, const u32* principalId, const u16* playerId)
{
    if (m_SendState != SEND_STATE_IDLE || !netgame::IsConnectedBestFriend()) {
        return false;
    }
    SvPlayer* player = g_pSvCurrentPlayer;
    if (player == NULL) {
        return false;
    }
    TextHeader header;
    header.version = HEADER_VERSION;
    header.targetId = *playerId;
    AssignPlayerId(&header.player, &player->playerInfo.playerData);
    header.townId = player->playerInfo.townData.tid;
    header.ids.Assign(NetGameMgr::s_pInstance->m_FriendListMgr.GetBestFriendIds());
    char work[214];
    u32 length = net::EncodeBase64(reinterpret_cast<const u8*>(&header), sizeof(header), m_HeaderText, sizeof(work), work, sizeof(work));
    if (length != HEADER_TEXT_LENGTH) {
        m_HeaderText[0] = 0;
    }
    sead::BufferedSafeStringBase<wchar_t> buffer(reinterpret_cast<wchar_t*>(m_Text), 256);
    buffer.format(reinterpret_cast<const wchar_t*>(m_HeaderText));
    AppendString(&buffer, text);
    AppendString(&buffer, sead::SafeStringBase<wchar_t>(s_Separator));
    AppendString(&buffer, sead::SafeStringBase<wchar_t>(reinterpret_cast<const wchar_t*>(player->playerInfo.townData.dataTownName)));

    Message message;
    message.principalId = s_NoPrincipal;
    CopyWord(message.text, 33, reinterpret_cast<const char16*>(text.getCString()), text.getLength() + 1);
    CopyWord(message.townName, 9, player->playerInfo.townData.dataTownName, 9);
    AssignPlayerId(&message.player, &player->playerInfo.playerData);
    message.townId = player->playerInfo.townData.tid;
    message.isNotification = *principalId == 0;
    message.isOwn = true;
    Enqueue(&message);
    m_TargetPrincipalId = *principalId;
    m_SendState = SEND_STATE_SEND;
    return true;
}

// 0x0062D268 (name is ours)
u32 netgame::NetGameMgr::BestFriendMgr::GetHistory(Message* messages, u32 maxNum, u32 start, s32* totalNum, bool clearsNew)
{
    m_CriticalSection.Enter();
    if (m_History.m_Count <= 0) {
        m_CriticalSection.Exit();
        return 0;
    }
    if (totalNum != NULL) {
        *totalNum = m_History.m_Count;
    }
    u32 total = m_History.m_Count;
    u32 num = 0;
    for (u32 i = start; i < total && num < maxNum; i++) {
        CopyMessage(&messages[num], m_History.At(i));
        num++;
    }
    if (clearsNew) {
        m_HasNewMessage = false;
    }
    m_CriticalSection.Exit();
    return num;
}

// 0x0062D340 (name is ours)
netgame::NetGameMgr::BestFriendMgr::BestFriendMgr()
{
    m_ConnectTimer = 0;
    m_RetryCount = 0;
    m_CriticalSection.Initialize();
    m_TargetPrincipalId = 0;
    m_SendState = SEND_STATE_NONE;
    m_HasNewMessage = false;
    m_IsHistoryCut = false;
    m_IsConnectRequested = false;
    m_IsDisconnecting = false;
    m_IsPaused = false;
}

// 0x0062D3A4 (name is ours)
netgame::NetGameMgr::BestFriendMgr::~BestFriendMgr()
{
    m_HasNewMessage = false;
    m_IsHistoryCut = false;
    Stop();
    m_IsPaused = false;
    if (m_Received.m_pBuffer != NULL) {
        delete[] m_Received.m_pBuffer;
        m_Received.Reset();
    }
    if (m_History.m_pBuffer != NULL) {
        delete[] m_History.m_pBuffer;
        m_History.Reset();
    }
}
} // namespace netgame
