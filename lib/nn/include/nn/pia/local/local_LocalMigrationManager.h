#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/local/local_LocalNetworkTypes.h"

namespace nn {
namespace pia {
namespace local {
class LocalHostMigrationJob;
class LocalNetworkDescription;

// RTTI N2nn3pia5local21LocalMigrationManagerE @ 0x008CFC10
// vtable 0x00900EFC (vptr 0x00900F04), offset_to_top 0, 11 entries
//
// The host migration of the local network: it keeps the nodes of the network (by transport id,
// with a key that identifies the station in the next network) and the network that the next host
// creates, and runs LocalHostMigrationJob when the host left. UdsMigrationManagerNew implements it
// with uds. The layout is from the constructor, Initialize and SetupParams; the member names and
// the names of the unnamed functions are ours.
class LocalMigrationManager : public ::nn::pia::common::RootObject
{
public:
    static const u32 NODE_NUM_MAX = 12;

    // m_State
    enum MigrationState : u8
    {
        MIGRATION_STATE_NONE = 0,
        MIGRATION_STATE_HOST_LEAVING = 1, // the host leaves and starts the migration (LocalDestroyNetworkJob)
        MIGRATION_STATE_NEW_HOST = 2,     // the station becomes the new host
        MIGRATION_STATE_CLIENT = 3,       // the station connects to the new host
    };

    // m_MigrationResult, how the last host migration ended
    enum MigrationResult : u8
    {
        MIGRATION_RESULT_NONE = 0,
        MIGRATION_RESULT_ALL_CONNECTED = 1,   // the new host: all stations came
        MIGRATION_RESULT_SOME_CONNECTED = 2,  // the new host: some stations did not come
        MIGRATION_RESULT_CONNECTED = 3,       // a client: connected to the new host
        MIGRATION_RESULT_FAILED = 4,
    };

    // a node of the network (by transport id - 1)
    struct NodeInfo
    {
        enum State : u8
        {
            STATE_NONE = 0,
            STATE_MIGRATING = 1, // not in the new network yet
            STATE_CONNECTED = 2,
        };

        NodeInfo() : m_State(STATE_NONE), m_TransportId(0), m_NodeId(0), m_Key(0) {}

        u8 m_State;       // 0x0
        u8 m_TransportId; // 0x1
        u16 m_NodeId;     // 0x2
        u64 m_Key;        // 0x8, of the station (GetLocalNodeKey)
    };

    LocalMigrationManager(); // 0x0041D478 | fefates:bytes [tier B]
    virtual ~LocalMigrationManager(); // 0x0041D524 slot 0x00 | fefates:bytes
    // 0x0041D4E8 slot 0x04 (deleting dtor)
    // the sizes of the buffers (m_SystemDataSize ... m_InvalidNodeKey)
    virtual void SetupParams() = 0; // slot 0x08
    // the network of the host is the base of the next network
    virtual nn::Result SetNetworkInfo(const nn::pia::local::LocalConnectNetworkSetting& setting) = 0; // slot 0x0C
    // the network is the one of the new host
    virtual bool IsNextNetwork(const nn::pia::local::LocalNetworkDescription* pDescription) = 0; // slot 0x10
    virtual nn::pia::local::LocalCreateNetworkSetting* GetCreateNetworkSetting() = 0; // slot 0x14
    virtual nn::pia::local::LocalConnectNetworkSetting* GetConnectNetworkSetting(nn::pia::local::LocalNetworkDescription* pDescription) = 0; // slot 0x18
    virtual u8 GetSubId() const = 0; // slot 0x1C
    virtual u32 GetLocalCommunicationId() const = 0; // slot 0x20
    virtual u64 GetLocalNodeKey(u16 nodeId) const = 0; // slot 0x24
    virtual void SetSystemDataToBeacon(void* pBeacon) = 0; // slot 0x28

