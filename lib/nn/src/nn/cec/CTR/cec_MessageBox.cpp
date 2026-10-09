#include "nn/cec/CTR/cec_MessageBox.h"
#include <string.h>
#include "nn/CTR/CTR_SystemMenuData.h"
#include "nn/cec/CTR/CTR_Api.h"
#include "nn/cec/CTR/cec_CecControl.h"
#include "nn/cec/CTR/cec_CecControlSys.h"
#include "nn/cec/CTR/cec_Message.h"
#include "nn/cec/CTR/cec_MessageId.h"
#include "nn/cec/CTR/detail/cec_Result.h"
#include "nn/cec/CTR/detail/detail_Api.h"
#include "nn/fs/CTR/CTR_Api.h"
#include "nn/fs/fs_Api.h"
#include "nn/os/os_CriticalSection.h"

namespace nn {
namespace cec {
namespace CTR {
namespace {
// the magics of the files (3dbrew "StreetPass")
const u16 BOX_INFO_MAGIC = 0x6262;
const u16 OUTBOX_INDEX_MAGIC = 0x6767;
const u16 MESSAGE_BOX_LIST_MAGIC = 0x6868;
const u16 MESSAGE_BOX_INFO_MAGIC = 0x6363;
const u16 MESSAGE_MAGIC = 0x6060;

// the flags of CECD:Open (3dbrew "CECD Services"; names are ours)
const u32 OPEN_READ = 0x2;
const u32 OPEN_WRITE = 0x4;
const u32 OPEN_CREATE = 0x8;
const u32 OPEN_CHECK = 0x10;

// the options of CECD:SetData (names are ours)
const u32 SET_DATA_OUTBOX_INDEX = 0;
const u32 SET_DATA_MESSAGE_WRITTEN = 2;
const u32 SET_DATA_MESSAGE_DELETED = 3;
const u32 SET_DATA_BOX_CLOSED = 5;

// the options of CECD:ReadData (names are ours)
const u32 READ_DATA_EULA_VERSION = 1;
const u32 READ_DATA_EULA_AGREED = 2;
const u32 READ_DATA_PARENTAL_CONTROL = 3;

// the data files of a message box (CEC_PATH_MBOX_DATA + n; names are ours)
const u32 MBOX_DATA_ICON = 101;
const u32 MBOX_DATA_NAME = 110;
const u32 MBOX_DATA_FLAG_0 = 140;
const u32 MBOX_DATA_FLAG_1 = 141;
const u32 MBOX_DATA_FLAG_2 = 142;
const u32 MBOX_DATA_FLAG_3 = 143;
const u32 MBOX_DATA_ACCESS = 150;
// the data files SetMessageBoxData may write: 101 to 199
const u32 MBOX_DATA_FIRST = 101;
const u32 MBOX_DATA_NUM = 99;
const u32 MBOX_DATA_SIZE_MAX = 0x2800;
const u32 MBOX_NAME_SIZE_MAX = 128;

// the limits of CreateMessageBox (values from the binary)
const u32 BOXES_SIZE_MAX = 0x100000;

// the extended header every message needs
const u32 EXHEADER_TYPE_2 = 2;

// the flag of GetMessageBoxNum that is ignored
const u8 BOX_NUM_FLAG_IGNORED = 0x80;

// the module of nn::fs and its "not found" descriptions (3dbrew "Error codes")
const bit32 MODULE_FS = 17;
const bit32 DESCRIPTION_FS_NOT_FOUND_BEGIN = 100;
const bit32 DESCRIPTION_FS_NOT_FOUND_END = 179;

// how often OpenFile, OpenAndReadFile and OpenAndWriteFile try
const s32 RETRY_COUNT = 3;
} // namespace

using namespace nn::cec::CTR::detail;

// the state of the message boxes (names are ours)
// 0x00975BB9
bool s_IsMessageBoxOpen = false;
// StopScanning was called and StartScanning has to follow
// 0x00975BBA
bool s_IsScanningStopped = false;
// 0x00975BBB
bool s_IsEulaAgreed = false;
// 0x00975BBC
bool s_IsInBoxBufferAllocated = false;
// 0x00975BBD
bool s_IsOutBoxBufferAllocated = false;
// the message id GetMessageIndex compares when a header is missing
// 0x00975BC7
u8 s_ErrorMessageId[] = "ERROR   ";
// 0x00AE1A94
nn::os::CriticalSection s_MessageBoxLock((nn::os::CriticalSection::InitializeTag()));
// 0x00AE1AA0
nn::os::CriticalSection s_BufferLock((nn::os::CriticalSection::InitializeTag()));

namespace {
// clears a box information header (the original writes each field)
inline void ClearBoxInfoHeader(CecBoxInfoHeader& header)
{
    header.magic = 0;
    header.padding = 0;
    header.boxInfoSize = 0;
    header.maxBoxSize = 0;
    header.boxSize = 0;
    header.maxMessageNum = 0;
    header.messageNum = 0;
    header.maxBatchSize = 0;
    header.maxMessageSize = 0;
}

// cecd is busy scanning: stop it and try again
inline void StopScanningForRetry()
{
    CecControl::StopScanning(true, false);
    s_IsScanningStopped = true;
}

inline void FreeBuffer(u8*& buffer, bool& isAllocated)
{
    nn::os::CriticalSection::ScopedLock lock(s_BufferLock);
    if (isAllocated) {
        os_free(buffer);
        isAllocated = false;
    }
}

inline nn::Result AllocateBuffer(u8*& buffer, bool& isAllocated, u32 size)
{
    nn::os::CriticalSection::ScopedLock lock(s_BufferLock);
    if (isAllocated) {
        os_free(buffer);
        isAllocated = false;
    }
    buffer = static_cast<u8*>(os_malloc(size, 4));
    if (buffer == NULL) {
        isAllocated = false;
        return nn::Result(RESULT_OUT_OF_MEMORY);
    }
    isAllocated = true;
    return nn::Result();
}
} // namespace

inline void nn::cec::CTR::MessageBox::AbortOpen()
{
    m_ProgramId = 0;
    CloseMessageBox(true);
    if (s_IsScanningStopped) {
        CecControl::StartScanning(false);
    }
}

// 0x001367C0 slot 0x00 | nintendogs:bytes
// 0x0034F3D0 (deleting dtor)
nn::cec::CTR::MessageBox::~MessageBox()
{
    Finalize();
}

// 0x0013B398 | nintendogs:bytes [tier X]
nn::Result nn::cec::CTR::MessageBox::Finalize()
{
    m_ProgramId = 0;
    if (s_IsMessageBoxOpen) {
        CloseMessageBox(false);
    }
    FreeBuffer(m_InBoxBuffer, s_IsInBoxBufferAllocated);
    FreeBuffer(m_OutBoxBuffer, s_IsOutBoxBufferAllocated);
    if (s_IsScanningStopped) {
        CecControl::StartScanning(false);
        s_IsScanningStopped = false;
    }
    if (m_IsValid) {
        m_IsValid = false;
    }
    return nn::Result();
}

// 0x0013EA84 | nintendogs:bytes [tier A]
void nn::cec::CTR::MessageBox::CloseMessageBox(bool isFailure)
{
    nn::os::CriticalSection::ScopedLock lock(s_MessageBoxLock);
    if (!isFailure) {
        if (m_ProgramId != 0) {
            if (m_IsInBoxChanged) {
                WriteBoxInfo(CEC_BOXTYPE_INBOX, m_InBoxHeader, m_InBoxMessages);
            }
            if (m_IsOutBoxChanged) {
                WriteBoxInfo(CEC_BOXTYPE_OUTBOX, m_OutBoxHeader, m_OutBoxMessages);
            }
            if (s_IsMessageBoxOpen) {
                u32 size;
                OpenFile(m_ProgramId, MBOX_DATA_ACCESS, OPEN_WRITE | OPEN_CHECK, &size);
                WriteMessageBoxInfo();
            }
        }
        SetData(m_ProgramId, NULL, 0, SET_DATA_BOX_CLOSED);
    }
    m_ProgramId = 0;
    ClearBoxInfoHeader(m_InBoxHeader);
    ClearBoxInfoHeader(m_OutBoxHeader);
    memset(m_InBoxMessages, 0, sizeof(m_InBoxMessages));
    memset(m_OutBoxMessages, 0, sizeof(m_OutBoxMessages));
    m_OutBoxIndexHeader.magic = 0;
    m_OutBoxIndexHeader.padding = 0;
    m_OutBoxIndexHeader.messageNum = 0;
    memset(m_OutBoxIndex, 0, sizeof(m_OutBoxIndex));
    FreeBuffer(m_InBoxBuffer, s_IsInBoxBufferAllocated);
    FreeBuffer(m_OutBoxBuffer, s_IsOutBoxBufferAllocated);
    m_InBoxHeader.magic = BOX_INFO_MAGIC;
    m_OutBoxHeader.magic = BOX_INFO_MAGIC;
    m_OutBoxIndexHeader.magic = OUTBOX_INDEX_MAGIC;
    if (!isFailure) {
        CecControl::StartScanning(false);
        s_IsScanningStopped = false;
    }
    s_IsMessageBoxOpen = false;
}

// 0x001408A0 | nintendogs:bytes [tier A]
nn::Result nn::cec::CTR::MessageBox::WriteBoxInfo(CecBoxType type, CecBoxInfoHeader& header, CecMessageHeader** messages)
{
    (void)messages;
    if (m_ProgramId == 0) {
        return nn::Result(RESULT_NOT_AUTHORIZED);
    }
    if (type == CEC_BOXTYPE_INBOX) {
        nn::Result result = OpenAndWriteFile(reinterpret_cast<const u8*>(&header), sizeof(CecBoxInfoHeader), m_ProgramId, CEC_PATH_INBOX_INFO, OPEN_WRITE | OPEN_CHECK);
        if (result.IsFailure()) {
            return result;
        }
        result = ReadBoxInfo(&m_InBoxHeader, m_InBoxMessages, m_InBoxBuffer, CEC_BOXTYPE_INBOX);
        m_IsInBoxChanged = false;
        return result;
    }
    nn::Result result = OpenAndWriteFile(reinterpret_cast<const u8*>(&header), sizeof(CecBoxInfoHeader), m_ProgramId, CEC_PATH_OUTBOX_INFO, OPEN_WRITE | OPEN_CHECK);
    if (result.IsFailure()) {
        return result;
    }
    result = ReadBoxInfo(&m_OutBoxHeader, m_OutBoxMessages, m_OutBoxBuffer, CEC_BOXTYPE_OUTBOX);
    m_IsOutBoxChanged = false;
    return result;
}

// 0x00140978 | nintendogs:bytes [tier A]
nn::Result nn::cec::CTR::MessageBox::WriteMessageBoxInfo()
{
    if (m_ProgramId == 0) {
        return nn::Result(RESULT_NOT_AUTHORIZED);
    }
    return OpenAndWriteFile(reinterpret_cast<const u8*>(&m_Info), sizeof(MessageBoxInfo), m_ProgramId, CEC_PATH_MBOX_INFO, OPEN_WRITE | OPEN_CHECK);
}

// 0x00143264 | nintendogs:bytes [tier A]
nn::Result nn::cec::CTR::MessageBox::ReadBoxInfo(CecBoxInfoHeader* header, CecMessageHeader** messages, u8* buffer, CecBoxType type)
{
    (void)buffer;
    nn::os::CriticalSection::ScopedLock lock(s_MessageBoxLock);
    if (m_ProgramId == 0) {
        return nn::Result(RESULT_NOT_AUTHORIZED);
    }
    u32 size = 0;
    u32 readSize = 0;
    if (type == CEC_BOXTYPE_INBOX) {
        OpenFile(m_ProgramId, CEC_PATH_INBOX_INFO, OPEN_READ, &size);
    } else {
        OpenFile(m_ProgramId, CEC_PATH_OUTBOX_INFO, OPEN_READ, &size);
    }
    if (size == 0) {
        return nn::Result(RESULT_NO_DATA);
    }

    nn::Result result;
    u8* data;
    if (type == CEC_BOXTYPE_INBOX) {
        if (s_IsInBoxBufferAllocated) {
            header->messageNum = 0;
            FreeBuffer(m_InBoxBuffer, s_IsInBoxBufferAllocated);
        }
        result = AllocateBuffer(m_InBoxBuffer, s_IsInBoxBufferAllocated, size);
        if (result.IsFailure()) {
            header->messageNum = 0;
            return result;
        }
        result = OpenAndReadFile(m_InBoxBuffer, size, &readSize, m_ProgramId, CEC_PATH_INBOX_INFO, OPEN_READ | OPEN_CHECK);
        data = m_InBoxBuffer;
    } else {
        if (type == CEC_BOXTYPE_OUTBOX && s_IsOutBoxBufferAllocated) {
            header->messageNum = 0;
            FreeBuffer(m_OutBoxBuffer, s_IsOutBoxBufferAllocated);
        }
        result = AllocateBuffer(m_OutBoxBuffer, s_IsOutBoxBufferAllocated, size);
        if (result.IsFailure()) {
            header->messageNum = 0;
            return result;
        }
        result = OpenAndReadFile(m_OutBoxBuffer, size, &readSize, m_ProgramId, CEC_PATH_OUTBOX_INFO, OPEN_READ | OPEN_CHECK);
        data = m_OutBoxBuffer;
    }
    if (result.IsFailure() || readSize == 0) {
        header->messageNum = 0;
        if (type == CEC_BOXTYPE_INBOX) {
            FreeBuffer(m_InBoxBuffer, s_IsInBoxBufferAllocated);
        } else {
            FreeBuffer(m_OutBoxBuffer, s_IsOutBoxBufferAllocated);
        }
        return nn::Result(RESULT_NO_DATA);
    }

    if (readSize < sizeof(CecBoxInfoHeader)) {
        return nn::Result(RESULT_INVALID_DATA);
    }
    memcpy(header, data, sizeof(CecBoxInfoHeader));
    if (header->boxInfoSize != readSize) {
        return nn::Result(RESULT_INVALID_DATA);
    }
    if (sizeof(CecBoxInfoHeader) + header->messageNum * sizeof(CecMessageHeader) > readSize) {
        return nn::Result(RESULT_INVALID_DATA);
    }
    u8* p = data + sizeof(CecBoxInfoHeader);
    for (u32 i = 0; i < header->messageNum; i++) {
        CecMessageHeader messageHeader;
        memcpy(&messageHeader, p, sizeof(CecMessageHeader));
        if (messageHeader.magic != MESSAGE_MAGIC || messageHeader.titleId != m_ProgramId) {
            return nn::Result(RESULT_INVALID_DATA);
        }
        messages[i] = reinterpret_cast<CecMessageHeader*>(p);
        p += sizeof(CecMessageHeader);
    }
    if (size == 0) {
        return nn::Result(RESULT_NO_DATA);
    }
    return nn::Result();
}

// 0x0034D668 | nintendogs:bytes [tier A]
nn::Result nn::cec::CTR::MessageBox::ReadMessage(void* buffer, u32 size, CecBoxType type, const MessageId& messageId)
{
    if (m_ProgramId == 0) {
        return nn::Result(RESULT_NOT_AUTHORIZED);
    }
    if (buffer == NULL || size < sizeof(CecMessageHeader)) {
        return nn::Result(RESULT_OUT_OF_RANGE);
    }
    u32 readSize = 0;
    nn::Result result = ReadMessage(m_ProgramId, type == CEC_BOXTYPE_OUTBOX, reinterpret_cast<const u8*>(&messageId), MessageId::SIZE, &readSize, buffer, size);
    if (result.IsFailure()) {
        return result;
    }
    if (readSize == 0) {
        return nn::Result(RESULT_NO_DATA);
    }
    return nn::Result();
}

// 0x0034D720 | nintendogs:bytes [tier A]
nn::Result nn::cec::CTR::MessageBox::ReadMessage(Message& message, void* buffer, u32 size, CecBoxType type, const MessageId& messageId)
{
    nn::Result result = ReadMessage(buffer, size, type, messageId);
    if (result.IsFailure()) {
        return result;
    }
    return message.InputMessage(buffer, size);
}

// 0x0034D770 (name after the overload)
nn::Result nn::cec::CTR::MessageBox::WriteMessage(const Message& message, CecBoxType type, MessageId& messageId)
{
    return WriteMessage(message, type, messageId, true);
}

// 0x0034D784 | nintendogs:bytes [tier A]
nn::Result nn::cec::CTR::MessageBox::WriteMessage(const Message& message, CecBoxType type, MessageId& messageId, bool writeBoxInfo)
{
    nn::os::CriticalSection::ScopedLock lock(s_MessageBoxLock);
    bool exists = false;
    MessageId id;
    u32 sizeChange = 0;
    u32 numChange = 0;
    if (!m_IsSystem) {
        nn::Result result = CheckEulaParentalControl();
        if (result.IsFailure()) {
            return result;
        }
    }
    if (m_ProgramId == 0) {
        return nn::Result(RESULT_NOT_AUTHORIZED);
    }
    if (message.m_Header.bodySize == 0) {
        return nn::Result(RESULT_NO_DATA);
    }
    {
        u32 exHeaderSize;
        void* exHeader = NULL;
        if (message.GetExHeader(EXHEADER_TYPE_2, &exHeaderSize, &exHeader).IsFailure()) {
            return nn::Result(RESULT_NO_DATA);
        }
    }
    if (m_ProgramId != message.m_Header.titleId) {
        return nn::Result(RESULT_INVALID_ID);
    }
    message.GetMessageId(&id);
    CecMessageHeader header;
    message.OutputMessageHeader(&header);
    if (id.IsEmpty()) {
        header.created = nn::fnd::DateTime::GetNow().GetParameters();
    } else if (GetMessageIndex(type, reinterpret_cast<u8*>(&id)) != -1) {
        exists = true;
    }
    if (message.m_Header.sendCount > 1 && message.m_Header.forwardCount > 1) {
        return nn::Result(RESULT_INVALID_COMBINATION);
    }

    // the limits of the box (with the changes not written yet)
    const CecBoxInfoHeader* box;
    u32 messageSizeMax;
    if (type != CEC_BOXTYPE_INBOX) {
        box = m_IsOutBoxChanged ? &m_OutBoxHeaderWork : &m_OutBoxHeader;
        messageSizeMax = m_OutBoxHeader.maxMessageSize;
    } else {
        box = m_IsInBoxChanged ? &m_InBoxHeaderWork : &m_InBoxHeader;
        messageSizeMax = m_InBoxHeader.maxMessageSize;
    }
    u32 boxSizeMax = box->maxBoxSize;
    u32 boxSize = box->boxSize;
    u32 messageNumMax = box->maxMessageNum;
    u32 messageNum = box->messageNum;
    if (messageSizeMax != 0 && message.m_Header.messageSize > messageSizeMax) {
        return nn::Result(RESULT_MESSAGE_TOO_LARGE);
    }
    if (exists) {
        s32 index = GetMessageIndex(type, reinterpret_cast<u8*>(&id));
        if (index != -1) {
            CecMessageHeader* old = GetMessHeader(type, index);
            u32 oldSize = (old != NULL) ? old->messageSize : 0;
            if (boxSizeMax != 0 && message.m_Header.messageSize + (boxSize - oldSize) > boxSizeMax) {
                return nn::Result(RESULT_BOX_SIZE_FULL);
            }
            sizeChange = message.m_Header.messageSize - oldSize;
        }
    } else {
        if (boxSizeMax != 0 && message.m_Header.messageSize + boxSize > boxSizeMax) {
            return nn::Result(RESULT_BOX_SIZE_FULL);
        }
        sizeChange = message.m_Header.messageSize;
    }
    if (!exists) {
        if (messageNumMax != 0 && messageNum + 1 > messageNumMax) {
            return nn::Result(RESULT_BOX_MESSAGES_FULL);
        }
        numChange = 1;
    }

    u8* binary = static_cast<u8*>(os_malloc(message.m_Header.messageSize, 4));
    if (binary == NULL) {
        return nn::Result(RESULT_OUT_OF_MEMORY);
    }
    message.MakeMessageBinary(binary);
    memcpy(binary, &header, sizeof(CecMessageHeader));
    u8 idBuffer[MessageId::SIZE] = {};
    if (!id.IsEmpty()) {
        id.GetBinary(idBuffer);
    }
    nn::Result result = WriteMessageWithHMAC(m_ProgramId, type != CEC_BOXTYPE_INBOX, idBuffer, MessageId::SIZE, binary, message.m_Header.messageSize, m_Info.hmacKey);
    if (writeBoxInfo) {
        nn::Result boxResult;
        if (type == CEC_BOXTYPE_OUTBOX) {
            boxResult = WriteBoxInfo(CEC_BOXTYPE_OUTBOX, m_OutBoxHeader, m_OutBoxMessages);
        } else {
            boxResult = WriteBoxInfo(CEC_BOXTYPE_INBOX, m_InBoxHeader, m_InBoxMessages);
        }
        if (boxResult.IsFailure()) {
            os_free(binary);
            return boxResult;
        }
    } else if (type == CEC_BOXTYPE_OUTBOX) {
        BeginBoxInfoChange(m_IsOutBoxChanged, m_OutBoxHeaderWork, m_OutBoxHeader);
        if (result.IsSuccess()) {
            m_OutBoxHeaderWork.boxSize += sizeChange;
            m_OutBoxHeaderWork.messageNum += numChange;
        }
    } else {
        BeginBoxInfoChange(m_IsInBoxChanged, m_InBoxHeaderWork, m_InBoxHeader);
        if (result.IsSuccess()) {
            m_InBoxHeaderWork.boxSize += sizeChange;
            m_InBoxHeaderWork.messageNum += numChange;
        }
    }
    if (result.IsSuccess()) {
        if (id.IsEmpty()) {
            id = MessageId(idBuffer);
            messageId = id;
        }
        if (type == CEC_BOXTYPE_OUTBOX && !exists) {
            if (SetData(m_ProgramId, reinterpret_cast<const u8*>(&id), MessageId::SIZE, SET_DATA_MESSAGE_WRITTEN).IsSuccess()) {
                ReadOutBoxIndex();
            }
        }
    }
    os_free(binary);
    return result;
}

// 0x0034DD68 | nintendogs:bytes [tier A]
nn::Result nn::cec::CTR::MessageBox::DeleteMessage(CecBoxType type, const MessageId& messageId, bool writeBoxInfo)
{
    nn::os::CriticalSection::ScopedLock lock(s_MessageBoxLock);
    if (m_ProgramId == 0) {
        return nn::Result(RESULT_NOT_AUTHORIZED);
    }
    u8* pMessageId = reinterpret_cast<u8*>(const_cast<MessageId*>(&messageId));
    u32 oldSize = 0;
    if (type == CEC_BOXTYPE_OUTBOX) {
        s32 index = GetMessageIndex(type, pMessageId);
        if (index != -1) {
            CecMessageHeader* old = GetMessHeader(type, index);
            if (old != NULL) {
                oldSize = old->messageSize;
            }
        }
        nn::Result result = Delete(m_ProgramId, CEC_PATH_OUTBOX_MSG, type != CEC_BOXTYPE_INBOX, pMessageId, MessageId::SIZE);
        if (result.IsFailure()) {
            if (writeBoxInfo) {
                WriteBoxInfo(CEC_BOXTYPE_OUTBOX, m_OutBoxHeader, m_OutBoxMessages);
            } else {
                BeginBoxInfoChange(m_IsOutBoxChanged, m_OutBoxHeaderWork, m_OutBoxHeader);
            }
            return result;
        }
        if (writeBoxInfo) {
            result = WriteBoxInfo(CEC_BOXTYPE_OUTBOX, m_OutBoxHeader, m_OutBoxMessages);
            if (result.IsFailure()) {
                return result;
            }
        } else {
            BeginBoxInfoChange(m_IsOutBoxChanged, m_OutBoxHeaderWork, m_OutBoxHeader);
            m_OutBoxHeaderWork.boxSize -= oldSize;
            m_OutBoxHeaderWork.messageNum -= 1;
        }
        if (SetData(m_ProgramId, pMessageId, MessageId::SIZE, SET_DATA_MESSAGE_DELETED).IsSuccess()) {
            ReadOutBoxIndex();
        }
        return result;
    }

    s32 index = GetMessageIndex(type, pMessageId);
    if (index != -1) {
        CecMessageHeader* old = GetMessHeader(type, index);
        if (old != NULL) {
            oldSize = old->messageSize;
        }
    }
    nn::Result result = Delete(m_ProgramId, CEC_PATH_INBOX_MSG, type != CEC_BOXTYPE_INBOX, pMessageId, MessageId::SIZE);
    if (result.IsFailure()) {
        if (writeBoxInfo) {
            WriteBoxInfo(CEC_BOXTYPE_INBOX, m_InBoxHeader, m_InBoxMessages);
        } else {
            BeginBoxInfoChange(m_IsInBoxChanged, m_InBoxHeaderWork, m_InBoxHeader);
        }
        return result;
    }
    if (writeBoxInfo) {
        return WriteBoxInfo(CEC_BOXTYPE_INBOX, m_InBoxHeader, m_InBoxMessages);
    }
    BeginBoxInfoChange(m_IsInBoxChanged, m_InBoxHeaderWork, m_InBoxHeader);
    m_InBoxHeaderWork.boxSize -= oldSize;
    m_InBoxHeaderWork.messageNum -= 1;
    return result;
}

// 0x0034E05C | nintendogs:bytes [tier A]
nn::Result nn::cec::CTR::MessageBox::OpenMessageBox(u32 programId, u32 privateId)
{
    nn::os::CriticalSection::ScopedLock lock(s_MessageBoxLock);
    CecControl::Suspend();
    s_IsScanningStopped = true;
    m_ProgramId = programId;
    MessageBoxInfo info;
    nn::Result result = ReadMessageBoxInfo(&info, programId);
    if (result.IsFailure()) {
        AbortOpen();
        return result;
    }
    if (info.privateId != privateId) {
        AbortOpen();
        return nn::Result(RESULT_NOT_AUTHORIZED);
    }
    m_IsInBoxChanged = false;
    m_IsOutBoxChanged = false;
    memcpy(&m_Info, &info, sizeof(MessageBoxInfo));
    result = ReadBoxInfo(&m_InBoxHeader, m_InBoxMessages, m_InBoxBuffer, CEC_BOXTYPE_INBOX);
    if (result.IsFailure()) {
        AbortOpen();
        return result;
    }
    result = ReadBoxInfo(&m_OutBoxHeader, m_OutBoxMessages, m_OutBoxBuffer, CEC_BOXTYPE_OUTBOX);
    if (result.IsFailure()) {
        AbortOpen();
        return result;
    }
    ReadOutBoxIndex();
    m_Info.flag40 = 0;
    m_Info.flag41 = 0;
    m_Info.lastAccessed = nn::fnd::DateTime::GetNow().GetParameters();
    s_IsMessageBoxOpen = true;
    return nn::Result();
}

// 0x0034E2D8 | nintendogs:bytes [tier A]
u32 nn::cec::CTR::MessageBox::ReadOutBoxIndex()
{
    if (m_ProgramId == 0) {
        return 0;
    }
    u32 size = 0;
    u32 readSize = 0;
    nn::Result result = OpenFile(m_ProgramId, CEC_PATH_OUTBOX_INDEX, OPEN_READ, &size);
    if (result.GetDescription() == DESCRIPTION_INVALID_HANDLE) {
        return 0;
    }
    if (size == 0 || (result.GetModule() == MODULE_FS && DESCRIPTION_FS_NOT_FOUND_BEGIN <= result.GetDescription() && result.GetDescription() <= DESCRIPTION_FS_NOT_FOUND_END)) {
        if (SetData(m_ProgramId, NULL, 0, SET_DATA_OUTBOX_INDEX).IsFailure()) {
            return 0;
        }
    }
    u8* buffer = static_cast<u8*>(os_malloc(sizeof(OutBoxIndexHeader) + sizeof(m_OutBoxIndex), 4));
    if (buffer == NULL) {
        m_OutBoxIndexHeader.messageNum = 0;
        return 0;
    }
    if (OpenAndReadFile(buffer, size, &readSize, m_ProgramId, CEC_PATH_OUTBOX_INDEX, OPEN_READ | OPEN_CHECK).IsFailure()) {
        os_free(buffer);
        return 0;
    }
    if (readSize != 0) {
        memcpy(&m_OutBoxIndexHeader, buffer, sizeof(OutBoxIndexHeader));
    }
    if (m_OutBoxIndexHeader.magic != OUTBOX_INDEX_MAGIC || m_OutBoxIndexHeader.messageNum != (size - sizeof(OutBoxIndexHeader)) / MessageId::SIZE) {
        // make cecd write the index again and read it once more
        if (SetData(m_ProgramId, NULL, 0, SET_DATA_OUTBOX_INDEX).IsFailure()) {
            os_free(buffer);
            return 0;
        }
        if (OpenFile(m_ProgramId, CEC_PATH_OUTBOX_INDEX, OPEN_READ, &size).IsFailure() || OpenAndReadFile(buffer, size, &readSize, m_ProgramId, CEC_PATH_OUTBOX_INDEX, OPEN_READ | OPEN_CHECK).IsFailure()) {
            m_OutBoxIndexHeader.messageNum = 0;
            os_free(buffer);
            return 0;
        }
        memcpy(&m_OutBoxIndexHeader, buffer, sizeof(OutBoxIndexHeader));
        if (m_OutBoxIndexHeader.magic != OUTBOX_INDEX_MAGIC || m_OutBoxIndexHeader.messageNum != (size - sizeof(OutBoxIndexHeader)) / MessageId::SIZE) {
            m_OutBoxIndexHeader.messageNum = 0;
            os_free(buffer);
            return 0;
        }
    }
    const u8* p = buffer + sizeof(OutBoxIndexHeader);
    for (u32 i = 0; i < m_OutBoxIndexHeader.messageNum; i++) {
        memcpy(m_OutBoxIndex[i], p, MessageId::SIZE);
        p += MessageId::SIZE;
    }
    os_free(buffer);
    return m_OutBoxIndexHeader.messageNum;
}

// 0x0034E534 (name is ours)
nn::Result nn::cec::CTR::MessageBox::CreateMessageBox(u32 programId, u32 privateId, const u8* pHmacKey, const void* icon, u32 iconSize, const void* name, u32 nameSize, u32 inBoxSizeMax, u32 outBoxSizeMax, u32 inBoxMessageNumMax, u32 outBoxMessageNumMax, u32 messageSizeMax)
{
    CecControl::Suspend();
    s_IsScanningStopped = true;
    m_ProgramId = programId;
    if (programId == 0) {
        return nn::Result(RESULT_NOT_AUTHORIZED);
    }
    if (inBoxSizeMax + outBoxSizeMax > BOXES_SIZE_MAX || inBoxMessageNumMax > MESSAGE_NUM_MAX ||
        outBoxMessageNumMax > MESSAGE_NUM_MAX || messageSizeMax > Message::MESSAGE_SIZE_MAX ||
        inBoxMessageNumMax == 0 || outBoxMessageNumMax == 0 ||
        inBoxSizeMax == 0 || outBoxSizeMax == 0 || messageSizeMax == 0) {
        if (s_IsScanningStopped) {
            CecControl::StartScanning(false);
        }
        m_ProgramId = 0;
        return nn::Result(RESULT_OUT_OF_RANGE);
    }
    if (pHmacKey == NULL) {
        m_ProgramId = 0;
        return nn::Result(RESULT_OUT_OF_RANGE);
    }

    nn::Result result;
    u32 boxNum = GetMessageBoxNum(0);
    u32 i;
    for (i = 0; i < boxNum; i++) {
        if ((Base64Str2CecTitleId(m_List.boxNames[i]) >> 8) == (programId >> 8)) {
            result = nn::Result(RESULT_BOX_ALREADY_EXISTS);
            break;
        }
    }
    if (i == boxNum) {
        result = (GetMessageBoxNum(1) >= MESSAGE_BOX_NUM_MAX) ? nn::Result(RESULT_BOX_NUM_FULL) : nn::Result();
    }
    if (result.IsFailure()) {
        if (s_IsScanningStopped) {
            CecControl::StartScanning(false);
        }
        m_ProgramId = 0;
        return result;
    }

    m_InBoxHeader.maxBoxSize = inBoxSizeMax;
    m_OutBoxHeader.maxBoxSize = outBoxSizeMax;
    m_InBoxHeader.maxMessageNum = inBoxMessageNumMax;
    m_OutBoxHeader.maxMessageNum = outBoxMessageNumMax;
    m_InBoxHeader.maxMessageSize = messageSizeMax;
    m_OutBoxHeader.maxMessageSize = messageSizeMax;
    u32 size;
    OpenFile(m_ProgramId, CEC_PATH_MBOX_DIR, OPEN_CREATE, &size);
    u32 boxSize;
    OpenFile(m_ProgramId, CEC_PATH_INBOX_DIR, OPEN_CREATE, &boxSize);
    WriteBoxInfo(CEC_BOXTYPE_INBOX, m_InBoxHeader, m_InBoxMessages);
    OpenFile(m_ProgramId, CEC_PATH_OUTBOX_DIR, OPEN_CREATE, &boxSize);
    WriteBoxInfo(CEC_BOXTYPE_OUTBOX, m_OutBoxHeader, m_OutBoxMessages);
    if (SetData(m_ProgramId, NULL, 0, SET_DATA_OUTBOX_INDEX).IsSuccess()) {
        ReadOutBoxIndex();
    }

    memset(&m_Info, 0, sizeof(MessageBoxInfo));
    m_Info.magic = MESSAGE_BOX_INFO_MAGIC;
    m_Info.programId = programId;
    m_Info.flag0D = 1;
    m_Info.privateId = privateId;
    m_Info.flag0C = 1;
    memcpy(m_Info.hmacKey, pHmacKey, sizeof(m_Info.hmacKey));
    m_Info.lastAccessed = nn::fnd::DateTime::GetNow().GetParameters();

    result = WriteMessageBoxInfo();
    if (result.IsSuccess()) {
        result = WriteMessageBoxList();
    }
    if (result.IsSuccess()) {
        if (nameSize > MBOX_NAME_SIZE_MAX) {
            result = nn::Result(RESULT_INVALID_DATA);
        } else {
            result = SetMessageBoxData(MBOX_DATA_NAME, name, nameSize);
        }
    }
    if (result.IsSuccess()) {
        result = SetMessageBoxData(MBOX_DATA_ICON, icon, iconSize);
    }
    if (result.IsFailure()) {
        RemoveMessageBox(m_ProgramId);
        if (s_IsScanningStopped) {
            CecControl::StartScanning(false);
        }
        m_ProgramId = 0;
        return result;
    }
    s_IsMessageBoxOpen = true;
    return nn::Result();
}

// 0x0034E98C (name is ours)
nn::Result nn::cec::CTR::MessageBox::RemoveMessageBox(u32 programId)
{
    Delete(programId, CEC_PATH_MBOX_DIR, false, NULL, 0);
    nn::Result result = WriteMessageBoxList();
    CloseMessageBox(false);
    return result;
}

// 0x0034E9E0 (name is ours)
nn::Result nn::cec::CTR::MessageBox::DeleteMessageBox()
{
    if (m_ProgramId == 0) {
        return nn::Result(RESULT_NOT_AUTHORIZED);
    }
    Delete(m_ProgramId, CEC_PATH_MBOX_DIR, false, NULL, 0);
    nn::Result result = WriteMessageBoxList();
    m_ProgramId = 0;
    CloseMessageBox(false);
    return result;
}

// 0x0034EA4C | nintendogs:bytes [tier A]
u32 nn::cec::CTR::MessageBox::GetMessageBoxNum(u8 flags)
{
    u32 count = 0;
    if (ReadMessageBoxList() == 0) {
        return 0;
    }
    for (u32 i = 0; i < m_List.boxNum; i++) {
        u32 programId = Base64Str2CecTitleId(m_List.boxNames[i]);
        MessageBoxInfo info;
        if (ReadMessageBoxInfo(&info, programId).IsFailure()) {
            continue;
        }
        if ((info.flag0C & (flags & ~BOX_NUM_FLAG_IGNORED)) != 0 || flags == 0) {
            count++;
        }
    }
    return count;
}

// 0x0034EBD0 | nintendogs:bytes [tier A]
nn::Result nn::cec::CTR::MessageBox::DeleteAllMessages(CecBoxType type)
{
    nn::os::CriticalSection::ScopedLock lock(s_MessageBoxLock);
    if (m_ProgramId == 0) {
        return nn::Result(RESULT_NOT_AUTHORIZED);
    }
    u32 size;
    if (type == CEC_BOXTYPE_OUTBOX) {
        nn::Result result = Delete(m_ProgramId, CEC_PATH_OUTBOX_DIR, type != CEC_BOXTYPE_INBOX, NULL, 0);
        if (result.IsFailure()) {
            return result;
        }
        OpenFile(m_ProgramId, CEC_PATH_OUTBOX_DIR, OPEN_CREATE, &size);
        WriteBoxInfo(CEC_BOXTYPE_OUTBOX, m_OutBoxHeader, m_OutBoxMessages);
        if (SetData(m_ProgramId, NULL, 0, SET_DATA_OUTBOX_INDEX).IsSuccess()) {
            ReadOutBoxIndex();
        }
        return result;
    }
    nn::Result result = Delete(m_ProgramId, CEC_PATH_INBOX_DIR, type != CEC_BOXTYPE_INBOX, NULL, 0);
    if (result.IsFailure()) {
        return result;
    }
    OpenFile(m_ProgramId, CEC_PATH_INBOX_DIR, OPEN_CREATE, &size);
    WriteBoxInfo(CEC_BOXTYPE_INBOX, m_InBoxHeader, m_InBoxMessages);
    return result;
}

// 0x0034ED24 | nintendogs:bytes [tier A]
nn::Result nn::cec::CTR::MessageBox::SetMessageBoxData(u32 type, const void* data, u32 size)
{
    if (m_ProgramId == 0) {
        return nn::Result(RESULT_NOT_AUTHORIZED);
    }
    if (size > MBOX_DATA_SIZE_MAX) {
        return nn::Result(RESULT_TOO_LARGE);
    }
    switch (type) {
    case MBOX_DATA_FLAG_0:
        m_Info.flag40 = *static_cast<const u8*>(data);
        return WriteMessageBoxInfo();
    case MBOX_DATA_FLAG_1:
        m_Info.flag41 = *static_cast<const u8*>(data);
        return WriteMessageBoxInfo();
    case MBOX_DATA_FLAG_2:
        m_Info.flag42 = *static_cast<const u8*>(data);
        return WriteMessageBoxInfo();
    case MBOX_DATA_FLAG_3:
        m_Info.flag43 = *static_cast<const u8*>(data);
        return WriteMessageBoxInfo();
    default:
        break;
    }
    if (type - MBOX_DATA_FIRST >= MBOX_DATA_NUM) {
        return nn::Result(RESULT_OUT_OF_RANGE);
    }
    if (size == 0 || data == NULL) {
        return Delete(m_ProgramId, type, false, NULL, 0);
    }
    return OpenAndWriteFile(static_cast<const u8*>(data), size, m_ProgramId, type, OPEN_WRITE | OPEN_CHECK);
}

// 0x0034EE38 | nintendogs:bytes [tier A]
u32 nn::cec::CTR::MessageBox::ReadMessageBoxList()
{
    u32 readSize = 0;
    nn::Result result = OpenAndReadFile(reinterpret_cast<u8*>(&m_List), sizeof(MessageBoxList), &readSize, m_ProgramId, CEC_PATH_MBOX_LIST, OPEN_READ | OPEN_CHECK);
    if (result.GetDescription() == DESCRIPTION_INVALID_HANDLE) {
        return 0;
    }
    if (readSize == 0) {
        WriteMessageBoxList();
        SetData(m_ProgramId, NULL, 0, SET_DATA_BOX_CLOSED);
        return 0;
    }
    if (result.IsFailure() || m_List.magic != MESSAGE_BOX_LIST_MAGIC) {
        WriteMessageBoxList();
        return 0;
    }
    for (u32 i = 0; i < m_List.boxNum; i++) {
        u32 size;
        if (OpenFile(Base64Str2CecTitleId(m_List.boxNames[i]), CEC_PATH_MBOX_DIR, 0, &size).IsFailure()) {
            // a box of the list is missing: write the list again
            if (WriteMessageBoxList().IsFailure()) {
                return 0;
            }
            break;
        }
    }
    return readSize;
}

// 0x0034EF7C | nintendogs:bytes [tier A]
nn::Result nn::cec::CTR::MessageBox::WriteMessageBoxList()
{
    u32 size;
    OpenFile(m_ProgramId, CEC_PATH_ROOT_DIR, OPEN_CREATE, &size);
    nn::Result result = OpenAndWriteFile(reinterpret_cast<const u8*>(&m_List), sizeof(MessageBoxList), m_ProgramId, CEC_PATH_MBOX_LIST, OPEN_WRITE | OPEN_CHECK);
    if (result.IsFailure()) {
        return result;
    }
    return OpenAndReadFile(reinterpret_cast<u8*>(&m_List), sizeof(MessageBoxList), &size, m_ProgramId, CEC_PATH_MBOX_LIST, OPEN_READ | OPEN_CHECK);
}

// 0x0034F0A0 | nintendogs:callgraph [tier A]
nn::Result nn::cec::CTR::MessageBox::CheckEulaParentalControl()
{
    u8 value[8];
    nn::Result result = ReadData(value, 1, READ_DATA_EULA_AGREED, NULL, 0);
    if (result.IsFailure()) {
        return result;
    }
    if (value[0] == 0) {
        return nn::Result(RESULT_EULA_NOT_AGREED);
    }
    result = ReadData(value, 1, READ_DATA_PARENTAL_CONTROL, NULL, 0);
    if (result.IsFailure()) {
        return result;
    }
    if (value[0] != 0) {
        return nn::Result(RESULT_PARENTAL_CONTROL);
    }
    if (!s_IsEulaAgreed) {
        result = IsAgreeEulaAppRequired();
        s_IsEulaAgreed = result.IsSuccess();
        if (!s_IsEulaAgreed) {
            if (result == nn::Result(RESULT_OUT_OF_MEMORY)) {
                return result;
            }
            return nn::Result(RESULT_EULA_NOT_AGREED);
        }
    }
    return nn::Result();
}

// 0x0034F294 slot 0x28 | nintendogs:bytes
nn::Result nn::cec::CTR::MessageBox::SetData(u32 programId, const u8* data, u32 size, u32 option)
{
    nn::Result result = detail::SetData(programId, data, size, option);
    if (result.IsFailure() && result == nn::Result(RESULT_BUSY)) {
        StopScanningForRetry();
        result = detail::SetData(programId, data, size, option);
    }
    return result;
}

// 0x0034E248 slot 0x14 (name after the command)
nn::Result nn::cec::CTR::MessageBox::ReadMessage(u32 programId, bool isOutBox, const u8* pMessageId, u32 messageIdSize, u32* pReadSize, void* buffer, u32 size) const
{
    nn::Result result = detail::ReadMessage(programId, isOutBox, pMessageId, messageIdSize, pReadSize, buffer, size);
    if (result.IsFailure() && result == nn::Result(RESULT_BUSY)) {
        StopScanningForRetry();
        result = detail::ReadMessage(programId, isOutBox, pMessageId, messageIdSize, pReadSize, buffer, size);
    }
    return result;
}

// 0x0034EB44 slot 0x1C (name after the command)
nn::Result nn::cec::CTR::MessageBox::WriteMessage(u32 programId, bool isOutBox, u8* pMessageId, u32 messageIdSize, const void* buffer, u32 size)
{
    nn::Result result = detail::WriteMessage(programId, isOutBox, pMessageId, messageIdSize, buffer, size);
    if (result.IsFailure() && result == nn::Result(RESULT_BUSY)) {
        StopScanningForRetry();
        result = detail::WriteMessage(programId, isOutBox, pMessageId, messageIdSize, buffer, size);
    }
    return result;
}

// 0x0034F010 slot 0x18 (name after the command)
nn::Result nn::cec::CTR::MessageBox::ReadMessageWithHMAC(u32 programId, bool isOutBox, const u8* pMessageId, u32 messageIdSize, u32* pReadSize, void* buffer, u32 size, const u8* pHmacKey) const
{
    nn::Result result = detail::ReadMessageWithHMAC(programId, isOutBox, pMessageId, messageIdSize, pReadSize, buffer, size, pHmacKey);
    if (result.IsFailure() && result == nn::Result(RESULT_BUSY)) {
        StopScanningForRetry();
        result = detail::ReadMessageWithHMAC(programId, isOutBox, pMessageId, messageIdSize, pReadSize, buffer, size, pHmacKey);
    }
    return result;
}

// 0x0034F180 slot 0x20 (name after the command)
nn::Result nn::cec::CTR::MessageBox::WriteMessageWithHMAC(u32 programId, bool isOutBox, u8* pMessageId, u32 messageIdSize, const void* buffer, u32 size, const u8* pHmacKey)
{
    nn::Result result = detail::WriteMessageWithHMAC(programId, isOutBox, pMessageId, messageIdSize, buffer, size, pHmacKey);
    if (result.IsFailure() && result == nn::Result(RESULT_BUSY)) {
        StopScanningForRetry();
        result = detail::WriteMessageWithHMAC(programId, isOutBox, pMessageId, messageIdSize, buffer, size, pHmacKey);
    }
    return result;
}

// 0x0034F210 slot 0x24 (name after the command)
nn::Result nn::cec::CTR::MessageBox::Delete(u32 programId, u32 path, bool isOutBox, const u8* pMessageId, u32 messageIdSize)
{
    nn::Result result = detail::Delete(programId, path, isOutBox, pMessageId, messageIdSize);
    if (result.IsFailure() && result == nn::Result(RESULT_BUSY)) {
        StopScanningForRetry();
        result = detail::Delete(programId, path, isOutBox, pMessageId, messageIdSize);
    }
    return result;
}

// 0x0034F30C | fefates:bytes [tier B]
nn::cec::CTR::MessageBox::MessageBox()
{
    m_IsValid = false;
    s_IsInBoxBufferAllocated = false;
    s_IsOutBoxBufferAllocated = false;
    m_IsInBoxChanged = false;
    m_IsOutBoxChanged = false;
    ClearBoxInfoHeader(m_InBoxHeaderWork);
    ClearBoxInfoHeader(m_OutBoxHeaderWork);
    m_InBoxHeader.magic = BOX_INFO_MAGIC;
    m_OutBoxHeader.magic = BOX_INFO_MAGIC;
    m_OutBoxIndexHeader.magic = OUTBOX_INDEX_MAGIC;
    m_ProgramId = 0;
    m_IsSystem = CecControlSys::IsInitializedSys();
    m_IsValid = true;
}

// 0x00728CCC | nintendogs:bytes [tier A]
nn::Result nn::cec::CTR::MessageBox::GetMessageId(MessageId* pMessageId, CecBoxType type, u32 index) const
{
    CecMessageHeader* header = GetMessHeader(type, index);
    if (header == NULL || header->messageId == NULL) {
        return nn::Result(RESULT_NO_DATA);
    }
    if (pMessageId != NULL) {
        *pMessageId = MessageId(header->messageId);
    }
    return nn::Result();
}

// 0x00728D24 | nintendogs:bytes [tier A]
nn::cec::CTR::CecMessageHeader* nn::cec::CTR::MessageBox::GetMessHeader(CecBoxType type, u32 index) const
{
    if (type == CEC_BOXTYPE_INBOX) {
        if (index < m_InBoxHeader.messageNum) {
            return m_InBoxMessages[index];
        }
        return NULL;
    }
    if (index >= m_OutBoxHeader.messageNum) {
        return NULL;
    }
    if (m_OutBoxIndexHeader.messageNum != m_OutBoxHeader.messageNum) {
        return m_OutBoxMessages[index];
    }
    // in sending order
    MessageId id(m_OutBoxIndex[index]);
    for (u32 i = 0; i < m_OutBoxHeader.messageNum; i++) {
        if (id.IsEqual(m_OutBoxMessages[i]->messageId)) {
            return m_OutBoxMessages[i];
        }
    }
    return NULL;
}

// 0x00728DE0 | fefates:bytes [tier B]
u16 nn::cec::CTR::MessageBox::GetMessageTag(CecBoxType type, u32 index) const
{
    CecMessageHeader* header = GetMessHeader(type, index);
    if (header == NULL) {
        return 0;
    }
    return header->userData;
}

// 0x00728DF4 | nintendogs:bytes [tier A]
u32 nn::cec::CTR::MessageBox::GetMessageSize(CecBoxType type, u32 index) const
{
    CecMessageHeader* header = GetMessHeader(type, index);
    if (header == NULL) {
        return 0;
    }
    return header->messageSize;
}

// 0x00728E08 | nintendogs:bytes-fuzzy [tier A]
s32 nn::cec::CTR::MessageBox::GetMessageIndex(CecBoxType type, u8* pMessageId) const
{
    u32 num = (type == CEC_BOXTYPE_INBOX) ? m_InBoxHeader.messageNum : m_OutBoxHeader.messageNum;
    for (u32 i = 0; i < num; i++) {
        CecMessageHeader* header = GetMessHeader(type, i);
        MessageId id = (header != NULL && header->messageId != NULL) ? MessageId(header->messageId) : MessageId(s_ErrorMessageId);
        if (memcmp(pMessageId, &id, MessageId::SIZE) == 0) {
            return i;
        }
    }
    return -1;
}

// 0x00728EC4 slot 0x34 | nintendogs:bytes
nn::Result nn::cec::CTR::MessageBox::OpenAndReadFile(u8* buffer, u32 size, u32* pReadSize, u32 programId, u32 path, u32 flags) const
{
    nn::Result result;
    for (s32 i = 0; i < RETRY_COUNT; i++) {
        result = detail::OpenAndReadFile(buffer, size, pReadSize, programId, path, flags);
        if (result != nn::Result(RESULT_BUSY)) {
            break;
        }
        StopScanningForRetry();
    }
    return result;
}

// 0x00728F3C slot 0x30 | nintendogs:bytes
nn::Result nn::cec::CTR::MessageBox::OpenAndWriteFile(const u8* buffer, u32 size, u32 programId, u32 path, u32 flags) const
{
    nn::Result result;
    for (s32 i = 0; i < RETRY_COUNT; i++) {
        result = detail::OpenAndWriteFile(buffer, size, programId, path, flags);
        if (result != nn::Result(RESULT_BUSY)) {
            break;
        }
        StopScanningForRetry();
    }
    return result;
}

// 0x00728FB8 | nintendogs:bytes [tier B]
nn::fnd::DateTimeParameters nn::cec::CTR::MessageBox::GetMessageRecvDate(CecBoxType type, u32 index) const
{
    CecMessageHeader* header = GetMessHeader(type, index);
    if (header == NULL) {
        nn::fnd::DateTime epoch = nn::fnd::DateTime::EPOCH;
        return epoch.GetParameters();
    }
    return header->received;
}

// 0x0072901C | nintendogs:callgraph [tier A]
nn::Result nn::cec::CTR::MessageBox::ReadMessageBoxInfo(MessageBoxInfo* info, u32 programId) const
{
    if (programId == 0) {
        return nn::Result(RESULT_OUT_OF_RANGE);
    }
    u32 readSize = 0;
    memset(info, 0, sizeof(MessageBoxInfo));
    nn::Result result = OpenAndReadFile(reinterpret_cast<u8*>(info), sizeof(MessageBoxInfo), &readSize, programId, CEC_PATH_MBOX_INFO, OPEN_READ | OPEN_CHECK);
    if (result.GetDescription() == DESCRIPTION_INVALID_HANDLE) {
        return result;
    }
    if (readSize == 0) {
        return nn::Result(RESULT_NO_DATA);
    }
    return result;
}

// 0x007290AC | nintendogs:bytes [tier B]
u8 nn::cec::CTR::MessageBox::GetMessageSendCount(CecBoxType type, u32 index) const
{
    CecMessageHeader* header = GetMessHeader(type, index);
    if (header == NULL) {
        return 0;
    }
    return header->sendCount;
}

// 0x007290C0 | fefates:bytes [tier B]
nn::Result nn::cec::CTR::MessageBox::IsAgreeEulaAppRequired() const
{
    u16 agreedVersion[4] = {};
    nn::Result result = ReadData(reinterpret_cast<u8*>(agreedVersion), sizeof(u16), READ_DATA_EULA_VERSION, NULL, 0);
    if (result.IsFailure()) {
        return result;
    }
    nn::CTR::SystemMenuData* data = static_cast<nn::CTR::SystemMenuData*>(os_malloc(sizeof(nn::CTR::SystemMenuData), 4));
    if (data == NULL) {
        return nn::Result(RESULT_OUT_OF_MEMORY);
    }
    if (!nn::fs::IsInitialized()) {
        os_free(data);
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    if (nn::fs::CTR::GetSelfSystemMenuData(data).IsFailure()) {
        os_free(data);
        return nn::Result(RESULT_EULA_NOT_AGREED);
    }
    u16 requiredVersion = data->settings.eulaVersionMinor | (data->settings.eulaVersionMajor << 8);
    os_free(data);
    if (agreedVersion[0] >= requiredVersion) {
        return nn::Result();
    }
    return nn::Result(RESULT_EULA_NOT_AGREED);
}

// 0x00729190 (name is ours)
bool nn::cec::CTR::MessageBox::IsOpened() const
{
    return m_ProgramId != 0;
}

// 0x007291A0 slot 0x08 | nintendogs:bytes
nn::Result nn::cec::CTR::MessageBox::OpenFile(u32 programId, u32 path, u32 flags, u32* pSize) const
{
    nn::Result result;
    for (s32 i = 0; i < RETRY_COUNT; i++) {
        result = detail::Open(programId, path, flags, pSize);
        if (!(result.IsFailure() && i < RETRY_COUNT - 1 && result == nn::Result(RESULT_BUSY))) {
            break;
        }
        StopScanningForRetry();
    }
    return result;
}

// 0x00729220 slot 0x2C (name after the command)
nn::Result nn::cec::CTR::MessageBox::ReadData(u8* buffer, u32 size, u32 option, const u8* parameter, u32 parameterSize) const
{
    nn::Result result = detail::ReadData(buffer, size, option, parameter, parameterSize);
    if (result.IsFailure() && result == nn::Result(RESULT_BUSY)) {
        StopScanningForRetry();
        result = detail::ReadData(buffer, size, option, parameter, parameterSize);
    }
    return result;
}

// 0x007292A4 slot 0x0C (name is ours)
nn::Result nn::cec::CTR::MessageBox::ReadFile(void* buffer, u32 size, u32* pReadSize) const
{
    nn::Result result = detail::Read(pReadSize, buffer, size);
    if (result == nn::Result(RESULT_BUSY)) {
        StopScanningForRetry();
        return detail::Read(pReadSize, buffer, size);
    }
    return result;
}

// 0x00729300 slot 0x10 (name is ours)
nn::Result nn::cec::CTR::MessageBox::WriteFile(const void* buffer, u32 size)
{
    nn::Result result = detail::Write(buffer, size);
    if (result == nn::Result(RESULT_BUSY)) {
        StopScanningForRetry();
        return detail::Write(buffer, size);
    }
    return result;
}

} // namespace CTR
} // namespace cec
} // namespace nn
