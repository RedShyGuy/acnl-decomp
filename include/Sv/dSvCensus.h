#pragma once

// Census (statistics) data of all players.
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "nn/fnd/fnd_DateTime.h"

struct SvCensusValue {
    /* 0x0000 */ u32 totalPlayerStat; // All player stats combined
    /* 0x0004 */ u32 playerStats[4]; // For each player

    // (inline; name is ours) everything 0
    void Clear()
    {
        totalPlayerStat = 0;
        playerStats[0] = 0;
        playerStats[1] = 0;
        playerStats[2] = 0;
        playerStats[3] = 0;
    }
};
ASSERT_SIZE(SvCensusValue, 0x14);

// The indices of the catch counts of the census (the order of the research names of the save data; the
// census code indexes the tables with them).
enum SvCensusInsect {
    INSECT_COMMON_BUTTERFLY = 0,
    INSECT_YELLOW_BUTTERFLY = 1,
    INSECT_TIGER_BUTTERFLY = 2,
    INSECT_PEACOCK_BUTTERFLY = 3,
    INSECT_MONARCH_BUTTERFLY = 4,
    INSECT_EMPEROR_BUTTERFLY = 5,
    INSECT_AGRIAS_BUTTERFLY = 6,
    INSECT_RAJA_B = 7,
    INSECT_BIRDWING_BUTTERFLY = 8,
    INSECT_MOTH = 9,
    INSECT_OAK_SILK_MOTH = 10,
    INSECT_HONEYBEE = 11,
    INSECT_BEE = 12,
    INSECT_LONG_LOCUST = 13,
    INSECT_MIGRATORY_LOCUST = 14,
    INSECT_RICE_GRASSHOPPER = 15,
    INSECT_MANTIS = 16,
    INSECT_ORCHID_MANTIS = 17,
    INSECT_BROWN_CICADA = 18,
    INSECT_ROBUST_CICADA = 19,
    INSECT_GIANT_CICADA = 20,
    INSECT_WALKER_CICADA = 21,
    INSECT_EVENING_CICADA = 22,
    INSECT_CICADA_SHELL = 23,
    INSECT_LANTERN_FLY = 24,
    INSECT_RED_DRAGONFLY = 25,
    INSECT_DARNER_DRAGONFLY = 26,
    INSECT_BANDED_DRAGONFLY = 27,
    INSECT_PETALTAIL_DRAGONFLY = 28,
    INSECT_ANT = 29,
    INSECT_PONDSKATER = 30,
    INSECT_DIVING_BEETLE = 31,
    INSECT_STINKBUG = 32,
    INSECT_SNAIL = 33,
    INSECT_CRICKET = 34,
    INSECT_BELL_CRICKET = 35,
    INSECT_GRASSHOPPER = 36,
    INSECT_MOLE_CRICKET = 37,
    INSECT_WALKING_LEAF = 38,
    INSECT_WALKING_STICK = 39,
    INSECT_BAGWORM = 40,
    INSECT_LADYBUG = 41,
    INSECT_VIOLIN_BEETLE = 42,
    INSECT_LONGHORN_BEETLE = 43,
    INSECT_TIGER_BEETLE = 44,
    INSECT_DUNG_BEETLE = 45,
    INSECT_WHARF_ROACH = 46,
    INSECT_HERMIT_CRAB = 47,
    INSECT_FIREFLY = 48,
    INSECT_FRUIT_BEETLE = 49,
    INSECT_SCARAB_BEETLE = 50,
    INSECT_JEWEL_BEETLE = 51,
    INSECT_MIYAMA_STAG = 52,
    INSECT_SAW_STAG = 53,
    INSECT_GIANT_STAG = 54,
    INSECT_RAINBOW_STAG = 55,
    INSECT_CYCLOMMATUS_STAG = 56,
    INSECT_GOLDEN_STAG = 57,
    INSECT_HORNED_DYNASTID = 58,
    INSECT_HORNED_ATLAS = 59,
    INSECT_HORNED_ELEPHANT = 60,
    INSECT_HORNED_HERCULES = 61,
    INSECT_GOLIATH_BEETLE = 62,
    INSECT_FLEA = 63,
    INSECT_PILL_BUG = 64,
    INSECT_MOSQUITO = 65,
    INSECT_FLY = 66,
    INSECT_HOUSE_CENTIPEDE = 67,
    INSECT_CENTIPEDE = 68,
    INSECT_SPIDER = 69,
    INSECT_TARANTULA = 70,
    INSECT_SCORPION = 71,
    INSECT_NUM = 72,
};

