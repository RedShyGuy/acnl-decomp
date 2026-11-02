#include "ssys/ma/dLoadSplit.h"
#include "ssys/ma/dFlushCache.h"
#include "ssys/ma/dLoadSplitMgr.h"
#include "sead/seadHeap.h"
#include "sead/seadThread.h"
#include "sead/seadTickSpan.h"

namespace ssys {
namespace ma {
// 0x00139538 (name is ours)
void ssys::ma::LoadSplit::CloseFile()
{
    if (!m_File.IsOpen()) {
        return;
    }
    m_File.Close();
    LoadSplitMgr::s_OpenFileCriticalSection.lock();
    LoadSplitMgr::s_OpenFileCount--;
    LoadSplitMgr::s_OpenFileCriticalSection.unlock();
}

// 0x00139588 (name is ours)
void ssys::ma::LoadSplit::Reset()
{
    if (m_State == STATE_NONE) {
        return;
    }
    if (m_State == STATE_REQUESTED && m_IsAsync) {
        LoadSplitMgr::s_QueueCriticalSection.lock();
        if (m_State == STATE_REQUESTED) {
            LoadSplitMgr::RemoveFromQueue(this);
        }
        LoadSplitMgr::s_QueueCriticalSection.unlock();
    }
    if (m_State == STATE_LOADING && m_IsAsync) {
        m_IsCancelRequested = true;
        sead::TickSpan span = sead::TickSpan::fromMilliSeconds(1);
        for (s32 i = 1000;;) {
            sead::Thread::sleep(span);
            if (--i <= 0 || IsFinished()) {
                break;
            }
        }
    }
    if (m_State != STATE_REQUESTED && m_IsAsync) {
        LoadSplitMgr::s_QueueCriticalSection.lock();
        if (LoadSplitMgr::s_Queue.Contains(this)) {
            LoadSplitMgr::RemoveFromQueue(this);
        }
        LoadSplitMgr::s_QueueCriticalSection.unlock();
    }
    if (m_File.IsOpen()) {
        CloseFile();
    }
    OnReset();
    if (m_pData != NULL) {
        m_pHeap->free(m_pData);
    }
    m_pHeap = NULL;
    m_pData = NULL;
    m_Size = 0;
    _D8 = 0;
    m_ChunkSize = 0;
    m_ReadSize = 0;
    m_Alignment = 0;
    m_State = STATE_NONE;
    m_IsAsync = false;
    m_IsCancelRequested = false;
}

// 0x0013CA08 slot 0x00
// 0x0056B85C (deleting dtor)
ssys::ma::LoadSplit::~LoadSplit()
{
    Reset();
}

// 0x0056B240 | libgarden
bool ssys::ma::LoadSplit::LoadBuffered(const sead::SafeStringBase<char>& path, sead::Heap* heap, u32 alignment)
{
    return Load(path, heap, alignment, false, LoadSplitMgr::s_DefaultChunkSize);
}

// 0x0056B268 (name is ours)
bool ssys::ma::LoadSplit::Load(const sead::SafeStringBase<char>& path, sead::Heap* heap, u32 alignment, bool async,
                               u32 chunkSize)
{
    if (IsFinished()) {
        return true;
    }
    if (m_State == STATE_NONE) {
        m_IsAsync = async;
        m_ChunkSize = chunkSize;
        m_State = STATE_REQUESTED;
        m_pHeap = heap;
        m_Path.copy(path);
        m_Alignment = alignment;
        if (!m_IsAsync) {
            m_State = STATE_LOADING;
        } else {
            LoadSplitMgr::s_QueueCriticalSection.lock();
            LoadSplitMgr::s_Queue.PushBack(this);
            LoadSplitMgr::s_QueueCriticalSection.unlock();
        }
    }
    if (m_IsAsync) {
        LoadSplitMgr::Wake();
    } else {
        Update();
    }
    return IsFinished();
}

// 0x0056B434 (name is ours)
bool ssys::ma::LoadSplit::Read(const sead::SafeStringBase<char>& path, sead::Heap* heap)
{
    return Load(path, heap, LoadSplitMgr::s_DefaultAlignment, true, LoadSplitMgr::s_DefaultChunkSize);
}

// 0x0056B45C (name is ours)
bool ssys::ma::LoadSplit::Read(const sead::SafeStringBase<char>& path, sead::Heap* heap, u32 alignment)
{
    return Load(path, heap, alignment, true, LoadSplitMgr::s_DefaultChunkSize);
}

// 0x0056B484 | libgarden
bool ssys::ma::LoadSplit::Read(const sead::SafeStringBase<char>& path, sead::Heap* heap, u32 alignment, u32 chunkSize)
{
    return Load(path, heap, alignment, true, chunkSize);
}

// 0x0056B4A4 (name is ours)
bool ssys::ma::LoadSplit::RequestCancel()
{
    u8 state = m_State;
    if (m_State == STATE_NONE) {
        return true;
    }
    if (state == STATE_CANCELING) {
        return false;
    }
    if (m_State == STATE_REQUESTED || m_State == STATE_DONE || m_State == STATE_ERROR || m_State == STATE_CANCELED) {
        m_State = STATE_CANCELING;
        if (!LoadSplitMgr::s_Queue.Contains(this)) {
            LoadSplitMgr::s_QueueCriticalSection.lock();
            LoadSplitMgr::s_Queue.PushBack(this);
            LoadSplitMgr::s_QueueCriticalSection.unlock();
        }
        return false;
    }
    if (m_State == STATE_LOADING && m_IsAsync) {
        m_IsCancelRequested = true;
        return false;
    }
    return true;
}

// 0x0056B570 (name is ours)
void ssys::ma::LoadSplit::SetDefaultChunkSize(u32 size)
{
    LoadSplitMgr::s_DefaultChunkSize = size;
}

// 0x0056B580 (name is ours)
bool ssys::ma::LoadSplit::Load(const sead::SafeStringBase<char>& path, sead::Heap* heap, u32 alignment)
{
    return Load(path, heap, alignment, false, 0);
}

// 0x0056B5C4 (name is ours)
void ssys::ma::LoadSplit::Update()
{
    if (m_IsCancelRequested || m_State != STATE_LOADING) {
        return;
    }
    if (m_pData == NULL) {
        Prepare();
        if (m_IsCancelRequested || m_pData == NULL) {
            return;
        }
    }
    u32 size = m_Size - m_ReadSize;
    if (m_ChunkSize < size) {
        size = m_ChunkSize;
    }
    u32 readSize = m_File.Read(m_pData + m_ReadSize, size);
    if (readSize != size) {
        SetError();
        return;
    }
    m_ReadSize += readSize;
    if (m_IsCancelRequested || m_Size > m_ReadSize) {
        return;
    }
    if (m_File.IsOpen()) {
        CloseFile();
    }
    FlushCache::Add(reinterpret_cast<uptr>(m_pData), m_Size);
    OnLoaded();
    m_State = STATE_DONE;
}

// 0x0056B6B4 (name is ours)
void ssys::ma::LoadSplit::SetError()
{
    if (m_File.IsOpen()) {
        CloseFile();
    }
    m_State = STATE_ERROR;
}

// 0x0056B6D8 (name is ours)
void ssys::ma::LoadSplit::Prepare()
{
    if (IsFinished() || m_IsCancelRequested) {
        return;
    }
    if (!((m_State == STATE_REQUESTED || m_State == STATE_LOADING) && m_pData == NULL)) {
        return;
    }
    if (m_Size == 0) {
        if (!m_File.IsOpen() && (m_State == STATE_LOADING || LoadSplitMgr::s_OpenFileCount < 8)) {
            if (!OpenFile()) {
                SetError();
                return;
            }
        }
        if (m_File.IsOpen()) {
            m_Size = m_File.GetSize();
            if (m_Size == 0) {
                SetError();
                return;
            }
            if (m_ChunkSize == 0) {
                m_ChunkSize = m_Size;
            }
        }
    }
    if (m_Size != 0) {
        m_pData = static_cast<u8*>(m_pHeap->tryAlloc(m_Size, m_Alignment));
        if (m_pData == NULL) {
            SetError();
        }
    }
}

// 0x0056B7F8 (name is ours)
bool ssys::ma::LoadSplit::OpenFile()
{
    if (m_File.IsOpen()) {
        return false;
    }
    if (!m_File.Open(m_Path)) {
        return false;
    }
    LoadSplitMgr::s_OpenFileCriticalSection.lock();
    LoadSplitMgr::s_OpenFileCount++;
    LoadSplitMgr::s_OpenFileCriticalSection.unlock();
    return true;
}

// 0x0056B854 slot 0x08
void ssys::ma::LoadSplit::OnLoaded()
{
    // empty (in the original too)
}

// 0x0056B858 slot 0x0C
void ssys::ma::LoadSplit::OnReset()
{
    // empty (in the original too)
}

} // namespace ma
} // namespace ssys
