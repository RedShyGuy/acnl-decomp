// The cecd commands and the functions that pick the session.
#include "nn/cec/CTR/detail/detail_Api.h"
#include <string.h>
#include "nn/cec/CTR/detail/cec_Result.h"
#include "nn/fnd/fnd_TimeSpan.h"
#include "nn/os/os_Thread.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/srv/srv_Api.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace cec {
namespace CTR {
namespace detail {
namespace {
// command headers (3dbrew "CECD Services")
const bit32 COMMAND_OPEN = 0x000100C2;
const bit32 COMMAND_READ = 0x00020042;
const bit32 COMMAND_READ_MESSAGE = 0x00030104;
const bit32 COMMAND_READ_MESSAGE_WITH_HMAC = 0x00040106;
const bit32 COMMAND_WRITE = 0x00050042;
const bit32 COMMAND_WRITE_MESSAGE = 0x00060104;
const bit32 COMMAND_WRITE_MESSAGE_WITH_HMAC = 0x00070106;
const bit32 COMMAND_DELETE = 0x00080102;
const bit32 COMMAND_SET_DATA = 0x000900C2;
const bit32 COMMAND_READ_DATA = 0x000A00C4;
const bit32 COMMAND_START = 0x000B0040;
const bit32 COMMAND_STOP = 0x000C0040;
const bit32 COMMAND_GET_CECD_STATE = 0x000E0000;
const bit32 COMMAND_GET_CHANGE_STATE_EVENT_HANDLE = 0x00100000;
const bit32 COMMAND_OPEN_AND_WRITE = 0x00110104;
const bit32 COMMAND_OPEN_AND_READ = 0x00120104;

// the translation descriptors (3dbrew "IPC")
const bit32 DESCRIPTOR_PROCESS_ID = 0x20;
// the HMAC key, a read buffer of 32 bytes
const bit32 HMAC_KEY_DESCRIPTOR = (32 << 4) | 0xA;

const char SERVICE_NAME[] = "cecd:u";
const s64 SESSION_WAIT_MSEC = 10;

inline bit32 ReadBufferDescriptor(size_t size)
{
    return (size << 4) | 0xA;
}

inline bit32 WriteBufferDescriptor(size_t size)
{
    return (size << 4) | 0xC;
}

inline bit32 ReadWriteBufferDescriptor(size_t size)
{
    return (size << 4) | 0xE;
}

inline void SetByte(bit32* word, u8 value)
{
    *reinterpret_cast<u8*>(word) = value;
}

inline nn::Result Send(nn::Handle session, bit32* command)
{
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

// the commands, on one session (CecdClient and CecdClient2 are copies of the same code)
inline nn::Result OpenCommand(nn::Handle session, u32 programId, u32 path, u32 flags, u32* pSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_OPEN;
    command[1] = programId;
    command[2] = path;
    command[3] = flags;
    command[4] = DESCRIPTOR_PROCESS_ID;
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    *pSize = command[2];
    return nn::Result(command[1]);
}

inline nn::Result ReadCommand(nn::Handle session, u32* pReadSize, void* pBuffer, u32 size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_READ;
    command[1] = size;
    command[2] = WriteBufferDescriptor(size);
    command[3] = reinterpret_cast<uptr>(pBuffer);
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    *pReadSize = command[2];
    return nn::Result(command[1]);
}

inline nn::Result ReadMessageCommand(nn::Handle session, u32 programId, bool isOutBox, const u8* pMessageId, u32 messageIdSize, u32* pReadSize, void* pBuffer, u32 size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_READ_MESSAGE;
    command[1] = programId;
    SetByte(&command[2], isOutBox);
    command[3] = messageIdSize;
    command[4] = size;
    command[5] = ReadBufferDescriptor(messageIdSize);
    command[6] = reinterpret_cast<uptr>(pMessageId);
    command[7] = WriteBufferDescriptor(size);
    command[8] = reinterpret_cast<uptr>(pBuffer);
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    *pReadSize = command[2];
    return nn::Result(command[1]);
}

inline nn::Result ReadMessageWithHMACCommand(nn::Handle session, u32 programId, bool isOutBox, const u8* pMessageId, u32 messageIdSize, u32* pReadSize, void* pBuffer, u32 size, const u8* pHmacKey)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_READ_MESSAGE_WITH_HMAC;
    command[1] = programId;
    SetByte(&command[2], isOutBox);
    command[3] = messageIdSize;
    command[4] = size;
    command[5] = ReadBufferDescriptor(messageIdSize);
    command[6] = reinterpret_cast<uptr>(pMessageId);
    command[7] = HMAC_KEY_DESCRIPTOR;
    command[8] = reinterpret_cast<uptr>(pHmacKey);
    command[9] = WriteBufferDescriptor(size);
    command[10] = reinterpret_cast<uptr>(pBuffer);
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    *pReadSize = command[2];
    return nn::Result(command[1]);
}

inline nn::Result WriteCommand(nn::Handle session, const void* pBuffer, u32 size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_WRITE;
    command[1] = size;
    command[3] = reinterpret_cast<uptr>(pBuffer);
    command[2] = ReadBufferDescriptor(size);
    return Send(session, command);
}

inline nn::Result WriteMessageCommand(nn::Handle session, u32 programId, bool isOutBox, u8* pMessageId, u32 messageIdSize, const void* pBuffer, u32 size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_WRITE_MESSAGE;
    command[1] = programId;
    SetByte(&command[2], isOutBox);
    command[3] = messageIdSize;
    command[4] = size;
    command[5] = ReadBufferDescriptor(size);
    command[6] = reinterpret_cast<uptr>(pBuffer);
    command[7] = ReadWriteBufferDescriptor(messageIdSize);
    command[8] = reinterpret_cast<uptr>(pMessageId);
    return Send(session, command);
}

inline nn::Result WriteMessageWithHMACCommand(nn::Handle session, u32 programId, bool isOutBox, u8* pMessageId, u32 messageIdSize, const void* pBuffer, u32 size, const u8* pHmacKey)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_WRITE_MESSAGE_WITH_HMAC;
    command[1] = programId;
    SetByte(&command[2], isOutBox);
    command[3] = messageIdSize;
    command[4] = size;
    command[5] = ReadBufferDescriptor(size);
    command[6] = reinterpret_cast<uptr>(pBuffer);
    command[7] = HMAC_KEY_DESCRIPTOR;
    command[8] = reinterpret_cast<uptr>(pHmacKey);
    command[9] = ReadWriteBufferDescriptor(messageIdSize);
    command[10] = reinterpret_cast<uptr>(pMessageId);
    return Send(session, command);
}

inline nn::Result DeleteCommand(nn::Handle session, u32 programId, u32 path, bool isOutBox, const u8* pMessageId, u32 messageIdSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_DELETE;
    command[1] = programId;
    command[2] = path;
    SetByte(&command[3], isOutBox);
    command[4] = messageIdSize;
    command[5] = ReadBufferDescriptor(messageIdSize);
    command[6] = reinterpret_cast<uptr>(pMessageId);
    return Send(session, command);
}

inline nn::Result SetDataCommand(nn::Handle session, u32 programId, const u8* pData, u32 size, u32 option)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_DATA;
    command[1] = programId;
    command[2] = size;
    command[3] = option;
    command[4] = ReadBufferDescriptor(size);
    command[5] = reinterpret_cast<uptr>(pData);
    return Send(session, command);
}

inline nn::Result ReadDataCommand(nn::Handle session, u8* pBuffer, u32 size, u32 option, const u8* pParameter, u32 parameterSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_READ_DATA;
    command[1] = size;
    command[2] = option;
    command[3] = parameterSize;
    command[4] = ReadBufferDescriptor(parameterSize);
    command[5] = reinterpret_cast<uptr>(pParameter);
    command[7] = reinterpret_cast<uptr>(pBuffer);
    command[6] = WriteBufferDescriptor(size);
    return Send(session, command);
}

// Start and Stop only differ in the command
inline nn::Result StartStopCommand(nn::Handle session, bit32 header, u32 cecCommand)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = header;
    command[1] = cecCommand;
    return Send(session, command);
}

