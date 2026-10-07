#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_CriticalSection.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/local/local_LocalNetworkTypes.h"

namespace nn {
namespace pia {
namespace local {
class LocalAroundNetworkSearchBackgroundJob;
class LocalAroundNetworkSearchJob;

// a network around the network of the station (the type name is from the symbols; the
// implementations add the description and the stations of the network, see
// UdsAroundNetworkSearchManager)
struct LocalAroundNetworkInfo
{
    u32 m_Unknown0x0; // 0x0 (not set)
};

// RTTI N2nn3pia5local31LocalAroundNetworkSearchManagerE @ 0x008CFD9C
// vtable 0x009013B8 (vptr 0x009013C0), offset_to_top 0, 10 entries
//
// The search of the networks around (a scan in the background while connected): the host starts and
// stops it with a command message to the clients, the stations send what they found to all others
// as status messages, and every message is answered. The messages of the search go through two own
// queues (LocalNetworkManager hands them over). The layout is from the constructor; the member
// names and the names of the unnamed functions are ours.
class LocalAroundNetworkSearchManager : public ::nn::pia::common::RootObject
{
public:
    class LocalAroundNetworkSearchCommandAckMessage;
    class LocalAroundNetworkSearchCommandMessage;
    class LocalAroundNetworkStatusAckMessage;
    class LocalAroundNetworkStatusMessage;

    static const u32 AROUND_NETWORK_STATUS_NUM = 16;
    static const u32 MESSAGE_NUM_MAX = 12;
    static const u32 MESSAGE_DATA_SIZE_MAX = 800;

    // a network the stations found
    struct AroundNetworkStatus
    {
        AroundNetworkStatus(); // 0x00423B28

        nn::pia::local::LocalAroundNetworkInfo* m_pInfo; // 0x00
        u32 m_Version;                                   // 0x04, of the status messages
        u32 m_DestinationBitmap;                         // 0x08, the stations that did not answer
        u32 m_LifeTime;                                  // 0x0C, milliseconds; 0: free
        u32 m_Key;                                       // 0x10, of the station that found it
    };

    // a message of the search in the queues
    struct Message
    {
        u8 m_Data[MESSAGE_DATA_SIZE_MAX]; // 0x000
        u32 m_Size;                       // 0x320
        u8 m_Type;                        // 0x324
        u16 m_NodeId;                     // 0x326, source or destination
    };

    LocalAroundNetworkSearchManager(); // 0x004248F4
    // (the destructor of UdsAroundNetworkSearchManager is a nop that falls into it)
    virtual ~LocalAroundNetworkSearchManager(); // 0x004249C0 slot 0x00 | fefates:bytes
    // 0x0042499C slot 0x04 (deleting dtor)
    virtual nn::Result Initialize(); // 0x00423984 slot 0x08 | fefates:bytes
    virtual void Finalize(); // 0x004247EC slot 0x0C | fefates:bytes
    virtual nn::pia::local::LocalAroundNetworkInfo* CreateLocalAroundNetworkInfo() = 0; // slot 0x10
    virtual nn::pia::local::LocalAroundNetworkSearchBackgroundJob* CreateLocalAroundNetworkSearchBackgroundJob() = 0; // slot 0x14
    // the found networks into the buffer (name is ours)
    virtual nn::Result GetAroundNetworkInfoList(void* pBuffer, u32* pNum, u32 bufferNum) = 0; // slot 0x18
    virtual void SerializeAroundNetworkStatus(nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkStatusMessage* pMessage, const nn::pia::local::LocalAroundNetworkSearchManager::AroundNetworkStatus* pStatus) const = 0; // slot 0x1C
    virtual void DeserializeAroundNetworkStatus(const nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkStatusMessage* pMessage) = 0; // slot 0x20
    virtual void vf_0x24(); // 0x007317F0 slot 0x24

    void EndSendMessage(u8 transportId); // 0x00423A40 | fefates:bytes [tier B]
    void EndSendMessage(); // 0x00423AC0 | fefates:bytes [tier B]
    // a new command version starts with the next command (name is ours)
    void ResetCommandVersion(); // 0x00423B1C
    void StartSendCommandMessage(u8 transportId); // 0x00423B44 | fefates:bytes [tier B]
    void PrepareNextCommandStatus(); // 0x00423B5C | fefates:bytes [tier B]
    void SendAroundNetworkStatusMessage(); // 0x00423B90 | fefates:bytes-fuzzy [tier B]
    void ParseAroundNetworkStatusMessage(const Message* pMessage); // 0x00423CF0
    void ParseAroundNetworkStatusAckMessage(const Message* pMessage); // 0x00423E8C
    void SendStopAroundNetworkSearchMessage(); // 0x00423F9C
    void SendStartAroundNetworkSearchMessage(); // 0x00424040
    // (symbols.json: with a bool more; the type of the message says if it starts the search)
    void ParseAroundNetworkSearchCommandMessage(const Message* pMessage); // 0x00424130
    // (inline in ParseMessages)
    void ParseAroundNetworkSearchCommandAckMessage(const Message* pMessage);
    // a message into the send / receive queue (names are ours)
    nn::Result PushSendMessage(const void* pData, u8 type, u32 size, u16 nodeId); // 0x004242EC
    void PushReceiveMessage(const void* pData, u8 type, u32 size, u16 nodeId); // 0x004243CC
    // the copied send queue goes out (name is ours)
    void SendMessages(); // 0x0042444C
    // the messages of the receive queue (symbols.json: ParseAroundNetworkSearchCommandAckMessage,
    // which is inline in it; the name is ours)
    void ParseMessages(); // 0x004244CC
    // the send queue is copied for SendMessages (name is ours)
    void CopySendQueue(); // 0x0042465C
    void Cleanup(); // 0x004246D0 (name is ours)
    nn::Result Startup(); // 0x00424750
    // the status of the index, null without one (inline everywhere; name is ours)
    AroundNetworkStatus* GetAroundNetworkStatus(u32 index) { return index < AROUND_NETWORK_STATUS_NUM ? &m_Statuses[index] : nullptr; }
    u32 GetMessageDestBitmap() const; // 0x00731770 | fefates:bytes [tier B]
    bool IsSendingStatusMessage() const; // 0x00731788 | fefates:bytes [tier B]
    bool IsSendingCommandMessage() const; // 0x007317E0

    nn::pia::local::LocalAroundNetworkSearchSetting m_Setting; // 0x0004
    nn::pia::local::LocalAroundNetworkSearchJob* m_pSearchJob; // 0x0018
    nn::pia::local::LocalAroundNetworkSearchBackgroundJob* m_pBackgroundJob; // 0x001C
    common::CallContext* m_pCallContext;  // 0x0020, of the search job
    bool m_IsSearching;                   // 0x0024, as the host commands it
    u32 m_Unknown0x28;                    // 0x0028
    u32 m_CommandVersion;                 // 0x002C
    u32 m_CommandDestinationBitmap;       // 0x0030, the stations that did not answer the command
    AroundNetworkStatus m_Statuses[AROUND_NETWORK_STATUS_NUM]; // 0x0034
    common::CriticalSection m_CriticalSection; // 0x0174
    u32 m_StatusVersion;                  // 0x0180, the last version of a status
    Message m_SendQueue[MESSAGE_NUM_MAX]; // 0x0184
    Message m_SendQueueCopy[MESSAGE_NUM_MAX]; // 0x2764
    u32 m_SendQueueNum;                   // 0x4D44
    u32 m_SendQueueCopyNum;               // 0x4D48
    bool m_IsSendQueueUpdated;            // 0x4D4C
    Message m_ReceiveQueue[MESSAGE_NUM_MAX]; // 0x4D50
    u32 m_ReceiveQueueNum;                // 0x7330
    bool m_IsReceiveQueueUpdated;         // 0x7334
};
ASSERT_SIZE(LocalAroundNetworkSearchManager::AroundNetworkStatus, 0x14);
ASSERT_SIZE(LocalAroundNetworkSearchManager::Message, 0x328);
ASSERT_OFFSET(LocalAroundNetworkSearchManager, m_CriticalSection, 0x174);
ASSERT_OFFSET(LocalAroundNetworkSearchManager, m_SendQueueNum, 0x4D44);
ASSERT_OFFSET(LocalAroundNetworkSearchManager, m_IsReceiveQueueUpdated, 0x7334);
ASSERT_SIZE(LocalAroundNetworkSearchManager, 0x7338);
} // namespace local
} // namespace pia
} // namespace nn
