#pragma once

#include "decomp.h"
#include "Sv/dSvStepInner.h"

class ProcThread;

// RTTI 6SvProc @ 0x008CD314
// vtable 0x008F8CF0 (vptr 0x008F8CF8), offset_to_top 0, 2 entries: only the destructor
class SvProc
{
public:
    virtual ~SvProc();

    // (called before the growth processing and in BsSvMgr::Initialize) 0x0027D8CC runs and unk_0x4 becomes -1
    void Func5CD2FC(); // 0x005CD2FC
    // (after every load: game code and the flag resetPending of the current player)
    void Func5CDB2C(); // 0x005CDB2C
    // the "Move In" character of mvplayer_pls.dat becomes player 0 of the town: player, house and the other parts
    void ApplyMovePlayerPls(); // 0x005CDBA8

    s32 unk_0x4;    // 0x04 (-1: none; the apply kinds 1, 3 and 8 start 0x0027C884 with 0 when it is negative)
};
ASSERT_SIZE(SvProc, 0x8);

// the instance (name is ours)
extern SvProc g_SvProc;

// The globals of the file of SvProc around the save thread (all names are ours).
// a request with a save kind runs (set by SvStepInner::Start, cleared when the thread is released)
extern u8 g_SvIsSaving;
extern u8 g_Unknown95D35D;
// the save process mounted the archive
extern bool g_SvIsArchiveMounted;
// whether the current heap had FLAG_ENABLE_LOCK before the save thread was started (set again afterwards)
extern bool g_SvWasHeapLockEnabled;
// the content of the file "/identity" (4 bytes; file kind 16, save kind 12) and its name
extern u32 g_SvIdentity;
extern const char* g_SvIdentityFileName;
// the running save thread
extern ProcThread* g_pSvProcThread;
// (FIELD_UNIT_SIZE_X / 5, through u16; nothing in code.bin reads it)
extern f32 g_Unknown95D36C;
// a request that a running process is pointed at when its own request goes away
extern SvStepInner g_SvDummyRequest;

// the save thread is released when it has ended: destroyed, the time runs again, the heap lock flag is restored
// (false: it still runs)
bool SvTryReleaseSaveThread(); // 0x004DC2C4
// the step "save failed": back to the stage Load (g_Unknown95D35D is set)
void SvReturnToLoad(); // 0x005CDB74
// how the loaded data is taken over (SvStepInner::FLAG_APPLY_MASK 1..11, table of SaveProcess::Run): the data of the
// game is built again from the save data (names are ours; kind 5 loads with exhibition.dat, kind 6 names the town
// "タイトルデモ" (title demo), kind 8 takes over mvplayer_pls.dat, kind 11 is kind 7 or kind 6)
void SvApplyKind1(); // 0x005CD154
void SvApplyKind2(); // 0x005CDADC
void SvApplyKind3(); // 0x005CD880
void SvApplyKind4(); // 0x005CD630
void SvApplyKind5(); // 0x005CDDEC
void SvApplyKind6(); // 0x005CD314
void SvApplyKind7(); // 0x005CD7A4
void SvApplyKind8(); // 0x005CE080
void SvApplyKind9(); // 0x005CD568
void SvApplyKind10(); // 0x005CD094
void SvApplyKind11(); // 0x005CD478
// the access to the archive ends if no save thread runs
void SvEndAccessIfIdle(); // 0x005CDDC4
// (copies in the shared area)
DECOMP_EMIT_COPY DECOMP_NOINLINE inline bool SvIsSaveThreadAlive() // 0x00126568
{
    return g_pSvProcThread != NULL;
}
DECOMP_EMIT_COPY DECOMP_NOINLINE void SvReleaseFinishedSaveThread(); // 0x00126580
