#pragma once

#include "decomp.h"
#include "imgdb/FileReader.h"

namespace imgdb {
// RTTI N5imgdb15PhotoFileReaderE @ 0x008D2B2C
// vtable 0x00908F40 (vptr 0x00908F48), offset_to_top 0, 4 entries
class PhotoFileReader : public ::imgdb::FileReader
{
public:
    PhotoFileReader(); // ctor address unknown
    virtual void vf_0x00(); // 0x005A9D38 slot 0x00 | nintendogs:callgraph
    virtual void vf_0x04(); // 0x005A9CE0 slot 0x04 | virtual slot, introduced by imgdb::FileReader
    void SetDefaultLimitFileSize(unsigned); // 0x005A9CC8 | nintendogs:bytes [tier B]
};
} // namespace imgdb
