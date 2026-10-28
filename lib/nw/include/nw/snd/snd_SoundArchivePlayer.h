#pragma once

#include "decomp.h"
#include "nw/io/io_FileStream.h"
#include "nw/snd/internal/driver/snd_MmlParser.h"
#include "nw/snd/internal/driver/snd_MmlSequenceTrackAllocator.h"
#include "nw/snd/internal/driver/snd_NoteOnCallback.h"
#include "nw/snd/internal/driver/snd_SequenceSoundLoader.h"
#include "nw/snd/internal/driver/snd_StreamBufferPool.h"
#include "nw/snd/internal/driver/snd_WaveSoundLoader.h"
#include "nw/snd/internal/snd_BasicSound.h"
#include "nw/snd/internal/snd_LoaderManager.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include "nw/snd/internal/snd_SoundInstanceManager.h"
#include "nw/snd/internal/snd_StreamSound.h"
#include "nw/snd/internal/snd_WaveSound.h"
#include "nw/snd/snd_SoundArchive.h"
#include "nw/snd/snd_SoundPlayer.h"
#include "nw/snd/snd_SoundStartable.h"

namespace nw {
namespace snd {
class SoundActor;
class SoundDataManager;

// Starts the sounds of a sound archive: the sound players, the sounds of each kind and their loaders in
// one buffer, the stream buffers in device memory (the class and function names are from the reference
// symbols, the members and the names of the unnamed parts are ours).
// RTTI N2nw3snd18SoundArchivePlayerE @ 0x008D0940
// vtable 0x00902FAC (vptr 0x00902FB4), offset_to_top 0, 4 entries
class SoundArchivePlayer : public ::nw::snd::SoundStartable
{
public:
    // the stream buffer of a stream channel (name is ours)
    static const u32 STREAM_BUFFER_SIZE_PER_CHANNEL = 0xA000;
    // the buffer of the file stream of a stream sound
    static const int STREAM_FILE_STREAM_BUFFER_SIZE = internal::StreamSound::FILE_STREAM_BUFFER_SIZE;

    // The bank of the notes of the sequences (the class name and NoteOn are from the reference symbols).
    // RTTI N2nw3snd18SoundArchivePlayer22SequenceNoteOnCallbackE @ 0x008D0934
    // vtable 0x00902F98 (vptr 0x00902FA0), offset_to_top 0, 3 entries
    class SequenceNoteOnCallback : public ::nw::snd::internal::driver::NoteOnCallback
    {
    public:
        // (inline)
        explicit SequenceNoteOnCallback(const SoundArchivePlayer& player) : m_SoundArchivePlayer(player) {}
        virtual ~SequenceNoteOnCallback(); // 0x004C2B7C slot 0x00

        virtual internal::driver::Channel* NoteOn(internal::driver::SequenceSoundPlayer* pPlayer, u8 bankIndex,
                                                  const internal::driver::NoteOnInfo& noteOnInfo); // 0x004C2AF4 slot 0x08 | nintendogs:callseq

        const SoundArchivePlayer& m_SoundArchivePlayer; // 0x4
    };

    // Gives the game files instead of the ones of the archive, by label (the class and all names are ours).
    class FilesHook
    {
    public:
        virtual ~FilesHook() {} // slot 0x00

        virtual bool IsTargetItem(const char* pLabel) = 0; // slot 0x08
        virtual void Lock() = 0; // slot 0x0C
        virtual void Unlock() = 0; // slot 0x10
        virtual io::FileStream* OpenFileStream(void* pBuffer, int bufferSize, void* pCacheBuffer, u32 cacheSize,
                                               const char* pLabel, const char* pFileType) = 0; // slot 0x14
        virtual const void* GetFileAddress(const char* pLabel, const char* pItemType, const char* pFileType,
                                           u32 fileIndex) = 0; // slot 0x18

        // (inline)
        bool IsEnabled() const { return m_IsEnabled; }

        bool m_IsEnabled; // 0x4
    };

    // (the struct name is from the reference symbols, the members are ours)
    struct InitializeParam
    {
        const SoundArchive* soundArchive;          // 0x00
        const SoundDataManager* soundDataManager;  // 0x04
        void* buffer;                              // 0x08
        u32 bufferSize;                            // 0x0C
        void* streamBuffer;                        // 0x10 in device memory
        u32 streamBufferSize;                      // 0x14
        void* streamCacheBuffer;                   // 0x18 (NULL: none)
        u32 streamCacheSize;                       // 0x1C
        u32 userParamSizePerSound;                 // 0x20
    };
    ASSERT_SIZE(InitializeParam, 0x24);

    SoundArchivePlayer(); // 0x001323C4 | fefates:bytes [tier B]
    virtual ~SoundArchivePlayer(); // 0x004C393C slot 0x00 | fefates:bytes

    virtual StartResult detail_SetupSound(SoundHandle* pHandle, u32 soundId, bool isHold,
                                          const StartInfo* pStartInfo); // 0x004C1038 slot 0x08 | nintendogs:bytes
    virtual u32 detail_GetItemId(const char* pLabel); // 0x0073EDA4 slot 0x0C

    bool Initialize(const InitializeParam& param); // 0x004C0B04 | fefates:bytes [tier B]
    void Finalize(); // 0x004C3488 | fefates:bytes [tier B]
    void Update(); // 0x004C31C4 | nintendogs:callseq [tier A]

