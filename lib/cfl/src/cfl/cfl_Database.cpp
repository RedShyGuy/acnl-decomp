#include "cfl/cfl_Database.h"
#include "cfl/cfl_Fs.h"
#include "cfl/cfl_Net.h"
#include "cfl/cfl_RecentDB.h"
#include "nn/err/CTR/CTR_Api.h"
#include "nn/fs/fs_Api.h"
#include "nn/fs/fs_FileInputStream.h"

#include <cstring>

using namespace cfl::detail;

// the globals of the database (names are ours)
// 0x009761AC
bool s_EnableSpecialMii = false;
// 0x009761AD
bool s_DatabaseLoaded = false;
// 0x009761AE
bool s_NetMagicN = false;
// 0x009761AF
bool s_DatabaseMounted = false;
// 0x009761B0
bool s_Restoring = false;
// 0x009761B4
CFLiDevice* s_pDatabaseDevice = NULL;
// 0x009761B8
CFLiReceivedList* s_pReceivedList = NULL;
// 0x009761BC
CFLiDatabase* s_pDatabase = NULL;
// 0x009761C0
u8* s_pMemoryDatabase = NULL;

// the resource archive (names are ours)
// 0x00976288
bool s_NandContentMounted = false;
// 0x0097628C
u32 s_ResFileSize = 0;
// 0x00976290
CFLiDevice* s_pResourceDevice = NULL;

// 0x00AE5AC0
CFLiOfficialData s_OfficialData;

// how the model axes map to the ones of the program (names are ours)
// 0x009761D8
u8 s_Axis[8] = {0, 0, 0, 1, 2, 0, 0, 0};
// 0x008A3CE4
const u8 s_DefaultAxis[8] = {0, 0, 0, 1, 2, 0, 0, 0};

namespace {
const char* const DATABASE_ARCHIVE = "$CFLDB:";
const char* const DATABASE_PATH = "$CFLDB:/CFL_DB.dat";
const char* const OLD_DATABASE_PATH = "$CFLDB:/CFL_OldDB.dat";
const char* const RESOURCE_ARCHIVE = "$CFLRES:";
const char* const RESOURCE_PATH = "$CFLRES:/CFL_Res.dat";

const u32 MAGIC_CFOG = 0x474F4643;
const u32 MAGIC_CFHE = 0x45484643;
const u32 MAGIC_CFRA = 0x41524643;

// the content of the resource archive and the buffer to mount it (names are ours)
const u64 RESOURCE_CONTENT_ID = 0x0004009B00010202;
// 0x00AE5D40
u8 s_ResourceMountBuffer[0x80];

// marks an asynchronous operation (inline; name is ours)
inline void BeginWorking()
{
    if (CFL_IsAvailable()) {
        CFLi_GetManager()->working = true;
        CFLi_GetManager()->status = CFL_RESULT_BUSY;
    }
}

// starts a sequence step on a device (inline; name is ours)
inline void StartStep(CFLiDevice* device, CFLiSequenceStep step, CFLiSequenceCallback callback)
{
    device->callback = callback;
    device->state = 0;
    device->offset = 0;
    device->buffer = NULL;
    device->size = 0;
    device->_10 = false;
    device->step = step;
    step(device, 0);
}

// "CFOG" and "CFHE" of an empty database (inline; name is ours)
inline void InitializeOfficial(CFLiDatabaseOfficial* official)
{
    official->magic = MAGIC_CFOG;
    official->_4_9 = 0;
    official->_4_8 = 1;
    official->version = 0;
    std::memset(official->owned, 0, sizeof(official->owned));
    official->hiddenMagic = MAGIC_CFHE;
    official->hiddenHead = 0xFFFF;
    official->hiddenTail = 0xFFFF;
    for (int i = 0; i < CFL_DATABASE_HIDDEN_NUM; i++) {
        CFLiHiddenNode* node = &official->hidden[i];
        std::memset(node, 0, sizeof(*node));
        node->prev = 0x7FFF;
        node->next = 0x7FFF;
    }
    std::memset(official->_C810, 0, sizeof(official->_C810));
    CFLi_PutCRC16(0, official, sizeof(*official));
}

// the CRC16 of the database in steps of 1 MiB (two copies in the original: 0x00166B60 and 0x00169AA8)
inline void ComputeCRCStepImpl(CFLiDatabase* db)
{
    for (;;) {
        u32 size = db->crcSize - db->crcPosition;
        if (size > 0x100000) {
            size = 0x100000;
        }
        db->crc = CFLi_GetCRC16(db->crc, db->crcData + db->crcPosition, size);
        db->crcPosition += size;
        if (db->crcPosition >= db->crcSize) {
            break;
        }
    }
    db->crcCallback(db->crcContext, __builtin_bswap16(db->crc));
}
} // namespace

// 0x00166B60 (name is ours)
// 0x00169AA8 (copy)
DECOMP_NOINLINE void CFLi_ComputeCRCStep(CFLiDatabase*)
{
    ComputeCRCStepImpl(s_pDatabase);
}

// computes the CRC16 of "CFOG"/"CFHE" for writing (name is ours)
// 0x0017CFB4 (name is ours)
DECOMP_NOINLINE void CFLi_StartComputeCRC(void* context, CFLiCRCCallback callback)
{
    CFLiDatabase* db = s_pDatabase;
    if (!CFLi_GetWorking()) {
        BeginWorking();
    }
    db->official.crc = 0;
    db->crcData = reinterpret_cast<const u8*>(db);
    db->crcSize = sizeof(CFLiDatabaseOfficial);
    db->crcPosition = 0;
    db->crc = 0;
    db->crcCallback = callback;
    db->crcContext = context;
    CFLi_ComputeCRCStep(db);
}

// 0x001554F0 (name is ours)
void CFLi_InitializeModelAxis()
{
    std::memcpy(s_Axis, s_DefaultAxis, sizeof(s_Axis));
}

// 0x00181498 (name is ours)
void CFLi_DrawDoneDefault()
{
    // empty (in the original too)
}

// 0x00155648 (name is ours)
DECOMP_NOINLINE bool CFLi_IsNandSharedExtMounted()
{
    return s_DatabaseMounted;
}