enum SvCensusFish {
    FISH_BITTERLING = 0,
    FISH_PALE_CHUB = 1,
    FISH_CRUCIAN_CARP = 2,
    FISH_DACE = 3,
    FISH_BARBEL_STEED = 4,
    FISH_CARP = 5,
    FISH_KOI = 6,
    FISH_GOLDFISH = 7,
    FISH_POP_EYED_GOLDFISH = 8,
    FISH_KILLIFISH = 9,
    FISH_CRAWFISH = 10,
    FISH_SOFT_SHELLED_TURTLE = 11,
    FISH_TADPOLE = 12,
    FISH_FROG = 13,
    FISH_FRESHWATER_GOBY = 14,
    FISH_LOACH = 15,
    FISH_CATFISH = 16,
    FISH_EEL = 17,
    FISH_GIANT_SNAKEHEAD = 18,
    FISH_BLUEGILL = 19,
    FISH_YELLOW_PERCH = 20,
    FISH_BLACK_BASS = 21,
    FISH_PIKE = 22,
    FISH_POND_SMELT = 23,
    FISH_SWEETFISH = 24,
    FISH_CHERRY_SALMON = 25,
    FISH_CHAR = 26,
    FISH_RAINBOW_TROUT = 27,
    FISH_STRINGFISH = 28,
    FISH_SALMON = 29,
    FISH_KING_SALMON = 30,
    FISH_MITTEN_CRAB = 31,
    FISH_GUPPY = 32,
    FISH_NIBBLE_FISH = 33,
    FISH_ANGELFISH = 34,
    FISH_NEON_TETRA = 35,
    FISH_PIRANHA = 36,
    FISH_AROWANA = 37,
    FISH_DORADO = 38,
    FISH_GAR = 39,
    FISH_ARAPAIMA = 40,
    FISH_SADDLED_BICHIR = 41,
    FISH_SEA_BUTTERFLY = 42,
    FISH_SEA_HORSE = 43,
    FISH_CLOWN_FISH = 44,
    FISH_SURGEONFISH = 45,
    FISH_BUTTERFLY_FISH = 46,
    FISH_NAPOLEONFISH = 47,
    FISH_ZEBRA_TURKEYFISH = 48,
    FISH_BLOWFISH = 49,
    FISH_PUFFER_FISH = 50,
    FISH_HORSEMACKEREL = 51,
    FISH_BARRED_KNIFEJAW = 52,
    FISH_SEA_BASS = 53,
    FISH_RED_SNAPPER = 54,
    FISH_DAB = 55,
    FISH_OLIVE_FLOUNDER = 56,
    FISH_SQUID = 57,
    FISH_MORAY_EEL = 58,
    FISH_RIBBON_EEL = 59,
    FISH_FOOTBALL_FISH = 60,
    FISH_TUNA = 61,
    FISH_BLUE_MARLIN = 62,
    FISH_GIANT_TREVALLY = 63,
    FISH_RAY = 64,
    FISH_OCEAN_SUNFISH = 65,
    FISH_HAMMERHEAD_SHARK = 66,
    FISH_SHARK = 67,
    FISH_SAW_SHARK = 68,
    FISH_WHALE_SHARK = 69,
    FISH_OARFISH = 70,
    FISH_COELACANTH = 71,
    FISH_NUM = 72,
};

// (the research had a 31st entry "waterEggCaught" at +0x78: that is the first word of the table per player, ACNL has
// 30 sea creatures)
enum SvCensusSeaCreature {
    SEA_CREATURE_SEAWEED = 0,
    SEA_CREATURE_SEA_GRAPES = 1,
    SEA_CREATURE_SEA_URCHIN = 2,
    SEA_CREATURE_ACORN_BARNACLE = 3,
    SEA_CREATURE_OYSTER = 4,
    SEA_CREATURE_TURBAN_SHELL = 5,
    SEA_CREATURE_ABALONE = 6,
    SEA_CREATURE_EAR_SHELL = 7,
    SEA_CREATURE_CLAM = 8,
    SEA_CREATURE_PEARL_OYSTER = 9,
    SEA_CREATURE_SCALLOP = 10,
    SEA_CREATURE_SEA_ANEMONE = 11,
    SEA_CREATURE_SEA_STAR = 12,
    SEA_CREATURE_SEA_CUCUMBER = 13,
    SEA_CREATURE_SEA_SLUG = 14,
    SEA_CREATURE_FLATWORM = 15,
    SEA_CREATURE_MANTIS_SHRIMP = 16,
    SEA_CREATURE_SWEET_SHRIMP = 17,
    SEA_CREATURE_TIGER_PRAWN = 18,
    SEA_CREATURE_SPINY_LOBSTER = 19,
    SEA_CREATURE_LOBSTER = 20,
    SEA_CREATURE_SNOW_CRAB = 21,
    SEA_CREATURE_HORSEHAIR_CRAB = 22,
    SEA_CREATURE_RED_KING_CRAB = 23,
    SEA_CREATURE_SPIDER_CRAB = 24,
    SEA_CREATURE_OCTOPUS = 25,
    SEA_CREATURE_SPOTTED_GARDEN_EEL = 26,
    SEA_CREATURE_CHAMBERED_NAUTILUS = 27,
    SEA_CREATURE_HORSESHOE_CRAB = 28,
    SEA_CREATURE_GIANT_ISOPOD = 29,
    SEA_CREATURE_NUM = 30,
};

