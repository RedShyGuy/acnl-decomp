#include "Bs/dBsPartsSaveMgr.h"

#include "Other/dHeapInit.h"
#include "Other/dSaveProcess.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGlobals.h"
#include "Sv/dSvHolder.h"
#include "Sv/dSvSaveIo.h"
#include "sead/seadExpHeap.h"
#include "sead/seadNew.h"

// The files of the "Move In" character from another town (game code; names are ours).

// 0x002F3964 (name is ours)
void SvCheckMovePlayerFiles()
{
    g_Unknown9510F0 = SvSaveIo::IsExistFile(g_SvMovePlayerPlsFileNames[0], true);
    if (!g_Unknown9510F0 && SvSaveIo::IsExistFile(g_SvMovePlayerFileName, true)) {
        g_Unknown9510F0 = 1;
        g_Unknown9510F1 = 1;
    }
}

// 0x002F39B4 (name is ours)
void SvDeleteMovePlayerFiles()
{
    SvSaveIo::DeleteFile(g_SvMovePlayerPlsFileNames[0]);
    SvSaveIo::DeleteFile(g_SvMovePlayerFileName);
    g_Unknown9510F0 = 0;
    g_Unknown9510F1 = 0;
}

// 0x002F39E8 (name is ours)
void SvStartLoadMovePlayerPls(sead::Heap* heap)
{
    if (g_SvMovePlayerPlsRequest.m_Result != SvStepInner::RESULT_RUNNING && !g_SvHasMovePlayerPls) {
        g_SvMovePlayerPlsRequest.Reset();
        g_SvMovePlayerPlsRequest.m_Flags =
            SvStepInner::FLAG_KEEP_ERROR | SvStepInner::FLAG_CHECK | SaveProcess::LOAD_FILE_MOVE_PLAYER_PLS;
        if (g_pSvMovePlayerPls == NULL) {
            if (heap == NULL) {
                heap = HeapInit::s_pStageHeap;
            }
            g_pSvMovePlayerPls = new (heap, g_SvMovePlayerPlsAlignment) SvMovePlayerPls();
            g_SvMovePlayerPlsSaveNo = SvGetPlayerSaveNo();
            if (!SvIsValidPlayerSaveNo(static_cast<PlayerSaveNumber>(g_SvMovePlayerPlsSaveNo))) {
                g_SvMovePlayerPlsSaveNo = 0;
            }
        }
    }
    // only the old mvplayer.dat: the step 8 of ModuleIncSave first
    if (g_Unknown9510F1) {
        g_SvMovePlayerPlsRequest.m_Flags = SvStepInner::FLAG_KEEP_ERROR | SvStepInner::FLAG_CHECK | (8ull << 18) |
                                           SaveProcess::LOAD_FILE_MOVE_PLAYER_PLS;
    }
}