// 0x001555B0 (name is ours)
DECOMP_NOINLINE bool CFLi_IsDatabaseLoaded()
{
    return s_DatabaseLoaded;
}

// 0x00155B94 (name is ours)
DECOMP_NOINLINE s32 CFLi_MountNandContent()
{
    if (s_NandContentMounted) {
        return CFL_RESULT_SUCCESS;
    }
    size_t size = nn::fs::GetContentRequiredMemorySize(nn::fs::MEDIA_TYPE_NAND, RESOURCE_CONTENT_ID, 0, 2, 2);
    if (size > sizeof(s_ResourceMountBuffer)) {
        nndbgPanic();
    }
    nn::Result result = nn::fs::MountContent(RESOURCE_ARCHIVE, nn::fs::MEDIA_TYPE_NAND, RESOURCE_CONTENT_ID, 0, 2, 2,
                                             s_ResourceMountBuffer, sizeof(s_ResourceMountBuffer), false);
    if (result.IsSuccess()) {
        s_NandContentMounted = true;
        return CFL_RESULT_SUCCESS;
    }
    nn::err::CTR::ThrowFatalErrAllIfFailure(result);
    return CFL_RESULT_FILE_ERROR;
}

// 0x00154568 (name is ours)
DECOMP_NOINLINE s32 CFLi_DeleteFile(const char* path)
{
    nn::Result result = TryDeleteFile(path);
    if (result.IsSuccess() || IsNotFound(result)) {
        return CFL_RESULT_SUCCESS;
    }
    return CFL_RESULT_FILE_ERROR;
}

