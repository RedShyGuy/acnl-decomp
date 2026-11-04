#pragma once

#include "decomp.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/pia/pia_Types.h"
#include "ssys/st/dList.h"
#include "ssys/st/dListNode.h"

namespace net {
// The requests of the game to send data to a station: three handles, free or waiting; the network
// thread sends the waiting ones (Process) (all names except the classes are ours)
class Sender
{
public:
    // a request: the station and the data to send
    // RTTI N3net6Sender6HandleE @ 0x008D0FF0
    // vtable 0x009046CC (vptr 0x009046D4), offset_to_top 0, 2 entries
    class Handle : public ::ssys::st::ListNode
    {
    public:
        Handle(); // 0x0051B248
        virtual ~Handle(); // 0x0051B2A0 slot 0x00

        // back to no request
        void Clear()
        {
            m_Section.Enter();
            m_StationId = nn::pia::StationId(0xFFFFFFFD, 0xFFFFFFFF);
            m_pData = NULL;
            m_Size = 0;
            m_IsSent = false;
            m_Section.Exit();
        }

        DECOMP_EMIT_COPY bool IsSent() // 0x00748298
        {
            m_Section.Enter();
            bool isSent = m_IsSent;
            m_Section.Exit();
            return isSent;
        }

        nn::os::CriticalSection m_Section; // 0x0C
        nn::pia::StationId m_StationId;    // 0x18
        const void* m_pData;               // 0x20
        u32 m_Size;                        // 0x24
        bool m_IsSent;                     // 0x28
        u32 unk_0x2C;                      // (never used)
    };

    // sends the data of a request to the station
    typedef bool (*SendFunction)(const nn::pia::StationId* stationId, const void* data, u32 size);

    Sender(); // 0x0051B4B0
    ~Sender(); // 0x0051B564

    // a request to send the data to the station (NULL if none is free)
    Handle* Send(const nn::pia::StationId* stationId, const void* data, u32 size); // 0x0051B40C
    // gives back the handle of a sent request
    void Release(Handle* handle); // 0x0051B130
    // drops the requests to the station of the handle and gives back the handle
    void Cancel(Handle* handle); // 0x0051B2B0
    // sends the waiting requests until one fails
    void Process(SendFunction function); // 0x0051B194
    // all handles free
    DECOMP_EMIT_COPY void Reset() // 0x0013C5E8
    {
        m_Section.Enter();
        m_UsedList.Clear();
        m_FreeList.Clear();
        for (s32 i = 0; i < 3; i++) {
            m_Handles[i].Clear();
            m_FreeList.PushBack(&m_Handles[i]);
        }
        m_Section.Exit();
    }

private:
    // the handle goes back to the free ones (the lock is held)
    void Free(Handle* handle)
    {
        handle->Clear();
        m_FreeList.PushBack(handle);
    }

    Handle m_Handles[3];               // 0x00
    ssys::st::List m_FreeList;         // 0x90
    ssys::st::List m_UsedList;         // 0x9C (waiting to be sent)
    nn::os::CriticalSection m_Section; // 0xA8
};
ASSERT_SIZE(Sender::Handle, 0x30);
ASSERT_SIZE(Sender, 0xB4);
} // namespace net
