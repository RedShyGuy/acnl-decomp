#include "Sv/dSvProc.h"

#include "Other/dProcThread.h"
#include "Other/dRegion.h"
#include "Sound/dSoundMgr.h"
#include "Bs/dBsSvMgr.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "Sv/dSvGlobals.h"
#include "Sv/dSvHolder.h"
#include "Sv/dSvProcExtern.h"
#include "Sv/dSvSaveMemory.h"
#include "Sv/dSvSaveIo.h"
#include "sead/seadHeap.h"
#include "sead/seadHeapMgr.h"
#include "event/event_Api.h"
#include "netgame/dGameFunctions.h"
#include "netgame/dGameGlobals.h"
#include "script/dScriptExtern.h"
#include "stage/dTransitionMgr.h"
#include "stage/stage_Api.h"
#include "time/time_Api.h"

extern const f32 FIELD_UNIT_SIZE_X;

// The globals of the file (names are ours). The static initializer builds them together with g_SvProc and the dummy
// request (and registers their destructors).
// 0x007855E8 (static initializer)

// 0x0095D35C
u8 g_SvIsSaving = 0;
// 0x0095D35D
u8 g_Unknown95D35D = 0;
// 0x0095D35E
bool g_SvIsArchiveMounted = false;
// 0x0095D35F
bool g_SvWasHeapLockEnabled = false;
// 0x0095D360
u32 g_SvIdentity = 0;
// 0x0095D364
const char* g_SvIdentityFileName = "/identity";
// 0x0095D368
ProcThread* g_pSvProcThread = NULL;
// 0x0095D36C
f32 g_Unknown95D36C = static_cast<u16>(FIELD_UNIT_SIZE_X * 0.2f);

// 0x005CE220
// 0x005CE21C (deleting dtor)
SvProc::~SvProc()
{
    // empty (in the original too)
}

// the instance
// 0x0095D370
SvProc g_SvProc;

// 0x00AD6638
SvStepInner g_SvDummyRequest(SvStepInner::FLAG_KEEP_ERROR);

namespace {
// (inline) the ended thread is destroyed and the state before it is restored
DECOMP_ALWAYS_INLINE void ReleaseSaveThread()
{
    if (g_SvIsSaving) {
        g_SvIsSaving = false;
    }
    g_pSvProcThread->destroy();
    delete g_pSvProcThread;
    g_pSvProcThread = NULL;
    if (time::IsFrozen()) {
        time::Unfreeze();
    }
    sead::Heap* heap = sead::HeapMgr::s_pInstance->getCurrentHeap();
    if (g_SvWasHeapLockEnabled) {
        heap->mFlag |= sead::Heap::FLAG_ENABLE_LOCK;
    } else {
        heap->mFlag &= ~sead::Heap::FLAG_ENABLE_LOCK;
    }
}

DECOMP_ALWAYS_INLINE bool IsSaveThreadEnded()
{
    const s32 state = g_pSvProcThread->mState;
    return state == sead::Thread::STATE_TERMINATED || state == sead::Thread::STATE_RELEASED;
}
} // namespace

// 0x00126580 (name is ours)
void SvReleaseFinishedSaveThread()
{
    if (g_pSvProcThread != NULL && IsSaveThreadEnded()) {
        ReleaseSaveThread();
    }
}

// 0x004DC2C4 (name is ours)
bool SvTryReleaseSaveThread()
{
    if (g_pSvProcThread != NULL) {
        if (!IsSaveThreadEnded()) {
            return false;
        }
        ReleaseSaveThread();
    }
    return true;
}

// 0x005CDDC4 (name is ours)
void SvEndAccessIfIdle()
{
    if (g_pSvProcThread == NULL) {
        SvSaveIo::EndAccessIfMounted();
        g_SvIsArchiveMounted = false;
    }
}

