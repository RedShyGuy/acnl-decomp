#pragma once

// The boss:U commands (3dbrew "BOSS Services") that User and Privileged both have: the two classes
// have their own copies of the same code, here once as inline functions on the session (the names
// of the functions are the ones of the methods; the header is ours).

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/boss/boss_Types.h"
#include "nn/fs/fs_Types.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace boss {
namespace detail {
namespace ipc {
// command headers (3dbrew "BOSS Services")
const bit32 COMMAND_INITIALIZE_SESSION = 0x00010082;
const bit32 COMMAND_SET_STORAGE_INFO = 0x00020100;
const bit32 COMMAND_UNREGISTER_STORAGE = 0x00030000;
const bit32 COMMAND_GET_STORAGE_INFO = 0x00040000;
const bit32 COMMAND_SET_OPTOUT_FLAG = 0x00090040;
const bit32 COMMAND_GET_OPTOUT_FLAG = 0x000A0000;
const bit32 COMMAND_REGISTER_TASK = 0x000B00C2;
const bit32 COMMAND_UNREGISTER_TASK = 0x000C0082;
const bit32 COMMAND_GET_NS_DATA_ID_LIST = 0x00100102;
const bit32 COMMAND_SEND_PROPERTY = 0x00140082;
const bit32 COMMAND_SEND_PROPERTY_HANDLE = 0x00150042;
const bit32 COMMAND_RECEIVE_PROPERTY = 0x00160082;
const bit32 COMMAND_UPDATE_TASK_COUNT = 0x00180082;
const bit32 COMMAND_GET_TASK_SERVICE_STATUS = 0x001B0042;
const bit32 COMMAND_START_TASK = 0x001C0042;
const bit32 COMMAND_START_TASK_IMMEDIATE = 0x001D0042;
const bit32 COMMAND_GET_TASK_FINISH_HANDLE = 0x001F0000;
const bit32 COMMAND_GET_TASK_STATE = 0x00200082;
const bit32 COMMAND_GET_TASK_RESULT = 0x00210042;
const bit32 COMMAND_GET_TASK_STATUS = 0x002300C2;
const bit32 COMMAND_DELETE_NS_DATA = 0x00260040;
const bit32 COMMAND_GET_NS_DATA_HEADER_INFO = 0x002700C2;
const bit32 COMMAND_READ_NS_DATA = 0x00280102;
const bit32 COMMAND_SET_NS_DATA_NEW_FLAG = 0x002B0080;
const bit32 COMMAND_GET_NS_DATA_NEW_FLAG = 0x002C0040;
const bit32 COMMAND_GET_NS_DATA_LAST_UPDATE = 0x002D0040;
const bit32 COMMAND_GET_ERROR_CODE = 0x002E0040;
const bit32 COMMAND_REGISTER_IMMEDIATE_TASK = 0x003500C2;

// the translation descriptors (3dbrew "IPC")
const bit32 DESCRIPTOR_PROCESS_ID = 0x20;
const bit32 DESCRIPTOR_COPY_HANDLE = 0;

inline bit32 ReadBufferDescriptor(size_t size)
{
    return (size << 4) | 0xA;
}

inline bit32 WriteBufferDescriptor(size_t size)
{
    return (size << 4) | 0xC;
}

inline void SetByte(bit32* word, u8 value)
{
    *reinterpret_cast<u8*>(word) = value;
}

inline void SetHalf(bit32* word, u16 value)
{
    *reinterpret_cast<u16*>(word) = value;
}

inline nn::Result Send(nn::Handle session, bit32* command)
{
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

inline nn::Result ReadNsData(nn::Handle session, unsigned serialId, long long offset, unsigned char* pBuffer, unsigned size, unsigned* pReadSize, unsigned* pVersion)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_READ_NS_DATA;
    command[1] = serialId;
    *reinterpret_cast<long long*>(&command[2]) = offset;
    command[4] = size;
    command[5] = WriteBufferDescriptor(size);
    command[6] = reinterpret_cast<uptr>(pBuffer);
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    *pReadSize = command[2];
    *pVersion = command[3];
    return nn::Result(command[1]);
}

inline nn::Result DeleteNsData(nn::Handle session, unsigned serialId)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_DELETE_NS_DATA;
    command[1] = serialId;
    return Send(session, command);
}

inline nn::Result GetErrorCode(nn::Handle session, unsigned* pErrorCode, nn::boss::TaskResultCode resultCode)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_ERROR_CODE;
    SetByte(&command[1], resultCode);
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    *pErrorCode = command[2];
    return nn::Result(command[1]);
}

