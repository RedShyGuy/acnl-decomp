#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/boss/boss_Types.h"
#include "nn/fs/fs_Types.h"

namespace nn {
namespace boss {
namespace detail {
// The commands of boss:U (3dbrew "BOSS Services") on the session of the IpcManager; the method
// names are from the symbols or after 3dbrew, the parameter names are ours.
class User
{
public:
    nn::Result InitializeSession(unsigned long long programId); // 0x0046EEA8 | nintendogs:bytes [tier A]
    nn::Result ReadNsData(unsigned serialId, long long offset, unsigned char* pBuffer, unsigned size, unsigned* pReadSize, unsigned* pVersion); // 0x0046E960 | nintendogs:callgraph [tier A]
    nn::Result DeleteNsData(unsigned serialId); // 0x0046E9C4 | tier B
    nn::Result GetErrorCode(unsigned* pErrorCode, nn::boss::TaskResultCode resultCode); // 0x0046E9F4 | nintendogs:callgraph [tier A]
    nn::Result GetTaskState(const unsigned char* pTaskId, unsigned size, unsigned char* pState, bool flag, unsigned* pCount, unsigned char* pDetail); // 0x0046EA38 | tier B
    nn::Result RegisterTask(const unsigned char* pTaskId, unsigned size, unsigned char hasOption, unsigned char option); // 0x0046EAA0 | nintendogs:callgraph [tier A]
    nn::Result SendProperty(nn::boss::PropertyType type, const unsigned char* pValue, unsigned int size); // 0x0046EAF4 | fefates:callgraph [tier A]
    nn::Result GetOptoutFlag(bool* pFlag); // 0x0046EB40 | tier C
    nn::Result GetTaskResult(const unsigned char* pTaskId, unsigned size, nn::boss::TaskResultCode* pResultCode, unsigned* pErrorCode, unsigned char* pDetail); // 0x0046EB74 | nintendogs:callgraph [tier A]
    nn::Result GetTaskStatus(const unsigned char* pTaskId, unsigned int size, bool flag, unsigned char* pState, unsigned char option); // 0x0046EBD4 | tier B
    nn::Result SetOptoutFlag(bool flag); // 0x0046EC34 | nintendogs:callgraph [tier A]
    nn::Result GetStorageInfo(unsigned* pSize); // 0x0046EC6C | nintendogs:callgraph [tier A]
    nn::Result UnregisterTask(const unsigned char* pTaskId, unsigned size, unsigned char option); // 0x0046ECA0 | nintendogs:callgraph [tier A]
    nn::Result GetNsDataIdList(unsigned filter, unsigned* pIds, unsigned capacity, unsigned short* pCount, unsigned short startIndex, unsigned nextId, unsigned short* pNextIndex); // 0x0046ECEC | nintendogs:callgraph [tier A]
    nn::Result ReceiveProperty(nn::boss::PropertyType type, unsigned char* pValue, unsigned size, unsigned* pReceivedSize); // 0x0046ED58 | tier B
    nn::Result RegisterStorage(unsigned long long extDataId, unsigned size, nn::fs::MediaType mediaType); // 0x0046EDB0 | tier C
    nn::Result UpdateTaskCount(const unsigned char* pTaskId, unsigned size, unsigned count); // 0x0046EDF0 | tier C
    nn::Result GetNsDataNewFlag(unsigned serialId, bool* pFlag); // 0x0046EE30 | tier C
    nn::Result SetNsDataNewFlag(unsigned serialId, bool flag); // 0x0046EE6C | nintendogs:callgraph [tier A]
    nn::Result UnregisterStorage(); // 0x0046EEE0 | tier C
    nn::Result SendPropertyHandle(nn::boss::PropertyType type, nn::Handle handle); // 0x0046EF08 | tier C
    nn::Result StartTaskImmediate(const unsigned char* pTaskId, unsigned size); // 0x0046EF4C | nintendogs:callgraph [tier A]
    nn::Result GetNsDataHeaderInfo(unsigned serialId, nn::boss::HeaderInfoType type, unsigned char* pValue, unsigned size); // 0x0046EF8C | nintendogs:callgraph [tier A]
    nn::Result GetTaskFinishHandle(nn::Handle* pEvent); // 0x0046EFE0 | tier C
    nn::Result GetNsDataLastUpdated(unsigned serialId, long long* pTime); // 0x0046F014 | tier C
    nn::Result GetTaskServiceStatus(const unsigned char* pTaskId, unsigned int size, nn::boss::TaskServiceStatus* pStatus); // 0x0046F054 | tier C
    nn::Result RegisterImmediateTask(const unsigned char* pTaskId, unsigned int size, unsigned char hasOption, unsigned char option); // 0x0046F0A0 | tier C
    nn::Result StartTask(const unsigned char* pTaskId, unsigned size); // 0x0046F0F4 | nintendogs:callgraph [tier A]

    nn::Handle m_Session; // 0x00
};
ASSERT_SIZE(User, 4);
} // namespace detail
} // namespace boss
} // namespace nn
