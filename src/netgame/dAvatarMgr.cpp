#include "netgame/dAvatarMgr.h"

#include "net/dNet.h"
#include "netgame/dNetGameMgr.h"
#include <cstring>

namespace netgame {
namespace {
DECOMP_ALWAYS_INLINE nn::pia::StationId InvalidStation()
{
    return nn::pia::StationId(0xFFFFFFFD, 0xFFFFFFFF);
}
} // namespace

// 0x0061B288 (name is ours)
void netgame::AvatarMgr::AvatarInfoBase::Reset()
{
    Principal::Reset();
    station = InvalidStation();
}

// 0x0061B2A4 (name is ours)
void netgame::AvatarMgr::AvatarInfoBase::Set(const nn::pia::StationId& newStation, const Principal& principal)
{
    principalId = principal.principalId;
    avatarId = principal.avatarId;
    station = newStation;
}

// 0x0061B2CC (name is ours)
netgame::AvatarMgr::AvatarInfoBase::AvatarInfoBase() : station(InvalidStation())
{
}

// 0x00624AF8 (name is ours)
netgame::AvatarMgr::AvatarInfoBase::~AvatarInfoBase()
{
    // empty (in the original too)
}

// 0x00627794 (name is ours)
void netgame::AvatarMgr::AvatarInfo::Set(const nn::pia::StationId& newStation)
{
    AvatarInfoBase::Reset();
    station = newStation;
    senderHandle = NULL;
}

// 0x006277B8 (name is ours)
void netgame::AvatarMgr::AvatarInfo::Assign(const AvatarInfoBase& info)
{
    principalId = info.principalId;
    avatarId = info.avatarId;
    station = info.station;
    senderHandle = NULL;
}

// 0x006277EC (name is ours)
void netgame::AvatarMgr::AvatarInfo::Clear()
{
    AvatarInfoBase::Reset();
    if (senderHandle != NULL) {
        net::Net::s_Instance.m_Sender.Cancel(senderHandle);
    }
    senderHandle = NULL;
}

// 0x00627818 (name is ours)
bool netgame::AvatarMgr::AvatarInfo::Send(const void* data, u32 size)
{
    if (!IsValid()) {
        return false;
    }
    net::Sender& sender = net::Net::s_Instance.m_Sender;
    if (senderHandle != NULL) {
        if (senderHandle->IsSent()) {
            sender.Release(senderHandle);
            senderHandle = NULL;
        } else if (senderHandle != NULL) {
            return false;
        }
    }
    senderHandle = sender.Send(&station, data, size);
    return senderHandle != NULL;
}

// 0x006278B0 (name is ours)
netgame::AvatarMgr::AvatarInfo::AvatarInfo()
{
    senderHandle = NULL;
}

// 0x006278C4 (name is ours)
netgame::AvatarMgr::AvatarInfo::~AvatarInfo()
{
    // empty (in the original too)
}

// 0x0062A3A4 (name is ours)
void netgame::AvatarMgr::Start()
{
    Reset();
    net::Net& net = net::Net::s_Instance;
    m_Infos[0].Set(net.IsHost() ? net.GetLocalStationId() : net.GetHostStationId());
}

// 0x0062A3E8 (name is ours)
void netgame::AvatarMgr::OnStationLeft(const nn::pia::StationId& stationId, u32 /* frame */)
{
    if (stationId == InvalidStation() || stationId == net::Net::s_Instance.GetLocalStationId()) {
        return;
    }
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    sead::CriticalSection& section = mgr->m_CriticalSection;
    section.lock();
    PlayerNo player = GetPlayerNo(stationId);
    if (player >= PLAYER_NO_MAX) {
        RemoveJoinInfo(stationId);
    } else {
        mgr->m_Bits[3].Remove(player);
        mgr->m_Bits[5].Remove(player);
        mgr->m_Bits[4].Remove(player);
        mgr->m_Bits[6].Remove(player);
        mgr->m_Entry.Remove(player);
        mgr->m_ExistingPlayers.Remove(player);
        RemovePlayer(player);
    }
    m_HasJoiner = false;
    section.unlock();
}

// 0x0062A530 (name after libgarden)
void netgame::AvatarMgr::Process()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    net::Net& net = net::Net::s_Instance;
    if (!mgr->m_PacketQueue.m_IsClosed && mgr->m_IsInNetSession) {
        MachineBitTable& table = mgr->m_Bits[3];
        sead::CriticalSection& section = mgr->m_CriticalSection;
        section.lock();
        const nn::pia::StationId& local = net.GetLocalStationId();
        const nn::pia::StationId& host = net.GetHostStationId();
        if (local != InvalidStation() && local == host) {
            bool isAdded = false;
            AvatarInfoBase info;
            u32 frame = net.GetFrameCount();
            s32 i = 0;
            while (i < 4) {
                if (!m_JoinInfos[i].IsValid() || static_cast<u32>(m_UpdateTick[i]) > frame ||
                    !net.IsParticipant(m_JoinInfos[i].station)) {
                    i++;
                    continue;
                }
                info = m_JoinInfos[i];
                PlayerNo freeNo = GetPlayerNo(InvalidStation());
                if (freeNo >= PLAYER_NO_MAX) {
                    break;
                }
                AvatarInfo* avatarInfo = GetInfo(freeNo);
                if (avatarInfo == NULL) {
                    break;
                }
                avatarInfo->Assign(info);
                PlayerNo no = GetPlayerNo(info.station);
                if (no < PLAYER_NO_MAX && !table.IsSet(no)) {
                    table.Add(no);
                    u32 principalId = info.principalId;
                    mgr->m_IslandMgr.AddPrincipal(no, &principalId, true);
                    isAdded = true;
                }
                RemoveJoinInfo(info.station);
                frame = net.GetFrameCount();
            }
            if (isAdded) {
                for (s32 j = 0; j < PLAYER_NO_MAX; j++) {
                    PlayerNo no = static_cast<PlayerNo>(j);
                    if (table.IsSet(no) && no != 0) {
                        mgr->m_Entry.Add(no);
                    }
                }
            }
        } else {
            AvatarInfoBase info;
            for (s32 i = 0;; i++) {
                u32 frame = net.GetFrameCount();
                if (i >= 4) {
                    break;
                }
                if (!m_JoinInfos[i].IsValid() || static_cast<u32>(m_UpdateTick[i]) > frame ||
                    !net.IsParticipant(m_JoinInfos[i].station)) {
                    continue;
                }
                info = m_JoinInfos[i];
                if (info.station == host) {
                    AvatarInfo* avatarInfo = GetInfo(PLAYER_NO_HOST);
                    if (avatarInfo != NULL && !avatarInfo->IsValid()) {
                        avatarInfo->Assign(info);
                    }
                }
                PlayerNo no = GetPlayerNo(info.station);
                if (no >= PLAYER_NO_MAX) {
                    continue;
                }
                if (!table.IsSet(no)) {
                    AvatarInfo* avatarInfo = GetInfo(no);
                    if (avatarInfo != NULL && avatarInfo->IsValid()) {
                        avatarInfo->principalId = info.principalId;
                        avatarInfo->avatarId = info.avatarId;
                    }
                    table.Add(no);
                    u32 principalId = info.principalId;
                    mgr->m_IslandMgr.AddPrincipal(no, &principalId, false);
                    mgr->m_Bits[4].Add(no);
                }
                RemoveJoinInfo(info.station);
            }
        }
        section.unlock();
    }
    if (mgr->GetMyPlayerNo() == PLAYER_NO_HOST && m_JoiningStation != InvalidStation() &&
        GetNetgameType() == TYPE_RANDOM_MATCH && !net.IsParticipant(m_JoiningStation)) {
        mgr->m_GateMgr.Open(GateMgr::FLAG_RANDOM_MATCH_PLAYER_JOINING);
        m_JoiningStation = InvalidStation();
    }
    sead::CriticalSection& section = mgr->m_CriticalSection;
    section.lock();
    nn::pia::StationId station = m_NewStation;
    if (station == InvalidStation()) {
        section.unlock();
        return;
    }
    m_NewStation = InvalidStation();
    section.unlock();
    if (net.IsHost() && GetNetgameType() == TYPE_RANDOM_MATCH && !mgr->m_GateMgr.IsClosed(GateMgr::FLAG_RANDOM_MATCH_PLAYER_JOINING) &&
        mgr->m_GateMgr.Close(GateMgr::FLAG_RANDOM_MATCH_PLAYER_JOINING)) {
        m_JoiningStation = station;
    }
}

