#pragma once

#include "decomp.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/session/session_ISessionInfoList.h"

namespace nn {
namespace pia {
namespace session {
// The ISessionInfoList of a network: capacity infos of type T, made by the constructor; m_Size of
// them hold the sessions found. The member names are ours.
//
// Instantiations found in the binary:
//   nn::pia::session::SessionInfoList<nn::pia::inet::NexSessionInfo>  typeinfo 0x008CFF9C  vtable 0x009017F8
//   nn::pia::session::SessionInfoList<nn::pia::local::UdsSessionInfo>  typeinfo 0x008CFFA8  vtable 0x00901824
template <typename T>
class SessionInfoList : public ISessionInfoList
{
public:
    // (inline in the NetworkFactory of the network)
    explicit SessionInfoList(u32 capacity) : m_Size(0), m_Capacity(capacity)
    {
        m_ppInfos = common::NewArray<T*>(capacity);
        for (u32 i = 0; i < m_Capacity; i++) {
            m_ppInfos[i] = new T();
        }
    }
    virtual ~SessionInfoList()
    {
        for (u32 i = 0; i < m_Capacity; i++) {
            if (m_ppInfos[i] != nullptr) {
                delete m_ppInfos[i];
            }
        }
        if (m_ppInfos != nullptr) {
            common::DeleteArray(m_ppInfos);
        }
    }
    virtual ISessionInfo** Begin() const { return reinterpret_cast<ISessionInfo**>(m_ppInfos); }
    virtual ISessionInfo** Begin() { return reinterpret_cast<ISessionInfo**>(m_ppInfos); }
    virtual ISessionInfo** End() const { return reinterpret_cast<ISessionInfo**>(m_ppInfos + m_Size); }
    virtual ISessionInfo** End() { return reinterpret_cast<ISessionInfo**>(m_ppInfos + m_Size); }
    virtual u32 GetSize() const { return m_Size; }
    virtual u32 GetCapacity() const { return m_Capacity; }
    virtual void Clear()
    {
        m_Size = 0;
        if (common::IsValidPointer(m_ppInfos)) {
            for (u32 i = 0; i < m_Capacity; i++) {
                m_ppInfos[i]->Clear();
            }
        }
    }

    // the next info, cleared; null if the list is full (inline in inet::NexMatchmakeSession;
    // name is ours)
    T* Add()
    {
        if (m_Capacity == 0 || m_Capacity <= m_Size) {
            return nullptr;
        }
        T* pInfo = m_ppInfos[m_Size];
        pInfo->Clear();
        m_Size++;
        return pInfo;
    }

    u32 m_Size;     // 0x4
    u32 m_Capacity; // 0x8
    T** m_ppInfos;  // 0xC
};
} // namespace session
} // namespace pia
} // namespace nn
