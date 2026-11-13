#include "Other/dSaveProcess.h"

#include "Bs/dBsPartsSaveMgr.h"
#include "Other/dHeapInit.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGlobals.h"
#include "Sv/dSvHolder.h"
#include "Sv/dSvIncludeSave.h"
#include "Sv/dSvProc.h"
#include "Sv/dSvSave.h"
#include "Sv/dSvSaveIo.h"
#include "Sv/dSvSaveMemory.h"
#include "Sv/dSvStepInner.h"
#include "netgame/netgame_Api.h"
#include "nn/util/util_Crc32.h"
#include "sead/seadExpHeap.h"
#include "time/time_Api.h"

namespace {
// how the loaded data is taken over (SvStepInner::FLAG_APPLY_MASK 1..11)
// 0x008902CC
void (*const s_ApplyFunctions[])() = {
    SvApplyNewTown, SvApplyKind2, SvApplyKind3, SvApplyNewResident, SvApplyGameStart, SvApplyTitleDemoTown,
    SvApplyLoadedTown, SvApplyNewTownWithMovePlayer, SvApplyKind9, SvApplyBeforeGrowup, SvApplyLoadedOrTitleDemoTown,
};

// (inline) the error code for a failed result of SvSaveIo
DECOMP_ALWAYS_INLINE void SetErrorCode(s32 result)
{
    if (result == SvSaveIo::RESULT_BROKEN) {
        g_SvErrorCode = 12;
    } else if (result != SvSaveIo::RESULT_NOT_FOUND) {
        g_SvErrorCode = 11;
    }
}

// (inline) the result of the request for a failed read of a part file
DECOMP_ALWAYS_INLINE void SetReadFailure(SvStepInner* request, s32 result)
{
    if (result == SvSaveIo::RESULT_BROKEN) {
        request->m_Result = 5;
        g_SvErrorCode = 12;
    } else if (result == SvSaveIo::RESULT_NOT_FOUND) {
        request->m_Result = 8;
    } else {
        request->m_Result = 12;
        g_SvErrorCode = 11;
    }
}

// (inline) the result of the request for a load of ModuleIncSave and the small files; true for RESULT_OK
DECOMP_ALWAYS_INLINE bool SetLoadResult(SvStepInner* request, s32 result)
{
    switch (result) {
    case SvSaveIo::RESULT_OK:
        request->m_Result = SvStepInner::RESULT_OK;
        return true;
    case SvSaveIo::RESULT_ARCHIVE_NOT_FOUND:
        request->m_Result = 3;
        break;
    case SvSaveIo::RESULT_BROKEN:
        request->m_Result = 5;
        break;
    case SvSaveIo::RESULT_NOT_FOUND:
        request->m_Result = 8;
        break;
    case SvSaveIo::RESULT_MOUNT_6:
        request->m_Result = 4;
        break;
    case SvSaveIo::RESULT_ARCHIVE_INVALIDATED:
        request->m_Result = 10;
        break;
    default:
        request->m_Result = 12;
        break;
    }
    return false;
}

// (inline) a part file of BsPartsSaveMgr<T> is read and checked
template <typename T>
DECOMP_ALWAYS_INLINE bool LoadPartFile(SvStepInner* request, u64 flags, const char* name, T* data, u32 size)
{
    const s32 result = SvSaveIo::ReadFile(name, 0, data, size);
    if (result != SvSaveIo::RESULT_OK) {
        SetReadFailure(request, result);
        return false;
    }
    if ((flags & SvStepInner::FLAG_CHECK) && !data->IsChecksumValid(true)) {
        request->m_Result = 5;
        g_SvErrorCode = 12;
        return false;
    }
    return true;
}
} // namespace

// 0x001D49D4 (name is ours)
// 0x001D49B4 (deleting dtor)
SaveProcess::~SaveProcess()
{
    if (m_pRequest != NULL) {
        m_pRequest->m_pProcess = NULL;
    }
}

