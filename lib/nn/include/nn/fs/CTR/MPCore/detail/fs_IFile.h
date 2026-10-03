#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
// RTTI N2nn2fs3CTR6MPCore6detail5IFileE @ 0x008CDD08
// An open file of an IArchive. The names of the slots without a symbol (0x08, 0x1C, 0x20, 0x2C)
// are ours, after the FS command the FileServerArchive::File slot sends.
class IFile
{
public:
    virtual nn::Result TryRead(s32* readSize, s64 offset, void* buffer, size_t size) = 0; // slot 0x00
    virtual nn::Result TryWrite(s32* writtenSize, s64 offset, const void* buffer, size_t size, bool flush) = 0; // slot 0x04
    virtual nn::Result TryGetAvailable(s64* available, s64 offset, s64 size) = 0; // slot 0x08
    virtual nn::Result TryGetSize(s64* size) const = 0; // slot 0x0C
    virtual nn::Result TrySetSize(s64 size) = 0; // slot 0x10
    virtual nn::Result TryFlush() = 0; // slot 0x14
    virtual nn::Result TrySetPriority(s32 priority) = 0; // slot 0x18
    virtual nn::Result TryGetPriority(s32* priority) const = 0; // slot 0x1C
    virtual nn::Result OpenSubFile(nn::Handle* file, s64 offset, s64 size) = 0; // slot 0x20
    virtual nn::Result OpenLinkHandle(nn::Handle* handle) = 0; // slot 0x24
    virtual nn::Handle GetFileHandle() = 0; // slot 0x28
    // forgets the handle without closing it
    virtual void DetachFileHandle() = 0; // slot 0x2C
    // closes and destroys the file, gives its memory back
    virtual void Close() = 0; // slot 0x30
    virtual ~IFile() {} // slots 0x34, 0x38
};
} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn
