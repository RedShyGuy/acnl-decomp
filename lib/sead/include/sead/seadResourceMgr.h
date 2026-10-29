#pragma once

#include "decomp.h"
#include "sead/seadSafeStringBase.h"
#include "sead/seadTListNode.h"

namespace sead {
class Decompressor;
class FileDevice;
class Heap;
class ResourceFactory;

// The factories that make resources from loaded files. A singleton without a vtable whose
// SingletonDisposer_ lies in its first 16 bytes (as FileDeviceMgr); the constructor creates the null
// factory (a DirectResourceFactory<DirectResource>), the default factory when no other is set.
// unregisterFactory is the name of the reference symbols, all other names are ours.
class ResourceMgr
{
public:
    class SingletonDisposer_;

    // what a factory loads (the fields the factories read)
    struct LoadArg
    {
        SafeStringBase<char> path;  // 0x00
        Heap* instanceHeap;         // 0x08, the heap of the resource object
        Heap* loadDataHeap;         // 0x0C, the heap of the loaded file
        s32 instanceAlignment;      // 0x10 (its sign also gives the direction of the loaded file)
        s32 loadDataAlignment;      // 0x14, 0: the one of the resource
        u8* loadDataBuffer;         // 0x18
        u32 loadDataBufferSize;     // 0x1C
        ResourceFactory* factory;   // 0x20 (not read by the factories)
        FileDevice* device;         // 0x24, NULL: by the drive of path
        u32 divSize;                // 0x28
        bool field_0x2C;            // 0x2C (FileDevice::LoadArg::field_0x1C)
    };

    // what tryCreate makes a resource from (a file already in memory)
    struct CreateArg
    {
        u8* buffer;           // 0x00
        u32 fileSize;         // 0x04
        u32 bufferSize;       // 0x08
        bool needUnload;      // 0x0C, the resource frees buffer
        u32 field_0x10[3];    // 0x10 (not read by the factories)
        Heap* heap;           // 0x1C
        s32 alignment;        // 0x20
    };

    ResourceMgr(); // 0x00132D30
    // (inline in deleteInstance and ~SingletonDisposer_; defined in seadResourceFactory.h, where the factory is
    // complete and its virtual destructor is called)
    inline ~ResourceMgr();

    static ResourceMgr* createInstance(Heap* heap); // 0x0012C43C
    static void deleteInstance(); // 0x0012C4A0

    // gives factory the name and adds it to the list
    void registerFactory(ResourceFactory* factory, const SafeStringBase<char>& name); // 0x00138AE0
    void unregisterFactory(ResourceFactory* factory); // 0x00540A8C | nintendogs:bytes [tier A]
    // makes factory (NULL: the null factory) the default one; returns the one before
    ResourceFactory* setDefaultFactory(ResourceFactory* factory); // 0x00540A44

    static ResourceMgr* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 mSingletonDisposerBuffer[4];      // 0x00, the SingletonDisposer_
    TList<ResourceFactory*> mFactoryList; // 0x10
    // presumably the decompressors (nothing in this build adds to it)
    TList<Decompressor*> mDecompList;     // 0x1C
    ResourceFactory* mNullFactory;        // 0x28
    ResourceFactory* mDefaultFactory;     // 0x2C
};
ASSERT_SIZE(ResourceMgr, 0x30);
} // namespace sead