    u32 GetRequiredMemSize(const SoundArchive* pArchive, u32 userParamSizePerSound) const; // 0x0073F124 | fefates:bytes [tier B]
    u32 GetRequiredStreamBufferSize(const SoundArchive* pArchive); // 0x0073F2B8 | nintendogs:bytes [tier A]
    u32 GetRequiredStreamCacheSize(const SoundArchive* pArchive, u32 cacheSizePerSound) const; // 0x0073F288 | fefates:bytes [tier B]

    SoundPlayer& GetSoundPlayer(u32 playerId); // 0x004C0C80 | fefates:bytes [tier B]
    // (inline) the sound archive is set and available
    // (inline; the copy is for sead)
    DECOMP_EMIT_COPY bool IsAvailable() const { return m_pSoundArchive != nullptr && m_pSoundArchive->IsAvailable(); } // 0x0073F104 (name is ours)
    // the callback of the user procedures of the sequences (name is ours)
    void SetSequenceUserprocCallback(SequenceUserprocCallback callback, void* pArg); // 0x00137CB4

    DECOMP_NOINLINE bool SetupMram(const SoundArchive* pArchive, void* pBuffer, u32 bufferSize, u32 userParamSizePerSound); // 0x004C3744 | fefates:bytes [tier B]
    DECOMP_NOINLINE bool SetupSoundPlayer(const SoundArchive* pArchive, void** ppBuffer, const void* pEnd); // 0x004C0E8C | fefates:bytes [tier B]
    bool SetupSequenceSound(int count, void** ppBuffer, const void* pEnd); // 0x004C1068 | fefates:bytes [tier B]
    bool SetupWaveSound(int count, void** ppBuffer, const void* pEnd); // 0x004C0C98 | fefates:bytes [tier B]
    bool SetupUserParamForBasicSound(void** ppBuffer, const void* pEnd, u32 size); // 0x004C30F0 | fefates:bytes [tier B]

    StartResult detail_SetupSoundImpl(SoundHandle* pHandle, u32 soundId, internal::BasicSound::AmbientInfo* pAmbientInfo,
                                      SoundActor* pActor, bool isHold, const StartInfo* pStartInfo); // 0x004C14F8 | nintendogs:callgraph [tier A]
    StartResult PrepareSequenceSoundImpl(internal::SequenceSound* pSound, const SoundArchive::SoundInfo* pSoundInfo,
                                         const SoundArchive::SequenceSoundInfo* pInfo, StartInfo::StartOffsetType startOffsetType,
                                         int startOffset, const StartInfo::SeqSoundInfo* pStartInfo); // 0x004C2B80 | nintendogs:callseq [tier A]
    StartResult PrepareStreamSoundImpl(internal::StreamSound* pSound, const SoundArchive::SoundInfo* pSoundInfo,
                                       const SoundArchive::StreamSoundInfo* pInfo, const SoundArchive::StreamSoundInfo2* pInfo2,
                                       StartInfo::StartOffsetType startOffsetType, int startOffset,
                                       const StartInfo::StreamSoundInfo* pStartInfo); // 0x004C24BC | fefates:bytes [tier B]
    StartResult PrepareWaveSoundImpl(internal::WaveSound* pSound, const SoundArchive::SoundInfo* pSoundInfo,
                                     const SoundArchive::WaveSoundInfo* pInfo, StartInfo::StartOffsetType startOffsetType,
                                     int startOffset, const StartInfo::WaveSoundInfo* pStartInfo); // 0x004C1294 | fefates:bytes [tier B]

    const SoundArchive* m_pSoundArchive;                                            // 0x004
    SequenceNoteOnCallback m_SequenceCallback;                                      // 0x008
    SequenceUserprocCallback m_SequenceUserprocCallback;                            // 0x010
    void* m_pSequenceUserprocCallbackArg;                                           // 0x014
    u32 m_SoundPlayerCount;                                                         // 0x018
    SoundPlayer* m_pSoundPlayers;                                                   // 0x01C
    internal::SoundInstanceManager<internal::SequenceSound> m_SequenceSoundInstanceManager; // 0x020
    internal::LoaderManager<internal::driver::SequenceSoundLoader> m_SequenceSoundLoaderManager; // 0x040
    internal::SoundInstanceManager<internal::WaveSound> m_WaveSoundInstanceManager; // 0x06C
    internal::LoaderManager<internal::driver::WaveSoundLoader> m_WaveSoundLoaderManager; // 0x08C
    internal::SoundInstanceManager<internal::StreamSound> m_StreamSoundInstanceManager; // 0x0B8
    internal::driver::StreamBufferPool m_StreamBufferPool;                          // 0x0D8
    internal::driver::SequenceTrackAllocator* m_pSequenceTrackAllocator;            // 0x0F0
    internal::driver::MmlSequenceTrackAllocator m_MmlSequenceTrackAllocator;        // 0x0F4
    internal::driver::MmlParser m_MmlParser;                                        // 0x100
    void* m_pSetupBuffer;                                                           // 0x104
    u32 m_SetupBufferSize;                                                          // 0x108
    u32 m_SoundUserParamSize;                                                       // 0x10C
    const SoundDataManager* m_pSoundDataManager;                                    // 0x110
    FilesHook* m_pFilesHook;                                                        // 0x114
};
ASSERT_SIZE(SoundArchivePlayer, 0x118);
} // namespace snd
} // namespace nw
