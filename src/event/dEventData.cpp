// the events of the calendar (the original dEventData.cpp; the names of the events are empty in this build, their ids are
// in EventBase::Info, names are ours)
#include "event/dEventAnniversary.h"
#include "event/dEventDate.h"
#include "event/dEventEveryDate.h"
#include "event/dEventMonthlyWeekday.h"
#include "event/dEventNpcBirthday.h"
#include "event/dEventPlayerBirthday.h"
#include "event/event_Api.h"
#include "sead/hostio/seadNode.h"

namespace event {
namespace {
// the host I/O node of the events (no function uses its values; names are ours)
// vtable 0x0085FB98 (vptr 0x0085FBA0), offset_to_top 0, 1 entries
class EventHostIO : public sead::hostio::Node
{
public:
    EventHostIO() : _4(0), _8(EVENT_ID_NONE), _C(EVENT_ID_NONE), _10(EVENT_ID_NONE), _14(EVENT_ID_NONE), _18(7), _1C(0),
                    _20(1), _24(1)
    {
    }
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00

    u32 _4;  // 0x04
    u32 _8;  // 0x08
    u32 _C;  // 0x0C
    u32 _10; // 0x10
    u32 _14; // 0x14
    u32 _18; // 0x18
    u32 _1C; // 0x1C
    u32 _20; // 0x20
    u32 _24; // 0x24
};

inline void EventHostIO::vf_0x00()
{
    // empty (in the original it returns 1; the code is shared with other nodes)
}
} // namespace

// the events (the temporary EventBase of each is copied by the constructor of its class)
// 0x007893F0 (static initializer)

// 0x009B3C04
EventMonthlyWeekday s_Event00(EventBase("", "", EventBase::Times{Time(6, 0), Time(22, 0), Time(9, 0), Time(18, 0)}, EventBase::Info{0x03FF, 4, EVENT_ID_FISHING_TOURNEY, 0}, EventBase::Range{Time(6, 0), Time(21, 0), 1}, EventBase::Schedule(5, 15, 25, Time(9, 0), Time(9, 5), Time(9, 5), Time(17, 55), Time(18, 0), 1)),
        EventMonthlyWeekday::Param{0x0415, 6, 3});

// 0x009B3C44
EventEveryDate s_Event01(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x0010, 0, EVENT_ID_MOTHERS_DAY, 2}, EventBase::Range{Time(0, 0), Time(0, 0), 0}, EventBase::Schedule()),
        s_DatesMothersDayUk);

// 0x009B3C84
EventDate s_Event02(EventBase("", "", EventBase::Times{Time(18, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x03FF, 4, EVENT_ID_HALLOWEEN, 2}, EventBase::Range{Time(18, 0), Time(0, 0), 1}, EventBase::Schedule(3, 13, 23, Time(18, 0), Time(18, 5), Time(18, 5), Time(23, 55), Time(0, 0), 1)),
        EventDate::Param{10, 31});

// 0x009B3CC4
EventMonthlyWeekday s_Event03(EventBase("", "", EventBase::Times{Time(6, 0), Time(22, 0), Time(9, 0), Time(18, 0)}, EventBase::Info{0x03FF, 4, EVENT_ID_FISHING_TOURNEY, 1}, EventBase::Range{Time(6, 0), Time(21, 0), 1}, EventBase::Schedule(5, 15, 25, Time(9, 0), Time(9, 5), Time(9, 5), Time(17, 55), Time(18, 0), 1)),
        EventMonthlyWeekday::Param{0x0A0A, 6, 2});

// 0x009B3D04
EventEveryDate s_Event04(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x0100, 0, EVENT_ID_FATHERS_DAY, 2}, EventBase::Range{Time(0, 0), Time(0, 0), 0}, EventBase::Schedule()),
        s_DatesFathersDayGermany);

// 0x009B3D44
EventDate s_Event05(EventBase("", "", EventBase::Times{Time(18, 0), Time(6, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x03FF, 4, EVENT_ID_TOY_DAY, 2}, EventBase::Range{Time(18, 0), Time(6, 0), 1}, EventBase::Schedule(1, 1, 0, Time(18, 0), Time(18, 5), Time(18, 5), Time(23, 55), TIME_NONE, 1)),
        EventDate::Param{12, 24});

// 0x009B3D84
EventMonthlyWeekday s_Event06(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, Time(22, 0)}, EventBase::Info{0x03FF, 4, EVENT_ID_HARVEST_FESTIVAL, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule(2, 12, 22, Time(6, 0), Time(6, 5), Time(6, 5), Time(21, 55), Time(22, 0), 1)),
        EventMonthlyWeekday::Param{0x0400, 4, 4});

