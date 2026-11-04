#include "net/dNetBoss.h"

#include "nn/ac/CTR/CTR_Api.h"
#include "nn/boss/boss_Api.h"
#include "nn/boss/boss_DataStoreDownloadAction.h"
#include "nn/boss/boss_FgOnlyTask.h"
#include "nn/boss/boss_NsDataIdList.h"
#include "nn/boss/boss_NsaDownloadAction.h"
#include "nn/boss/boss_Task.h"
#include "nn/boss/boss_TaskPolicy.h"
#include "nn/fnd/fnd_TimeSpan.h"
#include "sead/seadNew.h"
#include "sead/seadPooledList.h"

namespace net {
namespace boss {
namespace {
// the results of boss (module 62) that are tested here (names are ours, the descriptions after the use)
// status, invalid state, 66: no storage is registered
const bit32 RESULT_STORAGE_NOT_FOUND = 0xC8A0F842;
// permanent, would block, 69: the list is full, more ids follow
const bit32 RESULT_MORE_IDS = 0xD840F845;
// permanent, would block, 41: the wait timed out
const bit32 RESULT_WAIT_TIMEOUT = 0xD840F829;
// status, invalid state, 72: not connected
const bit32 RESULT_NOT_CONNECTED = 0xC8A0F848;

// the result code of a task that the game treats on its own
const u8 TASK_RESULT_39 = 39;
// the state of a task that has run successfully
const u8 TASK_STATE_DONE = 6;
} // namespace

// 0x00519E78 (name is ours)
void ClearOptoutFlag()
{
    if (nn::boss::Initialize().IsFailure()) {
        return;
    }
    nn::boss::SetOptoutFlag(false);
    nn::boss::Finalize();
}

// 0x00519EA4 (name is ours)
void DeleteNsData(const NsDataParam* param, bool deletesAll)
{
    if (nn::boss::Initialize().IsFailure()) {
        return;
    }
    Helper helper;
    helper.DeleteNsData(param, deletesAll);
    nn::boss::Finalize();
}

// 0x00519ED4 (name is ours)
void UnregisterTask(const char* taskId)
{
    if (nn::boss::Initialize().IsFailure()) {
        return;
    }
    Helper helper;
    helper.UnregisterTask(taskId);
    nn::boss::Finalize();
}

// 0x00519EFC (name is ours)
bool IsNotOptedOut()
{
    bool isOptedOut = false;
    if (nn::boss::Initialize().IsSuccess()) {
        bool flag;
        if (nn::boss::GetOptoutFlag(&flag).IsSuccess()) {
            isOptedOut = flag;
        } else {
            isOptedOut = false;
        }
        nn::boss::Finalize();
    }
    return !isOptedOut;
}

// 0x00519F38 (name is ours)
bool RegisterStorage(u32 extDataId, u32 size)
{
    bool isRegistered = false;
    if (nn::boss::Initialize().IsFailure()) {
        return isRegistered;
    }
    nn::Result result = nn::boss::GetStorageInfo(NULL);
    if (result.IsSuccess()) {
        isRegistered = true;
    } else if (result == RESULT_STORAGE_NOT_FOUND &&
               nn::boss::RegisterStorage(extDataId, size, nn::boss::STORAGE_TYPE_SD).IsSuccess()) {
        isRegistered = true;
    }
    nn::boss::Finalize();
    return isRegistered;
}

// 0x00519FA0 (name is ours)
void UnregisterStorage()
{
    if (nn::boss::Initialize().IsFailure()) {
        return;
    }
    nn::boss::UnregisterStorage();
    nn::boss::Finalize();
}

// 0x00519FC4 (name is ours)
s32 CompareLastUpdated(const void* a, const void* b)
{
    const nn::fnd::DateTime& lhs = static_cast<const NsDataEntry*>(a)->lastUpdated;
    const nn::fnd::DateTime& rhs = static_cast<const NsDataEntry*>(b)->lastUpdated;
    if (lhs < rhs) {
        return -1;
    }
    return lhs != rhs;
}

// 0x00519FF0 (name is ours)
s32 net::boss::NsDataEntry::Read(u8* buffer, u32 size)
{
    if (payloadSize > size) {
        return 0;
    }
    return nsData.ReadData(buffer, size);
}

// 0x0051A00C (name is ours)
bool IsStorageRegistered()
{
    bool isRegistered = false;
    if (nn::boss::Initialize().IsSuccess()) {
        isRegistered = true;
        if (nn::boss::GetStorageInfo(NULL).IsFailure()) {
            isRegistered = false;
        }
        nn::boss::Finalize();
    }
    return isRegistered;
}

// 0x0051A04C (name is ours)
bool IsServiceAvailable(const char* taskId)
{
    bool isAvailable = false;
    if (nn::boss::Initialize().IsFailure()) {
        return isAvailable;
    }
    isAvailable = true;
    if (taskId != NULL) {
        nn::boss::Task task;
        if (task.Initialize(taskId).IsFailure() || task.GetServiceStatus() != 0) {
            isAvailable = false;
        }
    } else {
        nn::boss::FgOnlyTask task;
        if (task.GetServiceStatus() != 0) {
            isAvailable = false;
        }
    }
    nn::boss::Finalize();
    return isAvailable;
}

// 0x0051A128 (name is ours)
bool RegisterTask(const TaskParam* param)
{
    bool isRegistered = false;
    if (nn::boss::Initialize().IsFailure()) {
        return isRegistered;
    }
    if (nn::boss::GetStorageInfo(NULL).IsSuccess()) {
        Helper helper;
        if (helper.UpdateTaskCount(param->taskId, param->count)) {
            isRegistered = true;
        } else {
            nn::boss::Task task;
            if (task.Initialize(param->taskId).IsSuccess()) {
                nn::boss::TaskPolicy policy;
                if (policy.InitializeWithSecInterval(param->intervalHours * 3600, param->count).IsSuccess()) {
                    nn::boss::NsaDownloadAction action;
                    if (action.Initialize(param->url).IsSuccess() && helper.StartTask(&task, &policy, &action)) {
                        isRegistered = true;
                    }
                }
            }
        }
    }
    nn::boss::Finalize();
    return isRegistered;
}

// 0x0051A288 (name is ours)
void SetOptoutFlag()
{
    if (nn::boss::Initialize().IsFailure()) {
        return;
    }
    nn::boss::SetOptoutFlag(true);
    nn::boss::Finalize();
}

// 0x0051A2B4 (name is ours)
u8 RunImmediateTask(const ImmediateTaskParam* param, u32* errorCode)
{
    u8 result = 1;
    if (nn::boss::Initialize().IsFailure()) {
        return result;
    }
    if (nn::boss::GetStorageInfo(NULL).IsSuccess()) {
        Helper helper;
        result = helper.RunImmediateTask(param, errorCode);
    }
    nn::boss::Finalize();
    return result;
}

// 0x0051A304 (name is ours)
bool RegisterDataStoreTask(const DataStoreTaskParam* param)
{
    bool isRegistered = false;
    if (nn::boss::Initialize().IsFailure()) {
        return isRegistered;
    }
    if (nn::boss::GetStorageInfo(NULL).IsSuccess()) {
        Helper helper;
        if (helper.UpdateTaskCount(param->taskId, param->count) || helper.RegisterDataStoreTask(param)) {
            isRegistered = true;
        }
    }
    nn::boss::Finalize();
    return isRegistered;
}

// 0x0051A36C (name is ours)
void net::boss::Helper::DeleteNsData(const NsDataParam* param, bool deletesAll)
{
    if (param->heap == NULL) {
        return;
    }
    u32* ids = new (param->heap, 4) u32[param->maxNum];
    nn::boss::NsDataIdList idList(ids, param->maxNum);
    idList.Reset();
    nn::Result result = nn::boss::GetNsDataIdList(param->filter, &idList);
    if (result.IsSuccess() || result == RESULT_MORE_IDS) {
        u16 count = idList.m_Count;
        NsDataEntry entry;
        for (u16 i = 0; i < count; i++) {
            nn::boss::NsDataHeaderInfo header;
            if (entry.nsData.Initialize(idList.GetNsDataId(i)).IsSuccess() &&
                entry.nsData.GetHeaderInfo(nn::boss::HEADER_INFO_TYPE_ALL, &header, sizeof(header)).IsSuccess()) {
                entry.dataType = header.dataType;
                entry.payloadSize = header.payloadSize;
                if (entry.nsData.GetLastUpdated(&entry.lastUpdated).IsSuccess() && !deletesAll) {
                    // only the read ones
                    bool isRead;
                    if (entry.nsData.GetReadFlag(&isRead).IsFailure() || !isRead) {
                        continue;
                    }
                }
            }
            entry.nsData.Delete();
        }
    }
    delete[] ids;
}

// 0x0051A4E8 (name is ours)
bool net::boss::Helper::UpdateTaskCount(const char* taskId, u32 count)
{
    nn::boss::Task task;
    if (task.Initialize(taskId).IsFailure()) {
        return false;
    }
    if (task.UpdateCount(count).IsFailure()) {
        return false;
    }
    return true;
}

// 0x0051A57C (name is ours)
bool net::boss::Helper::StartTask(nn::boss::Task* task, nn::boss::TaskPolicy* policy, nn::boss::TaskAction* action)
{
    if (nn::boss::RegisterTask(task, policy, action, NULL, 0).IsFailure()) {
        // (the original compares further results without effect: a removed debug output)
        return false;
    }
    return task->Start().IsSuccess();
}

// 0x0051A66C (name is ours)
bool net::boss::Helper::UnregisterTask(const char* taskId)
{
    nn::boss::Task task;
    if (task.Initialize(taskId).IsFailure()) {
        return false;
    }
    if (nn::boss::UnregisterTask(&task, 0).IsFailure()) {
        return false;
    }
    return true;
}

// 0x0051A6F0 (name is ours)
u8 net::boss::Helper::RunImmediateTask(const ImmediateTaskParam* param, u32* errorCode)
{
    if (errorCode != NULL) {
        *errorCode = 0;
    }
    nn::boss::FgOnlyTask task;
    nn::boss::NsaDownloadAction action;
    if (action.Initialize(param->url).IsFailure() || action.SetApInfoType(4).IsFailure()) {
        return 1;
    }
    if (nn::boss::RegisterImmediateTask(&task, &action, NULL, NULL, 0).IsFailure()) {
        return 1;
    }
    nn::Result result = task.StartImmediate();
    if (result.IsFailure()) {
        if (result == RESULT_NOT_CONNECTED && errorCode != NULL) {
            nn::ac::CTR::GetLastErrorCode(errorCode);
        }
        return 1;
    }
    result = task.WaitFinish(nn::fnd::TimeSpan::FromNanoSeconds(param->timeoutSeconds * 1000000000ULL));
    if (result.IsFailure()) {
        if (result == RESULT_WAIT_TIMEOUT) {
            return 2;
        }
        return 1;
    }
    if (task.GetState(false, NULL, NULL) == TASK_STATE_DONE) {
        return 0;
    }
    if (task.GetResult(NULL, NULL) == TASK_RESULT_39) {
        return 3;
    }
    if (errorCode != NULL) {
        nn::boss::GetErrorCode(errorCode, task.GetResult(NULL, NULL));
    }
    return 1;
}

// 0x0051A970 (name is ours)
bool net::boss::Helper::RegisterDataStoreTask(const DataStoreTaskParam* param)
{
    nn::boss::Task task;
    if (task.Initialize(param->taskId).IsFailure()) {
        return false;
    }
    nn::boss::TaskPolicy policy;
    if (policy.InitializeWithSecInterval(param->intervalHours * 3600, param->count).IsFailure()) {
        return false;
    }
    u8 permission = 0;
    if (policy.SetProperty(nn::boss::PROPERTY_PERMISSION, &permission, sizeof(permission)).IsFailure()) {
        return false;
    }
    nn::boss::DataStoreDownloadAction action;
    if (action.Initialize(param->gameId, param->key).IsFailure() || action.ClearData().IsFailure()) {
        return false;
    }
    return StartTask(&task, &policy, &action);
}

// 0x0051AAC0 (name is ours)
u32 net::boss::Helper::ProcessNsData(NsDataFunction function, const NsDataParam* param)
{
    if (param->heap == NULL) {
        return 0;
    }
    u32 processedNum = 0;
    u32* ids = new (param->heap, 4) u32[param->maxNum];
    nn::boss::NsDataIdList idList(ids, param->maxNum);
    idList.Reset();
    nn::Result result = nn::boss::GetNsDataIdList(param->filter, &idList);
    if (result.IsSuccess() || result == RESULT_MORE_IDS) {
        u32 count = idList.m_Count;
        if (count != 0) {
            sead::PooledList<NsDataEntry> entries;
            entries.allocBuffer(count, param->heap, 8);
            for (u32 i = 0; i < count; i++) {
                NsDataEntry* entry = entries.birthBack();
                nn::boss::NsDataHeaderInfo header;
                if (entry->nsData.Initialize(idList.GetNsDataId(i)).IsSuccess() &&
                    entry->nsData.GetHeaderInfo(nn::boss::HEADER_INFO_TYPE_ALL, &header, sizeof(header)).IsSuccess()) {
                    entry->dataType = header.dataType;
                    entry->payloadSize = header.payloadSize;
                    if (entry->nsData.GetLastUpdated(&entry->lastUpdated).IsSuccess()) {
                        continue;
                    }
                }
                entry->nsData.Delete();
                entries.erase(entry);
            }
            entries.sort(CompareLastUpdated);
            for (NsDataEntry* entry = entries.front(); entry != entries.endObj(); entry = entries.next(entry)) {
                processedNum++;
                if (function(entry)) {
                    entry->nsData.Delete();
                } else {
                    entry->nsData.SetReadFlag(true);
                }
            }
        }
    }
    delete[] ids;
    return processedNum;
}

// 0x0051ADEC (name is ours)
u32 ProcessNsData(NsDataFunction function, const NsDataParam* param)
{
    u32 processedNum = 0;
    if (nn::boss::Initialize().IsFailure()) {
        return processedNum;
    }
    Helper helper;
    processedNum = helper.ProcessNsData(function, param);
    nn::boss::Finalize();
    return processedNum;
}
} // namespace boss
} // namespace net
