// The census of the town (statistics of all players; the census menu is unused in the game, see docs/census.md): one
// function per value and kind of change. All names are ours, the values are named after the research of the save data.

#include "Sv/dSvCensus.h"

#include <cstring>

#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "netgame/netgame_Api.h"

namespace {
// (inline) value + add, kept in [0, max] (the stored value counts as signed)
DECOMP_ALWAYS_INLINE u32 AddCapped(u32 value, s32 add, u32 max)
{
    const s64 sum = static_cast<s64>(static_cast<s32>(value)) + add;
    if (sum < 0) {
        return 0;
    }
    if (sum > static_cast<s64>(max)) {
        return max;
    }
    return static_cast<u32>(sum);
}

// (inline) a value kept in [0, max]
DECOMP_ALWAYS_INLINE u32 Clamp(s32 value, u32 max)
{
    if (value < 0) {
        return 0;
    }
    return static_cast<s32>(max) >= value ? value : max;
}
} // namespace

// 0x006B59E8 (name is ours)
void SvCensus::ClearPlayer(u32 townSlot)
{
    bellsEarned.playerStats[townSlot] = 0;
    abdBalance.playerStats[townSlot] = 0;
    bellsSpent.playerStats[townSlot] = 0;
    loanPaid.playerStats[townSlot] = 0;
    turnipsBought.playerStats[townSlot] = 0;
    turnipsSold.playerStats[townSlot] = 0;
    turnipExpenses.playerStats[townSlot] = 0;
    turnipProfits.playerStats[townSlot] = 0;
    publicWorkExpenses.playerStats[townSlot] = 0;
    recycleShopEarnings.playerStats[townSlot] = 0;
    flowersWatered.playerStats[townSlot] = 0;
    fishCaught.playerStats[townSlot] = 0;
    bugsCaught.playerStats[townSlot] = 0;
    seaCreaturesCaught.playerStats[townSlot] = 0;
    publicWorksBuilt.playerStats[townSlot] = 0;
    fruitGrown.playerStats[townSlot] = 0;
    perfectFruitGrown.playerStats[townSlot] = 0;
    fruitsGrownOnTheBeach.playerStats[townSlot] = 0;
    flowersPlanted.playerStats[townSlot] = 0;
    treesPlanted.playerStats[townSlot] = 0;
    treesCutDown.playerStats[townSlot] = 0;
    dreamTownsVisited.playerStats[townSlot] = 0;
    kkSliderConcertsAttended.playerStats[townSlot] = 0;
    shrunkSketchesAttended.playerStats[townSlot] = 0;
    tourVisits.playerStats[townSlot] = 0;
    islandVisits.playerStats[townSlot] = 0;
    lettersSent.playerStats[townSlot] = 0;
    furnitureCustomized.playerStats[townSlot] = 0;
    nooklingsExpenses.playerStats[townSlot] = 0;
    graciesExpenses.playerStats[townSlot] = 0;
    gardeningShopExpenses.playerStats[townSlot] = 0;
    weedsPulled.playerStats[townSlot] = 0;
    ableSistersExpenses.playerStats[townSlot] = 0;
    kicksExpenses.playerStats[townSlot] = 0;
    katrinaVisits.playerStats[townSlot] = 0;
    streetPassVisitors.playerStats[townSlot] = 0;
    townsVisited.playerStats[townSlot] = 0;
    townVisitors.playerStats[townSlot] = 0;
    saharaRedecorations.playerStats[townSlot] = 0;
    jobsAtTheRoost.playerStats[townSlot] = 0;
    fossilsAnalyzed.playerStats[townSlot] = 0;
    proDesignsCreated.playerStats[townSlot] = 0;
    earnedBadges.playerStats[townSlot] = 0;
    helpedGulliver.playerStats[townSlot] = 0;
    shampoodleVisits.playerStats[townSlot] = 0;
    boughtArtAtRedds.playerStats[townSlot] = 0;
    scallopsGivenToPascal.playerStats[townSlot] = 0;
    helpedKatieTravel.playerStats[townSlot] = 0;
    balloonsPopped.playerStats[townSlot] = 0;
    tourneyFishCaught.playerStats[townSlot] = 0;
    tourneyInsectCaught.playerStats[townSlot] = 0;
    festivalFeathersCaught.playerStats[townSlot] = 0;
    eggsGivenToZipper.playerStats[townSlot] = 0;
    foundImposterBlanca.playerStats[townSlot] = 0;
    fireworkDesignsGivenToIsabelle.playerStats[townSlot] = 0;
    receivedCandyOnHalloween.playerStats[townSlot] = 0;
    harvestFestivalCoursesDone.playerStats[townSlot] = 0;
    snowmenBuiltByYouInTown.playerStats[townSlot] = 0;
    givenToyDayPresents.playerStats[townSlot] = 0;
    snowflakesCaught.playerStats[townSlot] = 0;
    partyPoppersPoppedAtNewYears.playerStats[townSlot] = 0;
    totalBingoWins.playerStats[townSlot] = 0;
    shootingStarWishes.playerStats[townSlot] = 0;
    npcPicturesReceived.playerStats[townSlot] = 0;
    lostItemsGivenBack.playerStats[townSlot] = 0;
    youVisitedVillagers.playerStats[townSlot] = 0;
    villagersVisitedYou.playerStats[townSlot] = 0;
    playedHideAndSeek.playerStats[townSlot] = 0;
    unk_0x550.playerStats[townSlot] = 0;
    unk_0x564.playerStats[townSlot] = 0;
    amiiboScanned.playerStats[townSlot] = 0;
    resetSurveillanceCenterVisits.playerStats[townSlot] = 0;
    djkkVisits.playerStats[townSlot] = 0;
    goldenRosesCreated.playerStats[townSlot] = 0;
    blueRosesCreated.playerStats[townSlot] = 0;
    famousMushroomsEaten.playerStats[townSlot] = 0;
    townInitiativesCompleted.playerStats[townSlot] = 0;
    meowCouponsEarned.playerStats[townSlot] = 0;
    meowCouponsSpent.playerStats[townSlot] = 0;
    unk_0x62C.playerStats[townSlot] = 0;
    for (u32 i = 0; i < INSECT_NUM; i++) {
        insectCatchCountsPerPlayer[i][townSlot] = 0;
    }
    for (u32 i = 0; i < FISH_NUM; i++) {
        fishCatchCountsPerPlayer[i][townSlot] = 0;
    }
    for (u32 i = 0; i < JUNK_NUM; i++) {
        junkCatchCountsPerPlayer[i][townSlot] = 0;
    }
    for (u32 i = 0; i < SEA_CREATURE_NUM; i++) {
        seaCreatureCatchCountsPerPlayer[i][townSlot] = 0;
    }
}

// 0x006B5BF0 (name is ours)
void SvCensus::AddJunkCatchTotal(u32 index, s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    junkCatchCounts[index] = AddCapped(junkCatchCounts[index], value, CENSUS_COUNT_MAX);
}

// 0x006B5C4C (name is ours)
void SvCensus::AddFishCatchTotal(u32 index, s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    fishCatchCounts[index] = AddCapped(fishCatchCounts[index], value, CENSUS_COUNT_MAX);
}

// 0x006B5CA4 (name is ours)
void SvCensus::AddInsectCatchTotal(u32 index, s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    insectCatchCounts[index] = AddCapped(insectCatchCounts[index], value, CENSUS_COUNT_MAX);
}

// 0x006B5CFC (name is ours)
void SvCensus::ClearTotalBingoWins()
{
    totalBingoWins.totalPlayerStat = 0;
    totalBingoWins.playerStats[0] = 0;
    totalBingoWins.playerStats[1] = 0;
    totalBingoWins.playerStats[2] = 0;
    totalBingoWins.playerStats[3] = 0;
}

// 0x006B5D18 (name is ours)
void SvCensus::ClearEggsGivenToZipper()
{
    eggsGivenToZipper.totalPlayerStat = 0;
    eggsGivenToZipper.playerStats[0] = 0;
    eggsGivenToZipper.playerStats[1] = 0;
    eggsGivenToZipper.playerStats[2] = 0;
    eggsGivenToZipper.playerStats[3] = 0;
}

// 0x006B5D34 (name is ours)
void SvCensus::AddSeaCreatureCatchTotal(u32 index, s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    seaCreatureCatchCounts[index] = AddCapped(seaCreatureCatchCounts[index], value, CENSUS_COUNT_MAX);
}