extern "C" {
// 0x0015463C | nintendogs:bytes [tier B]
void CFLi_EnableSpecialMii(u32 key)
{
    s_EnableSpecialMii = key == 0x13DE28CF;
}

// 0x0013A3EC | nintendogs:callseq-callee [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE bool CFLi_FileExists(const char* path)
{
    nn::fs::FileInputStream stream;
    nn::Result result = stream.FileBase::TryInitialize(path, nn::fs::OPEN_MODE_READ);
    bool exists = !IsNotFound(result);
    if (result.IsSuccess()) {
        stream.Finalize();
    }
    return exists;
}

// 0x00155C64 | nintendogs:callseq-callee [tier C]
DECOMP_NOINLINE s32 CFLi_MountNandSharedExt()
{
    if (s_DatabaseMounted) {
        return CFL_RESULT_SUCCESS;
    }
    nn::Result result = nn::fs::MountSharedExtSaveData(DATABASE_ARCHIVE, 0xF000000B);
    if (result.IsSuccess()) {
        s_DatabaseMounted = true;
        return CFL_RESULT_SUCCESS;
    }
    nn::err::CTR::ThrowFatalErrAllIfFailure(result);
    return CFL_RESULT_FILE_ERROR;
}

// 0x00135F18 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
void CFLi_UnmountNandContent()
{
    if (s_NandContentMounted) {
        nn::fs::Unmount(RESOURCE_ARCHIVE);
        s_NandContentMounted = false;
    }
}

// 0x00155504 | nintendogs:bytes [tier B]
void CFLi_InitializeResLoader()
{
    CFLiResLoader* loader = CFL_IsAvailable() ? &CFLi_GetManager()->resLoader : NULL;
    for (int i = 0; i < 20; i++) {
        loader->sections[i].num = 0;
        loader->sections[i].size = 0;
        loader->sections[i].offset = 0;
    }
    loader->version = 0;
    loader->_B0 = 0;
    loader->_B4 = 0;
    loader->cached = false;
    loader->size = 0;
    loader->data = NULL;
}

// 0x00155564 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
void CFLi_InitializeResourceDevice(void** buffer, u32* bufferSize)
{
    s_pResourceDevice = CFLi_InitializeDevice(buffer, bufferSize, RESOURCE_PATH, CFLi_GetDeviceInterfaceFile());
}

// 0x00135EF8 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
void CFLi_FinalizeResourceDevice()
{
    CFLi_DestroyDevice(s_pResourceDevice);
    s_pResourceDevice = NULL;
}

// 0x00155240 | nintendogs:callseq [tier C]
void CFLi_InitializeDatabase(void** buffer, u32* bufferSize)
{
    CFLi_MountNandSharedExt();
    const CFLiDeviceInterface* iface = CFLi_GetDeviceInterfaceFile();
    if (s_pMemoryDatabase != NULL) {
        iface = &s_MemoryDeviceInterface;
    }
    s_pDatabaseDevice = CFLi_InitializeDevice(buffer, bufferSize, DATABASE_PATH, iface);

    CFLiDatabase* db = static_cast<CFLiDatabase*>(CFLi_AllocateAlignedBuffer(buffer, bufferSize, sizeof(CFLiDatabase)));
    s_pDatabase = db;
    db->written = false;
    db->restored = false;
    db->writeDone = NULL;
    db->formatFailed = NULL;
    db->bootNext = NULL;
    InitializeOfficial(&db->official);

    CFLiReceivedList* recent =
        static_cast<CFLiReceivedList*>(CFLi_AllocateAlignedBuffer(buffer, bufferSize, sizeof(CFLiReceivedList)));
    s_pReceivedList = recent;
    recent->mirror = NULL;
    recent->hasMirror = false;
    recent->buffer = NULL;
    recent->written = -1;
    recent->_2A1C = -1;
    recent->_2A20 = 0;
    recent->_2A24 = 0;
    recent->_2A28 = 0;

    std::memset(&db->recent, 0, sizeof(db->recent));
    db->recent.magic = MAGIC_CFRA;

    s_OfficialData.num = 6;
    for (int i = 0; i < 6; i++) {
        CFLiMiiDataOfficial* data = &s_OfficialData.data[i];
        CFLi_GetDefaultMiiDataCore(&data->core, i);
        CFLi_GetDefaultCreateIDonCTR(&data->core.createID, i);
        CFLi_ClearCreatorName(data);
    }
    s_EnableSpecialMii = false;
}

// writes "CFRA" back if it changed, keeps a copy of the file and closes everything
// 0x00135D58 | nintendogs:callseq [confirmed by mk7dlp] [tier A]
void CFLi_FinalizeDatabase(bool keep)
{
    if (!keep) {
        CFLiDatabase* db;
        if (CFL_IsAvailable() && (db = s_pDatabase) != NULL && db->recent.dirty) {
            s32 status = CFL_RESULT_NOT_AVAILABLE;
            if (!CFLi_GetFileBroken(1) && !CFLi_GetFileBroken(0)) {
                db->finalizeCallback = NULL;
                db->writeDone = CFLi_FinalizeWriteDone;
                db->writeMode = 1;
                CFLi_PutCRC16(0, &db->recent, sizeof(db->recent));
                CFLiDevice* device = s_pDatabaseDevice;
                device->create = true;
                device->flush = true;
                CFLi_SequenceOpen(device, CFLi_FinalizeOpened);
                status = CFL_GetAsyncStatus();
            }
            if (status == CFL_RESULT_BUSY) {
                CFL_WaitAsync();
            }
        }
        db = s_pDatabase;
        if (db != NULL && !CFLi_GetFileBroken(1) && !CFLi_GetFileBroken(0)) {
            if (db->restored || !CFLi_FileExists(OLD_DATABASE_PATH)) {
                CFLi_CopyFile(OLD_DATABASE_PATH, DATABASE_PATH, -1);
            } else if (db->written) {
                CFLi_CopyFile(OLD_DATABASE_PATH, DATABASE_PATH, offsetof(CFLiDatabase, written));
            }
        }
    }
    s_pReceivedList = NULL;
    s_pDatabase = NULL;
    s_pDatabaseDevice->iface->Destroy(s_pDatabaseDevice);
    s_pDatabaseDevice = NULL;
    if (s_DatabaseMounted) {
        nn::fs::Unmount(DATABASE_ARCHIVE);
        s_DatabaseMounted = false;
    }
}

// starts reading the database; the callback runs when it is loaded
// 0x001544AC | nintendogs:bytes-fuzzy [confirmed by mk7dlp] [tier A]
s32 CFLi_BootLoadDatabaseAsync(CFLiSequenceCallback callback)
{
    s32 status = CFL_RESULT_4;
    if (CFL_IsAvailable() && !CFLi_GetWorking()) {
        CFLiDatabase* db = s_pDatabase;
        if (db != NULL) {
            db->bootCallback = callback;
            db->bootNext = CFLi_BootLoadDatabaseNext;
            CFLiDevice* device = s_pDatabaseDevice;
            device->create = false;
            device->flush = true;
            CFLi_SequenceOpen(device, CFLi_BootLoadDatabaseOpened);
            status = CFL_GetAsyncStatus();
        }
    }
    return status;
}

// restores the database from the copy
// 0x001558CC | nintendogs:callseq [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE s32 CFLi_LoadBackupedDatabase()
{
    s32 status = CFL_RESULT_4;
    if (s_DatabaseMounted && CFLi_FileExists(OLD_DATABASE_PATH)) {
        status = CFLi_CopyFile(DATABASE_PATH, OLD_DATABASE_PATH, -1);
        if (status == CFL_RESULT_SUCCESS) {
            CFLi_GetManager()->flags &= 4;
            status = CFL_RESULT_4;
            if (CFL_IsAvailable() && !CFLi_GetWorking()) {
                CFLiDatabase* db = s_pDatabase;
                if (db != NULL) {
                    db->bootCallback = NULL;
                    db->bootNext = CFLi_BootLoadDatabaseNext;
                    CFLiDevice* device = s_pDatabaseDevice;
                    device->create = false;
                    device->flush = true;
                    CFLi_SequenceOpen(device, CFLi_BootLoadDatabaseOpened);
                    status = CFL_GetAsyncStatus();
                    if (status == CFL_RESULT_BUSY) {
                        status = CFL_WaitAsync();
                    }
                }
            }
        }
    }
    return status;
}

// copies one image of a section of the resource archive; returns its size
// 0x001566B4 | nintendogs:bytes [tier B]
s32 CFLi_ReadArchiveImage(void* dst, int section, int index)
{
    s32 size = -1;
    if (!CFL_IsAvailable()) {
        return size;
    }
    CFLiResLoader* loader = &CFLi_GetManager()->resLoader;
    if (loader == NULL) {
        return size;
    }
    CFLiResSection* entry = &loader->sections[section];
    if (entry->num <= index || !CFL_IsAvailable() || !CFL_IsResourceCached()) {
        return size;
    }
    const u8* data = loader->data;
    const u32* table = reinterpret_cast<const u32*>(data + entry->offset);
    u32 start = table[index];
    u32 end;
    if (section == 14 && CFLi_IsHDModeEnabled()) {
        end = table[index + 1];
    } else {
        // the upper bits point to an image this one shares
        if ((start >> 22) != 0) {
            index = (start >> 22) - 1;
            start = table[index] & 0x3FFFFF;
        }
        end = table[index + 1] & 0x3FFFFF;
    }
    size = end - start;
    if (dst != NULL) {
        std::memcpy(dst, data + entry->offset + entry->num * 4 + start + 4, size);
    }
    return size;
}
} // extern "C"

// the steps of the database (names are ours)

// 0x0013AAF0 (name is ours)
void CFLi_FinalizeOpened(CFLiDevice* device, s32)
{
    if (CFL_GetAsyncStatus() == CFL_RESULT_SUCCESS) {
        CFLi_SequenceWrite(device, &s_pDatabase->recent, sizeof(CFLiRecentDBFile),
                           offsetof(CFLiDatabase, recent), true, CFLi_StartClose);
    }
}

// 0x0013AB48 (name is ours)
void CFLi_FinalizeWriteDone(CFLiDevice* device, s32 value)
{
    CFLiSequenceCallback callback = s_pDatabase->finalizeCallback;
    if (callback != NULL) {
        callback(device, value);
    }
}

// 0x0013AB70 (name is ours)
void CFLi_EmptyCallback()
{
    // empty (in the original too)
}

// 0x0013DF2C (name is ours)
void CFLi_StartClose(CFLiDevice* device, s32)
{
    device->state = 0;
    device->callback = s_pDatabase->writeDone;
    device->offset = 0;
    device->buffer = NULL;
    device->size = 0;
    device->_10 = false;
    device->step = CFLi_SequenceCloseStep;
    CFLi_SequenceCloseStep(device, 0);
}

// 0x00180520 (name is ours)
void CFLi_BootLoadDatabaseNext(CFLiDevice* device, s32 value)
{
    CFLiSequenceCallback callback = s_pDatabase->bootCallback;
    if (callback != NULL) {
        callback(device, value);
    }
}

// 0x0017A434 (name is ours)
void CFLi_BootLoadDatabaseOpened(CFLiDevice* device, s32)
{
    CFLiDatabase* db = s_pDatabase;
    s32 status = CFL_GetAsyncStatus();
    if (status == CFL_RESULT_SUCCESS) {
        BeginWorking();
        device->callback = CFLi_BootLoadDatabaseRead;
        device->state = 0;
        device->buffer = db;
        device->size = offsetof(CFLiDatabase, written);
        device->offset = 0;
        device->_10 = false;
        device->step = CFLi_SequenceReadStep;
        CFLi_SequenceReadStep(device, 0);
        return;
    }
    if (status == CFL_RESULT_FILE_ERROR) {
        s32 reason = CFL_GetLastReason();
        if (reason == CFL_REASON_CORRUPTED) {
            CFLi_GetManager()->status = 12;
        } else if (reason != CFL_REASON_NOT_FOUND) {
            CFLi_GetManager()->status = CFL_RESULT_4;
            CFLi_SetFileBroken(1);
        } else if (!s_DatabaseLoaded) {
            CFLi_GetManager()->status = CFL_RESULT_INVALID_DATA;
            CFLi_SetFileBroken(1);
        } else if (CFLi_FileExists(OLD_DATABASE_PATH) || s_Restoring) {
            CFLi_GetManager()->status = CFL_RESULT_4;
            CFLi_SetFileBroken(1);
        } else {
            // no file: make an empty database
            CFLiSequenceCallback callback = db->bootCallback;
            s_Restoring = true;
            CFLi_FormatDatabase(NULL);
            s32 result = CFL_WaitAsync();
            if (result == CFL_RESULT_5 || result == CFL_RESULT_FILE_ERROR) {
                CFLi_GetManager()->status = CFL_RESULT_4;
            }
            s_Restoring = false;
            db->bootCallback = callback;
        }
    }
    CFLiSequenceCallback next = db->bootNext;
    if (next != NULL) {
        next(device, 0);
    }
}

// 0x0017442C (name is ours)
void CFLi_BootLoadDatabaseRead(CFLiDevice* device, s32)
{
    CFLiDatabase* db = s_pDatabase;
    s32 status = CFL_GetAsyncStatus();
    if (status == CFL_RESULT_SUCCESS) {
        if (db->official.magic == MAGIC_CFOG) {
            if (db->recent.magic != MAGIC_CFRA ||
                CFLi_GetCRC16(0, &db->recent, sizeof(db->recent)) != 0) {
                std::memset(&db->recent, 0, sizeof(db->recent));
                db->recent.magic = MAGIC_CFRA;
            }
            db->recent.dirty = false;
            if (!CFL_IsAvailable() || !CFLi_GetWorking()) {
                BeginWorking();
            }
            db->crcData = reinterpret_cast<const u8*>(db);
            db->crcSize = sizeof(CFLiDatabaseOfficial);
            db->crcPosition = 0;
            db->crc = 0;
            db->crcCallback = reinterpret_cast<CFLiCRCCallback>(CFLi_BootLoadDatabaseChecked);
            db->crcContext = device;
            CFLi_ComputeCRCStep(db);
            return;
        }
    } else if (status != CFL_RESULT_FILE_ERROR) {
        goto close;
    }
    CFLi_GetManager()->status = CFL_RESULT_4;
    CFLi_SetFileBroken(1);
close:
    device->callback = db->bootNext;
    device->state = 0;
    device->offset = 0;
    device->buffer = NULL;
    device->size = 0;
    device->_10 = false;
    device->step = CFLi_SequenceCloseStep;
    CFLi_SequenceCloseStep(device, 0);
}

// 0x00174324 (name is ours)
void CFLi_BootLoadDatabaseChecked(CFLiDevice* device, u16 crc)
{
    CFLiDatabase* db = s_pDatabase;
    if (crc != 0) {
        CFLi_GetManager()->status = CFL_RESULT_4;
        CFLi_SetFileBroken(1);
    } else {
        CFLi_GetManager()->status = CFL_RESULT_SUCCESS;
        CFLiDatabaseOfficial* official = &db->official;
        if (official->hiddenMagic != MAGIC_CFHE) {
            official->hiddenMagic = MAGIC_CFHE;
            official->hiddenHead = 0xFFFF;
            official->hiddenTail = 0xFFFF;
            for (int i = 0; i < CFL_DATABASE_HIDDEN_NUM; i++) {
                CFLiHiddenNode* node = &official->hidden[i];
                std::memset(node, 0, sizeof(*node));
                node->prev = 0x7FFF;
                node->next = 0x7FFF;
            }
        }
    }
    device->callback = db->bootNext;
    device->state = 0;
    device->offset = 0;
    device->buffer = NULL;
    device->size = 0;
    device->_10 = false;
    device->step = CFLi_SequenceCloseStep;
    CFLi_SequenceCloseStep(device, 0);
}

