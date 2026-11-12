#include "Sv/dSvSaveIo.h"

#include "Bs/dBsPartsSaveMgr.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGlobals.h"
#include "Sv/dSvSaveMemory.h"
#include "netgame/netgame_Api.h"
#include "nn/err/CTR/CTR_Api.h"
#include "nn/fs/detail/detail_Api.h"
#include "nn/fs/fs_Api.h"
#include "sead/seadFileDevice.h"
#include "sead/seadFileDeviceMgr.h"
#include "sead/seadFileHandle.h"
#include "sead/seadFixedSafeString.h"

// 0x00952490
SvSaveIo::State SvSaveIo::s_State = {DEVICE_SAVE_DATA_2, false, false, 1, false, 0, 0, 0};
// the table of the files is set by the static initializer (the names are read from the globals of the other files)
// 0x00792258 (static initializer)
// 0x009524A4
const char* SvSaveIo::s_FileNames[FILE_NUM] = {
    g_SvGardenPlusFileName, "/exhibition.dat", g_SvMovePlayerPlsFileNames[0], g_SvAmiiboFileName, "updated.dat",
    g_SvGardenFileName, g_SvMovePlayerFileName,
};
// 0x009524C0
const char* const* SvSaveIo::s_PlayerFileNames[PLAYER_FILE_NUM] = {
    BsPartsSaveMgr<svholder::MyDesign>::s_FileNames,
    BsPartsSaveMgr<svholder::Mail>::s_FileNames,
    BsPartsSaveMgr<SvBestFriendList>::s_FileNames,
};

namespace {
// (inline) the result is one of the FS module with a description in the range
bool IsFsResult(nn::Result result, s32 begin, s32 end)
{
    return result.GetModule() == 17 && static_cast<s32>(result.GetDescription()) >= begin &&
           static_cast<s32>(result.GetDescription()) <= end;
}
} // namespace

// 0x001B85C4 (name is ours)
s32 SvSaveIo::DeleteFile(const char* name)
{
    sead::FixedSafeString<256> path;
    path.copy(sead::SafeStringBase<char>(s_State.device != DEVICE_SDMC ? "data:/" : "sdmc:/"));
    path.overwriteAt(-1, sead::SafeStringBase<char>(name), -1);
    nn::Result result = nn::fs::detail::GetGlobalFileSystemBase()->TryDeleteFile(path.getCString());
    if (result.IsSuccess() || IsFsResult(result, 100, 179)) {
        return RESULT_OK;
    }
    return ConvertResult(result);
}

// 0x001B890C (name is ours)
void SvSaveIo::BeginSave()
{
    s_State.isSaving = true;
}

// 0x001B8920 (name is ours)
s32 SvSaveIo::WriteAmiibo()
{
    return WriteFile(g_SvAmiiboFileName, 0, g_pSvAmiiboData, 0x128, false);
}

// 0x001B8954 (name is ours)
s32 SvSaveIo::WriteCensus()
{
    const u32 offset = reinterpret_cast<u8*>(&GetGardenPlus()->sharedData) - reinterpret_cast<u8*>(SvGetSaveMemory()) + 0xC0C;
    const u32 size = 0x1448;
    void* data = SvPrepareSaveWrite(reinterpret_cast<u8*>(&GetGardenPlus()->sharedData) + 0xC0C, size, offset);
    return WriteFile(g_SvGardenPlusFileName, offset, data, size, false);
}

// 0x001B89C4 (name is ours)
s32 SvSaveIo::WriteMinigameData()
{
    const u32 offset = SvGetMinigameDataOffset();
    SvGetSaveMemoryChecked();
    const u32 size = sizeof(SvMinigameData);
    void* data = SvPrepareSaveWrite(&GetGardenData()->minigameData, size, offset);
    return WriteFile(g_SvGardenPlusFileName, SvGetMinigameDataOffset(), data, size, false);
}

// 0x001B8A24 (name is ours)
void SvSaveIo::SetDevice2()
{
    s_State.device = DEVICE_SAVE_DATA_2;
}

// 0x001B8A38 (name is ours)
void SvSaveIo::EndSave()
{
    s_State.isSaving = false;
}

