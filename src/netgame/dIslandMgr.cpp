#include "applet/dErrEulaMgr.h"
#include "netgame/dIslandMgr.h"

#include "Bs/dBsSvMgr.h"
#include "Bs/dBsTelop.h"
#include "Bs/dBsTourMgr.h"
#include "Other/dFriendList.h"
#include "Other/dTourResult.h"
#include "Sv/dSvFgName.h"
#include "Sound/dSoundMgr.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "demo/demo_Api.h"
#include "net/dNet.h"
#include "net/nex/dImpl.h"
#include "netgame/dGameGlobals.h"
#include "netgame/dGameFunctions.h"
#include "netgame/dNetGameMgr.h"
#include "stage/dTransitionMgr.h"
#include "stage/stage_Extern.h"

namespace netgame {

// functions of the game that are not decompiled yet (named after their address)


// 0x0087CDA4 (name is ours)
extern const u32 s_NoPrincipal = 0;
// 0x00A22FF0 (name is ours)
nn::math::VEC3_ s_TourDeskPositions[4];

namespace {
const u32 TOUR_NUM = 64;
const u32 RETRY_FRAMES = 600;
const u32 ISLAND_MAP_SIZE = 0x102A;

enum Packet : u8
{
    PACKET_ISLAND_DATA = 25,
    PACKET_RETIRE = 38,
    PACKET_RETIRE_READY = 39,
    PACKET_NEW_HOST = 40,
    PACKET_REJOINED = 41,
    PACKET_TOUR_SPLIT = 42,
    PACKET_TOUR_GUEST_READY = 43,
    PACKET_TOUR_LEAVING = 44,
    PACKET_TOUR_RESULT = 45,
    PACKET_TOUR_PLAYER_LEFT = 46,
    PACKET_TOUR_ALL_STAY = 47,
    PACKET_TOUR_START = 49,
    PACKET_TOUR_DATA = 96,
    PACKET_TOUR_ARRIVE = 97,
    PACKET_DEPART_FOR_ISLAND = 99,
    PACKET_ISLAND_ARRIVE = 100,
    PACKET_ISLAND_ARRIVED = 101,
    PACKET_SEA_DEPARTURE_DONE = 102,
    PACKET_SEA_RETURN_DONE = 103,
    PACKET_RETIRE_FROM_ISLAND = 106,
    PACKET_ISLAND_TO_SEA = 107,
    PACKET_ISLAND_RETURN = 108,
    PACKET_TRIP_REQUEST = 110,
    PACKET_TRIP_SELECTED = 111,
    PACKET_TRIP_GUEST_READY = 112,
    PACKET_TRIP_GUEST_NOT_READY = 113,
    PACKET_TRIP_CANCEL_REQUEST = 114,
    PACKET_TRIP_CANCEL = 115,
    PACKET_TOWN_RETURN = 117,
    PACKET_TRIP_LEAVE_ISLAND = 119,
};

// sends a packet without data to the guests
DECOMP_ALWAYS_INLINE void SendToGuests(u8 packet)
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    PacketQueue& queue = mgr->m_PacketQueue;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (no != PLAYER_NO_HOST && mgr->IsAdmitted(no)) {
            queue.BeginPacket();
            queue.EndPacket(packet, no);
        }
    }
}

// fills the data of the tour and sends it to the guests
DECOMP_ALWAYS_INLINE void SendTourData(const u8* data)
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    PacketQueue& queue = mgr->m_PacketQueue;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (no != PLAYER_NO_HOST && mgr->IsAdmitted(no)) {
            queue.BeginPacket();
            queue.AddPacketData(data, 220);
            queue.EndPacket(PACKET_TOUR_DATA, no);
        }
    }
}

DECOMP_ALWAYS_INLINE void SetupTour(s8 tourName)
{
    u8* tour = reinterpret_cast<u8*>(TourResult::s_pInstance);
    Func6C4D50(tour);
    if (static_cast<u32>(tourName) < TOUR_NUM) {
        u8* data = tour + 16;
        Func6C547C(data, tourName);
        u8* current = reinterpret_cast<u8*>(TourResult::s_pInstance);
        Func6C499C(current);
        Func6A7498(current);
        Func6C53FC(data);
        Func6C4A60(tour);
    }
}

DECOMP_ALWAYS_INLINE bool AreOthersInRetire(NetGameMgr* mgr)
{
    return mgr->GetPlayerCount() - 1 == mgr->m_StageMgr.m_Stages.GetPlayerCount(stage::NAME_RANDOM_MATCHING_RETIRE) &&
           mgr->m_StageMgr.GetTransitioningPlayer() >= PLAYER_NO_MAX;
}

// sets the bit to according to the bit from
DECOMP_ALWAYS_INLINE void MoveFlag(volatile u8& flags, u8 from, u8 to)
{
    if ((flags & from) != 0) {
        flags |= to;
    } else {
        flags &= ~to;
    }
}

DECOMP_ALWAYS_INLINE void ToTourDesk()
{
    stage::GetTransitionMgr()->ClearReturnPoint();
    math::Angle16 rotations[PLAYER_NO_MAX] = {};
    stage::GetTransitionMgr()->TransitionWithStartPositions(stage::NAME_TOUR_DESK, PLAYER_ACTION_EXIT_CONNECTION_WALK, PLAYER_ACTION_HIDE,
                                                            PLAYER_ACTION_IDLE, PLAYER_ACTION_2, &s_TourDeskPositions[0], &rotations[0],
                                                            &s_TourDeskPositions[1], &rotations[1], &s_TourDeskPositions[2], &rotations[2],
                                                            &s_TourDeskPositions[3], &rotations[3], 0, 0, 1, 1);
    Func2D5DC0();
}

DECOMP_ALWAYS_INLINE void ShowTourTelop()
{
    BsTelop::ShowTourTelop(static_cast<BsTelop::TelopName>(21), static_cast<TourName>(g_TourStatus.tourName));
}
} // namespace

