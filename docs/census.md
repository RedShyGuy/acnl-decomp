# Census (SvCensus)

80 statistics values of the town (bells, catches, visits, events), each per player and as a town total, plus catch
counts per insect, fish, junk item and sea creature. The census menu is unused; the player flag
`SvPlayerFlags::canUseCensusMenu` (bit 0x2F.6, `SvPlayer` + 0x5700) enables it. The values are counted regardless.

Code: [src/Sv/dSvCensus.cpp](../src/Sv/dSvCensus.cpp) (0x6B59E8–0x6BA2A8, one function per value and operation),
[src/Sv/dSvCensusData.cpp](../src/Sv/dSvCensusData.cpp) (0x2A6BCC–0x2A7220, constructor and event resets),
[src/Sv/dSvChecksum.cpp](../src/Sv/dSvChecksum.cpp) (checksum).

## Location

`SvCensusData` lies in the shared data of the save (`SvSharedData::playerStats`, GardenPlus + 0x7250C):

| Offset | Size | Content |
|---|---|---|
| 0x0000 | 4 | CRC-32 (MSB first) over the rest (0x1444 bytes) |
| 0x0004 | 0x1414 | `SvCensus` (the values and catch counts) |
| 0x1418 | 12 × 4 | the date of the last event day of each event value (`SvDate`: u16 year, u8 month, u8 day) |

`GetCensus()` (0x2FD00C) and `GetCensusOfGardenPlus()` (0x6B618C) return the census.

## Values

Each value is a `SvCensusValue` (0x14 bytes): the town total (`totalPlayerStat`) and one word per player
(`playerStats[4]`, indexed by `SvPlayer::townSlot`, see `SvGetCurrentTownSlot()`).

- Nothing changes while visiting a dream town (`netgame::TYPE_DREAM`).
- Values are clamped to [0, limit]; the sum is computed in 64 bits, so negative changes work. Limit 999 999 999 for
  amounts of bells, 9 999 for counts, 99 for `publicWorksBuilt`.
- `Add<Value>`: current player, then `Add<Value>Total`. `…ForSlot`: player given by town slot.
- `Set<Value>`: sets the player value; the total only changes by the part above the limit.
- `UpdateAbdBalance` (0x6B9808, ForSlot 0x6B9018): the total changes by the difference to the old player value, then the
  player value is set. Called after bank transfers in network games.
- `Clear<Value>`: sets total and player values to 0.
- `ClearPlayer(townSlot)` (0x6B59E8) clears one player column (values and catch counts), `Clear()` (0x6B9B88) everything.

Index = position in the census, also used as id by the event code. "Functions" lists the census functions of a value
that exist in the binary.