namespace {
// (inline) the region of the console is stored in the town: the region of the game in the low four bits, its region
// index in the high four bits
DECOMP_ALWAYS_INLINE void UpdateTownRegion()
{
    u8& value = GetGardenData()->townData.unk_0xEDAA;
    const s32 region = GetRegion();
    value = (value & ~0xF) | (region & 0xF);
    value = (value & ~0xF0) | ((GetRegionIndexOf(region) << 4) & 0xF0);
}

// (inline) the steps at the end of every apply function
DECOMP_ALWAYS_INLINE void ResetGameState()
{
    Func5D1080();
    Func28554C();
    SoundMgr::SUpdateTownMelody();
    Func226DA0();
    netgame::Func582F4C();
    netgame::Func30048C(1, 1);
    g_Unknown94EDD4[0] = 4;
    g_Unknown94EDD4[1] = 0;
    g_Unknown94EDD4[2] = 0;
}

// (inline) the town map of the index is started when there is none yet
DECOMP_ALWAYS_INLINE void InitializeTownMap()
{
    if (g_SvProc.unk_0x4 < 0) {
        Func27D8CC();
        g_SvProc.unk_0x4 = 0;
        Func27C884(&GetGardenData()->townData.unk_0x5C, g_SvProc.unk_0x4);
    }
}

DECOMP_ALWAYS_INLINE SvNpc* GetNpcs()
{
    return GetGardenData()->GetNpcData()->npcs;
}

// (inline) the data counts as loaded
DECOMP_ALWAYS_INLINE void MarkLoaded()
{
    GetGardenData()->header.saveVerifier2 = 2;
    SvGetSaveMemoryChecked()->secureValue.saveInitialized = 1;
}

// (inline) the common part of the kinds 1, 3, 6 and 8
DECOMP_ALWAYS_INLINE void ApplyTown()
{
    Func20DE60(&GetGardenData()->townData.currentTime);
    GetGardenData()->Func5CBCAC();
    Func1E08AC(GetSharedData());
    Func51D3F4();
}

DECOMP_ALWAYS_INLINE void ApplyFirstDay()
{
    if (Func5C58D0()) {
        Func24DFF4();
        Func1DEDF0();
    }
    Func304D04();
}

// (inline) the kind 7 (also the kind 11)
DECOMP_ALWAYS_INLINE void ApplyKind7()
{
    Func6A6EE0();
    Func2422F0();
    Func5C06A4();
    netgame::Func3043F4(netgame::g_UnknownA83000);
    UpdateTownRegion();
    ResetGameState();
    Func537AD8();
    Func10F168(GetNpcs());
    Func116D0C();
    g_SvProc.Func5CDB2C();
    Func30C97C(GetNpcs(), 0, 0);
}

DECOMP_ALWAYS_INLINE SvMovePlayerPls* GetMovePlayerPls()
{
    return g_SvHasMovePlayerPls ? g_pSvMovePlayerPls : NULL;
}
} // namespace

// 0x005CD094 (name is ours)
void SvApplyKind10()
{
    Func6A6EE0();
    Func2422F0();
    UpdateTownRegion();
    ResetGameState();
    Func114D18(0);
    Func6ABAE4();
    Func4E8EF0();
    Func51D910();
    g_SvProc.Func5CDB2C();
    Func10CCA8(GetNpcs());
}

// 0x005CD154 (name is ours)
void SvApplyKind1()
{
    InitializeTownMap();
    Func6A6EE0();
    Func2422F0();
    UpdateTownRegion();
    ApplyFirstDay();
    ApplyTown();
    Func1082E4(GetGardenData()->townData.unk_0x1AE94.bBoardMessages);
    ResetGameState();
    Func10F168(GetNpcs());
}

// 0x005CD2FC (name is ours)
void SvProc::Func5CD2FC()
{
    Func27D8CC();
    unk_0x4 = -1;
}

// 0x005CD314 (name is ours)
void SvApplyKind6()
{
    Func27D8CC();
    Func27C884(&GetGardenData()->townData.unk_0x5C, 0);
    Func6A6EE0();
    Func2422F0();
    Func5C06A4();
    netgame::Func3043F4(netgame::g_UnknownA83000);
    UpdateTownRegion();
    // "タイトルデモ" (title demo; the image of the array lies behind the table of the apply
    // functions, 0x008902F8)
    const char16 townName[10] = {0x30BF, 0x30A4, 0x30C8, 0x30EB, 0x30C7, 0x30E2, 0, 0, 0, 0};
    GetGardenData()->header.saveVerifier2 = 0;
    const s32 region = GetRegion();
    Func6BCA08(&GetGardenData()->townData.townData1, townName, region);
    ApplyTown();
    ResetGameState();
    Func10F168(GetNpcs());
    event::RequestReset();
    Func116D0C();
    g_SvProc.Func5CDB2C();
    Func30C97C(GetNpcs(), 0, 0);
}

// 0x005CD478 (name is ours)
void SvApplyKind11()
{
    if (!Func150260()) {
        SvApplyKind6();
        return;
    }
    ApplyKind7();
}

// 0x005CD568 (name is ours)
void SvApplyKind9()
{
    Func116D0C();
    g_SvProc.Func5CDB2C();
    Func30C97C(GetNpcs(), 0, 0);
    MarkLoaded();
    for (u32 i = 0; i < 4; i++) {
        SvPlayer* player = GetGardenData()->players.Get(static_cast<u8>(i));
        if (player != NULL && player->playerInfo.IsValid() && player->playerFlags.unk_0x12_5) {
            GetGardenData()->townData.townFlags.hhsStandUnlocked = 1;
        }
    }
}

// 0x005CD630 (name is ours)
void SvApplyKind4()
{
    if (time::IsYearClamped()) {
        time::SaveOffset();
    }
    Func6A6EE0();
    Func2422F0();
    UpdateTownRegion();
    ApplyFirstDay();
    ResetGameState();
    Func537AD8();
    Func10F168(GetNpcs());
    Func116D0C();
    SvGetCurrentPlayer();
    Func537EF8();
    g_SvProc.Func5CDB2C();
    SvNpcData* npcData = GetGardenData()->GetNpcData();
    Func30C97C(npcData->npcs, 0, 0);
    Func30C48C(npcData->unk_0x172F8);
}

