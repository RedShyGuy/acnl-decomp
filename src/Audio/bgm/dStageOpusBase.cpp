#include "Audio/bgm/dStageOpusBase.h"
#include "Audio/bgm/dBgmClock.h"
#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dBgmRequestMgr.h"

// 0x005861BC
void audio::bgm::StageOpusBase::Initialize()
{
    // empty (in the original too)
}

// 0x005861C0
void audio::bgm::StageOpusBase::Setup(stage::Name)
{
    // empty (in the original too)
}

// 0x005861C4
void audio::bgm::StageOpusBase::Ready()
{
    // empty (in the original too)
}

// 0x005861C8
void audio::bgm::StageOpusBase::Enter()
{
    // empty (in the original too)
}

// 0x005861CC
void audio::bgm::StageOpusBase::Leave(stage::Name)
{
    // empty (in the original too)
}

// 0x005861D0
void audio::bgm::StageOpusBase::SetupKeep(stage::Name)
{
    // empty (in the original too)
}

// 0x005861D4
void audio::bgm::StageOpusBase::ReadyKeep()
{
    // empty (in the original too)
}

// 0x005861D8
void audio::bgm::StageOpusBase::EnterKeep()
{
    // empty (in the original too)
}

// 0x005861DC
void audio::bgm::StageOpusBase::Update()
{
    // empty (in the original too)
}

// 0x005861E0
u32 audio::bgm::StageOpusBase::GetBgmId(bool)
{
    return 0;
}

// 0x005861E8
bool audio::bgm::StageOpusBase::Start()
{
    if (!OpusBase::Start()) {
        return false;
    }
    m_State = STATE_PLAYING;
    return true;
}

// 0x0058626C
bool audio::bgm::StageOpusBase::Stop()
{
    if (!OpusBase::Stop()) {
        return false;
    }
    m_State = STATE_STOPPED;
    return true;
}

// 0x005862AC
void audio::bgm::StageOpusBase::UpdateHourly(bool isAlways, bool isNearHour, bool force)
{
    if (m_IsSuspended) {
        return;
    }
    if (!m_IsClockChanged && !IsTimeChanged() && !force) {
        return;
    }
    m_IsClockChanged = IsClockJumped() | force;
    if (IsInTime(0, 0, 0, 15)) {
        // the first seconds of the hour: the music of the new hour starts
        if (m_IsClockChanged) {
            u32 id = GetBgmId(false);
            if (m_BgmId == id) {
                return;
            }
            if (m_State == STATE_PLAYING) {
                m_IsSwitching = 1;
                if (Stop()) {
                    m_NextBgmId = id;
                    m_State = STATE_STOPPED;
                    m_BgmId = id;
                }
            } else if (m_State == STATE_STOPPED) {
                m_BgmId = id;
                m_NextBgmId = id;
            } else {
                GetRequestMgr()->Remove(m_Kind, m_BgmId);
                m_NextBgmId = id;
                m_BgmId = id;
                m_State = STATE_STOPPED;
            }
            return;
        }
        if (isNearHour && s_ClockState.isNearHour && (isAlways || m_BgmId != m_NextBgmId)) {
            if (m_State == STATE_PLAYING) {
                m_IsSwitching = 1;
                if (Stop()) {
                    m_State = STATE_STOPPED;
                    u32 id = GetBgmId(false);
                    m_NextBgmId = id;
                    m_BgmId = id;
                }
                return;
            }
            if (m_State == STATE_STOPPED) {
                return;
            }
        } else {
            if (m_State == STATE_PLAYING) {
                return;
            }
            if (m_State == STATE_STOPPED) {
                if (isAlways || m_BgmId != m_NextBgmId) {
                    return;
                }
                if (Start()) {
                    m_State = STATE_PLAYING;
                    m_NextBgmId = GetBgmId(true);
                }
                return;
            }
        }
        GetRequestMgr()->Remove(m_Kind, m_BgmId);
        m_State = STATE_STOPPED;
        m_NextBgmId = GetBgmId(false);
        return;
    }
    if (IsInTime(59, 45, 0, 0)) {
        // the last seconds of the hour: the music stops when the next hour has another one
        if (m_IsClockChanged) {
            u32 id = GetBgmId(false);
            u32 next = GetBgmId(true);
            m_NextBgmId = next;
            if (m_BgmId == id) {
                return;
            }
            if (m_State == STATE_PLAYING) {
                m_IsSwitching = 1;
                if (Stop()) {
                    m_BgmId = id;
                    m_State = STATE_STOPPED;
                }
            } else if (m_State == STATE_STOPPED) {
                m_BgmId = id;
            } else if (m_BgmId == next) {
                GetRequestMgr()->Remove(m_Kind, m_BgmId);
                m_State = STATE_STOPPED;
            }
            return;
        }
        if (m_State == STATE_PLAYING) {
            u32 next = GetBgmId(true);
            m_NextBgmId = next;
            if (!isAlways && m_BgmId == next) {
                return;
            }
            if (!Stop()) {
                return;
            }
            m_State = STATE_FADED;
            if (isAlways) {
                m_BgmId = 0;
            }
            return;
        }
        if (m_State == STATE_STOPPED && m_BgmId == m_NextBgmId) {
            if (isAlways & isNearHour) {
                return;
            }
            if (Start()) {
                m_State = STATE_PLAYING;
            }
        }
        return;
    }
    if (m_IsClockChanged) {
        u32 id = GetBgmId(false);
        if (m_BgmId != id) {
            if (m_State == STATE_PLAYING) {
                m_IsSwitching = 1;
                if (Stop()) {
                    m_NextBgmId = id;
                    m_State = STATE_STOPPED;
                }
            } else if (m_State == STATE_STOPPED) {
                m_NextBgmId = id;
            } else {
                GetRequestMgr()->Remove(m_Kind, m_BgmId);
                m_NextBgmId = id;
                m_State = STATE_STOPPED;
            }
            return;
        }
        if (m_State == STATE_STOPPED) {
            m_BgmId = id;
            if (!Start()) {
                return;
            }
        } else {
            if (m_State == STATE_FADED) {
                m_IsSwitching = 1;
                if (Stop()) {
                    m_NextBgmId = id;
                    m_State = STATE_STOPPED;
                }
            }
            return;
        }
    } else {
        if (m_State == STATE_FADED) {
            GetRequestMgr()->Remove(m_Kind, m_BgmId);
            m_State = STATE_STOPPED;
            m_NextBgmId = GetBgmId(false);
            return;
        }
        if (m_State != STATE_STOPPED) {
            return;
        }
        m_BgmId = m_NextBgmId;
        if (!Start()) {
            return;
        }
    }
    m_NextBgmId = GetBgmId(true);
    m_State = STATE_PLAYING;
}

// 0x00586744
audio::bgm::StageOpusBase::StageOpusBase()
    : m_BgmId(0), m_NextBgmId(0), m_State(STATE_STOPPED), m_IsReady(0), m_IsSuspended(0), m_IsCurrent(0), m_IsKept(0),
      m_IsClockChanged(0)
{
}