// the end of the boot: a broken file is restored or formatted, then the resource archive is parsed
// 0x0016C6BC (name is ours)
void CFLi_BootLoadDatabaseCallback(CFLiDevice*, s32)
{
    s32 status = CFL_GetAsyncStatus();
    if (!CFLi_IsDatabaseLoaded() && (status == CFL_RESULT_4 || status == CFL_RESULT_FILE_ERROR)) {
        CFLi_GetManager()->status = CFL_RESULT_INVALID_DATA;
    }
    status = CFL_GetAsyncStatus();
    if (status == CFL_RESULT_4 || status == CFL_RESULT_FILE_ERROR) {
        CFLi_LoadBackupedDatabase();
    }
    status = CFL_GetAsyncStatus();
    if ((status == CFL_RESULT_4 || status == CFL_RESULT_FILE_ERROR) && CFLi_IsNandSharedExtMounted()) {
        CFLi_FormatDatabase(NULL);
        CFL_WaitAsync();
        CFLi_GetManager()->status = CFL_RESULT_9;
    }
    CFLiResLoader* loader;
    if (CFL_IsAvailable() && (loader = &CFLi_GetManager()->resLoader) != NULL) {
        if (CFL_IsResourceCached()) {
            const u8* data = loader->data;
            u16 value = *reinterpret_cast<const u16*>(data + 2);
            loader->version = value;
            for (int i = 0; i < 20; i++) {
                u32 offset = reinterpret_cast<const u32*>(data + 4)[i];
                const u16* section = reinterpret_cast<const u16*>(data + offset);
                loader->sections[i].num = section[0];
                loader->sections[i].offset = offset + 4;
                loader->sections[i].size = section[1];
            }
        }
        CFLi_EndWorkingReason(CFL_RESULT_SUCCESS, 0);
    } else {
        CFLi_EndWorkingReason(CFL_RESULT_BROKEN, 0);
    }
    status = CFL_GetAsyncStatus();
    if (status == CFL_RESULT_4 || status == CFL_RESULT_5 || status == CFL_RESULT_FILE_ERROR) {
        CFLi_GetManager()->status = CFL_RESULT_SUCCESS;
    }
    s32 result = CFL_WaitAsync();
    if (result != CFL_RESULT_SUCCESS && result != CFL_RESULT_BUSY && result != CFL_RESULT_9) {
        CFL_Exit();
    }
}

// makes an empty database file
// 0x001546DC (name is ours)
DECOMP_NOINLINE s32 CFLi_FormatDatabase(CFLiSequenceCallback callback)
{
    s32 status = CFL_RESULT_NOT_AVAILABLE;
    if (CFL_IsAvailable()) {
        CFLiDatabase* db = s_pDatabase;
        if (db != NULL) {
            CFLi_DeleteFile(OLD_DATABASE_PATH);
            CFLi_GetManager()->flags &= 4;
            InitializeOfficial(&db->official);
            std::memset(&db->recent, 0, sizeof(db->recent));
            db->recent.magic = MAGIC_CFRA;
            CFLi_PutCRC16(0, &db->recent, sizeof(db->recent));
            db->formatCallback = callback;
            db->formatFailed = CFLi_FormatFailed;
            BeginWorking();
            s_pDatabase->formatNext = CFLi_FormatDirectoryCreated;
            CFLiDevice* device = s_pDatabaseDevice;
            device->create = true;
            device->flush = true;
            device->callback = CFLi_FormatNext;
            device->state = 0;
            device->offset = 0;
            device->buffer = NULL;
            device->size = 0;
            device->_10 = false;
            device->step = CFLi_SequenceCreateDirectoryStep;
            CFLi_SequenceCreateDirectoryStep(device, 0);
            status = CFL_GetAsyncStatus();
        }
    }
    return status;
}

// creates the directory of the path of the device
// 0x001737C8 (name is ours)
void CFLi_SequenceCreateDirectoryStep(CFLiDevice* device, s32)
{
    for (;;) {
        switch (device->state) {
        case 0: {
            device->offset = 0;
            device->state = 1;
            for (int i = CFLi_GetStringLength8(device->path) - 1; i > 0; i--) {
                if (device->path[i] == '/') {
                    device->offset = i;
                    device->path[i] = 0;
                    break;
                }
            }
            device->status = device->iface->CreateDirectory != NULL
                                 ? device->iface->CreateDirectory(device, device->path)
                                 : static_cast<s32>(CFL_RESULT_BROKEN);
            if (device->status == CFL_RESULT_BUSY) {
                return;
            }
            continue;
        }
        case 1: {
            if (device->offset != 0) {
                device->path[device->offset] = '/';
            }
            s32 status = device->status;
            if (status == CFL_RESULT_BUSY) {
                return;
            }
            s32 value = device->value;
            CFLi_EndWorkingReason(status, value);
            if (device->callback != NULL) {
                device->callback(device, device->value);
            }
            return;
        }
        default:
            return;
        }
    }
}

// deletes the file of the device
// 0x00164C08 (name is ours)
void CFLi_SequenceDeleteStep(CFLiDevice* device, s32)
{
    for (;;) {
        switch (device->state) {
        case 0:
            device->state = 1;
            device->status = device->iface->DeleteFile != NULL ? device->iface->DeleteFile(device)
                                                               : static_cast<s32>(CFL_RESULT_BROKEN);
            if (device->status == CFL_RESULT_BUSY) {
                return;
            }
            continue;
        case 1: {
            s32 status = device->status;
            if (status == CFL_RESULT_BUSY) {
                return;
            }
            s32 value = device->value;
            CFLi_EndWorkingReason(status, value);
            if (device->callback != NULL) {
                device->callback(device, device->value);
            }
            return;
        }
        default:
            return;
        }
    }
}

// 0x0018434C (name is ours)
void CFLi_FormatNext(CFLiDevice* device, s32 value)
{
    CFLiSequenceCallback callback = s_pDatabase->formatNext;
    if (callback != NULL) {
        callback(device, value);
    }
}

// 0x0017EDF8 (name is ours)
void CFLi_FormatFailed(CFLiDevice* device, s32 value)
{
    s32 status = CFL_GetAsyncStatus();
    if (status != CFL_RESULT_SUCCESS && status != CFL_RESULT_BUSY) {
        nn::err::CTR::ThrowFatalErrAllIfFailure(device->result);
    }
    CFLiSequenceCallback callback = s_pDatabase->formatCallback;
    if (callback != NULL) {
        callback(device, value);
    }
}