// 0x0061B388 (name is ours)
void netgame::IslandMgr::Process()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    PacketQueue& queue = mgr->m_PacketQueue;
    switch (m_State) {
    case 1:
        if (stage::GetCurrent() == stage::NAME_SEA_DEPARTURE) {
            Func1AD28C(g_pUnknown954238, 0);
            Func6A76AC();
            Func10DEA0(reinterpret_cast<u8*>(GetGardenData()->GetNpcData()) + 4);
            if (mgr->GetMyPlayerNo() == PLAYER_NO_HOST) {
                m_State = 3;
            } else {
                m_State = 0;
            }
        }
        break;
    case 2: {
        stage::GetTransitionMgr()->Transition(stage::NAME_SEA_DEPARTURE, 0, 0, 1, 1);
        Func32D4E0();
        PlayerNo myNo = mgr->GetMyPlayerNo();
        SendToGuests(PACKET_ISLAND_ARRIVE);
        m_Confirms[2].Update(myNo, false);
        m_State = 1;
        break;
    }
    case 3:
        if (m_Confirms[2].IsComplete()) {
            Func61F5F8();
            mgr->GetMyPlayerNo();
            SendToGuests(PACKET_ISLAND_ARRIVED);
            m_Confirms[2].MachineBitTable::Reset();
            if (GetNetgameType() == TYPE_RANDOM_MATCH) {
                mgr->SendSessionClosed(false);
            }
            m_State = 0;
        }
        break;
    case 4:
        if (m_Confirms[1].IsComplete()) {
            if (IsMultiPlayer()) {
                unk_0x147 = true;
            } else {
                BsTourMgr::SetState(static_cast<BsTourMgr::State>(2));
                SvPlayer* player = SvGetCurrentPlayer();
                if (player != NULL) {
                    u32 first = Func6BDA24(player);
                    u32 second = Func3037DC(reinterpret_cast<u8*>(player) + 0x6B8C);
                    Func6B9808(Func2FD00C(), second);
                    Func6B7ED0(Func2FD00C(), first);
                }
                Func5C6688();
                Func1148DC();
                Func61E708();
            }
            mgr->GetMyPlayerNo();
            SendToGuests(PACKET_TOUR_ARRIVE);
            m_Confirms[1].MachineBitTable::Reset();
            m_State = 0;
        }
        break;
    case 5:
        if (m_Confirms[2].IsComplete()) {
            ToTourDesk();
            if (GetNetgameType() == TYPE_RANDOM_MATCH) {
                mgr->m_GateMgr.Open(GateMgr::FLAG_RANDOM_MATCH_TOUR);
            }
            mgr->GetMyPlayerNo();
            SendToGuests(PACKET_TOWN_RETURN);
            m_Confirms[2].MachineBitTable::Reset();
            m_State = 0;
        }
        break;
    case 6: {
        Func114828();
        PlayerNo myNo = mgr->GetMyPlayerNo();
        SendToGuests(PACKET_ISLAND_RETURN);
        m_Confirms[2].Update(myNo, false);
        m_State = 7;
        break;
    }
    case 7:
        if (m_Confirms[2].IsComplete()) {
            StartIslandReturn();
            mgr->GetMyPlayerNo();
            SendToGuests(PACKET_ISLAND_TO_SEA);
            mgr->SendSessionClosed(false);
            m_Confirms[2].MachineBitTable::Reset();
            m_State = 0;
        }
        break;
    case 8:
        if (m_Confirms[3].GetCount() == 0 && m_Confirms[4].GetCount() == 0) {
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                PlayerNo no = static_cast<PlayerNo>(i);
                if (no != PLAYER_NO_HOST && mgr->IsAdmitted(no)) {
                    queue.BeginPacket();
                    u16 param = m_ChosenTrip | (m_TripRequestPlayer << 8);
                    queue.AddPacketData(&param, sizeof(param));
                    queue.EndPacket(PACKET_TRIP_SELECTED, no);
                }
            }
            m_Confirms[3].Update(mgr->GetMyPlayerNo(), false);
            m_State = 9;
        }
        break;
    case 9:
        if (GetNetgameType() == TYPE_RANDOM_MATCH && m_ChosenTrip == TOUR_DESK_SELECTION_TOUR) {
            PlayerNo myNo = mgr->GetMyPlayerNo();
            PlayerNo requester = m_TripRequestPlayer;
            if (requester >= PLAYER_NO_MAX || requester != myNo || !Func5C8B84(0, requester) || g_TourStatus.unk_0x4 ||
                !demo::MakeOrderNetgameLock()) {
                break;
            }
            m_PlayersThatDepartForTour.Reset();
            m_PlayersThatDepartForTour.Add(m_TripRequestPlayer);
            m_PlayersThatDoNotDepartForTour.Reset();
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                PlayerNo no = static_cast<PlayerNo>(i);
                if (m_TripRequestPlayer != no && ExistsPlayer(no)) {
                    if (Func5C8B84(0, no)) {
                        m_PlayersThatDepartForTour.Add(no);
                    } else {
                        m_PlayersThatDoNotDepartForTour.Add(no);
                    }
                }
            }
            UpdatePlayerTourSplit(m_PlayersThatDepartForTour.GetBit(), m_PlayersThatDoNotDepartForTour.GetBit());
            u32 split = (m_TourName << 16) | (m_PlayersThatDepartForTour.GetBit() << 8) | m_PlayersThatDoNotDepartForTour.GetBit();
            queue.BeginPacket();
            queue.AddPacketData(&split, sizeof(split));
            queue.EndPacket(PACKET_TOUR_SPLIT, mgr->GetMyPlayerNo() != PLAYER_NO_HOST ? PLAYER_NO_HOST : PLAYER_NO_INVALID);
            m_State = 17;
        } else {
            PlayerNo myNo = GetMyPlayerNo();
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                PlayerNo no = static_cast<PlayerNo>(i);
                if ((no == myNo || ExistsPlayer(no)) && !Func5C8B84(0, no)) {
                    return;
                }
            }
            if (demo::MakeOrderNetgameLock()) {
                if (mgr->GetMyPlayerNo() != PLAYER_NO_HOST) {
                    queue.BeginPacket();
                    queue.EndPacket(PACKET_TRIP_GUEST_READY, PLAYER_NO_HOST);
                } else {
                    m_Confirms[3].Add(myNo);
                }
                m_State = 10;
            }
        }
        break;
    case 10: {
        PlayerNo myNo = GetMyPlayerNo();
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            PlayerNo no = static_cast<PlayerNo>(i);
            if (no != myNo && ExistsPlayer(no) && !Func5C8B84(0, no)) {
                demo::RevokeNetgameLockOrder();
                if (mgr->GetMyPlayerNo() != PLAYER_NO_HOST) {
                    queue.BeginPacket();
                    queue.EndPacket(PACKET_TRIP_GUEST_NOT_READY, PLAYER_NO_HOST);
                } else {
                    m_Confirms[3].Remove(myNo);
                }
                m_State = 9;
                return;
            }
        }
        if (mgr->GetMyPlayerNo() != PLAYER_NO_HOST || !m_Confirms[3].IsComplete()) {
            break;
        }
        if (m_ChosenTrip == TOUR_DESK_SELECTION_TOUR) {
            m_Confirms[1].Update(myNo, true);
            if (IsMultiPlayer()) {
                SendToGuests(PACKET_TOUR_START);
            } else {
                SetupTour(m_TourName);
                SendTourData(reinterpret_cast<u8*>(TourResult::s_pInstance) + 16);
            }
            m_Confirms[3].MachineBitTable::Reset();
            m_Timer = s_TourTelopFrames;
            m_State = 11;
        } else if (m_ChosenTrip == TOUR_DESK_SELECTION_LEAVE_ISLAND) {
            queue.BeginPacket();
            queue.EndPacket(PACKET_TRIP_LEAVE_ISLAND, PLAYER_NO_INVALID);
            m_Confirms[3].MachineBitTable::Reset();
            m_State = 6;
        }
        break;
    }
    case 11:
        if (m_Timer != 0) {
            ShowTourTelop();
            m_ShowingTelop = true;
        } else if (mgr->GetMyPlayerNo() != PLAYER_NO_HOST) {
            unk_0x141 = false;
            Func608F60();
            stage::Transition(stage::NAME_PREPARE_TOUR, 1, 0, 1, 1);
            SoundMgr::PlaySound(SE_SYS_TOUR_SHIP_WHISTLE, 0.0f);
            m_State = 0;
        } else {
            unk_0x141 = false;
            if (IsMultiPlayer()) {
                Func608F44();
                stage::Transition(stage::NAME_PREPARE_TOUR, 1, 0, 1, 1);
                SoundMgr::PlaySound(SE_SYS_TOUR_SHIP_WHISTLE, 0.0f);
                m_Confirms[1].Update(PLAYER_NO_HOST, false);
                m_State = 14;
            } else {
                m_State = 4;
            }
        }
        if (m_Timer != 0) {
            m_Timer--;
        }
        break;
    case 12: {
        mgr->GetMyPlayerNo();
        SendToGuests(PACKET_TRIP_CANCEL);
        PlayerNo myNo = mgr->GetMyPlayerNo();
        if (demo::CheckOrder(demo::NETGAME_LOCK, false)) {
            demo::RevokeNetgameLockOrder();
        }
        m_Confirms[3].Remove(myNo);
        m_TourName = 0xFF;
        m_TripRequestPlayer = PLAYER_NO_INVALID;
        m_Confirms[4].Update(myNo, true);
        m_State = 13;
    }
        // fall through
    case 13:
        if (m_Confirms[4].IsComplete()) {
            m_Confirms[3].MachineBitTable::Reset();
            m_Confirms[4].MachineBitTable::Reset();
            m_ChosenTrip = TOUR_DESK_SELECTION_NONE;
            m_State = 0;
        }
        break;
    case 14:
        if (m_Confirms[1].IsComplete()) {
            m_State = 15;
        }
        break;
    case 15:
        if (CanSendToAll(PLAYER_NO_INVALID)) {
            unk_0x141 = true;
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                PlayerNo no = static_cast<PlayerNo>(i);
                if (no != PLAYER_NO_HOST && (ExistsPlayer(no) || no == PLAYER_NO_INVALID)) {
                    mgr->SendPacketBatch(no, NULL, NULL, 0, 20, 0, 0, 0);
                }
            }
            m_Confirms[1].Update(mgr->GetMyPlayerNo(), false);
            m_State = 16;
        }
        break;
    case 16:
        if (m_Confirms[1].IsComplete()) {
            PlayerNo myNo = GetMyPlayerNo();
            SetupTour(m_TourName);
            m_Confirms[1].Update(myNo, true);
            SendTourData(reinterpret_cast<u8*>(TourResult::s_pInstance) + 16);
            m_State = 4;
        }
        break;
    case 17:
        if (mgr->RequestLock7()) {
            m_State = 18;
        }
        break;
    case 18:
        if (mgr->GetLockResult() > 0) {
            m_Timer = s_TourTelopFrames;
            m_State = 19;
        }
        break;
    case 19:
        if (m_Timer != 0) {
            ShowTourTelop();
            m_ShowingTelop = true;
        } else {
            if (mgr->GetMyPlayerNo() != PLAYER_NO_HOST) {
                BsSvMgr::SetSaveStartTourRandomMatchNetVst();
            } else {
                BsSvMgr::SetSaveStartTourRandomMatchNetHst();
            }
            PlayerNo myNo = mgr->GetMyPlayerNo();
            unk_0x141 = false;
            m_Confirms[1].MachineBitTable::Reset();
            m_Confirms[3].MachineBitTable::Reset();
            m_Confirms[4].MachineBitTable::Reset();
            m_ChosenTrip = TOUR_DESK_SELECTION_NONE;
            if (IsHost()) {
                m_Confirms[1].Update(myNo, false);
                m_State = 69;
            } else {
                unk_0x145 = false;
                m_State = 74;
            }
            if (m_PlayersThatDepartForTour.IsSet(myNo)) {
                SoundMgr::PlaySound(SE_SYS_TOUR_SHIP_WHISTLE, 0.0f);
            }
            stage::Transition(stage::NAME_PREPARE_TOUR, 1, 0, 1, 1);
        }
        if (m_Timer != 0) {
            m_Timer--;
        }
        break;
    case 21:
        if (net::Net::s_Instance.IsError()) {
            if (net::Net::s_Instance.GetErrorLevel() >= 2) {
                m_State = 95;
                return;
            }
            if (net::nex::Impl::GetServerSetting1() <= m_RetryCount ||
                (net::Net::s_Instance.GetErrorKind() == 6 && net::nex::Impl::GetServerSetting2() == 0)) {
                m_State = 22;
                return;
            }
            m_RetryCount++;
        }
        if (mgr->CreateOrJoinRandomSession(m_RandomMatchParam)) {
            if (net::Net::s_Instance.IsHost()) {
                Func6A76AC();
                Func6A7CC4();
                Func1AD28C(g_pUnknown954238, 1);
                m_Timestamp = Func2FF63C(0x7FFFFFFF);
                m_State = 24;
            } else {
                unk_0x13A = 0;
                m_ResetsClock = true;
                m_State = 26;
            }
        }
        break;
    case 22:
        if (net::Net::s_Instance.IsError() && net::Net::s_Instance.GetErrorLevel() >= 2) {
            m_State = 95;
            return;
        }
        if (mgr->CreateRandomSession(m_RandomMatchParam, false)) {
            Func6A76AC();
            Func6A7CC4();
            m_Timestamp = Func2FF63C(0x7FFFFFFF);
            Func1AD28C(g_pUnknown954238, 1);
            m_State = 23;
        }
        break;
    case 23:
        if ((!net::Net::s_Instance.IsError() || net::Net::s_Instance.GetErrorLevel() < 2) &&
            net::Net::s_Instance.ModifySessionAttribute(NULL)) {
            m_State = 24;
        }
        break;
    case 24:
        if (Func6A7F04(m_Timestamp)) {
            m_State = 25;
        }
        break;
    case 25:
        if (Func6A7EE8()) {
            m_Confirms[2].Update(mgr->GetMyPlayerNo(), false);
            Func5C4D04(&SvFgName::s_Empty);
            Func5C7050();
            m_State = 3;
        }
        break;
    case 26:
        if (net::Net::s_Instance.GetSessionTime() < 0) {
            unk_0x13A++;
            if (unk_0x13A > 300) {
                m_State = 28;
            }
            return;
        }
        if (m_ResetsClock) {
            if (!net::Net::s_Instance.IsError() && !net::Net::s_Instance.ResetSyncClock()) {
                return;
            }
            m_ResetsClock = false;
        }
        if (mgr->RequestLock5()) {
            m_State = 27;
        }
        break;
    case 27: {
        if (!netgame::IsInSession(PLAYER_NO_HOST, true)) {
            m_State = 95;
            return;
        }
        s32 result = GetLockResult();
        if (result == 0) {
            return;
        }
        if (result < 0) {
            m_State = 28;
            return;
        }
        Func6A76AC();
        Func6A7CC4();
        Func5C4D04(&SvFgName::s_Empty);
        Func5C7050();
        StartEntry();
        m_State = 29;
        break;
    }
    case 28:
        if (net::Net::s_Instance.LeaveSession()) {
            mgr->InitEntryOrExit(true);
            m_Confirms[1].MachineBitTable::Reset();
            if (net::nex::Impl::GetServerSetting2() >= 2 && net::nex::Impl::GetServerSetting1() > m_RetryCount) {
                m_State = 21;
                m_RetryCount++;
            } else {
                m_State = 22;
            }
        }
        break;
    case 29:
        if (IsJoinStarted()) {
            SetJoinAck();
            m_State = 30;
        }
        // fall through
    case 30:
        if (AreJoinFlagsSet()) {
            m_State = 31;
        }
        break;
    case 31:
        if (Func6A7EE8()) {
            m_State = 32;
        }
        break;
    case 32:
        if (IsJoinDone()) {
            LoadEntry();
            m_State = 0;
            mgr->SendSessionClosed(false);
        }
        break;
    case 33:
        if (GetMgr8Flag()) {
            ResetMgr8();
            ClearLockSource();
            SetLockUnlocked();
            Func61F5F8();
            m_State = 0;
        }
        break;
    case 34:
        Func114828();
        m_Confirms[2].Update(mgr->GetMyPlayerNo(), false);
        m_State = 35;
        break;
    case 36:
        if (StopNet()) {
            m_Confirms[2].Update(PLAYER_NO_HOST, false);
            m_State = 7;
        }
        break;
    case 37:
        if (AreOthersInRetire(mgr)) {
            mgr->m_EntryData.SendFlag(mgr->GetMyPlayerNo());
            queue.BeginPacket();
            queue.EndPacket(PACKET_RETIRE, PLAYER_NO_INVALID);
            m_State = 38;
        }
        break;
    case 38: {
        PlayerNo myNo = mgr->GetMyPlayerNo();
        if (mgr->m_AvatarMgr.CanSend(myNo) && m_Confirms[5].IsComplete()) {
            queue.m_LeavingPlayer = myNo;
            queue.m_IsClosed = true;
            m_State = 39;
        }
        break;
    }
    case 40:
        if (unk_0x140) {
            mgr->m_Bits[2].Reset();
            m_State = 41;
        }
        break;
    case 41:
        if (mgr->DisconnectCurrent()) {
            mgr->m_LeaveMgr.NotifyEventB(m_LeavingPlayer);
            m_Confirms[2].Update(mgr->GetMyPlayerNo(), false);
            m_State = 42;
        }
        break;
    case 42:
        if (m_Confirms[2].IsSet(m_LeavingPlayer)) {
            StartIslandReturn();
            mgr->SendSessionClosed(false);
            m_Confirms[2].MachineBitTable::Reset();
            m_State = 43;
        }
        break;
    case 44:
        if (AreOthersInRetire(mgr)) {
            m_State = 45;
        }
        break;
    case 45: {
        PlayerNo myNo = mgr->GetMyPlayerNo();
        if (mgr->m_StageMgr.GetTransitioningPlayer() >= PLAYER_NO_MAX && m_Confirms[5].IsSet(myNo)) {
            PlayerNo leaving = m_LeavingPlayer;
            queue.BeginPacket();
            queue.EndPacket(PACKET_RETIRE_READY, leaving);
            m_State = 46;
        }
        break;
    }
    case 47:
        if (mgr->m_LeaveMgr.CheckLeaveA(m_LeavingPlayer)) {
            m_State = 48;
        }
        break;
    case 48:
        if (IsLeaveConfirmed()) {
            mgr->m_Bits[2].Remove(m_LeavingPlayer);
            m_State = 49;
        }
        break;
    case 49:
        if (!mgr->IsInSession(m_LeavingPlayer, true)) {
            m_State = 50;
        }
        break;
    case 51:
        if (mgr->m_StageMgr.GetTransitioningPlayer() >= PLAYER_NO_MAX && m_Confirms[1].IsComplete()) {
            mgr->m_PacketQueue.m_IsClosed = true;
            NetGameMgr::SaveDemoMgr::NotifyLeaveFinished();
            m_State = 52;
        }
        break;
    case 52:
        if (unk_0x140) {
            mgr->m_Bits[2].Reset();
            m_State = 53;
        }
        break;
    case 53:
        if (mgr->DisconnectCurrent()) {
            NetGameMgr::SaveDemoMgr::NotifyEvent();
            m_Confirms[2].Update(mgr->GetMyPlayerNo(), false);
            m_State = 54;
        }
        break;
    case 54:
        if (m_Confirms[2].IsSet(m_LeavingPlayer)) {
            StartIslandReturn();
            m_Confirms[2].MachineBitTable::Reset();
            m_State = 55;
        }
        break;
    case 56: {
        if (mgr->m_StageMgr.m_Stages.GetStage(PLAYER_NO_HOST) != stage::NAME_SEA_RETURN || !AreOthersInRetire(mgr)) {
            break;
        }
        PlayerNo myNo = mgr->m_MyPlayerNo;
        if (myNo >= PLAYER_NO_MAX || myNo == PLAYER_NO_HOST) {
            break;
        }
        mgr->m_EntryData.SendFlag(myNo);
        m_NewHostPrincipalId = 0;
        PlayerNo newHost = PLAYER_NO_INVALID;
        if (IsMultiplePlayer()) {
            for (s32 i = 1; i < PLAYER_NO_MAX; i++) {
                if (ExistsPlayer(static_cast<PlayerNo>(i))) {
                    newHost = static_cast<PlayerNo>(i);
                    break;
                }
            }
        }
        m_IsNewHost = newHost == myNo;
        if (m_IsNewHost) {
            m_NewHostPrincipalId = FriendList::s_pInstance->GetMyPrincipalId();
        } else {
            const AvatarMgr::AvatarInfo* info = mgr->m_AvatarMgr.GetAvatarInfo(newHost);
            if (info != NULL) {
                m_NewHostPrincipalId = info->principalId;
            }
        }
        u32 count = netgame::GetPlayerCount();
        m_StayingPlayerNum = count > 1 ? count - 1 : 1;
        m_State = 57;
        break;
    }
    case 58:
        if (IsLeaveSaved()) {
            mgr->m_Bits[2].Reset();
            m_State = 59;
        }
        break;
    case 59:
        if (!mgr->IsInSession(PLAYER_NO_HOST, false)) {
            NetGameMgr::SaveDemoMgr::NotifySessionEnd();
            m_State = 60;
        }
        break;
    case 60:
        if (net::Net::s_Instance.LeaveSession()) {
            NetGameMgr::SaveDemoMgr::NotifyEvent();
            Func5C5874();
            Func5C6C4C();
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                Func51DD6C(&m_NpcInfos[i], i);
            }
            CloseSession();
            mgr->InitEntryOrExit(true);
            m_IsRejoining = true;
            m_Confirms[1].MachineBitTable::Reset();
            m_RetryTimer = 0;
            m_State = m_IsNewHost ? 61 : 64;
        }
        break;
    case 61:
        if (mgr->CreateRandomSession(m_RandomMatchParam, true)) {
            m_Confirms[1].Add(PLAYER_NO_HOST);
            m_RetryTimer = 0;
            m_State = 62;
        }
        break;
    case 62:
        if (m_Confirms[1].GetCount() == m_StayingPlayerNum) {
            mgr->m_StageMgr.SetStages(mgr->m_Bits[0], stage::NAME_RANDOM_MATCHING_RETIRE);
            m_State = 63;
        } else if (m_RetryTimer < RETRY_FRAMES) {
            m_RetryTimer++;
        } else {
            mgr->m_Flags |= NetGameMgr::FLAG_STATION_LOST;
        }
        break;
    case 63:
        if ((!net::Net::s_Instance.IsError() || net::Net::s_Instance.GetErrorLevel() < 2) &&
            net::Net::s_Instance.ModifySessionAttribute(NULL)) {
            unk_0x13E = true;
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                PlayerNo no = static_cast<PlayerNo>(i);
                if (no != PLAYER_NO_HOST && mgr->IsAdmitted(no)) {
                    queue.BeginPacket();
                    queue.EndPacket(PACKET_NEW_HOST, no);
                    mgr->m_Mgr8B8.Add(no);
                }
            }
            m_IsRejoining = false;
            mgr->SendSessionClosed(false);
            mgr->m_AvatarMgr.m_HasJoiner = false;
            m_State = 67;
        }
        break;
    case 64:
        if ((!net::Net::s_Instance.IsError() || net::Net::s_Instance.GetErrorLevel() < 2) && mgr->m_ErrorFlags == 0 &&
            mgr->m_Flags == 0 && mgr->JoinSessionByOwner(&m_NewHostPrincipalId)) {
            m_RetryTimer = 0;
            unk_0x13A = 0;
            m_State = 65;
            return;
        }
        if (m_RetryTimer < RETRY_FRAMES) {
            m_RetryTimer++;
        } else if (net::Net::s_Instance.IsError()) {
            mgr->m_Flags |= NetGameMgr::FLAG_STATION_LOST;
        }
        break;
    case 65: {
        PlayerNo myNo = mgr->m_MyPlayerNo;
        if (myNo < PLAYER_NO_MAX && mgr->IsEntering(PLAYER_NO_HOST) && net::Net::s_Instance.GetSessionTime() >= 0) {
            u32 principalId = FriendList::s_pInstance->GetMyPrincipalId();
            AddPrincipal(myNo, &principalId, false);
            mgr->m_Bits[2].Add(PLAYER_NO_HOST);
            m_ResetsClock = true;
            m_State = 66;
        } else if (m_RetryTimer < RETRY_FRAMES) {
            m_RetryTimer++;
        } else {
            mgr->m_Flags |= NetGameMgr::FLAG_STATION_LOST;
            net::Net::s_Instance.GetSessionTime();
        }
        break;
    }
    case 66: {
        if (m_ResetsClock) {
            if (!net::Net::s_Instance.IsError() && !net::Net::s_Instance.ResetSyncClock()) {
                return;
            }
            m_ResetsClock = false;
        }
        PlayerNo myNo = mgr->GetMyPlayerNo();
        MachineBitTable& players = mgr->m_Bits[0];
        if (m_StayingPlayerNum != players.GetCount()) {
            return;
        }
        bool isReady = true;
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            PlayerNo no = static_cast<PlayerNo>(i);
            if (no == myNo || no == PLAYER_NO_HOST || !players.IsSet(no) || mgr->m_Bits[2].IsSet(no)) {
                continue;
            }
            if (mgr->IsEntering(no)) {
                mgr->m_Bits[2].Add(no);
            } else {
                isReady = false;
            }
        }
        if (isReady) {
            queue.BeginPacket();
            queue.EndPacket(PACKET_REJOINED, PLAYER_NO_HOST);
            mgr->m_StageMgr.SetStages(players, stage::NAME_RANDOM_MATCHING_RETIRE);
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                PlayerNo no = static_cast<PlayerNo>(i);
                if (no != myNo && players.IsSet(no)) {
                    mgr->m_Mgr8B8.Add(no);
                }
            }
            m_State = 67;
        }
        break;
    }
    case 69:
        if (m_Confirms[1].IsComplete()) {
            m_State = 70;
        }
        break;
    case 70:
        if (CanSendToAll(PLAYER_NO_INVALID)) {
            unk_0x141 = true;
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                PlayerNo no = static_cast<PlayerNo>(i);
                if (no != PLAYER_NO_HOST && (ExistsPlayer(no) || no == PLAYER_NO_INVALID)) {
                    mgr->SendPacketBatch(no, NULL, NULL, 0, 20, 0, 0, 0);
                }
            }
            m_Confirms[1].Update(mgr->GetMyPlayerNo(), false);
            m_State = 71;
        }
        break;
    case 71:
        if (m_Confirms[1].IsComplete()) {
            m_Confirms[1].MachineBitTable::Reset();
            m_State = 72;
        }
        break;
    case 72: {
        const MachineBitTable& existing = mgr->m_Bits[1];
        const MachineBitTable& hostSide =
            m_PlayersThatDepartForTour.IsSet(PLAYER_NO_HOST) ? m_PlayersThatDepartForTour : m_PlayersThatDoNotDepartForTour;
        PlayerNo myNo = mgr->GetMyPlayerNo();
        if (existing.GetBit() == hostSide.GetBit() && existing.GetCount() == hostSide.GetCount()) {
            ClearLockSource();
            SetLockUnlocked();
            mgr->m_StageMgr.SetStages(existing, stage::NAME_PREPARE_TOUR);
            queue.BeginPacket();
            queue.EndPacket(PACKET_TOUR_ALL_STAY, PLAYER_NO_INVALID);
            if (m_IsSelfDepartingForTour && m_PlayersThatDoNotDepartForTour.GetCount() != 0) {
                m_State = 80;
                return;
            }
            if (!m_IsSelfDepartingForTour) {
                mgr->m_GateMgr.Open(GateMgr::FLAG_RANDOM_MATCH_TOUR);
            }
            m_Confirms[1].MachineBitTable::Reset();
            m_State = 84;
            return;
        }
        const MachineBitTable& otherSide =
            m_PlayersThatDepartForTour.IsSet(PLAYER_NO_HOST) ? m_PlayersThatDoNotDepartForTour : m_PlayersThatDepartForTour;
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            PlayerNo no = static_cast<PlayerNo>(i);
            if (mgr->IsAdmitted(no) && otherSide.IsSet(no)) {
                m_LeavingPlayer = no;
                break;
            }
        }
        u8 leaving = m_LeavingPlayer;
        queue.BeginPacket();
        queue.AddPacketData(&leaving, sizeof(leaving));
        queue.EndPacket(PACKET_TOUR_LEAVING, PLAYER_NO_INVALID);
        m_Confirms[5].Update(myNo, false);
        m_State = 89;
        break;
    }
    case 73:
        if (m_Confirms[1].IsComplete()) {
            unk_0x147 = true;
            mgr->GetMyPlayerNo();
            SendToGuests(PACKET_TOUR_ARRIVE);
            m_Confirms[1].MachineBitTable::Reset();
            m_State = 0;
        }
        break;
    case 74:
        if (unk_0x143) {
            PlayerNo myNo = GetMyPlayerNo();
            if (m_LeavingPlayer != myNo) {
                m_State = 89;
            } else {
                mgr->m_EntryData.SendFlag(myNo);
                queue.BeginPacket();
                queue.EndPacket(PACKET_RETIRE, PLAYER_NO_INVALID);
                m_Confirms[5].Update(myNo, true);
                m_State = 75;
            }
            unk_0x143 = false;
        }
        break;
    case 75: {
        PlayerNo myNo = mgr->GetMyPlayerNo();
        if (mgr->m_AvatarMgr.CanSend(myNo) && m_Confirms[5].IsComplete()) {
            queue.m_LeavingPlayer = myNo;
            queue.m_IsClosed = true;
            mgr->m_Bits[2].Reset();
            m_State = 76;
        }
        break;
    }
    case 76:
        if (net::Net::s_Instance.LeaveSession()) {
            LeaveMgr& leave = mgr->m_LeaveMgr;
            leave.NotifyEventB(m_LeavingPlayer);
            leave.NotifyEventB(m_LeavingPlayer);
            leave.CheckLeaveB(m_LeavingPlayer);
            leave.NotifySessionEnd(m_LeavingPlayer);
            m_Confirms[2].Update(mgr->GetMyPlayerNo(), false);
            Func5C5874();
            Func5C6C4C();
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                Func51DD6C(&m_NpcInfos[i], i);
            }
            CloseSession();
            mgr->InitEntryOrExit(true);
            m_IsRejoining = true;
            m_Confirms[1].MachineBitTable::Reset();
            if (m_IsNewHost) {
                m_State = 77;
            } else {
                m_RetryTimer = 0;
                m_State = 86;
            }
        }
        break;
    case 77:
        if (mgr->CreateRandomSession(m_RandomMatchParam, true)) {
            m_Confirms[1].Add(PLAYER_NO_HOST);
            m_RetryTimer = 0;
            m_State = 78;
        }
        break;
    case 78:
        if (m_Confirms[1].GetCount() == m_StayingPlayerNum) {
            mgr->m_StageMgr.SetStages(mgr->m_Bits[0], stage::NAME_PREPARE_TOUR);
            m_State = 79;
        } else if (m_RetryTimer < RETRY_FRAMES) {
            m_RetryTimer++;
        } else {
            mgr->m_Flags |= NetGameMgr::FLAG_STATION_LOST;
        }
        break;
    case 79:
        if ((!net::Net::s_Instance.IsError() || net::Net::s_Instance.GetErrorLevel() < 2) &&
            net::Net::s_Instance.ModifySessionAttribute(NULL)) {
            unk_0x13E = true;
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                PlayerNo no = static_cast<PlayerNo>(i);
                if (no != PLAYER_NO_HOST && mgr->IsAdmitted(no)) {
                    queue.BeginPacket();
                    queue.EndPacket(PACKET_NEW_HOST, no);
                    mgr->m_Mgr8B8.Add(no);
                }
            }
            m_IsRejoining = false;
            mgr->SendSessionClosed(false);
            mgr->m_AvatarMgr.m_HasJoiner = false;
            unk_0x13E = false;
            if (m_IsSelfDepartingForTour) {
                mgr->m_GateMgr.Close(GateMgr::FLAG_RANDOM_MATCH_TOUR);
                m_State = 80;
            } else {
                m_State = 84;
            }
        }
        break;
    case 80:
        if (Func6A7F04(m_Timestamp)) {
            m_State = 81;
        }
        break;
    case 81:
        if (Func6A7EE8()) {
            PlayerNo myNo = mgr->GetMyPlayerNo();
            m_Confirms[0].Update(myNo, true);
            m_Confirms[1].Update(myNo, true);
            m_State = 82;
        }
        break;
    case 82: {
        bool isPending = false;
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            PlayerNo no = static_cast<PlayerNo>(i);
            if (m_Confirms[0].IsSet(no)) {
                continue;
            }
            if (mgr->CanSend(no, true, 1)) {
                const void* parts[2];
                size_t partSizes[2];
                u32 count = 1;
                parts[0] = static_cast<u8*>(g_pUnknown954238) + 16;
                partSizes[0] = 16;
                void* map = GetIslandMap();
                if (map != NULL) {
                    parts[1] = map;
                    partSizes[1] = ISLAND_MAP_SIZE;
                    count = 2;
                }
                if (mgr->SendPacketBatch(no, parts, partSizes, count, PACKET_ISLAND_DATA, 0, 1, 0)) {
                    m_Confirms[0].Add(no);
                }
            } else {
                queue.IncrementCounter(no);
            }
            isPending = true;
        }
        if (!isPending) {
            m_State = 83;
        }
        break;
    }
    case 83:
        if (m_Confirms[1].IsComplete()) {
            m_Confirms[1].MachineBitTable::Reset();
            m_State = 84;
        }
        break;
    case 84:
        if (m_IsSelfDepartingForTour) {
            PlayerNo myNo = mgr->GetMyPlayerNo();
            SetupTour(m_TourName);
            m_Confirms[1].Update(myNo, true);
            SendTourData(reinterpret_cast<u8*>(TourResult::s_pInstance) + 16);
            m_State = 73;
        } else {
            unk_0x147 = true;
            queue.BeginPacket();
            queue.EndPacket(PACKET_TOUR_RESULT, PLAYER_NO_INVALID);
            m_State = 0;
        }
        break;
    case 86:
        if ((!net::Net::s_Instance.IsError() || net::Net::s_Instance.GetErrorLevel() < 2) && mgr->m_ErrorFlags == 0 &&
            mgr->m_Flags == 0 && mgr->JoinSessionByOwner(&m_NewHostPrincipalId)) {
            m_RetryTimer = 0;
            unk_0x13A = 0;
            m_State = 87;
            return;
        }
        if (m_RetryTimer < RETRY_FRAMES) {
            m_RetryTimer++;
        } else if (net::Net::s_Instance.IsError()) {
            mgr->m_Flags |= NetGameMgr::FLAG_STATION_LOST;
        }
        break;
    case 87: {
        PlayerNo myNo = mgr->m_MyPlayerNo;
        if (myNo < PLAYER_NO_MAX && mgr->IsEntering(PLAYER_NO_HOST) && net::Net::s_Instance.GetSessionTime() >= 0) {
            u32 principalId = FriendList::s_pInstance->GetMyPrincipalId();
            AddPrincipal(myNo, &principalId, false);
            mgr->m_Bits[2].Add(PLAYER_NO_HOST);
            m_ResetsClock = true;
            m_State = 88;
        } else if (m_RetryTimer < RETRY_FRAMES) {
            m_RetryTimer++;
        } else {
            mgr->m_Flags |= NetGameMgr::FLAG_STATION_LOST;
            net::Net::s_Instance.GetSessionTime();
        }
        break;
    }
    case 88: {
        if (m_ResetsClock) {
            if (!net::Net::s_Instance.IsError() && !net::Net::s_Instance.ResetSyncClock()) {
                return;
            }
            m_ResetsClock = false;
        }
        PlayerNo myNo = mgr->GetMyPlayerNo();
        MachineBitTable& players = mgr->m_Bits[0];
        if (m_StayingPlayerNum != players.GetCount()) {
            return;
        }
        bool isReady = true;
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            PlayerNo no = static_cast<PlayerNo>(i);
            if (no == myNo || no == PLAYER_NO_HOST || !players.IsSet(no) || mgr->m_Bits[2].IsSet(no)) {
                continue;
            }
            if (mgr->IsEntering(no)) {
                mgr->m_Bits[2].Add(no);
            } else {
                isReady = false;
            }
        }
        if (isReady) {
            queue.BeginPacket();
            queue.EndPacket(PACKET_REJOINED, PLAYER_NO_HOST);
            mgr->m_StageMgr.SetStages(players, stage::NAME_PREPARE_TOUR);
            for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
                PlayerNo no = static_cast<PlayerNo>(i);
                if (no != myNo && players.IsSet(no)) {
                    mgr->m_Mgr8B8.Add(no);
                }
            }
            m_State = 85;
        }
        break;
    }
    case 89:
        if (m_Confirms[5].IsSet(mgr->GetMyPlayerNo())) {
            PlayerNo leaving = m_LeavingPlayer;
            queue.BeginPacket();
            queue.EndPacket(PACKET_RETIRE_READY, leaving);
            m_State = 90;
        }
        break;
    case 91:
        if (mgr->m_LeaveMgr.CheckLeaveA(m_LeavingPlayer)) {
            m_State = 92;
        }
        break;
    case 92: {
        PlayerNo leaving = m_LeavingPlayer;
        if (mgr->IsInSession(leaving, true)) {
            break;
        }
        mgr->m_LeaveMgr.NotifyLeaveFinished(m_LeavingPlayer);
        if (leaving >= PLAYER_NO_MAX) {
            break;
        }
        mgr->RemovePlayer(leaving);
        mgr->m_LockMgr.m_SourcePlayer = PLAYER_NO_INVALID;
        mgr->m_PacketQueue.m_LeavingPlayer = PLAYER_NO_INVALID;
        mgr->m_PacketQueue.m_IsClosed = false;
        PlayerNo myNo = mgr->GetMyPlayerNo();
        const MachineBitTable& hostSide =
            m_PlayersThatDepartForTour.IsSet(PLAYER_NO_HOST) ? m_PlayersThatDepartForTour : m_PlayersThatDoNotDepartForTour;
        if (hostSide.IsSet(myNo)) {
            OnPlayerLeft(leaving);
        }
        if (IsHost()) {
            m_Confirms[1].Update(myNo, true);
            queue.BeginPacket();
            queue.EndPacket(PACKET_TOUR_PLAYER_LEFT, PLAYER_NO_INVALID);
            m_State = 71;
        } else {
            m_State = 93;
        }
        break;
    }
    case 93:
        if (unk_0x145) {
            queue.BeginPacket();
            queue.EndPacket(PACKET_TOUR_GUEST_READY, PLAYER_NO_HOST);
            unk_0x145 = false;
            const MachineBitTable& hostSide =
                m_PlayersThatDepartForTour.IsSet(PLAYER_NO_HOST) ? m_PlayersThatDepartForTour : m_PlayersThatDoNotDepartForTour;
            const MachineBitTable& existing = mgr->m_Bits[1];
            if (!m_IsSelfDepartingForTour && existing.GetBit() == hostSide.GetBit() && existing.GetCount() == hostSide.GetCount()) {
                m_State = 94;
            } else {
                m_State = 74;
            }
        }
        break;
    default:
        break;
    }
}

