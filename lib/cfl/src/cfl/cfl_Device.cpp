#include "cfl/cfl_Database.h"
#include "cfl/cfl_Fs.h"
#include "nn/fs/fs_FileInputStream.h"
#include "nn/fs/fs_FileOutputStream.h"

#include <cstring>

using namespace cfl::detail;

namespace {
// sets status and value (inline; name is ours)
inline s32 SetResult(CFLiDevice* device, s32 status, s32 value)
{
    device->status = status;
    device->value = value;
    return status;
}

// the result of a failed nn::fs call (inline; name is ours)
inline void SetFsError(CFLiDevice* device)
{
    device->status = CFL_RESULT_FILE_ERROR;
    device->value = CFL_REASON_FS_ERROR;
}

// ends the sequence: the result goes to the manager and the callback (inline; name is ours)
inline void FinishSequence(CFLiDevice* device)
{
    s32 status = device->status;
    if (status == CFL_RESULT_BUSY) {
        return;
    }
    s32 value = device->value;
    CFLi_EndWorkingReason(status, value);
    if (device->callback != NULL) {
        device->callback(device, device->value);
    }
}

// marks an asynchronous operation (inline; name is ours)
inline void BeginWorking()
{
    if (CFL_IsAvailable()) {
        CFLi_GetManager()->working = true;
        CFLi_GetManager()->status = CFL_RESULT_BUSY;
    }
}
} // namespace

// the memory device

// 0x0017C500 (name is ours)
CFLiDevice* CFLi_MemoryDeviceCreate(void** buffer, u32* bufferSize)
{
    CFLiMemoryDevice* device =
        static_cast<CFLiMemoryDevice*>(CFLi_AllocateAlignedBuffer(buffer, bufferSize, sizeof(CFLiMemoryDevice)));
    device->device.opened = false;
    device->position = 0;
    device->size = CFL_DATABASE_FILE_SIZE;
    return &device->device;
}

// 0x00179884 (name is ours)
void CFLi_MemoryDeviceDestroy(CFLiDevice*)
{
    // empty (in the original too)
}

// 0x00175200 (name is ours)
s32 CFLi_MemoryDeviceCreateFile(CFLiDevice* device, u32)
{
    return SetResult(device, CFL_RESULT_SUCCESS, 0);
}

// 0x0017533C (name is ours)
s32 CFLi_MemoryDeviceDeleteFile(CFLiDevice* device)
{
    return SetResult(device, CFL_RESULT_SUCCESS, 0);
}

// 0x00171818 (name is ours)
s32 CFLi_MemoryDeviceOpen(CFLiDevice* device)
{
    device->opened = true;
    return SetResult(device, CFL_RESULT_SUCCESS, 0);
}

// 0x00172938 (name is ours)
s32 CFLi_MemoryDeviceClose(CFLiDevice* device)
{
    device->opened = false;
    return SetResult(device, CFL_RESULT_SUCCESS, 0);
}

// 0x0017A234 (name is ours)
s32 CFLi_MemoryDeviceGetSize(CFLiDevice* device, u32* size)
{
    *size = reinterpret_cast<CFLiMemoryDevice*>(device)->size;
    return SetResult(device, CFL_RESULT_SUCCESS, 0);
}

// 0x00171834 (name is ours)
s32 CFLi_MemoryDeviceRead(CFLiDevice* device, void* buffer, u32 size)
{
    CFLiMemoryDevice* memory = reinterpret_cast<CFLiMemoryDevice*>(device);
    u32 position = memory->position;
    u32 length = position + size <= memory->size ? size : memory->size - position;
    if (length != 0) {
        std::memcpy(buffer, s_pMemoryDatabase + position, length);
    }
    device->status = CFL_RESULT_SUCCESS;
    device->value = length;
    return CFL_RESULT_SUCCESS;
}

// 0x001742D4 (name is ours)
s32 CFLi_MemoryDeviceWrite(CFLiDevice* device, const void* buffer, u32 size, bool)
{
    CFLiMemoryDevice* memory = reinterpret_cast<CFLiMemoryDevice*>(device);
    u32 position = memory->position;
    u32 length = position + size <= memory->size ? size : memory->size - position;
    if (length != 0) {
        std::memcpy(s_pMemoryDatabase + position, buffer, length);
    }
    device->status = CFL_RESULT_SUCCESS;
    device->value = length;
    return CFL_RESULT_SUCCESS;
}

// 0x00171ADC (name is ours)
s32 CFLi_MemoryDeviceSeek(CFLiDevice* device, s32 position)
{
    reinterpret_cast<CFLiMemoryDevice*>(device)->position = position;
    device->status = CFL_RESULT_SUCCESS;
    device->value = position;
    return CFL_RESULT_SUCCESS;
}