// 0x001D3B44 (name is ours)
void SaveProcess::Finish(bool endAccess)
{
    if (endAccess && g_SvIsArchiveMounted) {
        SvSaveIo::EndAccess();
        g_SvIsArchiveMounted = false;
    }
    if (m_pRequest->m_Result == SvStepInner::RESULT_OK) {
        if (m_pRequest->m_OnSuccess != NULL) {
            m_pRequest->m_OnSuccess(m_pRequest->m_Result);
        }
    } else if (m_pRequest->m_OnFailure != NULL) {
        m_pRequest->m_OnFailure(m_pRequest->m_Result);
    }
    if (endAccess) {
        SvSaveIo::SetDevice2();
    }
    g_SvIsSaveMemoryLocked = 0;
    m_pRequest->m_IsDone = true;
}

// 0x001D3BCC (name is ours)
void SvPrepareSaveMail()
{
    svholder::Mail* mail = SvGetMailAlways();
    mail->checksum = mail->CalcChecksum();
}

// 0x001D3BE4 (name is ours)
void SvPrepareSaveAmiibo()
{
    u32* data = static_cast<u32*>(g_pSvAmiiboData);
    data[0] = nn::util::Crc32Msb::Calculate(data + 1, 292, 0);
}

// 0x001D3C0C (name is ours)
void SvPrepareSavePlayer()
{
    const PlayerSaveNumber saveNo = SvGetPlayerSaveNo();
    g_UnkAAFF18.pPlayer->UpdateChecksums(saveNo);
}

// 0x001D3C34 (name is ours)
void SvPrepareSaveGardenPlus()
{
    GetGardenPlus()->UpdateChecksums();
}

// 0x001D3C44 (name is ours)
s32 SvWriteIdentity()
{
    return SvSaveIo::WriteFile(g_SvIdentityFileName, 0, &g_SvIdentity, sizeof(g_SvIdentity), false);
}

// 0x001D3C70 (name is ours)
void SvPrepareSaveMyDesign()
{
    svholder::MyDesign* myDesign = SvGetMyDesignAlways();
    myDesign->checksum = myDesign->CalcChecksum();
}

// 0x001D3C88 (name is ours)
void SvPrepareSaveNothing()
{
    // empty (in the original too)
}

// 0x001D3C8C (name is ours)
void SvPrepareSaveBestFriendList()
{
    SvBestFriendList* list = SvGetBestFriendListAlways();
    list->checksum = list->CalcChecksum();
}

// 0x001D3CA4 (name is ours)
void SvPrepareSaveExhibition()
{
    SvExhibition* exhibition = SvGetExhibitionAlways();
    exhibition->checksum = exhibition->CalcChecksum();
}

// 0x001D3CBC (name is ours)
void SvPrepareSaveMovePlayerPls()
{
    (g_SvHasMovePlayerPls ? g_pSvMovePlayerPls : NULL)->UpdateChecksum();
}