// 0x0061E360 (name is ours)
void netgame::IslandMgr::ConfirmTourGuest()
{
    if (IsHost()) {
        m_Confirms[1].Add(PLAYER_NO_HOST);
        return;
    }
    PacketQueue& queue = NetGameMgr::s_pInstance->m_PacketQueue;
    queue.BeginPacket();
    queue.EndPacket(PACKET_TOUR_GUEST_READY, PLAYER_NO_HOST);
}

// 0x0061E3B0 (name after libgarden)
bool netgame::IslandMgr::StartFindRandomMatch()
{
    if (NetGameMgr::s_pInstance->m_IsConnectRandomMatch) {
        m_RetryCount = 0;
        m_State = 21;
    }
    return true;
}

// 0x0061E3E8 (name after libgarden)
bool netgame::IslandMgr::EndSeaDepartureDemo(bool stageChanged)
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->m_IsConnectRandomMatch) {
        if (!net::Net::s_Instance.IsHost() && !stageChanged) {
            SetJoinFinished();
            m_State = 33;
            return true;
        }
    } else if (mgr->GetMyPlayerNo() != PLAYER_NO_HOST) {
        PacketQueue& queue = mgr->m_PacketQueue;
        queue.BeginPacket();
        queue.EndPacket(PACKET_SEA_DEPARTURE_DONE, PLAYER_NO_HOST);
        return true;
    }
    m_Confirms[2].Add(GetMyPlayerNo());
    return true;
}

