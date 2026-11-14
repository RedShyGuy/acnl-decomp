// The census menu (unused; see docs/census.md): the value readers and the tables of the rows. All names are ours.

#include "Sv/dSvCensus.h"

#include "Sound/sound_Extern.h"
#include "event/event_Api.h"
#include "time/time_Api.h"

namespace {
// the value index of each row of the menu (the event values first)
// 0x008588E4
const u8 s_CensusRowValues[CENSUS_ROW_NUM] = {
    CENSUS_VALUE_FESTIVAL_FEATHERS_CAUGHT, CENSUS_VALUE_EGGS_GIVEN_TO_ZIPPER, CENSUS_VALUE_FOUND_IMPOSTER_BLANCA, CENSUS_VALUE_FIREWORK_DESIGNS_GIVEN_TO_ISABELLE,
    CENSUS_VALUE_RECEIVED_CANDY_ON_HALLOWEEN, CENSUS_VALUE_HARVEST_FESTIVAL_COURSES_DONE, CENSUS_VALUE_GIVEN_TOY_DAY_PRESENTS, CENSUS_VALUE_PARTY_POPPERS_POPPED_AT_NEW_YEARS,
    CENSUS_VALUE_TOURNEY_FISH_CAUGHT, CENSUS_VALUE_TOURNEY_INSECT_CAUGHT, CENSUS_VALUE_SNOWMEN_BUILT_BY_YOU_IN_TOWN, CENSUS_VALUE_SNOWFLAKES_CAUGHT,
    CENSUS_VALUE_TOTAL_BINGO_WINS, CENSUS_VALUE_BELLS_EARNED, CENSUS_VALUE_BELLS_SPENT, CENSUS_VALUE_ABD_BALANCE,
    CENSUS_VALUE_LOAN_PAID, CENSUS_VALUE_PUBLIC_WORK_EXPENSES, CENSUS_VALUE_PUBLIC_WORKS_BUILT, CENSUS_VALUE_LETTERS_SENT,
    CENSUS_VALUE_TOWNS_VISITED, CENSUS_VALUE_TOWN_VISITORS, CENSUS_VALUE_NOOKLINGS_EXPENSES, CENSUS_VALUE_ABLE_SISTERS_EXPENSES,
    CENSUS_VALUE_GARDENING_SHOP_EXPENSES, CENSUS_VALUE_KICKS_EXPENSES, CENSUS_VALUE_GRACIES_EXPENSES, CENSUS_VALUE_RECYCLE_SHOP_EARNINGS,
    CENSUS_VALUE_FURNITURE_CUSTOMIZED, CENSUS_VALUE_AMIIBO_SCANNED, CENSUS_VALUE_TOWN_INITIATIVES_COMPLETED, CENSUS_VALUE_MEOW_COUPONS_EARNED,
    CENSUS_VALUE_MEOW_COUPONS_SPENT, CENSUS_VALUE_UNK_0x62C, CENSUS_VALUE_FISH_CAUGHT, CENSUS_VALUE_BUGS_CAUGHT,
    CENSUS_VALUE_SEA_CREATURES_CAUGHT, CENSUS_VALUE_FOSSILS_ANALYZED, CENSUS_VALUE_FLOWERS_WATERED, CENSUS_VALUE_WEEDS_PULLED,
    CENSUS_VALUE_FLOWERS_PLANTED, CENSUS_VALUE_GOLDEN_ROSES_CREATED, CENSUS_VALUE_BLUE_ROSES_CREATED, CENSUS_VALUE_TREES_PLANTED,
    CENSUS_VALUE_FRUIT_GROWN, CENSUS_VALUE_PERFECT_FRUIT_GROWN, CENSUS_VALUE_FRUITS_GROWN_ON_THE_BEACH, CENSUS_VALUE_TREES_CUT_DOWN,
    CENSUS_VALUE_FAMOUS_MUSHROOMS_EATEN, CENSUS_VALUE_ISLAND_VISITS, CENSUS_VALUE_TOUR_VISITS, CENSUS_VALUE_TURNIPS_BOUGHT,
    CENSUS_VALUE_TURNIPS_SOLD, CENSUS_VALUE_TURNIP_EXPENSES, CENSUS_VALUE_TURNIP_PROFITS, CENSUS_VALUE_LOST_ITEMS_GIVEN_BACK,
    CENSUS_VALUE_YOU_VISITED_VILLAGERS, CENSUS_VALUE_VILLAGERS_VISITED_YOU, CENSUS_VALUE_PLAYED_HIDE_AND_SEEK, CENSUS_VALUE_UNK_0x550,
    CENSUS_VALUE_UNK_0x564, CENSUS_VALUE_NPC_PICTURES_RECEIVED, CENSUS_VALUE_DJKK_VISITS, CENSUS_VALUE_KK_SLIDER_CONCERTS_ATTENDED,
    CENSUS_VALUE_SHRUNK_SKETCHES_ATTENDED, CENSUS_VALUE_SHAMPOODLE_VISITS, CENSUS_VALUE_JOBS_AT_THE_ROOST, CENSUS_VALUE_RESET_SURVEILLANCE_CENTER_VISITS,
    CENSUS_VALUE_BOUGHT_ART_AT_REDDS, CENSUS_VALUE_SAHARA_REDECORATIONS, CENSUS_VALUE_KATRINA_VISITS, CENSUS_VALUE_SHOOTING_STAR_WISHES,
    CENSUS_VALUE_HELPED_KATIE_TRAVEL, CENSUS_VALUE_HELPED_GULLIVER, CENSUS_VALUE_SCALLOPS_GIVEN_TO_PASCAL, CENSUS_VALUE_DREAM_TOWNS_VISITED,
    CENSUS_VALUE_STREET_PASS_VISITORS, CENSUS_VALUE_PRO_DESIGNS_CREATED, CENSUS_VALUE_BALLOONS_POPPED, CENSUS_VALUE_EARNED_BADGES,
    0, 0, 0, 0,
};
// the unit of each value (a message of STR_Kokusei_Unit; the texts are empty in the game)
// 0x00858740
const u32 s_CensusUnits[CENSUS_ROW_NUM] = {
    0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 2, 3,
    3, 3, 6, 4, 4, 4, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 0, 0, 0, 2, 0, 0, 2, 5,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 4, 4, 2,
    2, 3, 3, 7, 4, 2, 2, 4, 4, 4, 4, 4,
    4, 2, 2, 7, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 8, 8, 8, 2, 7, 7, 2, 0, 0, 0, 0,
};
// the title of each value (a message of STR_Kokusei_Title; the texts are empty in the game)
// 0x008585F0
const u32 s_CensusTitles[CENSUS_ROW_NUM] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
    24, 25, 26, 27, 28, 29, 58, 30, 31, 32, 33, 34,
    35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,
    47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 59,
    60, 61, 63, 64, 66, 67, 68, 69, 70, 71, 72, 73,
    74, 75, 76, 77, 78, 79, 80, 81, 0, 0, 0, 0,
};
} // namespace

// 0x005D10F8 (name is ours)
bool IsCensusValueShown(u32 index)
{
    switch (index) {
    case CENSUS_VALUE_DREAM_TOWNS_VISITED:
        return GetCensusOfGardenPlus()->GetDreamTownsVisited() > 0;
    case CENSUS_VALUE_KK_SLIDER_CONCERTS_ATTENDED:
        return GetCensusOfGardenPlus()->GetKkSliderConcertsAttended() > 0;
    case CENSUS_VALUE_SHRUNK_SKETCHES_ATTENDED:
        return GetCensusOfGardenPlus()->GetShrunkSketchesAttended() > 0;
    case CENSUS_VALUE_TOUR_VISITS:
        return GetCensusOfGardenPlus()->GetTourVisits() > 0;
    case CENSUS_VALUE_ISLAND_VISITS:
        return GetCensusOfGardenPlus()->GetIslandVisits() > 0;
    case CENSUS_VALUE_LETTERS_SENT:
        return GetCensusOfGardenPlus()->GetLettersSent() > 0;
    case CENSUS_VALUE_FURNITURE_CUSTOMIZED:
        return GetCensusOfGardenPlus()->GetFurnitureCustomized() > 0;
    case CENSUS_VALUE_GRACIES_EXPENSES:
        return GetCensusOfGardenPlus()->GetGraciesExpenses() > 0;
    case CENSUS_VALUE_GARDENING_SHOP_EXPENSES:
        return GetCensusOfGardenPlus()->GetGardeningShopExpenses() > 0;
    case CENSUS_VALUE_KICKS_EXPENSES:
        return GetCensusOfGardenPlus()->GetKicksExpenses() > 0;
    case CENSUS_VALUE_KATRINA_VISITS:
        return GetCensusOfGardenPlus()->GetKatrinaVisits() > 0;
    case CENSUS_VALUE_STREET_PASS_VISITORS:
        return GetCensusOfGardenPlus()->GetStreetPassVisitorsTotal() > 0;
    case CENSUS_VALUE_TOWNS_VISITED:
        return GetCensusOfGardenPlus()->GetTownsVisited() > 0;
    case CENSUS_VALUE_TOWN_VISITORS:
        return GetCensusOfGardenPlus()->GetTownVisitors() > 0;
    case CENSUS_VALUE_SAHARA_REDECORATIONS:
        return GetCensusOfGardenPlus()->GetSaharaRedecorations() > 0;
    case CENSUS_VALUE_JOBS_AT_THE_ROOST:
        return GetCensusOfGardenPlus()->GetJobsAtTheRoost() > 0;
    case CENSUS_VALUE_FOSSILS_ANALYZED:
        return GetCensusOfGardenPlus()->GetFossilsAnalyzed() > 0;
    case CENSUS_VALUE_PRO_DESIGNS_CREATED:
        return GetCensusOfGardenPlus()->GetProDesignsCreated() > 0;
    case CENSUS_VALUE_EARNED_BADGES:
        return GetCensusOfGardenPlus()->GetEarnedBadges() > 0;
    case CENSUS_VALUE_HELPED_GULLIVER:
        return GetCensusOfGardenPlus()->GetHelpedGulliver() > 0;
    case CENSUS_VALUE_SHAMPOODLE_VISITS:
        return GetCensusOfGardenPlus()->GetShampoodleVisits() > 0;
    case CENSUS_VALUE_BOUGHT_ART_AT_REDDS:
        return GetCensusOfGardenPlus()->GetBoughtArtAtRedds() > 0;
    case CENSUS_VALUE_SCALLOPS_GIVEN_TO_PASCAL:
        return GetCensusOfGardenPlus()->GetScallopsGivenToPascal() > 0;
    case CENSUS_VALUE_HELPED_KATIE_TRAVEL:
        return GetCensusOfGardenPlus()->GetHelpedKatieTravel() > 0;
    case CENSUS_VALUE_BALLOONS_POPPED:
        return GetCensusOfGardenPlus()->GetBalloonsPopped() > 0;
    case CENSUS_VALUE_SHOOTING_STAR_WISHES:
        return GetCensusOfGardenPlus()->GetShootingStarWishes() > 0;
    case CENSUS_VALUE_NPC_PICTURES_RECEIVED:
        return GetCensusOfGardenPlus()->GetNpcPicturesReceived() > 0;
    case CENSUS_VALUE_LOST_ITEMS_GIVEN_BACK:
        return GetCensusOfGardenPlus()->GetLostItemsGivenBack() > 0;
    case CENSUS_VALUE_YOU_VISITED_VILLAGERS:
        return GetCensusOfGardenPlus()->GetYouVisitedVillagers() > 0;
    case CENSUS_VALUE_VILLAGERS_VISITED_YOU:
        return GetCensusOfGardenPlus()->GetVillagersVisitedYou() > 0;
    case CENSUS_VALUE_PLAYED_HIDE_AND_SEEK:
        return GetCensusOfGardenPlus()->GetPlayedHideAndSeek() > 0;
    case CENSUS_VALUE_UNK_0x550:
        return GetCensusOfGardenPlus()->GetUnk0x550() > 0;
    case CENSUS_VALUE_UNK_0x564:
        return GetCensusOfGardenPlus()->GetUnk0x564() > 0;
    case CENSUS_VALUE_AMIIBO_SCANNED:
        return GetCensusOfGardenPlus()->GetAmiiboScanned() > 0;
    case CENSUS_VALUE_RESET_SURVEILLANCE_CENTER_VISITS:
        return GetCensusOfGardenPlus()->GetResetSurveillanceCenterVisits() > 0;
    case CENSUS_VALUE_DJKK_VISITS:
        return GetCensusOfGardenPlus()->GetDjkkVisits() > 0;
    case CENSUS_VALUE_GOLDEN_ROSES_CREATED:
        return GetCensusOfGardenPlus()->GetGoldenRosesCreatedTotal() > 0;
    case CENSUS_VALUE_BLUE_ROSES_CREATED:
        return GetCensusOfGardenPlus()->GetBlueRosesCreatedTotal() > 0;
    case CENSUS_VALUE_FAMOUS_MUSHROOMS_EATEN:
        return GetCensusOfGardenPlus()->GetFamousMushroomsEaten() > 0;
    case CENSUS_VALUE_TOURNEY_FISH_CAUGHT:
        return event::IsActive(event::EVENT_ID_FISHING_TOURNEY);
    case CENSUS_VALUE_TOURNEY_INSECT_CAUGHT:
        return event::IsActive(event::EVENT_ID_BUG_OFF);
    case CENSUS_VALUE_FESTIVAL_FEATHERS_CAUGHT:
        return event::IsActive(event::EVENT_ID_FESTIVALE);
    case CENSUS_VALUE_EGGS_GIVEN_TO_ZIPPER:
        return event::IsActive(event::EVENT_ID_BUNNY_DAY);
    case CENSUS_VALUE_FOUND_IMPOSTER_BLANCA:
        return event::IsActive(event::EVENT_ID_APRIL_FOOLS_DAY);
    case CENSUS_VALUE_FIREWORK_DESIGNS_GIVEN_TO_ISABELLE:
        return event::IsActive(event::EVENT_ID_FIREWORKS_SHOW);
    case CENSUS_VALUE_RECEIVED_CANDY_ON_HALLOWEEN:
        return event::IsActive(event::EVENT_ID_HALLOWEEN);
    case CENSUS_VALUE_HARVEST_FESTIVAL_COURSES_DONE:
        return event::IsActive(event::EVENT_ID_HARVEST_FESTIVAL);
    case CENSUS_VALUE_SNOWMEN_BUILT_BY_YOU_IN_TOWN:
        return Func63061C();
    case CENSUS_VALUE_GIVEN_TOY_DAY_PRESENTS:
        return event::IsActive(event::EVENT_ID_TOY_DAY);
    case CENSUS_VALUE_SNOWFLAKES_CAUGHT: {
        // in the winter months
        const s8 month = time::GetCurrentDate()->month;
        if (month > 1 && month != 12) {
            return false;
        }
        return GetCensusOfGardenPlus()->GetSnowflakesCaught() > 0;
    }
    case CENSUS_VALUE_PARTY_POPPERS_POPPED_AT_NEW_YEARS: {
        // on January 1 before 6:00 (the night of the countdown)
        const nn::fnd::DateTimeParameters* date = time::GetCurrentDate();
        return date->month == 1 && date->day == 1 && date->hour < 6;
    }
    case CENSUS_VALUE_TOTAL_BINGO_WINS:
        if (!Func63061C() && !Func226D48()) {
            return false;
        }
        return GetCensusOfGardenPlus()->GetTotalBingoWins() > 0;
    case CENSUS_VALUE_BELLS_EARNED:
    case CENSUS_VALUE_ABD_BALANCE:
    case CENSUS_VALUE_BELLS_SPENT:
    case CENSUS_VALUE_LOAN_PAID:
    case CENSUS_VALUE_TURNIPS_BOUGHT:
    case CENSUS_VALUE_TURNIPS_SOLD:
    case CENSUS_VALUE_TURNIP_EXPENSES:
    case CENSUS_VALUE_TURNIP_PROFITS:
    case CENSUS_VALUE_PUBLIC_WORK_EXPENSES:
    case CENSUS_VALUE_RECYCLE_SHOP_EARNINGS:
    case CENSUS_VALUE_FLOWERS_WATERED:
    case CENSUS_VALUE_FISH_CAUGHT:
    case CENSUS_VALUE_BUGS_CAUGHT:
    case CENSUS_VALUE_SEA_CREATURES_CAUGHT:
    case CENSUS_VALUE_PUBLIC_WORKS_BUILT:
    case CENSUS_VALUE_FRUIT_GROWN:
    case CENSUS_VALUE_PERFECT_FRUIT_GROWN:
    case CENSUS_VALUE_FRUITS_GROWN_ON_THE_BEACH:
    case CENSUS_VALUE_FLOWERS_PLANTED:
    case CENSUS_VALUE_TREES_PLANTED:
    case CENSUS_VALUE_TREES_CUT_DOWN:
    case CENSUS_VALUE_NOOKLINGS_EXPENSES:
    case CENSUS_VALUE_WEEDS_PULLED:
    case CENSUS_VALUE_ABLE_SISTERS_EXPENSES:
    case CENSUS_VALUE_TOWN_INITIATIVES_COMPLETED:
    case CENSUS_VALUE_MEOW_COUPONS_EARNED:
    case CENSUS_VALUE_MEOW_COUPONS_SPENT:
    case CENSUS_VALUE_UNK_0x62C:
    case CENSUS_VALUE_NUM:
    case CENSUS_VALUE_NUM + 1:
    case CENSUS_VALUE_NUM + 2:
    case CENSUS_VALUE_NUM + 3:
        return true;
    default:
        return false;
    }
}