// 0x0062AB24 (name is ours)
void netgame::AvatarMgr::RemovePlayer(PlayerNo player)
{
    if (player >= PLAYER_NO_MAX) {
        return;
    }
    sead::CriticalSection& section = NetGameMgr::s_pInstance->m_CriticalSection;
    section.lock();
    AvatarInfo* info = &m_Infos[player];
    if (info->IsValid()) {
        nn::pia::StationId station = info->station;
        FriendCode* code = const_cast<FriendCode*>(FindFriendCode(station));
        if (code != NULL) {
            code->Clear();
        }
    }
    info->Clear();
    section.unlock();
}

// 0x0062ABDC (name is ours)
bool netgame::AvatarMgr::OnJoinApproval(const char16* /* name */, u32 /* length */)
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    return !m_HasJoiner && !mgr->m_IsSessionClosedForIsland && mgr->m_GateMgr.IsOpen() && mgr->unk_0x132B5 == 0 &&
           !mgr->unk_0x132B6;
}

// 0x0062AC3C (name is ours)
void netgame::AvatarMgr::OnMessage(const u32* principalId, const wchar_t* text, u32 length, u32 isNotification)
{
    NetGameMgr::s_pInstance->m_BestFriendMgr.OnMessage(principalId, text, length, isNotification);
}