// the other things caught with the fishing rod
enum SvCensusJunk {
    JUNK_EMPTY_CAN = 0,
    JUNK_BOOT = 1,
    JUNK_OLD_TIRE = 2,
    JUNK_NUM = 3,
};

// the limits of the census values (names are ours): most values, the counts (the catches and several values), the public
// works projects
const u32 CENSUS_VALUE_MAX = 999999999;
const u32 CENSUS_COUNT_MAX = 9999;
const u32 CENSUS_PUBLIC_WORKS_MAX = 99;

struct SvCensus {
    /* 0x0000 */ SvCensusValue bellsEarned;
    /* 0x0014 */ SvCensusValue abdBalance;
    /* 0x0028 */ SvCensusValue bellsSpent;
    /* 0x003C */ SvCensusValue loanPaid;
    /* 0x0050 */ SvCensusValue turnipsBought;
    /* 0x0064 */ SvCensusValue turnipsSold;
    /* 0x0078 */ SvCensusValue turnipExpenses;
    /* 0x008C */ SvCensusValue turnipProfits;
    /* 0x00A0 */ SvCensusValue publicWorkExpenses;
    /* 0x00B4 */ SvCensusValue recycleShopEarnings;
    /* 0x00C8 */ SvCensusValue flowersWatered;
    /* 0x00DC */ SvCensusValue fishCaught;
    /* 0x00F0 */ SvCensusValue bugsCaught;
    /* 0x0104 */ SvCensusValue seaCreaturesCaught;
    /* 0x0118 */ SvCensusValue publicWorksBuilt; // NON_PLAYER_SPECIFIC
    /* 0x012C */ SvCensusValue fruitGrown; // NON_PLAYER_SPECIFIC
    /* 0x0140 */ SvCensusValue perfectFruitGrown; // NON_PLAYER_SPECIFIC
    /* 0x0154 */ SvCensusValue fruitsGrownOnTheBeach; // NON_PLAYER_SPECIFIC
    /* 0x0168 */ SvCensusValue flowersPlanted;
    /* 0x017C */ SvCensusValue treesPlanted;
    /* 0x0190 */ SvCensusValue treesCutDown;
    /* 0x01A4 */ SvCensusValue dreamTownsVisited;
    /* 0x01B8 */ SvCensusValue kkSliderConcertsAttended;
    /* 0x01CC */ SvCensusValue shrunkSketchesAttended;
    /* 0x01E0 */ SvCensusValue tourVisits;
    /* 0x01F4 */ SvCensusValue islandVisits;
    /* 0x0208 */ SvCensusValue lettersSent;
    /* 0x021C */ SvCensusValue furnitureCustomized;
    /* 0x0230 */ SvCensusValue nooklingsExpenses;
    /* 0x0244 */ SvCensusValue graciesExpenses;
    /* 0x0258 */ SvCensusValue gardeningShopExpenses;
    /* 0x026C */ SvCensusValue weedsPulled;
    /* 0x0280 */ SvCensusValue ableSistersExpenses;
    /* 0x0294 */ SvCensusValue kicksExpenses;
    /* 0x02A8 */ SvCensusValue katrinaVisits;
    /* 0x02BC */ SvCensusValue streetPassVisitors; // NON_PLAYER_SPECIFIC
    /* 0x02D0 */ SvCensusValue townsVisited;
    /* 0x02E4 */ SvCensusValue townVisitors;
    /* 0x02F8 */ SvCensusValue saharaRedecorations;
    /* 0x030C */ SvCensusValue jobsAtTheRoost;
    /* 0x0320 */ SvCensusValue fossilsAnalyzed;
    /* 0x0334 */ SvCensusValue proDesignsCreated;
    /* 0x0348 */ SvCensusValue earnedBadges; // This one has no total amount
    /* 0x035C */ SvCensusValue helpedGulliver;
    /* 0x0370 */ SvCensusValue shampoodleVisits;
    /* 0x0384 */ SvCensusValue boughtArtAtRedds;
    /* 0x0398 */ SvCensusValue scallopsGivenToPascal;
    /* 0x03AC */ SvCensusValue helpedKatieTravel;
    /* 0x03C0 */ SvCensusValue balloonsPopped;
    /* 0x03D4 */ SvCensusValue tourneyFishCaught; // Get cleared after event
    /* 0x03E8 */ SvCensusValue tourneyInsectCaught; // Get cleared after event
    /* 0x03FC */ SvCensusValue festivalFeathersCaught;
    /* 0x0410 */ SvCensusValue eggsGivenToZipper;
    /* 0x0424 */ SvCensusValue foundImposterBlanca;
    /* 0x0438 */ SvCensusValue fireworkDesignsGivenToIsabelle; // will be set right after the fireworks started
    /* 0x044C */ SvCensusValue receivedCandyOnHalloween;
    /* 0x0460 */ SvCensusValue harvestFestivalCoursesDone;
    /* 0x0474 */ SvCensusValue snowmenBuiltByYouInTown;
    /* 0x0488 */ SvCensusValue givenToyDayPresents;
    /* 0x049C */ SvCensusValue snowflakesCaught;
    /* 0x04B0 */ SvCensusValue partyPoppersPoppedAtNewYears;
    /* 0x04C4 */ SvCensusValue totalBingoWins;
    /* 0x04D8 */ SvCensusValue shootingStarWishes;
    /* 0x04EC */ SvCensusValue npcPicturesReceived;
    /* 0x0500 */ SvCensusValue lostItemsGivenBack;
    /* 0x0514 */ SvCensusValue youVisitedVillagers;
    /* 0x0528 */ SvCensusValue villagersVisitedYou;
    /* 0x053C */ SvCensusValue playedHideAndSeek;
    /* 0x0550 */ SvCensusValue unk_0x550; // KOKT71
    /* 0x0564 */ SvCensusValue unk_0x564; // KOKT72
    /* 0x0578 */ SvCensusValue amiiboScanned;
    /* 0x058C */ SvCensusValue resetSurveillanceCenterVisits;
    /* 0x05A0 */ SvCensusValue djkkVisits;
    /* 0x05B4 */ SvCensusValue goldenRosesCreated; // NON_PLAYER_SPECIFIC
    /* 0x05C8 */ SvCensusValue blueRosesCreated; // NON_PLAYER_SPECIFIC
    /* 0x05DC */ SvCensusValue famousMushroomsEaten;
    /* 0x05F0 */ SvCensusValue townInitiativesCompleted;
    /* 0x0604 */ SvCensusValue meowCouponsEarned;
    /* 0x0618 */ SvCensusValue meowCouponsSpent;
    /* 0x062C */ SvCensusValue unk_0x62C; // NON_PLAYER_SPECIFIC / KOKT82
    // the catch counts: all players, then per player [index][town slot]
    /* 0x0640 */ u32 insectCatchCounts[INSECT_NUM];
    /* 0x0760 */ u32 insectCatchCountsPerPlayer[INSECT_NUM][4];
    /* 0x0BE0 */ u32 fishCatchCounts[FISH_NUM];
    /* 0x0D00 */ u32 fishCatchCountsPerPlayer[FISH_NUM][4];
    /* 0x1180 */ u32 junkCatchCounts[JUNK_NUM];
    /* 0x118C */ u32 junkCatchCountsPerPlayer[JUNK_NUM][4];
    /* 0x11BC */ u32 seaCreatureCatchCounts[SEA_CREATURE_NUM];
    /* 0x1234 */ u32 seaCreatureCatchCountsPerPlayer[SEA_CREATURE_NUM][4];

