#pragma once

#include "decomp.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace imgdb {
// RTTI N5imgdb10FileWriterE @ 0x008D2A8C
// vtable 0x00908EAC (vptr 0x00908EB4), offset_to_top 0, 3 entries
class FileWriter : public ::nn::util::ADLFireWall::NonCopyable<imgdb::FileWriter>
{
public:
    FileWriter(); // ctor candidate(s) 0x005A49C4 (unverified)
    virtual void vf_0x00(); // 0x005A49EC slot 0x00 | virtual slot, introduced by imgdb::FileWriter
    virtual void vf_0x04(); // 0x005A49E8 slot 0x04 | virtual slot, introduced by imgdb::FileWriter
    virtual void vf_0x08(); // 0x005A46EC slot 0x08 | nintendogs:callgraph
    FileWriter(imgdb::Allocator&); // 0x005A49C4 | nintendogs:bytes [tier B]
};
} // namespace imgdb
