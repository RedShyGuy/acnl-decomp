#pragma once

#include "decomp.h"
#include "Resource/dResourceLoadSkeletal.h"

// ResourceLoadSkeletal that loads its file in pieces (LoadFile is called until the file is there; names are ours).
// RTTI 25ResourceLoadAsyncSkeletal @ 0x008CD06C
// vtable 0x008F7CD4 (vptr 0x008F7CDC), offset_to_top 0, 11 entries
// vtable 0x008F7D30 (vptr 0x008F7D38), offset_to_top -312, 11 entries
class ResourceLoadAsyncSkeletal : public ::ResourceLoadSkeletal
{
public:
    ResourceLoadAsyncSkeletal(); // 0x0033FAD8
    virtual ~ResourceLoadAsyncSkeletal(); // 0x0033FB7C slot 0x00
    // 0x0033FB38 (deleting dtor)
    // (the setup of the file runs with the lock of the heap)
    bool LoadFile(const char* path) override; // 0x0033FA1C slot 0x24
};
