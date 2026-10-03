#include "nn/fs/ipc/ipc_FileSystem.h"
#include "nn/fs/ipc/ipc_Common.h"

namespace nn {
namespace fs {
namespace ipc {
namespace {

// command headers (id << 16 | normal parameters << 6 | translate parameters), 3dbrew "FS:*"
const bit32 COMMAND_OPEN_FILE = 0x080201C2;
const bit32 COMMAND_OPEN_FILE_DIRECTLY = 0x08030204;
const bit32 COMMAND_DELETE_FILE = 0x08040142;
const bit32 COMMAND_RENAME_FILE = 0x08050244;
const bit32 COMMAND_DELETE_DIRECTORY = 0x08060142;
const bit32 COMMAND_DELETE_DIRECTORY_RECURSIVELY = 0x08070142;
const bit32 COMMAND_CREATE_FILE = 0x08080202;
const bit32 COMMAND_CREATE_DIRECTORY = 0x08090182;
const bit32 COMMAND_RENAME_DIRECTORY = 0x080A0244;
const bit32 COMMAND_OPEN_DIRECTORY = 0x080B0102;
const bit32 COMMAND_OPEN_ARCHIVE = 0x080C00C2;
const bit32 COMMAND_CONTROL_ARCHIVE = 0x080D0144;
const bit32 COMMAND_CLOSE_ARCHIVE = 0x080E0080;
const bit32 COMMAND_GET_FREE_BYTES = 0x08120080;
const bit32 COMMAND_IS_SDMC_DETECTED = 0x08170000;
const bit32 COMMAND_IS_SDMC_WRITABLE = 0x08180000;
const bit32 COMMAND_GET_ARCHIVE_RESOURCE = 0x08490040;
const bit32 COMMAND_FORMAT_SAVE_DATA = 0x084C0242;
const bit32 COMMAND_CREATE_EXT_SAVE_DATA = 0x08510242;
const bit32 COMMAND_DELETE_EXT_SAVE_DATA = 0x08520100;
const bit32 COMMAND_GET_EXT_DATA_BLOCK_SIZE = 0x08540100;
const bit32 COMMAND_SET_ARCHIVE_PRIORITY = 0x085A00C0;
const bit32 COMMAND_GET_ARCHIVE_PRIORITY = 0x085B0080;
const bit32 COMMAND_INITIALIZE_WITH_SDK_VERSION = 0x08610042;
const bit32 COMMAND_SET_PRIORITY = 0x08620040;
const bit32 COMMAND_GET_PRIORITY = 0x08630000;
const bit32 COMMAND_GET_THIS_SAVE_DATA_SECURE_VALUE = 0x086F0040;
const bit32 COMMAND_SET_SAVE_ARCHIVE_SECURE_VALUE = 0x08750180;

} // namespace

// 0x0013044C | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::FileSystem::OpenArchive(u64* archive, u32 archiveId, u32 pathType, const u8* path, size_t pathSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_OPEN_ARCHIVE;
    command[1] = archiveId;
    command[2] = pathType;
    command[3] = pathSize;
    command[4] = StaticBufferDescriptor(pathSize, 0);
    command[5] = reinterpret_cast<uptr>(path);
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *archive = *reinterpret_cast<u64*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x001304AC | nintendogs:callgraph [tier A]
nn::Result nn::fs::ipc::FileSystem::CloseArchive(u64 archive)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_CLOSE_ARCHIVE;
    *reinterpret_cast<u64*>(&command[1]) = archive;
    return Send(mSession, command);
}

// 0x001304DC | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::FileSystem::OpenFileDirectly(nn::Handle* file, nn::fs::Transaction transaction, u32 archiveId,
                                                    u32 archivePathType, const u8* archivePath, size_t archivePathSize,
                                                    u32 pathType, const u8* path, size_t pathSize, u32 openFlags,
                                                    nn::fs::Attributes attributes)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_OPEN_FILE_DIRECTLY;
    command[1] = transaction;
    command[2] = archiveId;
    command[3] = archivePathType;
    command[4] = archivePathSize;
    command[5] = pathType;
    command[6] = pathSize;
    command[7] = openFlags;
    *reinterpret_cast<nn::fs::Attributes*>(&command[8]) = attributes;
    command[9] = StaticBufferDescriptor(archivePathSize, 2);
    command[10] = reinterpret_cast<uptr>(archivePath);
    command[11] = StaticBufferDescriptor(pathSize, 0);
    command[12] = reinterpret_cast<uptr>(path);
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *file = nn::Handle(command[3]);
    return nn::Result(command[1]);
}