// 0x00180688 (name is ours)
s32 CFLi_MemoryDeviceCreateDirectory(CFLiDevice* device, const char*)
{
    return SetResult(device, CFL_RESULT_SUCCESS, 0);
}

// 0x008A3C40
const CFLiDeviceInterface s_MemoryDeviceInterface = {
    CFLi_MemoryDeviceCreate,    CFLi_MemoryDeviceDestroy, CFLi_MemoryDeviceCreateFile,
    CFLi_MemoryDeviceDeleteFile, CFLi_MemoryDeviceOpen,    CFLi_MemoryDeviceClose,
    CFLi_MemoryDeviceGetSize,   CFLi_MemoryDeviceRead,    CFLi_MemoryDeviceWrite,
    CFLi_MemoryDeviceSeek,      CFLi_MemoryDeviceCreateDirectory,
};

// the file device

// 0x0017C0E8 (name is ours)
CFLiDevice* CFLi_FileDeviceCreate(void** buffer, u32* bufferSize)
{
    CFLiDevice* device =
        static_cast<CFLiDevice*>(CFLi_AllocateAlignedBuffer(buffer, bufferSize, sizeof(CFLiFileDevice)));
    device->opened = false;
    return device;
}

// 0x00179480 (name is ours)
void CFLi_FileDeviceDestroy(CFLiDevice*)
{
    // empty (in the original too)
}

// creates the directories of a path one after the other (name is ours)
// 0x00175214 (name is ours)
DECOMP_NOINLINE void CFLi_CreateDirectories(const char* path)
{
    char buffer[256];
    strlcpy(buffer, path, sizeof(buffer));
    bool first = true;
    char* p = buffer;
    for (;;) {
        int i = CFLi_SearchCharacter8(p, '/');
        if (p[i] == 0) {
            return;
        }
        if (!first && p[0] != '.') {
            p[i] = 0;
            nn::Result result = TryCreateDirectory(buffer);
            p[i] = '/';
            if (result.IsFailure() && !IsAlreadyExists(result)) {
                return;
            }
        }
        first = false;
        p += i + (p[i] == '/' ? 1 : 0);
    }
}

// 0x00174DBC (name is ours)
s32 CFLi_FileDeviceCreateFile(CFLiDevice* device, u32 size)
{
    CFLi_CreateDirectories(device->path);
    nn::Result result = TryCreateFile(device->path, size);
    device->result = result;
    if (result.IsSuccess()) {
        device->status = CFL_RESULT_SUCCESS;
        device->value = 0;
    } else if (IsAlreadyExists(result)) {
        device->value = CFL_REASON_ALREADY_EXISTS;
        device->status = CFL_RESULT_SUCCESS;
    } else {
        SetFsError(device);
    }
    return device->status;
}

// 0x00174EBC (name is ours)
s32 CFLi_FileDeviceDeleteFile(CFLiDevice* device)
{
    nn::Result result = TryDeleteFile(device->path);
    device->result = result;
    if (result.IsSuccess()) {
        device->status = CFL_RESULT_SUCCESS;
        device->value = 0;
    } else if (IsNotFound(result)) {
        device->status = CFL_RESULT_SUCCESS;
        device->value = CFL_REASON_NOT_FOUND;
    } else if (IsCorrupted(result)) {
        device->value = CFL_REASON_CORRUPTED;
        device->status = CFL_RESULT_FILE_ERROR;
    } else {
        device->status = CFL_RESULT_FILE_ERROR;
        device->value = CFL_REASON_FS_ERROR;
    }
    return device->status;
}

// 0x001708F4 (name is ours)
s32 CFLi_FileDeviceOpen(CFLiDevice* device)
{
    nn::fs::detail::FileBase& file = reinterpret_cast<CFLiFileDevice*>(device)->file;
    u32 mode = device->create ? nn::fs::OPEN_MODE_READ | nn::fs::OPEN_MODE_WRITE : nn::fs::OPEN_MODE_READ;
    nn::Result result = file.TryInitialize(device->path, mode);
    device->result = result;
    device->opened = false;
    if (result.IsSuccess()) {
        device->opened = true;
        device->status = CFL_RESULT_SUCCESS;
        device->value = 0;
    } else if (IsNotFound(result)) {
        device->status = CFL_RESULT_FILE_ERROR;
        device->value = CFL_REASON_NOT_FOUND;
    } else if (IsCorrupted(result)) {
        device->value = CFL_REASON_CORRUPTED;
        device->status = CFL_RESULT_FILE_ERROR;
    } else {
        device->status = CFL_RESULT_FILE_ERROR;
        device->value = CFL_REASON_NOT_FOUND;
    }
    return device->status;
}