// 0x009B3DC4
EventDate s_Event07(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x03FF, 0, EVENT_ID_VALENTINES_DAY, 2}, EventBase::Range{Time(0, 0), Time(0, 0), 0}, EventBase::Schedule()),
        EventDate::Param{2, 14});

// 0x009B3E04
EventDate s_Event08(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), Time(23, 0), TIME_NONE}, EventBase::Info{0x03FF, 0, EVENT_ID_COUNTDOWN, 2}, EventBase::Range{Time(6, 0), Time(6, 0), 1}, EventBase::Schedule(4, 14, 24, Time(23, 30), Time(23, 45), Time(23, 45), Time(23, 57), Time(0, 0), 1)),
        EventDate::Param{12, 31});

// 0x009B3E44
EventMonthlyWeekday s_Event09(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), Time(19, 0), TIME_NONE}, EventBase::Info{0x03FF, 0, EVENT_ID_FIREWORKS_SHOW, 2}, EventBase::Range{Time(6, 0), Time(6, 0), 1}, EventBase::Schedule(7, 17, 27, Time(19, 0), Time(19, 5), Time(19, 5), Time(23, 55), Time(0, 0), 1)),
        EventMonthlyWeekday::Param{0x0080, 0, 0});

// 0x009B3E84
EventMonthlyWeekday s_Event10(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x0020, 0, EVENT_ID_MOTHERS_DAY, 2}, EventBase::Range{Time(0, 0), Time(0, 0), 0}, EventBase::Schedule()),
        EventMonthlyWeekday::Param{0x0010, 0, 1});

// 0x009B3EC4
EventEveryDate s_Event11(EventBase("", "", EventBase::Times{Time(6, 0), Time(6, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x03FF, 4, EVENT_ID_FESTIVALE, 2}, EventBase::Range{Time(6, 0), Time(6, 0), 1}, EventBase::Schedule(9, 19, 0, Time(6, 0), Time(6, 5), Time(6, 5), Time(4, 30), TIME_NONE, 1)),
        s_DatesFestivale);

// 0x009B3F04
EventEveryDate s_Event12(EventBase("", "", EventBase::Times{Time(6, 0), Time(6, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x03FF, 4, EVENT_ID_BUNNY_DAY, 2}, EventBase::Range{Time(6, 0), Time(6, 0), 1}, EventBase::Schedule(8, 18, 0, Time(6, 0), Time(6, 5), Time(6, 5), Time(4, 30), TIME_NONE, 1)),
        s_DatesBunnyDay);

// 0x009B3F44
EventDate s_Event13(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x00A0, 0, EVENT_ID_FATHERS_DAY, 2}, EventBase::Range{Time(0, 0), Time(0, 0), 0}, EventBase::Schedule()),
        EventDate::Param{3, 19});

// 0x009B3F84
EventMonthlyWeekday s_Event14(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x018F, 0, EVENT_ID_MOTHERS_DAY, 2}, EventBase::Range{Time(0, 0), Time(0, 0), 0}, EventBase::Schedule()),
        EventMonthlyWeekday::Param{0x0010, 0, 2});

// 0x009B3FC4
EventDate s_Event15(EventBase("", "", EventBase::Times{Time(6, 0), Time(19, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x0001, 4, EVENT_ID_WEEDING_DAY, 2}, EventBase::Range{Time(6, 0), Time(19, 0), 1}, EventBase::Schedule()),
        EventDate::Param{9, 3});

// 0x009B4004
EventMonthlyWeekday s_Event16(EventBase("", "", EventBase::Times{Time(6, 0), Time(22, 0), Time(9, 0), Time(18, 0)}, EventBase::Info{0x03FF, 4, EVENT_ID_BUG_OFF, 2}, EventBase::Range{Time(6, 0), Time(21, 0), 1}, EventBase::Schedule(6, 16, 26, Time(9, 0), Time(9, 5), Time(9, 5), Time(17, 55), Time(18, 0), 1)),
        EventMonthlyWeekday::Param{0x01E0, 6, 3});

// 0x009B4044
EventMonthlyWeekday s_Event17(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x005F, 0, EVENT_ID_FATHERS_DAY, 2}, EventBase::Range{Time(0, 0), Time(0, 0), 0}, EventBase::Schedule()),
        EventMonthlyWeekday::Param{0x0020, 0, 3});

