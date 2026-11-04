#include "net/dSender.h"

namespace net {
// 0x0051B130 (name is ours)
void net::Sender::Release(Handle* handle)
{
    m_Section.Enter();
    Free(handle);
    m_Section.Exit();
}

// 0x0051B194 (name is ours)
void net::Sender::Process(SendFunction function)
{
    m_Section.Enter();
    Handle* handle = static_cast<Handle*>(m_UsedList.GetFirst());
    m_Section.Exit();
    while (handle != NULL) {
        bool isSent = function(&handle->m_StationId, handle->m_pData, handle->m_Size);
        m_Section.Enter();
        if (!isSent) {
            handle = static_cast<Handle*>(handle->GetNext());
        } else {
            if (m_UsedList.Contains(handle)) {
                m_UsedList.Erase(handle);
                handle->m_Section.Enter();
                handle->m_IsSent = true;
                handle->m_Section.Exit();
            }
            handle = static_cast<Handle*>(m_UsedList.GetFirst());
        }
        m_Section.Exit();
    }
}

// 0x0051B2B0 (name is ours)
void net::Sender::Cancel(Handle* handle)
{
    if (handle == NULL) {
        return;
    }
    m_Section.Enter();
    if (handle->m_StationId != nn::pia::StationId(0xFFFFFFFD, 0xFFFFFFFF)) {
        m_Section.Enter();
        Handle* it = static_cast<Handle*>(m_UsedList.GetFirst());
        while (it != NULL) {
            if (it->m_StationId != handle->m_StationId) {
                it = static_cast<Handle*>(it->GetNext());
            } else {
                m_UsedList.Erase(it);
                Free(it);
                it = static_cast<Handle*>(m_UsedList.GetFirst());
            }
        }
        m_Section.Exit();
    }
    handle->m_Section.Enter();
    bool isSent = handle->m_IsSent;
    handle->m_Section.Exit();
    if (isSent) {
        Release(handle);
    }
    m_Section.Exit();
}

// 0x0051B40C (name is ours)
net::Sender::Handle* net::Sender::Send(const nn::pia::StationId* stationId, const void* data, u32 size)
{
    if (*stationId == nn::pia::StationId(0xFFFFFFFD, 0xFFFFFFFF) || data == NULL || size == 0) {
        return NULL;
    }
    m_Section.Enter();
    Handle* handle = static_cast<Handle*>(m_FreeList.GetFirst());
    m_Section.Exit();
    if (handle == NULL) {
        return NULL;
    }
    handle->m_StationId = *stationId;
    handle->m_pData = data;
    handle->m_Size = size;
    m_Section.Enter();
    m_FreeList.Erase(handle);
    m_UsedList.PushBack(handle);
    m_Section.Exit();
    return handle;
}

// 0x0051B4B0 (name is ours)
net::Sender::Sender() : m_Section(nn::os::CriticalSection::InitializeTag())
{
    for (s32 i = 0; i < 3; i++) {
        m_FreeList.PushBack(&m_Handles[i]);
    }
}

// 0x0051B564 (name is ours)
net::Sender::~Sender()
{
    // (only the members are destroyed)
}
} // namespace net