inline nn::Result GetTaskState(nn::Handle session, const unsigned char* pTaskId, unsigned size, unsigned char* pState, bool flag, unsigned* pCount, unsigned char* pDetail)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_TASK_STATE;
    command[1] = size;
    SetByte(&command[2], flag);
    command[4] = reinterpret_cast<uptr>(pTaskId);
    command[3] = ReadBufferDescriptor(size);
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    *pState = *reinterpret_cast<u8*>(&command[2]);
    *pCount = command[3];
    *pDetail = *reinterpret_cast<u8*>(&command[4]);
    return nn::Result(command[1]);
}

// RegisterTask and RegisterImmediateTask only differ in the command
inline nn::Result RegisterTaskCommand(nn::Handle session, bit32 header, const unsigned char* pTaskId, unsigned size, unsigned char hasOption, unsigned char option)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = header;
    command[1] = size;
    SetByte(&command[2], hasOption);
    SetByte(&command[3], option);
    command[5] = reinterpret_cast<uptr>(pTaskId);
    command[4] = ReadBufferDescriptor(size);
    return Send(session, command);
}

inline nn::Result SendProperty(nn::Handle session, nn::boss::PropertyType type, const unsigned char* pValue, unsigned int size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SEND_PROPERTY;
    SetHalf(&command[1], type);
    command[2] = size;
    command[3] = ReadBufferDescriptor(size);
    command[4] = reinterpret_cast<uptr>(pValue);
    return Send(session, command);
}

inline nn::Result GetOptoutFlag(nn::Handle session, bool* pFlag)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_OPTOUT_FLAG;
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    *pFlag = *reinterpret_cast<bool*>(&command[2]);
    return nn::Result(command[1]);
}

inline nn::Result GetTaskResult(nn::Handle session, const unsigned char* pTaskId, unsigned size, nn::boss::TaskResultCode* pResultCode, unsigned* pErrorCode, unsigned char* pDetail)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_TASK_RESULT;
    command[1] = size;
    command[3] = reinterpret_cast<uptr>(pTaskId);
    command[2] = ReadBufferDescriptor(size);
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    *pResultCode = static_cast<nn::boss::TaskResultCode>(*reinterpret_cast<u8*>(&command[2]));
    *pErrorCode = command[3];
    *pDetail = *reinterpret_cast<u8*>(&command[4]);
    return nn::Result(command[1]);
}

inline nn::Result GetTaskStatus(nn::Handle session, const unsigned char* pTaskId, unsigned int size, bool flag, unsigned char* pState, unsigned char option)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_TASK_STATUS;
    command[1] = size;
    SetByte(&command[2], flag);
    SetByte(&command[3], option);
    command[5] = reinterpret_cast<uptr>(pTaskId);
    command[4] = ReadBufferDescriptor(size);
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    *pState = *reinterpret_cast<u8*>(&command[2]);
    return nn::Result(command[1]);
}

inline nn::Result SetOptoutFlag(nn::Handle session, bool flag)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_OPTOUT_FLAG;
    SetByte(&command[1], flag);
    return Send(session, command);
}

inline nn::Result GetStorageInfo(nn::Handle session, unsigned* pSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_STORAGE_INFO;
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    *pSize = command[2];
    return nn::Result(command[1]);
}

inline nn::Result UnregisterTask(nn::Handle session, const unsigned char* pTaskId, unsigned size, unsigned char option)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_UNREGISTER_TASK;
    command[1] = size;
    SetByte(&command[2], option);
    command[4] = reinterpret_cast<uptr>(pTaskId);
    command[3] = ReadBufferDescriptor(size);
    return Send(session, command);
}

inline nn::Result GetNsDataIdList(nn::Handle session, unsigned filter, unsigned* pIds, unsigned capacity, unsigned short* pCount, unsigned short startIndex, unsigned nextId, unsigned short* pNextIndex)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_NS_DATA_ID_LIST;
    command[1] = filter;
    command[2] = capacity;
    SetHalf(&command[3], startIndex);
    command[4] = nextId;
    command[5] = WriteBufferDescriptor(capacity * sizeof(u32));
    command[6] = reinterpret_cast<uptr>(pIds);
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    *pCount = *reinterpret_cast<u16*>(&command[2]);
    *pNextIndex = *reinterpret_cast<u16*>(&command[3]);
    return nn::Result(command[1]);
}

