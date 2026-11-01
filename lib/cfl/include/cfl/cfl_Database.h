#pragma once

// The devices of CFL (files or a copy of the database in memory, both behind a table of functions) with the
// asynchronous sequences that open, read, write and close them, and the database CFL_DB.dat. All names except the
// functions named in the reference symbols are ours; the sections of the database follow 3dbrew ("Mii",
// CFL_DB.dat: "CFOG", "CFHE", "CFRA").

#include "cfl/cfl_Api.h"
#include "cfl/cfl_RecentDB.h"
#include "nn/fs/detail/fs_FileBase.h"

struct CFLiDevice;

typedef void (*CFLiSequenceCallback)(CFLiDevice* device, s32 value);
typedef void (*CFLiSequenceStep)(CFLiDevice* device, s32 unused);

// what a device can do; every function sets status and value of the device and returns the status
struct CFLiDeviceInterface
{
    CFLiDevice* (*Create)(void** buffer, u32* bufferSize);
    void (*Destroy)(CFLiDevice* device);
    s32 (*CreateFile)(CFLiDevice* device, u32 size);
    s32 (*DeleteFile)(CFLiDevice* device);
    s32 (*Open)(CFLiDevice* device);
    s32 (*Close)(CFLiDevice* device);
    s32 (*GetSize)(CFLiDevice* device, u32* size);
    s32 (*Read)(CFLiDevice* device, void* buffer, u32 size);
    s32 (*Write)(CFLiDevice* device, const void* buffer, u32 size, bool flush);
    s32 (*Seek)(CFLiDevice* device, s32 position);
    s32 (*CreateDirectory)(CFLiDevice* device, const char* path);
};

// a device with the state of its sequence
struct CFLiDevice
{
    const CFLiDeviceInterface* iface;  // 0x00
    s32 status;                        // 0x04 CFLResult
    s32 value;                         // 0x08 bytes read/written, the size, or a reason (< 0)
    CFLiSequenceStep step;             // 0x0C
    bool _10;                          // 0x10
    bool opened;                       // 0x11
    bool create;                       // 0x12 open for writing, create the file if it is missing
    bool flush;                        // 0x13
    nn::Result result;                 // 0x14 of the last nn::fs call
    s32 offset;                        // 0x18
    u32 size;                          // 0x1C
    void* buffer;                      // 0x20
    s32 state;                         // 0x24
    CFLiSequenceCallback callback;     // 0x28
    char path[260];                    // 0x2C
};
ASSERT_SIZE(CFLiDevice, 0x130);

// a file of nn::fs
struct CFLiFileDevice
{
    CFLiDevice device;              // 0x000
    u32 _130;                       // 0x130
    u32 _134;                       // 0x134
    nn::fs::detail::FileBase file;  // 0x138
};
ASSERT_SIZE(CFLiFileDevice, 0x14C);

// the database in a buffer of the program (s_MemoryDatabase)
struct CFLiMemoryDevice
{
    CFLiDevice device;  // 0x000
    u32 position;       // 0x130
    u32 size;           // 0x134
};
ASSERT_SIZE(CFLiMemoryDevice, 0x138);

// the size of CFL_DB.dat
#define CFL_DATABASE_FILE_SIZE 0x4BD20

// reasons in CFLiDevice::value (names are ours)
#define CFL_REASON_FS_ERROR -128
#define CFL_REASON_ALREADY_EXISTS -1
#define CFL_REASON_NOT_FOUND -2
#define CFL_REASON_CORRUPTED -3

// a node of the list of hidden Miis ("CFHE", oldest first); 0x7FFF is no node
struct CFLiHiddenNode
{
    CFLiCreateID createID;  // 0x0
    u16 next : 15;          // 0xA
    u16 gender : 1;
    u16 prev : 15;          // 0xC
    u16 prevFlag : 1;
};
ASSERT_SIZE(CFLiHiddenNode, 0xE);

#define CFL_DATABASE_OWNED_NUM 100
#define CFL_DATABASE_HIDDEN_NUM 3000