| # | Offset | Value | Limit | Functions | Note |
|---|---|---|---|---|---|
| 0 | 0x000 | `bellsEarned` | 999 999 999 | `AddBellsEarnedTotal` 0x6B6290, `AddBellsEarnedForSlot` 0x6B7174, `AddBellsEarned` 0x6B7ED0 |  |
| 1 | 0x014 | `abdBalance` | 999 999 999 | `AddAbdBalanceTotal` 0x6B67AC, `SetAbdBalanceForSlot` 0x6B7558, `SetAbdBalance` 0x6B8608, `UpdateAbdBalanceForSlot` 0x6B9018, `UpdateAbdBalance` 0x6B9808 |  |
| 2 | 0x028 | `bellsSpent` | 999 999 999 | `AddBellsSpentTotal` 0x6B7620, `AddBellsSpent` 0x6B9468 |  |
| 3 | 0x03C | `loanPaid` | 999 999 999 | `AddLoanPaidTotal` 0x6B7484, `AddLoanPaid` 0x6B9380 |  |
| 4 | 0x050 | `turnipsBought` | 9 999 | `AddTurnipsBoughtTotal` 0x6B7308, `AddTurnipsBought` 0x6B930C |  |
| 5 | 0x064 | `turnipsSold` | 9 999 | `AddTurnipsSoldTotal` 0x6B7930, `AddTurnipsSold` 0x6B9638 |  |
| 6 | 0x078 | `turnipExpenses` | 999 999 999 | `AddTurnipExpensesTotal` 0x6B7D04, `AddTurnipExpenses` 0x6B985C |  |
| 7 | 0x08C | `turnipProfits` | 999 999 999 | `AddTurnipProfitsTotal` 0x6B84A4, `AddTurnipProfits` 0x6B99B8 |  |
| 8 | 0x0A0 | `publicWorkExpenses` | 999 999 999 | `AddPublicWorkExpensesTotal` 0x6B6A2C, `AddPublicWorkExpenses` 0x6B8900 |  |
| 9 | 0x0B4 | `recycleShopEarnings` | 999 999 999 | `AddRecycleShopEarningsTotal` 0x6B6B1C, `AddRecycleShopEarnings` 0x6B8A5C |  |
| 10 | 0x0C8 | `flowersWatered` | 9 999 | `AddFlowersWateredTotal` 0x6B6F78, `AddFlowersWatered` 0x6B8FA4 |  |
| 11 | 0x0DC | `fishCaught` | 9 999 | `AddFishCaughtTotal` 0x6B693C, `AddFishCaught` 0x6B87A4 |  |
| 12 | 0x0F0 | `bugsCaught` | 9 999 | `AddBugsCaughtTotal` 0x6B6D48, `AddBugsCaught` 0x6B8C90 |  |
| 13 | 0x104 | `seaCreaturesCaught` | 9 999 | `AddSeaCreaturesCaughtTotal` 0x6B657C, `AddSeaCreaturesCaught` 0x6B8178 |  |
| 14 | 0x118 | `publicWorksBuilt` | 99 | `SetPublicWorksBuiltTotal` 0x6B7524 | NON_PLAYER_SPECIFIC |
| 15 | 0x12C | `fruitGrown` | – | – | NON_PLAYER_SPECIFIC |
| 16 | 0x140 | `perfectFruitGrown` | – | – | NON_PLAYER_SPECIFIC |
| 17 | 0x154 | `fruitsGrownOnTheBeach` | – | – | NON_PLAYER_SPECIFIC |
| 18 | 0x168 | `flowersPlanted` | 9 999 | `AddFlowersPlantedTotal` 0x6B5FC4, `AddFlowersPlanted` 0x6B78BC |  |
| 19 | 0x17C | `treesPlanted` | 9 999 | `AddTreesPlantedTotal` 0x6B62E0, `AddTreesPlanted` 0x6B7F44 |  |
| 20 | 0x190 | `treesCutDown` | 9 999 | `AddTreesCutDownTotal` 0x6B5DE0, `AddTreesCutDown` 0x6B73C0 |  |
| 21 | 0x1A4 | `dreamTownsVisited` | 9 999 | `AddDreamTownsVisitedTotal` 0x6B6D98, `AddDreamTownsVisited` 0x6B8D04 |  |
| 22 | 0x1B8 | `kkSliderConcertsAttended` | 9 999 | `AddKkSliderConcertsAttendedTotal` 0x6B6F28, `AddKkSliderConcertsAttended` 0x6B8F30 |  |
| 23 | 0x1CC | `shrunkSketchesAttended` | 9 999 | `AddShrunkSketchesAttendedTotal` 0x6B6380, `AddShrunkSketchesAttended` 0x6B802C |  |
| 24 | 0x1E0 | `tourVisits` | 9 999 | `AddTourVisitsTotal` 0x6B6064, `AddTourVisits` 0x6B7AE4 |  |
| 25 | 0x1F4 | `islandVisits` | 9 999 | `AddIslandVisitsTotal` 0x6B666C, `AddIslandVisits` 0x6B82D4 |  |
| 26 | 0x208 | `lettersSent` | 9 999 | `AddLettersSentTotal` 0x6B6014, `AddLettersSent` 0x6B7980 |  |
| 27 | 0x21C | `furnitureCustomized` | 9 999 | `AddFurnitureCustomizedTotal` 0x6B675C, `AddFurnitureCustomized` 0x6B8430 |  |
| 28 | 0x230 | `nooklingsExpenses` | 999 999 999 | `AddNooklingsExpensesTotal` 0x6B8544, `AddNooklingsExpenses` 0x6B9AA0 |  |
| 29 | 0x244 | `graciesExpenses` | 999 999 999 | `AddGraciesExpensesTotal` 0x6B71DC, `AddGraciesExpenses` 0x6B9298 |  |
| 30 | 0x258 | `gardeningShopExpenses` | 999 999 999 | `AddGardeningShopExpensesTotal` 0x6B84F4, `AddGardeningShopExpenses` 0x6B9A2C |  |
| 31 | 0x26C | `weedsPulled` | 9 999 | `AddWeedsPulledTotal` 0x6B60B4, `AddWeedsPulled` 0x6B7BA8 |  |
| 32 | 0x280 | `ableSistersExpenses` | 999 999 999 | `AddAbleSistersExpensesTotal` 0x6B74D4, `AddAbleSistersExpenses` 0x6B93F4 |  |
| 33 | 0x294 | `kicksExpenses` | 999 999 999 | `AddKicksExpensesTotal` 0x6B6ED8, `AddKicksExpenses` 0x6B8E54 |  |
| 34 | 0x2A8 | `katrinaVisits` | 9 999 | `AddKatrinaVisitsTotal` 0x6B79F4, `AddKatrinaVisits` 0x6B96AC |  |
| 35 | 0x2BC | `streetPassVisitors` | 9 999 | `AddStreetPassVisitorsTotal` 0x6B5D90 | NON_PLAYER_SPECIFIC |
| 36 | 0x2D0 | `townsVisited` | 9 999 | `AddTownsVisitedTotal` 0x6B63D0, `AddTownsVisited` 0x6B80A0 |  |
| 37 | 0x2E4 | `townVisitors` | 9 999 | `AddTownVisitorsTotal` 0x6B6BBC, `AddTownVisitors` 0x6B8B44 |  |
| 38 | 0x2F8 | `saharaRedecorations` | 9 999 | `AddSaharaRedecorationsTotal` 0x6B670C, `AddSaharaRedecorations` 0x6B83BC |  |
| 39 | 0x30C | `jobsAtTheRoost` | 9 999 | `AddJobsAtTheRoostTotal` 0x6B5E84, `AddJobsAtTheRoost` 0x6B75AC |  |
| 40 | 0x320 | `fossilsAnalyzed` | 9 999 | `AddFossilsAnalyzedTotal` 0x6B6FE4, `AddFossilsAnalyzed` 0x6B9054 |  |
| 41 | 0x334 | `proDesignsCreated` | 9 999 | `AddProDesignsCreatedTotal` 0x6B70D4, `AddProDesignsCreated` 0x6B91B0 |  |
| 42 | 0x348 | `earnedBadges` | 9 999 | `AddEarnedBadgesTotal` 0x6B61A0, `SetEarnedBadgesForSlot` 0x6B722C, `SetEarnedBadges` 0x6B8114 |  |
| 43 | 0x35C | `helpedGulliver` | 9 999 | `AddHelpedGulliverTotal` 0x6B5F74, `AddHelpedGulliver` 0x6B7848 |  |
| 44 | 0x370 | `shampoodleVisits` | 9 999 | `AddShampoodleVisitsTotal` 0x6B6330, `AddShampoodleVisits` 0x6B7FB8 |  |
| 45 | 0x384 | `boughtArtAtRedds` | 9 999 | `AddBoughtArtAtReddsTotal` 0x6B69DC, `AddBoughtArtAtRedds` 0x6B888C |  |
| 46 | 0x398 | `scallopsGivenToPascal` | 9 999 | `AddScallopsGivenToPascalTotal` 0x6B6ACC, `AddScallopsGivenToPascal` 0x6B89E8 |  |
| 47 | 0x3AC | `helpedKatieTravel` | 9 999 | `AddHelpedKatieTravelTotal` 0x6B6240, `AddHelpedKatieTravel` 0x6B7E5C |  |
| 48 | 0x3C0 | `balloonsPopped` | 9 999 | `AddBalloonsPoppedTotal` 0x6B684C, `AddBalloonsPopped` 0x6B866C |  |
| 49 | 0x3D4 | `tourneyFishCaught` | 9 999 | `ClearTourneyFishCaught` 0x6B6C0C, `AddTourneyFishCaughtTotal` 0x6B76E4, `AddTourneyFishCaught` 0x6B94DC | Get cleared after event |
| 50 | 0x3E8 | `tourneyInsectCaught` | 9 999 | `ClearTourneyInsectCaught` 0x6B6FC8, `AddTourneyInsectCaughtTotal` 0x6B7D54, `AddTourneyInsectCaught` 0x6B98D0 | Get cleared after event |
| 51 | 0x3FC | `festivalFeathersCaught` | 9 999 | `ClearFestivalFeathersCaught` 0x6B6154, `AddFestivalFeathersCaughtTotal` 0x6B6CF8, `AddFestivalFeathersCaught` 0x6B8C1C |  |
| 52 | 0x410 | `eggsGivenToZipper` | 9 999 | `ClearEggsGivenToZipper` 0x6B5D18, `AddEggsGivenToZipperTotal` 0x6B65CC, `AddEggsGivenToZipper` 0x6B81EC |  |
| 53 | 0x424 | `foundImposterBlanca` | 9 999 | `ClearFoundImposterBlanca` 0x6B6420, `AddFoundImposterBlancaTotal` 0x6B7034, `AddFoundImposterBlanca` 0x6B90C8 |  |
| 54 | 0x438 | `fireworkDesignsGivenToIsabelle` | 9 999 | `ClearFireworkDesignsGivenToIsabelle` 0x6B6170, `AddFireworkDesignsGivenToIsabelleTotal` 0x6B6DE8, `AddFireworkDesignsGivenToIsabelleForSlot` 0x6B7DF4 | will be set right after the fireworks started |
| 55 | 0x44C | `receivedCandyOnHalloween` | 9 999 | `ClearReceivedCandyOnHalloween` 0x6B6458, `AddReceivedCandyOnHalloweenTotal` 0x6B7124, `AddReceivedCandyOnHalloween` 0x6B9224 |  |
| 56 | 0x460 | `harvestFestivalCoursesDone` | 9 999 | `ClearHarvestFestivalCoursesDone` 0x6B5E4C, `AddHarvestFestivalCoursesDoneTotal` 0x6B698C, `AddHarvestFestivalCoursesDone` 0x6B8818 |  |
| 57 | 0x474 | `snowmenBuiltByYouInTown` | 9 999 | `ClearSnowmenBuiltByYouInTown` 0x6B5E68, `AddSnowmenBuiltByYouInTownTotal` 0x6B6B6C, `AddSnowmenBuiltByYouInTown` 0x6B8AD0 |  |
| 58 | 0x488 | `givenToyDayPresents` | 9 999 | `ClearGivenToyDayPresents` 0x6B643C, `AddGivenToyDayPresentsTotal` 0x6B7084, `AddGivenToyDayPresents` 0x6B913C |  |
| 59 | 0x49C | `snowflakesCaught` | 9 999 | `AddSnowflakesCaughtTotal` 0x6B7A44, `AddSnowflakesCaught` 0x6B9720 |  |
| 60 | 0x4B0 | `partyPoppersPoppedAtNewYears` | 9 999 | `ClearPartyPoppersPoppedAtNewYears` 0x6B5E30, `AddPartyPoppersPoppedAtNewYearsTotal` 0x6B68EC, `AddPartyPoppersPoppedAtNewYears` 0x6B86E0 |  |
| 61 | 0x4C4 | `totalBingoWins` | 9 999 | `ClearTotalBingoWins` 0x6B5CFC, `AddTotalBingoWinsTotal` 0x6B61F0, `AddTotalBingoWins` 0x6B7C90 |  |
| 62 | 0x4D8 | `shootingStarWishes` | 9 999 | `AddShootingStarWishesTotal` 0x6B66BC, `AddShootingStarWishes` 0x6B8348 |  |
| 63 | 0x4EC | `npcPicturesReceived` | 9 999 | `AddNpcPicturesReceivedTotal` 0x6B689C, `SetNpcPicturesReceived` 0x6B8BB8 |  |
| 64 | 0x500 | `lostItemsGivenBack` | 9 999 | `AddLostItemsGivenBackTotal` 0x6B77F8, `AddLostItemsGivenBack` 0x6B95C4 |  |
| 65 | 0x514 | `youVisitedVillagers` | 9 999 | `AddYouVisitedVillagersTotal` 0x6B7B58, `AddYouVisitedVillagers` 0x6B9794 |  |
| 66 | 0x528 | `villagersVisitedYou` | 9 999 | `AddVillagersVisitedYouTotal` 0x6B7DA4, `AddVillagersVisitedYou` 0x6B9944 |  |
| 67 | 0x53C | `playedHideAndSeek` | 9 999 | `AddPlayedHideAndSeekTotal` 0x6B5F24, `AddPlayedHideAndSeek` 0x6B7734 |  |
| 68 | 0x550 | `unk_0x550` | 9 999 | `AddUnk0x550Total` 0x6B6E88, `AddUnk0x550` 0x6B8DE0 | KOKT71 |
| 69 | 0x564 | `unk_0x564` | 9 999 | `AddUnk0x564Total` 0x6B7A94, `AddUnk0x564ForSlot` 0x6B8EC8 | KOKT72 |
| 70 | 0x578 | `amiiboScanned` | 9 999 | `AddAmiiboScannedTotal` 0x6B6104, `AddAmiiboScanned` 0x6B7C1C |  |
| 71 | 0x58C | `resetSurveillanceCenterVisits` | 9 999 | `AddResetSurveillanceCenterVisitsTotal` 0x6B6A7C, `AddResetSurveillanceCenterVisits` 0x6B8974 |  |
| 72 | 0x5A0 | `djkkVisits` | 9 999 | `AddDjkkVisitsTotal` 0x6B5ED4, `AddDjkkVisits` 0x6B7670 |  |
| 73 | 0x5B4 | `goldenRosesCreated` | 9 999 | `AddGoldenRosesCreatedTotal` 0x6B6E38 | NON_PLAYER_SPECIFIC |
| 74 | 0x5C8 | `blueRosesCreated` | 9 999 | `AddBlueRosesCreatedTotal` 0x6B6CA8 | NON_PLAYER_SPECIFIC |
| 75 | 0x5DC | `famousMushroomsEaten` | 9 999 | `AddFamousMushroomsEatenTotal` 0x6B8754, `AddFamousMushroomsEaten` 0x6B9B14 |  |
| 76 | 0x5F0 | `townInitiativesCompleted` | 9 999 | `AddTownInitiativesCompletedTotal` 0x6B77A8, `AddTownInitiativesCompletedForSlot` 0x6B8D78, `AddTownInitiativesCompleted` 0x6B9550 |  |
| 77 | 0x604 | `meowCouponsEarned` | 9 999 | `AddMeowCouponsEarnedTotal` 0x6B661C, `AddMeowCouponsEarnedForSlot` 0x6B7358, `AddMeowCouponsEarned` 0x6B8260 |  |
| 78 | 0x618 | `meowCouponsSpent` | 9 999 | `AddMeowCouponsSpentTotal` 0x6B67FC, `AddMeowCouponsSpent` 0x6B8594 |  |
| 79 | 0x62C | `unk_0x62C` | 9 999 | `AddUnk0x62CTotal` 0x6B7434 | NON_PLAYER_SPECIFIC / KOKT82 |