// 0x001B8A4C (name is ours)
s32 SvSaveIo::WriteGardenData()
{
    GetGardenPlus();
    const u32 size = SvGetGardenDataSaveSize();
    const u32 offset = SvGetGardenDataOffset();
    GetGardenPlus();
    const u32 prepareSize = SvGetGardenDataSaveSize();
    void* data = SvPrepareSaveWrite(&GetGardenPlus()->data, prepareSize, offset);
    return WriteFile(g_SvGardenPlusFileName, SvGetGardenDataOffset(), data, size, false);
}

// 0x001B8AB4 (name is ours)
s32 SvSaveIo::WriteUnk52BB0()
{
    const u32 offset = SvGetUnk52BB0Offset();
    SvGetSaveMemoryChecked();
    const u32 size = sizeof(SvGardenDataUnk52BB0);
    void* data = SvPrepareSaveWrite(&GetGardenData()->unk_0x52BB0, size, offset);
    return WriteFile(g_SvGardenPlusFileName, SvGetUnk52BB0Offset(), data, size, false);
}

// 0x001B8B14 (name is ours)
s32 SvSaveIo::WriteSharedUnk24()
{
    const u32 offset = SvGetSharedUnk24Offset();
    const u32 size = 0xBE8;
    void* data = SvPrepareSaveWrite(SvGetSharedUnk24(), size, offset);
    return WriteFile(g_SvGardenPlusFileName, SvGetSharedUnk24Offset(), data, size, false);
}

// 0x001B8B6C (name is ours)
bool SvSaveIo::IsSaving()
{
    return s_State.isSaving;
}

// 0x001B8B7C (name is ours)
sead::FileDevice* SvSaveIo::GetDevice()
{
    sead::FixedSafeString<256> drive;
    drive.copy(sead::SafeStringBase<char>(s_State.device != DEVICE_SDMC ? "backup" : "sd"));
    return sead::FileDeviceMgr::s_pInstance->findDevice(drive);
}

// 0x001B8D18 (name is ours)
s32 SvSaveIo::WriteSharedData()
{
    const u32 offset = SvGetSharedDataOffset();
    const u32 size = 0x181E0;
    void* data = SvPrepareSaveWrite(&GetGardenPlus()->sharedData, size, offset);
    return WriteFile(g_SvGardenPlusFileName, SvGetSharedDataOffset2(), data, size, false);
}

// 0x001B8D74 (name is ours)
s32 SvSaveIo::WritePlayer()
{
    if (netgame::IsMultiplePlayer()) {
        const u32 offset = reinterpret_cast<u8*>(&GetGardenPlus()->sharedData) - reinterpret_cast<u8*>(SvGetSaveMemory()) + 0xC0C;
        const u32 size = 0x1448;
        void* data = SvPrepareSaveWrite(reinterpret_cast<u8*>(&GetGardenPlus()->sharedData) + 0xC0C, size, offset);
        const s32 result = WriteFile(g_SvGardenPlusFileName, offset, data, size, false);
        if (result != RESULT_OK) {
            return result;
        }
    }
    u32 saveNo;
    if (!netgame::IsMyPlayerHost() || (saveNo = SvGetPlayerSaveNo(netgame::PLAYER_NO_MY_NO)) >= PLAYER_NUM) {
        saveNo = netgame::GetSaveNo();
    }
    const u32 size = sizeof(SvPlayer);
    void* data = SvPrepareSaveWrite(g_UnkAAFF18.pPlayer, size, SvGetPlayerOffset(saveNo));
    return WriteFile(g_SvGardenPlusFileName, SvGetPlayerOffset(saveNo), data, size, false);
}

// 0x001B8E7C (name is ours)
void SvSaveIo::Unmount()
{
    if (--s_State.mountCount > 0) {
        return;
    }
    nn::fs::Unmount("data");
    s_State.isMounted = false;
}

// 0x001B8EC4 (name is ours)
void SvSaveIo::SetAccessible()
{
    s_State.isAccessible = true;
}

// 0x001B8ED8 (name is ours)
s32 SvSaveIo::WriteNothing()
{
    return RESULT_OK;
}