// 0x001D3CDC (name is ours)
void SaveProcess::Run()
{
    m_pRequest->m_Detail = -1;
    if (!(m_pRequest->m_Flags & SvStepInner::FLAG_KEEP_ERROR)) {
        g_SvErrorCode = -1;
    }
    const u64 includeSaveStep = m_pRequest->m_Flags & SvStepInner::FLAG_INCLUDE_SAVE_MASK;
    if (includeSaveStep != 0) {
        m_pRequest->m_Result = Func14FDF8(includeSaveStep);
        if (m_pRequest->m_Result != SvStepInner::RESULT_OK) {
            goto finish;
        }
    }
    if ((m_pRequest->m_Flags & SvStepInner::FLAG_SAVE_KIND_MASK) && netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        goto failSave;
    }
    if ((m_pRequest->m_Flags & SvStepInner::FLAG_MOUNT_MASK) && !g_SvIsArchiveMounted) {
        s32 result;
        if (m_pRequest->m_Flags & SvStepInner::FLAG_MOUNT_ACCESSIBLE) {
            result = SvSaveIo::Mount(false, false, 0x863);
        } else if (m_pRequest->m_Flags & SvStepInner::FLAG_MOUNT_ACCESSIBLE_14F1) {
            result = SvSaveIo::Mount(false, false, 0x14F1);
        } else {
            result = SvSaveIo::Mount(false, false, 0);
        }
        if (result != SvSaveIo::RESULT_OK) {
            m_pRequest->m_Detail = (m_pRequest->m_Flags & SvStepInner::FLAG_LOAD_FILE_MASK) ? 8 : 6;
            SetErrorCode(result);
            switch (result) {
            case SvSaveIo::RESULT_ARCHIVE_NOT_FOUND:
                m_pRequest->m_Result = 3;
                break;
            case SvSaveIo::RESULT_BROKEN:
                m_pRequest->m_Result = 5;
                break;
            case SvSaveIo::RESULT_MOUNT_6:
                m_pRequest->m_Result = 4;
                break;
            default:
                m_pRequest->m_Result = 12;
                break;
            }
            goto finish;
        }
        g_SvIsArchiveMounted = true;
    }
    if (m_pRequest->m_Flags & SvStepInner::FLAG_DELETE_ALL) {
        SvSaveIo::DeleteAllFiles(false);
    }
    if (m_pRequest->m_Flags & SvStepInner::FLAG_EXHIBITION) {
        g_SvExhibitionHolder.Allocate();
        if (Load(LOAD_FILE_EXHIBITION)) {
            g_SvExhibitionHolder.SetLoaded(false);
        } else {
            // a missing file is made new
            if (m_pRequest->m_Result != 8) {
                m_pRequest->m_Detail = 8;
                goto finish;
            }
            g_SvExhibitionHolder.SetLoaded(true);
            m_pRequest->m_Result = SvStepInner::RESULT_OK;
        }
    }
    if (m_pRequest->m_BeforeLoad != NULL && !(m_pRequest->m_Flags & SvStepInner::FLAG_DELETE_PLAYER_FILES)) {
        if (!m_pRequest->m_BeforeLoad()) {
            goto fail;
        }
    }
    if (m_pRequest->m_Flags & SvStepInner::FLAG_FORMAT) {
        SvSaveIo::Format();
    }
    if (m_pRequest->m_Flags & SvStepInner::FLAG_SAVE_FIRST) {
        if (!SvSaveExtra(this, m_pRequest->m_Flags)) {
            goto fail;
        }
        if (!SvSave(this, m_pRequest->m_Flags)) {
            goto failSave;
        }
    }
    if (!Load(m_pRequest->m_Flags)) {
        m_pRequest->m_Detail = 8;
        goto finish;
    }
    if (m_pRequest->m_BeforeLoad != NULL && (m_pRequest->m_Flags & SvStepInner::FLAG_DELETE_PLAYER_FILES)) {
        if (!m_pRequest->m_BeforeLoad()) {
            goto fail;
        }
    }
    if (m_pRequest->m_AfterLoad != NULL && !m_pRequest->m_AfterLoad()) {
        m_pRequest->m_Result = 7;
        goto finish;
    }
    {
        const u64 apply = m_pRequest->m_Flags & SvStepInner::FLAG_APPLY_MASK;
        if (apply >= (1 << 6) && apply <= (11 << 6)) {
            const s32 index = static_cast<s32>(apply >> 6) - 1;
            sead::ExpHeap* heap = HeapInit::s_pStageHeap;
            if (heap != NULL) {
                while (!Func1C2530(Func3044DC(), heap)) {
                }
            }
            Func110CF0();
            s_ApplyFunctions[index]();
            Func139580(Func3044DC());
            g_SvIsSaveMemoryValid = 1;
        }
    }
    if (!(m_pRequest->m_Flags & SvStepInner::FLAG_SAVE_FIRST)) {
        if (!SvSaveExtra(this, m_pRequest->m_Flags)) {
            goto fail;
        }
        if (!SvSave(this, m_pRequest->m_Flags)) {
            goto failSave;
        }
    }
    if (m_pRequest->m_Flags & SvStepInner::FLAG_DELETE_PLAYER_FILES) {
        const s32 result = SvSaveIo::DeletePlayerFiles();
        if (result != SvSaveIo::RESULT_OK) {
            m_pRequest->m_Detail = 6;
            m_pRequest->m_Result = 12;
            SetErrorCode(result);
            goto finish;
        }
    }
    if (m_pRequest->m_AfterSave != NULL && !m_pRequest->m_AfterSave()) {
        goto fail;
    }
    if (m_pRequest->m_Flags & SvStepInner::FLAG_EXHIBITION) {
        if (!SvSave(this, 7 << 11)) {
            m_pRequest->m_Result = 12;
            g_SvExhibitionHolder.Free();
            m_pRequest->m_Detail = 6;
            goto finish;
        }
    }
    if (m_pRequest->m_BeforeCommit != NULL && !m_pRequest->m_BeforeCommit()) {
        m_pRequest->m_Result = 7;
        goto finish;
    }
    if (m_pRequest->m_Flags & SvStepInner::FLAG_UNMOUNT) {
        SvSaveIo::EndAccessIfMounted();
        g_SvIsArchiveMounted = false;
    }
    if (m_pRequest->m_Flags & SvStepInner::FLAG_EXHIBITION) {
        g_SvExhibitionHolder.Free();
    }
    if ((m_pRequest->m_Flags & SvStepInner::FLAG_COMMIT_MASK) && !(m_pRequest->m_Flags & SvStepInner::FLAG_NO_COMMIT)) {
        const s32 result = SvSaveIo::Commit(!(m_pRequest->m_Flags & SvStepInner::FLAG_KEEP_SECURE_VALUE));
        if (result != SvSaveIo::RESULT_OK) {
            m_pRequest->m_Result = 12;
            m_pRequest->m_Detail = 6;
            SetErrorCode(result);
            goto finish;
        }
    }
    if (m_pRequest->m_OnEnd != NULL) {
        m_pRequest->m_OnEnd();
    }
    if (!(m_pRequest->m_Flags & SvStepInner::FLAG_NO_COMMIT) && (m_pRequest->m_Flags & SvStepInner::FLAG_MOUNT_MASK)) {
        SvSaveIo::EndAccess();
        g_SvIsArchiveMounted = false;
    }
    m_pRequest->m_Result = SvStepInner::RESULT_OK;
    Finish(!(m_pRequest->m_Flags & SvStepInner::FLAG_NO_COMMIT));
    return;

fail:
    m_pRequest->m_Result = 12;
    goto finish;

failSave:
    m_pRequest->m_Result = 12;
    m_pRequest->m_Detail = 6;
finish:
    Finish(true);
}

