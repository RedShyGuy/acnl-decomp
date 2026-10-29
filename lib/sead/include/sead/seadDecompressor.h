#pragma once

#include "decomp.h"
#include "sead/seadIDisposer.h"
#include "sead/seadResourceMgr.h"
#include "sead/seadTListNode.h"

namespace sead {
class Resource;

// A decompressor for ResourceFactory::tryLoadWithDecomp. The class name is from the reference symbols
// (their destructor is folded with the one of FileDevice); this build has no decompressor, only the
// call of slot 0x08 in DirectResourceFactoryBase. The layout (as FileDevice/ResourceFactory) and the
// slot name are ours.
class Decompressor : public TListNode<Decompressor*>, public IDisposer
{
public:
    virtual ~Decompressor();
    // loads the file of arg decompressed for resource; size of the data, size of the buffer and
    // whether resource has to free it
    virtual u8* tryDecompFromDevice(const ResourceMgr::LoadArg& arg, Resource* resource, u32* outSize,
                                    u32* outAllocSize, bool* outNeedDelete) = 0; // slot 0x08
};
} // namespace sead