// 0x001B8EE0 (name is ours)
s32 SvSaveIo::WriteSharedUnk0()
{
    const u32 offset = SvGetSharedDataOffset();
    const u32 size = 0x24;
    void* data = SvPrepareSaveWrite(&GetGardenPlus()->sharedData, size, offset);
    return WriteFile(g_SvGardenPlusFileName, SvGetSharedDataOffset(), data, size, false);
}

// 0x001B8F34 (name is ours)
s32 SvSaveIo::DeletePlayerFiles()
{
    const u32 saveNo = SvGetPlayerSaveNo(netgame::PLAYER_NO_MY_NO);
    s32 result = Mount(false, false, 0);
    if (result != RESULT_OK) {
        return result;
    }
    for (s32 i = 0; i < PLAYER_FILE_NUM; i++) {
        const s32 deleteResult = DeleteFile(s_PlayerFileNames[i][saveNo]);
        if (deleteResult != RESULT_OK) {
            result = deleteResult;
        }
    }
    EndAccess();
    return result;
}

// 0x001B8FC4 (name is ours)
s32 SvSaveIo::ConvertResult(const nn::Result& result)
{
    if (result.IsSuccess()) {
        return RESULT_OK;
    }
    if (IsFsResult(result, 100, 179)) {
        return RESULT_NOT_FOUND;
    }
    if (IsFsResult(result, 180, 199)) {
        return RESULT_FAILED;
    }
    if (IsFsResult(result, 390, 399)) {
        return RESULT_BROKEN;
    }
    if (IsFsResult(result, 230, 339)) {
        // "unacceptable": fatal for 280-334
        if (IsFsResult(result, 280, 334)) {
            nn::err::CTR::ThrowFatalErrAllIfFailure(result);
        } else {
            return RESULT_FAILED;
        }
    } else if (IsFsResult(result, 200, 219)) {
        return RESULT_NOT_ENOUGH_SPACE;
    }
    nn::err::CTR::ThrowFatalErrAllIfFailure(result);
    return RESULT_FAILED;
}

// 0x001B9130 (name is ours)
s32 SvSaveIo::ReadFile(const char* name, u32 offset, void* buffer, u32 size)
{
    s_State.offset = offset;
    s_State.size = size;
    sead::FileHandle handle;
    sead::FileDevice* device = GetDevice();
    if (device == NULL) {
        s_State.size = 0;
        s_State.offset = 0;
        return RESULT_FAILED;
    }
    device->tryOpen(&handle, sead::SafeStringBase<char>(name), sead::FileDevice::FILE_OPEN_FLAG_READ_WRITE, 0);
    s32 result = ConvertResult(nn::Result(device->getLastRawError()));
    if (result != RESULT_OK) {
        s_State.size = 0;
        s_State.offset = 0;
        return result;
    }
    if (offset != 0 && !handle.seek(offset, sead::FileDevice::SEEK_ORIGIN_BEGIN)) {
        result = ConvertResult(nn::Result(handle.mDevice->getLastRawError()));
        if (result != RESULT_OK) {
            if (!handle.close()) {
                const s32 closeResult = ConvertResult(nn::Result(handle.mDevice->getLastRawError()));
                if (closeResult != RESULT_OK) {
                    result = closeResult;
                }
            }
            s_State.size = 0;
            s_State.offset = 0;
            return result;
        }
    }
    u32 readSize;
    if (!handle.read(&readSize, static_cast<u8*>(buffer), size) || readSize != size) {
        result = ConvertResult(nn::Result(handle.mDevice->getLastRawError()));
        if (result != RESULT_OK) {
            if (!handle.close()) {
                const s32 closeResult = ConvertResult(nn::Result(handle.mDevice->getLastRawError()));
                if (closeResult != RESULT_OK) {
                    result = closeResult;
                }
            }
            s_State.size = 0;
            s_State.offset = 0;
            return result;
        }
    }
    if (!handle.close()) {
        result = ConvertResult(nn::Result(handle.mDevice->getLastRawError()));
        if (result != RESULT_OK) {
            s_State.size = 0;
            s_State.offset = 0;
            return result;
        }
    }
    s_State.size = 0;
    s_State.offset = 0;
    return RESULT_OK;
}

