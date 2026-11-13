// The census with its event dates (SvCensusData): the event values of the census start at 0 again for every event.
// All names are ours.

#include "Sv/dSvCensus.h"

#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "Sv/dSvInsectConventionExtern.h"
#include "event/event_Extern.h"
#include "time/time_Api.h"
#include "time/time_Extern.h"

namespace {
// the clear functions of the event values in the order of the event dates
// 0x0088F54C
void (SvCensus::*const s_ClearEventValueFunctions[12])() = {
    &SvCensus::ClearTourneyFishCaught,
    &SvCensus::ClearTourneyInsectCaught,
    &SvCensus::ClearFestivalFeathersCaught,
    &SvCensus::ClearEggsGivenToZipper,
    &SvCensus::ClearFoundImposterBlanca,
    &SvCensus::ClearFireworkDesignsGivenToIsabelle,
    &SvCensus::ClearReceivedCandyOnHalloween,
    &SvCensus::ClearHarvestFestivalCoursesDone,
    &SvCensus::ClearSnowmenBuiltByYouInTown,
    &SvCensus::ClearTotalBingoWins,
    &SvCensus::ClearGivenToyDayPresents,
    &SvCensus::ClearPartyPoppersPoppedAtNewYears,
};

// (inline) the date is set (not 0)
DECOMP_ALWAYS_INLINE bool IsValid(const SvDate* date)
{
    return date->year != 0 && date->month != 0 && date->day != 0;
}

// (inline) the date is that day
DECOMP_ALWAYS_INLINE bool IsSameDay(const SvDate* date, const nn::fnd::DateTimeParameters& day)
{
    return date->year == static_cast<u16>(day.year) && date->month == static_cast<u8>(day.month) &&
           date->day == static_cast<u8>(day.day);
}

// (inline) the year of the winter of a year: from November on the next year, kept in [2012, 2051)
DECOMP_ALWAYS_INLINE u16 KeepYear(u16 year)
{
    return year >= 2051 ? 2012 : year;
}
} // namespace

// 0x002A6BCC (name is ours)
void SvCensusData::Clear()
{
    census.Clear();
    ClearEventDates();
}

// 0x002A6BE8 (name is ours)
void SvCensusData::ClearEventDates()
{
    tourneyFishCaughtDate = SvDate();
    tourneyInsectCaughtDate = SvDate();
    festivalFeathersCaughtDate = SvDate();
    eggsGivenToZipperDate = SvDate();
    foundImposterBlancaDate = SvDate();
    fireworkDesignsGivenToIsabelleDate = SvDate();
    receivedCandyOnHalloweenDate = SvDate();
    harvestFestivalCoursesDoneDate = SvDate();
    snowmenBuiltByYouInTownDate = SvDate();
    givenToyDayPresentsDate = SvDate();
}

// 0x002A6CB8 (name is ours)
bool SvCensusData::UpdateEventValue(SvDate* date, const nn::fnd::DateTimeParameters* today, u8 valueIndex,
                                    void (SvCensus::*clear)(), bool isSameSeason)
{
    if (IsValid(date)) {
        nn::fnd::DateTimeParameters yesterday = *today;
        event::AddDays(&yesterday, -1);
        if (IsSameDay(date, *today) || IsSameDay(date, yesterday)) {
            // (still the same event)
            if (Func4DD108(valueIndex)) {
                event::Func30F6DC(date, today);
            }
            return false;
        }
        if (Func4DD108(valueIndex)) {
            event::Func30F6DC(date, today);
            if (isSameSeason) {
                return false;
            }
        } else {
            *date = SvDate();
        }
    } else if (Func4DD108(valueIndex)) {
        event::Func30F6DC(date, today);
    }
    (census.*clear)();
    return true;
}

// 0x002A6E18 (name is ours)
void SvCensusData::UpdateEventValues(const nn::fnd::DateTimeParameters* today)
{
    if (time::SvTimeIsValid(&GetGardenPlus()->data.townData.unk_0x1AE6E)) {
        // (the result is not used)
        const nn::fnd::DateTimeParameters now = *time::GetCurrentDate();
        Func2FB6DC(now, time::SvTimeToParameters(&GetGardenPlus()->data.townData.unk_0x1AE6E));
    }
    UpdateEventValue(&tourneyFishCaughtDate, today, CENSUS_EVENT_TOURNEY_FISH_CAUGHT, s_ClearEventValueFunctions[0], false);
    UpdateEventValue(&tourneyInsectCaughtDate, today, CENSUS_EVENT_TOURNEY_INSECT_CAUGHT, s_ClearEventValueFunctions[1], false);
    UpdateEventValue(&festivalFeathersCaughtDate, today, CENSUS_EVENT_FESTIVAL_FEATHERS_CAUGHT, s_ClearEventValueFunctions[2], false);
    UpdateEventValue(&eggsGivenToZipperDate, today, CENSUS_EVENT_EGGS_GIVEN_TO_ZIPPER, s_ClearEventValueFunctions[3], false);
    UpdateEventValue(&foundImposterBlancaDate, today, CENSUS_EVENT_FOUND_IMPOSTER_BLANCA, s_ClearEventValueFunctions[4], false);
    UpdateEventValue(&fireworkDesignsGivenToIsabelleDate, today, CENSUS_EVENT_FIREWORK_DESIGNS_GIVEN_TO_ISABELLE, s_ClearEventValueFunctions[5], false);
    UpdateEventValue(&receivedCandyOnHalloweenDate, today, CENSUS_EVENT_RECEIVED_CANDY_ON_HALLOWEEN, s_ClearEventValueFunctions[6], false);
    UpdateEventValue(&harvestFestivalCoursesDoneDate, today, CENSUS_EVENT_HARVEST_FESTIVAL_COURSES_DONE, s_ClearEventValueFunctions[7], false);
    // the snowflakes of the winter (the original compares the year of the stored date with 10, not its month)
    const u16 winter = KeepYear(static_cast<u16>(today->month > 10 ? today->year + 1 : today->year));
    bool isSameWinter = false;
    if (IsValid(&snowmenBuiltByYouInTownDate)) {
        const u16 year = snowmenBuiltByYouInTownDate.year;
        isSameWinter = winter == KeepYear(static_cast<u16>(year > 10 ? year + 1 : year));
    }
    UpdateEventValue(&snowmenBuiltByYouInTownDate, today, CENSUS_EVENT_SNOWMEN_BUILT_BY_YOU_IN_TOWN, s_ClearEventValueFunctions[8],
                     false);
    if (isSameWinter) {
        census.snowflakesCaught.Clear();
    }
    bool isSameBingoWinter = false;
    if (IsValid(&totalBingoWinsDate)) {
        const u16 year = totalBingoWinsDate.year;
        isSameBingoWinter = winter == KeepYear(static_cast<u16>(year > 10 ? year + 1 : year));
    }
    UpdateEventValue(&totalBingoWinsDate, today, CENSUS_EVENT_TOTAL_BINGO_WINS, s_ClearEventValueFunctions[9],
                     isSameBingoWinter);
    UpdateEventValue(&givenToyDayPresentsDate, today, CENSUS_EVENT_GIVEN_TOY_DAY_PRESENTS, s_ClearEventValueFunctions[10], false);
    UpdateEventValue(&partyPoppersPoppedAtNewYearsDate, today, CENSUS_EVENT_PARTY_POPPERS_POPPED_AT_NEW_YEARS,
                     s_ClearEventValueFunctions[11], false);
}

// 0x002A7110 (name is ours)
SvCensusData::SvCensusData()
{
    ClearEventDates();
}
