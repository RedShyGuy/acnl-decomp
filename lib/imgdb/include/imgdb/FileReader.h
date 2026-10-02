#pragma once

#include "decomp.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace imgdb {
// RTTI N5imgdb10FileReaderE @ 0x008D2A74
// vtable 0x00908E94 (vptr 0x00908E9C), offset_to_top 0, 4 entries
class FileReader : public ::nn::util::ADLFireWall::NonCopyable<imgdb::FileReader>
{
public:
    FileReader(); // ctor candidate(s) 0x005A45B8, 0x005A45F8 (unverified)
    virtual void vf_0x00(); // 0x005A4698 slot 0x00 | virtual slot, introduced by imgdb::FileReader
    virtual void vf_0x04(); // 0x005A4640 slot 0x04 | virtual slot, introduced by imgdb::FileReader
    virtual void Read(imgdb::StorageType, const wchar_t*); // 0x005A4280 slot 0x08 | nintendogs:bytes
    virtual void Read(imgdb::StorageType, const wchar_t*, long long, unsigned); // 0x005A42AC slot 0x0C | nintendogs:callgraph
    FileReader(void*, unsigned, bool); // 0x005A45B8 | nintendogs:bytes [tier B]
    FileReader(imgdb::Allocator&, unsigned, bool); // 0x005A45F8 | nintendogs:bytes [tier B]
};
} // namespace imgdb
