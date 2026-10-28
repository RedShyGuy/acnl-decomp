#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_Task.h"
#include "nw/snd/snd_Global.h"

namespace nw {
namespace snd {
class SoundArchive;
class SoundDataManager;
class SoundPlayer;

namespace internal {
class PlayerHeap;

namespace driver {
// Loads the data of a wave sound that is not loaded into a player heap: the wave sound file and the wave
// of the note (the class and function names are from the reference symbols, the members are ours).
// The loaders live in a LoaderManager of the sound archive player.
class WaveSoundLoader
{
public:
    // what the sound gives the loader (names are ours)
    struct Arg
    {
        // (inline)
        Arg() : soundArchive(nullptr), soundDataManager(nullptr), soundPlayer(nullptr), index(0) {}

        const SoundArchive* soundArchive;         // 0x00
        const SoundDataManager* soundDataManager; // 0x04 (NULL: none)
        SoundPlayer* soundPlayer;                 // 0x08
        LoadItemInfo loadInfoWsd;                 // 0x0C the wave sound file
        int index;                                // 0x14 the wave sound in the file
    };
    ASSERT_SIZE(Arg, 0x18);
    // what the sound archive player gives the sound (names are ours)
    struct LoadInfo
    {
        const SoundArchive* soundArchive;         // 0x0
        const SoundDataManager* soundDataManager; // 0x4
        LoadItemInfo* loadInfoWsd;                // 0x8
        SoundPlayer* soundPlayer;                 // 0xC
    };
    ASSERT_SIZE(LoadInfo, 0x10);

    // The task of the task thread that loads the data (the members are ours).
    // RTTI N2nw3snd8internal6driver15WaveSoundLoader12DataLoadTaskE @ 0x008D0A84
    // vtable 0x00903334 (vptr 0x0090333C), offset_to_top 0, 3 entries
    class DataLoadTask : public ::nw::snd::internal::Task
    {
    public:
        // (inline)
        DataLoadTask() : m_pWaveSoundFile(nullptr), m_pWaveFile(nullptr) {}
        virtual ~DataLoadTask(); // 0x004C9BB8 slot 0x00

        virtual void Execute(); // 0x004CD88C slot 0x08 | fefates:bytes

        Arg m_Arg;                      // 0x1C
        const void* m_pWaveSoundFile;   // 0x34 the loaded wave sound file
        const void* m_pWaveFile;        // 0x38 the loaded wave of the note
        PlayerHeap* m_pPlayerHeap;      // 0x3C
        bool m_IsSucceeded;             // 0x40
    };
    ASSERT_SIZE(DataLoadTask, 0x44);

    // the offset of m_Link (LoaderManager)
    static const long LINK_OFFSET = 0x44;

    // (inline; name is ours) a new load
    void Initialize(const Arg& arg)
    {
        m_Task.m_Status = Task::STATUS_FREE;
        m_Task.m_pWaveFile = nullptr;
        m_Task.m_pWaveSoundFile = nullptr;
        m_Task.m_IsSucceeded = false;
        m_Task.m_Arg = arg;
        m_Task.m_pPlayerHeap = nullptr;
    }
    // whether the task still runs
    bool IsInUse(); // 0x004CD9B8 | fefates:callgraph [tier C]
    // the player heap goes back to the sound player
    void Finalize(); // 0x004CD9D4 | fefates:bytes [tier B]

    DataLoadTask m_Task;      // 0x00
    ut::LinkListNode m_Link;  // 0x44 (LoaderManager)
};
ASSERT_SIZE(WaveSoundLoader, 0x4C);
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
