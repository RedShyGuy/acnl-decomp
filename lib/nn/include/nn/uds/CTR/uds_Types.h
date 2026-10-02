#pragma once

// Types of nn::uds (local wireless communication, service "nwm::UDS"). The type names are from the
// binary (mangled signatures); members and constants are ours, their layout follows from the code
// (and matches 3dbrew "NWM_UDS").

#include "decomp.h"
#include "nn/cfg/CTR/cfg_Types.h"
#include "nn/nwm/nwm_Types.h"

namespace nn {
namespace uds {
namespace CTR {

enum ConnectType : u8 {
    CONNECT_TYPE_CLIENT = 1,
    CONNECT_TYPE_SPECTATOR = 2
};

// an endpoint of CreateEndpoint (the bind id of the service; 0 = none)
struct EndpointDescriptor
{
    u32 id;     // 0x00
};
ASSERT_SIZE(EndpointDescriptor, 0x4);

// the local friend code seed of a node, XORed with a key (so that applications cannot read it)
struct ScrambledLocalFriendCode
{
    u16 code[4];        // 0x00 the seed ^ key
    u16 nodeId;         // 0x08 ^ key
    u16 key;            // 0x0A low half of the creation time of the process
};
ASSERT_SIZE(ScrambledLocalFriendCode, 0xC);

struct NodeInformation
{
    ScrambledLocalFriendCode scrambledLocalFriendCode;  // 0x00
    nn::cfg::CTR::UserName userName;                              // 0x0C
    u16 nodeId;                                         // 0x24
    u8 reserved[2];                                     // 0x26
};
ASSERT_SIZE(NodeInformation, 0x28);

struct ConnectionStatus
{
    bit32 data[12];     // the reply of GetConnectionStatus, passed on unchanged
};
ASSERT_SIZE(ConnectionStatus, 0x30);

namespace detail {

// a node as the service reports it
struct NodeInformationRaw
{
    u64 localFriendCodeSeed;    // 0x00
    nn::cfg::CTR::UserName userName;      // 0x08
    u16 nodeId;                 // 0x20
    u8 reserved[6];             // 0x22
};
ASSERT_SIZE(NodeInformationRaw, 0x28);

const s32 NODE_MAX = 16;

struct NodeInformationList
{
    NodeInformationRaw nodes[NODE_MAX];
};
ASSERT_SIZE(NodeInformationList, 0x280);

// the encrypted node lists of a beacon (two vendor specific tags)
struct NodeInformationElement
{
    u8 data[0xFE];
};
ASSERT_SIZE(NodeInformationElement, 0xFE);

// the network part of a beacon (vendor specific tag of Nintendo, OUI 00:1F:32 type 0x15)
struct NetworkDescriptionElement
{
    u8 oui[3];                  // 0x00
    u8 ouiType;                 // 0x03
    u32 localCommunicationId;   // 0x04 big endian
    u8 subId;                   // 0x08
    u8 unknown09;               // 0x09
    u16 attribute;              // 0x0A big endian, ATTRIBUTE_*
    u32 networkId;              // 0x0C
    u8 nodeCount;               // 0x10
    u8 nodeCountMax;            // 0x11
    u8 unknown12;               // 0x12
    u8 unknown13;               // 0x13
    u8 unknown14[0xB];          // 0x14
    u8 hash[20];                // 0x1F SHA-1 of the element up to the application data, with
                                //      this field 0
    u8 applicationDataSize;     // 0x33
    u8 applicationData[200];    // 0x34
};
ASSERT_SIZE(NetworkDescriptionElement, 0xFC);

// a received data block: what Unbind reports
struct ReceiveReport
{
    bit32 data[4];
};
ASSERT_SIZE(ReceiveReport, 0x10);

} // namespace detail

} // namespace CTR
} // namespace uds
} // namespace nn
