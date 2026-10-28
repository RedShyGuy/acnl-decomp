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
// Loads the data of a sequence sound that is not loaded into a player heap: the sequence file, its banks
// and their wave archives (the class and function names are from the reference symbols, the members are
// ours). The loaders live in a LoaderManager of the sound archive player.
class SequenceSoundLoader
{
public:
    static const int BANK_COUNT = 4; // (name is ours)

    // what the sound gives the loader (names are ours)
    struct Arg
    {
        // (inline)
        Arg() : soundArchive(nullptr), soundDataManager(nullptr), soundPlayer(nullptr) {}

        const SoundArchive* soundArchive;          // 0x00
        const SoundDataManager* soundDataManager;  // 0x04 (NULL: none)
        SoundPlayer* soundPlayer;                  // 0x08
        LoadItemInfo loadInfoSeq;                  // 0x0C the sequence file
        LoadItemInfo loadInfoBanks[BANK_COUNT];    // 0x14
    };
    ASSERT_SIZE(Arg, 0x34);

    // what the sound archive player gives the sound (names are ours)
    class LoadInfo
    {
    public:
        LoadInfo(const SoundArchive* pSoundArchive, const SoundDataManager* pSoundDataManager, LoadItemInfo* pLoadInfoSeq,
                 LoadItemInfo* pLoadInfoBanks, SoundPlayer* pSoundPlayer); // 0x004D2694 | fefates:bytes [tier B]

        const SoundArchive* soundArchive;          // 0x00
        const SoundDataManager* soundDataManager;  // 0x04
        LoadItemInfo* loadInfoSeq;                 // 0x08
        LoadItemInfo* loadInfoBanks[BANK_COUNT];   // 0x0C
        SoundPlayer* soundPlayer;                  // 0x1C
    };

    // what the loaded data is (names are ours)
    struct Result
    {
        const void* seqFile;                       // 0x00
        const void* bankFiles[BANK_COUNT];         // 0x04
        const void* warcFiles[BANK_COUNT];         // 0x14 the wave archive of the first wave of each bank
        bool warcIsIndividual[BANK_COUNT];         // 0x24
    };
    ASSERT_SIZE(Result, 0x28);

    // The task of the task thread that loads the data (the members are ours).
    // RTTI N2nw3snd8internal6driver19SequenceSoundLoader12DataLoadTaskE @ 0x008D0B04
    // vtable 0x00903454 (vptr 0x0090345C), offset_to_top 0, 3 entries
    class DataLoadTask : public ::nw::snd::internal::Task
    {
    public:
        // (inline)
        DataLoadTask()
        {
            m_Result.seqFile = nullptr;
            for (int i = 0; i < BANK_COUNT; ++i) {
                m_Result.bankFiles[i] = nullptr;
                m_Result.warcFiles[i] = nullptr;
                m_Result.warcIsIndividual[i] = false;
            }
        }
        virtual ~DataLoadTask(); // 0x004D265C slot 0x00

        virtual void Execute(); // 0x004D240C slot 0x08 | fefates:bytes

        Arg m_Arg;                  // 0x1C
        Result m_Result;            // 0x50
        PlayerHeap* m_pPlayerHeap;  // 0x78
        bool m_IsSucceeded;         // 0x7C
    };
    ASSERT_SIZE(DataLoadTask, 0x80);

    // the offset of m_Link (LoaderManager)
    static const long LINK_OFFSET = 0x80;

    // (inline; name is ours) a new load
    void Initialize(const Arg& arg)
    {
        m_Task.m_Status = Task::STATUS_FREE;
        m_Task.m_Result.seqFile = nullptr;
        for (int i = 0; i < BANK_COUNT; ++i) {
            m_Task.m_Result.bankFiles[i] = nullptr;
            m_Task.m_Result.warcFiles[i] = nullptr;
            m_Task.m_Result.warcIsIndividual[i] = false;
        }
        m_Task.m_IsSucceeded = false;
        m_Task.m_Arg = arg;
        m_Task.m_pPlayerHeap = nullptr;
    }
    // whether the task still runs
    bool IsInUse(); // 0x004D2660 | fefates:callgraph [tier C]
    // the player heap goes back to the sound player
    void Finalize(); // 0x004D267C | fefates:bytes [tier B]

    DataLoadTask m_Task;      // 0x00
    ut::LinkListNode m_Link;  // 0x80 (LoaderManager)
};
ASSERT_SIZE(SequenceSoundLoader, 0x88);
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
