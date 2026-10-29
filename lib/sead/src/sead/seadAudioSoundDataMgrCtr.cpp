#include "sead/seadAudioSoundDataMgrCtr.h"
#include "sead/seadAudioSoundHeapCtr.h"
#include "sead/seadNew.h"
#include "sead/seadSafeStringBase.h"

namespace sead {
// 0x0012CB74 | nintendogs:bytes [tier A]
bool sead::AudioSoundDataMgrCtr::loadData(u32 itemId, u32 loadFlag, u32 loadBlockSize, nw::snd::SoundHeap* heap)
{
    if (heap == nullptr) {
        heap = mSoundHeap;
        if (heap == nullptr) {
            return false;
        }
    }
    if (!SoundArchiveLoader::IsAvailable()) {
        return false;
    }
    return SoundArchiveLoader::LoadData(itemId, heap, loadFlag, loadBlockSize);
}

// 0x001332A0 | nintendogs:bytes [tier A]
sead::AudioSoundDataMgrCtr::AudioSoundDataMgrCtr()
    : SoundDataManager(), mArchive(nullptr), mWorkMem(nullptr), mSoundHeap(nullptr), mIsInitialized(false)
{
}

// 0x0054A24C | nintendogs:bytes [tier A]
void sead::AudioSoundDataMgrCtr::connectSoundHeap(AudioSoundHeapCtr* heap)
{
    mSoundHeap = heap;
    if (heap == nullptr) {
        return;
    }
    nw::snd::SoundArchive* archive = getSoundArchive();
    if (archive != nullptr) {
        mSoundHeap->setSoundDataManagement(*this, *archive);
    }
}

// 0x0054A2D8 (name is ours)
void sead::AudioSoundDataMgrCtr::openFsArchive(const SafeStringBase<char>& path, Heap* heap, bool loadLabelString)
{
    mArchive = new (heap, 32) AudioFsSoundArchiveCtr(heap);
    DynamicCast<AudioFsSoundArchiveCtr>(mArchive)->m_IsLoadLabelString = loadLabelString;
    if (!mArchive->open(path.getCString())) {
        return;
    }
    initializeTable_(heap);
}

// 0x0054A430 (name is ours)
void sead::AudioSoundDataMgrCtr::openRomArchive(const SafeStringBase<char>& path, Heap* heap, bool loadLabelString)
{
    mArchive = new (heap, 32) AudioRomSoundArchiveCtr(heap);
    DynamicCast<AudioRomSoundArchiveCtr>(mArchive)->m_IsLoadLabelString = loadLabelString;
    mArchive->open(path.getCString());
    initializeTable_(heap);
}

// 0x0054A580 (name is ours)
void sead::AudioSoundDataMgrCtr::openMemoryArchive(const void* data, Heap* heap)
{
    mArchive = new (heap, 32) AudioMemorySoundArchiveCtr();
    mArchive->open(data);
    initializeTable_(heap);
}

// 0x0054A6B0 slot 0x00
// 0x0054A634 (deleting dtor)
// 0x00774520 (thunk 12)
// 0x007744A4 (deleting thunk 12)
sead::AudioSoundDataMgrCtr::~AudioSoundDataMgrCtr()
{
    finalize();
}
} // namespace sead
