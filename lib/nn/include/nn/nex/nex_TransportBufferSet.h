#pragma once

#include "decomp.h"
#include "nn/nex/nex_ForcedCriticalSection.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qReservedQueue.h"

namespace nn {
namespace nex {
class Buffer;
class TransportBuffer;

// The buffers of a socket that a transport buffer thread sends or fills: the entries in use (in
// order) and the free ones. The buffers come from the transport buffer (else they are made). The
// class name is from the reference symbols; the names of the members and of the functions without
// a reference name are ours.
class TransportBufferSet
{
public:
    // a buffer and its address
    struct Entry
    {
        // (inline; the buffer is set when the entry is used)
        Entry() {}

        Buffer* m_pBuffer;     // 0x00
        InetAddress m_Address; // 0x04
    };

    static const u32 LOCK_LEVEL = 0x100000;
    // the size of the buffers that are made
    static const u32 DEFAULT_BUFFER_SIZE = 0x554;

    TransportBufferSet(nn::nex::TransportBuffer* pTransportBuffer, int count); // 0x00390AE0 | fefates:callgraph [tier C]
    ~TransportBufferSet(); // 0x00390CBC | fefates:bytes [tier B]
    // the entries in use are released into the queue of the free entries (the name is ours)
    void ReleaseAll(qReservedQueue<Entry*>* pEntries); // 0x003907DC
    // a free entry with a buffer, behind the entries in use; null if there is none
    Entry* Allocate(); // 0x003908D4
    // the entry (taken from the entries in use) is free again with its buffer
    void Release(Entry* pEntry); // 0x00390A24

    // (inline) the first entry in use, which is removed (null if there is none)
    Entry* PopUsed()
    {
        Entry* pEntry = nullptr;
        bool isEmpty;
        if (m_UsedEntries.m_Count != 0) {
            pEntry = m_UsedEntries.PopFront();
            isEmpty = m_UsedEntries.m_Count == 0;
        } else {
            isEmpty = true;
        }
        m_HasData = !isEmpty;
        m_IsFull = m_UsedEntries.m_Count == m_UsedEntries.m_Capacity;
        return pEntry;
    }

    bool m_HasData;                       // 0x00 (an entry is in use)
    bool m_IsFull;                        // 0x01
    qReservedQueue<Entry*> m_UsedEntries; // 0x04
    qReservedQueue<Entry*> m_FreeEntries; // 0x24
    u32 m_Unknown0x44;                    // 0x44
    u32 m_BufferSize;                     // 0x48
    ForcedCriticalSection m_Lock;         // 0x4C
    u32 m_FailureCount;                   // 0x74 (no free entry)
    TransportBuffer* m_pTransportBuffer;  // 0x78
};
ASSERT_OFFSET(TransportBufferSet, m_FreeEntries, 0x24);
ASSERT_OFFSET(TransportBufferSet, m_Lock, 0x4C);
ASSERT_SIZE(TransportBufferSet, 0x7C);
} // namespace nex
} // namespace nn
