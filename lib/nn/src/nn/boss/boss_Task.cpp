#include "nn/boss/boss_Task.h"
#include <string.h>
#include "nn/ac/CTR/CTR_Api.h"
#include "nn/boss/boss_Api.h"
#include "nn/boss/boss_TaskStatus.h"
#include "nn/boss/detail/boss_IpcManager.h"
#include "nn/boss/detail/detail_Api.h"

namespace nn {
namespace boss {
namespace {
// the codes of ChangeBossRetCodeToResult (names are ours)
const nn::boss::ResultCode CODE_NULL_STATUS = static_cast<nn::boss::ResultCode>(8);
const nn::boss::ResultCode CODE_INVALID_TASK_ID = static_cast<nn::boss::ResultCode>(26);
const nn::boss::ResultCode CODE_TASK_NOT_RUNNABLE = static_cast<nn::boss::ResultCode>(42);
// StartImmediate without a connection (status, invalid state, module 62, 72)
const bit32 RESULT_NOT_CONNECTED = 0xC8A0F848;

const size_t TASK_ID_SIZE = 8;
// the option GetStateDetail is called with by WaitFinish
const u8 WAIT_FINISH_OPTION = 128;

// the task states (3dbrew TaskStateCode; names are ours)
const u8 TASK_STATE_STOPPED = 0;
const u8 TASK_STATE_ERROR = 5;
const u8 TASK_STATE_DONE = 6;
const u8 TASK_STATE_DONE_7 = 7;
const u8 TASK_STATE_UNKNOWN = 9;

const nn::boss::TaskResultCode TASK_RESULT_UNKNOWN = static_cast<nn::boss::TaskResultCode>(2);
const nn::boss::TaskResultCode TASK_RESULT_INVALID_TASK = static_cast<nn::boss::TaskResultCode>(3);
const nn::boss::TaskServiceStatus TASK_SERVICE_STATUS_UNKNOWN = static_cast<nn::boss::TaskServiceStatus>(3);

// the size of the id for the service (with the terminating zero)
inline unsigned GetTaskIdSize(const char* pTaskId)
{
    return strlen(pTaskId) + 1;
}

inline const unsigned char* GetTaskIdBytes(const char* pTaskId)
{
    return reinterpret_cast<const unsigned char*>(pTaskId);
}
} // namespace

// 0x0046C1AC | nintendogs:bytes [tier B]
nn::Result nn::boss::Task::Initialize(const char* pTaskId)
{
    if (!detail::CheckTaskIdOk(pTaskId) || detail::strnlen(pTaskId, TASK_ID_SIZE) >= TASK_ID_SIZE ||
        detail::IsFgOnlyTaskId(pTaskId)) {
        return detail::ChangeBossRetCodeToResult(CODE_INVALID_TASK_ID);
    }
    strcpy(m_TaskId, pTaskId);
    return nn::Result();
}

// 0x0046C20C | fefates:bytes-fuzzy [tier B]
nn::Result nn::boss::Task::WaitFinish(const nn::fnd::TimeSpan& timeout)
{
    if (!detail::CheckTaskIdOk(m_TaskId)) {
        return detail::ChangeBossRetCodeToResult(CODE_INVALID_TASK_ID);
    }
    nn::Result result;
    TaskStatus status;
    bool isWaiting = true;
    while (result.IsSuccess()) {
        result = GetStateDetail(&status, false, 0, WAIT_FINISH_OPTION);
        if (result.IsFailure()) {
            break;
        }
        u8 state;
        status.GetProperty(PROPERTY_TASK_STATE_CODE, &state, sizeof(state));
        if (state == TASK_STATE_DONE || state == TASK_STATE_DONE_7) {
            result = nn::Result();
            break;
        }
        if (state == TASK_STATE_ERROR || state == TASK_STATE_STOPPED) {
            result = detail::ChangeBossRetCodeToResult(CODE_TASK_NOT_RUNNABLE);
            break;
        }
        result = WaitFinishWaitEvent(timeout);
        if (result.IsFailure() || !isWaiting) {
            break;
        }
    }
    return result;
}

// 0x0046C2FC | nintendogs:bytes [tier A]
nn::Result nn::boss::Task::UpdateCount(unsigned count)
{
    if (!detail::CheckTaskIdOk(m_TaskId)) {
        return detail::ChangeBossRetCodeToResult(CODE_INVALID_TASK_ID);
    }
    unsigned size = GetTaskIdSize(m_TaskId);
    detail::Privileged* pPrivileged;
    if (detail::GetPrivilegedIpcInstance(pPrivileged).IsSuccess()) {
        return pPrivileged->UpdateTaskCount(GetTaskIdBytes(m_TaskId), size, count);
    }
    detail::User* pUser;
    nn::Result result = detail::GetUserIpcInstance(pUser);
    if (result.IsFailure()) {
        return result;
    }
    return pUser->UpdateTaskCount(GetTaskIdBytes(m_TaskId), size, count);
}

// 0x0046C394 | fefates:bytes [tier B]
nn::Result nn::boss::Task::GetStateDetail(nn::boss::TaskStatus* pStatus, bool flag, unsigned char* pState, unsigned char option)
{
    if (pStatus == 0) {
        return detail::ChangeBossRetCodeToResult(CODE_NULL_STATUS);
    }
    if (!detail::CheckTaskIdOk(m_TaskId)) {
        return detail::ChangeBossRetCodeToResult(CODE_INVALID_TASK_ID);
    }
    unsigned size = GetTaskIdSize(m_TaskId);
    unsigned char state = 0;
    nn::Result result;
    detail::Privileged* pPrivileged;
    if (detail::GetPrivilegedIpcInstance(pPrivileged).IsSuccess()) {
        result = pPrivileged->GetTaskStatus(GetTaskIdBytes(m_TaskId), size, flag, &state, option);
    } else {
        detail::User* pUser;
        result = detail::GetUserIpcInstance(pUser);
        if (result.IsSuccess()) {
            result = pUser->GetTaskStatus(GetTaskIdBytes(m_TaskId), size, flag, &state, option);
        }
    }
    if (result.IsSuccess()) {
        result = detail::ReceiveUserTaskStatus(&pStatus->m_Info);
    }
    if (pState) {
        *pState = state;
    }
    return result;
}

// 0x0046C484 | nintendogs:bytes [tier A]
nn::Result nn::boss::Task::StartImmediate()
{
    if (!detail::CheckTaskIdOk(m_TaskId)) {
        return detail::ChangeBossRetCodeToResult(CODE_INVALID_TASK_ID);
    }
    if (!nn::ac::CTR::IsConnected()) {
        return nn::Result(RESULT_NOT_CONNECTED);
    }
    unsigned size = GetTaskIdSize(m_TaskId);
    detail::Privileged* pPrivileged;
    if (detail::GetPrivilegedIpcInstance(pPrivileged).IsSuccess()) {
        return pPrivileged->StartTaskImmediate(GetTaskIdBytes(m_TaskId), size);
    }
    detail::User* pUser;
    nn::Result result = detail::GetUserIpcInstance(pUser);
    if (result.IsFailure()) {
        return result;
    }
    return pUser->StartTaskImmediate(GetTaskIdBytes(m_TaskId), size);
}

// 0x0046C524 | fefates:bytes [tier B]
nn::boss::TaskServiceStatus nn::boss::Task::GetServiceStatus()
{
    if (!detail::CheckTaskIdOk(m_TaskId)) {
        return TASK_SERVICE_STATUS_UNKNOWN;
    }
    unsigned size = GetTaskIdSize(m_TaskId);
    nn::boss::TaskServiceStatus status = TASK_SERVICE_STATUS_UNKNOWN;
    nn::Result result;
    detail::Privileged* pPrivileged;
    if (detail::GetPrivilegedIpcInstance(pPrivileged).IsSuccess()) {
        result = pPrivileged->GetTaskServiceStatus(GetTaskIdBytes(m_TaskId), size, &status);
    } else {
        detail::User* pUser;
        if (detail::GetUserIpcInstance(pUser).IsFailure()) {
            return TASK_SERVICE_STATUS_UNKNOWN;
        }
        result = pUser->GetTaskServiceStatus(GetTaskIdBytes(m_TaskId), size, &status);
    }
    if (result.IsFailure()) {
        return TASK_SERVICE_STATUS_UNKNOWN;
    }
    return status;
}

// 0x0046C5C0 | nintendogs:bytes [tier A]
nn::Result nn::boss::Task::Start()
{
    if (!detail::CheckTaskIdOk(m_TaskId)) {
        return detail::ChangeBossRetCodeToResult(CODE_INVALID_TASK_ID);
    }
    unsigned size = GetTaskIdSize(m_TaskId);
    detail::Privileged* pPrivileged;
    if (detail::GetPrivilegedIpcInstance(pPrivileged).IsSuccess()) {
        return pPrivileged->StartTask(GetTaskIdBytes(m_TaskId), size);
    }
    detail::User* pUser;
    nn::Result result = detail::GetUserIpcInstance(pUser);
    if (result.IsFailure()) {
        return result;
    }
    return pUser->StartTask(GetTaskIdBytes(m_TaskId), size);
}

// 0x0046C64C | fefates:bytes-fuzzy [tier B]
u8 nn::boss::Task::GetState(bool flag, unsigned int* pCount, unsigned char* pDetail)
{
    if (!detail::CheckTaskIdOk(m_TaskId)) {
        return TASK_STATE_UNKNOWN;
    }
    unsigned size = GetTaskIdSize(m_TaskId);
    unsigned count = 0;
    unsigned char detailValue = 0;
    unsigned char state = TASK_STATE_UNKNOWN;
    nn::Result result;
    detail::Privileged* pPrivileged;
    if (detail::GetPrivilegedIpcInstance(pPrivileged).IsSuccess()) {
        result = pPrivileged->GetTaskState(GetTaskIdBytes(m_TaskId), size, &state, flag, &count, &detailValue);
    } else {
        detail::User* pUser;
        if (detail::GetUserIpcInstance(pUser).IsFailure()) {
            return TASK_STATE_UNKNOWN;
        }
        result = pUser->GetTaskState(GetTaskIdBytes(m_TaskId), size, &state, flag, &count, &detailValue);
    }
    if (result.IsFailure()) {
        return TASK_STATE_UNKNOWN;
    }
    if (pCount) {
        *pCount = count;
    }
    if (pDetail) {
        *pDetail = detailValue;
    }
    return state;
}

// 0x0046C738 | nintendogs:bytes [tier A]
nn::boss::TaskResultCode nn::boss::Task::GetResult(unsigned* pErrorCode, unsigned char* pDetail)
{
    if (!detail::CheckTaskIdOk(m_TaskId)) {
        return TASK_RESULT_INVALID_TASK;
    }
    unsigned size = GetTaskIdSize(m_TaskId);
    unsigned errorCode = 0;
    unsigned char detailValue = 0;
    nn::boss::TaskResultCode resultCode = TASK_RESULT_UNKNOWN;
    nn::Result result;
    detail::Privileged* pPrivileged;
    if (detail::GetPrivilegedIpcInstance(pPrivileged).IsSuccess()) {
        result = pPrivileged->GetTaskResult(GetTaskIdBytes(m_TaskId), size, &resultCode, &errorCode, &detailValue);
    } else {
        detail::User* pUser;
        if (detail::GetUserIpcInstance(pUser).IsFailure()) {
            return TASK_RESULT_UNKNOWN;
        }
        result = pUser->GetTaskResult(GetTaskIdBytes(m_TaskId), size, &resultCode, &errorCode, &detailValue);
    }
    if (result.IsFailure()) {
        return TASK_RESULT_UNKNOWN;
    }
    if (pErrorCode) {
        *pErrorCode = errorCode;
    }
    if (pDetail) {
        *pDetail = detailValue;
    }
    return resultCode;
}

// 0x0046C824 | tier C
nn::boss::Task::Task() : m_Unknown04(0), m_Unknown08(0)
{
    memset(m_TaskId, 0, sizeof(m_TaskId));
}

// 0x0046C850
// 0x0046C848 (deleting dtor)
nn::boss::Task::~Task()
{
}

} // namespace boss
} // namespace nn
