#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadAudioMgr.h"
#include "sead/seadAudioPlayerCtr.h"
#include "sead/seadAudioResourceLoader.h"
#include "sead/seadAudioResourceLoaderCtr.h"
#include "sead/seadAudioSoundDataMgrCtr.h"
#include "sead/seadHeapMgr.h"

namespace sead {
// 0x0013F6CC slot 0x08
// 0x0054B96C (deleting dtor)
sead::AudioResourceLoaderCtr::~AudioResourceLoaderCtr()
{
    // empty (in the original too)
}

// 0x0054B744 slot 0x10 (name is ours)
void sead::AudioResourceLoaderCtr::initialize(AudioMgr* mgr)
{
    mMgr = mgr;
    loadArchive_(mHeap);
}

// 0x0054B750 (name is ours)
void sead::AudioResourceLoaderCtr::loadArchive_(Heap* heap)
{
    AudioPlayerCtr* player = DynamicCast<AudioPlayerCtr>(mMgr->mPlayer);
    if (heap == nullptr) {
        heap = HeapMgr::s_pInstance->getCurrentHeap();
    }
    switch (mArchiveKind) {
    case ARCHIVE_KIND_ROM:
        player->m_pSoundDataMgr->openRomArchive(mPath, heap, true);
        break;
    case ARCHIVE_KIND_MEMORY:
        player->m_pSoundDataMgr->openMemoryArchive(mArchiveData, heap);
        break;
    case ARCHIVE_KIND_FS:
        player->m_pSoundDataMgr->openFsArchive(mPath, heap, true);
        break;
    default:
        break;
    }
}

// 0x0054B84C slot 0x14 (name is ours)
void sead::AudioResourceLoaderCtr::setupPlayer()
{
    setupPlayer_(mHeap);
}

// 0x0054B854 (name is ours)
void sead::AudioResourceLoaderCtr::setupPlayer_(Heap* heap)
{
    AudioPlayerCtr* player = DynamicCast<AudioPlayerCtr>(mMgr->mPlayer);
    if (heap == nullptr) {
        heap = HeapMgr::s_pInstance->getCurrentHeap();
    }
    if (mExtraStreamBufferSize != 0) {
        player->setupBuffers(mExtraStreamBufferSize, mStreamCacheSize, mUserParamSizePerSound, heap);
    } else {
        AudioPlayerCtr::SetupArg arg;
        arg.streamBufferScale = mStreamBufferScale;
        arg.streamCacheSize = mStreamCacheSize;
        arg.userParamSizePerSound = mUserParamSizePerSound;
        arg.heap = heap;
        player->setupBuffers(arg);
    }
    player->createSoundHeap(mSoundHeapSize, heap);
}

// 0x0054B968 slot 0x18 (name is ours)
void sead::AudioResourceLoaderCtr::finalize()
{
    // empty (in the original too)
}
} // namespace sead

// the type information of the classes below AudioResourceLoader
// 0x00829224 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::AudioResourceLoader>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
