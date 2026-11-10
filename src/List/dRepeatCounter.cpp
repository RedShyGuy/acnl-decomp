#include "List/dRepeatCounter.h"

// 0x006E636C
void RepeatCounter::Update(bool isHeld)
{
    if (!isHeld) {
        m_Counter = -1;
        m_Limit = 11;
        m_IsFirst = true;
        return;
    }
    m_Counter++;
    if (m_Limit > m_Counter) {
        return;
    }
    m_Counter = 0;
    m_Limit = 3;
    m_IsFirst = false;
}

// 0x006E63B0
RepeatCounter::RepeatCounter() : m_Counter(-1), m_Limit(11), m_IsFirst(true)
{
}
