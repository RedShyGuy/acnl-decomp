#include "netgame/dNetGameMgr.h"

#include "sead/seadHeap.h"
#include <cstring>

namespace netgame {
// 0x00624714 (name is ours)
void netgame::NetGameMgr::SendMgr::Reset(PlayerNo no)
{
    if (no >= PLAYER_NO_MAX) {
        return;
    }
    m_Sizes[no] = 0;
    sead::CriticalSection& section = NetGameMgr::s_pInstance->m_CriticalSection;
    section.lock();
    m_Pending[no] = 0;
    section.unlock();
}

// 0x00624764 (name is ours)
void netgame::NetGameMgr::SendMgr::AddPending(PlayerNo no, s32 delta)
{
    if (no >= PLAYER_NO_MAX) {
        return;
    }
    u8 amount = delta >= 0 ? delta : -delta;
    sead::CriticalSection& section = NetGameMgr::s_pInstance->m_CriticalSection;
    section.lock();
    if (delta != 0) {
        if (delta < 0) {
            m_Pending[no] -= amount;
        } else {
            m_Pending[no] += amount;
        }
    }
    section.unlock();
}

// 0x006247D4 (name is ours)
bool netgame::NetGameMgr::SendMgr::Commit(PlayerNo no, u32 size)
{
    if (no >= PLAYER_NO_MAX) {
        return false;
    }
    u32 total = m_Sizes[no] + size;
    if (total > BUFFER_SIZE) {
        return false;
    }
    m_Sizes[no] = total;
    return true;
}

// 0x00624808 (name is ours)
void netgame::NetGameMgr::SendMgr::Prepare(PlayerNo no)
{
    if (no < PLAYER_NO_MAX) {
        m_Sizes[no] = 0;
    }
}

// 0x00624820 (name is ours)
void netgame::NetGameMgr::SendMgr::AssignBuffers(PlayerNo myNo)
{
    if (m_IsAssigned || myNo >= PLAYER_NO_MAX || m_pBuffer == NULL) {
        return;
    }
    u8* buffer = m_pBuffer;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        if (i != myNo) {
            m_pBuffers[i] = buffer;
            buffer += BUFFER_SIZE;
        } else {
            m_pBuffers[i] = NULL;
        }
    }
    m_IsAssigned = true;
}

// 0x006248A4 (name is ours)
void netgame::NetGameMgr::SendMgr::ResetAll()
{
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        m_Sizes[i] = 0;
    }
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        m_pBuffers[i] = NULL;
    }
    m_IsAssigned = false;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        m_Pending[i] = 0;
    }
}

// 0x006248D4 (name is ours)
void netgame::NetGameMgr::SendMgr::AllocBuffers(sead::Heap* heap)
{
    if (heap != NULL && m_pBuffer == NULL) {
        m_pBuffer = static_cast<u8*>(heap->tryAlloc(BUFFER_SIZE * 3, 4));
    }
}

// 0x00624914 (name is ours)
netgame::NetGameMgr::SendMgr::SendMgr()
{
    m_pBuffer = NULL;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        m_Sizes[i] = 0;
    }
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        m_pBuffers[i] = NULL;
    }
    m_IsAssigned = false;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        m_Pending[i] = 0;
    }
}

// 0x0075F7A8 (name is ours)
u8* netgame::NetGameMgr::SendMgr::GetBuffer(PlayerNo no)
{
    if (no >= PLAYER_NO_MAX) {
        return NULL;
    }
    return m_pBuffers[no] + m_Sizes[no];
}

// 0x0075F7C8 (name is ours)
bool netgame::NetGameMgr::SendMgr::IsBusy(PlayerNo no) const
{
    if (no >= PLAYER_NO_MAX) {
        return false;
    }
    sead::CriticalSection& section = NetGameMgr::s_pInstance->m_CriticalSection;
    section.lock();
    u8 pending = m_Pending[no];
    section.unlock();
    return pending != 0;
}

// 0x0062A2E0 (name is ours)
void netgame::NetGameMgr::TransitionMgr::Reset()
{
    m_BufferSize = 0;
    m_EnteringPlayer = PLAYER_NO_INVALID;
    m_ExitingPlayer = PLAYER_NO_INVALID;
    m_State = 3;
}

// 0x0062A300 (name is ours)
void netgame::NetGameMgr::TransitionMgr::AllocBuffer(sead::Heap* heap)
{
    if (heap == NULL) {
        return;
    }
    if (m_pBuffer == NULL) {
        m_pBuffer = static_cast<u8*>(heap->tryAlloc(BUFFER_SIZE, 4));
    }
    m_BufferSize = 0;
}

// 0x0062A344 (name is ours)
void netgame::NetGameMgr::TransitionMgr::Add(const void* data, u32 size)
{
    if (data == NULL || m_pBuffer == NULL) {
        return;
    }
    u32 total = m_BufferSize + size;
    if (total <= BUFFER_SIZE) {
        std::memcpy(m_pBuffer + m_BufferSize, data, size);
        m_BufferSize = total;
    }
}

// 0x0062A380 (name is ours)
netgame::NetGameMgr::TransitionMgr::TransitionMgr()
{
    m_pBuffer = NULL;
    m_BufferSize = 0;
    m_EnteringPlayer = PLAYER_NO_INVALID;
    m_ExitingPlayer = PLAYER_NO_INVALID;
    m_State = 3;
}
} // namespace netgame
