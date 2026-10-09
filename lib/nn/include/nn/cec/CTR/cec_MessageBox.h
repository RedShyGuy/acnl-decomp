#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/cec/CTR/cec_Types.h"
#include "nn/fnd/fnd_DateTime.h"

namespace nn {
namespace cec {
namespace CTR {
class Message;
class MessageId;

// RTTI N2nn3cec3CTR10MessageBoxE @ 0x008CDE30
// vtable 0x008FBFF0 (vptr 0x008FBFF8), offset_to_top 0, 14 entries
// The message box of a title: its information, the inbox and the outbox (3dbrew "StreetPass").
// The virtual functions send the cecd commands and retry once (OpenFile, OpenAndReadFile and
// OpenAndWriteFile: up to three times) after stopping the scan when cecd is busy. Member names,
// the names of the virtual slots without symbols (after the cecd commands) and of the
// functions marked "(name is ours)" are ours.
class MessageBox
{
public:
    static const u32 MESSAGE_NUM_MAX = 99;
    // a title may have only 12 message boxes ... (value from the binary)
    static const u32 MESSAGE_BOX_NUM_MAX = 12;

    MessageBox(); // 0x0034F30C | fefates:bytes [tier B]
    // 0x0034F3D0 (deleting dtor)
    virtual ~MessageBox(); // 0x001367C0 slot 0x00 | nintendogs:bytes
    virtual nn::Result OpenFile(u32 programId, u32 path, u32 flags, u32* pSize) const; // 0x007291A0 slot 0x08 | nintendogs:bytes
    virtual nn::Result ReadFile(void* buffer, u32 size, u32* pReadSize) const; // 0x007292A4 slot 0x0C (name is ours)
    virtual nn::Result WriteFile(const void* buffer, u32 size); // 0x00729300 slot 0x10 (name is ours)
    virtual nn::Result ReadMessage(u32 programId, bool isOutBox, const u8* pMessageId, u32 messageIdSize, u32* pReadSize, void* buffer, u32 size) const; // 0x0034E248 slot 0x14 (name after the command)
    virtual nn::Result ReadMessageWithHMAC(u32 programId, bool isOutBox, const u8* pMessageId, u32 messageIdSize, u32* pReadSize, void* buffer, u32 size, const u8* pHmacKey) const; // 0x0034F010 slot 0x18 (name after the command)
    virtual nn::Result WriteMessage(u32 programId, bool isOutBox, u8* pMessageId, u32 messageIdSize, const void* buffer, u32 size); // 0x0034EB44 slot 0x1C (name after the command)
    virtual nn::Result WriteMessageWithHMAC(u32 programId, bool isOutBox, u8* pMessageId, u32 messageIdSize, const void* buffer, u32 size, const u8* pHmacKey); // 0x0034F180 slot 0x20 (name after the command)
    virtual nn::Result Delete(u32 programId, u32 path, bool isOutBox, const u8* pMessageId, u32 messageIdSize); // 0x0034F210 slot 0x24 (name after the command)
    virtual nn::Result SetData(u32 programId, const u8* data, u32 size, u32 option); // 0x0034F294 slot 0x28 | nintendogs:bytes
    virtual nn::Result ReadData(u8* buffer, u32 size, u32 option, const u8* parameter, u32 parameterSize) const; // 0x00729220 slot 0x2C (name after the command)
    virtual nn::Result OpenAndWriteFile(const u8* buffer, u32 size, u32 programId, u32 path, u32 flags) const; // 0x00728F3C slot 0x30 | nintendogs:bytes
    virtual nn::Result OpenAndReadFile(u8* buffer, u32 size, u32* pReadSize, u32 programId, u32 path, u32 flags) const; // 0x00728EC4 slot 0x34 | nintendogs:bytes