inline nn::Result ReceiveProperty(nn::Handle session, nn::boss::PropertyType type, unsigned char* pValue, unsigned size, unsigned* pReceivedSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_RECEIVE_PROPERTY;
    SetHalf(&command[1], type);
    command[2] = size;
    command[3] = WriteBufferDescriptor(size);
    command[4] = reinterpret_cast<uptr>(pValue);
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    *pReceivedSize = command[2];
    return nn::Result(command[1]);
}

inline nn::Result RegisterStorage(nn::Handle session, unsigned long long extDataId, unsigned size, nn::fs::MediaType mediaType)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_STORAGE_INFO;
    command[3] = size;
    *reinterpret_cast<unsigned long long*>(&command[1]) = extDataId;
    SetByte(&command[4], mediaType);
    return Send(session, command);
}

inline nn::Result UpdateTaskCount(nn::Handle session, const unsigned char* pTaskId, unsigned size, unsigned count)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_UPDATE_TASK_COUNT;
    command[1] = size;
    command[2] = count;
    command[4] = reinterpret_cast<uptr>(pTaskId);
    command[3] = ReadBufferDescriptor(size);
    return Send(session, command);
}

inline nn::Result GetNsDataNewFlag(nn::Handle session, unsigned serialId, bool* pFlag)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_NS_DATA_NEW_FLAG;
    command[1] = serialId;
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    *pFlag = *reinterpret_cast<bool*>(&command[2]);
    return nn::Result(command[1]);
}

inline nn::Result SetNsDataNewFlag(nn::Handle session, unsigned serialId, bool flag)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_NS_DATA_NEW_FLAG;
    command[1] = serialId;
    SetByte(&command[2], flag);
    return Send(session, command);
}

inline nn::Result UnregisterStorage(nn::Handle session)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_UNREGISTER_STORAGE;
    return Send(session, command);
}

inline nn::Result SendPropertyHandle(nn::Handle session, nn::boss::PropertyType type, nn::Handle handle)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SEND_PROPERTY_HANDLE;
    SetHalf(&command[1], type);
    command[2] = DESCRIPTOR_COPY_HANDLE;
    command[3] = handle.GetPrintableBits();
    return Send(session, command);
}

// StartTask and StartTaskImmediate only differ in the command
inline nn::Result StartTaskCommand(nn::Handle session, bit32 header, const unsigned char* pTaskId, unsigned size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = header;
    command[1] = size;
    command[3] = reinterpret_cast<uptr>(pTaskId);
    command[2] = ReadBufferDescriptor(size);
    return Send(session, command);
}

inline nn::Result GetNsDataHeaderInfo(nn::Handle session, unsigned serialId, nn::boss::HeaderInfoType type, unsigned char* pValue, unsigned size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_NS_DATA_HEADER_INFO;
    command[1] = serialId;
    SetByte(&command[2], type);
    command[3] = size;
    command[4] = WriteBufferDescriptor(size);
    command[5] = reinterpret_cast<uptr>(pValue);
    return Send(session, command);
}

inline nn::Result GetTaskFinishHandle(nn::Handle session, nn::Handle* pEvent)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_TASK_FINISH_HANDLE;
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    *pEvent = nn::Handle(command[3]);
    return nn::Result(command[1]);
}

inline nn::Result GetNsDataLastUpdated(nn::Handle session, unsigned serialId, long long* pTime)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_NS_DATA_LAST_UPDATE;
    command[1] = serialId;
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    *pTime = *reinterpret_cast<long long*>(&command[2]);
    return nn::Result(command[1]);
}

inline nn::Result GetTaskServiceStatus(nn::Handle session, const unsigned char* pTaskId, unsigned int size, nn::boss::TaskServiceStatus* pStatus)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_TASK_SERVICE_STATUS;
    command[1] = size;
    command[3] = reinterpret_cast<uptr>(pTaskId);
    command[2] = ReadBufferDescriptor(size);
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    *pStatus = static_cast<nn::boss::TaskServiceStatus>(*reinterpret_cast<u8*>(&command[2]));
    return nn::Result(command[1]);
}
} // namespace ipc
} // namespace detail
} // namespace boss
} // namespace nn
