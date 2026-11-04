#include "net/dNetCec.h"

#include "Other/dGameIcon.h"
#include "net/nex/dImpl.h"
#include "nn/cec/CTR/CTR_Api.h"
#include "nn/cec/CTR/cec_CecControl.h"
#include "nn/cec/CTR/cec_Message.h"
#include "nn/cec/CTR/cec_MessageId.h"
#include "nn/cec/CTR/detail/cec_Result.h"
#include "nn/fnd/fnd_IAllocator.h"
#include "script/dMsgLabelReader.h"
#include "script/dWordFix.h"
#include "script/dWordPtrSv.h"
#include "sead/seadNew.h"
#include "sead/seadSafeStringBase.h"
#include <cstring>

namespace net {
namespace cec {
using namespace nn::cec::CTR;
using namespace nn::cec::CTR::detail;

namespace {
// the memory of the cec library from the heap of the message box (the class is from the RTTI of the
// original; names are ours)
// vtable 0x0087C0B4 (vptr 0x0087C0BC), offset_to_top 0, 4 entries
class CecAllocator : public nn::fnd::IAllocator
{
public:
    virtual void* Allocate(size_t size, s32 alignment); // 0x004DC280 slot 0x00
    virtual void Free(void* p); // 0x004DC25C slot 0x04
    virtual ~CecAllocator(); // 0x004DC2A8 slot 0x08
};

// the HMAC key of the message box
const char* const HMAC_KEY = "ne2seon7xy1ars3duofzxjbmykufd0n8";

// reads the header values of a message (the original keeps the calls of a removed debug output; name
// is ours)
inline void DumpMessage(const Message& message)
{
    MessageId messageId;
    message.GetMessageId(&messageId);
    if (message.GetHeader().sendMethod == 0) {
        MessageId messageId2;
        message.GetMessageId2(&messageId2);
    }
    void* exHeader4;
    u32 exHeader4Size;
    message.GetExHeaderType4(&exHeader4, &exHeader4Size);
    u32 iconSize;
    void* icon;
    if (message.GetExHeader(2, &iconSize, &icon).IsSuccess() && iconSize > 16) {
        iconSize = 16;
    }
    u8 body[16];
    message.GetMessageBody(body, sizeof(body));
}
} // namespace

// 0x00954600
nn::cec::CTR::MessageBox* s_pMessageBox;
// 0x00954604
sead::ExpHeap* s_pHeap;

namespace {
// 0x00954608
CecAllocator s_Allocator;
} // namespace
// (builds s_Allocator)
// 0x00785428 (static initializer)

namespace {
// 0x004DC25C (name is ours)
void CecAllocator::Free(void* p)
{
    if (s_pHeap != NULL) {
        s_pHeap->free(p);
    }
}

// 0x004DC280 (name is ours)
void* CecAllocator::Allocate(size_t size, s32 alignment)
{
    if (s_pHeap == NULL) {
        return NULL;
    }
    return s_pHeap->tryAlloc(size, alignment);
}

// 0x004DC2A8 (name is ours)
// 0x004DC2A4 (deleting dtor)
CecAllocator::~CecAllocator()
{
    // empty (in the original too)
}
} // namespace

// 0x0050DB64 (name is ours)
sead::ExpHeap* GetHeap()
{
    return s_pHeap;
}

// 0x0050DB74 (name is ours)
u8 SendMessage(const void* data, u32 size, u16 userData, const void* exHeaderData, u32 exHeaderSize, u8 sendCount)
{
    if (size & 3) {
        size = (size + 3) & ~3;
    }
    MessageParam param;
    param.batchId = 1;
    param.userData = userData;
    param.sendMethod = 3;
    param.sendCount = sendCount;
    param.forwardCount = 1;
    param.flags = 3;
    g_GameIcon.LoadCecMessageIcon(s_pHeap);
    u8 result = WriteMessage(s_pMessageBox, &param, data, size, g_GameIcon.m_MessageIcon.m_pData,
                             g_GameIcon.m_MessageIcon.m_Size, exHeaderData, exHeaderSize);
    g_GameIcon.ReleaseCecMessageIcon();
    return result;
}

// 0x0050DC18 (name is ours)
void InitializeControl()
{
    CecControl::Initialize(s_Allocator);
}

// 0x0050DC24 (name is ours)
bool ReadMessage(CecBoxType type, u32 index, void* buffer, u32 size, nn::fnd::DateTimeParameters* received)
{
    Message message;
    u32 messageSize = s_pMessageBox->GetMessageSize(type, index);
    void* work = s_pHeap->tryAlloc(messageSize, 4);
    if (work == NULL) {
        return false;
    }
    const void* body = work;
    u32 bodySize = messageSize;
    u8 result = ReadMessageFromBox(s_pMessageBox, type, index, &message, work, messageSize);
    if (result == 0) {
        message.GetMessageBodyPointer(&body, &bodySize);
        if (body != NULL && bodySize == size) {
            std::memcpy(buffer, body, bodySize);
            if (received != NULL) {
                *received = message.GetHeader().received;
            }
        } else {
            result = 8;
        }
    }
    s_pHeap->free(work);
    return result == 0;
}

// 0x0050DD20 (name is ours)
u8 OpenBox(bool create)
{
    CecControl::Initialize(s_Allocator);
    nn::Result result = s_pMessageBox->OpenMessageBox(MakeCecTitleId(UNIQUE_ID, 0), PRIVATE_ID);
    if (result.IsSuccess()) {
        return 0;
    }
    if (result == RESULT_NO_DATA) {
        if (!create) {
            return 2;
        }
        BoxParam param;
        script::WordFix<64> word;
        script::MsgLabelReader::s_pInstance->Begin(NULL);
        script::MsgLabelReader::s_pInstance->ReadWord(&word, "HOME", 3, 0);
        script::MsgLabelReader::s_pInstance->End();
        char16 name[64];
        script::WordPtrSv nameWord(name, 64);
        nameWord.Copy(word);
        param.name = name;
        param.nameSize = nameWord.GetLength() * 2;
        g_GameIcon.LoadCecMessageBoxIcon(s_pHeap);
        param.hmacKey = HMAC_KEY;
        param.icon = g_GameIcon.m_BoxIcon.m_pData;
        param.iconSize = g_GameIcon.m_BoxIcon.m_Size;
        param.inBoxSize = 0xEC000;
        param.outBoxSize = 0x14000;
        param.inBoxMessageNum = 20;
        param.outBoxMessageNum = 2;
        u8 createResult = CreateBox(s_pMessageBox, &param, UNIQUE_ID, PRIVATE_ID, false);
        g_GameIcon.ReleaseCecMessageBoxIcon();
        return createResult;
    }
    if (result == RESULT_NOT_AUTHORIZED) {
        return 3;
    }
    if (result == RESULT_BUSY) {
        return 1;
    }
    return 4;
}

// 0x0050DF78 (name is ours)
void CloseBox()
{
    MessageBox* box = s_pMessageBox;
    if (box->IsOpened()) {
        box->CloseMessageBox();
    }
    CecControl::Finalize();
}

// 0x0050DFA4 (name is ours)
void Create()
{
    if (s_pMessageBox != NULL) {
        return;
    }
    sead::Heap* heap = g_pNetHeap;
    s_pHeap = sead::ExpHeap::create(0x28000, sead::SafeStringBase<char>("CecHeap"), heap,
                                    sead::Heap::HEAP_DIRECTION_FORWARD, false);
    CecControl::Initialize(s_Allocator);
    void* buffer = ::operator new(sizeof(MessageBox), heap, 4);
    std::memset(buffer, 0, sizeof(MessageBox));
    s_pMessageBox = new (buffer) MessageBox();
    CecControl::Finalize();
}

// 0x0050E04C (name is ours)
u8 ReadMessageFromBox(MessageBox* box, CecBoxType type, u32 index, Message* message, void* buffer, u32 size)
{
    if (index >= box->GetMessageNum(type) || size < box->GetMessageSize(type, index)) {
        return 8;
    }
    MessageId messageId;
    box->GetMessageId(&messageId, type, index);
    nn::Result result = box->ReadMessage(*message, buffer, size, type, messageId);
    if (result.IsFailure()) {
        if (result == RESULT_BUSY) {
            return 1;
        }
        if (result == RESULT_NOT_AUTHORIZED || result == RESULT_OUT_OF_RANGE) {
            return 8;
        }
        return 8;
    }
    DumpMessage(*message);
    return 0;
}

// 0x0050E1A0 (name is ours)
u8 WriteMessage(MessageBox* box, const MessageParam* param, const void* body, u32 bodySize, const void* icon, u32 iconSize,
                const void* exHeaderData, u32 exHeaderSize)
{
    Message message;
    if (message
            .NewMessage(MakeCecTitleId(UNIQUE_ID, 0), param->batchId, param->flags, param->sendMethod, param->sendCount,
                        param->forwardCount)
            .IsFailure()) {
        return 12;
    }
    if (body != NULL && bodySize != 0) {
        if (message.SetMessageBody(body, bodySize).IsFailure()) {
            return 12;
        }
    } else if (param->sendMethod != 1) {
        return 12;
    }
    if (icon == NULL || iconSize == 0) {
        return 12;
    }
    if (message.SetExHeader(2, iconSize, icon).IsFailure()) {
        return 12;
    }
    if (exHeaderData != NULL && exHeaderSize != 0) {
        if (message.SetExHeader(4, exHeaderSize, exHeaderData).IsFailure()) {
            return 12;
        }
    }
    message.SetUserData(param->userData);
    DumpMessage(message);
    MessageId messageId;
    nn::Result result = box->WriteMessage(message, CEC_BOXTYPE_OUTBOX, messageId);
    if (result.IsSuccess()) {
        return 0;
    }
    if (result == RESULT_EULA_NOT_AGREED) {
        return 9;
    }
    if (result == RESULT_PARENTAL_CONTROL) {
        return 10;
    }
    if (result == RESULT_NOT_AUTHORIZED) {
        return 12;
    }
    if (result == RESULT_BOX_SIZE_FULL || result == RESULT_BOX_MESSAGES_FULL) {
        return 11;
    }
    if (result == RESULT_BUSY) {
        return 1;
    }
    if (result == RESULT_MESSAGE_TOO_LARGE || result == RESULT_NO_DATA || result == RESULT_INVALID_COMBINATION) {
        return 12;
    }
    return 12;
}

// 0x0050E3F0 (name is ours)
bool DeleteMessage(MessageBox* box, CecBoxType type, u32 index)
{
    MessageId messageId;
    box->GetMessageId(&messageId, type, index);
    return box->DeleteMessage(type, messageId).IsSuccess();
}

// 0x0050E448 (name is ours)
bool DeleteAllMessages(MessageBox* box, CecBoxType type)
{
    return box->DeleteAllMessages(type).IsSuccess();
}

// 0x0050E464 (name is ours)
u8 CreateBox(MessageBox* box, const BoxParam* param, u32 uniqueId, u32 privateId, bool stopsScanning)
{
    if (stopsScanning) {
        CecControl::StopScanning(false, false);
    }
    nn::Result result = box->CreateMessageBox(MakeCecTitleId(uniqueId, 0), privateId,
                                              reinterpret_cast<const u8*>(param->hmacKey), param->icon, param->iconSize,
                                              param->name, param->nameSize, param->inBoxSize, param->outBoxSize,
                                              param->inBoxMessageNum, param->outBoxMessageNum, Message::MESSAGE_SIZE_MAX);
    if (result.IsSuccess()) {
        return 0;
    }
    if (result == RESULT_BOX_ALREADY_EXISTS) {
        return 5;
    }
    if (result == RESULT_BOX_NUM_FULL) {
        return 6;
    }
    if (result == RESULT_BUSY) {
        return 1;
    }
    if (result == RESULT_OUT_OF_RANGE || result == RESULT_TOO_LARGE || result == RESULT_INVALID_DATA) {
        return 7;
    }
    return 7;
}

// 0x0050E558 (name is ours)
bool DeleteBox(MessageBox* box)
{
    return box->DeleteMessageBox().IsSuccess();
}
} // namespace cec
} // namespace net