// 0x006B5D90 (name is ours)
void SvCensus::AddStreetPassVisitorsTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    streetPassVisitors.totalPlayerStat = AddCapped(streetPassVisitors.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B5DE0 (name is ours)
void SvCensus::AddTreesCutDownTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    treesCutDown.totalPlayerStat = AddCapped(treesCutDown.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B5E30 (name is ours)
void SvCensus::ClearPartyPoppersPoppedAtNewYears()
{
    partyPoppersPoppedAtNewYears.totalPlayerStat = 0;
    partyPoppersPoppedAtNewYears.playerStats[0] = 0;
    partyPoppersPoppedAtNewYears.playerStats[1] = 0;
    partyPoppersPoppedAtNewYears.playerStats[2] = 0;
    partyPoppersPoppedAtNewYears.playerStats[3] = 0;
}

// 0x006B5E4C (name is ours)
void SvCensus::ClearHarvestFestivalCoursesDone()
{
    harvestFestivalCoursesDone.totalPlayerStat = 0;
    harvestFestivalCoursesDone.playerStats[0] = 0;
    harvestFestivalCoursesDone.playerStats[1] = 0;
    harvestFestivalCoursesDone.playerStats[2] = 0;
    harvestFestivalCoursesDone.playerStats[3] = 0;
}

// 0x006B5E68 (name is ours)
void SvCensus::ClearSnowmenBuiltByYouInTown()
{
    snowmenBuiltByYouInTown.totalPlayerStat = 0;
    snowmenBuiltByYouInTown.playerStats[0] = 0;
    snowmenBuiltByYouInTown.playerStats[1] = 0;
    snowmenBuiltByYouInTown.playerStats[2] = 0;
    snowmenBuiltByYouInTown.playerStats[3] = 0;
}

// 0x006B5E84 (name is ours)
void SvCensus::AddJobsAtTheRoostTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    jobsAtTheRoost.totalPlayerStat = AddCapped(jobsAtTheRoost.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B5ED4 (name is ours)
void SvCensus::AddDjkkVisitsTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    djkkVisits.totalPlayerStat = AddCapped(djkkVisits.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B5F24 (name is ours)
void SvCensus::AddPlayedHideAndSeekTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    playedHideAndSeek.totalPlayerStat = AddCapped(playedHideAndSeek.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B5F74 (name is ours)
void SvCensus::AddHelpedGulliverTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    helpedGulliver.totalPlayerStat = AddCapped(helpedGulliver.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B5FC4 (name is ours)
void SvCensus::AddFlowersPlantedTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    flowersPlanted.totalPlayerStat = AddCapped(flowersPlanted.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6014 (name is ours)
void SvCensus::AddLettersSentTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    lettersSent.totalPlayerStat = AddCapped(lettersSent.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6064 (name is ours)
void SvCensus::AddTourVisitsTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    tourVisits.totalPlayerStat = AddCapped(tourVisits.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B60B4 (name is ours)
void SvCensus::AddWeedsPulledTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    weedsPulled.totalPlayerStat = AddCapped(weedsPulled.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6104 (name is ours)
void SvCensus::AddAmiiboScannedTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    amiiboScanned.totalPlayerStat = AddCapped(amiiboScanned.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6154 (name is ours)
void SvCensus::ClearFestivalFeathersCaught()
{
    festivalFeathersCaught.totalPlayerStat = 0;
    festivalFeathersCaught.playerStats[0] = 0;
    festivalFeathersCaught.playerStats[1] = 0;
    festivalFeathersCaught.playerStats[2] = 0;
    festivalFeathersCaught.playerStats[3] = 0;
}

// 0x006B6170 (name is ours)
void SvCensus::ClearFireworkDesignsGivenToIsabelle()
{
    fireworkDesignsGivenToIsabelle.totalPlayerStat = 0;
    fireworkDesignsGivenToIsabelle.playerStats[0] = 0;
    fireworkDesignsGivenToIsabelle.playerStats[1] = 0;
    fireworkDesignsGivenToIsabelle.playerStats[2] = 0;
    fireworkDesignsGivenToIsabelle.playerStats[3] = 0;
}

// 0x006B618C (name is ours)
SvCensus* GetCensusOfGardenPlus()
{
    return &GetGardenPlus()->sharedData.playerStats.census;
}

// 0x006B61A0 (name is ours)
void SvCensus::AddEarnedBadgesTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    earnedBadges.totalPlayerStat = AddCapped(earnedBadges.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B61F0 (name is ours)
void SvCensus::AddTotalBingoWinsTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    totalBingoWins.totalPlayerStat = AddCapped(totalBingoWins.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6240 (name is ours)
void SvCensus::AddHelpedKatieTravelTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    helpedKatieTravel.totalPlayerStat = AddCapped(helpedKatieTravel.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6290 (name is ours)
void SvCensus::AddBellsEarnedTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    bellsEarned.totalPlayerStat = AddCapped(bellsEarned.totalPlayerStat, value, CENSUS_VALUE_MAX);
}

// 0x006B62E0 (name is ours)
void SvCensus::AddTreesPlantedTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    treesPlanted.totalPlayerStat = AddCapped(treesPlanted.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6330 (name is ours)
void SvCensus::AddShampoodleVisitsTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    shampoodleVisits.totalPlayerStat = AddCapped(shampoodleVisits.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6380 (name is ours)
void SvCensus::AddShrunkSketchesAttendedTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    shrunkSketchesAttended.totalPlayerStat = AddCapped(shrunkSketchesAttended.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B63D0 (name is ours)
void SvCensus::AddTownsVisitedTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    townsVisited.totalPlayerStat = AddCapped(townsVisited.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6420 (name is ours)
void SvCensus::ClearFoundImposterBlanca()
{
    foundImposterBlanca.totalPlayerStat = 0;
    foundImposterBlanca.playerStats[0] = 0;
    foundImposterBlanca.playerStats[1] = 0;
    foundImposterBlanca.playerStats[2] = 0;
    foundImposterBlanca.playerStats[3] = 0;
}

// 0x006B643C (name is ours)
void SvCensus::ClearGivenToyDayPresents()
{
    givenToyDayPresents.totalPlayerStat = 0;
    givenToyDayPresents.playerStats[0] = 0;
    givenToyDayPresents.playerStats[1] = 0;
    givenToyDayPresents.playerStats[2] = 0;
    givenToyDayPresents.playerStats[3] = 0;
}

// 0x006B6458 (name is ours)
void SvCensus::ClearReceivedCandyOnHalloween()
{
    receivedCandyOnHalloween.totalPlayerStat = 0;
    receivedCandyOnHalloween.playerStats[0] = 0;
    receivedCandyOnHalloween.playerStats[1] = 0;
    receivedCandyOnHalloween.playerStats[2] = 0;
    receivedCandyOnHalloween.playerStats[3] = 0;
}

// 0x006B6474 (name is ours)
void SvCensus::AddJunkCatch(u32 index, s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(junkCatchCountsPerPlayer[index][SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    junkCatchCountsPerPlayer[index][SvGetCurrentTownSlot()] = stat;
    AddJunkCatchTotal(index, value);
}

// 0x006B64FC (name is ours)
void SvCensus::AddFishCatch(u32 index, s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(fishCatchCountsPerPlayer[index][SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    fishCatchCountsPerPlayer[index][SvGetCurrentTownSlot()] = stat;
    AddFishCatchTotal(index, value);
}

// 0x006B657C (name is ours)
void SvCensus::AddSeaCreaturesCaughtTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    seaCreaturesCaught.totalPlayerStat = AddCapped(seaCreaturesCaught.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B65CC (name is ours)
void SvCensus::AddEggsGivenToZipperTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    eggsGivenToZipper.totalPlayerStat = AddCapped(eggsGivenToZipper.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B661C (name is ours)
void SvCensus::AddMeowCouponsEarnedTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    meowCouponsEarned.totalPlayerStat = AddCapped(meowCouponsEarned.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B666C (name is ours)
void SvCensus::AddIslandVisitsTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    islandVisits.totalPlayerStat = AddCapped(islandVisits.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B66BC (name is ours)
void SvCensus::AddShootingStarWishesTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    shootingStarWishes.totalPlayerStat = AddCapped(shootingStarWishes.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B670C (name is ours)
void SvCensus::AddSaharaRedecorationsTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    saharaRedecorations.totalPlayerStat = AddCapped(saharaRedecorations.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B675C (name is ours)
void SvCensus::AddFurnitureCustomizedTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    furnitureCustomized.totalPlayerStat = AddCapped(furnitureCustomized.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B67AC (name is ours)
void SvCensus::AddAbdBalanceTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    abdBalance.totalPlayerStat = AddCapped(abdBalance.totalPlayerStat, value, CENSUS_VALUE_MAX);
}

// 0x006B67FC (name is ours)
void SvCensus::AddMeowCouponsSpentTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    meowCouponsSpent.totalPlayerStat = AddCapped(meowCouponsSpent.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B684C (name is ours)
void SvCensus::AddBalloonsPoppedTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    balloonsPopped.totalPlayerStat = AddCapped(balloonsPopped.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B689C (name is ours)
void SvCensus::AddNpcPicturesReceivedTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    npcPicturesReceived.totalPlayerStat = AddCapped(npcPicturesReceived.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B68EC (name is ours)
void SvCensus::AddPartyPoppersPoppedAtNewYearsTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    partyPoppersPoppedAtNewYears.totalPlayerStat = AddCapped(partyPoppersPoppedAtNewYears.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B693C (name is ours)
void SvCensus::AddFishCaughtTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    fishCaught.totalPlayerStat = AddCapped(fishCaught.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B698C (name is ours)
void SvCensus::AddHarvestFestivalCoursesDoneTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    harvestFestivalCoursesDone.totalPlayerStat = AddCapped(harvestFestivalCoursesDone.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B69DC (name is ours)
void SvCensus::AddBoughtArtAtReddsTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    boughtArtAtRedds.totalPlayerStat = AddCapped(boughtArtAtRedds.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6A2C (name is ours)
void SvCensus::AddPublicWorkExpensesTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    publicWorkExpenses.totalPlayerStat = AddCapped(publicWorkExpenses.totalPlayerStat, value, CENSUS_VALUE_MAX);
}

// 0x006B6A7C (name is ours)
void SvCensus::AddResetSurveillanceCenterVisitsTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    resetSurveillanceCenterVisits.totalPlayerStat = AddCapped(resetSurveillanceCenterVisits.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6ACC (name is ours)
void SvCensus::AddScallopsGivenToPascalTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    scallopsGivenToPascal.totalPlayerStat = AddCapped(scallopsGivenToPascal.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6B1C (name is ours)
void SvCensus::AddRecycleShopEarningsTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    recycleShopEarnings.totalPlayerStat = AddCapped(recycleShopEarnings.totalPlayerStat, value, CENSUS_VALUE_MAX);
}

// 0x006B6B6C (name is ours)
void SvCensus::AddSnowmenBuiltByYouInTownTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    snowmenBuiltByYouInTown.totalPlayerStat = AddCapped(snowmenBuiltByYouInTown.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6BBC (name is ours)
void SvCensus::AddTownVisitorsTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    townVisitors.totalPlayerStat = AddCapped(townVisitors.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6C0C (name is ours)
void SvCensus::ClearTourneyFishCaught()
{
    tourneyFishCaught.totalPlayerStat = 0;
    tourneyFishCaught.playerStats[0] = 0;
    tourneyFishCaught.playerStats[1] = 0;
    tourneyFishCaught.playerStats[2] = 0;
    tourneyFishCaught.playerStats[3] = 0;
}

// 0x006B6C28 (name is ours)
void SvCensus::AddInsectCatch(u32 index, s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(insectCatchCountsPerPlayer[index][SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    insectCatchCountsPerPlayer[index][SvGetCurrentTownSlot()] = stat;
    AddInsectCatchTotal(index, value);
}

// 0x006B6CA8 (name is ours)
void SvCensus::AddBlueRosesCreatedTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    blueRosesCreated.totalPlayerStat = AddCapped(blueRosesCreated.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6CF8 (name is ours)
void SvCensus::AddFestivalFeathersCaughtTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    festivalFeathersCaught.totalPlayerStat = AddCapped(festivalFeathersCaught.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6D48 (name is ours)
void SvCensus::AddBugsCaughtTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    bugsCaught.totalPlayerStat = AddCapped(bugsCaught.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6D98 (name is ours)
void SvCensus::AddDreamTownsVisitedTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    dreamTownsVisited.totalPlayerStat = AddCapped(dreamTownsVisited.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6DE8 (name is ours)
void SvCensus::AddFireworkDesignsGivenToIsabelleTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    fireworkDesignsGivenToIsabelle.totalPlayerStat = AddCapped(fireworkDesignsGivenToIsabelle.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6E38 (name is ours)
void SvCensus::AddGoldenRosesCreatedTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    goldenRosesCreated.totalPlayerStat = AddCapped(goldenRosesCreated.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6E88 (name is ours)
void SvCensus::AddUnk0x550Total(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    unk_0x550.totalPlayerStat = AddCapped(unk_0x550.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6ED8 (name is ours)
void SvCensus::AddKicksExpensesTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    kicksExpenses.totalPlayerStat = AddCapped(kicksExpenses.totalPlayerStat, value, CENSUS_VALUE_MAX);
}

// 0x006B6F28 (name is ours)
void SvCensus::AddKkSliderConcertsAttendedTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    kkSliderConcertsAttended.totalPlayerStat = AddCapped(kkSliderConcertsAttended.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6F78 (name is ours)
void SvCensus::AddFlowersWateredTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    flowersWatered.totalPlayerStat = AddCapped(flowersWatered.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B6FC8 (name is ours)
void SvCensus::ClearTourneyInsectCaught()
{
    tourneyInsectCaught.totalPlayerStat = 0;
    tourneyInsectCaught.playerStats[0] = 0;
    tourneyInsectCaught.playerStats[1] = 0;
    tourneyInsectCaught.playerStats[2] = 0;
    tourneyInsectCaught.playerStats[3] = 0;
}

// 0x006B6FE4 (name is ours)
void SvCensus::AddFossilsAnalyzedTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    fossilsAnalyzed.totalPlayerStat = AddCapped(fossilsAnalyzed.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B7034 (name is ours)
void SvCensus::AddFoundImposterBlancaTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    foundImposterBlanca.totalPlayerStat = AddCapped(foundImposterBlanca.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B7084 (name is ours)
void SvCensus::AddGivenToyDayPresentsTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    givenToyDayPresents.totalPlayerStat = AddCapped(givenToyDayPresents.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B70D4 (name is ours)
void SvCensus::AddProDesignsCreatedTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    proDesignsCreated.totalPlayerStat = AddCapped(proDesignsCreated.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B7124 (name is ours)
void SvCensus::AddReceivedCandyOnHalloweenTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    receivedCandyOnHalloween.totalPlayerStat = AddCapped(receivedCandyOnHalloween.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B7174 (name is ours)
void SvCensus::AddBellsEarnedForSlot(u32 townSlot, s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    bellsEarned.playerStats[townSlot] = AddCapped(bellsEarned.playerStats[townSlot], value, CENSUS_VALUE_MAX);
    AddBellsEarnedTotal(value);
}

// 0x006B71DC (name is ours)
void SvCensus::AddGraciesExpensesTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    graciesExpenses.totalPlayerStat = AddCapped(graciesExpenses.totalPlayerStat, value, CENSUS_VALUE_MAX);
}

// 0x006B722C (name is ours)
void SvCensus::SetEarnedBadgesForSlot(u32 townSlot, s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = Clamp(value, CENSUS_COUNT_MAX);
    earnedBadges.playerStats[townSlot] = stat;
    AddEarnedBadgesTotal(value - stat);
}

// 0x006B7280 (name is ours)
void SvCensus::AddSeaCreatureCatch(u32 index, s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(seaCreatureCatchCountsPerPlayer[index][SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    seaCreatureCatchCountsPerPlayer[index][SvGetCurrentTownSlot()] = stat;
    AddSeaCreatureCatchTotal(index, value);
}

// 0x006B7308 (name is ours)
void SvCensus::AddTurnipsBoughtTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    turnipsBought.totalPlayerStat = AddCapped(turnipsBought.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B7358 (name is ours)
void SvCensus::AddMeowCouponsEarnedForSlot(u32 townSlot, s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    meowCouponsEarned.playerStats[townSlot] = AddCapped(meowCouponsEarned.playerStats[townSlot], value, CENSUS_COUNT_MAX);
    AddMeowCouponsEarnedTotal(value);
}

// 0x006B73C0 (name is ours)
void SvCensus::AddTreesCutDown(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(treesCutDown.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    treesCutDown.playerStats[SvGetCurrentTownSlot()] = stat;
    AddTreesCutDownTotal(value);
}

// 0x006B7434 (name is ours)
void SvCensus::AddUnk0x62CTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    unk_0x62C.totalPlayerStat = AddCapped(unk_0x62C.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B7484 (name is ours)
void SvCensus::AddLoanPaidTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    loanPaid.totalPlayerStat = AddCapped(loanPaid.totalPlayerStat, value, CENSUS_VALUE_MAX);
}

// 0x006B74D4 (name is ours)
void SvCensus::AddAbleSistersExpensesTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    ableSistersExpenses.totalPlayerStat = AddCapped(ableSistersExpenses.totalPlayerStat, value, CENSUS_VALUE_MAX);
}

// 0x006B7524 (name is ours)
void SvCensus::SetPublicWorksBuiltTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    if (value < 0) {
        value = 0;
    } else if (value > static_cast<s32>(CENSUS_PUBLIC_WORKS_MAX)) {
        value = CENSUS_PUBLIC_WORKS_MAX;
    }
    publicWorksBuilt.totalPlayerStat = value;
}

// 0x006B7558 (name is ours)
void SvCensus::SetAbdBalanceForSlot(u32 townSlot, s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = Clamp(value, CENSUS_VALUE_MAX);
    abdBalance.playerStats[townSlot] = stat;
    AddAbdBalanceTotal(value - stat);
}

// 0x006B75AC (name is ours)
void SvCensus::AddJobsAtTheRoost(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(jobsAtTheRoost.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    jobsAtTheRoost.playerStats[SvGetCurrentTownSlot()] = stat;
    AddJobsAtTheRoostTotal(value);
}

// 0x006B7620 (name is ours)
void SvCensus::AddBellsSpentTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    bellsSpent.totalPlayerStat = AddCapped(bellsSpent.totalPlayerStat, value, CENSUS_VALUE_MAX);
}

// 0x006B7670 (name is ours)
void SvCensus::AddDjkkVisits(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(djkkVisits.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    djkkVisits.playerStats[SvGetCurrentTownSlot()] = stat;
    AddDjkkVisitsTotal(value);
}

// 0x006B76E4 (name is ours)
void SvCensus::AddTourneyFishCaughtTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    tourneyFishCaught.totalPlayerStat = AddCapped(tourneyFishCaught.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B7734 (name is ours)
void SvCensus::AddPlayedHideAndSeek(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(playedHideAndSeek.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    playedHideAndSeek.playerStats[SvGetCurrentTownSlot()] = stat;
    AddPlayedHideAndSeekTotal(value);
}

// 0x006B77A8 (name is ours)
void SvCensus::AddTownInitiativesCompletedTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    townInitiativesCompleted.totalPlayerStat = AddCapped(townInitiativesCompleted.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B77F8 (name is ours)
void SvCensus::AddLostItemsGivenBackTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    lostItemsGivenBack.totalPlayerStat = AddCapped(lostItemsGivenBack.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B7848 (name is ours)
void SvCensus::AddHelpedGulliver(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(helpedGulliver.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    helpedGulliver.playerStats[SvGetCurrentTownSlot()] = stat;
    AddHelpedGulliverTotal(value);
}

// 0x006B78BC (name is ours)
void SvCensus::AddFlowersPlanted(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(flowersPlanted.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    flowersPlanted.playerStats[SvGetCurrentTownSlot()] = stat;
    AddFlowersPlantedTotal(value);
}

// 0x006B7930 (name is ours)
void SvCensus::AddTurnipsSoldTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    turnipsSold.totalPlayerStat = AddCapped(turnipsSold.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B7980 (name is ours)
void SvCensus::AddLettersSent(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(lettersSent.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    lettersSent.playerStats[SvGetCurrentTownSlot()] = stat;
    AddLettersSentTotal(value);
}

// 0x006B79F4 (name is ours)
void SvCensus::AddKatrinaVisitsTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    katrinaVisits.totalPlayerStat = AddCapped(katrinaVisits.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B7A44 (name is ours)
void SvCensus::AddSnowflakesCaughtTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    snowflakesCaught.totalPlayerStat = AddCapped(snowflakesCaught.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B7A94 (name is ours)
void SvCensus::AddUnk0x564Total(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    unk_0x564.totalPlayerStat = AddCapped(unk_0x564.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B7AE4 (name is ours)
void SvCensus::AddTourVisits(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(tourVisits.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    tourVisits.playerStats[SvGetCurrentTownSlot()] = stat;
    AddTourVisitsTotal(value);
}

// 0x006B7B58 (name is ours)
void SvCensus::AddYouVisitedVillagersTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    youVisitedVillagers.totalPlayerStat = AddCapped(youVisitedVillagers.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B7BA8 (name is ours)
void SvCensus::AddWeedsPulled(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(weedsPulled.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    weedsPulled.playerStats[SvGetCurrentTownSlot()] = stat;
    AddWeedsPulledTotal(value);
}

// 0x006B7C1C (name is ours)
void SvCensus::AddAmiiboScanned(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(amiiboScanned.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    amiiboScanned.playerStats[SvGetCurrentTownSlot()] = stat;
    AddAmiiboScannedTotal(value);
}

// 0x006B7C90 (name is ours)
void SvCensus::AddTotalBingoWins(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(totalBingoWins.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    totalBingoWins.playerStats[SvGetCurrentTownSlot()] = stat;
    AddTotalBingoWinsTotal(value);
}

// 0x006B7D04 (name is ours)
void SvCensus::AddTurnipExpensesTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    turnipExpenses.totalPlayerStat = AddCapped(turnipExpenses.totalPlayerStat, value, CENSUS_VALUE_MAX);
}

// 0x006B7D54 (name is ours)
void SvCensus::AddTourneyInsectCaughtTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    tourneyInsectCaught.totalPlayerStat = AddCapped(tourneyInsectCaught.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B7DA4 (name is ours)
void SvCensus::AddVillagersVisitedYouTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    villagersVisitedYou.totalPlayerStat = AddCapped(villagersVisitedYou.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B7DF4 (name is ours)
void SvCensus::AddFireworkDesignsGivenToIsabelleForSlot(u32 townSlot, s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    fireworkDesignsGivenToIsabelle.playerStats[townSlot] = AddCapped(fireworkDesignsGivenToIsabelle.playerStats[townSlot], value, CENSUS_COUNT_MAX);
    AddFireworkDesignsGivenToIsabelleTotal(value);
}

// 0x006B7E5C (name is ours)
void SvCensus::AddHelpedKatieTravel(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(helpedKatieTravel.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    helpedKatieTravel.playerStats[SvGetCurrentTownSlot()] = stat;
    AddHelpedKatieTravelTotal(value);
}

// 0x006B7ED0 (name is ours)
void SvCensus::AddBellsEarned(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(bellsEarned.playerStats[SvGetCurrentTownSlot()], value, CENSUS_VALUE_MAX);
    bellsEarned.playerStats[SvGetCurrentTownSlot()] = stat;
    AddBellsEarnedTotal(value);
}

// 0x006B7F44 (name is ours)
void SvCensus::AddTreesPlanted(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(treesPlanted.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    treesPlanted.playerStats[SvGetCurrentTownSlot()] = stat;
    AddTreesPlantedTotal(value);
}

// 0x006B7FB8 (name is ours)
void SvCensus::AddShampoodleVisits(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(shampoodleVisits.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    shampoodleVisits.playerStats[SvGetCurrentTownSlot()] = stat;
    AddShampoodleVisitsTotal(value);
}

// 0x006B802C (name is ours)
void SvCensus::AddShrunkSketchesAttended(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(shrunkSketchesAttended.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    shrunkSketchesAttended.playerStats[SvGetCurrentTownSlot()] = stat;
    AddShrunkSketchesAttendedTotal(value);
}

// 0x006B80A0 (name is ours)
void SvCensus::AddTownsVisited(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(townsVisited.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    townsVisited.playerStats[SvGetCurrentTownSlot()] = stat;
    AddTownsVisitedTotal(value);
}

// 0x006B8114 (name is ours)
void SvCensus::SetEarnedBadges(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = Clamp(value, CENSUS_COUNT_MAX);
    earnedBadges.playerStats[SvGetCurrentTownSlot()] = stat;
    AddEarnedBadgesTotal(value - earnedBadges.playerStats[SvGetCurrentTownSlot()]);
}

// 0x006B8178 (name is ours)
void SvCensus::AddSeaCreaturesCaught(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(seaCreaturesCaught.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    seaCreaturesCaught.playerStats[SvGetCurrentTownSlot()] = stat;
    AddSeaCreaturesCaughtTotal(value);
}

// 0x006B81EC (name is ours)
void SvCensus::AddEggsGivenToZipper(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(eggsGivenToZipper.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    eggsGivenToZipper.playerStats[SvGetCurrentTownSlot()] = stat;
    AddEggsGivenToZipperTotal(value);
}

// 0x006B8260 (name is ours)
void SvCensus::AddMeowCouponsEarned(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(meowCouponsEarned.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    meowCouponsEarned.playerStats[SvGetCurrentTownSlot()] = stat;
    AddMeowCouponsEarnedTotal(value);
}

// 0x006B82D4 (name is ours)
void SvCensus::AddIslandVisits(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(islandVisits.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    islandVisits.playerStats[SvGetCurrentTownSlot()] = stat;
    AddIslandVisitsTotal(value);
}

// 0x006B8348 (name is ours)
void SvCensus::AddShootingStarWishes(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(shootingStarWishes.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    shootingStarWishes.playerStats[SvGetCurrentTownSlot()] = stat;
    AddShootingStarWishesTotal(value);
}

// 0x006B83BC (name is ours)
void SvCensus::AddSaharaRedecorations(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(saharaRedecorations.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    saharaRedecorations.playerStats[SvGetCurrentTownSlot()] = stat;
    AddSaharaRedecorationsTotal(value);
}

// 0x006B8430 (name is ours)
void SvCensus::AddFurnitureCustomized(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(furnitureCustomized.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    furnitureCustomized.playerStats[SvGetCurrentTownSlot()] = stat;
    AddFurnitureCustomizedTotal(value);
}

// 0x006B84A4 (name is ours)
void SvCensus::AddTurnipProfitsTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    turnipProfits.totalPlayerStat = AddCapped(turnipProfits.totalPlayerStat, value, CENSUS_VALUE_MAX);
}

// 0x006B84F4 (name is ours)
void SvCensus::AddGardeningShopExpensesTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    gardeningShopExpenses.totalPlayerStat = AddCapped(gardeningShopExpenses.totalPlayerStat, value, CENSUS_VALUE_MAX);
}

// 0x006B8544 (name is ours)
void SvCensus::AddNooklingsExpensesTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    nooklingsExpenses.totalPlayerStat = AddCapped(nooklingsExpenses.totalPlayerStat, value, CENSUS_VALUE_MAX);
}

// 0x006B8594 (name is ours)
void SvCensus::AddMeowCouponsSpent(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(meowCouponsSpent.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    meowCouponsSpent.playerStats[SvGetCurrentTownSlot()] = stat;
    AddMeowCouponsSpentTotal(value);
}

// 0x006B8608 (name is ours)
void SvCensus::SetAbdBalance(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = Clamp(value, CENSUS_VALUE_MAX);
    abdBalance.playerStats[SvGetCurrentTownSlot()] = stat;
    AddAbdBalanceTotal(value - abdBalance.playerStats[SvGetCurrentTownSlot()]);
}

// 0x006B866C (name is ours)
void SvCensus::AddBalloonsPopped(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(balloonsPopped.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    balloonsPopped.playerStats[SvGetCurrentTownSlot()] = stat;
    AddBalloonsPoppedTotal(value);
}

// 0x006B86E0 (name is ours)
void SvCensus::AddPartyPoppersPoppedAtNewYears(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(partyPoppersPoppedAtNewYears.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    partyPoppersPoppedAtNewYears.playerStats[SvGetCurrentTownSlot()] = stat;
    AddPartyPoppersPoppedAtNewYearsTotal(value);
}

// 0x006B8754 (name is ours)
void SvCensus::AddFamousMushroomsEatenTotal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    famousMushroomsEaten.totalPlayerStat = AddCapped(famousMushroomsEaten.totalPlayerStat, value, CENSUS_COUNT_MAX);
}

// 0x006B87A4 (name is ours)
void SvCensus::AddFishCaught(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(fishCaught.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    fishCaught.playerStats[SvGetCurrentTownSlot()] = stat;
    AddFishCaughtTotal(value);
}

// 0x006B8818 (name is ours)
void SvCensus::AddHarvestFestivalCoursesDone(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(harvestFestivalCoursesDone.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    harvestFestivalCoursesDone.playerStats[SvGetCurrentTownSlot()] = stat;
    AddHarvestFestivalCoursesDoneTotal(value);
}

// 0x006B888C (name is ours)
void SvCensus::AddBoughtArtAtRedds(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(boughtArtAtRedds.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    boughtArtAtRedds.playerStats[SvGetCurrentTownSlot()] = stat;
    AddBoughtArtAtReddsTotal(value);
}

// 0x006B8900 (name is ours)
void SvCensus::AddPublicWorkExpenses(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(publicWorkExpenses.playerStats[SvGetCurrentTownSlot()], value, CENSUS_VALUE_MAX);
    publicWorkExpenses.playerStats[SvGetCurrentTownSlot()] = stat;
    AddPublicWorkExpensesTotal(value);
}

// 0x006B8974 (name is ours)
void SvCensus::AddResetSurveillanceCenterVisits(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(resetSurveillanceCenterVisits.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    resetSurveillanceCenterVisits.playerStats[SvGetCurrentTownSlot()] = stat;
    AddResetSurveillanceCenterVisitsTotal(value);
}

// 0x006B89E8 (name is ours)
void SvCensus::AddScallopsGivenToPascal(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(scallopsGivenToPascal.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    scallopsGivenToPascal.playerStats[SvGetCurrentTownSlot()] = stat;
    AddScallopsGivenToPascalTotal(value);
}

// 0x006B8A5C (name is ours)
void SvCensus::AddRecycleShopEarnings(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(recycleShopEarnings.playerStats[SvGetCurrentTownSlot()], value, CENSUS_VALUE_MAX);
    recycleShopEarnings.playerStats[SvGetCurrentTownSlot()] = stat;
    AddRecycleShopEarningsTotal(value);
}

// 0x006B8AD0 (name is ours)
void SvCensus::AddSnowmenBuiltByYouInTown(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(snowmenBuiltByYouInTown.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    snowmenBuiltByYouInTown.playerStats[SvGetCurrentTownSlot()] = stat;
    AddSnowmenBuiltByYouInTownTotal(value);
}

// 0x006B8B44 (name is ours)
void SvCensus::AddTownVisitors(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(townVisitors.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    townVisitors.playerStats[SvGetCurrentTownSlot()] = stat;
    AddTownVisitorsTotal(value);
}

// 0x006B8BB8 (name is ours)
void SvCensus::SetNpcPicturesReceived(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = Clamp(value, CENSUS_COUNT_MAX);
    npcPicturesReceived.playerStats[SvGetCurrentTownSlot()] = stat;
    AddNpcPicturesReceivedTotal(value - npcPicturesReceived.playerStats[SvGetCurrentTownSlot()]);
}

// 0x006B8C1C (name is ours)
void SvCensus::AddFestivalFeathersCaught(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(festivalFeathersCaught.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    festivalFeathersCaught.playerStats[SvGetCurrentTownSlot()] = stat;
    AddFestivalFeathersCaughtTotal(value);
}

// 0x006B8C90 (name is ours)
void SvCensus::AddBugsCaught(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(bugsCaught.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    bugsCaught.playerStats[SvGetCurrentTownSlot()] = stat;
    AddBugsCaughtTotal(value);
}

// 0x006B8D04 (name is ours)
void SvCensus::AddDreamTownsVisited(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(dreamTownsVisited.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    dreamTownsVisited.playerStats[SvGetCurrentTownSlot()] = stat;
    AddDreamTownsVisitedTotal(value);
}

// 0x006B8D78 (name is ours)
void SvCensus::AddTownInitiativesCompletedForSlot(u32 townSlot, s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    townInitiativesCompleted.playerStats[townSlot] = AddCapped(townInitiativesCompleted.playerStats[townSlot], value, CENSUS_COUNT_MAX);
    AddTownInitiativesCompletedTotal(value);
}

// 0x006B8DE0 (name is ours)
void SvCensus::AddUnk0x550(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(unk_0x550.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    unk_0x550.playerStats[SvGetCurrentTownSlot()] = stat;
    AddUnk0x550Total(value);
}

// 0x006B8E54 (name is ours)
void SvCensus::AddKicksExpenses(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(kicksExpenses.playerStats[SvGetCurrentTownSlot()], value, CENSUS_VALUE_MAX);
    kicksExpenses.playerStats[SvGetCurrentTownSlot()] = stat;
    AddKicksExpensesTotal(value);
}

// 0x006B8EC8 (name is ours)
void SvCensus::AddUnk0x564ForSlot(u32 townSlot, s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    unk_0x564.playerStats[townSlot] = AddCapped(unk_0x564.playerStats[townSlot], value, CENSUS_COUNT_MAX);
    AddUnk0x564Total(value);
}

// 0x006B8F30 (name is ours)
void SvCensus::AddKkSliderConcertsAttended(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(kkSliderConcertsAttended.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    kkSliderConcertsAttended.playerStats[SvGetCurrentTownSlot()] = stat;
    AddKkSliderConcertsAttendedTotal(value);
}

// 0x006B8FA4 (name is ours)
void SvCensus::AddFlowersWatered(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(flowersWatered.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    flowersWatered.playerStats[SvGetCurrentTownSlot()] = stat;
    AddFlowersWateredTotal(value);
}

// 0x006B9018 (name is ours)
void SvCensus::UpdateAbdBalanceForSlot(u32 townSlot, s32 value)
{
    AddAbdBalanceTotal(value - abdBalance.playerStats[townSlot]);
    if (value < 0) {
        value = 0;
    } else if (value > static_cast<s32>(CENSUS_VALUE_MAX)) {
        value = CENSUS_VALUE_MAX;
    }
    abdBalance.playerStats[townSlot] = value;
}

// 0x006B9054 (name is ours)
void SvCensus::AddFossilsAnalyzed(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(fossilsAnalyzed.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    fossilsAnalyzed.playerStats[SvGetCurrentTownSlot()] = stat;
    AddFossilsAnalyzedTotal(value);
}

// 0x006B90C8 (name is ours)
void SvCensus::AddFoundImposterBlanca(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(foundImposterBlanca.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    foundImposterBlanca.playerStats[SvGetCurrentTownSlot()] = stat;
    AddFoundImposterBlancaTotal(value);
}

// 0x006B913C (name is ours)
void SvCensus::AddGivenToyDayPresents(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(givenToyDayPresents.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    givenToyDayPresents.playerStats[SvGetCurrentTownSlot()] = stat;
    AddGivenToyDayPresentsTotal(value);
}

// 0x006B91B0 (name is ours)
void SvCensus::AddProDesignsCreated(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(proDesignsCreated.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    proDesignsCreated.playerStats[SvGetCurrentTownSlot()] = stat;
    AddProDesignsCreatedTotal(value);
}

// 0x006B9224 (name is ours)
void SvCensus::AddReceivedCandyOnHalloween(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(receivedCandyOnHalloween.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    receivedCandyOnHalloween.playerStats[SvGetCurrentTownSlot()] = stat;
    AddReceivedCandyOnHalloweenTotal(value);
}

// 0x006B9298 (name is ours)
void SvCensus::AddGraciesExpenses(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(graciesExpenses.playerStats[SvGetCurrentTownSlot()], value, CENSUS_VALUE_MAX);
    graciesExpenses.playerStats[SvGetCurrentTownSlot()] = stat;
    AddGraciesExpensesTotal(value);
}

// 0x006B930C (name is ours)
void SvCensus::AddTurnipsBought(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(turnipsBought.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    turnipsBought.playerStats[SvGetCurrentTownSlot()] = stat;
    AddTurnipsBoughtTotal(value);
}

// 0x006B9380 (name is ours)
void SvCensus::AddLoanPaid(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(loanPaid.playerStats[SvGetCurrentTownSlot()], value, CENSUS_VALUE_MAX);
    loanPaid.playerStats[SvGetCurrentTownSlot()] = stat;
    AddLoanPaidTotal(value);
}

// 0x006B93F4 (name is ours)
void SvCensus::AddAbleSistersExpenses(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(ableSistersExpenses.playerStats[SvGetCurrentTownSlot()], value, CENSUS_VALUE_MAX);
    ableSistersExpenses.playerStats[SvGetCurrentTownSlot()] = stat;
    AddAbleSistersExpensesTotal(value);
}

// 0x006B9468 (name is ours)
void SvCensus::AddBellsSpent(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(bellsSpent.playerStats[SvGetCurrentTownSlot()], value, CENSUS_VALUE_MAX);
    bellsSpent.playerStats[SvGetCurrentTownSlot()] = stat;
    AddBellsSpentTotal(value);
}

// 0x006B94DC (name is ours)
void SvCensus::AddTourneyFishCaught(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(tourneyFishCaught.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    tourneyFishCaught.playerStats[SvGetCurrentTownSlot()] = stat;
    AddTourneyFishCaughtTotal(value);
}

// 0x006B9550 (name is ours)
void SvCensus::AddTownInitiativesCompleted(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(townInitiativesCompleted.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    townInitiativesCompleted.playerStats[SvGetCurrentTownSlot()] = stat;
    AddTownInitiativesCompletedTotal(value);
}

// 0x006B95C4 (name is ours)
void SvCensus::AddLostItemsGivenBack(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(lostItemsGivenBack.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    lostItemsGivenBack.playerStats[SvGetCurrentTownSlot()] = stat;
    AddLostItemsGivenBackTotal(value);
}

// 0x006B9638 (name is ours)
void SvCensus::AddTurnipsSold(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(turnipsSold.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    turnipsSold.playerStats[SvGetCurrentTownSlot()] = stat;
    AddTurnipsSoldTotal(value);
}

// 0x006B96AC (name is ours)
void SvCensus::AddKatrinaVisits(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(katrinaVisits.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    katrinaVisits.playerStats[SvGetCurrentTownSlot()] = stat;
    AddKatrinaVisitsTotal(value);
}

// 0x006B9720 (name is ours)
void SvCensus::AddSnowflakesCaught(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(snowflakesCaught.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    snowflakesCaught.playerStats[SvGetCurrentTownSlot()] = stat;
    AddSnowflakesCaughtTotal(value);
}

// 0x006B9794 (name is ours)
void SvCensus::AddYouVisitedVillagers(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(youVisitedVillagers.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    youVisitedVillagers.playerStats[SvGetCurrentTownSlot()] = stat;
    AddYouVisitedVillagersTotal(value);
}

// 0x006B9808 (name is ours)
void SvCensus::UpdateAbdBalance(s32 value)
{
    AddAbdBalanceTotal(value - abdBalance.playerStats[SvGetCurrentTownSlot()]);
    if (value < 0) {
        value = 0;
    } else if (value > static_cast<s32>(CENSUS_VALUE_MAX)) {
        value = CENSUS_VALUE_MAX;
    }
    abdBalance.playerStats[SvGetCurrentTownSlot()] = value;
}

// 0x006B985C (name is ours)
void SvCensus::AddTurnipExpenses(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(turnipExpenses.playerStats[SvGetCurrentTownSlot()], value, CENSUS_VALUE_MAX);
    turnipExpenses.playerStats[SvGetCurrentTownSlot()] = stat;
    AddTurnipExpensesTotal(value);
}

// 0x006B98D0 (name is ours)
void SvCensus::AddTourneyInsectCaught(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(tourneyInsectCaught.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    tourneyInsectCaught.playerStats[SvGetCurrentTownSlot()] = stat;
    AddTourneyInsectCaughtTotal(value);
}

// 0x006B9944 (name is ours)
void SvCensus::AddVillagersVisitedYou(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(villagersVisitedYou.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    villagersVisitedYou.playerStats[SvGetCurrentTownSlot()] = stat;
    AddVillagersVisitedYouTotal(value);
}

// 0x006B99B8 (name is ours)
void SvCensus::AddTurnipProfits(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(turnipProfits.playerStats[SvGetCurrentTownSlot()], value, CENSUS_VALUE_MAX);
    turnipProfits.playerStats[SvGetCurrentTownSlot()] = stat;
    AddTurnipProfitsTotal(value);
}

// 0x006B9A2C (name is ours)
void SvCensus::AddGardeningShopExpenses(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(gardeningShopExpenses.playerStats[SvGetCurrentTownSlot()], value, CENSUS_VALUE_MAX);
    gardeningShopExpenses.playerStats[SvGetCurrentTownSlot()] = stat;
    AddGardeningShopExpensesTotal(value);
}

// 0x006B9AA0 (name is ours)
void SvCensus::AddNooklingsExpenses(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(nooklingsExpenses.playerStats[SvGetCurrentTownSlot()], value, CENSUS_VALUE_MAX);
    nooklingsExpenses.playerStats[SvGetCurrentTownSlot()] = stat;
    AddNooklingsExpensesTotal(value);
}

// 0x006B9B14 (name is ours)
void SvCensus::AddFamousMushroomsEaten(s32 value)
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const u32 stat = AddCapped(famousMushroomsEaten.playerStats[SvGetCurrentTownSlot()], value, CENSUS_COUNT_MAX);
    famousMushroomsEaten.playerStats[SvGetCurrentTownSlot()] = stat;
    AddFamousMushroomsEatenTotal(value);
}

// 0x006B9B88 (name is ours)
void SvCensus::Clear()
{
    bellsEarned.totalPlayerStat = 0;
    bellsEarned.playerStats[0] = 0;
    bellsEarned.playerStats[1] = 0;
    bellsEarned.playerStats[2] = 0;
    bellsEarned.playerStats[3] = 0;
    abdBalance.totalPlayerStat = 0;
    abdBalance.playerStats[0] = 0;
    abdBalance.playerStats[1] = 0;
    abdBalance.playerStats[2] = 0;
    abdBalance.playerStats[3] = 0;
    bellsSpent.totalPlayerStat = 0;
    bellsSpent.playerStats[0] = 0;
    bellsSpent.playerStats[1] = 0;
    bellsSpent.playerStats[2] = 0;
    bellsSpent.playerStats[3] = 0;
    loanPaid.totalPlayerStat = 0;
    loanPaid.playerStats[0] = 0;
    loanPaid.playerStats[1] = 0;
    loanPaid.playerStats[2] = 0;
    loanPaid.playerStats[3] = 0;
    turnipsBought.totalPlayerStat = 0;
    turnipsBought.playerStats[0] = 0;
    turnipsBought.playerStats[1] = 0;
    turnipsBought.playerStats[2] = 0;
    turnipsBought.playerStats[3] = 0;
    turnipsSold.totalPlayerStat = 0;
    turnipsSold.playerStats[0] = 0;
    turnipsSold.playerStats[1] = 0;
    turnipsSold.playerStats[2] = 0;
    turnipsSold.playerStats[3] = 0;
    turnipExpenses.totalPlayerStat = 0;
    turnipExpenses.playerStats[0] = 0;
    turnipExpenses.playerStats[1] = 0;
    turnipExpenses.playerStats[2] = 0;
    turnipExpenses.playerStats[3] = 0;
    turnipProfits.totalPlayerStat = 0;
    turnipProfits.playerStats[0] = 0;
    turnipProfits.playerStats[1] = 0;
    turnipProfits.playerStats[2] = 0;
    turnipProfits.playerStats[3] = 0;
    publicWorkExpenses.totalPlayerStat = 0;
    publicWorkExpenses.playerStats[0] = 0;
    publicWorkExpenses.playerStats[1] = 0;
    publicWorkExpenses.playerStats[2] = 0;
    publicWorkExpenses.playerStats[3] = 0;
    recycleShopEarnings.totalPlayerStat = 0;
    recycleShopEarnings.playerStats[0] = 0;
    recycleShopEarnings.playerStats[1] = 0;
    recycleShopEarnings.playerStats[2] = 0;
    recycleShopEarnings.playerStats[3] = 0;
    flowersWatered.totalPlayerStat = 0;
    flowersWatered.playerStats[0] = 0;
    flowersWatered.playerStats[1] = 0;
    flowersWatered.playerStats[2] = 0;
    flowersWatered.playerStats[3] = 0;
    fishCaught.totalPlayerStat = 0;
    fishCaught.playerStats[0] = 0;
    fishCaught.playerStats[1] = 0;
    fishCaught.playerStats[2] = 0;
    fishCaught.playerStats[3] = 0;
    bugsCaught.totalPlayerStat = 0;
    bugsCaught.playerStats[0] = 0;
    bugsCaught.playerStats[1] = 0;
    bugsCaught.playerStats[2] = 0;
    bugsCaught.playerStats[3] = 0;
    seaCreaturesCaught.totalPlayerStat = 0;
    seaCreaturesCaught.playerStats[0] = 0;
    seaCreaturesCaught.playerStats[1] = 0;
    seaCreaturesCaught.playerStats[2] = 0;
    seaCreaturesCaught.playerStats[3] = 0;
    publicWorksBuilt.totalPlayerStat = 0;
    publicWorksBuilt.playerStats[0] = 0;
    publicWorksBuilt.playerStats[1] = 0;
    publicWorksBuilt.playerStats[2] = 0;
    publicWorksBuilt.playerStats[3] = 0;
    fruitGrown.totalPlayerStat = 0;
    fruitGrown.playerStats[0] = 0;
    fruitGrown.playerStats[1] = 0;
    fruitGrown.playerStats[2] = 0;
    fruitGrown.playerStats[3] = 0;
    perfectFruitGrown.totalPlayerStat = 0;
    perfectFruitGrown.playerStats[0] = 0;
    perfectFruitGrown.playerStats[1] = 0;
    perfectFruitGrown.playerStats[2] = 0;
    perfectFruitGrown.playerStats[3] = 0;
    fruitsGrownOnTheBeach.totalPlayerStat = 0;
    fruitsGrownOnTheBeach.playerStats[0] = 0;
    fruitsGrownOnTheBeach.playerStats[1] = 0;
    fruitsGrownOnTheBeach.playerStats[2] = 0;
    fruitsGrownOnTheBeach.playerStats[3] = 0;
    flowersPlanted.totalPlayerStat = 0;
    flowersPlanted.playerStats[0] = 0;
    flowersPlanted.playerStats[1] = 0;
    flowersPlanted.playerStats[2] = 0;
    flowersPlanted.playerStats[3] = 0;
    treesPlanted.totalPlayerStat = 0;
    treesPlanted.playerStats[0] = 0;
    treesPlanted.playerStats[1] = 0;
    treesPlanted.playerStats[2] = 0;
    treesPlanted.playerStats[3] = 0;
    treesCutDown.totalPlayerStat = 0;
    treesCutDown.playerStats[0] = 0;
    treesCutDown.playerStats[1] = 0;
    treesCutDown.playerStats[2] = 0;
    treesCutDown.playerStats[3] = 0;
    dreamTownsVisited.totalPlayerStat = 0;
    dreamTownsVisited.playerStats[0] = 0;
    dreamTownsVisited.playerStats[1] = 0;
    dreamTownsVisited.playerStats[2] = 0;
    dreamTownsVisited.playerStats[3] = 0;
    kkSliderConcertsAttended.totalPlayerStat = 0;
    kkSliderConcertsAttended.playerStats[0] = 0;
    kkSliderConcertsAttended.playerStats[1] = 0;
    kkSliderConcertsAttended.playerStats[2] = 0;
    kkSliderConcertsAttended.playerStats[3] = 0;
    shrunkSketchesAttended.totalPlayerStat = 0;
    shrunkSketchesAttended.playerStats[0] = 0;
    shrunkSketchesAttended.playerStats[1] = 0;
    shrunkSketchesAttended.playerStats[2] = 0;
    shrunkSketchesAttended.playerStats[3] = 0;
    tourVisits.totalPlayerStat = 0;
    tourVisits.playerStats[0] = 0;
    tourVisits.playerStats[1] = 0;
    tourVisits.playerStats[2] = 0;
    tourVisits.playerStats[3] = 0;
    islandVisits.totalPlayerStat = 0;
    islandVisits.playerStats[0] = 0;
    islandVisits.playerStats[1] = 0;
    islandVisits.playerStats[2] = 0;
    islandVisits.playerStats[3] = 0;
    lettersSent.totalPlayerStat = 0;
    lettersSent.playerStats[0] = 0;
    lettersSent.playerStats[1] = 0;
    lettersSent.playerStats[2] = 0;
    lettersSent.playerStats[3] = 0;
    furnitureCustomized.totalPlayerStat = 0;
    furnitureCustomized.playerStats[0] = 0;
    furnitureCustomized.playerStats[1] = 0;
    furnitureCustomized.playerStats[2] = 0;
    furnitureCustomized.playerStats[3] = 0;
    nooklingsExpenses.totalPlayerStat = 0;
    nooklingsExpenses.playerStats[0] = 0;
    nooklingsExpenses.playerStats[1] = 0;
    nooklingsExpenses.playerStats[2] = 0;
    nooklingsExpenses.playerStats[3] = 0;
    graciesExpenses.totalPlayerStat = 0;
    graciesExpenses.playerStats[0] = 0;
    graciesExpenses.playerStats[1] = 0;
    graciesExpenses.playerStats[2] = 0;
    graciesExpenses.playerStats[3] = 0;
    gardeningShopExpenses.totalPlayerStat = 0;
    gardeningShopExpenses.playerStats[0] = 0;
    gardeningShopExpenses.playerStats[1] = 0;
    gardeningShopExpenses.playerStats[2] = 0;
    gardeningShopExpenses.playerStats[3] = 0;
    weedsPulled.totalPlayerStat = 0;
    weedsPulled.playerStats[0] = 0;
    weedsPulled.playerStats[1] = 0;
    weedsPulled.playerStats[2] = 0;
    weedsPulled.playerStats[3] = 0;
    ableSistersExpenses.totalPlayerStat = 0;
    ableSistersExpenses.playerStats[0] = 0;
    ableSistersExpenses.playerStats[1] = 0;
    ableSistersExpenses.playerStats[2] = 0;
    ableSistersExpenses.playerStats[3] = 0;
    kicksExpenses.totalPlayerStat = 0;
    kicksExpenses.playerStats[0] = 0;
    kicksExpenses.playerStats[1] = 0;
    kicksExpenses.playerStats[2] = 0;
    kicksExpenses.playerStats[3] = 0;
    katrinaVisits.totalPlayerStat = 0;
    katrinaVisits.playerStats[0] = 0;
    katrinaVisits.playerStats[1] = 0;
    katrinaVisits.playerStats[2] = 0;
    katrinaVisits.playerStats[3] = 0;
    streetPassVisitors.totalPlayerStat = 0;
    streetPassVisitors.playerStats[0] = 0;
    streetPassVisitors.playerStats[1] = 0;
    streetPassVisitors.playerStats[2] = 0;
    streetPassVisitors.playerStats[3] = 0;
    townsVisited.totalPlayerStat = 0;
    townsVisited.playerStats[0] = 0;
    townsVisited.playerStats[1] = 0;
    townsVisited.playerStats[2] = 0;
    townsVisited.playerStats[3] = 0;
    townVisitors.totalPlayerStat = 0;
    townVisitors.playerStats[0] = 0;
    townVisitors.playerStats[1] = 0;
    townVisitors.playerStats[2] = 0;
    townVisitors.playerStats[3] = 0;
    saharaRedecorations.totalPlayerStat = 0;
    saharaRedecorations.playerStats[0] = 0;
    saharaRedecorations.playerStats[1] = 0;
    saharaRedecorations.playerStats[2] = 0;
    saharaRedecorations.playerStats[3] = 0;
    jobsAtTheRoost.totalPlayerStat = 0;
    jobsAtTheRoost.playerStats[0] = 0;
    jobsAtTheRoost.playerStats[1] = 0;
    jobsAtTheRoost.playerStats[2] = 0;
    jobsAtTheRoost.playerStats[3] = 0;
    fossilsAnalyzed.totalPlayerStat = 0;
    fossilsAnalyzed.playerStats[0] = 0;
    fossilsAnalyzed.playerStats[1] = 0;
    fossilsAnalyzed.playerStats[2] = 0;
    fossilsAnalyzed.playerStats[3] = 0;
    proDesignsCreated.totalPlayerStat = 0;
    proDesignsCreated.playerStats[0] = 0;
    proDesignsCreated.playerStats[1] = 0;
    proDesignsCreated.playerStats[2] = 0;
    proDesignsCreated.playerStats[3] = 0;
    earnedBadges.totalPlayerStat = 0;
    earnedBadges.playerStats[0] = 0;
    earnedBadges.playerStats[1] = 0;
    earnedBadges.playerStats[2] = 0;
    earnedBadges.playerStats[3] = 0;
    helpedGulliver.totalPlayerStat = 0;
    helpedGulliver.playerStats[0] = 0;
    helpedGulliver.playerStats[1] = 0;
    helpedGulliver.playerStats[2] = 0;
    helpedGulliver.playerStats[3] = 0;
    shampoodleVisits.totalPlayerStat = 0;
    shampoodleVisits.playerStats[0] = 0;
    shampoodleVisits.playerStats[1] = 0;
    shampoodleVisits.playerStats[2] = 0;
    shampoodleVisits.playerStats[3] = 0;
    boughtArtAtRedds.totalPlayerStat = 0;
    boughtArtAtRedds.playerStats[0] = 0;
    boughtArtAtRedds.playerStats[1] = 0;
    boughtArtAtRedds.playerStats[2] = 0;
    boughtArtAtRedds.playerStats[3] = 0;
    scallopsGivenToPascal.totalPlayerStat = 0;
    scallopsGivenToPascal.playerStats[0] = 0;
    scallopsGivenToPascal.playerStats[1] = 0;
    scallopsGivenToPascal.playerStats[2] = 0;
    scallopsGivenToPascal.playerStats[3] = 0;
    helpedKatieTravel.totalPlayerStat = 0;
    helpedKatieTravel.playerStats[0] = 0;
    helpedKatieTravel.playerStats[1] = 0;
    helpedKatieTravel.playerStats[2] = 0;
    helpedKatieTravel.playerStats[3] = 0;
    balloonsPopped.totalPlayerStat = 0;
    balloonsPopped.playerStats[0] = 0;
    balloonsPopped.playerStats[1] = 0;
    balloonsPopped.playerStats[2] = 0;
    balloonsPopped.playerStats[3] = 0;
    tourneyFishCaught.totalPlayerStat = 0;
    tourneyFishCaught.playerStats[0] = 0;
    tourneyFishCaught.playerStats[1] = 0;
    tourneyFishCaught.playerStats[2] = 0;
    tourneyFishCaught.playerStats[3] = 0;
    tourneyInsectCaught.totalPlayerStat = 0;
    tourneyInsectCaught.playerStats[0] = 0;
    tourneyInsectCaught.playerStats[1] = 0;
    tourneyInsectCaught.playerStats[2] = 0;
    tourneyInsectCaught.playerStats[3] = 0;
    festivalFeathersCaught.totalPlayerStat = 0;
    festivalFeathersCaught.playerStats[0] = 0;
    festivalFeathersCaught.playerStats[1] = 0;
    festivalFeathersCaught.playerStats[2] = 0;
    festivalFeathersCaught.playerStats[3] = 0;
    eggsGivenToZipper.totalPlayerStat = 0;
    eggsGivenToZipper.playerStats[0] = 0;
    eggsGivenToZipper.playerStats[1] = 0;
    eggsGivenToZipper.playerStats[2] = 0;
    eggsGivenToZipper.playerStats[3] = 0;
    foundImposterBlanca.totalPlayerStat = 0;
    foundImposterBlanca.playerStats[0] = 0;
    foundImposterBlanca.playerStats[1] = 0;
    foundImposterBlanca.playerStats[2] = 0;
    foundImposterBlanca.playerStats[3] = 0;
    fireworkDesignsGivenToIsabelle.totalPlayerStat = 0;
    fireworkDesignsGivenToIsabelle.playerStats[0] = 0;
    fireworkDesignsGivenToIsabelle.playerStats[1] = 0;
    fireworkDesignsGivenToIsabelle.playerStats[2] = 0;
    fireworkDesignsGivenToIsabelle.playerStats[3] = 0;
    receivedCandyOnHalloween.totalPlayerStat = 0;
    receivedCandyOnHalloween.playerStats[0] = 0;
    receivedCandyOnHalloween.playerStats[1] = 0;
    receivedCandyOnHalloween.playerStats[2] = 0;
    receivedCandyOnHalloween.playerStats[3] = 0;
    harvestFestivalCoursesDone.totalPlayerStat = 0;
    harvestFestivalCoursesDone.playerStats[0] = 0;
    harvestFestivalCoursesDone.playerStats[1] = 0;
    harvestFestivalCoursesDone.playerStats[2] = 0;
    harvestFestivalCoursesDone.playerStats[3] = 0;
    snowmenBuiltByYouInTown.totalPlayerStat = 0;
    snowmenBuiltByYouInTown.playerStats[0] = 0;
    snowmenBuiltByYouInTown.playerStats[1] = 0;
    snowmenBuiltByYouInTown.playerStats[2] = 0;
    snowmenBuiltByYouInTown.playerStats[3] = 0;
    givenToyDayPresents.totalPlayerStat = 0;
    givenToyDayPresents.playerStats[0] = 0;
    givenToyDayPresents.playerStats[1] = 0;
    givenToyDayPresents.playerStats[2] = 0;
    givenToyDayPresents.playerStats[3] = 0;
    snowflakesCaught.totalPlayerStat = 0;
    snowflakesCaught.playerStats[0] = 0;
    snowflakesCaught.playerStats[1] = 0;
    snowflakesCaught.playerStats[2] = 0;
    snowflakesCaught.playerStats[3] = 0;
    partyPoppersPoppedAtNewYears.totalPlayerStat = 0;
    partyPoppersPoppedAtNewYears.playerStats[0] = 0;
    partyPoppersPoppedAtNewYears.playerStats[1] = 0;
    partyPoppersPoppedAtNewYears.playerStats[2] = 0;
    partyPoppersPoppedAtNewYears.playerStats[3] = 0;
    totalBingoWins.totalPlayerStat = 0;
    totalBingoWins.playerStats[0] = 0;
    totalBingoWins.playerStats[1] = 0;
    totalBingoWins.playerStats[2] = 0;
    totalBingoWins.playerStats[3] = 0;
    shootingStarWishes.totalPlayerStat = 0;
    shootingStarWishes.playerStats[0] = 0;
    shootingStarWishes.playerStats[1] = 0;
    shootingStarWishes.playerStats[2] = 0;
    shootingStarWishes.playerStats[3] = 0;
    npcPicturesReceived.totalPlayerStat = 0;
    npcPicturesReceived.playerStats[0] = 0;
    npcPicturesReceived.playerStats[1] = 0;
    npcPicturesReceived.playerStats[2] = 0;
    npcPicturesReceived.playerStats[3] = 0;
    lostItemsGivenBack.totalPlayerStat = 0;
    lostItemsGivenBack.playerStats[0] = 0;
    lostItemsGivenBack.playerStats[1] = 0;
    lostItemsGivenBack.playerStats[2] = 0;
    lostItemsGivenBack.playerStats[3] = 0;
    youVisitedVillagers.totalPlayerStat = 0;
    youVisitedVillagers.playerStats[0] = 0;
    youVisitedVillagers.playerStats[1] = 0;
    youVisitedVillagers.playerStats[2] = 0;
    youVisitedVillagers.playerStats[3] = 0;
    villagersVisitedYou.totalPlayerStat = 0;
    villagersVisitedYou.playerStats[0] = 0;
    villagersVisitedYou.playerStats[1] = 0;
    villagersVisitedYou.playerStats[2] = 0;
    villagersVisitedYou.playerStats[3] = 0;
    playedHideAndSeek.totalPlayerStat = 0;
    playedHideAndSeek.playerStats[0] = 0;
    playedHideAndSeek.playerStats[1] = 0;
    playedHideAndSeek.playerStats[2] = 0;
    playedHideAndSeek.playerStats[3] = 0;
    unk_0x550.totalPlayerStat = 0;
    unk_0x550.playerStats[0] = 0;
    unk_0x550.playerStats[1] = 0;
    unk_0x550.playerStats[2] = 0;
    unk_0x550.playerStats[3] = 0;
    unk_0x564.totalPlayerStat = 0;
    unk_0x564.playerStats[0] = 0;
    unk_0x564.playerStats[1] = 0;
    unk_0x564.playerStats[2] = 0;
    unk_0x564.playerStats[3] = 0;
    amiiboScanned.totalPlayerStat = 0;
    amiiboScanned.playerStats[0] = 0;
    amiiboScanned.playerStats[1] = 0;
    amiiboScanned.playerStats[2] = 0;
    amiiboScanned.playerStats[3] = 0;
    resetSurveillanceCenterVisits.totalPlayerStat = 0;
    resetSurveillanceCenterVisits.playerStats[0] = 0;
    resetSurveillanceCenterVisits.playerStats[1] = 0;
    resetSurveillanceCenterVisits.playerStats[2] = 0;
    resetSurveillanceCenterVisits.playerStats[3] = 0;
    djkkVisits.totalPlayerStat = 0;
    djkkVisits.playerStats[0] = 0;
    djkkVisits.playerStats[1] = 0;
    djkkVisits.playerStats[2] = 0;
    djkkVisits.playerStats[3] = 0;
    goldenRosesCreated.totalPlayerStat = 0;
    goldenRosesCreated.playerStats[0] = 0;
    goldenRosesCreated.playerStats[1] = 0;
    goldenRosesCreated.playerStats[2] = 0;
    goldenRosesCreated.playerStats[3] = 0;
    blueRosesCreated.totalPlayerStat = 0;
    blueRosesCreated.playerStats[0] = 0;
    blueRosesCreated.playerStats[1] = 0;
    blueRosesCreated.playerStats[2] = 0;
    blueRosesCreated.playerStats[3] = 0;
    famousMushroomsEaten.totalPlayerStat = 0;
    famousMushroomsEaten.playerStats[0] = 0;
    famousMushroomsEaten.playerStats[1] = 0;
    famousMushroomsEaten.playerStats[2] = 0;
    famousMushroomsEaten.playerStats[3] = 0;
    townInitiativesCompleted.totalPlayerStat = 0;
    townInitiativesCompleted.playerStats[0] = 0;
    townInitiativesCompleted.playerStats[1] = 0;
    townInitiativesCompleted.playerStats[2] = 0;
    townInitiativesCompleted.playerStats[3] = 0;
    meowCouponsEarned.totalPlayerStat = 0;
    meowCouponsEarned.playerStats[0] = 0;
    meowCouponsEarned.playerStats[1] = 0;
    meowCouponsEarned.playerStats[2] = 0;
    meowCouponsEarned.playerStats[3] = 0;
    meowCouponsSpent.totalPlayerStat = 0;
    meowCouponsSpent.playerStats[0] = 0;
    meowCouponsSpent.playerStats[1] = 0;
    meowCouponsSpent.playerStats[2] = 0;
    meowCouponsSpent.playerStats[3] = 0;
    unk_0x62C.totalPlayerStat = 0;
    unk_0x62C.playerStats[0] = 0;
    unk_0x62C.playerStats[1] = 0;
    unk_0x62C.playerStats[2] = 0;
    unk_0x62C.playerStats[3] = 0;
    std::memset(insectCatchCounts, 0, sizeof(insectCatchCounts));
    for (u32 i = 0; i < INSECT_NUM; i++) {
        insectCatchCountsPerPlayer[i][0] = 0;
        insectCatchCountsPerPlayer[i][1] = 0;
        insectCatchCountsPerPlayer[i][2] = 0;
        insectCatchCountsPerPlayer[i][3] = 0;
    }
    std::memset(fishCatchCounts, 0, sizeof(fishCatchCounts));
    for (u32 i = 0; i < FISH_NUM; i++) {
        fishCatchCountsPerPlayer[i][0] = 0;
        fishCatchCountsPerPlayer[i][1] = 0;
        fishCatchCountsPerPlayer[i][2] = 0;
        fishCatchCountsPerPlayer[i][3] = 0;
    }
    std::memset(junkCatchCounts, 0, sizeof(junkCatchCounts));
    for (u32 i = 0; i < JUNK_NUM; i++) {
        junkCatchCountsPerPlayer[i][0] = 0;
        junkCatchCountsPerPlayer[i][1] = 0;
        junkCatchCountsPerPlayer[i][2] = 0;
        junkCatchCountsPerPlayer[i][3] = 0;
    }
    std::memset(seaCreatureCatchCounts, 0, sizeof(seaCreatureCatchCounts));
    for (u32 i = 0; i < SEA_CREATURE_NUM; i++) {
        seaCreatureCatchCountsPerPlayer[i][0] = 0;
        seaCreatureCatchCountsPerPlayer[i][1] = 0;
        seaCreatureCatchCountsPerPlayer[i][2] = 0;
        seaCreatureCatchCountsPerPlayer[i][3] = 0;
    }
}
