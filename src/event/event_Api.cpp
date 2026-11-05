#include "event/event_Api.h"
#include "Sv/dSvApi.h"
#include "event/dEventAnniversary.h"
#include "event/event_Extern.h"
#include "netgame/netgame_Api.h"
#include "time/time_Api.h"
#include "time/time_Extern.h"

namespace event {
// 0x009534E4
u8 s_Flag0;
// 0x009534E5
bool s_Flag1;
// 0x009534E6
u8 s_IsCheckPending;
// 0x009534E7
u8 s_IsForced;
// 0x009534E8
u8 s_IsResetRequested;
// 0x009534EC
EventBase* s_pActiveEvents;
// 0x009534F0
EventBase* s_pCurrentEvent;

// the globals of the game the events use (names are ours)
// 0x00A84AEC
UnknownA84AEC g_UnknownA84AEC;
// 0x0094FE58
void* g_pUnknown94FE58;
// 0x0094EDF8
u8 g_Unknown94EDF8;

namespace {
inline bool IsDream()
{
    return netgame::GetNetgameType() == netgame::TYPE_DREAM;
}

inline void* GetUnknown1AE50()
{
    return &GetGardenData()->townData.unk_0x1AE50;
}

// a date of the town (SvTownData + 0x1E444) that follows the registration date of the players
inline SvDate* GetTownDate()
{
    return reinterpret_cast<SvDate*>(reinterpret_cast<u8*>(&GetGardenData()->townData) + 0x1E444);
}

inline SvPlayer* GetRegisteredPlayer()
{
    return SvGetPlayer(static_cast<PlayerSaveNumber>(Func302218(reinterpret_cast<u8*>(GetGardenData()) + 0x20)));
}

// the first active event of the id
inline bool CallActiveEvent(u8 id, EventBase::FunctionIndex index, nn::fnd::DateTimeParameters* date)
{
    for (EventBase* event = s_pActiveEvents; event != NULL; event = event->m_pNext) {
        if (event->m_Info.id == id) {
            return (event->*s_EventFunctions[index])(date);
        }
    }
    return false;
}
} // namespace

// 0x0058E764 (name is ours)
void OnObon()
{
    // empty (in the original too)
}

// 0x0058E768 (name is ours)
bool IsEventOnDate(u8 id, nn::fnd::DateTimeParameters date, bool isGameDate)
{
    if (IsDream()) {
        return false;
    }
    if (isGameDate) {
        time::Func308EF0(&date);
    }
    for (s32 i = 0; i < EVENT_NUM; i++) {
        if (s_Events[i]->m_Info.id == id && s_Events[i]->IsOnDate(date.year, date.month, date.day)) {
            return true;
        }
    }
    return false;
}

// 0x0058E7EC (name is ours)
void OnBunnyDay()
{
    // empty (in the original too)
}

// 0x0058E7F0 (name is ours)
bool IsInPartOfDay(u32 index)
{
    if (IsDream()) {
        return false;
    }
    // 0x009B3BE4 (guard 0x009534F8)
    static const Time s_Starts[8] = {Time(6, 0), Time(6, 0), Time(6, 0), Time(6, 0),
                                     Time(8, 0), Time(6, 0), Time(6, 0), Time(0, 0)};
    // 0x009B3BF4 (guard 0x009534F4)
    static const Time s_Ends[8] = {Time(23, 0), Time(12, 0), Time(24, 0), Time(6, 0),
                                   Time(4, 0),  Time(6, 0),  Time(6, 0),  Time(0, 0)};
    if (index >= 7) {
        return false;
    }
    const nn::fnd::DateTimeParameters* date = time::GetCurrentDate();
    Time now = ToTime(*date);
    u16 nowTime = ToDayTime(now.value, date->hour);
    if (ToDayTime(s_Starts[index].value, s_Starts[index].GetHour()) > nowTime) {
        return false;
    }
    return nowTime < ToDayEndTime(s_Ends[index].value);
}

// 0x0058E9E0 (name is ours)
bool IsAfterEnd(u8 id, const nn::fnd::DateTimeParameters* date)
{
    if (IsDream()) {
        return false;
    }
    if (Func750EFC(date)) {
        return CallActiveEvent(id, EventBase::FUNCTION_HAS_TIME_LIMIT, NULL);
    }
    nn::fnd::DateTimeParameters copy = *date;
    return CallActiveEvent(id, EventBase::FUNCTION_IS_AFTER_END, &copy);
}

// 0x0058EADC (name is ours)
void OnNewYearsDay()
{
    // empty (in the original too)
}

// 0x0058EAE0 (name is ours)
void OnTsukimiChuseok()
{
    // empty (in the original too)
}

// 0x0058EAE4 (name is ours)
void OnToyDay()
{
    // empty (in the original too)
}

// 0x0058EAE8 (name is ours)
bool IsInRange(u8 id, const nn::fnd::DateTimeParameters* date)
{
    if (IsDream()) {
        return false;
    }
    nn::fnd::DateTimeParameters copy = *date;
    return CallActiveEvent(id, EventBase::FUNCTION_IS_IN_RANGE, &copy);
}

// 0x0058EB78 (name is ours)
bool IsBeforeStart(u8 id, const nn::fnd::DateTimeParameters* date)
{
    if (IsDream()) {
        return false;
    }
    if (Func750EFC(date)) {
        return CallActiveEvent(id, EventBase::FUNCTION_IS_BEFORE_START_ALL_DAY, NULL);
    }
    nn::fnd::DateTimeParameters copy = *date;
    return CallActiveEvent(id, EventBase::FUNCTION_IS_BEFORE_START, &copy);
}

// 0x0058EC74 (name is ours)
void OnFestivale()
{
    // empty (in the original too)
}

// 0x0058EC78 (name is ours)
void OnNatureDay()
{
    // empty (in the original too)
}

// 0x0058EC7C (name is ours)
void OnFireworksShow()
{
    // empty (in the original too)
}

// 0x0058EC80 (name is ours)
void OnGirlsDay()
{
    // empty (in the original too)
}

// 0x0058EC84 (name is ours)
void OnLaborDay()
{
    // empty (in the original too)
}

// 0x0058EC88 (name is ours)
void OnBeanDay()
{
    // empty (in the original too)
}

// 0x0058EC8C (name is ours)
void OnShamrockDay()
{
    // empty (in the original too)
}

// 0x0058EC90 (name is ours)
void OnStarcrossedDay()
{
    // empty (in the original too)
}

// 0x0058ED10 (name is ours)
bool IsEventDay(nn::fnd::DateTimeParameters date, bool withoutPersonal)
{
    if (IsDream()) {
        return false;
    }
    time::Func308EF0(&date);
    for (s32 i = 0; i < EVENT_NUM; i++) {
        if (withoutPersonal && s_Events[i]->m_Info.id >= EVENT_ID_PLAYER_BIRTHDAY) {
            continue;
        }
        if (s_Events[i]->m_Info.id == EVENT_ID_TOWN_ANNIVERSARY) {
            break;
        }
        if (s_Events[i]->IsOnDate(date.year, date.month, date.day)) {
            return true;
        }
    }
    return false;
}

// 0x0058EDB0 (name is ours)
void OnAprilFoolsDay()
{
    // empty (in the original too)
}

// 0x0058EDB4 (name is ours)
void OnCountdown()
{
    // empty (in the original too)
}

// 0x0058EDB8 (name is ours)
void OnDaeboreum()
{
    // empty (in the original too)
}

// 0x0058EDBC (name is ours)
void OnGroundhogDay()
{
    // empty (in the original too)
}

// 0x0058EDC0 (name is ours)
void OnHalloween()
{
    // empty (in the original too)
}

// 0x0058EDC4 (name is ours)
void OnWinterSolstice()
{
    // empty (in the original too)
}

// 0x0058EDC8 (name is ours)
void OnValentinesDay()
{
    // empty (in the original too)
}

// 0x0058EDCC (name is ours)
bool GetEndDate(u8 id, nn::fnd::DateTimeParameters* date)
{
    return CallActiveEvent(id, EventBase::FUNCTION_GET_END_DATE, date);
}

// 0x0058EE30 (name is ours)
u8 Func58EE30(const nn::fnd::DateTimeParameters* date)
{
    if (IsDream()) {
        return 7;
    }
    return Func75EAF4(GetUnknown1AE50(), *date);
}

// 0x0058EE68 (name is ours)
u8 Func58EE68()
{
    nn::fnd::DateTimeParameters date;
    time::Func3004A8(&date);
    if (IsDream()) {
        return 7;
    }
    return Func75EAF4(GetUnknown1AE50(), date);
}

// 0x0058EEB4 (name is ours)
bool Func58EEB4()
{
    if (s_pCurrentEvent != NULL && s_IsCheckPending) {
        nn::fnd::DateTimeParameters date = *time::GetCurrentDate();
        time::Func308EF0(&date);
        s_IsCheckPending = Func314104(date, s_Anniversary.m_Date);
        if (s_IsCheckPending) {
            return true;
        }
    }
    return false;
}

// 0x0058EF3C (name is ours)
void SetFlag0()
{
    s_Flag0 = true;
}

// 0x0058EF50 (name is ours)
bool IsEventDayOfKind(nn::fnd::DateTimeParameters date, u8 kind)
{
    if (IsDream()) {
        return false;
    }
    time::Func308EF0(&date);
    for (s32 i = 0; i < EVENT_NUM; i++) {
        if ((s_Events[i]->m_Info.kind & 3) == kind && s_Events[i]->IsOnDate(date.year, date.month, date.day)) {
            return true;
        }
    }
    return false;
}

// 0x0058EFDC (name is ours)
void OnAutumnMoon()
{
    // empty (in the original too)
}

// 0x0058EFE0 (name is ours)
void OnFathersDay()
{
    // empty (in the original too)
}

// 0x0058EFE4 (name is ours)
void OnMothersDay()
{
    // empty (in the original too)
}

// 0x0058EFF0 (name is ours)
bool IsAnniversary(const nn::fnd::DateTimeParameters& date)
{
    SvPlayer* player = SvGetCurrentPlayer();
    if (player == NULL) {
        return false;
    }
    if (!player->playerFlags.finishedFirstDay || !player->playerFlags.peteIntroduction ||
        !player->playerFlags.playerSetNameAndTownName) {
        return false;
    }
    SvDate* townDate = GetTownDate();
    if (townDate->year == 0 || townDate->month == 0 || townDate->day == 0) {
        return false;
    }
    if (CompareSvDate(townDate, date.year, date.month, date.day) >= 0) {
        return false;
    }
    if (townDate->year >= date.year) {
        return false;
    }
    if (GetRegisteredPlayer() == NULL) {
        return false;
    }
    SvDate registration = {0, 0, 0};
    if (!GetRegistrationDate(&registration, true)) {
        return false;
    }
    if (CompareSvDate(&registration, date.year, date.month, date.day) >= 0) {
        return false;
    }
    if (date.year - registration.year == 0) {
        return false;
    }
    SvDate anniversary = {0, 0, 0};
    if (!GetRegistrationDate(&anniversary, !IsLeapYear(date.year))) {
        return false;
    }
    nn::fnd::DateTimeParameters start = MakeDateTime(date.year, anniversary.month, anniversary.day, 0, 0, 0, 0);
    nn::fnd::DateTimeParameters end = MakeDateTime(date.year, anniversary.month, anniversary.day, 0, 0, 0, 0);
    AddDays(&end, 6);
    if (!IsNotBefore(date, start)) {
        return false;
    }
    return IsNotBefore(end, date);
}

// 0x0058F1D0 (name is ours)
bool GetRegistrationDate(SvDate* date, bool clampLeapDay)
{
    if (date == NULL) {
        return false;
    }
    SvPlayer* player = GetRegisteredPlayer();
    if (player == NULL) {
        return false;
    }
    const SvDate* registration = reinterpret_cast<const SvDate*>(&player->yearRegistered);
    if (registration->year == 0 || registration->month == 0 || registration->day == 0) {
        return false;
    }
    nn::fnd::DateTimeParameters dateTime = Func30A478(registration);
    SvDate result = {0, 0, 0};
    Func30F6DC(&result, &dateTime);
    if (clampLeapDay && result.day == 29 && result.month == 2) {
        SetSvDate(&result, result.year, result.month, 28);
    }
    SetSvDate(date, result.year, result.month, result.day);
    return true;
}

// 0x0058F2B4 (name is ours)
void Func58F2B4()
{
    SvDate registration = {0, 0, 0};
    if (!GetRegistrationDate(&registration, true)) {
        return;
    }
    SvDate* townDate = GetTownDate();
    if (townDate->year != 0 && townDate->month != 0 && townDate->day != 0 &&
        CompareSvDate(townDate, registration.year, registration.month, registration.day) >= 0) {
        return;
    }
    SetSvDate(townDate, registration.year, registration.month, registration.day);
}

// 0x0058F5BC (name is ours)
bool Func58F5BC()
{
    if (!s_Flag0 && s_IsCheckPending) {
        nn::fnd::DateTimeParameters date = *time::GetCurrentDate();
        time::Func308EF0(&date);
        s_IsCheckPending = Func314104(date, s_Anniversary.m_Date);
        if (s_IsCheckPending) {
            return false;
        }
    }
    return true;
}

// 0x0058F648 (name is ours)
s8 GetCurrentScheduleFlag()
{
    if (s_pCurrentEvent == NULL) {
        return 0;
    }
    return s_pCurrentEvent->m_Schedule.flag;
}

// 0x0058F660 (name is ours)
bool HasActiveEvent(bool withoutPersonal)
{
    if (IsDream()) {
        return false;
    }
    if (s_pActiveEvents == NULL) {
        return false;
    }
    if (withoutPersonal && s_pActiveEvents->m_Info.id >= EVENT_ID_PLAYER_BIRTHDAY) {
        return false;
    }
    return true;
}

// 0x0058F6AC (name is ours)
void OnTownAnniversary()
{
    // empty (in the original too)
}

// 0x0058F6B0 (name is ours)
void OnWeedingDay()
{
    // empty (in the original too)
}

// 0x0058F6B4 (name is ours)
void OnExplorersDay()
{
    // empty (in the original too)
}

// 0x0058F6B8 (name is ours)
void OnNpcBirthday()
{
    // empty (in the original too)
}

// 0x0058F6BC (name is ours)
void OnTeachersDay()
{
    // empty (in the original too)
}

// 0x0058F6C0 (name is ours)
u8 GetCurrentPhase()
{
    if (s_pCurrentEvent == NULL) {
        return 0;
    }
    return s_pCurrentEvent->GetPhase(*time::GetCurrentDate());
}

// 0x0058F6F0 (name is ours)
bool GetStartDate(u8 id, nn::fnd::DateTimeParameters* date)
{
    return CallActiveEvent(id, EventBase::FUNCTION_GET_START_DATE, date);
}

// 0x0058F754 (name is ours)
bool Func58F754(u8 kind)
{
    if (IsDream()) {
        return false;
    }
    if (Func58EE68() == kind && IsInPartOfDay(kind)) {
        return true;
    }
    return false;
}

// 0x0058F7D4 (name is ours)
void RequestReset()
{
    s_IsResetRequested = true;
}

// 0x0058F7E8 (name is ours)
bool IsNotDream()
{
    return !IsDream();
}

// 0x0058F800 (name is ours)
void OnChildrensDay()
{
    // empty (in the original too)
}

// 0x0058F804 (name is ours)
void OnSeollal()
{
    // empty (in the original too)
}

// 0x0058F808 (name is ours)
void OnNaughtyOrNiceDay()
{
    // empty (in the original too)
}

// 0x0058F80C (name is ours)
bool GetFlag1()
{
    return s_Flag1;
}

// 0x0058F81C (name is ours)
void OnTreeDay()
{
    // empty (in the original too)
}

// 0x0058F9E0 (name is ours)
s32 GetCurrentScheduleC()
{
    return s_pCurrentEvent->m_Schedule.c;
}

// 0x0058F9F4 (name is ours)
void SetFlag1()
{
    s_Flag1 = true;
}

// 0x0058FA08 (name is ours)
void OnPlayerBirthday()
{
    // empty (in the original too)
}

// 0x0058FA0C (name is ours)
void OnSummerSolstice()
{
    // empty (in the original too)
}

// 0x0058FA10 (name is ours)
s32 GetCurrentScheduleB()
{
    return s_pCurrentEvent->m_Schedule.b;
}

// 0x0058FA24 (name is ours)
void Func58FA24(bool check, const nn::fnd::DateTimeParameters* date)
{
    if (!check) {
        return;
    }
    if (netgame::GetNetgameType() != netgame::TYPE_NONE) {
        return;
    }
    for (EventBase* event = s_pActiveEvents; event != NULL; event = event->m_pNext) {
        if ((event->m_Info.kind & 3) == 3) {
            if (event->m_Info.id == EVENT_ID_TOWN_ANNIVERSARY && IsAnniversary(*date)) {
                g_Unknown94EDF8 = true;
            }
            return;
        }
    }
}

// 0x0058FAAC (name is ours)
void OnHarvestFestival()
{
    // empty (in the original too)
}

// 0x0058FAB0 (name is ours)
s32 GetCurrentScheduleA()
{
    return s_pCurrentEvent->m_Schedule.a;
}

// 0x0058FAC4 (name is ours)
void OnHalloweenPreparation()
{
    // empty (in the original too)
}

// 0x0058FAC8 (name is ours)
void OnBugOff()
{
    // empty (in the original too)
}

// 0x0058FACC (name is ours)
void OnFishingTourney()
{
    // empty (in the original too)
}

// 0x0058FAD0 (name is ours)
bool IsEventDay2(nn::fnd::DateTimeParameters date, bool withoutPersonal)
{
    if (IsDream()) {
        return false;
    }
    time::Func308EF0(&date);
    for (s32 i = 0; i < EVENT_NUM; i++) {
        u8 id = s_Events[i]->m_Info.id;
        if (withoutPersonal && id >= EVENT_ID_PLAYER_BIRTHDAY) {
            continue;
        }
        if (id == EVENT_ID_VALENTINES_DAY || id == EVENT_ID_MOTHERS_DAY || id == EVENT_ID_FATHERS_DAY ||
            id == EVENT_ID_HALLOWEEN_PREPARATION || id == EVENT_ID_TOWN_ANNIVERSARY) {
            continue;
        }
        if (s_Events[i]->IsOnDate(date.year, date.month, date.day)) {
            return true;
        }
    }
    return false;
}

// 0x0058FB7C (name is ours)
u8 GetActiveSpecialEventId()
{
    if (IsDream()) {
        return EVENT_ID_NONE;
    }
    for (EventBase* event = s_pActiveEvents; event != NULL; event = event->m_pNext) {
        u8 id = event->m_Info.id;
        if (id == EVENT_ID_FIREWORKS_SHOW || id == EVENT_ID_FISHING_TOURNEY || id == EVENT_ID_HARVEST_FESTIVAL ||
            id == EVENT_ID_COUNTDOWN || id == EVENT_ID_FESTIVALE || id == EVENT_ID_BUG_OFF) {
            return id;
        }
    }
    return EVENT_ID_NONE;
}

// 0x0058FBE8 (name is ours)
void Update(const nn::fnd::DateTimeParameters* date, bool force, bool flag)
{
    if (s_IsResetRequested) {
        s_pCurrentEvent = NULL;
        s_Flag0 = false;
        s_Flag1 = false;
        s_pActiveEvents = NULL;
        Func6140F8(GetUnknown1AE50());
        Func240B48();
        s_IsResetRequested = false;
        return;
    }
    if (s_IsForced) {
        s_IsForced = false;
        force = true;
    }
    s_Anniversary.m_Date = *date;
    s_IsCheckPending = true;
    EventBase* list = NULL;
    bool hasActive = false;
    bool isChanged = false;
    EventBase* current = NULL;
    for (s32 i = EVENT_NUM - 1; i >= 0; i--) {
        EventBase* event = s_Events[i];
        if (!event->IsActive(s_Anniversary.m_Date)) {
            continue;
        }
        if (event->m_Info.id == EVENT_ID_TOWN_ANNIVERSARY && !force) {
            continue;
        }
        if (event->m_Info.id == EVENT_ID_WEEDING_DAY && g_SvIsSaveMemoryValid &&
            !Func71D24C(reinterpret_cast<u8*>(GetGardenPlus()) + 0x666E4)) {
            continue;
        }
        event->m_pNext = list;
        list = event;
        if (event->HasSchedule()) {
            if (force || event != s_pCurrentEvent) {
                current = event;
                isChanged = true;
            } else {
                isChanged = false;
                if (s_pCurrentEvent != NULL && s_pCurrentEvent->m_Info.id == EVENT_ID_COUNTDOWN && s_Flag1 &&
                    s_pCurrentEvent->GetPhase(*time::GetCurrentDate()) != 4) {
                    s_Flag1 = false;
                }
            }
        } else if (!hasActive) {
            isChanged = true;
        }
        hasActive = true;
    }
    if (!hasActive || isChanged) {
        s_Flag0 = false;
        s_Flag1 = false;
        s_pCurrentEvent = current;
    }
    s_pActiveEvents = list;
    if (netgame::IsMyPlayerHost()) {
        Func6137F4(GetUnknown1AE50(), date, flag);
        Func613BAC(GetUnknown1AE50(), date);
    }
    if (g_pUnknown94FE58 != NULL) {
        Func18FA70(g_pUnknown94FE58);
    }
    Func240B48();
    Func240EF8();
    for (EventBase* event = s_pActiveEvents; event != NULL; event = event->m_pNext) {
        s_EventCallbacks[event->m_Info.id]();
    }
}
} // namespace event