    nn::Result Initialize(); // 0x0041CBE4 | fefates:bytes [tier B]
    void ClearNodeInfo(u8 transportId); // 0x0041CD78 | fefates:bytes [tier B]
    void ClearNodeInfo(); // 0x0041CDF8 | fefates:bytes [tier B]
    // the node of the transport id; a node without a key is not kept (name is ours)
    bool SetNodeInfo(u8 transportId, u16 nodeId, u64 key); // 0x0041CE8C
    nn::Result StartHostMigration(); // 0x0041CF78
    nn::Result CancelHostMigration(); // 0x0041D198 | fefates:bytes [tier B]
    void SetTransportIdToNodeIdTable(u8 transportId, u16 nodeId); // 0x0041D1AC | fefates:bytes [tier B]
    void ClearTransportIdToNodeIdTable(u8 transportId); // 0x0041D1D0 | fefates:bytes [tier B]
    void ClearTransportIdToNodeIdTable(); // 0x0041D208 | fefates:bytes [tier B]
    void Cleanup(); // 0x0041D258 | fefates:bytes [tier B]
    nn::Result Startup(); // 0x0041D2A8 | fefates:bytes [tier B]
    void Finalize(); // 0x0041D3A0 | fefates:bytes [tier B]
    u8 GetMigrationState(u8 transportId) const; // 0x00731238 | fefates:bytes [tier B]
    bool IsExistStateMigrating() const; // 0x00731264 | fefates:bytes [tier B]
    // the transport id of the station with the key; else the first one without a node (name is
    // ours)
    u8 GetTransportIdForNodeKey(u64 key, u32 num, bool* pIsFound) const; // 0x007312C0
    void RemoveBeaconSystemData(void* pBuffer, const void* pBeacon, u32 size) const; // 0x007312A8 | fefates:bytes [tier B]
    u8 ConvertLocalNodeIdToTransportId(u16 nodeId) const; // 0x007313D4 | fefates:bytes [tier B]
    u16 ConvertTransportIdToLocalNodeId(u8 transportId) const; // 0x00731444 | fefates:bytes [tier B]
    u8 GetNextHostCandidateTransportId() const; // 0x0073147C | fefates:bytes [tier B]

    NodeInfo* m_pNodeInfos;                       // 0x04, NODE_NUM_MAX
    u16* m_pNodeIds;                              // 0x08, NODE_NUM_MAX, by transport id - 1
    nn::pia::local::LocalHostMigrationJob* m_pHostMigrationJob; // 0x0C
    u8 m_State;                                   // 0x10, MigrationState
    u8 m_MigrationResult;                         // 0x11
    common::CallContext* m_pCallContext;          // 0x14, of the host migration job
    nn::pia::local::LocalNetworkDescription* m_pNextNetworkDescription; // 0x18
    u8* m_pBeacon;                                // 0x1C, system and application data
    u32 m_ApplicationDataSize;                    // 0x20
    u8* m_pPassphrase;                            // 0x24
    u32 m_PassphraseSize;                         // 0x28
    nn::pia::local::LocalCreateNetworkSetting* m_pCreateNetworkSetting;   // 0x2C
    nn::pia::local::LocalConnectNetworkSetting* m_pConnectNetworkSetting; // 0x30
    bool m_IsHostLeaving;                         // 0x34
    u32 m_SystemDataSize;                         // 0x38
    u32 m_ApplicationDataSizeMax;                 // 0x3C
    u32 m_PassphraseSizeMin;                      // 0x40
    u32 m_PassphraseSizeMax;                      // 0x44
    u32 m_InvalidNodeKey;                         // 0x48
};
ASSERT_SIZE(LocalMigrationManager::NodeInfo, 0x10);
ASSERT_OFFSET(LocalMigrationManager, m_pCallContext, 0x14);
ASSERT_OFFSET(LocalMigrationManager, m_IsHostLeaving, 0x34);
ASSERT_SIZE(LocalMigrationManager, 0x4C);
} // namespace local
} // namespace pia
} // namespace nn
