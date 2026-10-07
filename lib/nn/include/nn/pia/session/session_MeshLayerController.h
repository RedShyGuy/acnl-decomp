#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_Crypto.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/transport/transport_Station.h"

namespace nn {
namespace pia {
namespace session {
class CommonMatchmakeSession;
class JointSessionJob;

// RTTI N2nn3pia7session19MeshLayerControllerE @ 0x008D0044
//
// Starts the mesh of a matchmake session for Session (inet::NexMatchMeshLayerController,
// local::LocalMatchMeshLayerController): Startup keeps the setting, StartupMesh starts the mesh
// with it. Abstract (no vtable of its own in the binary); the constructor and the destructor
// are empty. Layout from Startup; the member names and the slot names are ours.
class MeshLayerController : public ::nn::pia::common::RootObject
{
public:
    MeshLayerController() {} // (inline)
    virtual ~MeshLayerController() {} // slot 0x00 (inline)
    // slot 0x04 (deleting dtor)
    // keeps the setting of Mesh::Startup (Session::Startup)
    virtual nn::Result Startup(bool isHostMigrationEnabled, const u8* pIdentificationData, nn::pia::common::Crypto::Mode cryptoMode,
                               u32 timeoutMSec, u32 keepAliveIntervalMSec, s32 bandwidthCheckBandwidth, u32 bandwidthCheckPacketSize,
                               bool isBandwidthCheckOneWay, s32 bandwidthCheckDurationMSec,
                               const nn::pia::transport::Station::PlayerName* pPlayerName, bool value); // 0x00439FB8 slot 0x08
    virtual void Cleanup() = 0; // slot 0x0C
    // starts the mesh with the setting and the signature of the session (the flag overrides the
    // one-way setting of the bandwidth check; the base ignores it)
    virtual nn::Result StartupMesh(nn::pia::session::CommonMatchmakeSession* pSession, bool isBandwidthCheckOneWay); // 0x00439D20 slot 0x10
    virtual void vf_0x14() = 0; // slot 0x14
    // the state of the network: 0 / 1 fine, 2 and 3 the disconnect state of Session (3: gone;
    // name is ours)
    virtual u8 GetNetworkStatus() = 0; // slot 0x18
    virtual void vf_0x1C() = 0; // slot 0x1C
    // the session id of Session's other / current matchmake session
    virtual void SetOtherSessionId(u32 sessionId); // 0x00439F28 slot 0x20
    virtual void SetCurrentSessionId(u32 sessionId); // 0x00439F58 slot 0x24
    virtual u32 vf_0x28() = 0; // slot 0x28
    virtual nn::pia::session::JointSessionJob* GetJointSessionJob(); // 0x00439F7C slot 0x2C
    // whether the network has a session other than the two (name is ours)
    virtual bool HasOtherSession(u32 sessionId, u32 otherSessionId); // 0x00439F94 slot 0x30
    virtual bool vf_0x34() const; // 0x00733920 slot 0x34
    virtual void vf_0x38(); // 0x00439F24 slot 0x38

    // the work of StartupMesh; isBandwidthCheckOneWay overrides the setting if true
    nn::Result StartupMeshCore(nn::pia::session::CommonMatchmakeSession* pSession, bool isBandwidthCheckOneWay); // 0x00439D28
    // the mesh and its callbacks
    static void CleanupMesh(); // 0x00439CDC
    void ClearUnknown0x69(); // 0x00439F9C

    static const u32 IDENTIFICATION_DATA_SIZE = 36;

    // 32 bytes (16 UTF-16 characters?), the length at 34 and one more byte at 35 (see Mesh::Startup)
    u8 m_IdentificationData[IDENTIFICATION_DATA_SIZE]; // 0x04
    bool m_IsIdentificationDataSet;                   // 0x28
    u32 m_TimeoutMSec;                                // 0x2C
    u32 m_KeepAliveIntervalMSec;                      // 0x30
    bool m_IsHostMigrationEnabled;                    // 0x34
    s32 m_BandwidthCheckBandwidth;                    // 0x38
    u32 m_BandwidthCheckPacketSize;                   // 0x3C
    bool m_IsBandwidthCheckOneWay;                    // 0x40
    s32 m_BandwidthCheckDurationMSec;                 // 0x44
    nn::pia::common::Crypto::Mode m_CryptoMode;       // 0x48
    transport::Station::PlayerName m_PlayerName;      // 0x49
    bool m_Unknown0x69;                               // 0x69
};
ASSERT_OFFSET(MeshLayerController, m_TimeoutMSec, 0x2C);
ASSERT_OFFSET(MeshLayerController, m_BandwidthCheckDurationMSec, 0x44);
ASSERT_OFFSET(MeshLayerController, m_PlayerName, 0x49);
ASSERT_OFFSET(MeshLayerController, m_Unknown0x69, 0x69);
ASSERT_SIZE(MeshLayerController, 0x6C);
} // namespace session
} // namespace pia
} // namespace nn