// 0x00136214 | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::FileSystem::SetPriority(s32 priority)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_PRIORITY;
    command[1] = priority;
    return Send(mSession, command);
}

// 0x0013AEE0 | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::FileSystem::GetPriority(s32* priority)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_PRIORITY;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *priority = command[2];
    return nn::Result(command[1]);
}

// 0x0013AF18 | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::FileSystem::InitializeWithSdkVersion(u32 version)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_INITIALIZE_WITH_SDK_VERSION;
    command[1] = version;
    command[2] = IPC_PROCESS_ID;
    return Send(mSession, command);
}

// 0x00348C30 | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::FileSystem::CreateFile(nn::fs::Transaction transaction, u64 archive, u32 pathType, const u8* path,
                                              size_t pathSize, nn::fs::Attributes attributes, s64 size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_CREATE_FILE;
    command[1] = transaction;
    *reinterpret_cast<u64*>(&command[2]) = archive;
    command[4] = pathType;
    command[5] = pathSize;
    *reinterpret_cast<nn::fs::Attributes*>(&command[6]) = attributes;
    *reinterpret_cast<s64*>(&command[7]) = size;
    command[9] = StaticBufferDescriptor(pathSize, 0);
    command[10] = reinterpret_cast<uptr>(path);
    return Send(mSession, command);
}

// 0x00348C8C | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::FileSystem::DeleteFile(nn::fs::Transaction transaction, u64 archive, u32 pathType, const u8* path,
                                              size_t pathSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_DELETE_FILE;
    command[1] = transaction;
    *reinterpret_cast<u64*>(&command[2]) = archive;
    command[4] = pathType;
    command[5] = pathSize;
    command[6] = StaticBufferDescriptor(pathSize, 0);
    command[7] = reinterpret_cast<uptr>(path);
    return Send(mSession, command);
}

// 0x00348CDC | fefates:bytes [tier B]
nn::Result nn::fs::ipc::FileSystem::RenameFile(nn::fs::Transaction transaction, u64 archive, u32 pathType, const u8* path,
                                              size_t pathSize, u64 newArchive, u32 newPathType, const u8* newPath,
                                              size_t newPathSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_RENAME_FILE;
    command[1] = transaction;
    *reinterpret_cast<u64*>(&command[2]) = archive;
    command[4] = pathType;
    command[5] = pathSize;
    *reinterpret_cast<u64*>(&command[6]) = newArchive;
    command[8] = newPathType;
    command[9] = newPathSize;
    command[10] = StaticBufferDescriptor(pathSize, 1);
    command[11] = reinterpret_cast<uptr>(path);
    command[12] = StaticBufferDescriptor(newPathSize, 2);
    command[13] = reinterpret_cast<uptr>(newPath);
    return Send(mSession, command);
}

// 0x00348D6C (name after 3dbrew)
nn::Result nn::fs::ipc::FileSystem::GetFreeBytes(s64* freeBytes, u64 archive)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_FREE_BYTES;
    *reinterpret_cast<u64*>(&command[1]) = archive;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *freeBytes = *reinterpret_cast<s64*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x00348DA8 | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::FileSystem::OpenDirectory(nn::Handle* directory, u64 archive, u32 pathType, const u8* path,
                                                 size_t pathSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_OPEN_DIRECTORY;
    *reinterpret_cast<u64*>(&command[1]) = archive;
    command[3] = pathType;
    command[4] = pathSize;
    command[5] = StaticBufferDescriptor(pathSize, 0);
    command[6] = reinterpret_cast<uptr>(path);
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *directory = nn::Handle(command[3]);
    return nn::Result(command[1]);
}

// 0x00348E0C | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::FileSystem::ControlArchive(u64 archive, u32 action, const void* input, size_t inputSize,
                                                  void* output, size_t outputSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_CONTROL_ARCHIVE;
    *reinterpret_cast<u64*>(&command[1]) = archive;
    command[3] = action;
    command[4] = inputSize;
    command[5] = outputSize;
    command[6] = ReadBufferDescriptor(inputSize);
    command[7] = reinterpret_cast<uptr>(input);
    command[8] = WriteBufferDescriptor(outputSize);
    command[9] = reinterpret_cast<uptr>(output);
    return Send(mSession, command);
}

// 0x00348E78 | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::FileSystem::FormatSaveData(u32 archiveId, u32 pathType, const u8* path, size_t pathSize, u32 blocks,
                                                  u32 directories, u32 files, u32 directoryBuckets, u32 fileBuckets,
                                                  bool duplicateData)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_FORMAT_SAVE_DATA;
    command[1] = archiveId;
    command[2] = pathType;
    command[3] = pathSize;
    command[4] = blocks;
    command[5] = directories;
    command[6] = files;
    command[7] = directoryBuckets;
    command[8] = fileBuckets;
    *reinterpret_cast<bool*>(&command[9]) = duplicateData;
    command[10] = StaticBufferDescriptor(pathSize, 0);
    command[11] = reinterpret_cast<uptr>(path);
    return Send(mSession, command);
}

// 0x00348ED0 | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::FileSystem::IsSdmcDetected(bool* detected)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_IS_SDMC_DETECTED;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *detected = *reinterpret_cast<bool*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x00348F08 | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::FileSystem::IsSdmcWritable(bool* writable)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_IS_SDMC_WRITABLE;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *writable = *reinterpret_cast<bool*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x00348F40 | nintendogs:bytes [tier B]
nn::Result nn::fs::ipc::FileSystem::CreateDirectory(nn::fs::Transaction transaction, u64 archive, u32 pathType,
                                                   const u8* path, size_t pathSize, nn::fs::Attributes attributes)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_CREATE_DIRECTORY;
    command[1] = transaction;
    *reinterpret_cast<u64*>(&command[2]) = archive;
    command[4] = pathType;
    command[5] = pathSize;
    *reinterpret_cast<nn::fs::Attributes*>(&command[6]) = attributes;
    command[7] = StaticBufferDescriptor(pathSize, 0);
    command[8] = reinterpret_cast<uptr>(path);
    return Send(mSession, command);
}

// 0x00348F94 | nintendogs:bytes [tier B]
nn::Result nn::fs::ipc::FileSystem::DeleteDirectory(nn::fs::Transaction transaction, u64 archive, u32 pathType,
                                                   const u8* path, size_t pathSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_DELETE_DIRECTORY;
    command[1] = transaction;
    *reinterpret_cast<u64*>(&command[2]) = archive;
    command[4] = pathType;
    command[5] = pathSize;
    command[6] = StaticBufferDescriptor(pathSize, 0);
    command[7] = reinterpret_cast<uptr>(path);
    return Send(mSession, command);
}

// 0x00348FE4 | manual:3dbrew [tier B]
nn::Result nn::fs::ipc::FileSystem::RenameDirectory(nn::fs::Transaction transaction, u64 archive, u32 pathType,
                                                   const u8* path, size_t pathSize, u64 newArchive, u32 newPathType,
                                                   const u8* newPath, size_t newPathSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_RENAME_DIRECTORY;
    command[1] = transaction;
    *reinterpret_cast<u64*>(&command[2]) = archive;
    command[4] = pathType;
    command[5] = pathSize;
    *reinterpret_cast<u64*>(&command[6]) = newArchive;
    command[8] = newPathType;
    command[9] = newPathSize;
    command[10] = StaticBufferDescriptor(pathSize, 1);
    command[11] = reinterpret_cast<uptr>(path);
    command[12] = StaticBufferDescriptor(newPathSize, 2);
    command[13] = reinterpret_cast<uptr>(newPath);
    return Send(mSession, command);
}

// 0x00349074 | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::FileSystem::CreateExtSaveData(const nn::fs::ExtSaveDataSpecifier& specifier, u32 directories,
                                                     u32 files, s64 sizeLimit, const void* smdh, size_t smdhSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_CREATE_EXT_SAVE_DATA;
    // the specifier fills words 1 to 3 of the 4 that 3dbrew lists; word 4 stays as it is
    *reinterpret_cast<nn::fs::ExtSaveDataSpecifier*>(&command[1]) = specifier;
    command[5] = directories;
    command[6] = files;
    *reinterpret_cast<s64*>(&command[7]) = sizeLimit;
    command[9] = smdhSize;
    command[10] = ReadBufferDescriptor(smdhSize);
    command[11] = reinterpret_cast<uptr>(smdh);
    return Send(mSession, command);
}

// 0x003490D0 | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::FileSystem::DeleteExtSaveData(const nn::fs::ExtSaveDataSpecifier& specifier)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_DELETE_EXT_SAVE_DATA;
    *reinterpret_cast<nn::fs::ExtSaveDataSpecifier*>(&command[1]) = specifier;
    return Send(mSession, command);
}