// 0x005CD7A4 (name is ours)
void SvApplyKind7()
{
    ApplyKind7();
}

// 0x005CD880 (name is ours)
void SvApplyKind3()
{
    InitializeTownMap();
    Func6A6EE0();
    Func2422F0();
    UpdateTownRegion();
    ApplyFirstDay();
    ApplyTown();
    Func1082E4(GetGardenData()->townData.unk_0x1AE94.bBoardMessages);
    ResetGameState();
    Func10F168(GetNpcs());
    Func116D0C();
    g_SvProc.Func5CDB2C();
    Func30C97C(GetNpcs(), 0, 0);
    MarkLoaded();
}

// 0x005CDADC (name is ours)
void SvApplyKind2()
{
    Func116D0C();
    g_SvProc.Func5CDB2C();
    Func30C97C(GetNpcs(), 0, 0);
    MarkLoaded();
}

// 0x005CDB2C (name is ours)
void SvProc::Func5CDB2C()
{
    GetGardenData()->townData.unk_0xEE2C.Func1144A0(0);
    Func10E9A4(GetNpcs());
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL) {
        player->playerFlags.resetPending = 1;
    }
}

// 0x005CDB74 (name is ours)
void SvReturnToLoad()
{
    g_Unknown95D35D = 1;
    Func2904BC();
    stage::Transition(stage::NAME_LOAD, 0, 0, 1, 1);
}

// 0x005CDBA8 (name is ours)
void SvProc::ApplyMovePlayerPls()
{
    Func6BD94C(GetGardenData()->players.Get(0), &GetMovePlayerPls()->player);
    Func24E16C(Func3180C8(&GetGardenData()->townData.playerHouses, 0), &GetMovePlayerPls()->house);
    Func6CDAB8(Func1DF90C(GetSharedData(), 0), GetMovePlayerPls()->unk_0xB6B0);
    if (GetMovePlayerPls()->unk_0xC8B0) {
        GetGardenData()->townData.townFlags.unk_0xD_3 = 1;
        GetGardenData()->townData.townFlags.unk_0xD_7 = 1;
        GetGardenData()->players.Get(0)->playerFlags.unk_0x2B_4 = 1;
        GetGardenData()->players.Get(0)->townSlot = 0;
    }
    const SvMovePlayerPls* data = GetMovePlayerPls();
    SvGetSaveMemoryChecked();
    Func2BF890(&GetGardenData()->minigameData, data->unk_0xBC84, 0);
    data = GetMovePlayerPls();
    SvGetSaveMemoryChecked();
    Func2D49E8(&GetGardenData()->unk_0x52BB0, data->unk_0xC6C0, 0);
}

// 0x005CDDEC (name is ours)
void SvApplyKind5()
{
    if (stage::GetCurrent() == stage::NAME_PLAYER_SELECT) {
        if (time::IsYearClamped()) {
            time::SaveOffset();
        }
        Func27E6F8();
    }
    Func6A6EE0();
    Func2422F0();
    UpdateTownRegion();
    ResetGameState();
    Func537AD8();
    Func27AB50(&GetGardenData()->players);
    for (u32 i = 0; i < 4; i++) {
        SvPlayer* player = SvGetPlayer(static_cast<PlayerSaveNumber>(i & 0xFF));
        if (player == NULL || !player->playerInfo.IsValid()) {
            continue;
        }
        void* house = Func5C2320(player);
        if (house == NULL || Func76E7C0(player)) {
            continue;
        }
        if (player->playerFlags.unk_0xE_4 || player->playerFlags.unk_0xE_5) {
            continue;
        }
        bool found = false;
        for (u32 j = 0; j < 6; j++) {
            void* entry = Func3180E4(house, static_cast<u8>(j));
            if (!Func305240(entry) || Func720FC0(entry)) {
                found = true;
                break;
            }
        }
        if (player->playerFlags.unlockedSecretStorage && !found) {
            player->playerFlags.unk_0xE_4 = 1;
        }
    }
    Func116D0C();
    Func6ABAE4();
    Func4E8EF0();
    netgame::g_UnknownAAFF14 &= ~4;
    Func51D910();
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL) {
        player->Func6BE93C();
    }
    Func537EF8();
    g_SvProc.Func5CDB2C();
    Func5D07C0();
    Func304D04();
    Func3052C4();
    Func305080();
}

// 0x005CE080 (name is ours)
void SvApplyKind8()
{
    InitializeTownMap();
    g_SvProc.ApplyMovePlayerPls();
    Func6A6EE0();
    Func2422F0();
    UpdateTownRegion();
    ApplyTown();
    g_SvProc.ApplyMovePlayerPls();
    Func1082E4(GetGardenData()->townData.unk_0x1AE94.bBoardMessages);
    Func1DEE04();
    ResetGameState();
    Func10F168(GetNpcs());
}