// "CFOG" + "CFHE": the Miis of the console and the hidden Miis, with a CRC16 over both
struct CFLiDatabaseOfficial
{
    u32 magic;              // 0x0000 "CFOG"
    u32 version : 8;        // 0x0004
    u32 _4_8 : 1;
    u32 _4_9 : 23;
    CFLiMiiDataOfficial owned[CFL_DATABASE_OWNED_NUM];  // 0x0008
    u32 hiddenMagic;        // 0x23F8 "CFHE"
    s16 hiddenHead;         // 0x23FC
    s16 hiddenTail;         // 0x23FE
    CFLiHiddenNode hidden[CFL_DATABASE_HIDDEN_NUM];     // 0x2400
    u8 _C810[14];           // 0xC810
    u16 crc;                // 0xC81E
};
ASSERT_SIZE(CFLiDatabaseOfficial, 0xC820);

typedef void (*CFLiCRCCallback)(void* context, u16 crc);

// the database with the state of its operations
struct CFLiDatabase
{
    CFLiDatabaseOfficial official;          // 0x0000
    CFLiRecentDBFile recent;                // 0xC820
    bool written;                           // 0xE4C0 the file was written (the backup copies only the head)
    bool restored;                          // 0xE4C1
    CFLiSequenceCallback finalizeCallback;  // 0xE4C4
    CFLiSequenceCallback bootCallback;      // 0xE4C8 of CFLi_BootLoadDatabaseAsync
    CFLiSequenceCallback formatCallback;    // 0xE4CC
    CFLiSequenceCallback formatNext;        // 0xE4D0
    CFLiSequenceCallback bootNext;          // 0xE4D4
    CFLiSequenceCallback writeDone;         // 0xE4D8
    CFLiSequenceCallback formatFailed;      // 0xE4DC
    s32 writeMode;                          // 0xE4E0 1 close after writing, 2 write without opening
    u8 _E4E4[0x60];                         // 0xE4E4
    // the CRC16 computed in steps of 1 MiB
    const u8* crcData;                      // 0xE544
    u32 crcSize;                            // 0xE548
    u32 crcPosition;                        // 0xE54C
    u16 crc;                                // 0xE550
    u32 _E554;                              // 0xE554
    CFLiCRCCallback crcCallback;            // 0xE558
    void* crcContext;                       // 0xE55C
};
ASSERT_SIZE(CFLiDatabase, 0xE560);

// globals of the database (names are ours)
extern bool s_DatabaseLoaded;
extern bool s_NetMagicN;
extern bool s_DatabaseMounted;
extern bool s_Restoring;
extern CFLiDevice* s_pDatabaseDevice;
extern CFLiReceivedList* s_pReceivedList;
extern CFLiDatabase* s_pDatabase;
extern u8* s_pMemoryDatabase;
extern bool s_NandContentMounted;
extern u32 s_ResFileSize;
extern CFLiDevice* s_pResourceDevice;

extern const CFLiDeviceInterface s_MemoryDeviceInterface;
extern const CFLiDeviceInterface s_FileDeviceInterface;

extern "C" {
const CFLiDeviceInterface* CFLi_GetDeviceInterfaceFile();
CFLiDevice* CFLi_InitializeDevice(void** buffer, u32* bufferSize, const char* path, const CFLiDeviceInterface* iface);
void CFLi_SequenceOpen(CFLiDevice* device, CFLiSequenceCallback callback);
s32 CFLi_FlushDevice(CFLiFileDevice* device);
bool CFLi_FileExists(const char* path);
void CFLi_InitializeResLoader();
void CFLi_InitializeResourceDevice(void** buffer, u32* bufferSize);
void CFLi_FinalizeResourceDevice();
void CFLi_InitializeDatabase(void** buffer, u32* bufferSize);
void CFLi_FinalizeDatabase(bool keep);
s32 CFLi_MountNandSharedExt();
void CFLi_UnmountNandContent();
s32 CFLi_BootLoadDatabaseAsync(CFLiSequenceCallback callback);
s32 CFLi_LoadBackupedDatabase();
s32 CFLi_ReadArchiveImage(void* dst, int section, int index);
} // extern "C"

// (names are ours)
void CFLi_DestroyDevice(CFLiDevice* device);                    // 0x0013A4B8
void CFLi_SequenceOpenStep(CFLiDevice* device, s32 unused);     // 0x0013DFF0
void CFLi_SequenceWrite(CFLiDevice* device, const void* buffer, u32 size, s32 offset, bool flush,
                        CFLiSequenceCallback callback);         // 0x0013DF70
