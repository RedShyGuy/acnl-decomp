#include "Sv/dSvSave.h"

#include "Bs/dBsPartsSaveMgr.h"
#include "Sv/dSvGlobals.h"
#include "Sv/dSvHolder.h"
#include "Sv/dSvIncludeSave.h"
#include "Sv/dSvProc.h"
#include "Sv/dSvSaveIo.h"
#include "Sv/dSvSaveMemory.h"
#include "Sv/dSvStepInner.h"
#include "Other/dSaveProcess.h"
#include "nn/fs/detail/detail_Api.h"
#include "sead/seadFormatFixedSafeString.h"

namespace {
// (inline) a file of the save data archive is deleted
DECOMP_ALWAYS_INLINE void DeleteFile(const char* path)
{
    nn::fs::detail::GetGlobalFileSystemBase()->TryDeleteFile(path);
}

// the functions of the save kinds (index kind - 1; NULL: none)
// 0x0090C9F0
const sv::SaveFunction s_BeginFunctions[sv::SAVE_KIND_NUM] = {
    SvSaveIo::BeginSave, SvSaveIo::BeginSave, SvSaveIo::BeginSave, SvSaveIo::BeginSave, SvSaveIo::BeginSave,
    SvSaveIo::BeginSave, SvExhibitionHolder::BeginSave, BsPartsSaveMgr<svholder::MyDesign>::BeginSave,
    BsPartsSaveMgr<svholder::Mail>::BeginSave, BsPartsSaveMgr<SvBestFriendList>::BeginSave, SvBeginSaveMovePlayerPls, NULL,
    SvSaveIo::BeginSave, SvSaveIo::BeginSave, NULL, SvSaveIo::BeginSave, SvSaveIo::BeginSave, NULL, SvSaveIo::BeginSave,
};
// 0x0090CA3C
const sv::SaveFunction s_PrepareFunctions[sv::SAVE_KIND_NUM] = {
    SvPrepareSaveAll,
    SvPrepareSaveNothing,
    SvPrepareSavePlayer,
    SvUpdateNpcDataChecksum,
    SvPrepareSaveGardenPlus,
    SvUpdateSharedUnk0Checksum,
    SvPrepareSaveExhibition,
    SvPrepareSaveMyDesign,
    SvPrepareSaveMail,
    SvPrepareSaveBestFriendList,
    SvPrepareSaveMovePlayerPls,
    NULL,
    SvUpdateMinigameDataChecksum,
    SvUpdateUnk52BB0Checksum,
    NULL,
    SvUpdateSharedUnk24Checksum,
    SvUpdateCensusChecksum,
    SvPrepareSaveAmiibo,
    SvUpdateSharedDataChecksums,
};
// 0x0090CA88
const sv::SaveFunction s_EndFunctions[sv::SAVE_KIND_NUM] = {
    SvSaveIo::EndSave, SvSaveIo::EndSave, SvSaveIo::EndSave, SvSaveIo::EndSave, SvSaveIo::EndSave,
    SvSaveIo::EndSave, SvExhibitionHolder::EndSave, BsPartsSaveMgr<svholder::MyDesign>::EndSave,
    BsPartsSaveMgr<svholder::Mail>::EndSave, BsPartsSaveMgr<SvBestFriendList>::EndSave, SvEndSaveMovePlayerPls, NULL,
    SvSaveIo::EndSave, SvSaveIo::EndSave, NULL, SvSaveIo::EndSave, SvSaveIo::EndSave, NULL, SvSaveIo::EndSave,
};
// 0x0090CAD4
const sv::WriteFunction s_WriteFunctions[sv::SAVE_KIND_NUM] = {
    SvSaveIo::WriteGardenPlus,
    SvSaveIo::WriteNothing,
    SvSaveIo::WritePlayer,
    SvSaveIo::WriteNpcData,
    SvSaveIo::WriteGardenData,
    SvSaveIo::WriteSharedUnk0,
    SvExhibitionHolder::WriteFile,
    BsPartsSaveMgr<svholder::MyDesign>::Write,
    BsPartsSaveMgr<svholder::Mail>::Write,
    BsPartsSaveMgr<SvBestFriendList>::Write,
    SvWriteMovePlayerPls,
    SvWriteIdentity,
    SvSaveIo::WriteMinigameData,
    SvSaveIo::WriteUnk52BB0,
    Func7B2C88,
    SvSaveIo::WriteSharedUnk24,
    SvSaveIo::WriteCensus,
    SvSaveIo::WriteAmiibo,
    SvSaveIo::WriteSharedData,
};
} // namespace

// 0x0070F2BC (name is ours)
bool SvSave(SaveProcess* process, u64 flags)
{
    const u64 kind = flags & SvStepInner::FLAG_SAVE_KIND_MASK;
    if (kind == 0) {
        return true;
    }
    const s32 index = static_cast<s32>(kind >> 11) - 1;
    if (s_BeginFunctions[index] != NULL) {
        s_BeginFunctions[index]();
    }
    if (s_PrepareFunctions[index] != NULL) {
        s_PrepareFunctions[index]();
    }
    SvCreateSaveCopy();
    const s32 result = s_WriteFunctions[index]();
    SvDestroySaveCopy();
    if (s_EndFunctions[index] != NULL) {
        s_EndFunctions[index]();
    }
    if (kind == (1 << 11) && g_Unknown9510F0) {
        SvDeleteMovePlayerFiles();
    }
    if (flags & SvStepInner::FLAG_SAVE_FRIENDS) {
        return SvSave(process, 10 << 11);
    }
    if (result == SvSaveIo::RESULT_OK) {
        return true;
    }
    if (result == SvSaveIo::RESULT_BROKEN) {
        g_SvErrorCode = 12;
    } else if (result != SvSaveIo::RESULT_NOT_FOUND) {
        g_SvErrorCode = 11;
    }
    return false;
}

// 0x0070F3B4 (name is ours)
bool SvSaveExtra(SaveProcess* process, u64 flags)
{
    (void)process;
    if (flags & SvStepInner::FLAG_EXTRA) {
        Func5CB274(SvGetSaveMemoryChecked());
    }
    const u64 kind = flags & SvStepInner::FLAG_SAVE_KIND_MASK;
    if (kind == (1 << 11) || kind == (5 << 11)) {
        if (!(flags & SvStepInner::FLAG_SAVE_FIRST)) {
            Func5CBAA0(SvGetSaveMemoryChecked());
        }
        if (kind == (1 << 11)) {
            nn::fs::detail::GetGlobalFileSystemBase()->TryCreateFile("data:/updated.dat", 40);
            {
                sead::FormatFixedSafeString<128> path("data:%s", g_SvGardenFileName);
                DeleteFile(path.getCString());
            }
            {
                sead::FormatFixedSafeString<128> path("data:%s", g_SvIdentityFileName);
                DeleteFile(path.getCString());
            }
        }
    }
    return true;
}
