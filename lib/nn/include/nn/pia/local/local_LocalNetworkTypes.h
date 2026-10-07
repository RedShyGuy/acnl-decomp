#pragma once

// The arguments of the network requests of pia local (LocalNetwork and its jobs) and the update
// event of the network. The type names are from the symbols; the layouts are from the constructor of
// UdsBackgroundProcessJob (which keeps a copy of each) and from the calls of the uds API, the member
// names and the file name are ours.

#include "decomp.h"
#include <cstring>

namespace nn {
namespace pia {
namespace local {
class LocalNetworkDescription;

// the reason of a call of the update event callback (LocalNetwork::RegisterUpdateEventCallback)
enum LocalUpdateEvent
{
    LOCAL_UPDATE_EVENT_DISCONNECTED = 1,      // a station left (its node id)
    LOCAL_UPDATE_EVENT_MIGRATION_STARTED = 2, // the host left, the host migration starts
};

typedef void (*LocalUpdateEventCallback)(LocalUpdateEvent event, u8 nodeId, void* pArg);

// a new network (uds::CTR::CreateNetwork)
struct LocalCreateNetworkSetting
{
    static const u32 PASSPHRASE_SIZE_MIN = 8;
    static const u32 PASSPHRASE_SIZE_MAX = 255;
    static const u32 APPLICATION_DATA_SIZE_MAX = 200;

    LocalCreateNetworkSetting() : m_SubId(0), m_NodeCountMax(0), m_LocalCommunicationId(0), m_PassphraseSize(0), m_Channel(0), m_ApplicationDataSize(0)
    {
        std::memset(m_Passphrase, 0, sizeof(m_Passphrase));
        std::memset(m_ApplicationData, 0, sizeof(m_ApplicationData));
    }

    u8 m_SubId;                                      // 0x000, at most 254
    u8 m_NodeCountMax;                               // 0x001
    u32 m_LocalCommunicationId;                      // 0x004
    char m_Passphrase[PASSPHRASE_SIZE_MAX];          // 0x008
    u32 m_PassphraseSize;                            // 0x108, 8 to 255
    u8 m_Channel;                                    // 0x10C, 0 (any), 1, 6 or 11
    u8 m_ApplicationData[APPLICATION_DATA_SIZE_MAX]; // 0x10D, of the beacon
    u32 m_ApplicationDataSize;                       // 0x1D8
};
ASSERT_OFFSET(LocalCreateNetworkSetting, m_PassphraseSize, 0x108);
ASSERT_OFFSET(LocalCreateNetworkSetting, m_ApplicationDataSize, 0x1D8);
ASSERT_SIZE(LocalCreateNetworkSetting, 0x1DC);

// a scan for networks; the found networks go to the descriptions (LocalNetwork's)
struct LocalScanNetworkSetting
{
    LocalScanNetworkSetting() : m_pDescriptions(nullptr), m_pDescriptionNum(nullptr), m_SubId(0), m_LocalCommunicationId(0), m_pBuffer(nullptr), m_BufferSize(0) {}

    LocalNetworkDescription* m_pDescriptions; // 0x00
    u32* m_pDescriptionNum;                   // 0x04
    u8 m_SubId;                               // 0x08
    u32 m_LocalCommunicationId;               // 0x0C
    void* m_pBuffer;                          // 0x10, for the beacons
    u32 m_BufferSize;                         // 0x14
};
ASSERT_SIZE(LocalScanNetworkSetting, 0x18);

// the connection to a found network
struct LocalConnectNetworkSetting
{
    static const u32 PASSPHRASE_SIZE_MAX = 255;

    LocalConnectNetworkSetting() : m_pDescription(nullptr), m_PassphraseSize(0) { std::memset(m_Passphrase, 0, sizeof(m_Passphrase)); }

    const LocalNetworkDescription* m_pDescription; // 0x000
    char m_Passphrase[PASSPHRASE_SIZE_MAX];        // 0x004
    u8 m_PassphraseSize;                           // 0x103, 8 to 255
};
ASSERT_SIZE(LocalConnectNetworkSetting, 0x104);

// a node of the network (LocalNetwork::GetStationInfo; copied from uds::CTR::NodeInformation)
struct LocalStationInfo
{
    enum Role : u8
    {
        ROLE_NONE = 0,
        ROLE_HOST = 1,
        ROLE_CLIENT = 2,
    };

    LocalStationInfo() : m_Role(ROLE_NONE) {} // 0x00416948

    u8 m_Role;                         // 0x00
    u8 m_Padding0;                     // 0x01
    u8 m_ScrambledLocalFriendCode[12]; // 0x02, unaligned
    u8 m_Padding[2];                   // 0x0E
    u8 m_UserName[24];                 // 0x10, nn::cfg::CTR::UserName
};
ASSERT_SIZE(LocalStationInfo, 0x28);

// the system data of pia in the beacon (in front of the application data; LocalNetworkManager keeps
// the one of the local network). With the host migration the beacon carries more (28 bytes, see
// LocalMigrationManager). The member names are ours.
struct LocalBeaconSystemData
{
    static const u8 VERSION = 1;

    LocalBeaconSystemData() : m_Unknown0x0(0), m_SessionId(0), m_Version(VERSION), m_ApplicationVersion(0) { std::memset(m_Reserved, 0, sizeof(m_Reserved)); }

    u32 m_Unknown0x0;     // 0x00 (LocalNetworkManager 0x1238)
    u32 m_SessionId;      // 0x04 (LocalNetworkManager::CreateSessionId)
    u8 m_Version;         // 0x08
    u8 m_ApplicationVersion; // 0x09
    u8 m_Reserved[6];     // 0x0A
};
ASSERT_SIZE(LocalBeaconSystemData, 0x10);

// the search of the networks around (LocalAroundNetworkSearchManager; the type name is from the
// symbols, the member names are ours)
struct LocalAroundNetworkSearchSetting
{
    LocalAroundNetworkSearchSetting() : m_LocalCommunicationId(0), m_SubId(0xFF), m_SearchIntervalMsec(200), m_ScanTime(20), m_LifeTimeMsec(5000) {}

    u32 m_LocalCommunicationId; // 0x00, 0: any
    u8 m_SubId;                 // 0x04, 0xFF: any
    u32 m_SearchIntervalMsec;   // 0x08, between two scans (plus 0, 15 or 30)
    u16 m_ScanTime;             // 0x0C, of uds::CTR::ScanOnConnection
    u32 m_LifeTimeMsec;         // 0x10, of a found network
};
ASSERT_SIZE(LocalAroundNetworkSearchSetting, 0x14);
} // namespace local
} // namespace pia
} // namespace nn
