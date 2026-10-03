#include "nn/fs/CTR/MPCore/detail/detail_Api.h"
#include "nn/fs/CTR/MPCore/detail/fs_ContentRomFsArchive.h"
#include "nn/fs/CTR/MPCore/detail/fs_UserFileSystem.h"
#include "nn/fs/ipc/ipc_FileSystem.h"

#include <new>

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
namespace {

// the contents of titles (3dbrew "Filesystem services")
const u32 ARCHIVE_ID_NCCH = 0x2345678A;

// ArchivePath type (3dbrew "FS Path")
const u32 PATH_TYPE_BINARY = 2;

// the binary archive path and file path of ARCHIVE_ID_NCCH (names are ours)
struct NcchArchivePath
{
    u64 programId;
    nn::fs::MediaType mediaType;
    u32 reserved;   // not set
};

struct NcchFilePath
{
    u32 type;           // 0: the RomFS
    u32 contentIndex;
    u32 reserved[3];
};

} // namespace

// 0x00347CD8 | fefates:bytes [tier B]
nn::Result OpenDataContent(nn::fs::CTR::MPCore::detail::IArchive** archive, const nn::fs::CTR::DataContentArchivePath& path, u32 maxFiles, u32 maxDirectories, void* buffer, size_t bufferSize, bool useCache)
{
    IArchive* opened = 0;
    nn::Result result;
    ContentRomFsArchive* created = new (ContentRomFsArchive::AllocateBuffer()) ContentRomFsArchive();
    if (created == 0) {
        result = nn::Result(RESULT_OUT_OF_OBJECTS);
    } else {
        NcchArchivePath archivePath;
        archivePath.programId = path.programId;
        archivePath.mediaType = static_cast<nn::fs::MediaType>(path.mediaType);
        NcchFilePath filePath = {0, path.contentIndex, {0, 0, 0}};
        nn::fs::Attributes attributes = {};
        nn::Handle handle;
        result = nn::fs::ipc::FileSystem(s_Session).OpenFileDirectly(
            &handle, TRANSACTION_NONE, ARCHIVE_ID_NCCH, PATH_TYPE_BINARY, reinterpret_cast<const u8*>(&archivePath),
            sizeof(archivePath), PATH_TYPE_BINARY, reinterpret_cast<const u8*>(&filePath), sizeof(filePath),
            OPEN_MODE_READ, attributes);
        if (result.IsSuccess()) {
            IFile* file;
            result = OpenFileServerFile(&file, handle);
            if (result.IsSuccess()) {
                result = created->Initialize(file, maxFiles, maxDirectories, buffer, bufferSize, useCache);
                if (result.IsFailure()) {
                    file->Close();
                }
            }
        }
        if (result.IsSuccess()) {
            opened = created;
        } else {
            created->DeleteObject();
        }
    }
    if (result.IsFailure()) {
        return result;
    }
    *archive = opened;
    return nn::Result();
}

} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn
