#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundDataManager.h"
#include "sead/seadAudioFsSoundArchiveCtr.h"
#include "sead/seadAudioMemorySoundArchiveCtr.h"
#include "sead/seadAudioRomSoundArchiveCtr.h"
#include "sead/seadAudioSoundArchiveBaseCtr.h"

namespace sead {
class AudioSoundHeapCtr;
class Heap;
template <typename T>
class SafeStringBase;

// RTTI N4sead20AudioSoundDataMgrCtrE @ 0x008D1E34
// vtable 0x009063F4 (vptr 0x009063FC), offset_to_top 0, 6 entries
// vtable 0x00906414 (vptr 0x0090641C), offset_to_top -12, 5 entries
// The data manager of the sound archive player with the archive it opens and the sound heap the data
// is loaded into (member names and the names of the open functions are ours).
class AudioSoundDataMgrCtr : public ::nw::snd::SoundDataManager
{
public:
    AudioSoundDataMgrCtr(); // 0x001332A0 | nintendogs:bytes [tier A]
    virtual ~AudioSoundDataMgrCtr(); // 0x0054A6B0 slot 0x00
    // 0x0054A634 slot 0x04 (deleting dtor)

    // the data of the item into heap (NULL: the connected sound heap)
    bool loadData(u32 itemId, u32 loadFlag, u32 loadBlockSize, nw::snd::SoundHeap* heap); // 0x0012CB74 | nintendogs:bytes [tier A]
    void connectSoundHeap(AudioSoundHeapCtr* heap); // 0x0054A24C | nintendogs:bytes [tier A]
    // the archive (NULL without one)
    DECOMP_NOINLINE nw::snd::SoundArchive* getSoundArchive() const // 0x0074CF8C | nintendogs:callgraph [tier A]
    {
        if (mArchive == nullptr) {
            return nullptr;
        }
        switch (mArchive->m_ArchiveType) {
        case 0: {
            AudioRomSoundArchiveCtr* archive = DynamicCast<AudioRomSoundArchiveCtr>(mArchive);
            if (archive != nullptr) {
                return archive;
            }
            break;
        }
        case 1: {
            AudioMemorySoundArchiveCtr* archive = DynamicCast<AudioMemorySoundArchiveCtr>(mArchive);
            if (archive != nullptr) {
                return archive;
            }
            break;
        }
        case 2: {
            AudioFsSoundArchiveCtr* archive = DynamicCast<AudioFsSoundArchiveCtr>(mArchive);
            if (archive != nullptr) {
                return archive;
            }
            break;
        }
        }
        return nullptr;
    }
    // closes the archive and frees the table
    DECOMP_EMIT_COPY void finalize() // 0x0054A280
    {
        if (mIsInitialized) {
            Finalize();
            mIsInitialized = false;
        }
        if (mArchive != nullptr) {
            delete mArchive;
            mArchive = nullptr;
        }
        if (mWorkMem != nullptr) {
            delete[] mWorkMem;
            mWorkMem = nullptr;
        }
    }
    // the archive on the file system, on the ROM, in memory, and the table of its files
    void openFsArchive(const SafeStringBase<char>& path, Heap* heap, bool loadLabelString); // 0x0054A2D8
    void openRomArchive(const SafeStringBase<char>& path, Heap* heap, bool loadLabelString); // 0x0054A430
    void openMemoryArchive(const void* data, Heap* heap); // 0x0054A580
    // the table of the files of the opened archive
    void initializeTable_(Heap* heap)
    {
        u32 size = GetRequiredMemSize(getSoundArchive());
        mWorkMem = new (heap, 32) u8[size];
        Initialize(getSoundArchive(), mWorkMem, size);
        mIsInitialized = true;
    }

    AudioSoundArchiveBaseCtr* mArchive; // 0x220
    u8* mWorkMem;                        // 0x224
    AudioSoundHeapCtr* mSoundHeap;       // 0x228
    bool mIsInitialized;                 // 0x22C
};
ASSERT_SIZE(AudioSoundDataMgrCtr, 0x230);
} // namespace sead