// 0x0061E494 (name is ours)
void netgame::IslandMgr::ApplyTourData()
{
    u8* tour = reinterpret_cast<u8*>(TourResult::s_pInstance);
    Func6C499C(tour);
    Func6A7498(tour);
}
// 0x0061E4B8 (name is ours)
bool netgame::IslandMgr::ProcessLeave()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->unk_0x132B4 || applet::ErrEulaMgr::IsInputBlocked()) {
        return true;
    }
    bool result = true;
    if (m_State == 43) {
        if (!mgr->m_LeaveMgr.CheckLeaveB(m_LeavingPlayer)) {
            result = false;
        }
        if (!mgr->m_LeaveMgr.CheckLeaveC(m_LeavingPlayer)) {
            result = false;
        }
    } else if (m_State == 55) {
        if (!NetGameMgr::SaveDemoMgr::CheckLeave()) {
            result = false;
        }
    }
    return result;
}

// 0x0061E568 (name after libgarden)
void netgame::IslandMgr::StartTourDeskArrivalAfterTour()
{
    if (NetGameMgr::s_pInstance->GetMyPlayerNo() == PLAYER_NO_HOST) {
        m_Confirms[2].Update(GetMyPlayerNo(), false);
        m_State = 5;
    }
}

// 0x0061E5A8 (name is ours)
void netgame::IslandMgr::StartTelopTimer()
{
    m_Timer = s_TourTelopFrames;
}

