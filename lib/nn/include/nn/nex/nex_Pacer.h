#pragma once

#include "decomp.h"
#include "nn/nex/nex_Chrono.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
class Event;
class EventHandler;

// Keeps a loop at a number of frames per second: Pace waits for the rest of the frame (on an
// event nobody signals) and keeps statistics of the work time. The names of the members and of the
// functions are ours.
// RTTI N2nn3nex5PacerE @ 0x008CF538
// vtable 0x008FF9D8 (vptr 0x008FF9E0), offset_to_top 0, 3 entries
class Pacer : public ::nn::nex::RootObject
{
public:
    // fps: the frames per second (0: no wait)
    explicit Pacer(float fps); // 0x003CEBDC
    virtual ~Pacer(); // 0x00136C58 slot 0x00
    // 0x003CECCC slot 0x04 (deleting dtor)
    // the end of a frame: waits for the rest of its time and starts the next one
    virtual void Pace(); // 0x003CEA40 slot 0x08

    float m_Fps;                   // 0x04
    Chrono m_Chrono;               // 0x08 (the time of the current frame)
    bool m_IsFirst;                // 0x30
    EventHandler* m_pEventHandler; // 0x34
    Event* m_pEvent;               // 0x38
    u32 m_FrameTime;               // 0x3C (ms)
    u32 m_TotalTime;               // 0x40
    u32 m_TotalWorkTime;           // 0x44
    s32 m_MinWorkTime;             // 0x48
    s32 m_MaxWorkTime;             // 0x4C
    u32 m_FrameCount;              // 0x50
    bool m_HasStatistics;          // 0x54
};
ASSERT_OFFSET(Pacer, m_IsFirst, 0x30);
ASSERT_SIZE(Pacer, 0x58);
} // namespace nex
} // namespace nn
