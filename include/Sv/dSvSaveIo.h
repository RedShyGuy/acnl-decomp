#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace sead {
class FileDevice;
}

// (game code) an empty function, called for the SD card where the archive is unmounted
void Func56B888(); // 0x0056B888

// The file access of the save data (game code 0x001B85C4..0x001B9E9C; all names are ours). The save files lie in the save
// data archive of the game (mounted as "data", sead device "backup") or, on the device DEVICE_SDMC, on the SD card (sead
// device "sd"). The parts of the save data are written by the save kinds of SaveProcess (the Write* functions, called
// over the tables of the save dispatcher); the access functions of the save data check the running write (IsSaving,
// GetOffset, GetSize).
class SvSaveIo
{
public:
    // the results of the functions (after the description ranges of the FS module, 3dbrew "Error codes")
    enum Result
    {
        RESULT_OK = 1,
        RESULT_ARCHIVE_NOT_FOUND = 2,    // mount: "not found" (100-179)
        RESULT_BROKEN = 3,               // "bad format" (360-389) or "verification failure" (390-399)
        RESULT_NOT_ENOUGH_SPACE = 4,     // 200-219
        RESULT_NOT_FOUND = 5,            // 100-179
        RESULT_MOUNT_6 = 6,              // mount: 340-359 (no name on 3dbrew)
        RESULT_ARCHIVE_INVALIDATED = 7,  // mount: "invalidated archive" (220-229)
        RESULT_FAILED = 8,
    };
    // where the files are
    enum Device
    {
        DEVICE_SDMC = 0,         // the SD card
        DEVICE_SAVE_DATA = 1,    // the save data archive
        DEVICE_SAVE_DATA_2 = 2,  // (set by SetDevice2, the start value; Format makes it DEVICE_SAVE_DATA)
    };
    // the files of all players and of each player
    static const s32 FILE_NUM = 7;
    static const s32 PLAYER_FILE_NUM = 3;
    static const s32 PLAYER_NUM = 4;

    struct State
    {
        u8 device;          // 0x00 Device
        bool isMounted;     // 0x01
        bool isAccessible;  // 0x02 the archive is mounted with nn::fs::MountAccessibleSaveData (set by SetAccessible)
        u8 unk_0x3;         // 0x03 (1, never used)
        bool isSaving;      // 0x04 between BeginSave and EndSave
        s32 mountCount;     // 0x08 the archive is unmounted when it drops to 0
        u32 offset;         // 0x0C of the running read or write
        u32 size;           // 0x10
    };

    // the file is deleted (not found counts as deleted)
    static s32 DeleteFile(const char* name); // 0x001B85C4
    // the result of the FS module as a Result of the save I/O (fatal for the other results)
    static s32 ConvertResult(const nn::Result& result); // 0x001B8FC4
    // the sead device of the files
    static sead::FileDevice* GetDevice(); // 0x001B8B7C
    // size bytes of the file from offset on into buffer
    static s32 ReadFile(const char* name, u32 offset, void* buffer, u32 size); // 0x001B9130
    // size bytes of data into the file at offset (with "not enough space" the file is deleted and written again; at most
    // 10 tries); isWholeFile is passed for the whole save file and not used
    static s32 WriteFile(const char* name, u32 offset, const void* data, u32 size, bool isWholeFile); // 0x001B9400
    // the save data archive is mounted (once; the others count); broken: formatted if formatIfBroken, then mounted again;
    // saveId: mounted with nn::fs::MountAccessibleSaveData (0x863 for 0)
    static s32 Mount(bool formatIfBroken, bool unused, u32 saveId); // 0x001B9810
    // the mount count drops; at 0 the archive is unmounted
    DECOMP_NOINLINE static void Unmount(); // 0x001B8E7C
    // the access of a function that mounted ends (the archive is unmounted, nothing for the SD card)
    DECOMP_EMIT_COPY DECOMP_NOINLINE static void EndAccess() // 0x001B9D2C
    {
        switch (s_State.device) {
        case DEVICE_SDMC:
            Func56B888();
            break;
        case DEVICE_SAVE_DATA:
            Unmount();
            break;
        }
    }
    // ... the same if the archive is mounted (nothing for DEVICE_SAVE_DATA_2)
    static void EndAccessIfMounted(); // 0x001B9D58
    // the save data is committed (with updateSecureValue: the secure value goes up and is written with the commit)
    static s32 Commit(bool updateSecureValue); // 0x001B99BC
    // the save data archive is formatted (on the SD card: the files are deleted)
    static void Format(); // 0x001B9A6C
    // all save files are deleted (mount: the archive is mounted for it); the last failure
    static s32 DeleteAllFiles(bool mount); // 0x001B9D98
    // the files of the player of this console are deleted
    static s32 DeletePlayerFiles(); // 0x001B8F34
    static bool IsExistFile(const char* name, bool mount); // 0x001B9BB4
    // the whole save file is read into the save data
    static s32 ReadGardenPlus(); // 0x001B9C5C

    // the running write (the save I/O checks of the access functions)
    static void BeginSave(); // 0x001B890C
    static void EndSave(); // 0x001B8A38
    static bool IsSaving(); // 0x001B8B6C
    static u32 GetSize(); // 0x001B9E7C
    static u32 GetOffset(); // 0x001B9E8C
    static void SetDevice2(); // 0x001B8A24
    static void SetAccessible(); // 0x001B8EC4

    // the writes of the save kinds (see SaveProcess)
    static s32 WriteGardenPlus(); // 0x001B9C8C kind 1: the whole file
    static s32 WriteNothing(); // 0x001B8ED8 kind 2
    static s32 WritePlayer(); // 0x001B8D74 kind 3: the census and the player (network game: the census first)
    static s32 WriteNpcData(); // 0x001B9CD0 kind 4
    static s32 WriteGardenData(); // 0x001B8A4C kind 5
    static s32 WriteSharedUnk0(); // 0x001B8EE0 kind 6: the first 0x24 bytes of the shared data
    static s32 WriteMinigameData(); // 0x001B89C4 kind 13
    static s32 WriteUnk52BB0(); // 0x001B8AB4 kind 14
    static s32 WriteSharedUnk24(); // 0x001B8B14 kind 16
    static s32 WriteCensus(); // 0x001B8954 kind 17
    static s32 WriteAmiibo(); // 0x001B8920 kind 18
    static s32 WriteSharedData(); // 0x001B8D18 kind 19

    static State s_State;
    // the files deleted by DeleteAllFiles (set by the static initializer from the names of the other files)
    static const char* s_FileNames[FILE_NUM];
    // the files of the players: s_PlayerFileNames[file][player]
    static const char* const* s_PlayerFileNames[PLAYER_FILE_NUM];
};
ASSERT_SIZE(SvSaveIo::State, 0x14);



// (other files of the game; names are ours) cleared by Format
extern u8 g_Unknown9510F0;
extern u8 g_Unknown9510F1;