inline nn::Result GetCecdStateCommand(nn::Handle session, u32* pState)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_CECD_STATE;
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    *pState = command[2];
    return nn::Result(command[1]);
}

inline nn::Result GetChangeStateEventHandleCommand(nn::Handle session, nn::Handle* pEvent)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_CHANGE_STATE_EVENT_HANDLE;
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    *pEvent = nn::Handle(command[3]);
    return nn::Result(command[1]);
}

inline nn::Result OpenAndWriteCommand(nn::Handle session, const void* pBuffer, u32 size, u32 programId, u32 path, u32 flags)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_OPEN_AND_WRITE;
    command[1] = size;
    command[2] = programId;
    command[3] = path;
    command[4] = flags;
    command[5] = DESCRIPTOR_PROCESS_ID;
    command[8] = reinterpret_cast<uptr>(pBuffer);
    command[7] = ReadBufferDescriptor(size);
    return Send(session, command);
}

inline nn::Result OpenAndReadCommand(nn::Handle session, void* pBuffer, u32 size, u32* pReadSize, u32 programId, u32 path, u32 flags)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_OPEN_AND_READ;
    command[3] = path;
    command[4] = flags;
    command[5] = DESCRIPTOR_PROCESS_ID;
    command[1] = size;
    command[2] = programId;
    command[8] = reinterpret_cast<uptr>(pBuffer);
    command[7] = WriteBufferDescriptor(size);
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    *pReadSize = command[2];
    return nn::Result(command[1]);
}
} // namespace