    // (inline in the constructor of SvCensusData)
    SvCensus() { Clear(); }
    // everything is 0
    void Clear(); // 0x006B9B88

    // (the census functions of the game, one copy per value; names are ours, the values after the research names)
    // Add<Value>: to the value of the current player (SvGetCurrentTownSlot) and to the total; Add<Value>Total: to the
    // total only; ...ForSlot: for a player by its town slot; Set<Value>: the value of the player is set, the total
    // changes by the part of the value above the limit; Clear<Value>: everything 0; Add<Kind>Catch(index): a catch of the
    // player and of all. Nothing changes in the Dream Suite (netgame::TYPE_DREAM). The values stay in [0, limit].
    // the values of a player are cleared (the player leaves the town)
    void ClearPlayer(u32 townSlot); // 0x006B59E8
    DECOMP_NOINLINE void AddJunkCatchTotal(u32 index, s32 value); // 0x006B5BF0
    DECOMP_NOINLINE void AddFishCatchTotal(u32 index, s32 value); // 0x006B5C4C
    DECOMP_NOINLINE void AddInsectCatchTotal(u32 index, s32 value); // 0x006B5CA4
    void ClearTotalBingoWins(); // 0x006B5CFC
    void ClearEggsGivenToZipper(); // 0x006B5D18
    DECOMP_NOINLINE void AddSeaCreatureCatchTotal(u32 index, s32 value); // 0x006B5D34
    DECOMP_NOINLINE void AddStreetPassVisitorsTotal(s32 value); // 0x006B5D90
    DECOMP_NOINLINE void AddTreesCutDownTotal(s32 value); // 0x006B5DE0
    void ClearPartyPoppersPoppedAtNewYears(); // 0x006B5E30
    void ClearHarvestFestivalCoursesDone(); // 0x006B5E4C
    void ClearSnowmenBuiltByYouInTown(); // 0x006B5E68
    DECOMP_NOINLINE void AddJobsAtTheRoostTotal(s32 value); // 0x006B5E84
    DECOMP_NOINLINE void AddDjkkVisitsTotal(s32 value); // 0x006B5ED4
    DECOMP_NOINLINE void AddPlayedHideAndSeekTotal(s32 value); // 0x006B5F24
    DECOMP_NOINLINE void AddHelpedGulliverTotal(s32 value); // 0x006B5F74
    DECOMP_NOINLINE void AddFlowersPlantedTotal(s32 value); // 0x006B5FC4
    DECOMP_NOINLINE void AddLettersSentTotal(s32 value); // 0x006B6014
    DECOMP_NOINLINE void AddTourVisitsTotal(s32 value); // 0x006B6064
    DECOMP_NOINLINE void AddWeedsPulledTotal(s32 value); // 0x006B60B4
    DECOMP_NOINLINE void AddAmiiboScannedTotal(s32 value); // 0x006B6104
    void ClearFestivalFeathersCaught(); // 0x006B6154
    void ClearFireworkDesignsGivenToIsabelle(); // 0x006B6170
    DECOMP_NOINLINE void AddEarnedBadgesTotal(s32 value); // 0x006B61A0
    DECOMP_NOINLINE void AddTotalBingoWinsTotal(s32 value); // 0x006B61F0
    DECOMP_NOINLINE void AddHelpedKatieTravelTotal(s32 value); // 0x006B6240
    DECOMP_NOINLINE void AddBellsEarnedTotal(s32 value); // 0x006B6290
    DECOMP_NOINLINE void AddTreesPlantedTotal(s32 value); // 0x006B62E0
    DECOMP_NOINLINE void AddShampoodleVisitsTotal(s32 value); // 0x006B6330
    DECOMP_NOINLINE void AddShrunkSketchesAttendedTotal(s32 value); // 0x006B6380
    DECOMP_NOINLINE void AddTownsVisitedTotal(s32 value); // 0x006B63D0
    void ClearFoundImposterBlanca(); // 0x006B6420
    void ClearGivenToyDayPresents(); // 0x006B643C
    void ClearReceivedCandyOnHalloween(); // 0x006B6458
    void AddJunkCatch(u32 index, s32 value); // 0x006B6474
    void AddFishCatch(u32 index, s32 value); // 0x006B64FC
    DECOMP_NOINLINE void AddSeaCreaturesCaughtTotal(s32 value); // 0x006B657C
    DECOMP_NOINLINE void AddEggsGivenToZipperTotal(s32 value); // 0x006B65CC
    DECOMP_NOINLINE void AddMeowCouponsEarnedTotal(s32 value); // 0x006B661C
    DECOMP_NOINLINE void AddIslandVisitsTotal(s32 value); // 0x006B666C
    DECOMP_NOINLINE void AddShootingStarWishesTotal(s32 value); // 0x006B66BC
    DECOMP_NOINLINE void AddSaharaRedecorationsTotal(s32 value); // 0x006B670C
    DECOMP_NOINLINE void AddFurnitureCustomizedTotal(s32 value); // 0x006B675C
    DECOMP_NOINLINE void AddAbdBalanceTotal(s32 value); // 0x006B67AC
    DECOMP_NOINLINE void AddMeowCouponsSpentTotal(s32 value); // 0x006B67FC
    DECOMP_NOINLINE void AddBalloonsPoppedTotal(s32 value); // 0x006B684C
    DECOMP_NOINLINE void AddNpcPicturesReceivedTotal(s32 value); // 0x006B689C
    DECOMP_NOINLINE void AddPartyPoppersPoppedAtNewYearsTotal(s32 value); // 0x006B68EC
    DECOMP_NOINLINE void AddFishCaughtTotal(s32 value); // 0x006B693C
    DECOMP_NOINLINE void AddHarvestFestivalCoursesDoneTotal(s32 value); // 0x006B698C
    DECOMP_NOINLINE void AddBoughtArtAtReddsTotal(s32 value); // 0x006B69DC
    DECOMP_NOINLINE void AddPublicWorkExpensesTotal(s32 value); // 0x006B6A2C
    DECOMP_NOINLINE void AddResetSurveillanceCenterVisitsTotal(s32 value); // 0x006B6A7C
    DECOMP_NOINLINE void AddScallopsGivenToPascalTotal(s32 value); // 0x006B6ACC
    DECOMP_NOINLINE void AddRecycleShopEarningsTotal(s32 value); // 0x006B6B1C
    DECOMP_NOINLINE void AddSnowmenBuiltByYouInTownTotal(s32 value); // 0x006B6B6C
    DECOMP_NOINLINE void AddTownVisitorsTotal(s32 value); // 0x006B6BBC
    void ClearTourneyFishCaught(); // 0x006B6C0C
    void AddInsectCatch(u32 index, s32 value); // 0x006B6C28
    DECOMP_NOINLINE void AddBlueRosesCreatedTotal(s32 value); // 0x006B6CA8
    DECOMP_NOINLINE void AddFestivalFeathersCaughtTotal(s32 value); // 0x006B6CF8
    DECOMP_NOINLINE void AddBugsCaughtTotal(s32 value); // 0x006B6D48
    DECOMP_NOINLINE void AddDreamTownsVisitedTotal(s32 value); // 0x006B6D98
    DECOMP_NOINLINE void AddFireworkDesignsGivenToIsabelleTotal(s32 value); // 0x006B6DE8
    DECOMP_NOINLINE void AddGoldenRosesCreatedTotal(s32 value); // 0x006B6E38
    DECOMP_NOINLINE void AddUnk0x550Total(s32 value); // 0x006B6E88
    DECOMP_NOINLINE void AddKicksExpensesTotal(s32 value); // 0x006B6ED8
    DECOMP_NOINLINE void AddKkSliderConcertsAttendedTotal(s32 value); // 0x006B6F28
    DECOMP_NOINLINE void AddFlowersWateredTotal(s32 value); // 0x006B6F78
    void ClearTourneyInsectCaught(); // 0x006B6FC8
    DECOMP_NOINLINE void AddFossilsAnalyzedTotal(s32 value); // 0x006B6FE4
    DECOMP_NOINLINE void AddFoundImposterBlancaTotal(s32 value); // 0x006B7034
    DECOMP_NOINLINE void AddGivenToyDayPresentsTotal(s32 value); // 0x006B7084
    DECOMP_NOINLINE void AddProDesignsCreatedTotal(s32 value); // 0x006B70D4
    DECOMP_NOINLINE void AddReceivedCandyOnHalloweenTotal(s32 value); // 0x006B7124
    void AddBellsEarnedForSlot(u32 townSlot, s32 value); // 0x006B7174
    DECOMP_NOINLINE void AddGraciesExpensesTotal(s32 value); // 0x006B71DC
    void SetEarnedBadgesForSlot(u32 townSlot, s32 value); // 0x006B722C
    void AddSeaCreatureCatch(u32 index, s32 value); // 0x006B7280
    DECOMP_NOINLINE void AddTurnipsBoughtTotal(s32 value); // 0x006B7308
    void AddMeowCouponsEarnedForSlot(u32 townSlot, s32 value); // 0x006B7358
    void AddTreesCutDown(s32 value); // 0x006B73C0
    DECOMP_NOINLINE void AddUnk0x62CTotal(s32 value); // 0x006B7434
    DECOMP_NOINLINE void AddLoanPaidTotal(s32 value); // 0x006B7484
    DECOMP_NOINLINE void AddAbleSistersExpensesTotal(s32 value); // 0x006B74D4
    void SetPublicWorksBuiltTotal(s32 value); // 0x006B7524
    void SetAbdBalanceForSlot(u32 townSlot, s32 value); // 0x006B7558
    void AddJobsAtTheRoost(s32 value); // 0x006B75AC
    DECOMP_NOINLINE void AddBellsSpentTotal(s32 value); // 0x006B7620
    void AddDjkkVisits(s32 value); // 0x006B7670
    DECOMP_NOINLINE void AddTourneyFishCaughtTotal(s32 value); // 0x006B76E4
    void AddPlayedHideAndSeek(s32 value); // 0x006B7734
    DECOMP_NOINLINE void AddTownInitiativesCompletedTotal(s32 value); // 0x006B77A8
    DECOMP_NOINLINE void AddLostItemsGivenBackTotal(s32 value); // 0x006B77F8
    void AddHelpedGulliver(s32 value); // 0x006B7848
    void AddFlowersPlanted(s32 value); // 0x006B78BC
    DECOMP_NOINLINE void AddTurnipsSoldTotal(s32 value); // 0x006B7930
    void AddLettersSent(s32 value); // 0x006B7980
    DECOMP_NOINLINE void AddKatrinaVisitsTotal(s32 value); // 0x006B79F4
    DECOMP_NOINLINE void AddSnowflakesCaughtTotal(s32 value); // 0x006B7A44
    DECOMP_NOINLINE void AddUnk0x564Total(s32 value); // 0x006B7A94
    void AddTourVisits(s32 value); // 0x006B7AE4
    DECOMP_NOINLINE void AddYouVisitedVillagersTotal(s32 value); // 0x006B7B58
    void AddWeedsPulled(s32 value); // 0x006B7BA8
    void AddAmiiboScanned(s32 value); // 0x006B7C1C
    void AddTotalBingoWins(s32 value); // 0x006B7C90
    DECOMP_NOINLINE void AddTurnipExpensesTotal(s32 value); // 0x006B7D04
    DECOMP_NOINLINE void AddTourneyInsectCaughtTotal(s32 value); // 0x006B7D54
    DECOMP_NOINLINE void AddVillagersVisitedYouTotal(s32 value); // 0x006B7DA4
    void AddFireworkDesignsGivenToIsabelleForSlot(u32 townSlot, s32 value); // 0x006B7DF4
    void AddHelpedKatieTravel(s32 value); // 0x006B7E5C
    void AddBellsEarned(s32 value); // 0x006B7ED0
    void AddTreesPlanted(s32 value); // 0x006B7F44
    void AddShampoodleVisits(s32 value); // 0x006B7FB8
    void AddShrunkSketchesAttended(s32 value); // 0x006B802C
    void AddTownsVisited(s32 value); // 0x006B80A0
    void SetEarnedBadges(s32 value); // 0x006B8114
    void AddSeaCreaturesCaught(s32 value); // 0x006B8178
    void AddEggsGivenToZipper(s32 value); // 0x006B81EC
    void AddMeowCouponsEarned(s32 value); // 0x006B8260
    void AddIslandVisits(s32 value); // 0x006B82D4
    void AddShootingStarWishes(s32 value); // 0x006B8348
    void AddSaharaRedecorations(s32 value); // 0x006B83BC
    void AddFurnitureCustomized(s32 value); // 0x006B8430
    DECOMP_NOINLINE void AddTurnipProfitsTotal(s32 value); // 0x006B84A4
    DECOMP_NOINLINE void AddGardeningShopExpensesTotal(s32 value); // 0x006B84F4
    DECOMP_NOINLINE void AddNooklingsExpensesTotal(s32 value); // 0x006B8544
    void AddMeowCouponsSpent(s32 value); // 0x006B8594
    void SetAbdBalance(s32 value); // 0x006B8608
    void AddBalloonsPopped(s32 value); // 0x006B866C
    void AddPartyPoppersPoppedAtNewYears(s32 value); // 0x006B86E0
    DECOMP_NOINLINE void AddFamousMushroomsEatenTotal(s32 value); // 0x006B8754
    void AddFishCaught(s32 value); // 0x006B87A4
    void AddHarvestFestivalCoursesDone(s32 value); // 0x006B8818
    void AddBoughtArtAtRedds(s32 value); // 0x006B888C
    void AddPublicWorkExpenses(s32 value); // 0x006B8900
    void AddResetSurveillanceCenterVisits(s32 value); // 0x006B8974
    void AddScallopsGivenToPascal(s32 value); // 0x006B89E8
    void AddRecycleShopEarnings(s32 value); // 0x006B8A5C
    void AddSnowmenBuiltByYouInTown(s32 value); // 0x006B8AD0
    void AddTownVisitors(s32 value); // 0x006B8B44
    void SetNpcPicturesReceived(s32 value); // 0x006B8BB8
    void AddFestivalFeathersCaught(s32 value); // 0x006B8C1C
    void AddBugsCaught(s32 value); // 0x006B8C90
    void AddDreamTownsVisited(s32 value); // 0x006B8D04
    void AddTownInitiativesCompletedForSlot(u32 townSlot, s32 value); // 0x006B8D78
    void AddUnk0x550(s32 value); // 0x006B8DE0
    void AddKicksExpenses(s32 value); // 0x006B8E54
    void AddUnk0x564ForSlot(u32 townSlot, s32 value); // 0x006B8EC8
    void AddKkSliderConcertsAttended(s32 value); // 0x006B8F30
    void AddFlowersWatered(s32 value); // 0x006B8FA4
    void UpdateAbdBalanceForSlot(u32 townSlot, s32 value); // 0x006B9018
    void AddFossilsAnalyzed(s32 value); // 0x006B9054
    void AddFoundImposterBlanca(s32 value); // 0x006B90C8
    void AddGivenToyDayPresents(s32 value); // 0x006B913C
    void AddProDesignsCreated(s32 value); // 0x006B91B0
    void AddReceivedCandyOnHalloween(s32 value); // 0x006B9224
    void AddGraciesExpenses(s32 value); // 0x006B9298
    void AddTurnipsBought(s32 value); // 0x006B930C
    void AddLoanPaid(s32 value); // 0x006B9380
    void AddAbleSistersExpenses(s32 value); // 0x006B93F4
    void AddBellsSpent(s32 value); // 0x006B9468
    void AddTourneyFishCaught(s32 value); // 0x006B94DC
    void AddTownInitiativesCompleted(s32 value); // 0x006B9550
    void AddLostItemsGivenBack(s32 value); // 0x006B95C4
    void AddTurnipsSold(s32 value); // 0x006B9638
    void AddKatrinaVisits(s32 value); // 0x006B96AC
    void AddSnowflakesCaught(s32 value); // 0x006B9720
    void AddYouVisitedVillagers(s32 value); // 0x006B9794
    void UpdateAbdBalance(s32 value); // 0x006B9808
    void AddTurnipExpenses(s32 value); // 0x006B985C
    void AddTourneyInsectCaught(s32 value); // 0x006B98D0
    void AddVillagersVisitedYou(s32 value); // 0x006B9944
    void AddTurnipProfits(s32 value); // 0x006B99B8
    void AddGardeningShopExpenses(s32 value); // 0x006B9A2C
    void AddNooklingsExpenses(s32 value); // 0x006B9AA0
    void AddFamousMushroomsEaten(s32 value); // 0x006B9B14
};
ASSERT_SIZE(SvCensus, 0x1414);