// 0x001D42D0 (name is ours)
bool SaveProcess::Load(u64 flags)
{
    const u64 file = flags & SvStepInner::FLAG_LOAD_FILE_MASK;
    if (file == LOAD_FILE_GARDEN_PLUS) {
        g_SvIsSaveMemoryValid = 0;
        SvSaveIo::BeginSave();
        const s32 result = SvSaveIo::ReadGardenPlus();
        g_SvIsSaveMemoryValid = 1;
        if (result == SvSaveIo::RESULT_OK) {
            if (flags & SvStepInner::FLAG_CHECK) {
                SvGardenPlus* data = GetGardenPlus();
                if (!(static_cast<s32>(data->secureValue.saveInitialized) > g_SvSaveInitializedMin &&
                      data->secureValue.saveInitialized <= 1)) {
                    m_pRequest->m_Result = 11;
                    g_SvErrorCode = 37;
                    SvSaveIo::EndSave();
                    return false;
                }
                if (!GetGardenPlus()->IsValid(true) || !GetGardenPlus()->IsVersionValid()) {
                    // (once more; the logs of the results are removed)
                    GetGardenPlus()->IsValid(true);
                    GetGardenPlus()->IsVersionValid();
                    m_pRequest->m_Result = 5;
                    g_SvErrorCode = 12;
                    SvSaveIo::EndSave();
                    return false;
                }
                if (!SvCheckSecureValue()) {
                    m_pRequest->m_Result = 6;
                    g_SvErrorCode = 33;
                    SvSaveIo::EndSave();
                    return false;
                }
            }
            SvSaveIo::EndSave();
            time::LoadOffset();
            return true;
        }
        if (result == SvSaveIo::RESULT_NOT_FOUND) {
            m_pRequest->m_Result = 8;
            if (flags & SvStepInner::FLAG_TRY_INCLUDE_SAVE) {
                bool isSecureValueWrong;
                bool isBroken;
                if (Func14FE00(&isBroken, &isSecureValueWrong)) {
                    g_Unknown9516D1 = 1;
                } else if (isSecureValueWrong) {
                    m_pRequest->m_Result = 6;
                    g_SvErrorCode = 33;
                    return false;
                } else if (isBroken) {
                    m_pRequest->m_Result = 5;
                    g_SvErrorCode = 12;
                    SvSaveIo::EndSave();
                    return false;
                }
            }
        } else if (result == SvSaveIo::RESULT_BROKEN) {
            m_pRequest->m_Result = 5;
            g_SvErrorCode = 12;
        } else {
            m_pRequest->m_Result = 12;
            g_SvErrorCode = 11;
        }
        SvSaveIo::EndSave();
        return false;
    } else if (file == LOAD_FILE_EXHIBITION) {
        const s32 result = g_SvExhibitionHolder.Read();
        if (result != SvSaveIo::RESULT_OK) {
            SetReadFailure(m_pRequest, result);
            return false;
        }
        if ((flags & SvStepInner::FLAG_CHECK) && !g_SvExhibitionHolder.IsValid()) {
            m_pRequest->m_Result = 5;
            g_SvErrorCode = 12;
            return false;
        }
        return true;
    } else if (file == LOAD_FILE_MY_DESIGN) {
        return LoadPartFile(m_pRequest, flags,
                            BsPartsSaveMgr<svholder::MyDesign>::s_FileNames[BsPartsSaveMgr<svholder::MyDesign>::s_SaveNo],
                            BsPartsSaveMgr<svholder::MyDesign>::s_pData, sizeof(svholder::MyDesign));
    } else if (file == LOAD_FILE_MAIL) {
        return LoadPartFile(m_pRequest, flags,
                            BsPartsSaveMgr<svholder::Mail>::s_FileNames[BsPartsSaveMgr<svholder::Mail>::s_SaveNo],
                            BsPartsSaveMgr<svholder::Mail>::s_pData, sizeof(svholder::Mail));
    } else if (file == LOAD_FILE_BEST_FRIEND_LIST) {
        return LoadPartFile(m_pRequest, flags,
                            BsPartsSaveMgr<SvBestFriendList>::s_FileNames[BsPartsSaveMgr<SvBestFriendList>::s_SaveNo],
                            BsPartsSaveMgr<SvBestFriendList>::s_pData, sizeof(SvBestFriendList));
    } else if (file == LOAD_FILE_MOVE_PLAYER_PLS) {
        return LoadPartFile(m_pRequest, flags, g_SvMovePlayerPlsFileNames[g_SvMovePlayerPlsSaveNo], g_pSvMovePlayerPls,
                            sizeof(SvMovePlayerPls));
    } else if (file == LOAD_FILE_INCLUDE_SAVE_14) {
        return SetLoadResult(m_pRequest, Func14FE08());
    } else if (file == LOAD_FILE_IDENTITY) {
        return SetLoadResult(m_pRequest, SvSaveIo::ReadFile(g_SvIdentityFileName, 0, &g_SvIdentity, sizeof(g_SvIdentity)));
    } else if (file == LOAD_FILE_INCLUDE_SAVE_18) {
        return SetLoadResult(m_pRequest, Func14FE10());
    } else if (file == LOAD_FILE_AMIIBO) {
        const s32 result = SvSaveIo::ReadFile(g_SvAmiiboFileName, 0, g_pSvAmiiboData, 296);
        if (result == SvSaveIo::RESULT_OK) {
            u32* data = static_cast<u32*>(g_pSvAmiiboData);
            if (nn::util::Crc32Msb::Calculate(data + 1, 292, 0) != data[0]) {
                m_pRequest->m_Result = 5;
                return false;
            }
        }
        return SetLoadResult(m_pRequest, result);
    }
    return true;
}

// 0x005CDA94 (name is ours)
bool SvRunSaveProcess(SvStepInner* request)
{
    SaveProcess process(request);
    process.Run();
    return true;
}
