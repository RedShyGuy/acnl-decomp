#pragma once

#include "Sv/dSvPersonalId.h"
#include "decomp.h"
#include "net/dScanBufBase.h"
#include "netgame/netgame_Api.h"

namespace sead {
class Heap;
} // namespace sead

namespace netgame {
// The sessions that a search found (class names from the RTTI, the others are ours)
class MatchingList
{
public:
    class SingletonDisposer_;

    static const u32 ENTRY_MAX = 100;
    static const u32 BUFFER_SIZE = 512;

    // the application data of a session
    struct ScanData
    {
        ScanData() : size(0)
        {
            Clear();
        }
        void Clear();

        u8 data[BUFFER_SIZE];
        u32 size;
    };

    // RTTI N7netgame12MatchingList7ScanBufE @ 0x008D3F44
    // vtable 0x0090BD24 (vptr 0x0090BD2C), offset_to_top 0, 4 entries
    class ScanBuf : public ::net::ScanBufBase
    {
    public:
        DECOMP_NOINLINE ScanBuf(); // 0x006207E0
        virtual ~ScanBuf(); // 0x0062083C slot 0x00
        virtual void* GetBuffer(); // 0x006207D8 slot 0x08
        virtual u32 GetBufferSize(); // 0x0075F290 slot 0x0C

        ScanData m_Data; // 0x10
    };
    ASSERT_SIZE(ScanBuf, 0x214);

    // a session with the players in it
    struct Entry
    {
        DECOMP_NOINLINE Entry(); // 0x006203F8
        void Clear();

        u32 sessionId;              // 0x0
        SvPersonalId players[4];    // 0x4
    };
    ASSERT_SIZE(Entry, 0xBC);

    static MatchingList* createInstance(sead::Heap* heap); // 0x0062044C
    static void deleteInstance(); // 0x006205B0

    DECOMP_ALWAYS_INLINE MatchingList();
    // (only the members are destroyed)
    ~MatchingList() {}

    void Update(); // 0x006202B8
    void RequestSearch(); // 0x00620618
    void Reset(); // 0x00620714
    void Search(); // 0x00620770
    Entry* GetEntry(u32 index); // 0x0075F27C
    SvPersonalId* GetEntryPlayers(u32 index); // 0x0075F268
    u32 GetEntryNum() const; // 0x0075F258

    static MatchingList* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 m_SingletonDisposerBuffer[4]; // 0x00000
    Entry m_Entries[ENTRY_MAX];       // 0x00010
    ScanBuf m_ScanBufs[ENTRY_MAX];    // 0x04980
    ::net::ScanBufBase* m_pScanBufs[ENTRY_MAX]; // 0x11950
    u32 m_EntryNum;                   // 0x11AE0
    u32 m_FoundNum;                   // 0x11AE4
    bool m_IsSearching;               // 0x11AE8
    bool m_IsRequested;               // 0x11AE9
};
ASSERT_SIZE(MatchingList, 0x11AEC);
} // namespace netgame
