#include "nn/boss/boss_Api.h"
#include <string.h>
#include "nn/boss/boss_NsDataIdList.h"
#include "nn/boss/boss_Task.h"
#include "nn/boss/boss_TaskAction.h"
#include "nn/boss/boss_TaskPolicy.h"
#include "nn/boss/detail/boss_IpcManager.h"
#include "nn/boss/detail/detail_Api.h"
#include "nn/ndm/ndm_Api.h"
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/os/os_Tick.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace boss {
namespace {
// the codes of ChangeBossRetCodeToResult (names are ours)
const nn::boss::ResultCode CODE_NULL_POLICY = static_cast<nn::boss::ResultCode>(1);
const nn::boss::ResultCode CODE_NULL_ACTION = static_cast<nn::boss::ResultCode>(2);
const nn::boss::ResultCode CODE_INVALID_ID_LIST = static_cast<nn::boss::ResultCode>(7);
const nn::boss::ResultCode CODE_NULL_FLAG = static_cast<nn::boss::ResultCode>(12);
const nn::boss::ResultCode CODE_INVALID_TASK_ID = static_cast<nn::boss::ResultCode>(26);
const nn::boss::ResultCode CODE_INVALID_OPTION = static_cast<nn::boss::ResultCode>(27);
// the results that are written out (module 62)
const bit32 RESULT_UNKNOWN = 0xC960F84D;       // 77
const bit32 RESULT_WAIT_TIMEOUT = 0xD840F829;  // 41
const bit32 RESULT_MORE_IDS = 0xD840F845;      // 69: the list is full, more ids follow

// the option RegisterTask does not take
const u8 OPTION_INVALID = 128;
const u32 ID_LIST_CAPACITY_MAX = 0x1000;
// the extdata of the NAND storage (the high word of the extdata id)
const u32 NAND_EXTDATA_ID_HIGH = 0x48000;
// the description of the timeout of WaitSynchronization1 (3dbrew "Error codes")
const bit32 DESCRIPTION_TIMEOUT = 1022;

// the interval and the count of an immediate task
const u32 IMMEDIATE_TASK_VALUE = 1;

inline unsigned GetTaskIdSize(const char* pTaskId)
{
    return strlen(pTaskId) + 1;
}

inline const unsigned char* GetTaskIdBytes(const char* pTaskId)
{
    return reinterpret_cast<const unsigned char*>(pTaskId);
}
} // namespace

// 0x00975BAC
bool s_IsInitialized;

// 0x0046A764 | nintendogs:bytes [tier A]
nn::Result Initialize()
{
    if (s_IsInitialized) {
        return nn::Result();
    }
    s_IsInitialized = true;
    nn::Result result = nn::ndm::Initialize();
    if (result.IsFailure()) {
        return result;
    }
    return detail::InitilizeUserIpc();
}

// 0x0046AD38 | nintendogs:bytes [tier A]
nn::Result GetErrorCode(unsigned* pErrorCode, nn::boss::TaskResultCode resultCode)
{
    if (pErrorCode == 0) {
        return nn::Result(RESULT_UNKNOWN);
    }
    detail::Privileged* pPrivileged;
    if (detail::GetPrivilegedIpcInstance(pPrivileged).IsSuccess()) {
        return pPrivileged->GetErrorCode(pErrorCode, resultCode);
    }
    detail::User* pUser;
    nn::Result result = detail::GetUserIpcInstance(pUser);
    if (result.IsFailure()) {
        return result;
    }
    return pUser->GetErrorCode(pErrorCode, resultCode);
}

// 0x0046AE1C | nintendogs:bytes [tier A]
nn::Result RegisterTask(nn::boss::Task* pTask, nn::boss::TaskPolicy* pPolicy, nn::boss::TaskAction* pAction, nn::boss::TaskOption* pOption, unsigned char option)
{
    if (pTask == 0) {
        return detail::ChangeBossRetCodeToResult(CODE_INVALID_TASK_ID);
    }
    if (pPolicy == 0) {
        return detail::ChangeBossRetCodeToResult(CODE_NULL_POLICY);
    }
    if (pAction == 0) {
        return detail::ChangeBossRetCodeToResult(CODE_NULL_ACTION);
    }
    const char* pTaskId = pTask->m_TaskId;
    nn::boss::TaskActionConfig* pActionConfig = &pAction->m_Config;
    if (!detail::CheckTaskIdOk(pTaskId) || detail::IsFgOnlyTaskId(pTaskId)) {
        return detail::ChangeBossRetCodeToResult(CODE_INVALID_TASK_ID);
    }
    if (option == OPTION_INVALID) {
        return detail::ChangeBossRetCodeToResult(CODE_INVALID_OPTION);
    }
    nn::Result result = detail::VerifyTaskPolicyConfig(pPolicy);
    if (result.IsFailure()) {
        return result;
    }
    result = detail::VerifyTaskActionConfig(pActionConfig);
    if (result.IsFailure()) {
        return result;
    }
    if (pOption) {
        result = detail::VerifyTaskOptionConfig(pOption);
        if (result.IsFailure()) {
            return result;
        }
    }

    nn::boss::TaskPolicyConfig* pPolicyConfig = &pPolicy->m_Config;
    nn::boss::TaskOptionConfig* pOptionConfig = 0;
    bool hasOption = false;
    if (pOption) {
        pOptionConfig = &pOption->m_Config;
        hasOption = true;
    }
    result = detail::SendUserTaskPolicy(pPolicyConfig);
    if (result.IsFailure()) {
        return result;
    }
    result = detail::SendUserTaskAction(pActionConfig);
    if (result.IsFailure()) {
        return result;
    }
    if (pOption) {
        result = detail::SendUserTaskOption(pOptionConfig);
        if (result.IsFailure()) {
            return result;
        }
    }

    detail::Privileged* pPrivileged;
    if (detail::GetPrivilegedIpcInstance(pPrivileged).IsSuccess()) {
        return pPrivileged->RegisterTask(GetTaskIdBytes(pTaskId), GetTaskIdSize(pTaskId), hasOption, option);
    }
    detail::User* pUser;
    result = detail::GetUserIpcInstance(pUser);
    if (result.IsFailure()) {
        return result;
    }
    return pUser->RegisterTask(GetTaskIdBytes(pTaskId), GetTaskIdSize(pTaskId), hasOption, option);
}

// 0x0046B004 | tier X
nn::Result GetOptoutFlag(bool* pFlag)
{
    if (pFlag == 0) {
        return detail::ChangeBossRetCodeToResult(CODE_NULL_FLAG);
    }
    detail::Privileged* pPrivileged;
    if (detail::GetPrivilegedIpcInstance(pPrivileged).IsSuccess()) {
        return pPrivileged->GetOptoutFlag(pFlag);
    }
    detail::User* pUser;
    nn::Result result = detail::GetUserIpcInstance(pUser);
    if (result.IsFailure()) {
        return result;
    }
    return pUser->GetOptoutFlag(pFlag);
}

// 0x0046B070 | nintendogs:bytes [tier B]
nn::Result SetOptoutFlag(bool flag)
{
    detail::Privileged* pPrivileged;
    if (detail::GetPrivilegedIpcInstance(pPrivileged).IsSuccess()) {
        return pPrivileged->SetOptoutFlag(flag);
    }
    detail::User* pUser;
    nn::Result result = detail::GetUserIpcInstance(pUser);
    if (result.IsFailure()) {
        return result;
    }
    return pUser->SetOptoutFlag(flag);
}

// 0x0046B0C8 | nintendogs:bytes [tier A]
nn::Result GetStorageInfo(unsigned* pSize)
{
    unsigned size = 0;
    nn::Result result;
    detail::Privileged* pPrivileged;
    if (detail::GetPrivilegedIpcInstance(pPrivileged).IsSuccess()) {
        result = pPrivileged->GetStorageInfo(&size);
    } else {
        detail::User* pUser;
        result = detail::GetUserIpcInstance(pUser);
        if (result.IsSuccess()) {
            result = pUser->GetStorageInfo(&size);
        }
    }
    if (pSize) {
        *pSize = size;
    }
    return result;
}

// 0x0046B764 | nintendogs:bytes [tier A]
nn::Result UnregisterTask(nn::boss::Task* pTask, unsigned char option)
{
    if (pTask == 0 || !detail::CheckTaskIdOk(pTask->m_TaskId)) {
        return detail::ChangeBossRetCodeToResult(CODE_INVALID_TASK_ID);
    }
    const char* pTaskId = pTask->m_TaskId;
    detail::Privileged* pPrivileged;
    if (detail::GetPrivilegedIpcInstance(pPrivileged).IsSuccess()) {
        return pPrivileged->UnregisterTask(GetTaskIdBytes(pTaskId), GetTaskIdSize(pTaskId), option);
    }
    detail::User* pUser;
    nn::Result result = detail::GetUserIpcInstance(pUser);
    if (result.IsFailure()) {
        return result;
    }
    return pUser->UnregisterTask(GetTaskIdBytes(pTaskId), GetTaskIdSize(pTaskId), option);
}

// 0x0046B808 | nintendogs:bytes [tier A]
nn::Result GetNsDataIdList(unsigned filter, nn::boss::NsDataIdList* pList)
{
    if (pList == 0 || pList->m_Capacity == 0 || pList->m_Capacity > ID_LIST_CAPACITY_MAX) {
        return detail::ChangeBossRetCodeToResult(CODE_INVALID_ID_LIST);
    }
    nn::Result result;
    detail::Privileged* pPrivileged;
    if (detail::GetPrivilegedIpcInstance(pPrivileged).IsSuccess()) {
        result = pPrivileged->GetNsDataIdList(filter, pList->m_pIds, pList->m_Capacity, &pList->m_Count,
                                              pList->m_StartIndex, pList->m_NextId, &pList->m_StartIndex);
    } else {
        detail::User* pUser;
        result = detail::GetUserIpcInstance(pUser);
        if (result.IsSuccess()) {
            result = pUser->GetNsDataIdList(filter, pList->m_pIds, pList->m_Capacity, &pList->m_Count,
                                            pList->m_StartIndex, pList->m_NextId, &pList->m_StartIndex);
        }
    }
    if (result == RESULT_MORE_IDS) {
        pList->m_NextId = pList->GetNsDataId(pList->m_Count - 1);
    }
    return result;
}

// 0x0046B910 | nintendogs:bytes [tier B]
nn::Result RegisterStorage(unsigned extDataId, unsigned size, nn::boss::StorageType type)
{
    u32 high;
    nn::fs::MediaType mediaType;
    if (type == STORAGE_TYPE_SD) {
        high = 0;
        mediaType = nn::fs::MEDIA_TYPE_SDMC;
    } else {
        high = NAND_EXTDATA_ID_HIGH;
        mediaType = nn::fs::MEDIA_TYPE_NAND;
    }
    unsigned long long id = (static_cast<unsigned long long>(high) << 32) | extDataId;
    detail::Privileged* pPrivileged;
    if (detail::GetPrivilegedIpcInstance(pPrivileged).IsSuccess()) {
        return pPrivileged->RegisterStorage(id, size, mediaType);
    }
    detail::User* pUser;
    nn::Result result = detail::GetUserIpcInstance(pUser);
    if (result.IsFailure()) {
        return result;
    }
    return pUser->RegisterStorage(id, size, mediaType);
}

// 0x0046BADC | fefates:bytes [tier B]
nn::Result UnregisterStorage()
{
    detail::Privileged* pPrivileged;
    if (detail::GetPrivilegedIpcInstance(pPrivileged).IsSuccess()) {
        return pPrivileged->UnregisterStorage();
    }
    detail::User* pUser;
    nn::Result result = detail::GetUserIpcInstance(pUser);
    if (result.IsSuccess()) {
        result = pUser->UnregisterStorage();
    }
    return result;
}

// 0x0046BB24 | fefates:bytes [tier B]
nn::Result WaitFinishWaitEvent(const nn::fnd::TimeSpan& timeout)
{
    s64 remaining = timeout.GetNanoSeconds();
    nn::Result result;
    bool isWaiting;
    do {
        isWaiting = true;
        nn::Handle event;
        nn::Handle waitedEvent;
        bool isSignaled = false;
        s64 startTick = nn::svc::GetSystemTick();
        detail::Privileged* pPrivileged;
        if (detail::GetPrivilegedIpcInstance(pPrivileged).IsSuccess()) {
            result = pPrivileged->GetTaskFinishHandle(&event);
        } else {
            detail::User* pUser;
            result = detail::GetUserIpcInstance(pUser);
            if (result.IsSuccess()) {
                result = pUser->GetTaskFinishHandle(&event);
            }
        }
        if (result.IsSuccess()) {
            waitedEvent = event;
            nn::Result waitResult = nn::svc::WaitSynchronization1(event, remaining);
            if (waitResult.IsFailure()) {
                nn::os::CTR::detail::HandleInternalError(waitResult);
            }
            isSignaled = waitResult.GetDescription() != DESCRIPTION_TIMEOUT;
        }
        remaining -= nn::os::Tick(nn::svc::GetSystemTick() - startTick).ToTimeSpan().GetNanoSeconds();
        if (isSignaled) {
            isWaiting = false;
            result = nn::Result();
        } else if (remaining <= 0) {
            result = nn::Result(RESULT_WAIT_TIMEOUT);
            isWaiting = false;
        }
        if (waitedEvent.IsValid()) {
            nn::svc::CloseHandle(waitedEvent);
        }
    } while (isWaiting);
    return result;
}

// 0x0046BCB4 | fefates:bytes [tier B]
nn::Result RegisterImmediateTask(nn::boss::Task* pTask, nn::boss::TaskAction* pAction, nn::boss::TaskPolicy* pPolicy, nn::boss::TaskOption* pOption, unsigned char option)
{
    if (pTask == 0) {
        return detail::ChangeBossRetCodeToResult(CODE_INVALID_TASK_ID);
    }
    if (pAction == 0) {
        return detail::ChangeBossRetCodeToResult(CODE_NULL_ACTION);
    }
    const char* pTaskId = pTask->m_TaskId;
    nn::boss::TaskActionConfig* pActionConfig = &pAction->m_Config;
    if (!detail::CheckTaskIdOk(pTaskId) || !detail::IsFgOnlyTaskId(pTaskId)) {
        return detail::ChangeBossRetCodeToResult(CODE_INVALID_TASK_ID);
    }
    if (option == OPTION_INVALID) {
        return detail::ChangeBossRetCodeToResult(CODE_INVALID_OPTION);
    }
    nn::Result result = detail::VerifyTaskActionConfig(pActionConfig);
    if (result.IsFailure()) {
        return result;
    }
    if (pOption) {
        result = detail::VerifyTaskOptionConfig(pOption);
        if (result.IsFailure()) {
            return result;
        }
    }

    TaskPolicy defaultPolicy;
    if (pPolicy == 0) {
        defaultPolicy.InitializeWithSecInterval(1, 1);
        pPolicy = &defaultPolicy;
    }
    pPolicy->SetProperty(PROPERTY_COUNT, &IMMEDIATE_TASK_VALUE, sizeof(IMMEDIATE_TASK_VALUE));
    pPolicy->SetProperty(PROPERTY_INTERVAL, &IMMEDIATE_TASK_VALUE, sizeof(IMMEDIATE_TASK_VALUE));

    nn::boss::TaskPolicyConfig* pPolicyConfig = &pPolicy->m_Config;
    nn::boss::TaskOptionConfig* pOptionConfig = 0;
    bool hasOption = false;
    if (pOption) {
        pOptionConfig = &pOption->m_Config;
        hasOption = true;
    }
    result = detail::SendUserTaskPolicy(pPolicyConfig);
    if (result.IsFailure()) {
        return result;
    }
    result = detail::SendUserTaskAction(pActionConfig);
    if (result.IsFailure()) {
        return result;
    }
    if (pOption) {
        result = detail::SendUserTaskOption(pOptionConfig);
        if (result.IsFailure()) {
            return result;
        }
    }

    detail::Privileged* pPrivileged;
    if (detail::GetPrivilegedIpcInstance(pPrivileged).IsSuccess()) {
        return pPrivileged->RegisterImmediateTask(GetTaskIdBytes(pTaskId), GetTaskIdSize(pTaskId), hasOption, option);
    }
    detail::User* pUser;
    result = detail::GetUserIpcInstance(pUser);
    if (result.IsFailure()) {
        return result;
    }
    return pUser->RegisterImmediateTask(GetTaskIdBytes(pTaskId), GetTaskIdSize(pTaskId), hasOption, option);
}

// 0x0046F178 | nintendogs:bytes [tier A]
nn::Result Finalize()
{
    if (!s_IsInitialized) {
        return nn::Result();
    }
    s_IsInitialized = false;
    nn::ndm::Finalize();
    return detail::FinalizeUserIpc();
}

} // namespace boss
} // namespace nn