// 0x0061E5BC (name is ours)
void netgame::IslandMgr::ProcessEnd()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (!applet::ErrEulaMgr::IsInputBlocked() && !mgr->unk_0x132B4) {
        switch (m_State) {
        case 43:
            mgr->m_LeaveMgr.NotifySessionEnd(m_LeavingPlayer);
            mgr->ResetSession();
            break;
        case 50: {
            mgr->m_LeaveMgr.NotifyLeaveFinished(m_LeavingPlayer);
            PlayerNo leaving = m_LeavingPlayer;
            if (leaving < PLAYER_NO_MAX) {
                mgr->RemovePlayer(leaving);
                mgr->m_LockMgr.m_SourcePlayer = PLAYER_NO_INVALID;
                mgr->m_PacketQueue.m_LeavingPlayer = PLAYER_NO_INVALID;
                mgr->m_PacketQueue.m_IsClosed = false;
                OnPlayerLeft(leaving);
                Reset();
            }
            break;
        }
        case 55:
            mgr->ResetSession();
            break;
        case 67:
        case 85:
        case 94:
            Reset();
            unk_0x13E = false;
            m_IsRejoining = false;
            mgr->unk_0x132B6 = false;
            m_State = 0;
            break;
        default:
            break;
        }
    }
    if (m_ShowingTelop) {
        BsTelop::EndTelop();
        m_ShowingTelop = false;
        m_Timer = 0;
    }
}