// 0x0017F0AC (name is ours)
void CFLi_FormatDirectoryCreated(CFLiDevice* device, s32)
{
    CFLiDevice* databaseDevice = s_pDatabaseDevice;
    databaseDevice->create = true;
    databaseDevice->flush = true;
    if (CFL_GetAsyncStatus() != CFL_RESULT_SUCCESS) {
        CFLiSequenceCallback callback = s_pDatabase->formatFailed;
        if (callback != NULL) {
            callback(databaseDevice, 0);
        }
        return;
    }
    BeginWorking();
    StartStep(databaseDevice, CFLi_SequenceDeleteStep, CFLi_FormatDeleted);
}

// 0x0017F66C (name is ours)
void CFLi_FormatDeleted(CFLiDevice* device, s32)
{
    if (CFL_GetAsyncStatus() != CFL_RESULT_SUCCESS) {
        CFLiSequenceCallback callback = s_pDatabase->formatFailed;
        if (callback != NULL) {
            callback(device, 0);
        }
        return;
    }
    CFLi_SequenceOpen(device, CFLi_FormatOpened);
}

// 0x0016FE90 (name is ours)
void CFLi_FormatOpened(CFLiDevice* device, s32)
{
    if (CFL_GetAsyncStatus() != CFL_RESULT_SUCCESS) {
        CFLiSequenceCallback callback = s_pDatabase->formatFailed;
        if (callback != NULL) {
            callback(device, 0);
        }
        return;
    }
    CFLi_StartComputeCRC(device, reinterpret_cast<CFLiCRCCallback>(CFLi_FormatCRCDone));
}

// writes the database after its CRC changed (name is ours)
// 0x0018018C (name is ours)
void CFLi_WriteDatabase(CFLiDevice* device, u16 crc)
{
    CFLiDatabase* db = s_pDatabase;
    db->official.crc = crc;
    if (db->writeMode == 2) {
        CFLi_SequenceWrite(device, db, sizeof(CFLiDatabaseOfficial), 0, true, CFLi_DatabaseWritten);
        return;
    }
    CFLi_SequenceOpen(device, CFLi_WriteDatabaseOpened);
}

// 0x00174B4C (name is ours)
void CFLi_WriteDatabaseOpened(CFLiDevice* device, s32)
{
    if (CFL_GetAsyncStatus() != CFL_RESULT_SUCCESS) {
        CFLi_GetManager()->status = CFL_RESULT_5;
        return;
    }
    CFLi_SequenceWrite(device, s_pDatabase, sizeof(CFLiDatabaseOfficial), 0, true, CFLi_DatabaseWritten);
}

// 0x0016CA64 (name is ours)
void CFLi_DatabaseWritten(CFLiDevice* device, s32)
{
    CFLiDatabase* db = s_pDatabase;
    if (CFL_GetAsyncStatus() == CFL_RESULT_FILE_ERROR) {
        CFLi_GetManager()->status = CFL_RESULT_5;
    }
    if (CFL_GetAsyncStatus() == CFL_RESULT_SUCCESS) {
        db->written = true;
    }
    if (db->writeMode == 1) {
        device->callback = db->writeDone;
        device->state = 0;
        device->offset = 0;
        device->buffer = NULL;
        device->size = 0;
        device->_10 = false;
        device->step = CFLi_SequenceCloseStep;
        CFLi_SequenceCloseStep(device, 0);
        return;
    }
    CFLiSequenceCallback callback = db->writeDone;
    if (callback != NULL) {
        callback(device, device->value);
    }
}

// 0x001738DC (name is ours)
void CFLi_StartWriteDatabase(CFLiDevice* device, s32)
{
    if (CFL_GetAsyncStatus() != CFL_RESULT_SUCCESS) {
        CFLi_GetManager()->status = CFL_RESULT_5;
        return;
    }
    CFLi_MergeRecentDatabase(device);
}

// 0x00179BF4 (name is ours)
void CFLi_FormatCRCDone(CFLiDevice* device, u16 crc)
{
    CFLiDatabase* db = s_pDatabase;
    db->official.crc = crc;
    CFLiReceivedList* received = s_pReceivedList;
    received->buffer = NULL;
    received->written = -1;
    received->buffer = static_cast<CFLiHiddenEntry*>(CFLi_AllocateMemory(1000 * sizeof(CFLiHiddenEntry), 32));
    CFLi_SequenceWrite(device, db, offsetof(CFLiDatabase, written), 0, true, CFLi_FormatWritten);
}

