#pragma once

#include "decomp.h"
#include "nn/boss/boss_NsData.h"
#include "nn/fnd/fnd_DateTime.h"

namespace nn {
namespace boss {
class Task;
class TaskAction;
class TaskPolicy;
} // namespace boss
} // namespace nn

namespace sead {
class Heap;
} // namespace sead

namespace net {
// SpotPass (nn::boss) of the game: each function opens the boss library for itself (Initialize/Finalize).
// The namespace, types, functions and members are ours (no RTTI or file name in the binary).
namespace boss {
// a downloaded data item (sorted: oldest first)
struct NsDataEntry
{
    NsDataEntry() : payloadSize(0), dataType(0) {}

    // the data into buffer if it fits (the bytes read, 0 if it is too large)
    s32 Read(u8* buffer, u32 size); // 0x00519FF0

    nn::fnd::DateTime lastUpdated; // 0x00
    nn::boss::NsData nsData;       // 0x08
    u32 payloadSize;               // 0x30
    u16 dataType;                  // 0x34
};
ASSERT_SIZE(NsDataEntry, 0x38);

// which data items
struct NsDataParam
{
    sead::Heap* heap; // 0x0 (for the id list)
    u32 filter;       // 0x4 (of GetNsDataIdList)
    u16 maxNum;       // 0x8
};

// a task that downloads an NSA archive every intervalHours hours
struct TaskParam
{
    const char* taskId;  // 0x0
    const char* url;     // 0x4
    u32 intervalHours;   // 0x8
    u32 count;           // 0xC
};

// a task that downloads from the DataStore server
struct DataStoreTaskParam
{
    const char* taskId;  // 0x00
    u32 intervalHours;   // 0x04
    u32 count;           // 0x08
    u32 gameId;          // 0x0C
    const wchar_t* key;  // 0x10
};

// a download at once
struct ImmediateTaskParam
{
    const char* taskId;  // 0x0 (not used)
    const char* url;     // 0x4
    u32 timeoutSeconds;  // 0x8
};

// true: the data item is deleted, false: it is marked as read
typedef bool (*NsDataFunction)(NsDataEntry* entry);

// the functions with the boss library open (a local object without data in the original)
class Helper
{
public:
    // deletes the broken data items and the read ones (all with deletesAll)
    void DeleteNsData(const NsDataParam* param, bool deletesAll); // 0x0051A36C
    bool UpdateTaskCount(const char* taskId, u32 count); // 0x0051A4E8
    bool StartTask(nn::boss::Task* task, nn::boss::TaskPolicy* policy, nn::boss::TaskAction* action); // 0x0051A57C
    bool UnregisterTask(const char* taskId); // 0x0051A66C
    // 0 done, 1 failed, 2 timeout, 3 result 39
    u8 RunImmediateTask(const ImmediateTaskParam* param, u32* errorCode); // 0x0051A6F0
    bool RegisterDataStoreTask(const DataStoreTaskParam* param); // 0x0051A970
    // passes the data items, oldest first, to function; the number of items
    u32 ProcessNsData(NsDataFunction function, const NsDataParam* param); // 0x0051AAC0
};

void ClearOptoutFlag(); // 0x00519E78
void DeleteNsData(const NsDataParam* param, bool deletesAll); // 0x00519EA4
void UnregisterTask(const char* taskId); // 0x00519ED4
bool IsNotOptedOut(); // 0x00519EFC
bool RegisterStorage(u32 extDataId, u32 size); // 0x00519F38
void UnregisterStorage(); // 0x00519FA0
// the order of the data items by their date
s32 CompareLastUpdated(const void* a, const void* b); // 0x00519FC4
bool IsStorageRegistered(); // 0x0051A00C
// whether the service can run the task (the immediate task without taskId)
bool IsServiceAvailable(const char* taskId); // 0x0051A04C
bool RegisterTask(const TaskParam* param); // 0x0051A128
void SetOptoutFlag(); // 0x0051A288
// 1 if the storage is missing, else as Helper::RunImmediateTask
u8 RunImmediateTask(const ImmediateTaskParam* param, u32* errorCode); // 0x0051A2B4
bool RegisterDataStoreTask(const DataStoreTaskParam* param); // 0x0051A304
u32 ProcessNsData(NsDataFunction function, const NsDataParam* param); // 0x0051ADEC
} // namespace boss
} // namespace net
