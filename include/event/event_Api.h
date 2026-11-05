#pragma once

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "event/dEventBase.h"

namespace event {
class EventAnniversary;

// the events (names after the event lists of Nookipedia and animalcrossingwiki.de for New Leaf - Welcome amiibo)
enum EventId : u8
{
    EVENT_ID_FISHING_TOURNEY = 0, // 3rd Saturday of January, March, May, November, 2nd of February, April, October, December
    EVENT_ID_MOTHERS_DAY = 1, // 2nd Sunday of May; UK: Mothering Sunday, Spain: 1st, France: last Sunday of May; Korea: Parents' Day (May 8)
    EVENT_ID_HALLOWEEN = 2,
    EVENT_ID_FATHERS_DAY = 3, // 3rd Sunday of June; Spain, Italy: March 19; Germany: Ascension
    EVENT_ID_TOY_DAY = 4,
    EVENT_ID_HARVEST_FESTIVAL = 5,
    EVENT_ID_VALENTINES_DAY = 6,
    EVENT_ID_COUNTDOWN = 7,
    EVENT_ID_FIREWORKS_SHOW = 8, // every Sunday of August
    EVENT_ID_FESTIVALE = 9,
    EVENT_ID_BUNNY_DAY = 10,
    EVENT_ID_WEEDING_DAY = 11, // Japan: September 3; America: last Friday, Europe: last Saturday of April; Korea: March 2
    EVENT_ID_BUG_OFF = 12, // 3rd Saturday of June to September
    EVENT_ID_APRIL_FOOLS_DAY = 13,
    EVENT_ID_HALLOWEEN_PREPARATION = 14, // October 1 to 7: Jack is in the town before Halloween
    EVENT_ID_NEW_YEARS_DAY = 15,
    EVENT_ID_BEAN_DAY = 16,
    EVENT_ID_SEOLLAL = 17,
    EVENT_ID_GIRLS_DAY = 18,
    EVENT_ID_CHILDRENS_DAY = 19,
    EVENT_ID_TSUKIMI_CHUSEOK = 20, // Japan: Tsukimi (お月見), Korea: Chuseok (추석)
    EVENT_ID_DAEBOREUM = 21,
    EVENT_ID_AUTUMN_MOON = 22, // the other regions
    EVENT_ID_GROUNDHOG_DAY = 23,
    EVENT_ID_STARCROSSED_DAY = 24,
    EVENT_ID_NATURE_DAY = 25,
    EVENT_ID_LABOR_DAY = 26,
    EVENT_ID_SUMMER_SOLSTICE = 27,
    EVENT_ID_EXPLORERS_DAY = 28,
    EVENT_ID_NAUGHTY_OR_NICE_DAY = 29,
    EVENT_ID_WINTER_SOLSTICE = 30,
    EVENT_ID_TREE_DAY = 31,
    EVENT_ID_TEACHERS_DAY = 32,
    EVENT_ID_SHAMROCK_DAY = 33,
    EVENT_ID_OBON = 34,
    EVENT_ID_PLAYER_BIRTHDAY = 35, // EventPlayerBirthday
    EVENT_ID_NPC_BIRTHDAY = 36, // EventNpcBirthday (a villager)
    EVENT_ID_TOWN_ANNIVERSARY = 37, // EventAnniversary (Welcome amiibo)
    EVENT_ID_NUM = 38,
    EVENT_ID_NONE = 38,
};
const s32 EVENT_NUM = 56;

// the state of the events (names are ours)
extern u8 s_Flag0;              // set by SetFlag0, cleared when the current event changes
extern bool s_Flag1;             // set by SetFlag1, cleared when the current event changes
extern u8 s_IsCheckPending;     // Update sets it, Func58EEB4/Func58F5BC check and clear it
extern u8 s_IsForced;           // the next Update takes the current event again
extern u8 s_IsResetRequested;   // the next Update clears the events
extern EventBase* s_pActiveEvents;  // the active events, linked by EventBase::m_pNext
extern EventBase* s_pCurrentEvent;  // the active event with a schedule
// the events (dEventData.cpp)
extern EventBase* s_Events[EVENT_NUM];
extern EventAnniversary s_Anniversary;
// the dates of the events that move every year (2012..2050)
extern const MonthDay s_DatesMothersDayUk[39];
extern const MonthDay s_DatesFathersDayGermany[39];
extern const MonthDay s_DatesFestivale[39];
extern const MonthDay s_DatesBunnyDay[39];
extern const MonthDay s_DatesMothersDayFrance[39];
extern const MonthDay s_DatesWeedingDayAmerica[39];
extern const MonthDay s_DatesWeedingDayEurope[39];
extern const MonthDay s_DatesSeollal[39];
extern const MonthDay s_DatesTsukimiChuseok[39];
extern const MonthDay s_DatesDaeboreum[39];
extern const MonthDay s_DatesAutumnMoon[39];
extern const MonthDay s_DatesSummerSolsticeJapanKorea[39];
extern const MonthDay s_DatesWinterSolsticeJapanKorea[39];
extern const MonthDay s_DatesObon[39];

// the member functions the events are asked with (EventBase::FunctionIndex)
extern const EventBase::Function s_EventFunctions[EventBase::FUNCTION_NUM];
// a function for each event id, called by Update for the active events (all empty in this build; names are ours)
extern void (*const s_EventCallbacks[EVENT_ID_NUM])();
void OnFishingTourney();
void OnMothersDay();
void OnHalloween();
void OnFathersDay();
void OnToyDay();
void OnHarvestFestival();
void OnValentinesDay();
void OnCountdown();
void OnFireworksShow();
void OnFestivale();
void OnBunnyDay();
void OnWeedingDay();
void OnBugOff();
void OnAprilFoolsDay();
void OnHalloweenPreparation();
void OnNewYearsDay();
void OnBeanDay();
void OnSeollal();
void OnGirlsDay();
void OnChildrensDay();
void OnTsukimiChuseok();
void OnDaeboreum();
void OnAutumnMoon();
void OnGroundhogDay();
void OnStarcrossedDay();
void OnNatureDay();
void OnLaborDay();
void OnSummerSolstice();
void OnExplorersDay();
void OnNaughtyOrNiceDay();
void OnWinterSolstice();
void OnTreeDay();
void OnTeachersDay();
void OnShamrockDay();
void OnObon();
void OnPlayerBirthday();
void OnNpcBirthday();
void OnTownAnniversary();

// takes the active events of the date: the list, the current event, the callbacks
void Update(const nn::fnd::DateTimeParameters* date, bool force, bool flag); // 0x0058FBE8
void RequestReset(); // 0x0058F7D4
void SetFlag0(); // 0x0058EF3C
void SetFlag1(); // 0x0058F9F4
bool GetFlag1(); // 0x0058F80C
bool IsNotDream(); // 0x0058F7E8

// an event of the id is on the date (isGameDate: Func308EF0 first)
bool IsEventOnDate(u8 id, nn::fnd::DateTimeParameters date, bool isGameDate); // 0x0058E768
// some event is on the date (withoutPersonal: not the events of players and villagers)
bool IsEventDay(nn::fnd::DateTimeParameters date, bool withoutPersonal); // 0x0058ED10
// the same without Mother's Day, Father's Day, Valentine's Day, the Halloween preparation and the anniversary
bool IsEventDay2(nn::fnd::DateTimeParameters date, bool withoutPersonal); // 0x0058FAD0
bool IsEventDayOfKind(nn::fnd::DateTimeParameters date, u8 kind); // 0x0058EF50
// the current time is in the part of the day of the index (0..6)
bool IsInPartOfDay(u32 index); // 0x0058E7F0
// the first active event of the id
bool IsAfterEnd(u8 id, const nn::fnd::DateTimeParameters* date); // 0x0058E9E0
bool IsInRange(u8 id, const nn::fnd::DateTimeParameters* date); // 0x0058EAE8
bool IsBeforeStart(u8 id, const nn::fnd::DateTimeParameters* date); // 0x0058EB78
bool GetEndDate(u8 id, nn::fnd::DateTimeParameters* date); // 0x0058EDCC
bool GetStartDate(u8 id, nn::fnd::DateTimeParameters* date); // 0x0058F6F0
bool HasActiveEvent(bool withoutPersonal); // 0x0058F660
// the first active event with a special day (the tourneys, the fireworks, Festivale, the Harvest Festival, the
// Countdown; EVENT_ID_NONE: none)
u8 GetActiveSpecialEventId(); // 0x0058FB7C
u8 GetCurrentPhase(); // 0x0058F6C0
s8 GetCurrentScheduleFlag(); // 0x0058F648
s32 GetCurrentScheduleA(); // 0x0058FAB0
s32 GetCurrentScheduleB(); // 0x0058FA10
s32 GetCurrentScheduleC(); // 0x0058F9E0
u8 Func58EE30(const nn::fnd::DateTimeParameters* date); // 0x0058EE30
u8 Func58EE68(); // 0x0058EE68
bool Func58EEB4(); // 0x0058EEB4
bool Func58F5BC(); // 0x0058F5BC
bool Func58F754(u8 kind); // 0x0058F754
void Func58FA24(bool check, const nn::fnd::DateTimeParameters* date); // 0x0058FA24

// the date is in the week from the anniversary of the registration of the player (not in its first year)
bool IsAnniversary(const nn::fnd::DateTimeParameters& date); // 0x0058EFF0
// the date (from the registration of the player of GardenData+0x20) in this year; clampLeapDay: February 29 -> 28
bool GetRegistrationDate(SvDate* date, bool clampLeapDay); // 0x0058F1D0
void Func58F2B4(); // 0x0058F2B4
} // namespace event