// 0x0062AC64 (name is ours)
bool netgame::AvatarMgr::SetStations(u32* newNum, const void* stations)
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    const nn::pia::StationId* source = static_cast<const nn::pia::StationId*>(stations);
    sead::CriticalSection& section = mgr->m_CriticalSection;
    section.lock();
    nn::pia::StationId received[4];
    u32 localNum = 0;
    MachineBitTable changed;
    if (newNum == NULL) {
        newNum = &localNum;
    }
    u32 freeNum = 0;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        nn::pia::StationId station(0, 0);
        std::memcpy(&station, &source[i], sizeof(nn::pia::StationId));
        received[i] = station;
        AvatarInfo& info = m_Infos[i];
        PlayerNo no = static_cast<PlayerNo>(i);
        if (!info.IsValid()) {
            freeNum++;
            if (station != InvalidStation()) {
                (*newNum)++;
                changed.Add(no);
            }
            continue;
        }
        if (station == info.station) {
            continue;
        }
        if (station != InvalidStation()) {
            // the old station is still on its way in
            if (FindJoinInfo(info.station) != JOIN_INFO_NOT_FOUND) {
                section.unlock();
                return false;
            }
            (*newNum)++;
            changed.Add(no);
        }
        mgr->m_Bits[3].Remove(no);
        mgr->m_Bits[5].Remove(no);
        mgr->m_Bits[4].Remove(no);
        mgr->m_Bits[6].Remove(no);
        mgr->m_Entry.Remove(no);
        mgr->m_ExistingPlayers.Remove(no);
        RemovePlayer(no);
        freeNum++;
    }
    if (*newNum != 0) {
        if (*newNum > freeNum) {
            section.unlock();
            return false;
        }
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            if (changed.IsSet(static_cast<PlayerNo>(i))) {
                m_Infos[i].Set(received[i]);
            }
        }
    }
    section.unlock();
    return true;
}

// 0x0062AFD8 (name is ours)
netgame::AvatarMgr::FriendCode::FriendCode()
{
    Clear();
}

// 0x0062AFF4 (name is ours)
void netgame::AvatarMgr::OnNotification(const u32* principalId, u32 param1, u32 param2, const wchar_t* text, u32 length,
                                        bool isFromHandler)
{
    NetGameMgr::s_pInstance->m_FriendListMgr.OnNotification(principalId, param1, param2, text, length, isFromHandler);
}

// 0x0062B028 (name is ours)
const netgame::AvatarMgr::FriendCode* netgame::AvatarMgr::FindFriendCode(const nn::pia::StationId& id) const
{
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        if (m_FriendCodes[i].station == id) {
            return &m_FriendCodes[i];
        }
    }
    return NULL;
}

// 0x0062B0A4 (name is ours)
void netgame::AvatarMgr::RemoveJoinInfo(const nn::pia::StationId& id)
{
    sead::CriticalSection& section = NetGameMgr::s_pInstance->m_CriticalSection;
    section.lock();
    if (id != InvalidStation()) {
        u32 index = FindJoinInfo(id);
        if (index < PLAYER_NO_MAX) {
            for (s32 i = index; i < 3; i++) {
                m_JoinInfos[i] = m_JoinInfos[i + 1];
                m_UpdateTick[i] = m_UpdateTick[i + 1];
            }
            m_JoinInfos[3].Reset();
            m_UpdateTick[3] = 0;
        }
    }
    section.unlock();
}

// 0x0062B170 (name is ours)
bool netgame::AvatarMgr::FetchFriendCode(PlayerNo player)
{
    const FriendCode* code = NULL;
    if (player < PLAYER_NO_MAX) {
        AvatarInfo* info = &m_Infos[player];
        if (!info->IsValid()) {
            return true;
        }
        nn::pia::StationId station = info->station;
        code = FindFriendCode(station);
    }
    if (code == NULL) {
        return true;
    }
    return net::Net::s_Instance.GetFriendCode(&code->station, const_cast<u64*>(reinterpret_cast<const u64*>(code->friendCode)));
}

// 0x0062B1FC (name is ours)
void netgame::AvatarMgr::Reset()
{
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        m_Infos[i].Clear();
    }
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        m_JoinInfos[i].Reset();
        m_UpdateTick[i] = 0;
    }
    m_FriendCodes[0].Clear();
    m_FriendCodes[1].Clear();
    m_FriendCodes[2].Clear();
    m_FriendCodes[3].Clear();
    m_HasJoiner = false;
    m_NewStation = InvalidStation();
    m_JoiningStation = InvalidStation();
}

// 0x0062B2BC (name is ours)
void netgame::AvatarMgr::OnStationJoined(const nn::pia::StationId& stationId, const char16* name, u32 length, u32 frame)
{
    if (stationId == InvalidStation()) {
        return;
    }
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    sead::CriticalSection& section = mgr->m_CriticalSection;
    section.lock();
    if (stationId != InvalidStation() && FindJoinInfo(stationId) >= PLAYER_NO_MAX) {
        u32 index = FindJoinInfo(InvalidStation());
        if (index < PLAYER_NO_MAX) {
            Principal principal;
            DecodePrincipal(&principal, name, length);
            m_JoinInfos[index].Set(stationId, principal);
            m_UpdateTick[index] = frame;
        }
    }
    if (mgr->GetMyPlayerNo() == PLAYER_NO_HOST) {
        m_HasJoiner = true;
        if (!mgr->m_IslandMgr.m_IsRejoining) {
            m_NewStation = stationId;
        }
    }
    mgr->m_IslandMgr.OnStationJoined();
    if (stationId != InvalidStation()) {
        FriendCode* code = const_cast<FriendCode*>(FindFriendCode(InvalidStation()));
        if (code == NULL) {
            // the codes of stations that have left go
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                FriendCode& entry = m_FriendCodes[i];
                if (entry.station == InvalidStation()) {
                    continue;
                }
                PlayerNo player = GetPlayerNo(entry.station);
                if (player >= PLAYER_NO_MAX) {
                    entry.Clear();
                }
            }
            code = const_cast<FriendCode*>(FindFriendCode(InvalidStation()));
        }
        if (code != NULL) {
            net::Net::s_Instance.GetFriendCodeDirect(&stationId, reinterpret_cast<u64*>(code->friendCode));
            code->station = stationId;
        }
    }
    section.unlock();
}

