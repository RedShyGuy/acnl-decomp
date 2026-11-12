#pragma once

#include "decomp.h"

class SaveProcess;

// A request to the save thread (a SaveProcess on a ProcThread): which files are loaded and saved (the flags),
// callbacks between the steps and the result. Only the class name is from the binary (RTTI; size after libgarden); all
// other names are ours.
// RTTI 11SvStepInner @ 0x008CB368
class SvStepInner
{
public:
    typedef bool (*StepCallback)();
    typedef void (*Callback)();
    typedef void (*ResultCallback)(s32 result);

    // the bits of the flags (see SaveProcess::Run)
    static const u64 FLAG_FORMAT = 1ull << 0;            // the save data archive is formatted first
    static const u64 FLAG_LOAD_FILE_MASK = 0x3Eull;      // the file that is loaded (bits 1..5, see SaveProcess::Load)
    static const u64 FLAG_APPLY_MASK = 0x3C0ull;         // how the loaded data is taken over (bits 6..9: 1..11)
    static const u64 FLAG_SAVE_KIND_MASK = 0xF800ull;    // the part that is saved (bits 11..15: 1..19, see SvSave)
    static const u64 FLAG_COMMIT = 1ull << 16;           // the save data is committed (also without a save kind)
    static const u64 FLAG_UNMOUNT = 1ull << 17;          // the archive is unmounted at the end
    static const u64 FLAG_INCLUDE_SAVE_MASK = 0x3C0000ull; // a step of ModuleIncSave first (bits 18..21)
    static const u64 FLAG_CHECK = 1ull << 22;            // the loaded data is checked
    static const u64 FLAG_KEEP_ERROR = 1ull << 23;       // the error code of the save (0x009516DC) is not reset
    static const u64 FLAG_CLEAR_SAVE_MEMORY = 1ull << 24; // the save data in memory is cleared before the thread runs
    static const u64 FLAG_EXTRA = 1ull << 25;            // see SvSaveExtra
    static const u64 FLAG_EXHIBITION = 1ull << 26;       // exhibition.dat is loaded (and saved) too
    static const u64 FLAG_NO_COMMIT = 1ull << 27;        // nothing is committed and the access is not ended
    static const u64 FLAG_DELETE_PLAYER_FILES = 1ull << 28; // the files of the player are deleted after the save
    static const u64 FLAG_SAVE_FRIENDS = 1ull << 29;     // the save kind 10 follows
    static const u64 FLAG_KEEP_SECURE_VALUE = 1ull << 30; // the commit does not raise the secure value
    static const u64 FLAG_MOUNT_ACCESSIBLE = 1ull << 31; // the archive is mounted with the save id 0x863
    static const u64 FLAG_MOUNT_ACCESSIBLE_14F1 = 1ull << 32; // ... with the save id 0x14F1
    static const u64 FLAG_TRY_INCLUDE_SAVE = 1ull << 33; // a missing garden_plus.dat asks ModuleIncSave
    static const u64 FLAG_SAVE_FIRST = 1ull << 36;       // the save comes before the load
    static const u64 FLAG_DELETE_ALL = 1ull << 37;       // all save files are deleted first
    // the flags that need the mounted archive / a commit
    static const u64 FLAG_MOUNT_MASK = 0x401F83Eull;
    // (Start: the save data in memory is locked while one of them is set; the mount mask without FLAG_COMMIT)
    static const u64 FLAG_LOCK_MASK = 0x400F83Eull;
    static const u64 FLAG_COMMIT_MASK = 0x401F800ull;
    // (Start: the time of the game is frozen while one of them is set)
    static const u64 FLAG_FREEZE_TIME_MASK = 0xFBFEull;
    // (Start: g_SvIsSaving is set)
    static const u64 FLAG_SAVING_MASK = 0x2400F800ull;

    // the results
    enum
    {
        RESULT_RUNNING = 1,
        RESULT_OK = 2,
    };

    SvStepInner(u64 flags); // 0x00304AA0
    // a process that still runs is pointed at the dummy request
    ~SvStepInner(); // 0x00304AE0
    // the defaults (a process that still runs is pointed at the dummy request)
    void Reset(); // 0x005CDD40
    // the save thread is made and started (false: a save thread still runs, an error is shown or the input is blocked)
    bool Start(); // 0x003047E8

    SaveProcess* m_pProcess;        // 0x00 the process that works on it
    u64 m_Flags;                    // 0x08
    StepCallback m_BeforeLoad;      // 0x10 before the load (with FLAG_DELETE_PLAYER_FILES after it); false: failed
    StepCallback m_AfterSave;       // 0x14
    ResultCallback m_OnSuccess;     // 0x18 with the result (RESULT_OK)
    ResultCallback m_OnFailure;     // 0x1C with the result
    StepCallback m_AfterLoad;       // 0x20
    StepCallback m_BeforeCommit;    // 0x24
    Callback m_OnEnd;               // 0x28
    s32 m_Result;                   // 0x2C RESULT_*
    bool m_IsDone;                  // 0x30
    s32 m_Detail;                   // 0x34 (-1, 6, 8)
};
ASSERT_SIZE(SvStepInner, 0x38);
