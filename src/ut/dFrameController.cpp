#include "ut/dFrameController.h"

namespace ut {
// 0x004DB7C0 slot 0x08
void ut::FrameController::Reset()
{
    m_Frame = m_PlayMode == PLAY_MODE_ONCE_REVERSE ? m_EndFrame : m_StartFrame;
    m_IsEnd = false;
}

// 0x004DB600 slot 0x1C
void ut::FrameController::SetEndFrame(f32 frame)
{
    m_EndFrame = frame;
}

// 0x004DB608 slot 0x20
void ut::FrameController::SetStartFrame(f32 frame)
{
    m_StartFrame = frame;
}

// 0x004DB610 slot 0x24
void ut::FrameController::SetSpeed(f32 speed)
{
    m_Speed = speed;
}

// 0x004DB5F8 slot 0x28
void ut::FrameController::SetPlayMode(s32 mode)
{
    m_PlayMode = mode;
}

// 0x004DB618 slot 0x2C
void ut::FrameController::SetFrame(f32 frame)
{
    m_Frame = frame;
}

// 0x004DB620
bool ut::FrameController::Update()
{
    const f32 speed = m_Speed;
    if (m_State == STATE_PLAYING) {
        switch (m_PlayMode) {
        case PLAY_MODE_ONCE:
            if (m_IsEnd) {
                m_State = STATE_STOPPING;
                break;
            }
            m_Frame += speed;
            if (m_Frame >= m_EndFrame) {
                m_Frame = m_EndFrame;
                m_IsEnd = true;
            }
            break;
        case PLAY_MODE_ONCE_REVERSE:
            if (m_IsEnd) {
                m_State = STATE_STOPPING;
                break;
            }
            m_Frame -= speed;
            if (m_Frame <= m_StartFrame) {
                m_Frame = m_StartFrame;
                m_IsEnd = true;
            }
            break;
        case PLAY_MODE_LOOP:
            m_Frame += speed;
            if (m_Frame >= m_EndFrame) {
                const f32 length = m_EndFrame - m_StartFrame;
                do {
                    m_Frame -= length;
                } while (m_Frame >= m_EndFrame);
            }
            break;
        case PLAY_MODE_LOOP_REVERSE:
            m_Frame -= speed;
            if (m_Frame <= m_StartFrame) {
                const f32 length = m_EndFrame - m_StartFrame;
                do {
                    m_Frame += length;
                } while (m_Frame <= m_StartFrame);
            }
            break;
        case PLAY_MODE_PING_PONG:
            if (m_IsReverse) {
                m_Frame -= speed;
                if (m_Frame <= m_StartFrame) {
                    m_Frame = m_StartFrame;
                    m_IsReverse = false;
                }
            } else {
                m_Frame += speed;
                if (m_Frame >= m_EndFrame) {
                    m_Frame = m_EndFrame;
                    m_IsReverse = true;
                }
            }
            break;
        }
        if (m_IsStopRequested) {
            m_State = STATE_STOPPING;
        }
    } else if (m_State == STATE_STOPPING || m_State == STATE_3) {
        m_State = STATE_STOPPED;
    }
    m_IsStopRequested = false;
    return false;
}

// 0x004DB7E0
u32 GetTextLength(const wchar_t* text)
{
    u32 length = 0;
    for (;;) {
        const u16 c = *text;
        if (c == 0) {
            return length;
        }
        if (c == 0x0E) {
            // the tag: group, index, size of the parameters in bytes, the parameters
            const u16 size = text[3];
            text += 3;
            length += 3 + size / 2;
            text = reinterpret_cast<const wchar_t*>(reinterpret_cast<const u8*>(text) + (size & ~1));
        } else if (c == 0x0F) {
            // the end of a tag: group, index
            text += 2;
            length += 2;
        }
        text++;
        length++;
    }
}

} // namespace ut
