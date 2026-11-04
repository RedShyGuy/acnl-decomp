#include "netgame/dNetGameMgr.h"

#include "sead/seadHeap.h"

namespace netgame {
// 0x006259EC (name is ours)
void netgame::NetGameMgr::Receiver::Clear(PlayerNo no)
{
    if (no < PLAYER_NO_MAX) {
        m_IsEnabled[no] = false;
        m_WasHandled[no] = false;
    }
}

// 0x00625A08 (name is ours)
void netgame::NetGameMgr::Receiver::AssignBuffers(PlayerNo myNo)
{
    if (m_IsAssigned || myNo >= PLAYER_NO_MAX || m_pBuffer == NULL) {
        return;
    }
    u8* buffer = m_pBuffer;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        if (myNo == i) {
            m_pBuffers[i] = NULL;
        } else {
            m_pBuffers[i] = buffer;
            buffer += RECEIVE_BUFFER_SIZE;
        }
    }
    m_IsAssigned = true;
}

// 0x00625A8C | libgarden
void netgame::NetGameMgr::Receiver::SetWasHandled(u8 no, bool set)
{
    if (no < PLAYER_NO_MAX) {
        m_WasHandled[no] = set;
    }
}

// 0x00625A9C | libgarden
void netgame::NetGameMgr::Receiver::SetIsEnabled(u8 no, bool set)
{
    if (no < PLAYER_NO_MAX) {
        m_IsEnabled[no] = set;
    }
}

// 0x00625AAC (name is ours)
void netgame::NetGameMgr::Receiver::Reset()
{
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        m_pBuffers[i] = NULL;
    }
    m_IsAssigned = false;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        SetIsEnabled(i, false);
        SetWasHandled(i, false);
    }
}

// 0x00625AF8 (name is ours)
void netgame::NetGameMgr::Receiver::AllocBuffer(sead::Heap* heap)
{
    if (heap != NULL && m_pBuffer == NULL) {
        m_pBuffer = static_cast<u8*>(heap->tryAlloc(RECEIVE_BUFFER_SIZE * (PLAYER_NO_MAX - 1), 4));
    }
}

// 0x00625B38 (name is ours)
netgame::NetGameMgr::Receiver::Receiver()
{
    m_pBuffer = NULL;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        m_pBuffers[i] = NULL;
    }
    m_IsAssigned = false;
    for (s32 i = 0; i < PLAYER_NO_MAX; i++) {
        SetIsEnabled(i, false);
    }
}
} // namespace netgame