// 0x0097FA40
nn::Handle s_CecdSession;
// 0x0097FA44
nn::Handle s_CecdSession2;

// 0x0034FB9C | tier B
nn::Result CecdClient::Open(u32 programId, u32 path, u32 flags, u32* pSize)
{
    return OpenCommand(s_CecdSession, programId, path, flags, pSize);
}

// 0x0034FBEC (name after the command)
nn::Result CecdClient::Read(u32* pReadSize, void* pBuffer, u32 size)
{
    return ReadCommand(s_CecdSession, pReadSize, pBuffer, size);
}

// 0x0034F858 | tier B
nn::Result CecdClient::ReadMessage(u32 programId, bool isOutBox, const u8* pMessageId, u32 messageIdSize, u32* pReadSize, void* pBuffer, u32 size)
{
    return ReadMessageCommand(s_CecdSession, programId, isOutBox, pMessageId, messageIdSize, pReadSize, pBuffer, size);
}

// 0x0034FA4C | tier B
nn::Result CecdClient::ReadMessageWithHMAC(u32 programId, bool isOutBox, const u8* pMessageId, u32 messageIdSize, u32* pReadSize, void* pBuffer, u32 size, const u8* pHmacKey)
{
    return ReadMessageWithHMACCommand(s_CecdSession, programId, isOutBox, pMessageId, messageIdSize, pReadSize, pBuffer, size, pHmacKey);
}

// 0x0034FC74 | tier B
nn::Result CecdClient::Write(const void* pBuffer, u32 size)
{
    return WriteCommand(s_CecdSession, pBuffer, size);
}

// 0x0034F914 (name after the command)
nn::Result CecdClient::WriteMessage(u32 programId, bool isOutBox, u8* pMessageId, u32 messageIdSize, const void* pBuffer, u32 size)
{
    return WriteMessageCommand(s_CecdSession, programId, isOutBox, pMessageId, messageIdSize, pBuffer, size);
}

// 0x0034FAE0 (name after the command)
nn::Result CecdClient::WriteMessageWithHMAC(u32 programId, bool isOutBox, u8* pMessageId, u32 messageIdSize, const void* pBuffer, u32 size, const u8* pHmacKey)
{
    return WriteMessageWithHMACCommand(s_CecdSession, programId, isOutBox, pMessageId, messageIdSize, pBuffer, size, pHmacKey);
}

// 0x0034FCBC | tier B
nn::Result CecdClient::Delete(u32 programId, u32 path, bool isOutBox, const u8* pMessageId, u32 messageIdSize)
{
    return DeleteCommand(s_CecdSession, programId, path, isOutBox, pMessageId, messageIdSize);
}

// 0x0034FD18 | tier B
nn::Result CecdClient::SetData(u32 programId, const u8* pData, u32 size, u32 option)
{
    return SetDataCommand(s_CecdSession, programId, pData, size, option);
}

// 0x0034FD60 | tier B
nn::Result CecdClient::ReadData(u8* pBuffer, u32 size, u32 option, const u8* pParameter, u32 parameterSize)
{
    return ReadDataCommand(s_CecdSession, pBuffer, size, option, pParameter, parameterSize);
}

// 0x00144B80 | tier B
nn::Result CecdClient::Start(u32 command)
{
    return StartStopCommand(s_CecdSession, COMMAND_START, command);
}

// 0x0034FC3C | tier B
nn::Result CecdClient::Stop(u32 command)
{
    return StartStopCommand(s_CecdSession, COMMAND_STOP, command);
}

// 0x0034F8D8 (name after the command)
nn::Result CecdClient::GetCecdState(u32* pState)
{
    return GetCecdStateCommand(s_CecdSession, pState);
}