// 0x0061E708 (name is ours)
void Func61E708()
{
    GetMyPlayerNo();
    nn::math::VEC3_ center;
    Func6C45A8(&center);
    nn::math::VEC3_ positions[PLAYER_NO_MAX];
    math::Angle16 rotations[PLAYER_NO_MAX];
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        Func6C4750(&positions[i], i);
        rotations[i] = math::Angle16::Atan2(center.x - positions[i].x, center.z - positions[i].z);
    }
    stage::Name stage = Func6C4514(Func6C4DD0(TourResult::s_pInstance->m_Tour));
    u8 isSolo = !IsMultiPlayer();
    stage::GetTransitionMgr()->TransitionWithStartPositions(stage, PLAYER_ACTION_IDLE, PLAYER_ACTION_2, &positions[0], &rotations[0],
                                                            &positions[1], &rotations[1], &positions[2], &rotations[2], &positions[3],
                                                            &rotations[3], isSolo, 0, 1, 1);
}

// 0x0061E84C (name is ours)
void netgame::IslandMgr::RemovePlayer(PlayerNo player)
{
    if (player >= PLAYER_NO_MAX) {
        return;
    }
    m_Confirms[1].Remove(player);
    m_Confirms[2].Remove(player);
    m_Confirms[3].Remove(player);
    m_Confirms[4].Remove(player);
    m_Confirms[5].Remove(player);
    m_Confirms[6].Remove(player);
    m_LeaveKind = 3;
    unk_0x140 = false;
    unk_0x142 = false;
}

// 0x0061E8BC (name after libgarden)
bool netgame::IslandMgr::DepartForIsland()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->m_IsConnectRandomMatch) {
        stage::GetTransitionMgr()->Transition(stage::NAME_SEA_DEPARTURE, 0, 0, 1, 1);
        m_State = 20;
    } else if (mgr->GetMyPlayerNo() == PLAYER_NO_HOST) {
        m_State = 2;
    } else {
        PacketQueue& queue = mgr->m_PacketQueue;
        queue.BeginPacket();
        queue.EndPacket(PACKET_DEPART_FOR_ISLAND, PLAYER_NO_HOST);
    }
    return true;
}

// 0x0061E954 (name after libgarden)
bool netgame::IslandMgr::RetireFromIsland()
{
    if (GetNetgameType() == TYPE_RANDOM_MATCH) {
        LeaveRandomMatch(LEAVE_RANDOM_MATCH_MODE_RETIRE);
    } else {
        NetGameMgr* mgr = NetGameMgr::s_pInstance;
        if (mgr->GetMyPlayerNo() == PLAYER_NO_HOST) {
            m_State = 6;
        } else {
            PacketQueue& queue = mgr->m_PacketQueue;
            queue.BeginPacket();
            queue.EndPacket(PACKET_RETIRE_FROM_ISLAND, PLAYER_NO_HOST);
        }
    }
    return true;
}

// 0x0061E9C8 (name is ours)
bool netgame::IslandMgr::EndSeaReturnDemo()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->GetMyPlayerNo() != PLAYER_NO_HOST) {
        PacketQueue& queue = mgr->m_PacketQueue;
        queue.BeginPacket();
        queue.EndPacket(PACKET_SEA_RETURN_DONE, PLAYER_NO_HOST);
    } else {
        m_Confirms[2].Add(GetMyPlayerNo());
    }
    return true;
}

DECOMP_ALWAYS_INLINE s32 netgame::IslandMgr::FindStayingPrincipal(u32 principalId) const
{
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        if (m_StayingPrincipals[i] == principalId) {
            return i;
        }
    }
    return -1;
}

// the principal ids of the players that stay in the session
DECOMP_ALWAYS_INLINE void netgame::IslandMgr::StoreStayingPrincipals()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    AvatarMgr& avatars = mgr->m_AvatarMgr;
    PlayerNo myNo = GetMyPlayerNo();
    if (myNo >= PLAYER_NO_MAX) {
        return;
    }
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        m_StayingPrincipals[i] = 0;
        if (!m_StayingPlayers.IsSet(no)) {
            continue;
        }
        if (no == myNo) {
            Principal principal;
            principal.SetFromMyself();
            m_StayingPrincipals[i] = principal.principalId;
        } else {
            const AvatarMgr::AvatarInfo* info = avatars.GetAvatarInfo(no);
            if (info != NULL) {
                m_StayingPrincipals[i] = info->principalId;
            }
        }
    }
}

// 0x0061EA30 (name after libgarden)
void netgame::IslandMgr::UpdatePlayerTourSplit(u8 depart, u8 stay)
{
    m_PlayersThatDepartForTour.Add(depart);
    m_PlayersThatDoNotDepartForTour.Add(stay);
    MachineBitTable& otherSide =
        m_PlayersThatDepartForTour.IsSet(PLAYER_NO_HOST) ? m_PlayersThatDoNotDepartForTour : m_PlayersThatDepartForTour;
    m_StayingPlayers.Add(otherSide.GetBit());
    m_StayingPlayerNum = m_StayingPlayers.GetCount();
    m_NewHostPrincipalId = 0;
    if (m_StayingPlayerNum == 0) {
        m_IsSelfDepartingForTour = true;
        m_IsNewHost = false;
        return;
    }
    PlayerNo myNo = GetMyPlayerNo();
    m_IsSelfDepartingForTour = m_PlayersThatDepartForTour.IsSet(myNo);
    PlayerNo newHost = PLAYER_NO_INVALID;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (otherSide.IsSet(no)) {
            newHost = no;
            break;
        }
    }
    m_IsNewHost = newHost == myNo;
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (m_IsNewHost) {
        m_NewHostPrincipalId = FriendList::s_pInstance->GetMyPrincipalId();
    } else {
        const AvatarMgr::AvatarInfo* info = mgr->m_AvatarMgr.GetAvatarInfo(newHost);
        if (info != NULL) {
            m_NewHostPrincipalId = info->principalId;
        }
    }
    StoreStayingPrincipals();
}