void CFLi_SequenceWriteStep(CFLiDevice* device, s32 unused);    // 0x00140204
void CFLi_SequenceReadStep(CFLiDevice* device, s32 unused);     // 0x00165F08
void CFLi_SequenceCloseStep(CFLiDevice* device, s32 unused);    // 0x00140398
s32 CFLi_CopyFile(const char* dst, const char* src, s32 maxSize);  // 0x0013A594
s32 CFLi_MountNandContent();                                    // 0x00155B94
bool CFLi_IsNandSharedExtMounted();                                  // 0x00155648
bool CFLi_IsDatabaseLoaded();                                   // 0x001555B0
s32 CFLi_DeleteFile(const char* path);                          // 0x00154568
void CFLi_InitializeModelAxis();                                     // 0x001554F0
void CFLi_DrawDoneDefault();                                    // 0x00181498
void CFLi_BootLoadDatabaseCallback(CFLiDevice* device, s32 value);  // 0x0016C6BC

// the steps of the database (names are ours)
void CFLi_ComputeCRCStep(CFLiDatabase* db);                              // 0x00166B60
void CFLi_StartComputeCRC(void* context, CFLiCRCCallback callback);      // 0x0017CFB4
void CFLi_FinalizeOpened(CFLiDevice* device, s32 value);                 // 0x0013AAF0
void CFLi_FinalizeWriteDone(CFLiDevice* device, s32 value);              // 0x0013AB48
void CFLi_EmptyCallback();                                               // 0x0013AB70
void CFLi_StartClose(CFLiDevice* device, s32 value);                     // 0x0013DF2C
void CFLi_BootLoadDatabaseNext(CFLiDevice* device, s32 value);           // 0x00180520
void CFLi_BootLoadDatabaseOpened(CFLiDevice* device, s32 value);         // 0x0017A434
void CFLi_BootLoadDatabaseRead(CFLiDevice* device, s32 value);           // 0x0017442C
void CFLi_BootLoadDatabaseChecked(CFLiDevice* device, u16 crc);          // 0x00174324
s32 CFLi_FormatDatabase(CFLiSequenceCallback callback);                  // 0x001546DC
void CFLi_SequenceCreateDirectoryStep(CFLiDevice* device, s32 unused);   // 0x001737C8
void CFLi_SequenceDeleteStep(CFLiDevice* device, s32 unused);            // 0x00164C08
void CFLi_FormatNext(CFLiDevice* device, s32 value);                     // 0x0018434C
void CFLi_FormatFailed(CFLiDevice* device, s32 value);                   // 0x0017EDF8
void CFLi_FormatDirectoryCreated(CFLiDevice* device, s32 value);         // 0x0017F0AC
void CFLi_FormatDeleted(CFLiDevice* device, s32 value);                  // 0x0017F66C
void CFLi_FormatOpened(CFLiDevice* device, s32 value);                   // 0x0016FE90
void CFLi_FormatCRCDone(CFLiDevice* device, u16 crc);                    // 0x00179BF4
void CFLi_WriteDatabase(CFLiDevice* device, u16 crc);                    // 0x0018018C
void CFLi_WriteDatabaseOpened(CFLiDevice* device, s32 value);            // 0x00174B4C
void CFLi_DatabaseWritten(CFLiDevice* device, s32 value);                // 0x0016CA64
void CFLi_StartWriteDatabase(CFLiDevice* device, s32 value);             // 0x001738DC
void CFLi_MergeRecentDatabase(CFLiDevice* device);                       // 0x0017390C
void CFLi_FormatWritten(CFLiDevice* device, s32 value);                  // 0x00171974
void CFLi_ApplyAxis(f32* v);                                             // 0x007C6DD0

// the axis of the models (names are ours)
extern u8 s_Axis[8];
extern const u8 s_DefaultAxis[8];
void CFLi_FormatReloaded(CFLiDevice* device, s32 value);                 // 0x0017188C
const CFLiMiiDataOfficial* CFLi_GetDatabaseData(u16 index, bool checkSpecial);  // 0x0016CB30

extern "C" {
s32 CFLi_AddRecentDBCharInfo(const CFLiCharInfo* info);
s32 CFLi_AddRecentDBData(const CFLiMiiDataCore* data);
} // extern "C"
u32 CFL_GetResFileSize();          // 0x00153098 (name is ours)
s32 CFL_LoadResFile(void* buffer); // 0x00153794 (name is ours)