// 0x0034FB60 (name after the command)
nn::Result CecdClient::GetChangeStateEventHandle(nn::Handle* pEvent)
{
    return GetChangeStateEventHandleCommand(s_CecdSession, pEvent);
}

// 0x0034F9F4 | tier B
nn::Result CecdClient::OpenAndWrite(const void* pBuffer, u32 size, u32 programId, u32 path, u32 flags)
{
    return OpenAndWriteCommand(s_CecdSession, pBuffer, size, programId, path, flags);
}

// 0x0034F984 | tier B
nn::Result CecdClient::OpenAndRead(void* pBuffer, u32 size, u32* pReadSize, u32 programId, u32 path, u32 flags)
{
    return OpenAndReadCommand(s_CecdSession, pBuffer, size, pReadSize, programId, path, flags);
}

// 0x003501F0 | tier B
nn::Result CecdClient2::Open(u32 programId, u32 path, u32 flags, u32* pSize)
{
    return OpenCommand(s_CecdSession2, programId, path, flags, pSize);
}

// 0x00350240 (name after the command)
nn::Result CecdClient2::Read(u32* pReadSize, void* pBuffer, u32 size)
{
    return ReadCommand(s_CecdSession2, pReadSize, pBuffer, size);
}

// 0x0034FEAC | tier B
nn::Result CecdClient2::ReadMessage(u32 programId, bool isOutBox, const u8* pMessageId, u32 messageIdSize, u32* pReadSize, void* pBuffer, u32 size)
{
    return ReadMessageCommand(s_CecdSession2, programId, isOutBox, pMessageId, messageIdSize, pReadSize, pBuffer, size);
}

// 0x003500A0 | tier B
nn::Result CecdClient2::ReadMessageWithHMAC(u32 programId, bool isOutBox, const u8* pMessageId, u32 messageIdSize, u32* pReadSize, void* pBuffer, u32 size, const u8* pHmacKey)
{
    return ReadMessageWithHMACCommand(s_CecdSession2, programId, isOutBox, pMessageId, messageIdSize, pReadSize, pBuffer, size, pHmacKey);
}

// 0x003502C8 | tier B
nn::Result CecdClient2::Write(const void* pBuffer, u32 size)
{
    return WriteCommand(s_CecdSession2, pBuffer, size);
}

// 0x0034FF68 (name after the command)
nn::Result CecdClient2::WriteMessage(u32 programId, bool isOutBox, u8* pMessageId, u32 messageIdSize, const void* pBuffer, u32 size)
{
    return WriteMessageCommand(s_CecdSession2, programId, isOutBox, pMessageId, messageIdSize, pBuffer, size);
}

// 0x00350134 (name after the command)
nn::Result CecdClient2::WriteMessageWithHMAC(u32 programId, bool isOutBox, u8* pMessageId, u32 messageIdSize, const void* pBuffer, u32 size, const u8* pHmacKey)
{
    return WriteMessageWithHMACCommand(s_CecdSession2, programId, isOutBox, pMessageId, messageIdSize, pBuffer, size, pHmacKey);
}

// 0x00350310 | tier B
nn::Result CecdClient2::Delete(u32 programId, u32 path, bool isOutBox, const u8* pMessageId, u32 messageIdSize)
{
    return DeleteCommand(s_CecdSession2, programId, path, isOutBox, pMessageId, messageIdSize);
}

// 0x0035036C | tier B
nn::Result CecdClient2::SetData(u32 programId, const u8* pData, u32 size, u32 option)
{
    return SetDataCommand(s_CecdSession2, programId, pData, size, option);
}

// 0x003503B4 | tier B
nn::Result CecdClient2::ReadData(u8* pBuffer, u32 size, u32 option, const u8* pParameter, u32 parameterSize)
{
    return ReadDataCommand(s_CecdSession2, pBuffer, size, option, pParameter, parameterSize);
}

// 0x00144BB8 (name after the copy in CecdClient)
nn::Result CecdClient2::Start(u32 command)
{
    return StartStopCommand(s_CecdSession2, COMMAND_START, command);
}

// 0x00350290 | tier B
nn::Result CecdClient2::Stop(u32 command)
{
    return StartStopCommand(s_CecdSession2, COMMAND_STOP, command);
}

// 0x0034FF2C (name after the command)
nn::Result CecdClient2::GetCecdState(u32* pState)
{
    return GetCecdStateCommand(s_CecdSession2, pState);
}