## Catch counts

| Offset | Content |
|---|---|
| 0x0640 | insects caught, total, `u32[72]` (index = `SvCensusInsect`) |
| 0x0760 | insects per player, `u32[72][4]` (index × 16 + town slot × 4) |
| 0x0BE0 | fish, total, `u32[72]` (`SvCensusFish`) |
| 0x0D00 | fish per player, `u32[72][4]` |
| 0x1180 | empty can, boot, old tire, total, `u32[3]` (`SvCensusJunk`) |
| 0x118C | the same per player, `u32[3][4]` |
| 0x11BC | sea creatures, total, `u32[30]` (`SvCensusSeaCreature`) |
| 0x1234 | sea creatures per player, `u32[30][4]` |

Limit 9 999. Functions: `Add<Kind>Catch(index, value)` (player and total) and `Add<Kind>CatchTotal(index, value)` for
Insect, Fish, Junk and SeaCreature.

## Event values

Thirteen values only count during their event and are reset at the next one (`SvCensusData::UpdateEventValues`
0x2A6E18, clear functions in the table 0x88F54C). Each value stores the date of its last event day. If the stored date
is neither today nor yesterday, the value is cleared; if the event takes place today, the date is set to today.
`Func4DD108(index)` decides whether the event of a value takes place today:

