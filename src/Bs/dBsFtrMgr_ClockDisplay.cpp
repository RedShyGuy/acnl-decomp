#include "Bs/dBsFtrMgr_ClockDisplay.h"
#include "time/time_Api.h"

// 0x0058E4E0
FtrClockDisplay::FtrClockDisplay() : m_BlinkTimer(BLINK_FRAMES)
{
    m_Date = *time::GetCurrentDate();
}

// 0x0058E3C4
void FtrClockDisplay::UpdateDigits()
{
    m_PreviousDate = m_Date;
    m_HourTens.previous = m_HourTens.value;
    m_HourOnes.previous = m_HourOnes.value;
    m_MinuteTens.previous = m_MinuteTens.value;
    m_MinuteOnes.previous = m_MinuteOnes.value;
    m_SecondTens.previous = m_SecondTens.value;
    m_SecondOnes.previous = m_SecondOnes.value;
    m_Date = *time::GetCurrentDate();
    const s8 hour = m_Date.hour;
    const s8 minute = m_Date.minute;
    const s8 second = m_Date.second;
    if (hour < 0 || minute < 0 || second < 0) {
        return;
    }
    m_HourTens.value = hour / 10;
    m_HourOnes.value = hour - m_HourTens.value * 10;
    m_MinuteTens.value = minute / 10;
    m_MinuteOnes.value = minute - m_MinuteTens.value * 10;
    m_SecondTens.value = second / 10;
    m_SecondOnes.value = second - m_SecondTens.value * 10;
}

// 0x0058E4AC
void FtrClockDisplay::UpdateBlink()
{
    if (--m_BlinkTimer == 0) {
        m_BlinkTimer = BLINK_FRAMES;
        m_ColonAlpha = 255;
    } else if (static_cast<u32>(m_BlinkTimer - 1) <= 11) {
        m_ColonAlpha = 0;
    } else {
        m_ColonAlpha = 255;
    }
}