// 0x001B9400 (name is ours)
s32 SvSaveIo::WriteFile(const char* name, u32 offset, const void* data, u32 size, bool isWholeFile)
{
    (void)isWholeFile;
    s32 result;
    s_State.offset = offset;
    s_State.size = size;
    s32 tries = 0;
    for (;;) {
        sead::FileHandle handle;
        sead::FileDevice* device = GetDevice();
        if (device == NULL || ++tries > 10) {
            result = RESULT_FAILED;
            break;
        }
        {
            // the file can be opened for writing
            sead::FileHandle check;
            sead::FileDevice* checkDevice =
                device->tryOpen(&check, sead::SafeStringBase<char>(name), sead::FileDevice::FILE_OPEN_FLAG_WRITE_ONLY, 0);
            if (checkDevice == NULL) {
                result = RESULT_FAILED;
                break;
            }
            const nn::Result error(checkDevice->getLastRawError());
            if (!check.close()) {
                result = RESULT_FAILED;
                break;
            }
            result = ConvertResult(error);
        }
        if (result == RESULT_OK) {
            device->tryOpen(&handle, sead::SafeStringBase<char>(name), sead::FileDevice::FILE_OPEN_FLAG_READ_WRITE, 0);
            result = ConvertResult(nn::Result(device->getLastRawError()));
        }
        if (result == RESULT_OK && offset != 0 && !handle.seek(offset, sead::FileDevice::SEEK_ORIGIN_BEGIN)) {
            result = ConvertResult(nn::Result(handle.mDevice->getLastRawError()));
        }
        if (result == RESULT_OK) {
            u32 writtenSize;
            if (!handle.write(&writtenSize, static_cast<const u8*>(data), size) || writtenSize != size) {
                result = ConvertResult(nn::Result(handle.mDevice->getLastRawError()));
            }
        }
        if (result == RESULT_NOT_ENOUGH_SPACE) {
            // the file is made again
            DeleteFile(name);
            continue;
        }
        if (result == RESULT_OK && !handle.close()) {
            result = ConvertResult(nn::Result(handle.mDevice->getLastRawError()));
        }
        break;
    }
    s_State.offset = 0;
    s_State.size = 0;
    return result;
}

// 0x001B9810 (name is ours)
s32 SvSaveIo::Mount(bool formatIfBroken, bool unused, u32 saveId)
{
    (void)unused;
    s_State.device = DEVICE_SAVE_DATA;
    if (!s_State.isMounted) {
        nn::Result result;
        if (!s_State.isAccessible && saveId == 0) {
            result = nn::fs::MountSaveData("data:");
        } else if (saveId == 0 || saveId == 0x863) {
            result = nn::fs::MountAccessibleSaveData("data", 0x863);
        } else {
            result = nn::fs::MountAccessibleSaveData("data", saveId);
        }
        if (result.IsFailure()) {
            if (IsFsResult(result, 340, 359)) {
                return RESULT_MOUNT_6;
            }
            if (IsFsResult(result, 390, 399) || IsFsResult(result, 360, 389)) {
                if (!formatIfBroken) {
                    return RESULT_BROKEN;
                }
                Format();
                return Mount(false, true, 0);
            }
            if (IsFsResult(result, 220, 229)) {
                return RESULT_ARCHIVE_INVALIDATED;
            }
            if (IsFsResult(result, 100, 179)) {
                return RESULT_ARCHIVE_NOT_FOUND;
            }
            nn::err::CTR::ThrowFatalErrAllIfFailure(result);
            return RESULT_FAILED;
        }
        s_State.isMounted = true;
    }
    s_State.mountCount++;
    return RESULT_OK;
}

// 0x001B99BC (name is ours)
s32 SvSaveIo::Commit(bool updateSecureValue)
{
    if (updateSecureValue) {
        SvIncrementSecureValue();
        const s32 result = WriteFile(g_SvGardenPlusFileName, 0, SvGetSaveMemory(), sizeof(u64), false);
        if (result != RESULT_OK) {
            return result;
        }
    }
    if (s_State.device == DEVICE_SAVE_DATA && s_State.isMounted) {
        nn::Result result;
        if (updateSecureValue) {
            result = nn::fs::SetSaveDataSecureValue("data:", GetGardenPlus()->secureValue.secureValue);
        } else {
            result = nn::fs::CommitSaveData("data:");
        }
        if (result.IsFailure()) {
            return RESULT_FAILED;
        }
    }
    return RESULT_OK;
}

// 0x001B9A6C (name is ours)
void SvSaveIo::Format()
{
    switch (s_State.device) {
    case DEVICE_SAVE_DATA_2:
        s_State.device = DEVICE_SAVE_DATA;
        // fall through
    case DEVICE_SAVE_DATA: {
        if (s_State.isMounted) {
            Unmount();
        }
        nn::Result result = nn::fs::FormatSaveData(16, 1, true);
        if (result.IsFailure()) {
            nn::err::CTR::ThrowFatalErrAllIfFailure(result);
        }
        break;
    }
    case DEVICE_SDMC:
        DeleteAllFiles(true);
        break;
    }
    g_Unknown9510F0 = 0;
    g_Unknown9510F1 = 0;
}

// 0x001B9BB4 (name is ours)
bool SvSaveIo::IsExistFile(const char* name, bool mount)
{
    if (mount && Mount(false, false, 0) != RESULT_OK) {
        return false;
    }
    bool isExist = false;
    sead::FileDevice* device = GetDevice();
    if (device != NULL) {
        bool exist;
        if (device->tryIsExistFile(&exist, sead::SafeStringBase<char>(name))) {
            isExist = exist;
        }
    }
    if (mount) {
        EndAccess();
    }
    return isExist;
}

// 0x001B9C5C (name is ours)
s32 SvSaveIo::ReadGardenPlus()
{
    return ReadFile(g_SvGardenPlusFileName, 0, SvGetSaveMemory(), g_SvGardenPlusSize);
}

// 0x001B9C8C (name is ours)
s32 SvSaveIo::WriteGardenPlus()
{
    void* data = SvPrepareSaveWrite(GetGardenPlus(), g_SvGardenPlusSize, 0);
    return WriteFile(g_SvGardenPlusFileName, 0, data, g_SvGardenPlusSize, true);
}

// 0x001B9CD0 (name is ours)
s32 SvSaveIo::WriteNpcData()
{
    const u32 offset = SvGetNpcDataOffset();
    const u32 size = 0x22BCC;
    void* data = SvPrepareSaveWrite(&GetGardenPlus()->data.npcData, size, offset);
    return WriteFile(g_SvGardenPlusFileName, SvGetNpcDataOffset(), data, size, false);
}

// 0x001B9D58 (name is ours)
void SvSaveIo::EndAccessIfMounted()
{
    switch (s_State.device) {
    case DEVICE_SAVE_DATA_2:
        break;
    case DEVICE_SDMC:
        Func56B888();
        break;
    case DEVICE_SAVE_DATA:
        if (s_State.isMounted) {
            Unmount();
        }
        break;
    }
}

// 0x001B9D98 (name is ours)
s32 SvSaveIo::DeleteAllFiles(bool mount)
{
    s32 result = RESULT_OK;
    if (mount) {
        result = Mount(false, false, 0);
        if (result != RESULT_OK) {
            if (result == RESULT_BROKEN) {
                Format();
                return RESULT_OK;
            }
            return result;
        }
    }
    for (s32 i = 0; i < FILE_NUM; i++) {
        const s32 deleteResult = DeleteFile(s_FileNames[i]);
        if (deleteResult != RESULT_OK) {
            result = deleteResult;
        }
    }
    for (s32 player = 0; player < PLAYER_NUM; player++) {
        for (s32 i = 0; i < PLAYER_FILE_NUM; i++) {
            const s32 deleteResult = DeleteFile(s_PlayerFileNames[i][player]);
            if (deleteResult != RESULT_OK) {
                result = deleteResult;
            }
        }
    }
    if (mount) {
        EndAccess();
    }
    return result;
}

// 0x001B9E7C (name is ours)
u32 SvSaveIo::GetSize()
{
    return s_State.size;
}

// 0x001B9E8C (name is ours)
u32 SvSaveIo::GetOffset()
{
    return s_State.offset;
}