| # | Value | Event |
|---|---|---|
| 49 | `tourneyFishCaught` | Fishing Tourney (event 0) |
| 50 | `tourneyInsectCaught` | Bug-Off (event 12) |
| 51 | `festivalFeathersCaught` | Festivale (event 9) |
| 52 | `eggsGivenToZipper` | Bunny Day (event 10) |
| 53 | `foundImposterBlanca` | April Fools' Day (event 13) |
| 54 | `fireworkDesignsGivenToIsabelle` | Fireworks Show (event 8) |
| 55 | `receivedCandyOnHalloween` | Halloween (event 2) |
| 56 | `harvestFestivalCoursesDone` | Harvest Festival (event 5) |
| 57 | `snowmenBuiltByYouInTown` | winter check 0x63061C |
| 58 | `givenToyDayPresents` | Toy Day (event 4) |
| 59 | `snowflakesCaught` | January or December, and the player already has snowflakes |
| 60 | `partyPoppersPoppedAtNewYears` | January 1 before 6:00 |
| 61 | `totalBingoWins` | winter check 0x63061C or 0x226D48 |

- Snowflakes (59) have no date and no clear function of their own; they are cleared when the snowmen date (57) lies in
  the same winter (November to the following year).
- Bingo (61) is not cleared when its date lies in the same winter.
- `ClearEventDates` (0x2A6BE8) clears only the first ten dates; the dates of the party poppers and the bingo, whose
  events span the new year, are kept.