// 0x0034910C (name after 3dbrew)
nn::Result nn::fs::ipc::FileSystem::GetArchivePriority(s32* priority, u64 archive)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_ARCHIVE_PRIORITY;
    *reinterpret_cast<u64*>(&command[1]) = archive;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *priority = command[2];
    return nn::Result(command[1]);
}

// 0x00349148 | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::FileSystem::GetArchiveResource(nn::fs::ArchiveResource* resource, nn::fs::SystemMediaType mediaType)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_ARCHIVE_RESOURCE;
    *reinterpret_cast<nn::fs::SystemMediaType*>(&command[1]) = mediaType;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *resource = *reinterpret_cast<nn::fs::ArchiveResource*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x00349198 | manual:3dbrew [tier B]
nn::Result nn::fs::ipc::FileSystem::SetArchivePriority(u64 archive, s32 priority)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_ARCHIVE_PRIORITY;
    *reinterpret_cast<u64*>(&command[1]) = archive;
    command[3] = priority;
    return Send(mSession, command);
}

// 0x003491D0 | nintendogs:bytes [tier B]
nn::Result nn::fs::ipc::FileSystem::GetExtDataBlockSize(s64* totalBlocks, s64* freeBlocks, s32* blockSize,
                                                       const nn::fs::ExtSaveDataSpecifier& specifier)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_EXT_DATA_BLOCK_SIZE;
    *reinterpret_cast<nn::fs::ExtSaveDataSpecifier*>(&command[1]) = specifier;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *totalBlocks = *reinterpret_cast<s64*>(&command[2]);
    *freeBlocks = *reinterpret_cast<s64*>(&command[4]);
    *blockSize = command[6];
    return nn::Result(command[1]);
}

// 0x0034923C | manual:3dbrew [tier B]
nn::Result nn::fs::ipc::FileSystem::SetSaveArchiveSecureValue(u64 archive, u32 slot, u64 value, bool unknown)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_SAVE_ARCHIVE_SECURE_VALUE;
    *reinterpret_cast<u64*>(&command[1]) = archive;
    command[3] = slot;
    *reinterpret_cast<u64*>(&command[4]) = value;
    *reinterpret_cast<bool*>(&command[6]) = unknown;
    return Send(mSession, command);
}

// 0x0034928C | manual:3dbrew [tier B]
nn::Result nn::fs::ipc::FileSystem::DeleteDirectoryRecursively(nn::fs::Transaction transaction, u64 archive, u32 pathType,
                                                              const u8* path, size_t pathSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_DELETE_DIRECTORY_RECURSIVELY;
    command[1] = transaction;
    *reinterpret_cast<u64*>(&command[2]) = archive;
    command[4] = pathType;
    command[5] = pathSize;
    command[6] = StaticBufferDescriptor(pathSize, 0);
    command[7] = reinterpret_cast<uptr>(path);
    return Send(mSession, command);
}

// 0x003492DC (name after 3dbrew)
nn::Result nn::fs::ipc::FileSystem::GetThisSaveDataSecureValue(bool* exists, bool* isGameCard, u64* value, u32 slot)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_THIS_SAVE_DATA_SECURE_VALUE;
    command[1] = slot;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *exists = *reinterpret_cast<bool*>(&command[2]);
    *isGameCard = *reinterpret_cast<bool*>(&command[3]);
    *value = *reinterpret_cast<u64*>(&command[4]);
    return nn::Result(command[1]);
}

// 0x00349334 | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::FileSystem::OpenFile(nn::Handle* file, nn::fs::Transaction transaction, u64 archive, u32 pathType,
                                            const u8* path, size_t pathSize, u32 openFlags, nn::fs::Attributes attributes)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_OPEN_FILE;
    command[1] = transaction;
    *reinterpret_cast<u64*>(&command[2]) = archive;
    command[4] = pathType;
    command[5] = pathSize;
    command[6] = openFlags;
    *reinterpret_cast<nn::fs::Attributes*>(&command[7]) = attributes;
    command[8] = StaticBufferDescriptor(pathSize, 0);
    command[9] = reinterpret_cast<uptr>(path);
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *file = nn::Handle(command[3]);
    return nn::Result(command[1]);
}

} // namespace ipc
} // namespace fs
} // namespace nn