// 0x00172614 (name is ours)
s32 CFLi_FileDeviceClose(CFLiDevice* device)
{
    if (device->opened) {
        reinterpret_cast<CFLiFileDevice*>(device)->file.Finalize();
        device->opened = false;
    }
    device->status = CFL_RESULT_SUCCESS;
    device->value = 0;
    return CFL_RESULT_SUCCESS;
}

// 0x00179E38 (name is ours)
s32 CFLi_FileDeviceGetSize(CFLiDevice* device, u32* size)
{
    s64 fileSize;
    nn::Result result = reinterpret_cast<CFLiFileDevice*>(device)->file.TryGetSize(&fileSize);
    device->result = result;
    if (result.IsSuccess()) {
        device->status = CFL_RESULT_SUCCESS;
        device->value = 0;
        *size = fileSize;
    } else {
        SetFsError(device);
    }
    return device->status;
}

// 0x00170A30 (name is ours)
s32 CFLi_FileDeviceRead(CFLiDevice* device, void* buffer, u32 size)
{
    s32 readSize;
    nn::Result result = reinterpret_cast<CFLiFileDevice*>(device)->file.TryRead(&readSize, buffer, size);
    device->result = result;
    if (result.IsSuccess()) {
        device->status = CFL_RESULT_SUCCESS;
        device->value = readSize;
    } else {
        SetFsError(device);
    }
    return device->status;
}

// 0x001726F0 (name is ours)
s32 CFLi_FileDeviceWrite(CFLiDevice* device, const void* buffer, u32 size, bool flush)
{
    s32 writtenSize;
    nn::Result result =
        reinterpret_cast<CFLiFileDevice*>(device)->file.TryWrite(&writtenSize, buffer, size, flush ? true : false);
    device->result = result;
    if (result.IsSuccess()) {
        device->status = CFL_RESULT_SUCCESS;
        device->value = writtenSize;
    } else {
        SetFsError(device);
    }
    return device->status;
}

// 0x00170A7C (name is ours)
s32 CFLi_FileDeviceSeek(CFLiDevice* device, s32 position)
{
    nn::Result result = reinterpret_cast<CFLiFileDevice*>(device)->file.TrySetPosition(position);
    device->result = result;
    if (result.IsFailure()) {
        SetFsError(device);
        return CFL_RESULT_FILE_ERROR;
    }
    device->status = CFL_RESULT_SUCCESS;
    device->value = position;
    return CFL_RESULT_SUCCESS;
}

// creates the directory of a path ending in '/' (name is ours)
// 0x00180400 (name is ours)
s32 CFLi_FileDeviceCreateDirectory(CFLiDevice* device, const char* path)
{
    int length = CFLi_GetStringLength8(path);
    int slash = CFLi_SearchCharacter8(path, '/');
    if (slash != length && slash != length - 1) {
        CFLi_CreateDirectories(path);
        nn::Result result = TryCreateDirectory(device->path);
        device->result = result;
        if (result.IsFailure()) {
            if (IsAlreadyExists(result)) {
                device->value = CFL_REASON_ALREADY_EXISTS;
                device->status = CFL_RESULT_SUCCESS;
            } else {
                SetFsError(device);
            }
            return device->status;
        }
    }
    device->status = CFL_RESULT_SUCCESS;
    device->value = 0;
    return device->status;
}

// 0x008BAE28
const CFLiDeviceInterface s_FileDeviceInterface = {
    CFLi_FileDeviceCreate,    CFLi_FileDeviceDestroy, CFLi_FileDeviceCreateFile, CFLi_FileDeviceDeleteFile,
    CFLi_FileDeviceOpen,      CFLi_FileDeviceClose,   CFLi_FileDeviceGetSize,    CFLi_FileDeviceRead,
    CFLi_FileDeviceWrite,     CFLi_FileDeviceSeek,    CFLi_FileDeviceCreateDirectory,
};

