#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/fs/CTR/MPCore/detail/fs_UserFileSystem.h"
#include "nn/fs/detail/fs_FileBaseImpl.h"
#include "nn/fs/fs_Types.h"

namespace nn {
namespace fs {
namespace detail {
// RTTI N2nn2fs6detail8FileBaseE @ 0x008CDD54
// An open file of the user file system: the IFile (as void*), the position and the size as last
// read. The streams contain one. Member names, the inline functions and the names marked so are
// ours.
class FileBase : public ::nn::fs::detail::FileBaseImpl
{
public:
    FileBase() : mHandle(0), mPosition(0), mSize(0) {}
    ~FileBase() { Finalize(); }

    // inline (e.g. in nn::ubl)
    nn::Result TryInitialize(const wchar_t* path, u32 mode)
    {
        mSize = 0;
        mPosition = 0;
        return nn::fs::CTR::MPCore::detail::UserFileSystem::TryOpenFile(reinterpret_cast<void**>(&mHandle), path, mode);
    }
    // a path in the multibyte encoding of the C library
    nn::Result TryInitialize(const char* path, u32 mode); // 0x0013E638 (name is ours)

    // closes the file; written data must have been flushed (inline)
    void Finalize()
    {
        if (GetHandle() != 0) {
            if (mHandle & HANDLE_NOT_FLUSHED) {
                nndbgPanic();
            }
            nn::fs::CTR::MPCore::detail::UserFileSystem::CloseFile(GetHandle());
            mHandle = 0;
        }
    }

    nn::Result TryRead(s32* readSize, void* buffer, size_t size); // 0x0013E6E0 | nintendogs:bytes [tier A]
    nn::Result TryWrite(s32* writtenSize, const void* buffer, size_t size, bool flush); // 0x0013E768 | fefates:bytes [tier B]
    nn::Result TryGetSize(s64* size) const; // 0x0013F9B8 | nintendogs:bytes [tier A]
    nn::Result TrySetSize(s64 size); // 0x0034971C | fefates:bytes [tier B]
    nn::Result TrySetPosition(s64 position); // 0x0034976C | nintendogs:bytes [tier A]
    nn::Result TrySeek(s64 offset, nn::fs::PositionBase base); // 0x0034980C | nintendogs:bytes [tier A]
    // (symbols.json also names 0x004C5D5C FileBase::TrySeek; that one belongs to another class)
    nn::Result TryFlush(); // 0x003498C4 | tier C (confirmed by the code)

    // inline
    nn::Result TryGetPosition(s64* position) const
    {
        *position = mPosition;
        return nn::Result();
    }
    s64 GetPosition() const { return mPosition; }

protected:
    // bit 0 of mHandle: data was written without a flush
    static const uptr HANDLE_NOT_FLUSHED = 1;

    void* GetHandle() const { return reinterpret_cast<void*>(mHandle & ~HANDLE_NOT_FLUSHED); }

    uptr mHandle;               // 0x00, the IFile
    s64_align4 mPosition;       // 0x04
    mutable s64_align4 mSize;   // 0x0C

    static void CheckLayout()
    {
        ASSERT_OFFSET(FileBase, mPosition, 0x4);
        ASSERT_OFFSET(FileBase, mSize, 0xC);
    }
};
ASSERT_SIZE(FileBase, 0x14);
} // namespace detail
} // namespace fs
} // namespace nn
