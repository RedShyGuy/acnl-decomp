#pragma once

#include "imgdb/imgdb_Types.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace imgdb {

// RTTI N5imgdb10FileWriterE
// vtable 0x00908EAC (vptr 0x00908EB4), 3 entries
// writes a file (an existing one is replaced; a failed write deletes it)
class FileWriter : private nn::util::ADLFireWall::NonCopyable<FileWriter>
{
public:
    explicit FileWriter(Allocator& allocator);
    virtual ~FileWriter();

    virtual bool Write(StorageType storage, const wchar_t* path, const void* data, u32 size);

    Result GetResult() const { return static_cast<Result>(m_Result); }

protected:
    Allocator* m_pAllocator;  // +0x04
    u8 m_Result;              // +0x08 Result
    nn::Result m_FsResult;    // +0x0C
};
ASSERT_SIZE(FileWriter, 0x10);

} // namespace imgdb
