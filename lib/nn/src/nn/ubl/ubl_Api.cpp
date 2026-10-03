#include "nn/ubl/ubl_Api.h"
#include "nn/cfg/CTR/CTR_Api.h"
#include "nn/fnd/fnd_DateTime.h"
#include "nn/fnd/fnd_TimeSpan.h"
#include "nn/fs/detail/detail_Api.h"
#include "nn/fs/fs_Api.h"
#include "nn/fs/fs_FileInputStream.h"
#include "nn/fs/fs_FileOutputStream.h"
#include "nn/os/os_Thread.h"

namespace nn {
namespace ubl {
namespace {

// results (module 58); the names are ours
const bit32 RESULT_ALREADY_INITIALIZED = 0xD820EBF9;    // permanent, nothing happened, 1017
const bit32 RESULT_NOT_INITIALIZED = 0xD820EBF8;        // permanent, nothing happened, 1016
const bit32 RESULT_OWN_ID = 0xD8E0EBE8;                 // permanent, invalid argument, 1000: the console's own id
const bit32 RESULT_MOUNT_FAILED = 0xF960EBFF;           // fatal, internal, 1023

// the shared extra save data of the list
const u32 SAVE_ID = 0xF000000B;
const s32 ENTRY_COUNT = 1000;
// FS results that mean "busy, try again" (descriptions 230 to 339)
const s32 MAX_RETRIES = 100;

// the time an id was added (seconds in steps of 10); 7 in the seconds with everything else 0
// marks an empty entry
union Date
{
    u32 raw;
    struct
    {
        u32 second10 : 3;
        u32 minute : 6;
        u32 hour : 5;
        u32 day : 5;
        u32 month : 4;
        u32 year : 9;   // - 1900
    } fields;
};
const u32 EMPTY_DATE = 7;

typedef u64 u64_align4 __attribute__((aligned(4)));

struct Entry
{
    u64_align4 id;
    Date date;
};
ASSERT_SIZE(Entry, 12);

// 0x008A3830
const wchar_t* const s_FilePath = L"ubl_:/ubll.lst";

} // namespace

// the globals of this file (ARMCC addresses them from 0x00975FC0; names are ours)
// 0x00975FC0
bool s_IsInitialized;
// 0x00975FC4
const char* s_MountName = "ubl_:";
// 0x00975FC8
nn::fnd::TimeSpan s_RetryInterval = nn::fnd::TimeSpan::FromMilliSeconds(10);
// 0x00AE2050
Entry s_Entries[ENTRY_COUNT];

namespace {

// the FS service is busy (inline)
bool IsRetryable(nn::Result result)
{
    return result.GetModule() == 17 && result.GetDescription() >= 230 && result.GetDescription() < 340;
}

// 0x004674BC (name is ours)
// writes the list; the file is made again if it is missing or has the wrong size
DECOMP_NOINLINE nn::Result Save()
{
    nn::fs::FileOutputStream stream;
    if (nn::fs::MountSharedExtSaveData(s_MountName, SAVE_ID).IsFailure()) {
        return nn::Result(RESULT_MOUNT_FAILED);
    }
    nn::Result result;
    s32 retries = 0;
    for (;;) {
        result = stream.TryInitialize(s_FilePath);
        if (result.IsFailure()) {
            if (IsRetryable(result)) {
                if (++retries >= MAX_RETRIES) {
                    break;
                }
                nn::os::Thread::SleepImpl(s_RetryInterval);
                continue;
            }
        } else {
            s64 size = 0;
            if (stream.TryGetSize(&size).IsSuccess() && size == sizeof(s_Entries)) {
                s32 writtenSize;
                result = stream.TryWrite(&writtenSize, s_Entries, sizeof(s_Entries), true);
                stream.Finalize();
                break;
            }
            stream.Finalize();
        }
        nn::fs::detail::GetGlobalFileSystemBase()->TryDeleteFile(s_FilePath);
        result = nn::fs::detail::GetGlobalFileSystemBase()->TryCreateFile(s_FilePath, sizeof(s_Entries));
        if (result.IsFailure()) {
            break;
        }
    }
    nn::fs::Unmount(s_MountName);
    return result;
}

// 0x00467724 (name is ours)
// reads the list; an empty list if it cannot be read
DECOMP_NOINLINE nn::Result Load(bool unused)
{
    nn::fs::FileInputStream stream;
    bool isInvalid = true;
    if (nn::fs::MountSharedExtSaveData(s_MountName, SAVE_ID).IsFailure()) {
        return nn::Result(RESULT_MOUNT_FAILED);
    }
    s32 retries = 0;
    for (;;) {
        nn::Result result = stream.TryInitialize(s_FilePath);
        if (result.IsSuccess()) {
            s64 size;
            if (stream.TryGetSize(&size).IsSuccess() && size == sizeof(s_Entries)) {
                s32 readSize;
                if (stream.TryRead(&readSize, s_Entries, sizeof(s_Entries)).IsSuccess()) {
                    isInvalid = false;
                }
            }
            stream.Finalize();
            break;
        }
        if (!IsRetryable(result) || ++retries >= MAX_RETRIES) {
            break;
        }
        nn::os::Thread::SleepImpl(s_RetryInterval);
    }
    if (isInvalid) {
        for (s32 i = 0; i < ENTRY_COUNT; i++) {
            s_Entries[i].id = 0;
            s_Entries[i].date.raw = EMPTY_DATE;
        }
    }
    nn::fs::Unmount(s_MountName);
    return nn::Result();
}

// the entry of id, -1 if there is none (inline)
s32 FindEntry(u64 id)
{
    for (s32 i = 0; i < ENTRY_COUNT; i++) {
        if (s_Entries[i].id == id && s_Entries[i].date.raw != EMPTY_DATE) {
            return i;
        }
    }
    return -1;
}

} // namespace

// 0x00467460 (name is ours)
nn::Result Initialize()
{
    if (s_IsInitialized) {
        return nn::Result(RESULT_ALREADY_INITIALIZED);
    }
    nn::fs::Initialize();
    nn::cfg::CTR::Initialize();
    nn::Result result = Load(true);
    if (result.IsFailure()) {
        nn::cfg::CTR::Finalize();
        return result;
    }
    s_IsInitialized = true;
    return nn::Result();
}

// 0x0012A9B4 (name is ours)
void Finalize()
{
    if (s_IsInitialized) {
        nn::cfg::CTR::Finalize();
        s_IsInitialized = false;
    }
}

// 0x00467950 (name is ours)
nn::Result Add(u64 id, const nn::fnd::DateTime& date)
{
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    if (id == nn::cfg::CTR::GetTransferableId(0)) {
        return nn::Result(RESULT_OWN_ID);
    }
    s32 index = FindEntry(id);
    if (index < 0) {
        // an empty entry, else the oldest one
        for (index = 0; index < ENTRY_COUNT; index++) {
            if (s_Entries[index].date.raw == EMPTY_DATE) {
                break;
            }
        }
        if (index == ENTRY_COUNT) {
            u32 oldest = 0xFFFFFFFF;
            index = 0;
            for (s32 i = 0; i < ENTRY_COUNT; i++) {
                if (s_Entries[i].date.raw < oldest) {
                    index = i;
                    oldest = s_Entries[i].date.raw;
                }
            }
        }
        s_Entries[index].id = id;
    }
    Entry& entry = s_Entries[index];
    entry.date.fields.year = date.GetYear() - 1900;
    entry.date.fields.month = date.GetMonth();
    entry.date.fields.day = date.GetDay();
    entry.date.fields.hour = date.GetHour();
    entry.date.fields.minute = date.GetMinute();
    entry.date.fields.second10 = date.GetSecond() / 10;
    return Save();
}

// 0x00467B80 | nintendogs:bytes [tier B]
bool IsExist(u64 id, u32 unknown0, u64 unknown1)
{
    if (!s_IsInitialized) {
        return false;
    }
    bool isExist = false;
    for (u32 i = 0; i < ENTRY_COUNT; i++) {
        if (s_Entries[i].id == id && s_Entries[i].date.raw != EMPTY_DATE) {
            isExist = true;
            break;
        }
    }
    return isExist;
}

} // namespace ubl
} // namespace nn
