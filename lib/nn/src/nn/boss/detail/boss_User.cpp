#include "nn/boss/detail/boss_User.h"
#include "nn/boss/detail/boss_Ipc.h"

namespace nn {
namespace boss {
namespace detail {
// 0x0046EEA8 | nintendogs:bytes [tier A]
nn::Result nn::boss::detail::User::InitializeSession(unsigned long long programId)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = ipc::COMMAND_INITIALIZE_SESSION;
    *reinterpret_cast<unsigned long long*>(&command[1]) = programId;
    command[3] = ipc::DESCRIPTOR_PROCESS_ID;
    return ipc::Send(m_Session, command);
}

// 0x0046E960 | nintendogs:callgraph [tier A]
nn::Result nn::boss::detail::User::ReadNsData(unsigned serialId, long long offset, unsigned char* pBuffer, unsigned size, unsigned* pReadSize, unsigned* pVersion)
{
    return ipc::ReadNsData(m_Session, serialId, offset, pBuffer, size, pReadSize, pVersion);
}

// 0x0046E9C4 | tier B
nn::Result nn::boss::detail::User::DeleteNsData(unsigned serialId)
{
    return ipc::DeleteNsData(m_Session, serialId);
}

// 0x0046E9F4 | nintendogs:callgraph [tier A]
nn::Result nn::boss::detail::User::GetErrorCode(unsigned* pErrorCode, nn::boss::TaskResultCode resultCode)
{
    return ipc::GetErrorCode(m_Session, pErrorCode, resultCode);
}

// 0x0046EA38 | tier B
nn::Result nn::boss::detail::User::GetTaskState(const unsigned char* pTaskId, unsigned size, unsigned char* pState, bool flag, unsigned* pCount, unsigned char* pDetail)
{
    return ipc::GetTaskState(m_Session, pTaskId, size, pState, flag, pCount, pDetail);
}

// 0x0046EAA0 | nintendogs:callgraph [tier A]
nn::Result nn::boss::detail::User::RegisterTask(const unsigned char* pTaskId, unsigned size, unsigned char hasOption, unsigned char option)
{
    return ipc::RegisterTaskCommand(m_Session, ipc::COMMAND_REGISTER_TASK, pTaskId, size, hasOption, option);
}

// 0x0046EAF4 | fefates:callgraph [tier A]
nn::Result nn::boss::detail::User::SendProperty(nn::boss::PropertyType type, const unsigned char* pValue, unsigned int size)
{
    return ipc::SendProperty(m_Session, type, pValue, size);
}

// 0x0046EB40 | tier C
nn::Result nn::boss::detail::User::GetOptoutFlag(bool* pFlag)
{
    return ipc::GetOptoutFlag(m_Session, pFlag);
}

// 0x0046EB74 | nintendogs:callgraph [tier A]
nn::Result nn::boss::detail::User::GetTaskResult(const unsigned char* pTaskId, unsigned size, nn::boss::TaskResultCode* pResultCode, unsigned* pErrorCode, unsigned char* pDetail)
{
    return ipc::GetTaskResult(m_Session, pTaskId, size, pResultCode, pErrorCode, pDetail);
}

// 0x0046EBD4 | tier B
nn::Result nn::boss::detail::User::GetTaskStatus(const unsigned char* pTaskId, unsigned int size, bool flag, unsigned char* pState, unsigned char option)
{
    return ipc::GetTaskStatus(m_Session, pTaskId, size, flag, pState, option);
}

// 0x0046EC34 | nintendogs:callgraph [tier A]
nn::Result nn::boss::detail::User::SetOptoutFlag(bool flag)
{
    return ipc::SetOptoutFlag(m_Session, flag);
}

// 0x0046EC6C | nintendogs:callgraph [tier A]
nn::Result nn::boss::detail::User::GetStorageInfo(unsigned* pSize)
{
    return ipc::GetStorageInfo(m_Session, pSize);
}

// 0x0046ECA0 | nintendogs:callgraph [tier A]
nn::Result nn::boss::detail::User::UnregisterTask(const unsigned char* pTaskId, unsigned size, unsigned char option)
{
    return ipc::UnregisterTask(m_Session, pTaskId, size, option);
}

// 0x0046ECEC | nintendogs:callgraph [tier A]
nn::Result nn::boss::detail::User::GetNsDataIdList(unsigned filter, unsigned* pIds, unsigned capacity, unsigned short* pCount, unsigned short startIndex, unsigned nextId, unsigned short* pNextIndex)
{
    return ipc::GetNsDataIdList(m_Session, filter, pIds, capacity, pCount, startIndex, nextId, pNextIndex);
}

// 0x0046ED58 | tier B
nn::Result nn::boss::detail::User::ReceiveProperty(nn::boss::PropertyType type, unsigned char* pValue, unsigned size, unsigned* pReceivedSize)
{
    return ipc::ReceiveProperty(m_Session, type, pValue, size, pReceivedSize);
}

// 0x0046EDB0 | tier C
nn::Result nn::boss::detail::User::RegisterStorage(unsigned long long extDataId, unsigned size, nn::fs::MediaType mediaType)
{
    return ipc::RegisterStorage(m_Session, extDataId, size, mediaType);
}

// 0x0046EDF0 | tier C
nn::Result nn::boss::detail::User::UpdateTaskCount(const unsigned char* pTaskId, unsigned size, unsigned count)
{
    return ipc::UpdateTaskCount(m_Session, pTaskId, size, count);
}

// 0x0046EE30 | tier C
nn::Result nn::boss::detail::User::GetNsDataNewFlag(unsigned serialId, bool* pFlag)
{
    return ipc::GetNsDataNewFlag(m_Session, serialId, pFlag);
}

// 0x0046EE6C | nintendogs:callgraph [tier A]
nn::Result nn::boss::detail::User::SetNsDataNewFlag(unsigned serialId, bool flag)
{
    return ipc::SetNsDataNewFlag(m_Session, serialId, flag);
}

// 0x0046EEE0 | tier C
nn::Result nn::boss::detail::User::UnregisterStorage()
{
    return ipc::UnregisterStorage(m_Session);
}

// 0x0046EF08 | tier C
nn::Result nn::boss::detail::User::SendPropertyHandle(nn::boss::PropertyType type, nn::Handle handle)
{
    return ipc::SendPropertyHandle(m_Session, type, handle);
}

// 0x0046EF4C | nintendogs:callgraph [tier A]
nn::Result nn::boss::detail::User::StartTaskImmediate(const unsigned char* pTaskId, unsigned size)
{
    return ipc::StartTaskCommand(m_Session, ipc::COMMAND_START_TASK_IMMEDIATE, pTaskId, size);
}

// 0x0046EF8C | nintendogs:callgraph [tier A]
nn::Result nn::boss::detail::User::GetNsDataHeaderInfo(unsigned serialId, nn::boss::HeaderInfoType type, unsigned char* pValue, unsigned size)
{
    return ipc::GetNsDataHeaderInfo(m_Session, serialId, type, pValue, size);
}

// 0x0046EFE0 | tier C
nn::Result nn::boss::detail::User::GetTaskFinishHandle(nn::Handle* pEvent)
{
    return ipc::GetTaskFinishHandle(m_Session, pEvent);
}

// 0x0046F014 | tier C
nn::Result nn::boss::detail::User::GetNsDataLastUpdated(unsigned serialId, long long* pTime)
{
    return ipc::GetNsDataLastUpdated(m_Session, serialId, pTime);
}

// 0x0046F054 | tier C
nn::Result nn::boss::detail::User::GetTaskServiceStatus(const unsigned char* pTaskId, unsigned int size, nn::boss::TaskServiceStatus* pStatus)
{
    return ipc::GetTaskServiceStatus(m_Session, pTaskId, size, pStatus);
}

// 0x0046F0A0 | tier C
nn::Result nn::boss::detail::User::RegisterImmediateTask(const unsigned char* pTaskId, unsigned int size, unsigned char hasOption, unsigned char option)
{
    return ipc::RegisterTaskCommand(m_Session, ipc::COMMAND_REGISTER_IMMEDIATE_TASK, pTaskId, size, hasOption, option);
}

// 0x0046F0F4 | nintendogs:callgraph [tier A]
nn::Result nn::boss::detail::User::StartTask(const unsigned char* pTaskId, unsigned size)
{
    return ipc::StartTaskCommand(m_Session, ipc::COMMAND_START_TASK, pTaskId, size);
}

} // namespace detail
} // namespace boss
} // namespace nn