extern "C" {
// 0x00154D78 | nintendogs:callgraph [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE const CFLiDeviceInterface* CFLi_GetDeviceInterfaceFile()
{
    return &s_FileDeviceInterface;
}

// 0x001554C0 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE CFLiDevice* CFLi_InitializeDevice(void** buffer, u32* bufferSize, const char* path, const CFLiDeviceInterface* iface)
{
    CFLiDevice* device = iface->Create(buffer, bufferSize);
    device->iface = iface;
    strlcpy(device->path, path, sizeof(device->path));
    return device;
}

// 0x001546A0 | nintendogs:bytes [tier B]
DECOMP_NOINLINE s32 CFLi_FlushDevice(CFLiFileDevice* device)
{
    nn::Result result = device->file.TryFlush();
    device->device.result = result;
    if (result.IsSuccess()) {
        device->device.status = CFL_RESULT_SUCCESS;
        device->device.value = 0;
    } else {
        SetFsError(&device->device);
    }
    return device->device.status;
}

// opens the device, creating the file when it is missing and the device writes
// 0x0013A510 | nintendogs:bytes [tier B]
DECOMP_NOINLINE void CFLi_SequenceOpen(CFLiDevice* device, CFLiSequenceCallback callback)
{
    BeginWorking();
    device->state = 0;
    device->callback = callback;
    device->offset = 0;
    device->buffer = NULL;
    device->size = 0;
    device->step = CFLi_SequenceOpenStep;
    device->_10 = false;
    CFLi_SequenceOpenStep(device, 0);
}
} // extern "C"

// 0x0013A4B8 (name is ours)
DECOMP_NOINLINE void CFLi_DestroyDevice(CFLiDevice* device)
{
    device->iface->Destroy(device);
}

// 0x0013DFF0 (name is ours)
void CFLi_SequenceOpenStep(CFLiDevice* device, s32)
{
    for (;;) {
        switch (device->state) {
        case 0:
            if (!device->opened) {
                device->state = 2;
                device->status = CFL_RESULT_SUCCESS;
                continue;
            }
            device->state = 1;
            device->status =
                device->iface->Close != NULL ? device->iface->Close(device) : static_cast<s32>(CFL_RESULT_BROKEN);
            if (device->status == CFL_RESULT_BUSY) {
                return;
            }
            continue;
        case 1:
            if (device->status != CFL_RESULT_SUCCESS) {
                FinishSequence(device);
                return;
            }
            device->state = 2;
            continue;
        case 2:
            device->state = 3;
            device->opened = false;
            device->status =
                device->iface->Open != NULL ? device->iface->Open(device) : static_cast<s32>(CFL_RESULT_BROKEN);
            if (device->status == CFL_RESULT_BUSY) {
                return;
            }
            continue;
        case 3:
            if (device->value == CFL_REASON_NOT_FOUND) {
                if (device->create) {
                    device->state = 4;
                    device->status = device->iface->CreateFile != NULL
                                         ? device->iface->CreateFile(device, CFL_DATABASE_FILE_SIZE)
                                         : static_cast<s32>(CFL_RESULT_BROKEN);
                    if (device->status == CFL_RESULT_BUSY) {
                        return;
                    }
                    continue;
                }
                device->opened = false;
            } else {
                device->opened = device->value == 0;
            }
            FinishSequence(device);
            return;
        case 4:
            device->state = 5;
            if (device->status == CFL_RESULT_SUCCESS) {
                device->state = 2;
                continue;
            }
            FinishSequence(device);
            return;
        default:
            return;
        }
    }
}

// writes size bytes at offset
// 0x0013DF70 (name is ours)
DECOMP_NOINLINE void CFLi_SequenceWrite(CFLiDevice* device, const void* buffer, u32 size, s32 offset, bool flush,
                        CFLiSequenceCallback callback)
{
    BeginWorking();
    device->flush = flush;
    device->state = 0;
    device->callback = callback;
    device->buffer = const_cast<void*>(buffer);
    device->offset = offset;
    device->size = size;
    device->_10 = false;
    device->step = CFLi_SequenceWriteStep;
    CFLi_SequenceWriteStep(device, 0);
}

// 0x00140204 (name is ours)
void CFLi_SequenceWriteStep(CFLiDevice* device, s32)
{
    for (;;) {
        switch (device->state) {
        case 0:
            device->state = 1;
            device->status = device->iface->Seek != NULL ? device->iface->Seek(device, device->offset)
                                                         : static_cast<s32>(CFL_RESULT_BROKEN);
            if (device->status == CFL_RESULT_BUSY) {
                return;
            }
            continue;
        case 1:
            device->state = 2;
            if (device->status == CFL_RESULT_SUCCESS) {
                if (device->value != device->offset) {
                    device->status = CFL_RESULT_5;
                } else {
                    device->status = device->iface->Write != NULL
                                         ? device->iface->Write(device, device->buffer, device->size, device->flush)
                                         : static_cast<s32>(CFL_RESULT_BROKEN);
                    if (device->status == CFL_RESULT_BUSY) {
                        return;
                    }
                    continue;
                }
            }
            FinishSequence(device);
            return;
        case 2:
            if (device->status == CFL_RESULT_SUCCESS && static_cast<u32>(device->value) != device->size) {
                device->status = CFL_RESULT_5;
            }
            FinishSequence(device);
            return;
        default:
            return;
        }
    }
}