// 0x005D1908 (name is ours)
u8 GetCensusValueOfRow(u32 row)
{
    return s_CensusRowValues[row];
}

// 0x005D1918 (name is ours)
s32 GetCensusValueForSlot(u32 index, u32 townSlot)
{
    switch (index) {
    case CENSUS_VALUE_BELLS_EARNED:
        return GetCensusOfGardenPlus()->GetBellsEarnedForSlot(townSlot);
    case CENSUS_VALUE_ABD_BALANCE:
        return GetCensusOfGardenPlus()->GetAbdBalanceForSlot(townSlot);
    case CENSUS_VALUE_BELLS_SPENT:
        return GetCensusOfGardenPlus()->GetBellsSpentForSlot(townSlot);
    case CENSUS_VALUE_LOAN_PAID:
        return GetCensusOfGardenPlus()->GetLoanPaidForSlot(townSlot);
    case CENSUS_VALUE_TURNIPS_BOUGHT:
        return GetCensusOfGardenPlus()->GetTurnipsBoughtForSlot(townSlot);
    case CENSUS_VALUE_TURNIPS_SOLD:
        return GetCensusOfGardenPlus()->GetTurnipsSoldForSlot(townSlot);
    case CENSUS_VALUE_TURNIP_EXPENSES:
        return GetCensusOfGardenPlus()->GetTurnipExpensesForSlot(townSlot);
    case CENSUS_VALUE_TURNIP_PROFITS:
        return GetCensusOfGardenPlus()->GetTurnipProfitsForSlot(townSlot);
    case CENSUS_VALUE_PUBLIC_WORK_EXPENSES:
        return GetCensusOfGardenPlus()->GetPublicWorkExpensesForSlot(townSlot);
    case CENSUS_VALUE_RECYCLE_SHOP_EARNINGS:
        return GetCensusOfGardenPlus()->GetRecycleShopEarningsForSlot(townSlot);
    case CENSUS_VALUE_FLOWERS_WATERED:
        return GetCensusOfGardenPlus()->GetFlowersWateredForSlot(townSlot);
    case CENSUS_VALUE_FISH_CAUGHT:
        return GetCensusOfGardenPlus()->GetFishCaughtForSlot(townSlot);
    case CENSUS_VALUE_BUGS_CAUGHT:
        return GetCensusOfGardenPlus()->GetBugsCaughtForSlot(townSlot);
    case CENSUS_VALUE_SEA_CREATURES_CAUGHT:
        return GetCensusOfGardenPlus()->GetSeaCreaturesCaughtForSlot(townSlot);
    case CENSUS_VALUE_FLOWERS_PLANTED:
        return GetCensusOfGardenPlus()->GetFlowersPlantedForSlot(townSlot);
    case CENSUS_VALUE_TREES_PLANTED:
        return GetCensusOfGardenPlus()->GetTreesPlantedForSlot(townSlot);
    case CENSUS_VALUE_TREES_CUT_DOWN:
        return GetCensusOfGardenPlus()->GetTreesCutDownForSlot(townSlot);
    case CENSUS_VALUE_DREAM_TOWNS_VISITED:
        return GetCensusOfGardenPlus()->GetDreamTownsVisitedForSlot(townSlot);
    case CENSUS_VALUE_KK_SLIDER_CONCERTS_ATTENDED:
        return GetCensusOfGardenPlus()->GetKkSliderConcertsAttendedForSlot(townSlot);
    case CENSUS_VALUE_SHRUNK_SKETCHES_ATTENDED:
        return GetCensusOfGardenPlus()->GetShrunkSketchesAttendedForSlot(townSlot);
    case CENSUS_VALUE_TOUR_VISITS:
        return GetCensusOfGardenPlus()->GetTourVisitsForSlot(townSlot);
    case CENSUS_VALUE_ISLAND_VISITS:
        return GetCensusOfGardenPlus()->GetIslandVisitsForSlot(townSlot);
    case CENSUS_VALUE_LETTERS_SENT:
        return GetCensusOfGardenPlus()->GetLettersSentForSlot(townSlot);
    case CENSUS_VALUE_FURNITURE_CUSTOMIZED:
        return GetCensusOfGardenPlus()->GetFurnitureCustomizedForSlot(townSlot);
    case CENSUS_VALUE_NOOKLINGS_EXPENSES:
        return GetCensusOfGardenPlus()->GetNooklingsExpensesForSlot(townSlot);
    case CENSUS_VALUE_GRACIES_EXPENSES:
        return GetCensusOfGardenPlus()->GetGraciesExpensesForSlot(townSlot);
    case CENSUS_VALUE_GARDENING_SHOP_EXPENSES:
        return GetCensusOfGardenPlus()->GetGardeningShopExpensesForSlot(townSlot);
    case CENSUS_VALUE_WEEDS_PULLED:
        return GetCensusOfGardenPlus()->GetWeedsPulledForSlot(townSlot);
    case CENSUS_VALUE_ABLE_SISTERS_EXPENSES:
        return GetCensusOfGardenPlus()->GetAbleSistersExpensesForSlot(townSlot);
    case CENSUS_VALUE_KICKS_EXPENSES:
        return GetCensusOfGardenPlus()->GetKicksExpensesForSlot(townSlot);
    case CENSUS_VALUE_KATRINA_VISITS:
        return GetCensusOfGardenPlus()->GetKatrinaVisitsForSlot(townSlot);
    case CENSUS_VALUE_TOWNS_VISITED:
        return GetCensusOfGardenPlus()->GetTownsVisitedForSlot(townSlot);
    case CENSUS_VALUE_TOWN_VISITORS:
        return GetCensusOfGardenPlus()->GetTownVisitorsForSlot(townSlot);
    case CENSUS_VALUE_SAHARA_REDECORATIONS:
        return GetCensusOfGardenPlus()->GetSaharaRedecorationsForSlot(townSlot);
    case CENSUS_VALUE_JOBS_AT_THE_ROOST:
        return GetCensusOfGardenPlus()->GetJobsAtTheRoostForSlot(townSlot);
    case CENSUS_VALUE_FOSSILS_ANALYZED:
        return GetCensusOfGardenPlus()->GetFossilsAnalyzedForSlot(townSlot);
    case CENSUS_VALUE_PRO_DESIGNS_CREATED:
        return GetCensusOfGardenPlus()->GetProDesignsCreatedForSlot(townSlot);
    case CENSUS_VALUE_EARNED_BADGES:
        return GetCensusOfGardenPlus()->GetEarnedBadgesForSlot(townSlot);
    case CENSUS_VALUE_HELPED_GULLIVER:
        return GetCensusOfGardenPlus()->GetHelpedGulliverForSlot(townSlot);
    case CENSUS_VALUE_SHAMPOODLE_VISITS:
        return GetCensusOfGardenPlus()->GetShampoodleVisitsForSlot(townSlot);
    case CENSUS_VALUE_BOUGHT_ART_AT_REDDS:
        return GetCensusOfGardenPlus()->GetBoughtArtAtReddsForSlot(townSlot);
    case CENSUS_VALUE_SCALLOPS_GIVEN_TO_PASCAL:
        return GetCensusOfGardenPlus()->GetScallopsGivenToPascalForSlot(townSlot);
    case CENSUS_VALUE_HELPED_KATIE_TRAVEL:
        return GetCensusOfGardenPlus()->GetHelpedKatieTravelForSlot(townSlot);
    case CENSUS_VALUE_BALLOONS_POPPED:
        return GetCensusOfGardenPlus()->GetBalloonsPoppedForSlot(townSlot);
    case CENSUS_VALUE_TOURNEY_FISH_CAUGHT:
        return GetCensusOfGardenPlus()->GetTourneyFishCaughtForSlot(townSlot);
    case CENSUS_VALUE_TOURNEY_INSECT_CAUGHT:
        return GetCensusOfGardenPlus()->GetTourneyInsectCaughtForSlot(townSlot);
    case CENSUS_VALUE_FESTIVAL_FEATHERS_CAUGHT:
        return GetCensusOfGardenPlus()->GetFestivalFeathersCaughtForSlot(townSlot);
    case CENSUS_VALUE_EGGS_GIVEN_TO_ZIPPER:
        return GetCensusOfGardenPlus()->GetEggsGivenToZipperForSlot(townSlot);
    case CENSUS_VALUE_FOUND_IMPOSTER_BLANCA:
        return GetCensusOfGardenPlus()->GetFoundImposterBlancaForSlot(townSlot);
    case CENSUS_VALUE_FIREWORK_DESIGNS_GIVEN_TO_ISABELLE:
        return GetCensusOfGardenPlus()->GetFireworkDesignsGivenToIsabelleForSlot(townSlot);
    case CENSUS_VALUE_RECEIVED_CANDY_ON_HALLOWEEN:
        return GetCensusOfGardenPlus()->GetReceivedCandyOnHalloweenForSlot(townSlot);
    case CENSUS_VALUE_HARVEST_FESTIVAL_COURSES_DONE:
        return GetCensusOfGardenPlus()->GetHarvestFestivalCoursesDoneForSlot(townSlot);
    case CENSUS_VALUE_SNOWMEN_BUILT_BY_YOU_IN_TOWN:
        return GetCensusOfGardenPlus()->GetSnowmenBuiltByYouInTownForSlot(townSlot);
    case CENSUS_VALUE_GIVEN_TOY_DAY_PRESENTS:
        return GetCensusOfGardenPlus()->GetGivenToyDayPresentsForSlot(townSlot);
    case CENSUS_VALUE_SNOWFLAKES_CAUGHT:
        return GetCensusOfGardenPlus()->GetSnowflakesCaughtForSlot(townSlot);
    case CENSUS_VALUE_PARTY_POPPERS_POPPED_AT_NEW_YEARS:
        return GetCensusOfGardenPlus()->GetPartyPoppersPoppedAtNewYearsForSlot(townSlot);
    case CENSUS_VALUE_TOTAL_BINGO_WINS:
        return GetCensusOfGardenPlus()->GetTotalBingoWinsForSlot(townSlot);
    case CENSUS_VALUE_SHOOTING_STAR_WISHES:
        return GetCensusOfGardenPlus()->GetShootingStarWishesForSlot(townSlot);
    case CENSUS_VALUE_NPC_PICTURES_RECEIVED:
        return GetCensusOfGardenPlus()->GetNpcPicturesReceivedForSlot(townSlot);
    case CENSUS_VALUE_LOST_ITEMS_GIVEN_BACK:
        return GetCensusOfGardenPlus()->GetLostItemsGivenBackForSlot(townSlot);
    case CENSUS_VALUE_YOU_VISITED_VILLAGERS:
        return GetCensusOfGardenPlus()->GetYouVisitedVillagersForSlot(townSlot);
    case CENSUS_VALUE_VILLAGERS_VISITED_YOU:
        return GetCensusOfGardenPlus()->GetVillagersVisitedYouForSlot(townSlot);
    case CENSUS_VALUE_PLAYED_HIDE_AND_SEEK:
        return GetCensusOfGardenPlus()->GetPlayedHideAndSeekForSlot(townSlot);
    case CENSUS_VALUE_UNK_0x550:
        return GetCensusOfGardenPlus()->GetUnk0x550ForSlot(townSlot);
    case CENSUS_VALUE_UNK_0x564:
        return GetCensusOfGardenPlus()->GetUnk0x564ForSlot(townSlot);
    case CENSUS_VALUE_AMIIBO_SCANNED:
        return GetCensusOfGardenPlus()->GetAmiiboScannedForSlot(townSlot);
    case CENSUS_VALUE_RESET_SURVEILLANCE_CENTER_VISITS:
        return GetCensusOfGardenPlus()->GetResetSurveillanceCenterVisitsForSlot(townSlot);
    case CENSUS_VALUE_DJKK_VISITS:
        return GetCensusOfGardenPlus()->GetDjkkVisitsForSlot(townSlot);
    case CENSUS_VALUE_FAMOUS_MUSHROOMS_EATEN:
        return GetCensusOfGardenPlus()->GetFamousMushroomsEatenForSlot(townSlot);
    case CENSUS_VALUE_TOWN_INITIATIVES_COMPLETED:
        return GetCensusOfGardenPlus()->GetTownInitiativesCompletedForSlot(townSlot);
    case CENSUS_VALUE_MEOW_COUPONS_EARNED:
        return GetCensusOfGardenPlus()->GetMeowCouponsEarnedForSlot(townSlot);
    case CENSUS_VALUE_MEOW_COUPONS_SPENT:
        return GetCensusOfGardenPlus()->GetMeowCouponsSpentForSlot(townSlot);
    default:
        // (armlink replaced the tail call by a nop: the code falls into GetCensusValueTotal)
        return GetCensusValueTotal(index);
    }
}

// 0x005D2010 (name is ours)
s32 GetCensusValueTotal(u32 index)
{
    switch (index) {
    case CENSUS_VALUE_BELLS_EARNED:
        return GetCensusOfGardenPlus()->GetBellsEarnedTotal();
    case CENSUS_VALUE_ABD_BALANCE:
        return GetCensusOfGardenPlus()->GetAbdBalanceTotal();
    case CENSUS_VALUE_BELLS_SPENT:
        return GetCensusOfGardenPlus()->GetBellsSpentTotal();
    case CENSUS_VALUE_LOAN_PAID:
        return GetCensusOfGardenPlus()->GetLoanPaidTotal();
    case CENSUS_VALUE_TURNIPS_BOUGHT:
        return GetCensusOfGardenPlus()->GetTurnipsBoughtTotal();
    case CENSUS_VALUE_TURNIPS_SOLD:
        return GetCensusOfGardenPlus()->GetTurnipsSoldTotal();
    case CENSUS_VALUE_TURNIP_EXPENSES:
        return GetCensusOfGardenPlus()->GetTurnipExpensesTotal();
    case CENSUS_VALUE_TURNIP_PROFITS:
        return GetCensusOfGardenPlus()->GetTurnipProfitsTotal();
    case CENSUS_VALUE_PUBLIC_WORK_EXPENSES:
        return GetCensusOfGardenPlus()->GetPublicWorkExpensesTotal();
    case CENSUS_VALUE_RECYCLE_SHOP_EARNINGS:
        return GetCensusOfGardenPlus()->GetRecycleShopEarningsTotal();
    case CENSUS_VALUE_FLOWERS_WATERED:
        return GetCensusOfGardenPlus()->GetFlowersWateredTotal();
    case CENSUS_VALUE_FISH_CAUGHT:
        return GetCensusOfGardenPlus()->GetFishCaughtTotal();
    case CENSUS_VALUE_BUGS_CAUGHT:
        return GetCensusOfGardenPlus()->GetBugsCaughtTotal();
    case CENSUS_VALUE_SEA_CREATURES_CAUGHT:
        return GetCensusOfGardenPlus()->GetSeaCreaturesCaughtTotal();
    case CENSUS_VALUE_PUBLIC_WORKS_BUILT:
        return GetCensusOfGardenPlus()->GetPublicWorksBuiltTotal();
    case CENSUS_VALUE_FRUIT_GROWN:
        return GetCensusOfGardenPlus()->GetFruitGrownTotal();
    case CENSUS_VALUE_PERFECT_FRUIT_GROWN:
        return GetCensusOfGardenPlus()->GetPerfectFruitGrownTotal();
    case CENSUS_VALUE_FRUITS_GROWN_ON_THE_BEACH:
        return GetCensusOfGardenPlus()->GetFruitsGrownOnTheBeachTotal();
    case CENSUS_VALUE_FLOWERS_PLANTED:
        return GetCensusOfGardenPlus()->GetFlowersPlantedTotal();
    case CENSUS_VALUE_TREES_PLANTED:
        return GetCensusOfGardenPlus()->GetTreesPlantedTotal();
    case CENSUS_VALUE_TREES_CUT_DOWN:
        return GetCensusOfGardenPlus()->GetTreesCutDownTotal();
    case CENSUS_VALUE_DREAM_TOWNS_VISITED:
        return GetCensusOfGardenPlus()->GetDreamTownsVisitedTotal();
    case CENSUS_VALUE_KK_SLIDER_CONCERTS_ATTENDED:
        return GetCensusOfGardenPlus()->GetKkSliderConcertsAttendedTotal();
    case CENSUS_VALUE_SHRUNK_SKETCHES_ATTENDED:
        return GetCensusOfGardenPlus()->GetShrunkSketchesAttendedTotal();
    case CENSUS_VALUE_TOUR_VISITS:
        return GetCensusOfGardenPlus()->GetTourVisitsTotal();
    case CENSUS_VALUE_ISLAND_VISITS:
        return GetCensusOfGardenPlus()->GetIslandVisitsTotal();
    case CENSUS_VALUE_LETTERS_SENT:
        return GetCensusOfGardenPlus()->GetLettersSentTotal();
    case CENSUS_VALUE_FURNITURE_CUSTOMIZED:
        return GetCensusOfGardenPlus()->GetFurnitureCustomizedTotal();
    case CENSUS_VALUE_NOOKLINGS_EXPENSES:
        return GetCensusOfGardenPlus()->GetNooklingsExpensesTotal();
    case CENSUS_VALUE_GRACIES_EXPENSES:
        return GetCensusOfGardenPlus()->GetGraciesExpensesTotal();
    case CENSUS_VALUE_GARDENING_SHOP_EXPENSES:
        return GetCensusOfGardenPlus()->GetGardeningShopExpensesTotal();
    case CENSUS_VALUE_WEEDS_PULLED:
        return GetCensusOfGardenPlus()->GetWeedsPulledTotal();
    case CENSUS_VALUE_ABLE_SISTERS_EXPENSES:
        return GetCensusOfGardenPlus()->GetAbleSistersExpensesTotal();
    case CENSUS_VALUE_KICKS_EXPENSES:
        return GetCensusOfGardenPlus()->GetKicksExpensesTotal();
    case CENSUS_VALUE_KATRINA_VISITS:
        return GetCensusOfGardenPlus()->GetKatrinaVisitsTotal();
    case CENSUS_VALUE_STREET_PASS_VISITORS:
        return GetCensusOfGardenPlus()->GetStreetPassVisitorsTotal();
    case CENSUS_VALUE_TOWNS_VISITED:
        return GetCensusOfGardenPlus()->GetTownsVisitedTotal();
    case CENSUS_VALUE_TOWN_VISITORS:
        return GetCensusOfGardenPlus()->GetTownVisitorsTotal();
    case CENSUS_VALUE_SAHARA_REDECORATIONS:
        return GetCensusOfGardenPlus()->GetSaharaRedecorationsTotal();
    case CENSUS_VALUE_JOBS_AT_THE_ROOST:
        return GetCensusOfGardenPlus()->GetJobsAtTheRoostTotal();
    case CENSUS_VALUE_FOSSILS_ANALYZED:
        return GetCensusOfGardenPlus()->GetFossilsAnalyzedTotal();
    case CENSUS_VALUE_PRO_DESIGNS_CREATED:
        return GetCensusOfGardenPlus()->GetProDesignsCreatedTotal();
    case CENSUS_VALUE_EARNED_BADGES:
        return GetCensusOfGardenPlus()->GetEarnedBadgesTotal();
    case CENSUS_VALUE_HELPED_GULLIVER:
        return GetCensusOfGardenPlus()->GetHelpedGulliverTotal();
    case CENSUS_VALUE_SHAMPOODLE_VISITS:
        return GetCensusOfGardenPlus()->GetShampoodleVisitsTotal();
    case CENSUS_VALUE_BOUGHT_ART_AT_REDDS:
        return GetCensusOfGardenPlus()->GetBoughtArtAtReddsTotal();
    case CENSUS_VALUE_SCALLOPS_GIVEN_TO_PASCAL:
        return GetCensusOfGardenPlus()->GetScallopsGivenToPascalTotal();
    case CENSUS_VALUE_HELPED_KATIE_TRAVEL:
        return GetCensusOfGardenPlus()->GetHelpedKatieTravelTotal();
    case CENSUS_VALUE_BALLOONS_POPPED:
        return GetCensusOfGardenPlus()->GetBalloonsPoppedTotal();
    case CENSUS_VALUE_TOURNEY_FISH_CAUGHT:
        return GetCensusOfGardenPlus()->GetTourneyFishCaughtTotal();
    case CENSUS_VALUE_TOURNEY_INSECT_CAUGHT:
        return GetCensusOfGardenPlus()->GetTourneyInsectCaughtTotal();
    case CENSUS_VALUE_FESTIVAL_FEATHERS_CAUGHT:
        return GetCensusOfGardenPlus()->GetFestivalFeathersCaughtTotal();
    case CENSUS_VALUE_EGGS_GIVEN_TO_ZIPPER:
        return GetCensusOfGardenPlus()->GetEggsGivenToZipperTotal();
    case CENSUS_VALUE_FOUND_IMPOSTER_BLANCA:
        return GetCensusOfGardenPlus()->GetFoundImposterBlancaTotal();
    case CENSUS_VALUE_FIREWORK_DESIGNS_GIVEN_TO_ISABELLE:
        return GetCensusOfGardenPlus()->GetFireworkDesignsGivenToIsabelleTotal();
    case CENSUS_VALUE_RECEIVED_CANDY_ON_HALLOWEEN:
        return GetCensusOfGardenPlus()->GetReceivedCandyOnHalloweenTotal();
    case CENSUS_VALUE_HARVEST_FESTIVAL_COURSES_DONE:
        return GetCensusOfGardenPlus()->GetHarvestFestivalCoursesDoneTotal();
    case CENSUS_VALUE_SNOWMEN_BUILT_BY_YOU_IN_TOWN:
        return GetCensusOfGardenPlus()->GetSnowmenBuiltByYouInTownTotal();
    case CENSUS_VALUE_GIVEN_TOY_DAY_PRESENTS:
        return GetCensusOfGardenPlus()->GetGivenToyDayPresentsTotal();
    case CENSUS_VALUE_SNOWFLAKES_CAUGHT:
        return GetCensusOfGardenPlus()->GetSnowflakesCaughtTotal();
    case CENSUS_VALUE_PARTY_POPPERS_POPPED_AT_NEW_YEARS:
        return GetCensusOfGardenPlus()->GetPartyPoppersPoppedAtNewYearsTotal();
    case CENSUS_VALUE_TOTAL_BINGO_WINS:
        return GetCensusOfGardenPlus()->GetTotalBingoWinsTotal();
    case CENSUS_VALUE_SHOOTING_STAR_WISHES:
        return GetCensusOfGardenPlus()->GetShootingStarWishesTotal();
    case CENSUS_VALUE_NPC_PICTURES_RECEIVED:
        return GetCensusOfGardenPlus()->GetNpcPicturesReceivedTotal();
    case CENSUS_VALUE_LOST_ITEMS_GIVEN_BACK:
        return GetCensusOfGardenPlus()->GetLostItemsGivenBackTotal();
    case CENSUS_VALUE_YOU_VISITED_VILLAGERS:
        return GetCensusOfGardenPlus()->GetYouVisitedVillagersTotal();
    case CENSUS_VALUE_VILLAGERS_VISITED_YOU:
        return GetCensusOfGardenPlus()->GetVillagersVisitedYouTotal();
    case CENSUS_VALUE_PLAYED_HIDE_AND_SEEK:
        return GetCensusOfGardenPlus()->GetPlayedHideAndSeekTotal();
    case CENSUS_VALUE_UNK_0x550:
        return GetCensusOfGardenPlus()->GetUnk0x550Total();
    case CENSUS_VALUE_UNK_0x564:
        return GetCensusOfGardenPlus()->GetUnk0x564Total();
    case CENSUS_VALUE_AMIIBO_SCANNED:
        return GetCensusOfGardenPlus()->GetAmiiboScannedTotal();
    case CENSUS_VALUE_RESET_SURVEILLANCE_CENTER_VISITS:
        return GetCensusOfGardenPlus()->GetResetSurveillanceCenterVisitsTotal();
    case CENSUS_VALUE_DJKK_VISITS:
        return GetCensusOfGardenPlus()->GetDjkkVisitsTotal();
    case CENSUS_VALUE_GOLDEN_ROSES_CREATED:
        return GetCensusOfGardenPlus()->GetGoldenRosesCreatedTotal();
    case CENSUS_VALUE_BLUE_ROSES_CREATED:
        return GetCensusOfGardenPlus()->GetBlueRosesCreatedTotal();
    case CENSUS_VALUE_FAMOUS_MUSHROOMS_EATEN:
        return GetCensusOfGardenPlus()->GetFamousMushroomsEatenTotal();
    case CENSUS_VALUE_TOWN_INITIATIVES_COMPLETED:
        return GetCensusOfGardenPlus()->GetTownInitiativesCompletedTotal();
    case CENSUS_VALUE_MEOW_COUPONS_EARNED:
        return GetCensusOfGardenPlus()->GetMeowCouponsEarnedTotal();
    case CENSUS_VALUE_MEOW_COUPONS_SPENT:
        return GetCensusOfGardenPlus()->GetMeowCouponsSpentTotal();
    case CENSUS_VALUE_UNK_0x62C:
        return GetCensusOfGardenPlus()->GetUnk0x62CTotal();
    default:
        return 0;
    }
}

// 0x005D27A8 (name is ours)
u32 GetCensusUnitIndex(u32 index)
{
    if (index < CENSUS_ROW_NUM) {
        return s_CensusUnits[index];
    }
    return 0;
}

// 0x005D27C0 (name is ours)
u32 GetCensusTitleIndex(u32 index)
{
    if (index < CENSUS_ROW_NUM) {
        return s_CensusTitles[index];
    }
    return 0;
}
