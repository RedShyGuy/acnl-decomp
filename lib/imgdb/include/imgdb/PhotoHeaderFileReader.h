#pragma once

#include "decomp.h"
#include "imgdb/FileReader.h"

namespace imgdb {
// RTTI N5imgdb21PhotoHeaderFileReaderE @ 0x008D2BBC
// vtable 0x00909018 (vptr 0x00909020), offset_to_top 0, 4 entries
class PhotoHeaderFileReader : public ::imgdb::FileReader
{
public:
    PhotoHeaderFileReader(); // ctor address unknown
    virtual void vf_0x00(); // 0x005AB744 slot 0x00 | nintendogs:callgraph
    virtual void vf_0x04(); // 0x005AB6EC slot 0x04 | virtual slot, introduced by imgdb::FileReader
    virtual void Read(imgdb::StorageType, const wchar_t*); // 0x005AB658 slot 0x08 | slot vf_0x08 of imgdb::FileReader
    virtual void Read(imgdb::StorageType, const wchar_t*, long long, unsigned); // 0x005AB67C slot 0x0C | nintendogs:bytes
    void SetDefaultLimitFileSize(unsigned); // 0x005AB640 | nintendogs:bytes [tier B]
};
} // namespace imgdb
