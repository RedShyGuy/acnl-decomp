#pragma once

// Types of the cec library (StreetPass). The type names are from the binary (mangled signatures of
// nn::cec::CTR); the layouts are after 3dbrew "CECD Services" / "StreetPass" and the code of
// MessageBox (the member names are ours). Like all ARMCC enums CecBoxType is one byte.

#include "decomp.h"
#include "nn/fnd/fnd_DateTime.h"

namespace nn {
namespace cec {
namespace CTR {

// the two boxes of a message box
enum CecBoxType : u8 {
    CEC_BOXTYPE_INBOX = 0,
    CEC_BOXTYPE_OUTBOX = 1,
};

// the paths (file kinds) of cecd (3dbrew "CECD:Open", CecDataPathType; names are ours)
enum CecPath : u8 {
    CEC_PATH_MBOX_LIST = 1,
    CEC_PATH_MBOX_INFO = 2,
    CEC_PATH_INBOX_INFO = 3,
    CEC_PATH_OUTBOX_INFO = 4,
    CEC_PATH_OUTBOX_INDEX = 5,
    CEC_PATH_INBOX_MSG = 6,
    CEC_PATH_OUTBOX_MSG = 7,
    CEC_PATH_ROOT_DIR = 10,
    CEC_PATH_MBOX_DIR = 11,
    CEC_PATH_INBOX_DIR = 12,
    CEC_PATH_OUTBOX_DIR = 13,
    CEC_PATH_MBOX_DATA = 100,
};

// the header of a message (3dbrew "CEC Message format")
struct CecMessageHeader
{
    u16 magic;                           // 0x00, 0x6060
    u16 padding;                         // 0x02
    u32 messageSize;                     // 0x04, header, extended headers, body and HMAC
    u32 totalHeaderSize;                 // 0x08, with the extended headers
    u32 bodySize;                        // 0x0C
    u32 titleId;                         // 0x10
    u32 titleId2;                        // 0x14
    u32 batchId;                         // 0x18
    u32 unknown1C;                       // 0x1C
    u8 messageId[8];                     // 0x20
    u32 messageVersion;                  // 0x28
    u8 messageId2[8];                    // 0x2C
    u8 flags;                            // 0x34
    u8 sendMethod;                       // 0x35
    u8 isUnopened;                       // 0x36
    u8 isNew;                            // 0x37
    u32 senderId[2];                     // 0x38, a u64 (two words: the header is only 4-aligned)
    u32 senderId2[2];                    // 0x40
    nn::fnd::DateTimeParameters sent;    // 0x48
    nn::fnd::DateTimeParameters received; // 0x54
    nn::fnd::DateTimeParameters created; // 0x60
    u8 sendCount;                        // 0x6C
    u8 forwardCount;                     // 0x6D
    u16 userData;                        // 0x6E, the tag
};
ASSERT_SIZE(CecMessageHeader, 0x70);

// the header of a box (3dbrew "BoxInfo")
struct CecBoxInfoHeader
{
    u16 magic;           // 0x00, 0x6262
    u16 padding;         // 0x02
    u32 boxInfoSize;     // 0x04, the header and the message headers
    u32 maxBoxSize;      // 0x08
    u32 boxSize;         // 0x0C
    u32 maxMessageNum;   // 0x10
    u32 messageNum;      // 0x14
    u32 maxBatchSize;    // 0x18
    u32 maxMessageSize;  // 0x1C
};
ASSERT_SIZE(CecBoxInfoHeader, 0x20);

// the information of a message box (3dbrew "MBoxInfo")
struct MessageBoxInfo
{
    u16 magic;                                // 0x00, 0x6363
    u16 padding;                              // 0x02
    u32 programId;                            // 0x04
    u32 privateId;                            // 0x08
    u8 flag0C;                                // 0x0C
    u8 flag0D;                                // 0x0D
    u8 hmacKey[32];                           // 0x10
    u32 padding30;                            // 0x30
    nn::fnd::DateTimeParameters lastAccessed; // 0x34
    u8 flag40;                                // 0x40, set with SetMessageBoxData(140)
    u8 flag41;                                // 0x41, 141
    u8 flag42;                                // 0x42, 142
    u8 flag43;                                // 0x43, 143
    nn::fnd::DateTimeParameters lastReceived; // 0x44
    u8 padding50[16];                         // 0x50
};
ASSERT_SIZE(MessageBoxInfo, 0x60);

// the list of the message boxes of the system (3dbrew "MBoxList"; the name is ours)
struct MessageBoxList
{
    u16 magic;              // 0x000, 0x6868
    u16 padding;            // 0x002
    u32 version;            // 0x004
    u32 boxNum;             // 0x008
    u8 boxNames[24][16];    // 0x00C, the program ids as text
};
ASSERT_SIZE(MessageBoxList, 0x18C);

// the ids of the messages in the outbox, in sending order (3dbrew "OBIndex"; the name is ours)
struct OutBoxIndexHeader
{
    u16 magic;      // 0x0, 0x6767
    u16 padding;    // 0x2
    u32 messageNum; // 0x4
};
ASSERT_SIZE(OutBoxIndexHeader, 8);

} // namespace CTR
} // namespace cec
} // namespace nn