// 0x003501B4 (name after the command)
nn::Result CecdClient2::GetChangeStateEventHandle(nn::Handle* pEvent)
{
    return GetChangeStateEventHandleCommand(s_CecdSession2, pEvent);
}

// 0x00350048 | tier B
nn::Result CecdClient2::OpenAndWrite(const void* pBuffer, u32 size, u32 programId, u32 path, u32 flags)
{
    return OpenAndWriteCommand(s_CecdSession2, pBuffer, size, programId, path, flags);
}

// 0x0034FFD8 | tier B
nn::Result CecdClient2::OpenAndRead(void* pBuffer, u32 size, u32* pReadSize, u32 programId, u32 path, u32 flags)
{
    return OpenAndReadCommand(s_CecdSession2, pBuffer, size, pReadSize, programId, path, flags);
}

// 0x00143694 | nintendogs:callgraph [tier A]
nn::Result Start(unsigned command)
{
    if (s_CecdSession2.IsValid()) {
        return CecdClient2::Start(command);
    }
    if (!s_CecdSession.IsValid()) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return CecdClient::Start(command);
}

// 0x0034F4E4 (name after the command)
nn::Result ReadMessage(u32 programId, bool isOutBox, const u8* pMessageId, u32 messageIdSize, u32* pReadSize, void* pBuffer, u32 size)
{
    if (s_CecdSession2.IsValid()) {
        return CecdClient2::ReadMessage(programId, isOutBox, pMessageId, messageIdSize, pReadSize, pBuffer, size);
    }
    if (!s_CecdSession.IsValid()) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return CecdClient::ReadMessage(programId, isOutBox, pMessageId, messageIdSize, pReadSize, pBuffer, size);
}

// 0x0034F54C | tier C
nn::Result GetCecdState(unsigned int* pState)
{
    if (s_CecdSession2.IsValid()) {
        return CecdClient2::GetCecdState(pState);
    }
    if (!s_CecdSession.IsValid()) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return CecdClient::GetCecdState(pState);
}

// 0x0034F588 (name after the command)
nn::Result WriteMessage(u32 programId, bool isOutBox, u8* pMessageId, u32 messageIdSize, const void* pBuffer, u32 size)
{
    if (s_CecdSession2.IsValid()) {
        return CecdClient2::WriteMessage(programId, isOutBox, pMessageId, messageIdSize, pBuffer, size);
    }
    if (!s_CecdSession.IsValid()) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return CecdClient::WriteMessage(programId, isOutBox, pMessageId, messageIdSize, pBuffer, size);
}

// 0x0034F5F0 | nintendogs:callgraph [tier A]
nn::Result OpenAndReadFile(unsigned char* pBuffer, unsigned size, unsigned* pReadSize, unsigned programId, unsigned path, unsigned flags)
{
    if (s_CecdSession2.IsValid()) {
        return CecdClient2::OpenAndRead(pBuffer, size, pReadSize, programId, path, flags);
    }
    if (!s_CecdSession.IsValid()) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return CecdClient::OpenAndRead(pBuffer, size, pReadSize, programId, path, flags);
}

// 0x0034F658 | nintendogs:callgraph [tier A]
nn::Result OpenAndWriteFile(const unsigned char* pBuffer, unsigned size, unsigned programId, unsigned path, unsigned flags)
{
    if (s_CecdSession2.IsValid()) {
        return CecdClient2::OpenAndWrite(pBuffer, size, programId, path, flags);
    }
    if (!s_CecdSession.IsValid()) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return CecdClient::OpenAndWrite(pBuffer, size, programId, path, flags);
}

// 0x0034F6A4 | nintendogs:bytes [tier A]
nn::Result FinalizeCecControl()
{
    if (!s_CecdSession.IsValid()) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return nn::svc::CloseHandle(s_CecdSession);
}

// 0x0034F6C8 (name after the command)
nn::Result ReadMessageWithHMAC(u32 programId, bool isOutBox, const u8* pMessageId, u32 messageIdSize, u32* pReadSize, void* pBuffer, u32 size, const u8* pHmacKey)
{
    if (s_CecdSession2.IsValid()) {
        return CecdClient2::ReadMessageWithHMAC(programId, isOutBox, pMessageId, messageIdSize, pReadSize, pBuffer, size, pHmacKey);
    }
    if (!s_CecdSession.IsValid()) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return CecdClient::ReadMessageWithHMAC(programId, isOutBox, pMessageId, messageIdSize, pReadSize, pBuffer, size, pHmacKey);
}

