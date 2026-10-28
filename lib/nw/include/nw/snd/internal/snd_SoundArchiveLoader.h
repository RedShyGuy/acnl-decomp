#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundMemoryAllocatable.h"

namespace nw {
namespace snd {
class SoundArchive;

namespace internal {
// Loads the files of the items of a sound archive (sounds, sound groups, banks, wave archives, groups)
// into memory and keeps their addresses in a table of the derived class (the class and function names
// are from the reference symbols, the members and the names of the flags are ours).
class SoundArchiveLoader
{
public:
    // what of an item is loaded
    enum LoadFlag
    {
        LOAD_FLAG_SEQUENCE = 0x1,
        LOAD_FLAG_WAVE_SOUND = 0x2,
        LOAD_FLAG_BANK = 0x4,
        LOAD_FLAG_WAVE_ARCHIVE = 0x8,
    };
    // the buffer of the file streams of the archive
    static const int STREAM_AREA_SIZE = 512;

    SoundArchiveLoader(); // 0x0013C2EC | nintendogs:bytes [tier A]
    virtual ~SoundArchiveLoader(); // 0x004C8CF4 slot 0x00 | fefates:callgraph

    // the address of a loaded file into the table; the old one
    virtual const void* SetFileAddressToTable(u32 fileId, const void* pAddress) = 0; // slot 0x08
    virtual const void* GetFileAddressFromTable(u32 fileId) const = 0; // slot 0x0C
    // the address of a file in the table or in the archive (NULL: not loaded)
    virtual const void* GetFileAddressImpl(u32 fileId) const = 0; // slot 0x10

    void SetSoundArchive(const SoundArchive* pSoundArchive); // 0x004C8B2C | nintendogs:callgraph [tier A]
    bool IsAvailable() const; // 0x001347B8 | nintendogs:callgraph [tier A]

    // loads the files of an item; loadBlockSize: the bytes read at once (0: all)
    bool LoadData(u32 itemId, SoundMemoryAllocatable* pAllocator, u32 loadFlag, u32 loadBlockSize); // 0x00132564 | nintendogs:callgraph [tier A]
    // whether the files of an item are loaded
    bool IsDataLoaded(u32 itemId, u32 loadFlag) const; // 0x00134990 (name is ours)

    bool LoadSequenceSound(u32 soundId, SoundMemoryAllocatable* pAllocator, u32 loadFlag, u32 loadBlockSize); // 0x00137F44 | nintendogs:bytes [tier A]
    bool LoadWaveSound(u32 soundId, SoundMemoryAllocatable* pAllocator, u32 loadFlag, u32 loadBlockSize, u32 waveSoundSetId); // 0x0013BC2C | nintendogs:bytes [tier A]
    bool LoadSoundGroup(u32 soundGroupId, SoundMemoryAllocatable* pAllocator, u32 loadFlag, u32 loadBlockSize); // 0x00137D94 | nintendogs:bytes [tier A]
    bool LoadBank(u32 bankId, SoundMemoryAllocatable* pAllocator, u32 loadFlag, u32 loadBlockSize); // 0x0013BFB8 | nintendogs:callgraph [tier A]
    bool LoadWaveArchiveImpl(u32 waveArchiveId, u32 waveIndex, SoundMemoryAllocatable* pAllocator, u32 loadFlag, u32 loadBlockSize); // 0x0013BDC0 | nintendogs:bytes [tier A]
    // one wave of a wave archive that is loaded individually
    bool LoadIndividualWave(u32 waveArchiveId, u32 waveIndex, SoundMemoryAllocatable* pAllocator, u32 loadBlockSize); // 0x0013F19C | nintendogs:callgraph [tier A]
    // the header of an individually loaded wave archive with an empty table of its waves
    const void* LoadWaveArchiveTable(u32 waveArchiveId, SoundMemoryAllocatable* pAllocator, u32 loadBlockSize); // 0x0014123C | nintendogs:callseq [tier A]
    // the files of a loaded group into the table and the wave archive tables of its items
    bool PostProcessForLoadedGroupFile(const void* pGroupFile, SoundMemoryAllocatable* pAllocator, u32 loadBlockSize); // 0x00138028 | nintendogs:callseq [tier A]
    // for an individually loaded wave archive embedded in a group: a table of its waves
    void SetWaveArchiveTableInEmbeddedGroupImpl(u32 waveArchiveId, SoundMemoryAllocatable* pAllocator); // 0x0013BE80 | nintendogs:callseq [tier A]
    // a file into memory (isDeviceMemory: the memory must be device memory); its address (NULL: failed)
    const void* LoadImpl(u32 fileId, SoundMemoryAllocatable* pAllocator, u32 loadBlockSize, bool isDeviceMemory); // 0x0013C1C4 | nintendogs:bytes [tier A]
    // size bytes of a file from offset; the bytes read (-1: failed or canceled)
    s32 ReadFile(u32 fileId, void* pBuffer, s32 size, s32 offset, u32 loadBlockSize); // 0x001413C4 | nintendogs:callgraph [tier A]

    bool IsSequenceSoundDataLoaded(u32 soundId, u32 loadFlag) const; // 0x00139BE0 | nintendogs:bytes [tier B]
    bool IsWaveSoundDataLoaded(u32 soundId, u32 loadFlag) const; // 0x001348A8 | nintendogs:bytes [tier B]
    bool IsSoundGroupDataLoaded(u32 soundGroupId, u32 loadFlag) const; // 0x00134AA0 | nintendogs:bytes [tier B]
    bool IsBankDataLoaded(u32 bankId, u32 loadFlag) const; // 0x001347D8 (name is ours)
    // the wave of an archive (waveIndex -1: all waves) is loaded
    bool IsWaveArchiveDataLoaded(u32 waveArchiveId, u32 waveIndex) const; // 0x00139ADC (name is ours)

    bool detail_LoadWaveArchiveByBankFile(const void* pBankFile, SoundMemoryAllocatable* pAllocator); // 0x004C8B34 | nintendogs:callseq [tier A]
    bool detail_LoadWaveArchiveByWaveSoundFile(const void* pWaveSoundFile, int index, SoundMemoryAllocatable* pAllocator); // 0x004C8C54 | nintendogs:bytes [tier A]
    // the address of the loaded file of an item (NULL: not loaded)
    const void* detail_GetFileAddressByItemId(u32 itemId) const; // 0x007406F8 | nintendogs:callseq-callee [tier A]
    const void* GetFileAddressFromSoundArchive(u32 fileId) const; // 0x007407DC | nintendogs:bytes [tier A]

    const SoundArchive* m_pSoundArchive;   // 0x004
    u8 m_StreamArea[STREAM_AREA_SIZE];     // 0x008
    u16 m_LoadingCount;                    // 0x208 the calls of LoadData in progress
    bool m_IsCancelLoading;                // 0x20A
};
ASSERT_SIZE(SoundArchiveLoader, 0x20C);
} // namespace internal
} // namespace snd
} // namespace nw
