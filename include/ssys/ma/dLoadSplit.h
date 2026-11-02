#pragma once

#include "decomp.h"
#include "ssys/ma/dFile.h"
#include "ssys/st/dListNode.h"
#include "sead/seadFixedSafeString.h"

namespace sead {
class Heap;
} // namespace sead

namespace ssys {
namespace ma {
// loads a file into a buffer of a heap in pieces, at once or (async) on the thread of LoadSplitMgr; the node links it
// into the queue of LoadSplitMgr (function names except LoadBuffered/Read and all member names are ours)
// RTTI N4ssys2ma9LoadSplitE @ 0x008D250C
// vtable 0x009073C4 (vptr 0x009073CC), offset_to_top 0, 4 entries
class LoadSplit : public ::ssys::st::ListNode
{
public:
    enum State : u8
    {
        STATE_NONE = 0,
        STATE_REQUESTED = 1,   // waits in the queue
        STATE_LOADING = 2,
        STATE_DONE = 3,
        STATE_ERROR = 4,
        STATE_CANCELED = 5,
        STATE_CANCELING = 6,   // the thread resets it
    };

    DECOMP_EMIT_COPY DECOMP_NOINLINE LoadSplit() // 0x0012D3C8
        : m_pHeap(NULL), m_pData(NULL), m_Size(0), _D8(0), m_ChunkSize(0), m_ReadSize(0), m_Alignment(0),
          m_State(STATE_NONE), m_IsAsync(false), m_IsCancelRequested(false)
    {
    }
    virtual ~LoadSplit(); // 0x0013CA08 slot 0x00
    // when the file is in the buffer
    virtual void OnLoaded();  // 0x0056B854 slot 0x08
    // when the load is reset
    virtual void OnReset();   // 0x0056B858 slot 0x0C

    // starts loading path (or goes on with it); true when it has finished (done, failed or canceled)
    bool Load(const sead::SafeStringBase<char>& path, sead::Heap* heap, u32 alignment, bool async,
              u32 chunkSize); // 0x0056B268
    // at once, in pieces of the default size
    bool LoadBuffered(const sead::SafeStringBase<char>& path, sead::Heap* heap, u32 alignment); // 0x0056B240 | libgarden
    // at once, all of the file in one piece
    bool Load(const sead::SafeStringBase<char>& path, sead::Heap* heap, u32 alignment); // 0x0056B580
    // async
    bool Read(const sead::SafeStringBase<char>& path, sead::Heap* heap, u32 alignment, u32 chunkSize); // 0x0056B484 | libgarden
    bool Read(const sead::SafeStringBase<char>& path, sead::Heap* heap, u32 alignment); // 0x0056B45C
    bool Read(const sead::SafeStringBase<char>& path, sead::Heap* heap); // 0x0056B434

    // asks to stop; true if there is nothing to wait for
    bool RequestCancel(); // 0x0056B4A4
    // stops at once (waits up to a second for the thread), frees the buffer and forgets the file
    DECOMP_EMIT_COPY DECOMP_NOINLINE void Reset(); // 0x00139588
    // reads the next piece
    void Update(); // 0x0056B5C4
    DECOMP_EMIT_COPY bool IsFinished() const // 0x0056B5A0
    {
        return m_State == STATE_DONE || m_State == STATE_ERROR || m_State == STATE_CANCELED;
    }
    DECOMP_EMIT_COPY bool IsSucceeded() const { return m_State == STATE_DONE; } // 0x0056B7E4

    static void SetDefaultChunkSize(u32 size); // 0x0056B570

    // opens the file, allocates the buffer
    void Prepare(); // 0x0056B6D8
    void SetError(); // 0x0056B6B4
    bool OpenFile(); // 0x0056B7F8
    DECOMP_EMIT_COPY DECOMP_NOINLINE void CloseFile(); // 0x00139538

    File m_File;                        // 0x0C
    sead::FixedSafeString<128> m_Path;  // 0x40
    sead::Heap* m_pHeap;                // 0xCC
    u8* m_pData;                      // 0xD0 (name after libgarden)
    u32 m_Size;                         // 0xD4
    u32 _D8;                            // 0xD8
    u32 m_ChunkSize;                    // 0xDC
    u32 m_ReadSize;                     // 0xE0
    u32 m_Alignment;                    // 0xE4
    u8 m_State;                         // 0xE8 State
    bool m_IsAsync;                     // 0xE9
    bool m_IsCancelRequested;           // 0xEA
};
ASSERT_SIZE(LoadSplit, 0xEC);
} // namespace ma
} // namespace ssys

#include "ssys/ma/dLoadSplitMgr.h"

// (inline)
inline void ssys::ma::LoadSplitMgr::RemoveFromQueue(LoadSplit* loadSplit)
{
    s_QueueCriticalSection.lock();
    s_Queue.Erase(loadSplit);
    s_QueueCriticalSection.unlock();
}
