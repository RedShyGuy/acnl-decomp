#include "nn/nex/nex_Pacer.h"
#include "nn/nex/nex_Event.h"
#include "nn/nex/nex_EventHandler.h"

namespace nn {
namespace nex {
namespace {
// the milliseconds of a second
const float MILLISECONDS_PER_SECOND = 1000.0f;
} // namespace

// 0x003CEA40 slot 0x08 (name is ours)
void nn::nex::Pacer::Pace()
{
    if (m_IsFirst) {
        m_IsFirst = false;
        m_Chrono.StartInline();
        return;
    }
    s32 workTime = m_Chrono.CheckInline();
    if (workTime < 0) {
        workTime = m_FrameTime;
    }
    if (static_cast<s32>(m_FrameTime) > workTime && m_FrameTime - workTime != 0) {
        Event* pEvent;
        m_pEventHandler->WaitForBoolEvent(m_FrameTime - workTime, &pEvent, true);
    }
    if (m_HasStatistics) {
        m_FrameCount++;
        m_TotalWorkTime += workTime;
        m_TotalTime += m_Chrono.Stop();
        if (workTime > m_MaxWorkTime) {
            m_MaxWorkTime = workTime;
        }
        if (workTime < m_MinWorkTime) {
            m_MinWorkTime = workTime;
        }
    } else {
        m_TotalTime = m_Chrono.Stop();
        m_FrameCount = 1;
        m_TotalWorkTime = workTime;
        m_HasStatistics = true;
        m_MaxWorkTime = workTime;
        m_MinWorkTime = workTime;
    }
    m_Chrono.StartInline();
}

// 0x003CEBDC (name is ours)
nn::nex::Pacer::Pacer(float fps) : m_Fps(fps)
{
    if (fps == 0.0f) {
        m_FrameTime = 0;
    } else {
        m_FrameTime = static_cast<u32>(MILLISECONDS_PER_SECOND / fps);
    }
    m_IsFirst = true;
    m_HasStatistics = false;
    m_pEventHandler = new EventHandler(1);
    m_pEvent = m_pEventHandler->CreateEventObject(0, 0);
}

// 0x00136C58 slot 0x00 (name is ours)
// 0x003CECCC (deleting dtor)
nn::nex::Pacer::~Pacer()
{
    if (m_pEventHandler != nullptr) {
        if (m_pEvent != nullptr) {
            m_pEventHandler->DeleteEventObject(m_pEvent);
        }
        delete m_pEventHandler;
    }
}

} // namespace nex
} // namespace nn
