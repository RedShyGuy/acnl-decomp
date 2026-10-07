#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_FixedObjList.h"
#include "nn/pia/common/common_Job.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/session/session_MeshLayerController.h"

namespace nn {
namespace nex {
class NgsBridgeInterface;
} // namespace nex
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet27NexMatchMeshLayerControllerE @ 0x008CFA44
// vtable 0x009006BC (vptr 0x009006C4), offset_to_top 0, 15 entries
//
// The mesh layer of inet: the nex bridge of the game, the NAT session of NexFacade and the
// notifications of the server (NexNotificationEventHandler keeps the participants of the matchmake
// sessions). The layout is from the constructor; the member and the function names are ours.
class NexMatchMeshLayerController : public ::nn::pia::session::MeshLayerController
{
public:
    class NexNotificationEventHandler;

    // the participants of the sessions in the notifications (NexNotificationEventHandler; here so
    // that FindSessionEntry can return them)
    static const u32 PARTICIPATION_JOINED = 1;
    static const u32 PARTICIPATION_7 = 7;

    static const u32 PARTICIPANT_NUM_MAX = 12;
    // older events than this of the left participants are dropped
    static const s32 PARTICIPANT_TIMEOUT_MSEC = 200;

    // the participation events of one principal: +1 per join, -1 per leave
    struct Participant
    {
        u32 m_PrincipalId; // 0x0
        s16 m_Count;       // 0x4
        u16 m_Type;        // 0x6, the subtype of the last event
    };

    // the participants of one session
    struct SessionEntry
    {
        typedef common::FixedObjList<Participant, PARTICIPANT_NUM_MAX> List;

        // (out of line; armlink placed them in front of NatProbeData)
        DECOMP_NOINLINE SessionEntry(); // 0x003E434C
        DECOMP_NOINLINE ~SessionEntry(); // 0x003E4448

        void Clear()
        {
            m_SessionId = 0;
            m_List.ClearNodes();
            m_Time.SetNow();
        }
        // the events of left principals are dropped if the last event is too old
        void DropOldEvents()
        {
            common::Time now;
            now.SetNow();
            if (static_cast<s32>((now - m_Time).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick()) >= PARTICIPANT_TIMEOUT_MSEC) {
                for (List::Node* node = m_List.Begin(); node != m_List.End();) {
                    List::Node* next = List::Advance(node);
                    if (node->m_Value.m_Count < 0 && node->m_Value.m_Type == PARTICIPATION_7) {
                        m_List.Erase(&node->m_Value);
                    }
                    node = next;
                }
            }
            m_Time.SetNow();
        }

        u32 m_SessionId;  // 0x00
        List m_List;      // 0x04
        common::Time m_Time; // 0xF0, of the last event
    };

    NexMatchMeshLayerController(); // 0x00410F40
    virtual ~NexMatchMeshLayerController(); // 0x00411074 slot 0x00
    // 0x00411028 slot 0x04 (deleting dtor)
    virtual nn::Result Startup(bool isHostMigrationEnabled, const u8* pIdentificationData, nn::pia::common::Crypto::Mode cryptoMode,
                               u32 timeoutMSec, u32 keepAliveIntervalMSec, s32 bandwidthCheckBandwidth, u32 bandwidthCheckPacketSize,
                               bool isBandwidthCheckOneWay, s32 bandwidthCheckDurationMSec,
                               const nn::pia::transport::Station::PlayerName* pPlayerName, bool value); // 0x00410E28 slot 0x08
    virtual void Cleanup(); // 0x00410CE8 slot 0x0C
    virtual nn::Result StartupMesh(nn::pia::session::CommonMatchmakeSession* pSession, bool isBandwidthCheckOneWay); // 0x0040FC0C slot 0x10
    // the mesh and the NAT session end
    virtual void vf_0x14(); // 0x0040FBD8 slot 0x14
    // 3 without the connection of the game, 2 if a session was deleted, else 1
    virtual u8 GetNetworkStatus(); // 0x0040FE7C slot 0x18
    // (empty)
    virtual void vf_0x1C(); // 0x0072FA5C slot 0x1C
    // the number of the joined participants of the current session (from the notifications)
    virtual u32 vf_0x28(); // 0x0072F980 slot 0x28
    virtual bool HasOtherSession(u32 sessionId, u32 otherSessionId); // 0x0040FEE4 slot 0x30
    // NexNatTraversalProtocol gets a flag
    virtual void vf_0x38(); // 0x0040FC18 slot 0x38

    // whether the notifications have an entry of the session
    bool HasSessionEntry(u32 sessionId) const; // 0x0040FC38
    void RemoveSessionEntry(u32 sessionId); // 0x0040FC78
    // the NAT session of NexFacade with the matchmaking client of the current session (it does
    // not use the controller)
    nn::Result StartNatSession(); // 0x0040FC8C
    void ResetSessionEntries(); // 0x0040FD28
    // CONTINUE when the NAT session started, SUCCESS when it failed (pResult), else NEXT_DISPATCH
    common::ExecuteResult::State WaitStartNatSession(nn::Result* pResult); // 0x0040FDDC
    nn::Result CancelStartNatSession(); // 0x0040FECC
    // (armlink placed them at the end of the code)
    u32 GetSessionEntryNum() const; // 0x0072F970
    u32 GetSessionId(u32 index) const; // 0x0072F860
    bool HasParticipant(u32 sessionId, u32 principalId) const; // 0x0072F8D4
    // the entry of the session in the notifications, null without one
    SessionEntry* FindSessionEntry(u32 sessionId) const; // 0x0072F884

    nn::nex::NgsBridgeInterface* m_pNgsBridge;                 // 0x6C, of NexFacade
    NexNotificationEventHandler* m_pNotificationEventHandler;  // 0x70
};
ASSERT_OFFSET(NexMatchMeshLayerController, m_pNgsBridge, 0x6C);
ASSERT_SIZE(NexMatchMeshLayerController, 0x74);
} // namespace inet
} // namespace pia
} // namespace nn