// moves the received Miis into the list of hidden Miis (the oldest is reused when it is full) and writes them
// 0x0017390C (name is ours)
DECOMP_NOINLINE void CFLi_MergeRecentDatabase(CFLiDevice* device)
{
    CFLiReceivedList* received = s_pReceivedList;
    CFLiDatabaseOfficial* official = &s_pDatabase->official;
    s16* indices = static_cast<s16*>(CFLi_AllocateMemory(256, 32));
    for (int i = 0; i < received->num; i++) {
        const CFLiHiddenEntry* entry = &received->entries[i];
        s16 index = -1;
        for (int j = 0; j < CFL_DATABASE_HIDDEN_NUM; j++) {
            if (CFLi_IsNullMiiID(&official->hidden[j].createID)) {
                index = j;
                break;
            }
        }
        s16 found = -1;
        for (int j = 0; j < CFL_DATABASE_HIDDEN_NUM; j++) {
            if (CFLi_IsSameMiiID(&official->hidden[j].createID, &entry->core.createID)) {
                found = j;
                break;
            }
        }
        if (found >= 0) {
            // a hidden Mii again: to the end of the list
            index = found;
            if (received->moveToEnd && index < CFL_DATABASE_HIDDEN_NUM) {
                CFLiHiddenNode* node = &official->hidden[index];
                if (!CFLi_IsNullMiiID(&node->createID) && official->hiddenTail != index) {
                    if (node->prev != 0x7FFF) {
                        official->hidden[node->prev].next = node->next != 0x7FFF ? node->next : 0x7FFF;
                    }
                    if (node->next != 0x7FFF) {
                        official->hidden[node->next].prev = node->prev != 0x7FFF ? node->prev : 0x7FFF;
                    }
                    if (official->hiddenHead == index) {
                        official->hiddenHead = node->next != 0x7FFF ? static_cast<s16>(node->next) : -1;
                    }
                    official->hidden[official->hiddenTail].next = index;
                    node->prev = official->hiddenTail != -1 ? official->hiddenTail : 0x7FFF;
                    node->next = 0x7FFF;
                    official->hiddenTail = index;
                }
            }
        } else if (index < 0) {
            // full: the oldest makes room
            s16 head = official->hiddenHead;
            s16 newIndex = -1;
            if (head < CFL_DATABASE_HIDDEN_NUM) {
                CFLiHiddenNode* node = &official->hidden[head];
                official->hiddenHead = node->next != 0x7FFF ? static_cast<s16>(node->next) : -1;
                s16 tail = official->hiddenTail;
                official->hiddenTail = head;
                official->hidden[official->hiddenHead].prev = 0x7FFF;
                official->hidden[tail].next = head != -1 ? head : 0x7FFF;
                std::memset(node, 0, sizeof(*node));
                node->next = 0x7FFF;
                node->prev = 0x7FFF;
                node->prev = tail != -1 ? tail : 0x7FFF;
                node->next = 0x7FFF;
                node->createID = entry->core.createID;
                node->gender = entry->core.gender;
                newIndex = head;
            }
            index = newIndex;
        } else {
            CFLiHiddenNode* node = &official->hidden[index];
            std::memset(node, 0, sizeof(*node));
            node->next = 0x7FFF;
            node->prev = 0x7FFF;
            if (official->hiddenTail != -1) {
                official->hidden[official->hiddenTail].next = index;
                node->prev = official->hiddenTail != -1 ? official->hiddenTail : 0x7FFF;
                official->hiddenTail = index;
                node->next = 0x7FFF;
            } else {
                official->hiddenHead = index;
                official->hiddenTail = index;
                node->prev = 0x7FFF;
                node->next = 0x7FFF;
            }
            node->createID = entry->core.createID;
            node->gender = entry->core.gender;
        }
        indices[i] = index;
    }

    // write runs of consecutive slots at once
    for (int i = 0; i < received->num;) {
        s16 start = indices[i];
        int run = 1;
        while (i + run < received->num && indices[i + run] == start + run) {
            run++;
        }
        u32 size = run * sizeof(CFLiHiddenEntry);
        const CFLiHiddenEntry* src = &received->entries[i];
        BeginWorking();
        device->flush = false;
        device->state = 0;
        device->callback = NULL;
        device->offset = offsetof(CFLiDatabase, written) + start * sizeof(CFLiHiddenEntry);
        device->size = size;
        device->buffer = const_cast<CFLiHiddenEntry*>(src);
        device->_10 = false;
        device->step = CFLi_SequenceWriteStep;
        CFLi_SequenceWriteStep(device, 0);
        if (CFL_GetAsyncStatus() == CFL_RESULT_BUSY) {
            CFL_WaitAsync();
        }
        if (CFL_GetAsyncStatus() != CFL_RESULT_SUCCESS) {
            CFLi_GetManager()->status = CFL_RESULT_5;
            break;
        }
        if (CFL_IsAvailable() && s_pReceivedList->hasMirror) {
            std::memcpy(&received->mirror[start], src, size);
        }
        i += run;
    }
    CFLi_FreeMemory(indices);
    CFLi_FlushDevice(reinterpret_cast<CFLiFileDevice*>(device));
    if (device->status != CFL_RESULT_SUCCESS) {
        CFLi_GetManager()->status = CFL_RESULT_5;
    }
    if (CFL_GetAsyncStatus() != CFL_RESULT_SUCCESS) {
        device->state = 0;
        device->callback = NULL;
        device->offset = 0;
        device->buffer = NULL;
        device->size = 0;
        device->step = CFLi_SequenceCloseStep;
        device->_10 = false;
        CFLi_SequenceCloseStep(device, 0);
        return;
    }
    CFLiDatabase* db = s_pDatabase;
    db->restored = true;
    CFLiSequenceCallback callback = received->callback;
    if (db != NULL && !CFLi_GetFileBroken(1) && !CFLi_GetFileBroken(0)) {
        db->writeDone = callback;
        db->writeMode = 2;
        CFLi_StartComputeCRC(device, reinterpret_cast<CFLiCRCCallback>(CFLi_WriteDatabase));
        CFL_GetAsyncStatus();
    }
}

// clears the hidden Miis after the database in steps of 1000, then reloads the database
// 0x00171974 (name is ours)
void CFLi_FormatWritten(CFLiDevice* device, s32)
{
    CFLiReceivedList* received = s_pReceivedList;
    if (CFL_GetAsyncStatus() == CFL_RESULT_SUCCESS) {
        if (received->written == -1) {
            received->written = 0;
        } else {
            received->written += 1000;
            if (received->written >= CFL_DATABASE_HIDDEN_NUM) {
                goto done;
            }
        }
        s32 offset;
        offset = received->written * sizeof(CFLiHiddenEntry);
        if (offset >= 0) {
            for (int i = 0; i < 1000; i++) {
                std::memset(&received->buffer[i], 0, sizeof(CFLiHiddenEntry));
            }
            CFLi_SequenceWrite(device, received->buffer, 1000 * sizeof(CFLiHiddenEntry),
                               offsetof(CFLiDatabase, written) + offset, true, CFLi_FormatWritten);
            return;
        }
    done:
        if (CFL_IsAvailable() && s_pReceivedList->hasMirror) {
            std::memset(received->mirror, 0, CFL_DATABASE_HIDDEN_NUM * sizeof(CFLiHiddenEntry));
        }
    }
    CFLi_FreeMemory(received->buffer);
    received->buffer = NULL;
    device->callback = CFLi_FormatReloaded;
    device->state = 0;
    device->offset = 0;
    device->buffer = NULL;
    device->size = 0;
    device->_10 = false;
    device->step = CFLi_SequenceCloseStep;
    CFLi_SequenceCloseStep(device, 0);
}