// 0x0062B4F4 (name is ours)
void netgame::AvatarMgr::OnReceive(const nn::pia::StationId& stationId, const void* data, u32 size)
{
    if (stationId == InvalidStation() || data == NULL || size < sizeof(NetGameMgr::PacketHead)) {
        return;
    }
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    sead::CriticalSection& section = mgr->m_CriticalSection;
    section.lock();
    if (mgr->m_ErrorFlags == 0 && GetPlayerNo(stationId) < PLAYER_NO_MAX) {
        PlayerNo player = GetPlayerNo(stationId);
        if (player < PLAYER_NO_MAX) {
            NetGameMgr::PacketHead head = *static_cast<const NetGameMgr::PacketHead*>(data);
            u32 bodySize = size - sizeof(NetGameMgr::PacketHead);
            const void* body = static_cast<const u8*>(data) + sizeof(NetGameMgr::PacketHead);
            s32 handled = -head.handled;
            if (handled < 0) {
                mgr->m_SendMgr.AddPending(player, handled);
            }
            if (head.isSystem) {
                u32 kind = head.kind;
                s8 counterKind = PacketQueue::GetSystemPacketKind(kind);
                if (counterKind > 0) {
                    mgr->CheckReceiveCounter(player, head.counter, true);
                } else if (counterKind == 0) {
                    mgr->CheckReceiveCounter(player, head.counter, false);
                }
                PacketQueue::DispatchSystemPacket(player, body, bodySize, kind);
            } else {
                mgr->CheckReceiveCounter(player, head.counter, true);
                if (size <= NetGameMgr::Receiver::RECEIVE_BUFFER_SIZE) {
                    void* buffer = mgr->m_Receiver.GetBuffer(player);
                    if (buffer != NULL) {
                        std::memcpy(buffer, data, size);
                        mgr->m_Receiver.SetIsEnabled(player, true);
                    }
                }
            }
        }
    }
    section.unlock();
}

// 0x0062B6B0 (name is ours)
bool netgame::AvatarMgr::Send(PlayerNo player, const void* data, u32 size)
{
    sead::CriticalSection& section = NetGameMgr::s_pInstance->m_CriticalSection;
    section.lock();
    AvatarInfo* info = GetInfo(player);
    if (info != NULL) {
        bool result = info->Send(data, size);
        section.unlock();
        return result;
    }
    section.unlock();
    return false;
}

// 0x0062B728 (name is ours)
netgame::AvatarMgr::AvatarMgr()
{
    m_NewStation = InvalidStation();
    m_JoiningStation = InvalidStation();
    m_HasJoiner = false;
}

// 0x0062B7F8 slot 0x00 (name is ours)
// 0x0062B7A8 (deleting dtor)
netgame::AvatarMgr::~AvatarMgr()
{
    // (only the members are destroyed)
}

// 0x0075FF84 (name is ours)
bool netgame::AvatarMgr::AvatarInfo::CanSend() const
{
    if (!IsValid()) {
        return false;
    }
    if (senderHandle == NULL) {
        return true;
    }
    return senderHandle->IsSent();
}

// 0x0075FFC0 (name is ours)
u32 netgame::AvatarMgr::AvatarInfo::GetLatency() const
{
    if (!IsValid()) {
        return 0;
    }
    return net::Net::s_Instance.GetLatency(station);
}

// 0x007603E8 (name after libgarden)
netgame::PlayerNo netgame::AvatarMgr::GetPlayerNo(const nn::pia::StationId& id) const
{
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        if (m_Infos[i].station == id) {
            return static_cast<PlayerNo>(i);
        }
    }
    return PLAYER_NO_INVALID;
}

