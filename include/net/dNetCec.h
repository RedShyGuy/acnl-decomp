#pragma once

#include "decomp.h"
#include "nn/cec/CTR/cec_MessageBox.h"
#include "sead/seadExpHeap.h"

namespace nn {
namespace cec {
namespace CTR {
class Message;
} // namespace CTR
} // namespace cec
} // namespace nn

namespace net {
// StreetPass (nn::cec) of the game, from the original file dNetCec.cpp: the message box of the
// game in its own heap, writing, reading and deleting messages. The results are small codes: 0 success,
// 1 cecd busy, others per function. (The namespace, functions, types and members are ours.)
namespace cec {
// the unique id of the game for cec and the private id of its message box
const u32 UNIQUE_ID = 0x198D;
const u32 PRIVATE_ID = 0x2B95A30C;

// the header values of a new message
struct MessageParam
{
    u32 batchId;      // 0x0
    u16 userData;     // 0x4
    u8 sendMethod;    // 0x6
    u8 sendCount;     // 0x7
    u8 forwardCount;  // 0x8
    u8 flags;         // 0x9
};

// the values of a new message box
struct BoxParam
{
    BoxParam()
        : hmacKey(NULL), icon(NULL), iconSize(0), name(NULL), nameSize(0), inBoxSize(0x80000), outBoxSize(0x80000),
          inBoxMessageNum(99), outBoxMessageNum(99)
    {
    }

    const char* hmacKey; // 0x00
    const void* icon;    // 0x04
    u32 iconSize;        // 0x08
    const void* name;    // 0x0C
    u32 nameSize;        // 0x10
    u32 inBoxSize;       // 0x14
    u32 outBoxSize;      // 0x18
    u32 inBoxMessageNum; // 0x1C
    u32 outBoxMessageNum; // 0x20
};

sead::ExpHeap* GetHeap(); // 0x0050DB64
// a message to the outbox with the message icon (the size is rounded up to 4)
u8 SendMessage(const void* data, u32 size, u16 userData, const void* exHeaderData, u32 exHeaderSize,
               u8 sendCount); // 0x0050DB74
void InitializeControl(); // 0x0050DC18
// the body of a message into buffer, if it has exactly size bytes; received gets the date
bool ReadMessage(nn::cec::CTR::CecBoxType type, u32 index, void* buffer, u32 size,
                 nn::fnd::DateTimeParameters* received); // 0x0050DC24
// opens the message box of the game (and creates it if create is set): 0 open, 2 not there, 3 not
// authorized, 4 failed, else the result of CreateBox
u8 OpenBox(bool create); // 0x0050DD20
void CloseBox(); // 0x0050DF78
// the heap and the message box
void Create(); // 0x0050DFA4
u8 ReadMessageFromBox(nn::cec::CTR::MessageBox* box, nn::cec::CTR::CecBoxType type, u32 index, nn::cec::CTR::Message* message,
                      void* buffer, u32 size); // 0x0050E04C
u8 WriteMessage(nn::cec::CTR::MessageBox* box, const MessageParam* param, const void* body, u32 bodySize, const void* icon,
                u32 iconSize, const void* exHeaderData, u32 exHeaderSize); // 0x0050E1A0
bool DeleteMessage(nn::cec::CTR::MessageBox* box, nn::cec::CTR::CecBoxType type, u32 index); // 0x0050E3F0
bool DeleteAllMessages(nn::cec::CTR::MessageBox* box, nn::cec::CTR::CecBoxType type); // 0x0050E448
u8 CreateBox(nn::cec::CTR::MessageBox* box, const BoxParam* param, u32 uniqueId, u32 privateId,
             bool stopsScanning); // 0x0050E464
bool DeleteBox(nn::cec::CTR::MessageBox* box); // 0x0050E558

extern nn::cec::CTR::MessageBox* s_pMessageBox;
extern sead::ExpHeap* s_pHeap;

// the message box and the heap go
DECOMP_EMIT_COPY inline void Destroy() // 0x00132954
{
    if (s_pMessageBox != NULL) {
        delete s_pMessageBox;
        s_pMessageBox = NULL;
        s_pHeap->destroy();
        s_pHeap = NULL;
    }
}
} // namespace cec
} // namespace net