// the census with its checksum (CRC-32 MSB first, nn::util::Crc32Msb), in the shared data (name is ours)
// the census of the save data in memory: the same as GetCensus (dSvApi.h), but through GardenPlus (name is ours)
SvCensus* GetCensusOfGardenPlus(); // 0x006B618C

// The values of the census that count during an event (the census value indices 49..61, from tourneyFishCaught to
// totalBingoWins): whether the event of a value takes place today (names after the address).
bool Func4DD108(u8 valueIndex); // 0x004DD108

// The census values that start at 0 again for every event, by their index in the census (the order of the values).
enum SvCensusEventValue {
    CENSUS_EVENT_TOURNEY_FISH_CAUGHT = 49,
    CENSUS_EVENT_TOURNEY_INSECT_CAUGHT = 50,
    CENSUS_EVENT_FESTIVAL_FEATHERS_CAUGHT = 51,
    CENSUS_EVENT_EGGS_GIVEN_TO_ZIPPER = 52,
    CENSUS_EVENT_FOUND_IMPOSTER_BLANCA = 53,
    CENSUS_EVENT_FIREWORK_DESIGNS_GIVEN_TO_ISABELLE = 54,
    CENSUS_EVENT_RECEIVED_CANDY_ON_HALLOWEEN = 55,
    CENSUS_EVENT_HARVEST_FESTIVAL_COURSES_DONE = 56,
    CENSUS_EVENT_SNOWMEN_BUILT_BY_YOU_IN_TOWN = 57,
    CENSUS_EVENT_GIVEN_TOY_DAY_PRESENTS = 58,
    CENSUS_EVENT_SNOWFLAKES_CAUGHT = 59,
    CENSUS_EVENT_PARTY_POPPERS_POPPED_AT_NEW_YEARS = 60,
    CENSUS_EVENT_TOTAL_BINGO_WINS = 61,
};

struct SvCensusData {
    // (the constructor of the save data builds it; names are ours)
    SvCensusData(); // 0x002A7110
    // the census and the event dates are cleared (the dates of the party poppers and the bingo, whose events go over
    // the new year, stay)
    void Clear(); // 0x002A6BCC
    DECOMP_NOINLINE void ClearEventDates(); // 0x002A6BE8
    // an event value: the date is the last day of its event; on the first day of a new event (the date is not today or
    // yesterday) the value is cleared, unless isSameSeason (the date is set to today when the event takes place today;
    // true: not cleared) / all event values for today (the snowflakes are cleared with the snowmen of the same winter)
    DECOMP_NOINLINE bool UpdateEventValue(SvDate* date, const nn::fnd::DateTimeParameters* today, u8 valueIndex,
                          void (SvCensus::*clear)(), bool isSameSeason); // 0x002A6CB8
    void UpdateEventValues(const nn::fnd::DateTimeParameters* today); // 0x002A6E18

    // the checksum of the census and the event dates (names are ours)
    DECOMP_NOINLINE void UpdateChecksum(); // 0x0071CD88
    DECOMP_NOINLINE bool IsChecksumValid(); // 0x0071CDAC

    /* 0x0000 */ u32 checksum;
    /* 0x0004 */ SvCensus census;
    // the last day of the event of each event value (the snowflakes have no date of their own)
    /* 0x1418 */ SvDate tourneyFishCaughtDate;
    /* 0x141C */ SvDate tourneyInsectCaughtDate;
    /* 0x1420 */ SvDate festivalFeathersCaughtDate;
    /* 0x1424 */ SvDate eggsGivenToZipperDate;
    /* 0x1428 */ SvDate foundImposterBlancaDate;
    /* 0x142C */ SvDate fireworkDesignsGivenToIsabelleDate;
    /* 0x1430 */ SvDate receivedCandyOnHalloweenDate;
    /* 0x1434 */ SvDate harvestFestivalCoursesDoneDate;
    /* 0x1438 */ SvDate snowmenBuiltByYouInTownDate;
    /* 0x143C */ SvDate givenToyDayPresentsDate;
    /* 0x1440 */ SvDate partyPoppersPoppedAtNewYearsDate;
    /* 0x1444 */ SvDate totalBingoWinsDate;
};
ASSERT_SIZE(SvCensusData, 0x1448);

