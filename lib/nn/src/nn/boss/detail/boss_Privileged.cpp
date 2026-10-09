#include "nn/boss/detail/boss_Privileged.h"
#include "nn/boss/detail/boss_Ipc.h"

namespace nn {
namespace boss {
namespace detail {
namespace {
// the boss:P commands for the data of other programs (3dbrew "BOSS Services")
const bit32 COMMAND_DELETE_NS_DATA_PRIVILEGED = 0x041500C0;
const bit32 COMMAND_GET_NS_DATA_HEADER_INFO_PRIVILEGED = 0x04160142;
const bit32 COMMAND_READ_NS_DATA_PRIVILEGED = 0x04170182;
const bit32 COMMAND_SET_NS_DATA_NEW_FLAG_PRIVILEGED = 0x041A0100;
const bit32 COMMAND_GET_NS_DATA_NEW_FLAG_PRIVILEGED = 0x041B00C0;
const bit32 COMMAND_GET_NS_DATA_LAST_UPDATE_PRIVILEGED = 0x041C00C0;
} // namespace

// 0x0046D0D4 | nintendogs:bytes [tier A]
nn::Result nn::boss::detail::Privileged::ReadNsData(unsigned serialId, long long offset, unsigned char* pBuffer, unsigned size, unsigned* pReadSize, unsigned* pVersion)
{
    return ipc::ReadNsData(m_Session, serialId, offset, pBuffer, size, pReadSize, pVersion);
}

// 0x0046D138 | tier B
nn::Result nn::boss::detail::Privileged::DeleteNsData(unsigned serialId)
{
    return ipc::DeleteNsData(m_Session, serialId);
}

// 0x0046D168 | nintendogs:bytes [tier A]
nn::Result nn::boss::detail::Privileged::GetErrorCode(unsigned* pErrorCode, nn::boss::TaskResultCode resultCode)
{
    return ipc::GetErrorCode(m_Session, pErrorCode, resultCode);
}

// 0x0046D1AC | tier B
nn::Result nn::boss::detail::Privileged::GetTaskState(const unsigned char* pTaskId, unsigned size, unsigned char* pState, bool flag, unsigned* pCount, unsigned char* pDetail)
{
    return ipc::GetTaskState(m_Session, pTaskId, size, pState, flag, pCount, pDetail);
}

// 0x0046D214 | nintendogs:bytes [tier A]
nn::Result nn::boss::detail::Privileged::RegisterTask(const unsigned char* pTaskId, unsigned size, unsigned char hasOption, unsigned char option)
{
    return ipc::RegisterTaskCommand(m_Session, ipc::COMMAND_REGISTER_TASK, pTaskId, size, hasOption, option);
}

// 0x0046D268 | tier C
nn::Result nn::boss::detail::Privileged::SendProperty(nn::boss::PropertyType type, const unsigned char* pValue, unsigned int size)
{
    return ipc::SendProperty(m_Session, type, pValue, size);
}

// 0x0046D2B4 | tier C
nn::Result nn::boss::detail::Privileged::GetOptoutFlag(bool* pFlag)
{
    return ipc::GetOptoutFlag(m_Session, pFlag);
}

// 0x0046D2E8 | nintendogs:bytes [tier A]
nn::Result nn::boss::detail::Privileged::GetTaskResult(const unsigned char* pTaskId, unsigned size, nn::boss::TaskResultCode* pResultCode, unsigned* pErrorCode, unsigned char* pDetail)
{
    return ipc::GetTaskResult(m_Session, pTaskId, size, pResultCode, pErrorCode, pDetail);
}

// 0x0046D348 (name after the copy in User)
nn::Result nn::boss::detail::Privileged::GetTaskStatus(const unsigned char* pTaskId, unsigned int size, bool flag, unsigned char* pState, unsigned char option)
{
    return ipc::GetTaskStatus(m_Session, pTaskId, size, flag, pState, option);
}

// 0x0046D3A8 | tier C
nn::Result nn::boss::detail::Privileged::SetOptoutFlag(bool flag)
{
    return ipc::SetOptoutFlag(m_Session, flag);
}

// 0x0046D3E0 | nintendogs:bytes [tier A]
nn::Result nn::boss::detail::Privileged::GetStorageInfo(unsigned* pSize)
{
    return ipc::GetStorageInfo(m_Session, pSize);
}

// 0x0046D414 | nintendogs:bytes [tier A]
nn::Result nn::boss::detail::Privileged::UnregisterTask(const unsigned char* pTaskId, unsigned size, unsigned char option)
{
    return ipc::UnregisterTask(m_Session, pTaskId, size, option);
}

// 0x0046D460 | nintendogs:bytes [tier A]
nn::Result nn::boss::detail::Privileged::GetNsDataIdList(unsigned filter, unsigned* pIds, unsigned capacity, unsigned short* pCount, unsigned short startIndex, unsigned nextId, unsigned short* pNextIndex)
{
    return ipc::GetNsDataIdList(m_Session, filter, pIds, capacity, pCount, startIndex, nextId, pNextIndex);
}

// 0x0046D4CC | tier B
nn::Result nn::boss::detail::Privileged::ReceiveProperty(nn::boss::PropertyType type, unsigned char* pValue, unsigned size, unsigned* pReceivedSize)
{
    return ipc::ReceiveProperty(m_Session, type, pValue, size, pReceivedSize);
}

// 0x0046D524 | tier C
nn::Result nn::boss::detail::Privileged::RegisterStorage(unsigned long long extDataId, unsigned size, nn::fs::MediaType mediaType)
{
    return ipc::RegisterStorage(m_Session, extDataId, size, mediaType);
}

// 0x0046D564 | tier C
nn::Result nn::boss::detail::Privileged::UpdateTaskCount(const unsigned char* pTaskId, unsigned size, unsigned count)
{
    return ipc::UpdateTaskCount(m_Session, pTaskId, size, count);
}

// 0x0046D5A4 | tier C
nn::Result nn::boss::detail::Privileged::GetNsDataNewFlag(unsigned serialId, bool* pFlag)
{
    return ipc::GetNsDataNewFlag(m_Session, serialId, pFlag);
}

// 0x0046D5E0 | nintendogs:bytes [tier A]
nn::Result nn::boss::detail::Privileged::SetNsDataNewFlag(unsigned serialId, bool flag)
{
    return ipc::SetNsDataNewFlag(m_Session, serialId, flag);
}

// 0x0046D61C | tier C
nn::Result nn::boss::detail::Privileged::UnregisterStorage()
{
    return ipc::UnregisterStorage(m_Session);
}

// 0x0046D644 | tier C
nn::Result nn::boss::detail::Privileged::SendPropertyHandle(nn::boss::PropertyType type, nn::Handle handle)
{
    return ipc::SendPropertyHandle(m_Session, type, handle);
}

// 0x0046D688 | nintendogs:bytes [tier A]
nn::Result nn::boss::detail::Privileged::StartTaskImmediate(const unsigned char* pTaskId, unsigned size)
{
    return ipc::StartTaskCommand(m_Session, ipc::COMMAND_START_TASK_IMMEDIATE, pTaskId, size);
}

// 0x0046D6C8 | nintendogs:bytes [tier A]
nn::Result nn::boss::detail::Privileged::GetNsDataHeaderInfo(unsigned serialId, nn::boss::HeaderInfoType type, unsigned char* pValue, unsigned size)
{
    return ipc::GetNsDataHeaderInfo(m_Session, serialId, type, pValue, size);
}

// 0x0046D71C | tier C
nn::Result nn::boss::detail::Privileged::GetTaskFinishHandle(nn::Handle* pEvent)
{
    return ipc::GetTaskFinishHandle(m_Session, pEvent);
}

// 0x0046D750 | tier C
nn::Result nn::boss::detail::Privileged::GetNsDataLastUpdated(unsigned serialId, long long* pTime)
{
    return ipc::GetNsDataLastUpdated(m_Session, serialId, pTime);
}

// 0x0046D790 | tier C
nn::Result nn::boss::detail::Privileged::GetTaskServiceStatus(const unsigned char* pTaskId, unsigned int size, nn::boss::TaskServiceStatus* pStatus)
{
    return ipc::GetTaskServiceStatus(m_Session, pTaskId, size, pStatus);
}

// 0x0046D850 | tier C
nn::Result nn::boss::detail::Privileged::RegisterImmediateTask(const unsigned char* pTaskId, unsigned int size, unsigned char hasOption, unsigned char option)
{
    return ipc::RegisterTaskCommand(m_Session, ipc::COMMAND_REGISTER_IMMEDIATE_TASK, pTaskId, size, hasOption, option);
}

// 0x0046DA04 | nintendogs:bytes [tier A]
nn::Result nn::boss::detail::Privileged::StartTask(const unsigned char* pTaskId, unsigned size)
{
    return ipc::StartTaskCommand(m_Session, ipc::COMMAND_START_TASK, pTaskId, size);
}

// 0x0046D7DC | nintendogs:bytes [tier A]
nn::Result nn::boss::detail::Privileged::ReadNsDataPrivileged(unsigned long long programId, unsigned serialId, long long offset, unsigned char* pBuffer, unsigned size, unsigned* pReadSize, unsigned* pVersion)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_READ_NS_DATA_PRIVILEGED;
    command[6] = size;
    *reinterpret_cast<unsigned long long*>(&command[1]) = programId;
    command[3] = serialId;
    command[7] = ipc::WriteBufferDescriptor(size);
    command[8] = reinterpret_cast<uptr>(pBuffer);
    *reinterpret_cast<long long*>(&command[4]) = offset;
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pReadSize = command[2];
    *pVersion = command[3];
    return nn::Result(command[1]);
}

// 0x0046D8A4 | tier B
nn::Result nn::boss::detail::Privileged::DeleteNsDataPrivileged(unsigned long long programId, unsigned int serialId)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_DELETE_NS_DATA_PRIVILEGED;
    command[3] = serialId;
    *reinterpret_cast<unsigned long long*>(&command[1]) = programId;
    return ipc::Send(m_Session, command);
}

// 0x0046D8DC | nintendogs:bytes [tier A]
nn::Result nn::boss::detail::Privileged::GetNsDataNewFlagPrivileged(unsigned long long programId, unsigned serialId, bool* pFlag)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_NS_DATA_NEW_FLAG_PRIVILEGED;
    command[3] = serialId;
    *reinterpret_cast<unsigned long long*>(&command[1]) = programId;
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pFlag = *reinterpret_cast<bool*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x0046D920 | nintendogs:bytes [tier A]
nn::Result nn::boss::detail::Privileged::SetNsDataNewFlagPrivileged(unsigned long long programId, unsigned serialId, bool flag)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_NS_DATA_NEW_FLAG_PRIVILEGED;
    command[3] = serialId;
    *reinterpret_cast<unsigned long long*>(&command[1]) = programId;
    ipc::SetByte(&command[4], flag);
    return ipc::Send(m_Session, command);
}

// 0x0046D960 | nintendogs:bytes [tier A]
nn::Result nn::boss::detail::Privileged::GetNsDataHeaderInfoPrivileged(unsigned long long programId, unsigned serialId, nn::boss::HeaderInfoType type, unsigned char* pValue, unsigned size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_NS_DATA_HEADER_INFO_PRIVILEGED;
    *reinterpret_cast<unsigned long long*>(&command[1]) = programId;
    command[3] = serialId;
    ipc::SetByte(&command[4], type);
    command[5] = size;
    command[7] = reinterpret_cast<uptr>(pValue);
    command[6] = ipc::WriteBufferDescriptor(size);
    return ipc::Send(m_Session, command);
}

// 0x0046D9BC | tier B
nn::Result nn::boss::detail::Privileged::GetNsDataLastUpdatedPrivileged(unsigned long long programId, unsigned serialId, long long* pTime)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_NS_DATA_LAST_UPDATE_PRIVILEGED;
    command[3] = serialId;
    *reinterpret_cast<unsigned long long*>(&command[1]) = programId;
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pTime = *reinterpret_cast<long long*>(&command[2]);
    return nn::Result(command[1]);
}

} // namespace detail
} // namespace boss
} // namespace nn