// 0x009B4084
EventDate s_Event18(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x0200, 0, EVENT_ID_MOTHERS_DAY, 2}, EventBase::Range{Time(0, 0), Time(0, 0), 0}, EventBase::Schedule()),
        EventDate::Param{5, 8});

// 0x009B40C4
EventEveryDate s_Event19(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x0040, 0, EVENT_ID_MOTHERS_DAY, 2}, EventBase::Range{Time(0, 0), Time(0, 0), 0}, EventBase::Schedule()),
        s_DatesMothersDayFrance);

// 0x009B4104
EventEveryDate s_Event20(EventBase("", "", EventBase::Times{Time(6, 0), Time(19, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x000E, 0, EVENT_ID_WEEDING_DAY, 2}, EventBase::Range{Time(6, 0), Time(19, 0), 1}, EventBase::Schedule()),
        s_DatesWeedingDayAmerica);

// 0x009B4144
EventEveryDate s_Event21(EventBase("", "", EventBase::Times{Time(6, 0), Time(19, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x01F0, 0, EVENT_ID_WEEDING_DAY, 2}, EventBase::Range{Time(6, 0), Time(19, 0), 1}, EventBase::Schedule()),
        s_DatesWeedingDayEurope);

// 0x009B4184
EventDate s_Event22(EventBase("", "", EventBase::Times{Time(6, 0), Time(19, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x0200, 4, EVENT_ID_WEEDING_DAY, 2}, EventBase::Range{Time(6, 0), Time(19, 0), 1}, EventBase::Schedule()),
        EventDate::Param{3, 2});

// 0x009B41C4
EventDate s_Event23(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x03FF, 0, EVENT_ID_APRIL_FOOLS_DAY, 2}, EventBase::Range{Time(6, 0), Time(6, 0), 1}, EventBase::Schedule()),
        EventDate::Param{4, 1});

// 0x009B4204
EventDate s_Event24(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x03FF, 0, EVENT_ID_HALLOWEEN_PREPARATION, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        EventDate::Param{10, 1});

// 0x009B4244
EventDate s_Event25(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x03FF, 0, EVENT_ID_HALLOWEEN_PREPARATION, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        EventDate::Param{10, 2});

// 0x009B4284
EventDate s_Event26(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x03FF, 0, EVENT_ID_HALLOWEEN_PREPARATION, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        EventDate::Param{10, 3});

// 0x009B42C4
EventDate s_Event27(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x03FF, 0, EVENT_ID_HALLOWEEN_PREPARATION, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        EventDate::Param{10, 4});

// 0x009B4304
EventDate s_Event28(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x03FF, 0, EVENT_ID_HALLOWEEN_PREPARATION, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        EventDate::Param{10, 5});

// 0x009B4344
EventDate s_Event29(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x03FF, 0, EVENT_ID_HALLOWEEN_PREPARATION, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        EventDate::Param{10, 6});

// 0x009B4384
EventDate s_Event30(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x03FF, 0, EVENT_ID_HALLOWEEN_PREPARATION, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        EventDate::Param{10, 7});

// 0x009B43C4
EventDate s_Event31(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x03FF, 1, EVENT_ID_NEW_YEARS_DAY, 2}, EventBase::Range{Time(0, 0), Time(0, 0), 0}, EventBase::Schedule()),
        EventDate::Param{1, 1});

// 0x009B4404
EventDate s_Event32(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x0001, 1, EVENT_ID_BEAN_DAY, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        EventDate::Param{2, 3});

// 0x009B4444
EventEveryDate s_Event33(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x0200, 1, EVENT_ID_SEOLLAL, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        s_DatesSeollal);

// 0x009B4484
EventDate s_Event34(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x0001, 1, EVENT_ID_GIRLS_DAY, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        EventDate::Param{3, 3});

// 0x009B44C4
EventDate s_Event35(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x0001, 1, EVENT_ID_CHILDRENS_DAY, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        EventDate::Param{5, 5});

// 0x009B4504
EventEveryDate s_Event36(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x0201, 1, EVENT_ID_TSUKIMI_CHUSEOK, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        s_DatesTsukimiChuseok);

// 0x009B4544
EventEveryDate s_Event37(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x0200, 1, EVENT_ID_DAEBOREUM, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        s_DatesDaeboreum);

// 0x009B4584
EventEveryDate s_Event38(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x01FE, 1, EVENT_ID_AUTUMN_MOON, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        s_DatesAutumnMoon);

