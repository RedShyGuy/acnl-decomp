#include "netgame/dMatchingList.h"

#include "net/dNet.h"
#include "netgame/dMatchingList_SingletonDisposer_.h"
#include "netgame/dGameFunctions.h"
#include "netgame/dNetGameMgr.h"
#include "sead/seadHeap.h"
#include "sead/seadNew.h"
#include <cstring>
#include <new>

namespace netgame {

// 0x00954640
MatchingList* netgame::MatchingList::s_pInstance;
// 0x00954644
MatchingList::SingletonDisposer_* netgame::MatchingList::s_pSingletonDisposer;

DECOMP_ALWAYS_INLINE void netgame::MatchingList::ScanData::Clear()
{
    std::memset(data, 0, BUFFER_SIZE);
    size = 0;
}

DECOMP_ALWAYS_INLINE void netgame::MatchingList::Entry::Clear()
{
    for (s32 i = 0; i < 4; i++) {
        ClearPersonalId(&players[i]);
    }
    sessionId = 0;
}

// 0x006202B8 (name is ours)
void netgame::MatchingList::Update()
{
    if (m_IsSearching) {
        return;
    }
    m_EntryNum = 0;
    for (u32 i = 0; i < m_FoundNum; i++) {
        Entry& entry = m_Entries[m_EntryNum];
        ScanBuf& buf = m_ScanBufs[i];
        ReadSessionPlayers(buf.m_Data.data, entry.players);
        if (!net::Net::s_Instance.IsInternet(false) || (buf.m_Param & 1) == 0 ||
            NetGameMgr::s_pInstance->m_FriendListMgr.HasMeAsBestFriend(&buf.m_OwnerPrincipalId)) {
            entry.sessionId = buf.m_SessionId;
            m_EntryNum++;
        }
    }
    for (u32 i = m_EntryNum; i < ENTRY_MAX; i++) {
        m_Entries[i].Clear();
    }
}

// 0x006203F8 (name is ours)
netgame::MatchingList::Entry::Entry()
{
    Clear();
}

// 0x0062044C (name is ours)
netgame::MatchingList* netgame::MatchingList::createInstance(sead::Heap* heap)
{
    if (s_pInstance == NULL) {
        u8* buffer = new (heap, 4) u8[sizeof(MatchingList)];
        MatchingList* instance = NULL;
        if (buffer == NULL) {
            s_pSingletonDisposer = NULL;
        } else {
            s_pSingletonDisposer = new (buffer) SingletonDisposer_(heap);
            instance = new (buffer) MatchingList();
        }
        s_pInstance = instance;
    }
    return s_pInstance;
}

netgame::MatchingList::MatchingList()
{
    m_EntryNum = 0;
    m_FoundNum = 0;
    m_IsSearching = false;
    m_IsRequested = false;
    for (u32 i = 0; i < ENTRY_MAX; i++) {
        m_Entries[i].Clear();
    }
    for (u32 i = 0; i < ENTRY_MAX; i++) {
        m_pScanBufs[i] = &m_ScanBufs[i];
    }
}

// 0x006205B0 (name is ours)
void netgame::MatchingList::deleteInstance()
{
    SingletonDisposer_* disposer = s_pSingletonDisposer;
    if (disposer == NULL) {
        return;
    }
    s_pSingletonDisposer = NULL;
    disposer->~SingletonDisposer_();
    if (s_pInstance != NULL) {
        s_pInstance->~MatchingList();
        ::operator delete(s_pInstance);
    }
    s_pInstance = NULL;
}

// 0x00620618 (name is ours)
void netgame::MatchingList::RequestSearch()
{
    if (net::Net::s_Instance.IsLocal()) {
        m_IsRequested = true;
    }
}

// 0x00620714 (name is ours)
void netgame::MatchingList::Reset()
{
    for (u32 i = 0; i < ENTRY_MAX; i++) {
        m_ScanBufs[i].m_Data.Clear();
    }
    m_FoundNum = 0;
    m_IsRequested = false;
}

// 0x00620770 (name is ours)
void netgame::MatchingList::Search()
{
    m_IsSearching = true;
    if (net::Net::s_Instance.SearchSessions(m_pScanBufs, ENTRY_MAX, &m_FoundNum, 1, 0)) {
        m_IsSearching = false;
        m_IsRequested = false;
    }
}

// 0x006207D8 slot 0x08 (name is ours)
void* netgame::MatchingList::ScanBuf::GetBuffer()
{
    return m_Data.data;
}

// 0x006207E0 (name is ours)
netgame::MatchingList::ScanBuf::ScanBuf()
{
    m_OwnerPrincipalId = 0;
    m_SessionId = 0;
    m_Param = 0;
    m_ParticipantNum = 0;
    m_Data.Clear();
}

// 0x0062083C slot 0x00 (name is ours)
// 0x00620838 (deleting dtor)
netgame::MatchingList::ScanBuf::~ScanBuf()
{
    // empty (in the original too)
}

// 0x0075F268 (name is ours)
SvPersonalId* netgame::MatchingList::GetEntryPlayers(u32 index)
{
    return m_Entries[index].players;
}

// 0x0075F27C (name is ours)
netgame::MatchingList::Entry* netgame::MatchingList::GetEntry(u32 index)
{
    return &m_Entries[index];
}

// 0x0075F290 slot 0x0C (name is ours)
u32 netgame::MatchingList::ScanBuf::GetBufferSize()
{
    return BUFFER_SIZE;
}
} // namespace netgame
