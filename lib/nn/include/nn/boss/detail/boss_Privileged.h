#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/boss/boss_Types.h"
#include "nn/fs/fs_Types.h"

namespace nn {
namespace boss {
namespace detail {
// The commands of boss:P (the commands of boss:U and the ones for the data of other programs,
// 3dbrew "BOSS Services"); the copies of the boss:U commands are the same code as in User. This
// program never opens boss:P (GetPrivilegedIpcInstance fails and the callers use User).
class Privileged
{
public:
    nn::Result ReadNsData(unsigned serialId, long long offset, unsigned char* pBuffer, unsigned size, unsigned* pReadSize, unsigned* pVersion); // 0x0046D0D4 | nintendogs:bytes [tier A]
    nn::Result DeleteNsData(unsigned serialId); // 0x0046D138 | tier B
    nn::Result GetErrorCode(unsigned* pErrorCode, nn::boss::TaskResultCode resultCode); // 0x0046D168 | nintendogs:bytes [tier A]
    nn::Result GetTaskState(const unsigned char* pTaskId, unsigned size, unsigned char* pState, bool flag, unsigned* pCount, unsigned char* pDetail); // 0x0046D1AC | tier B
    nn::Result RegisterTask(const unsigned char* pTaskId, unsigned size, unsigned char hasOption, unsigned char option); // 0x0046D214 | nintendogs:bytes [tier A]
    nn::Result SendProperty(nn::boss::PropertyType type, const unsigned char* pValue, unsigned int size); // 0x0046D268 | tier C
    nn::Result GetOptoutFlag(bool* pFlag); // 0x0046D2B4 | tier C
    nn::Result GetTaskResult(const unsigned char* pTaskId, unsigned size, nn::boss::TaskResultCode* pResultCode, unsigned* pErrorCode, unsigned char* pDetail); // 0x0046D2E8 | nintendogs:bytes [tier A]
    // (symbols.json names 0x0046D348 User::GetTaskStatus; it is the copy in Privileged)
    nn::Result GetTaskStatus(const unsigned char* pTaskId, unsigned int size, bool flag, unsigned char* pState, unsigned char option); // 0x0046D348 (name after the copy in User)
    nn::Result SetOptoutFlag(bool flag); // 0x0046D3A8 | tier C
    nn::Result GetStorageInfo(unsigned* pSize); // 0x0046D3E0 | nintendogs:bytes [tier A]
    nn::Result UnregisterTask(const unsigned char* pTaskId, unsigned size, unsigned char option); // 0x0046D414 | nintendogs:bytes [tier A]
    nn::Result GetNsDataIdList(unsigned filter, unsigned* pIds, unsigned capacity, unsigned short* pCount, unsigned short startIndex, unsigned nextId, unsigned short* pNextIndex); // 0x0046D460 | nintendogs:bytes [tier A]
    nn::Result ReceiveProperty(nn::boss::PropertyType type, unsigned char* pValue, unsigned size, unsigned* pReceivedSize); // 0x0046D4CC | tier B
    nn::Result RegisterStorage(unsigned long long extDataId, unsigned size, nn::fs::MediaType mediaType); // 0x0046D524 | tier C
    nn::Result UpdateTaskCount(const unsigned char* pTaskId, unsigned size, unsigned count); // 0x0046D564 | tier C
    nn::Result GetNsDataNewFlag(unsigned serialId, bool* pFlag); // 0x0046D5A4 | tier C
    nn::Result SetNsDataNewFlag(unsigned serialId, bool flag); // 0x0046D5E0 | nintendogs:bytes [tier A]
    nn::Result UnregisterStorage(); // 0x0046D61C | tier C
    nn::Result SendPropertyHandle(nn::boss::PropertyType type, nn::Handle handle); // 0x0046D644 | tier C
    nn::Result StartTaskImmediate(const unsigned char* pTaskId, unsigned size); // 0x0046D688 | nintendogs:bytes [tier A]
    nn::Result GetNsDataHeaderInfo(unsigned serialId, nn::boss::HeaderInfoType type, unsigned char* pValue, unsigned size); // 0x0046D6C8 | nintendogs:bytes [tier A]
    nn::Result GetTaskFinishHandle(nn::Handle* pEvent); // 0x0046D71C | tier C
    nn::Result GetNsDataLastUpdated(unsigned serialId, long long* pTime); // 0x0046D750 | tier C
    nn::Result GetTaskServiceStatus(const unsigned char* pTaskId, unsigned int size, nn::boss::TaskServiceStatus* pStatus); // 0x0046D790 | tier C
    nn::Result ReadNsDataPrivileged(unsigned long long programId, unsigned serialId, long long offset, unsigned char* pBuffer, unsigned size, unsigned* pReadSize, unsigned* pVersion); // 0x0046D7DC | nintendogs:bytes [tier A]
    nn::Result RegisterImmediateTask(const unsigned char* pTaskId, unsigned int size, unsigned char hasOption, unsigned char option); // 0x0046D850 | tier C
    nn::Result DeleteNsDataPrivileged(unsigned long long programId, unsigned int serialId); // 0x0046D8A4 | tier B
    nn::Result GetNsDataNewFlagPrivileged(unsigned long long programId, unsigned serialId, bool* pFlag); // 0x0046D8DC | nintendogs:bytes [tier A]
    nn::Result SetNsDataNewFlagPrivileged(unsigned long long programId, unsigned serialId, bool flag); // 0x0046D920 | nintendogs:bytes [tier A]
    nn::Result GetNsDataHeaderInfoPrivileged(unsigned long long programId, unsigned serialId, nn::boss::HeaderInfoType type, unsigned char* pValue, unsigned size); // 0x0046D960 | nintendogs:bytes [tier A]
    nn::Result GetNsDataLastUpdatedPrivileged(unsigned long long programId, unsigned serialId, long long* pTime); // 0x0046D9BC | tier B
    nn::Result StartTask(const unsigned char* pTaskId, unsigned size); // 0x0046DA04 | nintendogs:bytes [tier A]

    nn::Handle m_Session; // 0x00
};
ASSERT_SIZE(Privileged, 4);
} // namespace detail
} // namespace boss
} // namespace nn