### Bugs
- The winter of the stored date is computed with `year > 10` instead of `month > 10`, so the stored year always counts
  one year later.

## Readers

- `0x5D10F8`: for each census index 0..83, whether the entry is shown. Event values (49..61) via their event check, most
  others when the current player's value is above 0 (getter copies at 0x768xxx); indices 0..20, 28, 31, 32 and 76..83
  always. Reads via `GetCensusOfGardenPlus()`.
- `0x5D1918`: switch over the same indices that returns the value.

## Unknown values

- `unk_0x550` (index 68): only `AddUnk0x550` (0x6B8DE0) adds to it, called only from ModuleIndoor (thunk +0x392C, call
  at +0x4AC98) in a talk handler of a villager indoors (flow event 24, near `AcNpcIn::InTalkRcpt`). Before that, the
  player's town (`SvPlayer::playerInfo`) is written into `SvNpcData` + 0x172F8 (up to three 22-byte entries, count at
  +0xAB, date at +0xA2; functions 0x5BD3AC/0x5BD354).
- `unk_0x564` (index 69): only `AddUnk0x564ForSlot` (0x6B8EC8) adds to it, from 0x5BADA0 (state machine of the entries
  at `SvNpcData` + 0x1744C, two entries of 0x40 per player, the time capsule). +1 for the entry's player when an entry
  goes from state 0/1 to state 3 and the date bytes +0x3C/+0x3D are set. Probably time capsules buried.
- `unk_0x62C` (index 79, total only): only `AddUnk0x62CTotal` (0x6B7434) adds to it, from ModuleVillage
  `LytTicketExchangeSmaphoLinkSeq` (+0x139D0). Reads the current player's 6-byte entry at `SvStorageDataUnk153FC` +
  0xC90 (0x2B8858 has tickets, 0x2B8820 count), adds the count to this total, gives MEOW coupons for it (0x2B8890 →
  `AddMeowCouponsEarned`) and clears the entry (0x2B88C8). Counts tickets exchanged via smartphone link.

## Open

- The menu/layout that would show the census.
- The meaning of `unk_0x550` and `unk_0x564`; where the smartphone tickets come from.
