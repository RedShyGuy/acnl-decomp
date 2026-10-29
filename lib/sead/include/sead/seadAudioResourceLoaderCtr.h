#pragma once

#include "decomp.h"
#include "sead/seadSafeStringBase.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadAudioResourceLoader.h"

namespace sead {
class Heap;

// RTTI N4sead22AudioResourceLoaderCtrE @ 0x008D1EF0
// vtable 0x0090668C (vptr 0x00906694), offset_to_top 0, 7 entries
class AudioResourceLoaderCtr : public ::sead::AudioResourceLoader
{
public:
    AudioResourceLoaderCtr(); // (made by the game)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074DC94 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return AudioResourceLoader::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074DC48 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983544 (guard 0x00983540)
        static const RuntimeTypeInfo::Derive<AudioResourceLoader> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~AudioResourceLoaderCtr(); // 0x0013F6CC slot 0x08
    // 0x0054B96C slot 0x0C (deleting dtor)
    // (the original falls into loadArchive_ with a nop)
    virtual void initialize(AudioMgr* mgr); // 0x0054B744 slot 0x10
    // (the original falls into setupPlayer_ with a nop)
    virtual void setupPlayer(); // 0x0054B84C slot 0x14
    virtual void finalize(); // 0x0054B968 slot 0x18

    // opens the archive into the data manager of the player
    void loadArchive_(Heap* heap); // 0x0054B750
    // the buffers of the player and its sound heap
    void setupPlayer_(Heap* heap); // 0x0054B854

    // where the archive is (names are ours)
    enum ArchiveKind : u8
    {
        ARCHIVE_KIND_NONE = 0,
        ARCHIVE_KIND_ROM = 1,
        ARCHIVE_KIND_MEMORY = 2,
        ARCHIVE_KIND_FS = 3,
    };

    ArchiveKind mArchiveKind;        // 0x04
    Heap* mHeap;                     // 0x08
    u32 mSoundHeapSize;              // 0x0C
    u32 mExtraStreamBufferSize;      // 0x10, 0: mStreamBufferScale
    f32 mStreamBufferScale;          // 0x14
    u32 mStreamCacheSize;            // 0x18
    u32 mUserParamSizePerSound;      // 0x1C
    SafeStringBase<char> mPath;      // 0x20
    const void* mArchiveData;        // 0x28
    AudioMgr* mMgr;                  // 0x2C
};
ASSERT_SIZE(AudioResourceLoaderCtr, 0x30);
} // namespace sead