// 0x0061EBF4 (name after libgarden)
void netgame::IslandMgr::LeaveRandomMatch(LeaveRandomMatchMode mode)
{
    if (GetNetgameType() != TYPE_RANDOM_MATCH) {
        return;
    }
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    LockMgr& lock = mgr->m_LockMgr;
    PlayerNo leaving;
    if (mode != LEAVE_RANDOM_MATCH_MODE_RETIRE) {
        leaving = lock.m_SourcePlayer;
        m_LeaveKind = leaving == PLAYER_NO_HOST ? 1 : 2;
    } else {
        lock.m_SuccessfulCmd = LockMgr::CMD_RANDOM_MATCH_LEAVE;
        leaving = mgr->m_MyPlayerNo;
        lock.m_SourcePlayer = leaving;
        if (netgame::GetPlayerCount() <= 1) {
            m_LeaveKind = 0;
        } else {
            m_LeaveKind = IsHost() ? 1 : 2;
        }
    }
    PlayerNo myNo = mgr->GetMyPlayerNo();
    if (netgame::GetPlayerCount() <= 1) {
        m_LeavingPlayer = leaving;
        mgr->SendSessionClosed(true);
        m_State = 34;
        mgr->unk_0x132B6 = true;
        mgr->unk_0x132B7 = false;
        return;
    }
    if (mgr->GetMyPlayerNo() == PLAYER_NO_HOST && mode == LEAVE_RANDOM_MATCH_MODE_RETIRE) {
        m_LeavingPlayer = leaving;
        m_Confirms[1].Update(myNo, true);
        m_Confirms[5].Update(myNo, true);
        m_Confirms[6].Update(myNo, false);
        mgr->m_EntryData.SendToGuests(reinterpret_cast<const u16*>(&mgr->m_Bits[1]));
        mgr->SendSessionClosed(true);
        Func114828();
        mgr->unk_0x132B6 = true;
        mgr->unk_0x132B7 = false;
        m_State = 51;
        return;
    }
    if (myNo == leaving) {
        m_LeavingPlayer = leaving;
        m_Confirms[5].Update(myNo, true);
        Func114828();
        m_Confirms[2].Update(myNo, false);
        mgr->SendSessionClosed(true);
        m_State = 40;
        return;
    }
    if (m_LeaveKind == 1) {
        m_LeavingPlayer = leaving;
        m_Confirms[1].Update(myNo, false);
        m_Confirms[6].Update(myNo, false);
        m_Confirms[6].Add(leaving);
        m_StayingPlayers = mgr->m_Bits[1];
        m_StayingPlayers.Remove(PLAYER_NO_HOST);
        StoreStayingPrincipals();
        m_State = 56;
        return;
    }
    if (mgr->GetMyPlayerNo() == PLAYER_NO_HOST) {
        MachineBitTable table;
        table.Add(leaving);
        mgr->m_EntryData.SendToGuests(reinterpret_cast<const u16*>(&table));
        mgr->unk_0x132B6 = true;
        mgr->unk_0x132B7 = false;
    }
    m_LeavingPlayer = leaving;
    m_Confirms[5].Update(myNo, false);
    m_Confirms[6].Update(myNo, false);
    m_Confirms[6].Add(leaving);
    m_State = 44;
}

// 0x0061EF60 (name is ours)
void netgame::IslandMgr::OnPlayerGone()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    switch (m_State) {
    case 46: {
        PlayerNo leaving = m_LeavingPlayer;
        mgr->m_PacketQueue.m_LeavingPlayer = leaving;
        mgr->m_StageMgr.RemovePlayer(leaving);
        mgr->m_LeaveMgr.NotifyEventA(leaving);
        m_State = 47;
        break;
    }
    case 57:
        mgr->m_PacketQueue.m_IsClosed = true;
        m_State = 58;
        break;
    case 90: {
        PlayerNo leaving = m_LeavingPlayer;
        mgr->m_PacketQueue.m_LeavingPlayer = leaving;
        mgr->m_StageMgr.RemovePlayer(leaving);
        mgr->m_Bits[2].Remove(leaving);
        mgr->m_LeaveMgr.NotifyEventA(leaving);
        m_State = 91;
        break;
    }
    default:
        break;
    }
}

// 0x0061F018 (name after libgarden)
void netgame::IslandMgr::ConfirmLeaveSelf()
{
    m_Confirms[5].Update(NetGameMgr::s_pInstance->GetMyPlayerNo(), true);
    m_State = 37;
}

// 0x0061F04C (name is ours)
void netgame::IslandMgr::StartTourTransition()
{
    if (GetNetgameType() != TYPE_RANDOM_MATCH || m_IsSelfDepartingForTour) {
        Func283328();
        BsTourMgr::SetState(static_cast<BsTourMgr::State>(2));
        SvPlayer* player = SvGetCurrentPlayer();
        if (player != NULL) {
            u32 first = Func6BDA24(player);
            u32 second = Func3037DC(reinterpret_cast<u8*>(player) + 0x6B8C);
            Func6B9808(Func2FD00C(), second);
            Func6B7ED0(Func2FD00C(), first);
        }
        Func5C6688();
        Func1148DC();
        Func61E708();
    } else {
        Func283234();
        stage::GetTransitionMgr()->TransitionToReturnPoint(0, 0, 1, 1);
        if (IsHost()) {
            Reset();
        }
    }
    NetGameMgr::s_pInstance->unk_0x132B6 = false;
}

// 0x0061F168 (name is ours)
bool netgame::IslandMgr::RequestTourData(PlayerNo player)
{
    if (!CanSendToAll(player)) {
        return false;
    }
    unk_0x141 = true;
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        PlayerNo no = static_cast<PlayerNo>(i);
        if (no != PLAYER_NO_HOST && (ExistsPlayer(no) || player == no)) {
            mgr->SendPacketBatch(no, NULL, NULL, 0, 20, 0, 0, 0);
        }
    }
    return true;
}

// 0x0061F204 (name after libgarden)
bool StageTransitionTourResult()
{
    stage::Name stage = Func6C4734(Func6C4DD0(TourResult::s_pInstance->m_Tour));
    return stage::GetTransitionMgr()->Transition(stage, 1, 1, 1, 1);
}
// 0x0061F250 (name is ours)
void netgame::IslandMgr::StartIslandReturn()
{
    StartIslandReturn(false);
}

// the transition of all players back to the village; isLeaving: this console leaves the session
// 0x0061F258 (name is ours)
void netgame::IslandMgr::StartIslandReturn(bool isLeaving)
{
    // the start positions around the station
    // 0x00A22FB4 (guard 0x0095465C)
    static const nn::math::VEC3_ s_StartOffsets[PLAYER_NO_MAX] = {
        {-22.0f, 0.0f, 252.0f}, {82.0f, 0.0f, 182.0f}, {44.0f, 0.0f, 150.0f}, {-2.0f, 0.0f, 132.0f}};
    // 0x00A22FE4 (guard 0x00954658)
    static const nn::math::VEC3_ s_LookOffset = {-3.0f, 0.0f, 188.0f};

    nn::math::VEC3_ base;
    if (GetStructurePosition(&base, 92)) {
        nn::math::VEC3_ positions[PLAYER_NO_MAX];
        math::Angle16 rotations[PLAYER_NO_MAX];
        nn::math::VEC3_ target = {base.x + s_LookOffset.x, base.y + s_LookOffset.y, base.z + s_LookOffset.z};
        for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
            positions[i].x = base.x + s_StartOffsets[i].x;
            positions[i].y = base.y + s_StartOffsets[i].y;
            positions[i].z = base.z + s_StartOffsets[i].z;
            if (i == 0) {
                rotations[0] = Func6DB524();
            } else {
                rotations[i] = Func57C4F0(&positions[i], &target);
            }
        }
        if (GetNetgameType() == TYPE_RANDOM_MATCH || isLeaving) {
            math::Angle16Vec3 rotation = {0, rotations[0], 0};
            stage::GetTransitionMgr()->TransitionTo(stage::NAME_VILLAGE, &positions[0], &rotation, PLAYER_ACTION_191, 0, 0, 1, 1);
            Func6A7C4C();
            Func6A7620();
            if (isLeaving) {
                NetGameMgr::s_pInstance->m_IsConnectRandomMatch = false;
                return;
            }
        } else {
            stage::GetTransitionMgr()->TransitionWithStartPositions(stage::NAME_VILLAGE, PLAYER_ACTION_191, PLAYER_ACTION_3,
                                                                    PLAYER_ACTION_IDLE, PLAYER_ACTION_2, &positions[0], &rotations[0],
                                                                    &positions[1], &rotations[1], &positions[2], &rotations[2],
                                                                    &positions[3], &rotations[3], 0, 0, 1, 1);
            Func6A7C4C();
            Func6A7620();
        }
    } else {
        stage::GetTransitionMgr()->Transition(stage::NAME_VILLAGE, 0, 0, 1, 1);
        Func6A7C4C();
        Func6A7620();
        if (isLeaving) {
            NetGameMgr::s_pInstance->m_IsConnectRandomMatch = false;
            return;
        }
    }
    Func5C401C();
    Func5C69C4();
    NetGameMgr::s_pInstance->m_IsConnectRandomMatch = false;
}

// 0x0061F5F8 (name is ours)
void Func61F5F8()
{
    nn::math::VEC3_ position = {256.0f, 0.0f, 176.0f};
    stage::GetTransitionMgr()->ClearReturnPoint();
    if (GetNetgameType() == TYPE_RANDOM_MATCH) {
        nn::math::VEC3_ start = position;
        start.x += Func56671C(8.0f);
        math::Angle16Vec3 rotation = {0, 0, 0};
        stage::GetTransitionMgr()->TransitionTo(stage::NAME_TOUR_DESK, &start, &rotation, PLAYER_ACTION_EXIT_CONNECTION_WALK, 0, 0, 1, 1);
    } else {
        math::Angle16 rotation = {0};
        stage::GetTransitionMgr()->TransitionWithStartPositions(stage::NAME_TOUR_DESK, PLAYER_ACTION_EXIT_CONNECTION_WALK, PLAYER_ACTION_HIDE, PLAYER_ACTION_IDLE,
                                                                PLAYER_ACTION_2, &position, &rotation, &s_TourDeskPositions[1], &rotation,
                                                                &s_TourDeskPositions[2], &rotation, &s_TourDeskPositions[3], &rotation, 0, 0, 1, 1);
        Func5C4D04(&SvFgName::s_Empty);
        Func5C7050();
    }
    Func2D5DC0();
}