    // (symbols.json gives tier X; the code fits: the destructor calls it)
    nn::Result Finalize(); // 0x0013B398 | nintendogs:bytes [tier X]
    // writes back the box information unless closing after a failure
    void CloseMessageBox(bool isFailure); // 0x0013EA84 | nintendogs:bytes [tier A]
    nn::Result WriteBoxInfo(CecBoxType type, CecBoxInfoHeader& header, CecMessageHeader** messages); // 0x001408A0 | nintendogs:bytes [tier A]
    nn::Result WriteMessageBoxInfo(); // 0x00140978 | nintendogs:bytes [tier A]
    nn::Result ReadBoxInfo(CecBoxInfoHeader* header, CecMessageHeader** messages, u8* buffer, CecBoxType type); // 0x00143264 | nintendogs:bytes [tier A]
    nn::Result ReadMessage(void* buffer, u32 size, CecBoxType type, const MessageId& messageId); // 0x0034D668 | nintendogs:bytes [tier A]
    nn::Result ReadMessage(Message& message, void* buffer, u32 size, CecBoxType type, const MessageId& messageId); // 0x0034D720 | nintendogs:bytes [tier A]
    nn::Result WriteMessage(const Message& message, CecBoxType type, MessageId& messageId); // 0x0034D770 (name after the overload)
    nn::Result WriteMessage(const Message& message, CecBoxType type, MessageId& messageId, bool writeBoxInfo); // 0x0034D784 | nintendogs:bytes [tier A]
    nn::Result DeleteMessage(CecBoxType type, const MessageId& messageId, bool writeBoxInfo); // 0x0034DD68 | nintendogs:bytes [tier A]
    nn::Result OpenMessageBox(u32 programId, u32 privateId); // 0x0034E05C | nintendogs:bytes [tier A]
    u32 ReadOutBoxIndex(); // 0x0034E2D8 | nintendogs:bytes [tier A]
    nn::Result CreateMessageBox(u32 programId, u32 privateId, const u8* pHmacKey, const void* icon, u32 iconSize, const void* name, u32 nameSize, u32 inBoxSizeMax, u32 outBoxSizeMax, u32 inBoxMessageNumMax, u32 outBoxMessageNumMax, u32 messageSizeMax); // 0x0034E534 (name is ours)
    // removes the box (a copy of the inline function CreateMessageBox uses on failure; no callers)
    nn::Result RemoveMessageBox(u32 programId); // 0x0034E98C (name is ours)
    nn::Result DeleteMessageBox(); // 0x0034E9E0 (name is ours)
    u32 GetMessageBoxNum(u8 flags); // 0x0034EA4C | nintendogs:bytes [tier A]
    nn::Result DeleteAllMessages(CecBoxType type); // 0x0034EBD0 | nintendogs:bytes [tier A]
    nn::Result SetMessageBoxData(u32 type, const void* data, u32 size); // 0x0034ED24 | nintendogs:bytes [tier A]
    u32 ReadMessageBoxList(); // 0x0034EE38 | nintendogs:bytes [tier A]
    nn::Result WriteMessageBoxList(); // 0x0034EF7C | nintendogs:bytes [tier A]
    nn::Result CheckEulaParentalControl(); // 0x0034F0A0 | nintendogs:callgraph [tier A]
    nn::Result GetMessageId(MessageId* pMessageId, CecBoxType type, u32 index) const; // 0x00728CCC | nintendogs:bytes [tier A]
    CecMessageHeader* GetMessHeader(CecBoxType type, u32 index) const; // 0x00728D24 | nintendogs:bytes [tier A]
    u16 GetMessageTag(CecBoxType type, u32 index) const; // 0x00728DE0 | fefates:bytes [tier B]
    u32 GetMessageSize(CecBoxType type, u32 index) const; // 0x00728DF4 | nintendogs:bytes [tier A]
    s32 GetMessageIndex(CecBoxType type, u8* pMessageId) const; // 0x00728E08 | nintendogs:bytes-fuzzy [tier A]
    nn::fnd::DateTimeParameters GetMessageRecvDate(CecBoxType type, u32 index) const; // 0x00728FB8 | nintendogs:bytes [tier B]
    nn::Result ReadMessageBoxInfo(MessageBoxInfo* info, u32 programId) const; // 0x0072901C | nintendogs:callgraph [tier A]
    u8 GetMessageSendCount(CecBoxType type, u32 index) const; // 0x007290AC | nintendogs:bytes [tier B]
    nn::Result IsAgreeEulaAppRequired() const; // 0x007290C0 | fefates:bytes [tier B]
    bool IsOpened() const; // 0x00729190 (name is ours)

private:
    // the box information as the changes without writeBoxInfo leave it (inline, name is ours)
    static void BeginBoxInfoChange(bool& isChanged, CecBoxInfoHeader& work, const CecBoxInfoHeader& header)
    {
        if (!isChanged) {
            work = header;
        }
        isChanged = true;
    }
    // closes after a failure in OpenMessageBox (inline, name is ours)
    void AbortOpen();

    u32 m_ProgramId;                                // 0x004, 0: no box open
    bool m_IsValid;                                 // 0x008
    bool m_IsSystem;                                // 0x009, no EULA check
    bool m_IsInBoxChanged;                          // 0x00A
    bool m_IsOutBoxChanged;                         // 0x00B
    u32 m_Padding0C;                                // 0x00C
    MessageBoxList m_List;                          // 0x010
    MessageBoxInfo m_Info;                          // 0x19C
    CecBoxInfoHeader m_InBoxHeader;                 // 0x1FC
    CecMessageHeader* m_InBoxMessages[MESSAGE_NUM_MAX];   // 0x21C
    u8* m_InBoxBuffer;                              // 0x3A8
    CecBoxInfoHeader m_OutBoxHeader;                // 0x3AC
    CecMessageHeader* m_OutBoxMessages[MESSAGE_NUM_MAX];  // 0x3CC
    u8* m_OutBoxBuffer;                             // 0x558
    OutBoxIndexHeader m_OutBoxIndexHeader;          // 0x55C
    u8 m_OutBoxIndex[MESSAGE_NUM_MAX][8];           // 0x564, the message ids in sending order
    CecBoxInfoHeader m_InBoxHeaderWork;             // 0x87C
    CecBoxInfoHeader m_OutBoxHeaderWork;            // 0x89C
};
ASSERT_SIZE(MessageBox, 0x8BC);
} // namespace CTR
} // namespace cec
} // namespace nn