// 0x0034F73C | nintendogs:bytes [tier A]
nn::Result WaitForSessionValid()
{
    while (!s_CecdSession.IsValid()) {
        nn::os::Thread::SleepImpl(nn::fnd::TimeSpan::FromMilliSeconds(SESSION_WAIT_MSEC));
    }
    return nn::Result();
}

// 0x0034F778 | nintendogs:bytes [tier A]
nn::Result InitializeCecControl()
{
    nn::Result result = nn::srv::Initialize();
    if (result.IsFailure()) {
        return result;
    }
    return nn::srv::GetServiceHandle(&s_CecdSession, SERVICE_NAME, strlen(SERVICE_NAME), 0);
}

// 0x0034F7B4 (name after the command)
nn::Result WriteMessageWithHMAC(u32 programId, bool isOutBox, u8* pMessageId, u32 messageIdSize, const void* pBuffer, u32 size, const u8* pHmacKey)
{
    if (s_CecdSession2.IsValid()) {
        return CecdClient2::WriteMessageWithHMAC(programId, isOutBox, pMessageId, messageIdSize, pBuffer, size, pHmacKey);
    }
    if (!s_CecdSession.IsValid()) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return CecdClient::WriteMessageWithHMAC(programId, isOutBox, pMessageId, messageIdSize, pBuffer, size, pHmacKey);
}

// 0x0034F81C | tier C
nn::Result GetChangeStateEventHandle(nn::Handle* pEvent)
{
    if (s_CecdSession2.IsValid()) {
        return CecdClient2::GetChangeStateEventHandle(pEvent);
    }
    if (!s_CecdSession.IsValid()) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return CecdClient::GetChangeStateEventHandle(pEvent);
}

// 0x0034FDBC | nintendogs:callgraph [tier A]
nn::Result Open(unsigned programId, unsigned path, unsigned flags, unsigned* pSize)
{
    if (s_CecdSession2.IsValid()) {
        return CecdClient2::Open(programId, path, flags, pSize);
    }
    if (!s_CecdSession.IsValid()) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return CecdClient::Open(programId, path, flags, pSize);
}

// 0x0034FDF8 (name after the command)
nn::Result Read(u32* pReadSize, void* pBuffer, u32 size)
{
    if (s_CecdSession2.IsValid()) {
        return CecdClient2::Read(pReadSize, pBuffer, size);
    }
    if (!s_CecdSession.IsValid()) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return CecdClient::Read(pReadSize, pBuffer, size);
}

// 0x0034FE34 | tier C
nn::Result Stop(unsigned int command)
{
    if (s_CecdSession2.IsValid()) {
        return CecdClient2::Stop(command);
    }
    if (!s_CecdSession.IsValid()) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return CecdClient::Stop(command);
}

// 0x0034FE70 (name after the command)
nn::Result Write(const void* pBuffer, u32 size)
{
    if (s_CecdSession2.IsValid()) {
        return CecdClient2::Write(pBuffer, size);
    }
    if (!s_CecdSession.IsValid()) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return CecdClient::Write(pBuffer, size);
}

// 0x00350410 (name after the command)
nn::Result Delete(u32 programId, u32 path, bool isOutBox, const u8* pMessageId, u32 messageIdSize)
{
    if (s_CecdSession2.IsValid()) {
        return CecdClient2::Delete(programId, path, isOutBox, pMessageId, messageIdSize);
    }
    if (!s_CecdSession.IsValid()) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return CecdClient::Delete(programId, path, isOutBox, pMessageId, messageIdSize);
}

// 0x0035045C | nintendogs:callgraph [tier A]
nn::Result SetData(unsigned programId, const unsigned char* pData, unsigned size, unsigned option)
{
    if (s_CecdSession2.IsValid()) {
        return CecdClient2::SetData(programId, pData, size, option);
    }
    if (!s_CecdSession.IsValid()) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return CecdClient::SetData(programId, pData, size, option);
}

// 0x00350498 (name after the command)
nn::Result ReadData(u8* pBuffer, u32 size, u32 option, const u8* pParameter, u32 parameterSize)
{
    if (s_CecdSession2.IsValid()) {
        return CecdClient2::ReadData(pBuffer, size, option, pParameter, parameterSize);
    }
    if (!s_CecdSession.IsValid()) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return CecdClient::ReadData(pBuffer, size, option, pParameter, parameterSize);
}

} // namespace detail
} // namespace CTR
} // namespace cec
} // namespace nn