// reads size bytes at offset; the buffer is cleared when that fails
// 0x00165F08 (name is ours)
void CFLi_SequenceReadStep(CFLiDevice* device, s32)
{
    for (;;) {
        switch (device->state) {
        case 0:
            device->state = 1;
            device->status = device->iface->Seek != NULL ? device->iface->Seek(device, device->offset)
                                                         : static_cast<s32>(CFL_RESULT_BROKEN);
            if (device->status == CFL_RESULT_BUSY) {
                return;
            }
            continue;
        case 1:
            device->state = 2;
            if (device->status == CFL_RESULT_SUCCESS) {
                if (device->value != device->offset) {
                    device->status = CFL_RESULT_4;
                } else {
                    device->status = device->iface->Read != NULL
                                         ? device->iface->Read(device, device->buffer, device->size)
                                         : static_cast<s32>(CFL_RESULT_BROKEN);
                    if (device->status == CFL_RESULT_BUSY) {
                        return;
                    }
                    continue;
                }
            }
            FinishSequence(device);
            return;
        case 2:
            device->state = 3;
            if (device->status == CFL_RESULT_SUCCESS) {
                if (static_cast<u32>(device->value) == device->size) {
                    FinishSequence(device);
                    return;
                }
                device->status = CFL_RESULT_4;
            }
            std::memset(device->buffer, 0, device->size);
            FinishSequence(device);
            return;
        default:
            return;
        }
    }
}

// closes the device; the result of the operation before stays
// 0x00140398 (name is ours)
void CFLi_SequenceCloseStep(CFLiDevice* device, s32)
{
    for (;;) {
        switch (device->state) {
        case 0: {
            device->state = 1;
            CFLiManager* manager = CFLi_GetManager();
            manager->savedStatus = manager->status;
            manager->savedReason = manager->reason;
            device->status =
                device->iface->Close != NULL ? device->iface->Close(device) : static_cast<s32>(CFL_RESULT_BROKEN);
            if (device->status == CFL_RESULT_BUSY) {
                return;
            }
            continue;
        }
        case 1:
            device->state = 2;
            if (device->value == 0) {
                device->opened = false;
                device->status = CFLi_GetManager()->savedStatus;
                device->value = CFLi_GetManager()->savedReason;
            }
            FinishSequence(device);
            return;
        default:
            return;
        }
    }
}

// copies a file (up to maxSize bytes, all for -1) in blocks of 128 KiB
// 0x0013A594 (name is ours)
DECOMP_NOINLINE s32 CFLi_CopyFile(const char* dst, const char* src, s32 maxSize)
{
    nn::fs::FileInputStream input;
    if (input.TryInitialize(src).IsSuccess()) {
        s64 size;
        input.FileBase::TryGetSize(&size);
        s32 fileSize = size;
        if (fileSize != 0) {
            nn::fs::FileOutputStream output;
            nn::Result result = output.TryInitialize(dst);
            if (IsNotFound(result)) {
                if (TryCreateFile(dst, size).IsFailure()) {
                    return CFL_RESULT_FILE_ERROR;
                }
                result = output.TryInitialize(dst);
            }
            if (result.IsSuccess()) {
                s32 copySize = (maxSize == -1 || maxSize > fileSize) ? fileSize : maxSize;
                s32 status = CFL_RESULT_SUCCESS;
                void* buffer = CFLi_AllocateMemory(0x20000, 32);
                for (s32 done = 0; done < copySize; done += 0x20000) {
                    s32 blockSize = done + 0x20000 <= copySize ? 0x20000 : copySize - done;
                    s32 readSize = blockSize;
                    nn::Result readResult = input.FileBase::TryRead(&readSize, buffer, blockSize);
                    if ((readResult.IsSuccess() ? readSize : 0) == 0) {
                        status = CFL_RESULT_FILE_ERROR;
                        break;
                    }
                    s32 writtenSize = readSize;
                    if (output.FileBase::TryWrite(&writtenSize, buffer, readSize, true).IsFailure()) {
                        status = CFL_RESULT_FILE_ERROR;
                        break;
                    }
                }
                CFLi_FreeMemory(buffer);
                return status;
            }
        }
    }
    return CFL_RESULT_FILE_ERROR;
}