// 0x00760214 (name is ours)
void netgame::AvatarMgr::GetFriendId(FriendId* id, PlayerNo player) const
{
    const AvatarInfo* info = GetInfo(player);
    if (info != NULL && info->IsValid()) {
        id->principalId = info->principalId;
    } else {
        id->principalId = 0;
    }
    const FriendCode* code = NULL;
    if (info != NULL && info->IsValid()) {
        code = FindFriendCode(info->station);
    }
    if (code != NULL && code->station != InvalidStation()) {
        id->friendCode[0] = code->friendCode[0];
        id->friendCode[1] = code->friendCode[1];
    } else {
        id->friendCode[0] = 0;
        id->friendCode[1] = 0;
    }
}

// 0x00760368 (name is ours)
bool netgame::AvatarMgr::HasSameStations(const void* stations) const
{
    const nn::pia::StationId* source = static_cast<const nn::pia::StationId*>(stations);
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        nn::pia::StationId station(0, 0);
        std::memcpy(&station, &source[i], sizeof(nn::pia::StationId));
        if (station != m_Infos[i].station) {
            return false;
        }
    }
    return true;
}

// 0x00760464 (name is ours)
bool netgame::AvatarMgr::CanSend(PlayerNo player) const
{
    sead::CriticalSection& section = NetGameMgr::s_pInstance->m_CriticalSection;
    section.lock();
    const AvatarInfo* info = GetInfo(player);
    if (info != NULL) {
        bool result = info->CanSend();
        section.unlock();
        return result;
    }
    section.unlock();
    return false;
}

// 0x007604CC (name is ours)
u32 netgame::AvatarMgr::GetLatency(PlayerNo player) const
{
    const AvatarInfo* info = GetInfo(player);
    if (info != NULL) {
        return info->GetLatency();
    }
    return 0;
}

// 0x007604F0 (name after libgarden)
const netgame::AvatarMgr::AvatarInfo* netgame::AvatarMgr::GetAvatarInfo(PlayerNo player) const
{
    const AvatarInfo* info = GetInfo(player);
    if (info != NULL && info->IsValid()) {
        return info;
    }
    return NULL;
}

// 0x00760530 (name is ours)
bool netgame::AvatarMgr::GetStations(nn::pia::StationId* stations) const
{
    if (stations == NULL) {
        return false;
    }
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        nn::pia::StationId station = m_Infos[i].station;
        std::memcpy(&stations[i], &station, sizeof(nn::pia::StationId));
    }
    return true;
}

// 0x00760594 (name is ours)
netgame::PlayerNo netgame::AvatarMgr::FindMyPlayerNo() const
{
    const nn::pia::StationId& local = net::Net::s_Instance.GetLocalStationId();
    if (local == InvalidStation()) {
        return PLAYER_NO_INVALID;
    }
    return GetPlayerNo(local);
}

// 0x007605DC (name is ours)
u32 netgame::AvatarMgr::FindJoinInfo(const nn::pia::StationId& id) const
{
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        if (m_JoinInfos[i].station == id) {
            return i;
        }
    }
    return JOIN_INFO_NOT_FOUND;
}

// 0x00760660 (name after libgarden: netgame::NetGameMgr::IsInSession)
bool netgame::AvatarMgr::IsInSession(PlayerNo player, bool skipSelf) const
{
    sead::CriticalSection& section = NetGameMgr::s_pInstance->m_CriticalSection;
    section.lock();
    const AvatarInfo* info = GetInfo(player);
    if (info != NULL && info->IsValid()) {
        nn::pia::StationId station = info->station;
        net::Net& net = net::Net::s_Instance;
        if (!skipSelf || station != net.GetLocalStationId()) {
            bool result = net.IsParticipant(station);
            section.unlock();
            return result;
        }
    }
    section.unlock();
    return false;
}
} // namespace netgame