// 0x0061F760 (name after libgarden)
void netgame::IslandMgr::StartTourRandomMatch()
{
    PlayerNo myNo = NetGameMgr::s_pInstance->GetMyPlayerNo();
    unk_0x141 = false;
    m_Confirms[1].MachineBitTable::Reset();
    m_Confirms[3].MachineBitTable::Reset();
    m_Confirms[4].MachineBitTable::Reset();
    m_ChosenTrip = TOUR_DESK_SELECTION_NONE;
    if (IsHost()) {
        m_Confirms[1].Update(myNo, false);
        m_State = 69;
    } else {
        unk_0x145 = false;
        m_State = 74;
    }
    if (m_PlayersThatDepartForTour.IsSet(myNo)) {
        SoundMgr::PlaySound(SE_SYS_TOUR_SHIP_WHISTLE, 0.0f);
    }
}

// 0x0061F800 (name is ours)
bool netgame::IslandMgr::ContinueLeaving()
{
    if (m_State == 35) {
        m_State = 36;
    }
    return true;
}

// 0x0061F818 (name is ours)
void netgame::IslandMgr::TransitionToTourDesk()
{
    ToTourDesk();
}

// 0x0061F8E8 (name after libgarden)
bool netgame::IslandMgr::CancelTrip()
{
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->GetMyPlayerNo() == PLAYER_NO_HOST) {
        m_State = 12;
    } else {
        PacketQueue& queue = mgr->m_PacketQueue;
        queue.BeginPacket();
        queue.EndPacket(PACKET_TRIP_CANCEL_REQUEST, PLAYER_NO_HOST);
    }
    return true;
}

// 0x0061F938 (name is ours)
void netgame::IslandMgr::OnStationJoined()
{
    if (m_RetryTimer > 150) {
        m_RetryTimer = 150;
    }
}

// 0x0061F94C (name is ours)
void netgame::IslandMgr::AddPrincipal(PlayerNo player, const u32* principalId, bool countsAsEntered)
{
    if (GetNetgameType() != TYPE_RANDOM_MATCH) {
        return;
    }
    switch (m_State) {
    case 61:
    case 62:
    case 64:
    case 65:
    case 66:
    case 67:
    case 77:
    case 78:
    case 87:
    case 88:
        break;
    default:
        return;
    }
    if (player >= PLAYER_NO_MAX || *principalId == 0) {
        return;
    }
    s32 oldNo = FindStayingPrincipal(*principalId);
    if (oldNo < 0 || m_StayingPrincipals[oldNo] == 0) {
        return;
    }
    u8 saveNo = player != 0 ? player + 3 : 7;
    if (player == oldNo) {
        SetPlayerActive(player, saveNo, NULL);
    } else {
        s32 freeNo = FindStayingPrincipal(s_NoPrincipal);
        if (freeNo < 0) {
            return;
        }
        SvPlayer* newPlayer = SvGetPlayer(static_cast<PlayerSaveNumber>(GetDefaultSaveNo(oldNo)));
        if (newPlayer == NULL) {
            return;
        }
        u8 bit = 1 << player;
        u8 upperBit = 1 << (player + 4);
        if (m_StayingPrincipals[player] != 0) {
            SvPlayer* oldPlayer = Func3038EC(saveNo);
            if (oldPlayer != NULL && oldPlayer->playerInfo.IsValid()) {
                SetPlayerActive(freeNo, freeNo != 0 ? freeNo + 3 : 7, oldPlayer);
                m_StayingPrincipals[freeNo] = m_StayingPrincipals[player];
                MoveFlag(g_PlayerFlags, bit, 1 << freeNo);
                MoveFlag(g_PlayerFlags, upperBit, 1 << (freeNo + 4));
                Func283200(freeNo, (g_TourStatus.unk_0x4 & bit) != 0);
                Func5C660C(player, freeNo);
                Func5C7284(player, freeNo);
            }
        }
        SetPlayerActive(player, saveNo, newPlayer);
        m_StayingPrincipals[player] = *principalId;
        m_StayingPrincipals[oldNo] = 0;
        u8 oldBit = 1 << oldNo;
        u8 oldUpperBit = 1 << (oldNo + 4);
        MoveFlag(g_PlayerFlags, oldBit, bit);
        g_PlayerFlags |= oldBit;
        MoveFlag(g_PlayerFlags, oldUpperBit, upperBit);
        g_PlayerFlags &= ~oldUpperBit;
        Func283200(player, (g_TourStatus.unk_0x4 & oldBit) != 0);
        Func283200(oldNo, false);
        ClearPlayer(oldNo);
    }
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    Principal myself;
    myself.SetFromMyself();
    if (*principalId != myself.principalId) {
        if (countsAsEntered) {
            mgr->m_Bits[2].Add(player);
        }
        if (net::Net::s_Instance.IsHost()) {
            mgr->m_AvatarMgr.m_HasJoiner = false;
        }
    }
    mgr->m_Bits[0].Add(player);
    mgr->m_Bits[1].Add(player);
    Func5C7AE0(oldNo, player);
    Func5C664C(oldNo, player);
    LoadNpcInfo(&m_NpcInfos[oldNo], player);
}

// 0x0061FD78 (name after libgarden)
bool netgame::IslandMgr::RequestRandomMatchSeeOff()
{
    if (IsMultiPlayer() && GetNetgameType() == TYPE_RANDOM_MATCH && m_State == 0) {
        return NetGameMgr::s_pInstance->RequestLock6();
    }
    return false;
}

// 0x0061FDC0 (name after libgarden)
bool netgame::IslandMgr::ChooseTour(TourName tour)
{
    if (static_cast<u32>(tour) >= TOUR_NUM) {
        return false;
    }
    m_TourName = static_cast<u8>(tour);
    m_ChosenTrip = TOUR_DESK_SELECTION_TOUR;
    m_TripRequestPlayer = GetMyPlayerNo();
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->GetMyPlayerNo() == PLAYER_NO_HOST) {
        m_State = 8;
    } else {
        u16 param = tour & 0xFF;
        PacketQueue& queue = mgr->m_PacketQueue;
        queue.BeginPacket();
        queue.AddPacketData(&param, sizeof(param));
        queue.EndPacket(PACKET_TRIP_REQUEST, PLAYER_NO_HOST);
    }
    return true;
}

// 0x0061FE4C (name is ours)
bool netgame::IslandMgr::ChooseLeaveIsland()
{
    m_TourName = TOUR_NUM;
    m_ChosenTrip = TOUR_DESK_SELECTION_LEAVE_ISLAND;
    m_TripRequestPlayer = GetMyPlayerNo();
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    if (mgr->GetMyPlayerNo() == PLAYER_NO_HOST) {
        m_State = 8;
    } else {
        u16 param = TOUR_NUM | (TOUR_DESK_SELECTION_LEAVE_ISLAND << 8);
        PacketQueue& queue = mgr->m_PacketQueue;
        queue.BeginPacket();
        queue.AddPacketData(&param, sizeof(param));
        queue.EndPacket(PACKET_TRIP_REQUEST, PLAYER_NO_HOST);
    }
    return true;
}

// 0x0061FECC (name is ours)
void netgame::IslandMgr::AllocBuffers(sead::Heap* /* heap */)
{
    Reset();
}

// 0x0061FED0 (name is ours)
void netgame::IslandMgr::Reset()
{
    m_State = 0;
    m_Confirms[1].MachineBitTable::Reset();
    m_Confirms[2].MachineBitTable::Reset();
    m_Confirms[3].MachineBitTable::Reset();
    m_Confirms[4].MachineBitTable::Reset();
    m_TourName = 0xFF;
    m_LeavingPlayer = PLAYER_NO_INVALID;
    m_TripRequestPlayer = PLAYER_NO_INVALID;
    m_PlayersThatDepartForTour.Reset();
    m_PlayersThatDoNotDepartForTour.Reset();
    m_Confirms[0].MachineBitTable::Reset();
    m_Confirms[5].MachineBitTable::Reset();
    m_Confirms[6].MachineBitTable::Reset();
    m_LeaveKind = 3;
    unk_0x140 = false;
    unk_0x141 = false;
    unk_0x142 = false;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        m_StayingPrincipals[i] = 0;
    }
    m_StayingPlayers.Reset();
    m_ShowingTelop = false;
    m_Timer = 0;
    m_RetryTimer = 0;
    m_RetryCount = 0;
    m_NewHostPrincipalId = 0;
    unk_0x13A = 0;
    m_IsNewHost = false;
    unk_0x13E = false;
    m_IsRejoining = false;
    unk_0x147 = false;
}

// 0x0061FF98 (name is ours)
netgame::IslandMgr::IslandMgr()
{
    m_State = 0;
    m_ChosenTrip = TOUR_DESK_SELECTION_NONE;
    m_TourName = 0xFF;
    m_LeavingPlayer = PLAYER_NO_INVALID;
    m_TripRequestPlayer = PLAYER_NO_INVALID;
    m_LeaveKind = 3;
    m_RandomMatchParam = 0;
    m_Timer = 0;
    m_RetryTimer = 0;
    m_Timestamp = 0;
    m_RetryCount = 0;
    m_NewHostPrincipalId = 0;
    unk_0x13A = 0;
    m_StayingPlayerNum = 0;
    m_IsNewHost = false;
    unk_0x13E = false;
    m_IsRejoining = false;
    unk_0x140 = false;
    unk_0x141 = false;
    unk_0x142 = false;
    unk_0x143 = false;
    m_IsSelfDepartingForTour = false;
    unk_0x145 = false;
    m_ShowingTelop = false;
    unk_0x147 = false;
    m_ResetsClock = false;
}

// 0x006200B8 (name is ours)
netgame::IslandMgr::~IslandMgr()
{
    // (only the members are destroyed)
}
} // namespace netgame
