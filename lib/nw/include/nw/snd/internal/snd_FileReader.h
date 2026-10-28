#pragma once

#include "decomp.h"
#include "nn/err/CTR/CTR_Api.h"
#include "nn/fs/fs_FileInputStream.h"

namespace nw {
namespace snd {
namespace internal {
// A file of the file system for the sound streams (the class and Initialize/Read/Finalize are from the
// reference symbols, the members are ours). In "try" mode the results are kept in m_Result, else a
// failure is fatal.
class FileReader
{
public:
    // the priority of the file system accesses (passed on to the file system; the values are what the sound
    // archives pass for their FileStreamPriority; names are ours)
    enum Priority
    {
        PRIORITY_HIGH = -16,
        PRIORITY_NORMAL = -8,
        PRIORITY_LOW = 8,
    };

    // (inline)
    FileReader() : m_Result(RESULT_NOT_INITIALIZED) {}

    // opens the file for reading
    bool Initialize(const char* pPath, bool isTry); // 0x004C5B98 | fefates:bytes [tier B]
    bool Read(void* pBuffer, int size, int* pReadSize); // 0x004C5CF8 | fefates:bytes [tier B]
    // origin: io::FileStream::SeekOrigin
    bool Seek(int offset, int origin); // 0x004C5D5C (name is ours; symbols.json says FileBase::TrySeek)
    void Finalize(); // 0x004C5DD8 | fefates:bytes [tier B]

    // (inline; name is ours)
    bool GetSize(s32* pSize)
    {
        s64 size = 0;
        if (m_IsTry) {
            m_Result = m_File.nn::fs::detail::FileBase::TryGetSize(&size);
            if (m_Result.IsSuccess()) {
                *pSize = size;
            }
            return m_Result.IsSuccess();
        }
        nn::Result result = m_File.nn::fs::detail::FileBase::TryGetSize(&size);
        nn::err::CTR::ThrowFatalErrAllIfFailure(result);
        *pSize = size;
        return true;
    }
    // (inline; name is ours)
    bool SetPriority(Priority priority)
    {
        if (m_IsTry) {
            m_Result = m_File.TrySetPriority(priority);
            return m_Result.IsSuccess();
        }
        nn::Result result = m_File.TrySetPriority(priority);
        nn::err::CTR::ThrowFatalErrAllIfFailure(result);
        return true;
    }

    // the value of m_Result before the first access
    static const bit32 RESULT_NOT_INITIALIZED = 0xE7E3FFFF; // (name is ours)

    nn::fs::FileInputStream m_File; // 0x00
    nn::Result m_Result;            // 0x18 the last result in try mode
    bool m_IsTry;                   // 0x1C
};
ASSERT_SIZE(FileReader, 0x20);
} // namespace internal
} // namespace snd
} // namespace nw
