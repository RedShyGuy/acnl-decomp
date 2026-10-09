#include "nn/fs/CTR/CTR_Api.h"
#include "nn/CTR/CTR_SystemMenuData.h"
#include "nn/fs/CTR/MPCore/detail/fs_FileServerArchive.h"
#include "nn/fs/CTR/MPCore/detail/fs_IFile.h"
#include "nn/fs/CTR/MPCore/detail/fs_UserFileSystem.h"
#include "nn/fs/fs_Types.h"
#include "nn/fs/ipc/ipc_FileSystem.h"

namespace nn {
namespace fs {
namespace CTR {
namespace {
// archive SelfNCCH and its paths (3dbrew "Filesystem services", "FS Path")
const u32 ARCHIVE_ID_SELF_NCCH = 0x00000003;
const u32 PATH_TYPE_EMPTY = 1;
const u32 PATH_TYPE_BINARY = 2;

// a file of SelfNCCH: the ExeFS file of the name (3dbrew: file path type 2)
struct SelfNcchFilePath
{
    u32 type;     // 0x0
    char name[8]; // 0x4, ExeFS file name
};
const u32 SELF_NCCH_FILE_TYPE_EXEFS = 2;
} // namespace

// 0x00346A68 | nintendogs:bytes-fuzzy [tier A]
nn::Result GetSelfSystemMenuData(nn::CTR::SystemMenuData* data)
{
    using namespace MPCore::detail;
    FileServerArchive archive(nn::Handle(), 0);

    // an empty path (the service does not look at its byte)
    u8 empty;
    u64 handle;
    nn::Result result = nn::fs::ipc::FileSystem(s_Session).OpenArchive(&handle, ARCHIVE_ID_SELF_NCCH, PATH_TYPE_EMPTY, &empty, sizeof(empty));
    if (result.IsFailure()) {
        return result;
    }
    archive = FileServerArchive(s_Session, handle);

    SelfNcchFilePath filePath = {SELF_NCCH_FILE_TYPE_EXEFS, {'i', 'c', 'o', 'n', 0, 0, 0, 0}};
    ArchivePath path;
    path.type = PATH_TYPE_BINARY;
    path.data = reinterpret_cast<const u8*>(&filePath);
    path.size = sizeof(filePath);
    IFile* file;
    result = archive.OpenFile(&file, path, OPEN_MODE_READ);
    if (result.IsFailure()) {
        return result;
    }
    s32 readSize;
    result = file->TryRead(&readSize, 0, data, sizeof(nn::CTR::SystemMenuData));
    file->Close();
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result();
}

} // namespace CTR
} // namespace fs
} // namespace nn