// 0x009B45C4
EventDate s_Event39(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x000E, 1, EVENT_ID_GROUNDHOG_DAY, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        EventDate::Param{2, 2});

// 0x009B4604
EventDate s_Event40(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x0001, 1, EVENT_ID_STARCROSSED_DAY, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        EventDate::Param{7, 7});

// 0x009B4644
EventDate s_Event41(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x000E, 1, EVENT_ID_NATURE_DAY, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        EventDate::Param{4, 22});

// 0x009B4684
EventMonthlyWeekday s_Event42(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x000E, 1, EVENT_ID_LABOR_DAY, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        EventMonthlyWeekday::Param{0x0100, 1, 1});

// 0x009B46C4
EventEveryDate s_Event43(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x0201, 1, EVENT_ID_SUMMER_SOLSTICE, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        s_DatesSummerSolsticeJapanKorea);

// 0x009B4704
EventDate s_Event44(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x01FE, 1, EVENT_ID_SUMMER_SOLSTICE, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        EventDate::Param{6, 21});

// 0x009B4744
EventMonthlyWeekday s_Event45(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x000E, 1, EVENT_ID_EXPLORERS_DAY, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        EventMonthlyWeekday::Param{0x0200, 1, 2});

// 0x009B4784
EventDate s_Event46(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x01F0, 1, EVENT_ID_NAUGHTY_OR_NICE_DAY, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        EventDate::Param{12, 6});

// 0x009B47C4
EventEveryDate s_Event47(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x0201, 1, EVENT_ID_WINTER_SOLSTICE, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        s_DatesWinterSolsticeJapanKorea);

// 0x009B4804
EventDate s_Event48(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x01FE, 1, EVENT_ID_WINTER_SOLSTICE, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        EventDate::Param{12, 21});

// 0x009B4844
EventDate s_Event49(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x0200, 1, EVENT_ID_TREE_DAY, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        EventDate::Param{4, 5});

// 0x009B4884
EventDate s_Event50(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x0200, 1, EVENT_ID_TEACHERS_DAY, 2}, EventBase::Range{Time(6, 0), Time(0, 0), 1}, EventBase::Schedule()),
        EventDate::Param{5, 15});

// 0x009B48C4
EventDate s_Event51(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x01FE, 1, EVENT_ID_SHAMROCK_DAY, 2}, EventBase::Range{Time(0, 0), Time(0, 0), 0}, EventBase::Schedule()),
        EventDate::Param{3, 17});

// 0x009B4904
EventEveryDate s_Event52(EventBase("", "", EventBase::Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE}, EventBase::Info{0x0001, 1, EVENT_ID_OBON, 2}, EventBase::Range{Time(0, 0), Time(0, 0), 0}, EventBase::Schedule()),
        s_DatesObon);

// 0x009B4944
EventPlayerBirthday s_PlayerBirthday;
// 0x009B4980
EventNpcBirthday s_NpcBirthday;
// 0x009B49BC
EventAnniversary s_Anniversary;

namespace {
// 0x009B4A04
EventHostIO s_HostIO;
} // namespace

// 0x009534FC
EventBase* s_Events[EVENT_NUM] = {
    &s_Event00,
    &s_Event01,
    &s_Event02,
    &s_Event03,
    &s_Event04,
    &s_Event05,
    &s_Event06,
    &s_Event07,
    &s_Event08,
    &s_Event09,
    &s_Event10,
    &s_Event11,
    &s_Event12,
    &s_Event13,
    &s_Event14,
    &s_Event15,
    &s_Event16,
    &s_Event17,
    &s_Event18,
    &s_Event19,
    &s_Event20,
    &s_Event21,
    &s_Event22,
    &s_Event23,
    &s_Event24,
    &s_Event25,
    &s_Event26,
    &s_Event27,
    &s_Event28,
    &s_Event29,
    &s_Event30,
    &s_Event31,
    &s_Event32,
    &s_Event33,
    &s_Event34,
    &s_Event35,
    &s_Event36,
    &s_Event37,
    &s_Event38,
    &s_Event39,
    &s_Event40,
    &s_Event41,
    &s_Event42,
    &s_Event43,
    &s_Event44,
    &s_Event45,
    &s_Event46,
    &s_Event47,
    &s_Event48,
    &s_Event49,
    &s_Event50,
    &s_Event51,
    &s_Event52,
    &s_PlayerBirthday,
    &s_NpcBirthday,
    &s_Anniversary,
};
} // namespace event