// 0x0017188C (name is ours)
void CFLi_FormatReloaded(CFLiDevice* device, s32)
{
    if (CFL_GetAsyncStatus() != CFL_RESULT_SUCCESS) {
        CFLiSequenceCallback callback = s_pDatabase->formatFailed;
        if (callback != NULL) {
            callback(device, 0);
        }
        return;
    }
    CFLiSequenceCallback callback = s_pDatabase->formatCallback;
    if (CFL_IsAvailable() && !CFLi_GetWorking()) {
        CFLiDatabase* db = s_pDatabase;
        if (db != NULL) {
            db->bootCallback = callback;
            db->bootNext = CFLi_BootLoadDatabaseNext;
            CFLiDevice* databaseDevice = s_pDatabaseDevice;
            databaseDevice->create = false;
            databaseDevice->flush = true;
            CFLi_SequenceOpen(databaseDevice, CFLi_BootLoadDatabaseOpened);
            CFL_GetAsyncStatus();
        }
    }
}

extern "C" {
// adds a Mii of another console to the recently used ones
// 0x0015421C | nintendogs:bytes-fuzzy [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE s32 CFLi_AddRecentDBData(const CFLiMiiDataCore* data)
{
    s32 status = CFL_RESULT_NOT_AVAILABLE;
    if (!CFL_IsAvailable()) {
        return status;
    }
    CFLiDatabase* db = s_pDatabase;
    if (db == NULL || CFLi_GetFileBroken(1) || CFLi_GetFileBroken(0)) {
        return status;
    }
    if (!CFLi_IsValidMiiID(&data->createID) || !CFLi_IsNormalMiiID(&data->createID) ||
        CFLi_IsNullAuthorID(&data->authorID) || CFLi_IsHomeAuthorID(&data->authorID)) {
        return CFL_RESULT_SUCCESS;
    }
    if (&data->createID != NULL) {
        for (int i = 0; i < 6; i++) {
            CFLiCreateID id;
            CFLi_GetDefaultCreateIDonCTR(&id, i);
            if (CFLi_IsSameMiiID(&id, &data->createID)) {
                return CFL_RESULT_SUCCESS;
            }
        }
    }
    if (CFL_SearchOfficialData(NULL, &data->createID)) {
        return CFL_RESULT_SUCCESS;
    }
    CFLiRecentDBFile* recent = &db->recent;
    int slot = -1;
    for (int i = 0; i < recent->num; i++) {
        int s = recent->order[i];
        if (CFLi_IsSameMiiID(&recent->entries[s].createID, &data->createID)) {
            slot = s;
            break;
        }
    }
    if (slot == -1) {
        if (recent->num < CFL_RECENT_NUM) {
            for (int s = 0; s < CFL_RECENT_NUM; s++) {
                if (CFLi_IsNullMiiID(&recent->entries[s].createID)) {
                    slot = s;
                    break;
                }
            }
        }
        if (slot == -1) {
            slot = recent->order[0];
            recent->DeleteIndex(0);
        }
    }
    std::memcpy(&recent->entries[slot], data, sizeof(CFLiMiiDataCore));
    int position = -1;
    for (int i = 0; i < recent->num; i++) {
        if (recent->order[i] == slot) {
            position = i;
            break;
        }
    }
    if (position != -1) {
        recent->DeleteIndex(position);
    } else if (recent->num >= CFL_RECENT_NUM) {
        recent->DeleteIndex(0);
    }
    recent->order[recent->num] = slot;
    recent->num++;
    recent->dirty = true;
    return CFL_RESULT_SUCCESS;
}

// 0x001541F0 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
s32 CFLi_AddRecentDBCharInfo(const CFLiCharInfo* info)
{
    CFLiMiiDataCore data;
    CFLi_PackMiiDataCore(&data, NULL, info, false);
    return CFLi_AddRecentDBData(&data);
}
} // extern "C"

// the axes of the model in the order of the program (name is ours)
// 0x007C6DD0 (name is ours)
DECOMP_NOINLINE void CFLi_ApplyAxis(f32* v)
{
    f32 x = v[0];
    f32 y = v[1];
    f32 z = v[2];
    if (s_Axis[2]) {
        x = -x;
    }
    if (s_Axis[0]) {
        y = -y;
    }
    if (s_Axis[1]) {
        z = -z;
    }
    v[s_Axis[5]] = x;
    v[s_Axis[3]] = y;
    v[s_Axis[4]] = z;
}

// the size of CFL_Res.dat, 0 if the content cannot be mounted (name is ours)
// 0x00153098 (name is ours)
u32 CFL_GetResFileSize()
{
    u32 size = 0;
    if (CFLi_MountNandContent() != CFL_RESULT_SUCCESS) {
        return size;
    }
    if (s_ResFileSize != 0) {
        return s_ResFileSize;
    }
    nn::fs::FileInputStream stream;
    nn::Result result = stream.TryInitialize(RESOURCE_PATH);
    if (result.IsSuccess()) {
        s64 fileSize;
        nn::err::CTR::ThrowFatalErrAllIfFailure(stream.FileBase::TryGetSize(&fileSize));
        size = fileSize;
        stream.Finalize();
        s_ResFileSize = size;
    } else {
        nn::err::CTR::ThrowFatalErrAllIfFailure(result);
    }
    return size;
}

// reads CFL_Res.dat into the buffer (name is ours)
// 0x00153794 (name is ours)
s32 CFL_LoadResFile(void* buffer)
{
    s32 status = CFL_RESULT_BROKEN;
    if (CFLi_MountNandContent() != CFL_RESULT_SUCCESS) {
        return status;
    }
    nn::fs::FileInputStream stream;
    nn::Result result = stream.TryInitialize(RESOURCE_PATH);
    if (result.IsSuccess()) {
        s64 fileSize;
        nn::err::CTR::ThrowFatalErrAllIfFailure(stream.FileBase::TryGetSize(&fileSize));
        s32 size = fileSize;
        s32 readSize;
        result = stream.FileBase::TryRead(&readSize, buffer, size);
        if (result.IsSuccess() && readSize == size) {
            status = CFL_RESULT_SUCCESS;
        }
        stream.Finalize();
    }
    nn::err::CTR::ThrowFatalErrAllIfFailure(result);
    return status;
}
