#pragma once

// Census (statistics) data of all players.
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "nn/fnd/fnd_DateTime.h"

// (dSvApi.h; declared here because dSvApi.h includes this header through SvGardenPlus)
u32 SvGetCurrentTownSlot();

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

// The values of the census by their index (the order of the values in SvCensus; the census menu, its tables and the
// event code use it)
enum SvCensusValueId : u8 {
    CENSUS_VALUE_BELLS_EARNED = 0,
    CENSUS_VALUE_ABD_BALANCE = 1,
    CENSUS_VALUE_BELLS_SPENT = 2,
    CENSUS_VALUE_LOAN_PAID = 3,
    CENSUS_VALUE_TURNIPS_BOUGHT = 4,
    CENSUS_VALUE_TURNIPS_SOLD = 5,
    CENSUS_VALUE_TURNIP_EXPENSES = 6,
    CENSUS_VALUE_TURNIP_PROFITS = 7,
    CENSUS_VALUE_PUBLIC_WORK_EXPENSES = 8,
    CENSUS_VALUE_RECYCLE_SHOP_EARNINGS = 9,
    CENSUS_VALUE_FLOWERS_WATERED = 10,
    CENSUS_VALUE_FISH_CAUGHT = 11,
    CENSUS_VALUE_BUGS_CAUGHT = 12,
    CENSUS_VALUE_SEA_CREATURES_CAUGHT = 13,
    CENSUS_VALUE_PUBLIC_WORKS_BUILT = 14,
    CENSUS_VALUE_FRUIT_GROWN = 15,
    CENSUS_VALUE_PERFECT_FRUIT_GROWN = 16,
    CENSUS_VALUE_FRUITS_GROWN_ON_THE_BEACH = 17,
    CENSUS_VALUE_FLOWERS_PLANTED = 18,
    CENSUS_VALUE_TREES_PLANTED = 19,
    CENSUS_VALUE_TREES_CUT_DOWN = 20,
    CENSUS_VALUE_DREAM_TOWNS_VISITED = 21,
    CENSUS_VALUE_KK_SLIDER_CONCERTS_ATTENDED = 22,
    CENSUS_VALUE_SHRUNK_SKETCHES_ATTENDED = 23,
    CENSUS_VALUE_TOUR_VISITS = 24,
    CENSUS_VALUE_ISLAND_VISITS = 25,
    CENSUS_VALUE_LETTERS_SENT = 26,
    CENSUS_VALUE_FURNITURE_CUSTOMIZED = 27,
    CENSUS_VALUE_NOOKLINGS_EXPENSES = 28,
    CENSUS_VALUE_GRACIES_EXPENSES = 29,
    CENSUS_VALUE_GARDENING_SHOP_EXPENSES = 30,
    CENSUS_VALUE_WEEDS_PULLED = 31,
    CENSUS_VALUE_ABLE_SISTERS_EXPENSES = 32,
    CENSUS_VALUE_KICKS_EXPENSES = 33,
    CENSUS_VALUE_KATRINA_VISITS = 34,
    CENSUS_VALUE_STREET_PASS_VISITORS = 35,
    CENSUS_VALUE_TOWNS_VISITED = 36,
    CENSUS_VALUE_TOWN_VISITORS = 37,
    CENSUS_VALUE_SAHARA_REDECORATIONS = 38,
    CENSUS_VALUE_JOBS_AT_THE_ROOST = 39,
    CENSUS_VALUE_FOSSILS_ANALYZED = 40,
    CENSUS_VALUE_PRO_DESIGNS_CREATED = 41,
    CENSUS_VALUE_EARNED_BADGES = 42,
    CENSUS_VALUE_HELPED_GULLIVER = 43,
    CENSUS_VALUE_SHAMPOODLE_VISITS = 44,
    CENSUS_VALUE_BOUGHT_ART_AT_REDDS = 45,
    CENSUS_VALUE_SCALLOPS_GIVEN_TO_PASCAL = 46,
    CENSUS_VALUE_HELPED_KATIE_TRAVEL = 47,
    CENSUS_VALUE_BALLOONS_POPPED = 48,
    CENSUS_VALUE_TOURNEY_FISH_CAUGHT = 49,
    CENSUS_VALUE_TOURNEY_INSECT_CAUGHT = 50,
    CENSUS_VALUE_FESTIVAL_FEATHERS_CAUGHT = 51,
    CENSUS_VALUE_EGGS_GIVEN_TO_ZIPPER = 52,
    CENSUS_VALUE_FOUND_IMPOSTER_BLANCA = 53,
    CENSUS_VALUE_FIREWORK_DESIGNS_GIVEN_TO_ISABELLE = 54,
    CENSUS_VALUE_RECEIVED_CANDY_ON_HALLOWEEN = 55,
    CENSUS_VALUE_HARVEST_FESTIVAL_COURSES_DONE = 56,
    CENSUS_VALUE_SNOWMEN_BUILT_BY_YOU_IN_TOWN = 57,
    CENSUS_VALUE_GIVEN_TOY_DAY_PRESENTS = 58,
    CENSUS_VALUE_SNOWFLAKES_CAUGHT = 59,
    CENSUS_VALUE_PARTY_POPPERS_POPPED_AT_NEW_YEARS = 60,
    CENSUS_VALUE_TOTAL_BINGO_WINS = 61,
    CENSUS_VALUE_SHOOTING_STAR_WISHES = 62,
    CENSUS_VALUE_NPC_PICTURES_RECEIVED = 63,
    CENSUS_VALUE_LOST_ITEMS_GIVEN_BACK = 64,
    CENSUS_VALUE_YOU_VISITED_VILLAGERS = 65,
    CENSUS_VALUE_VILLAGERS_VISITED_YOU = 66,
    CENSUS_VALUE_PLAYED_HIDE_AND_SEEK = 67,
    CENSUS_VALUE_UNK_0x550 = 68,
    CENSUS_VALUE_UNK_0x564 = 69,
    CENSUS_VALUE_AMIIBO_SCANNED = 70,
    CENSUS_VALUE_RESET_SURVEILLANCE_CENTER_VISITS = 71,
    CENSUS_VALUE_DJKK_VISITS = 72,
    CENSUS_VALUE_GOLDEN_ROSES_CREATED = 73,
    CENSUS_VALUE_BLUE_ROSES_CREATED = 74,
    CENSUS_VALUE_FAMOUS_MUSHROOMS_EATEN = 75,
    CENSUS_VALUE_TOWN_INITIATIVES_COMPLETED = 76,
    CENSUS_VALUE_MEOW_COUPONS_EARNED = 77,
    CENSUS_VALUE_MEOW_COUPONS_SPENT = 78,
    CENSUS_VALUE_UNK_0x62C = 79,
    CENSUS_VALUE_NUM = 80,
    // the census menu has four more rows after the values (always shown, no value)
    CENSUS_ROW_NUM = 84,
};

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

    // the getters of the values (copies in the shared area; names are ours): Get<Value>Total, Get<Value>ForSlot(town slot)
    // and Get<Value> (the current player, SvGetCurrentTownSlot); signed like in the original (the census menu tests > 0)
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetStreetPassVisitorsTotal() const { return streetPassVisitors.totalPlayerStat; } // 0x007680D4
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTreesCutDownTotal() const { return treesCutDown.totalPlayerStat; } // 0x007680DC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetJobsAtTheRoostTotal() const { return jobsAtTheRoost.totalPlayerStat; } // 0x007680E4
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetDjkkVisitsTotal() const { return djkkVisits.totalPlayerStat; } // 0x007680EC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetPlayedHideAndSeekTotal() const { return playedHideAndSeek.totalPlayerStat; } // 0x007680F4
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetHelpedGulliverTotal() const { return helpedGulliver.totalPlayerStat; } // 0x007680FC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFlowersPlantedTotal() const { return flowersPlanted.totalPlayerStat; } // 0x00768104
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetLettersSentTotal() const { return lettersSent.totalPlayerStat; } // 0x0076810C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTourVisitsTotal() const { return tourVisits.totalPlayerStat; } // 0x00768114
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetWeedsPulledTotal() const { return weedsPulled.totalPlayerStat; } // 0x0076811C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetAmiiboScannedTotal() const { return amiiboScanned.totalPlayerStat; } // 0x00768124
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetEarnedBadgesTotal() const { return earnedBadges.totalPlayerStat; } // 0x0076812C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTotalBingoWinsTotal() const { return totalBingoWins.totalPlayerStat; } // 0x00768134
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetHelpedKatieTravelTotal() const { return helpedKatieTravel.totalPlayerStat; } // 0x0076813C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetBellsEarnedTotal() const { return bellsEarned.totalPlayerStat; } // 0x00768144
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTreesPlantedTotal() const { return treesPlanted.totalPlayerStat; } // 0x0076814C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetShampoodleVisitsTotal() const { return shampoodleVisits.totalPlayerStat; } // 0x00768154
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetShrunkSketchesAttendedTotal() const { return shrunkSketchesAttended.totalPlayerStat; } // 0x0076815C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTownsVisitedTotal() const { return townsVisited.totalPlayerStat; } // 0x00768164
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetSeaCreaturesCaughtTotal() const { return seaCreaturesCaught.totalPlayerStat; } // 0x0076816C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetEggsGivenToZipperTotal() const { return eggsGivenToZipper.totalPlayerStat; } // 0x00768174
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetMeowCouponsEarnedTotal() const { return meowCouponsEarned.totalPlayerStat; } // 0x0076817C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetIslandVisitsTotal() const { return islandVisits.totalPlayerStat; } // 0x00768184
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetShootingStarWishesTotal() const { return shootingStarWishes.totalPlayerStat; } // 0x0076818C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetSaharaRedecorationsTotal() const { return saharaRedecorations.totalPlayerStat; } // 0x00768194
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFurnitureCustomizedTotal() const { return furnitureCustomized.totalPlayerStat; } // 0x0076819C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetAbdBalanceTotal() const { return abdBalance.totalPlayerStat; } // 0x007681A4
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetMeowCouponsSpentTotal() const { return meowCouponsSpent.totalPlayerStat; } // 0x007681AC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetBalloonsPoppedTotal() const { return balloonsPopped.totalPlayerStat; } // 0x007681B4
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetNpcPicturesReceivedTotal() const { return npcPicturesReceived.totalPlayerStat; } // 0x007681BC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetPartyPoppersPoppedAtNewYearsTotal() const { return partyPoppersPoppedAtNewYears.totalPlayerStat; } // 0x007681C4
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFishCaughtTotal() const { return fishCaught.totalPlayerStat; } // 0x007681CC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetHarvestFestivalCoursesDoneTotal() const { return harvestFestivalCoursesDone.totalPlayerStat; } // 0x007681D4
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTreesCutDownForSlot(u32 townSlot) const { return treesCutDown.playerStats[townSlot]; } // 0x007681DC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetBoughtArtAtReddsTotal() const { return boughtArtAtRedds.totalPlayerStat; } // 0x007681E8
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetPublicWorkExpensesTotal() const { return publicWorkExpenses.totalPlayerStat; } // 0x007681F0
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetResetSurveillanceCenterVisitsTotal() const { return resetSurveillanceCenterVisits.totalPlayerStat; } // 0x007681F8
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetScallopsGivenToPascalTotal() const { return scallopsGivenToPascal.totalPlayerStat; } // 0x00768200
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetRecycleShopEarningsTotal() const { return recycleShopEarnings.totalPlayerStat; } // 0x00768208
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetSnowmenBuiltByYouInTownTotal() const { return snowmenBuiltByYouInTown.totalPlayerStat; } // 0x00768210
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTownVisitorsTotal() const { return townVisitors.totalPlayerStat; } // 0x00768218
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetBlueRosesCreatedTotal() const { return blueRosesCreated.totalPlayerStat; } // 0x00768220
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetJobsAtTheRoostForSlot(u32 townSlot) const { return jobsAtTheRoost.playerStats[townSlot]; } // 0x00768228
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFestivalFeathersCaughtTotal() const { return festivalFeathersCaught.totalPlayerStat; } // 0x00768234
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetBugsCaughtTotal() const { return bugsCaught.totalPlayerStat; } // 0x0076823C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetDjkkVisitsForSlot(u32 townSlot) const { return djkkVisits.playerStats[townSlot]; } // 0x00768244
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetDreamTownsVisitedTotal() const { return dreamTownsVisited.totalPlayerStat; } // 0x00768250
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFireworkDesignsGivenToIsabelleTotal() const { return fireworkDesignsGivenToIsabelle.totalPlayerStat; } // 0x00768258
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetGoldenRosesCreatedTotal() const { return goldenRosesCreated.totalPlayerStat; } // 0x00768260
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetPlayedHideAndSeekForSlot(u32 townSlot) const { return playedHideAndSeek.playerStats[townSlot]; } // 0x00768268
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetUnk0x550Total() const { return unk_0x550.totalPlayerStat; } // 0x00768274
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetHelpedGulliverForSlot(u32 townSlot) const { return helpedGulliver.playerStats[townSlot]; } // 0x0076827C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFlowersPlantedForSlot(u32 townSlot) const { return flowersPlanted.playerStats[townSlot]; } // 0x00768288
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetLettersSentForSlot(u32 townSlot) const { return lettersSent.playerStats[townSlot]; } // 0x00768294
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetKicksExpensesTotal() const { return kicksExpenses.totalPlayerStat; } // 0x007682A0
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetKkSliderConcertsAttendedTotal() const { return kkSliderConcertsAttended.totalPlayerStat; } // 0x007682A8
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTourVisitsForSlot(u32 townSlot) const { return tourVisits.playerStats[townSlot]; } // 0x007682B0
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFlowersWateredTotal() const { return flowersWatered.totalPlayerStat; } // 0x007682BC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetWeedsPulledForSlot(u32 townSlot) const { return weedsPulled.playerStats[townSlot]; } // 0x007682C4
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetAmiiboScannedForSlot(u32 townSlot) const { return amiiboScanned.playerStats[townSlot]; } // 0x007682D0
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFossilsAnalyzedTotal() const { return fossilsAnalyzed.totalPlayerStat; } // 0x007682DC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFoundImposterBlancaTotal() const { return foundImposterBlanca.totalPlayerStat; } // 0x007682E4
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetEarnedBadgesForSlot(u32 townSlot) const { return earnedBadges.playerStats[townSlot]; } // 0x007682EC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTotalBingoWinsForSlot(u32 townSlot) const { return totalBingoWins.playerStats[townSlot]; } // 0x007682F8
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetGivenToyDayPresentsTotal() const { return givenToyDayPresents.totalPlayerStat; } // 0x00768304
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetProDesignsCreatedTotal() const { return proDesignsCreated.totalPlayerStat; } // 0x0076830C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetPerfectFruitGrownTotal() const { return perfectFruitGrown.totalPlayerStat; } // 0x00768314
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetReceivedCandyOnHalloweenTotal() const { return receivedCandyOnHalloween.totalPlayerStat; } // 0x0076831C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetHelpedKatieTravelForSlot(u32 townSlot) const { return helpedKatieTravel.playerStats[townSlot]; } // 0x00768324
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetBellsEarnedForSlot(u32 townSlot) const { return bellsEarned.playerStats[townSlot]; } // 0x00768330
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTreesPlantedForSlot(u32 townSlot) const { return treesPlanted.playerStats[townSlot]; } // 0x0076833C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetShampoodleVisitsForSlot(u32 townSlot) const { return shampoodleVisits.playerStats[townSlot]; } // 0x00768348
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetGraciesExpensesTotal() const { return graciesExpenses.totalPlayerStat; } // 0x00768354
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetShrunkSketchesAttendedForSlot(u32 townSlot) const { return shrunkSketchesAttended.playerStats[townSlot]; } // 0x0076835C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTownsVisitedForSlot(u32 townSlot) const { return townsVisited.playerStats[townSlot]; } // 0x00768368
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTurnipsBoughtTotal() const { return turnipsBought.totalPlayerStat; } // 0x00768374
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetSeaCreaturesCaughtForSlot(u32 townSlot) const { return seaCreaturesCaught.playerStats[townSlot]; } // 0x0076837C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetEggsGivenToZipperForSlot(u32 townSlot) const { return eggsGivenToZipper.playerStats[townSlot]; } // 0x00768388
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetMeowCouponsEarnedForSlot(u32 townSlot) const { return meowCouponsEarned.playerStats[townSlot]; } // 0x00768394
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetIslandVisitsForSlot(u32 townSlot) const { return islandVisits.playerStats[townSlot]; } // 0x007683A0
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetShootingStarWishesForSlot(u32 townSlot) const { return shootingStarWishes.playerStats[townSlot]; } // 0x007683AC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetPublicWorksBuiltTotal() const { return publicWorksBuilt.totalPlayerStat; } // 0x007683B8
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetUnk0x62CTotal() const { return unk_0x62C.totalPlayerStat; } // 0x007683C0
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetLoanPaidTotal() const { return loanPaid.totalPlayerStat; } // 0x007683C8
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetSaharaRedecorationsForSlot(u32 townSlot) const { return saharaRedecorations.playerStats[townSlot]; } // 0x007683D0
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFurnitureCustomizedForSlot(u32 townSlot) const { return furnitureCustomized.playerStats[townSlot]; } // 0x007683DC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetAbdBalanceForSlot(u32 townSlot) const { return abdBalance.playerStats[townSlot]; } // 0x007683E8
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetAbleSistersExpensesTotal() const { return ableSistersExpenses.totalPlayerStat; } // 0x007683F4
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetMeowCouponsSpentForSlot(u32 townSlot) const { return meowCouponsSpent.playerStats[townSlot]; } // 0x007683FC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetBalloonsPoppedForSlot(u32 townSlot) const { return balloonsPopped.playerStats[townSlot]; } // 0x00768408
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetNpcPicturesReceivedForSlot(u32 townSlot) const { return npcPicturesReceived.playerStats[townSlot]; } // 0x00768414
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetJobsAtTheRoost() const { return jobsAtTheRoost.playerStats[SvGetCurrentTownSlot()]; } // 0x00768420
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetBellsSpentTotal() const { return bellsSpent.totalPlayerStat; } // 0x00768438
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetPartyPoppersPoppedAtNewYearsForSlot(u32 townSlot) const { return partyPoppersPoppedAtNewYears.playerStats[townSlot]; } // 0x00768440
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetDjkkVisits() const { return djkkVisits.playerStats[SvGetCurrentTownSlot()]; } // 0x0076844C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTourneyFishCaughtTotal() const { return tourneyFishCaught.totalPlayerStat; } // 0x00768464
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFishCaughtForSlot(u32 townSlot) const { return fishCaught.playerStats[townSlot]; } // 0x0076846C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetHarvestFestivalCoursesDoneForSlot(u32 townSlot) const { return harvestFestivalCoursesDone.playerStats[townSlot]; } // 0x00768478
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetPlayedHideAndSeek() const { return playedHideAndSeek.playerStats[SvGetCurrentTownSlot()]; } // 0x00768484
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTownInitiativesCompletedTotal() const { return townInitiativesCompleted.totalPlayerStat; } // 0x0076849C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetLostItemsGivenBackTotal() const { return lostItemsGivenBack.totalPlayerStat; } // 0x007684A4
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFruitsGrownOnTheBeachTotal() const { return fruitsGrownOnTheBeach.totalPlayerStat; } // 0x007684AC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetBoughtArtAtReddsForSlot(u32 townSlot) const { return boughtArtAtRedds.playerStats[townSlot]; } // 0x007684B4
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetHelpedGulliver() const { return helpedGulliver.playerStats[SvGetCurrentTownSlot()]; } // 0x007684C0
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetPublicWorkExpensesForSlot(u32 townSlot) const { return publicWorkExpenses.playerStats[townSlot]; } // 0x007684D8
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetResetSurveillanceCenterVisitsForSlot(u32 townSlot) const { return resetSurveillanceCenterVisits.playerStats[townSlot]; } // 0x007684E4
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetScallopsGivenToPascalForSlot(u32 townSlot) const { return scallopsGivenToPascal.playerStats[townSlot]; } // 0x007684F0
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTurnipsSoldTotal() const { return turnipsSold.totalPlayerStat; } // 0x007684FC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetRecycleShopEarningsForSlot(u32 townSlot) const { return recycleShopEarnings.playerStats[townSlot]; } // 0x00768504
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetLettersSent() const { return lettersSent.playerStats[SvGetCurrentTownSlot()]; } // 0x00768510
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetKatrinaVisitsTotal() const { return katrinaVisits.totalPlayerStat; } // 0x00768528
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetSnowflakesCaughtTotal() const { return snowflakesCaught.totalPlayerStat; } // 0x00768530
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetSnowmenBuiltByYouInTownForSlot(u32 townSlot) const { return snowmenBuiltByYouInTown.playerStats[townSlot]; } // 0x00768538
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetUnk0x564Total() const { return unk_0x564.totalPlayerStat; } // 0x00768544
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTourVisits() const { return tourVisits.playerStats[SvGetCurrentTownSlot()]; } // 0x0076854C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetYouVisitedVillagersTotal() const { return youVisitedVillagers.totalPlayerStat; } // 0x00768564
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTownVisitorsForSlot(u32 townSlot) const { return townVisitors.playerStats[townSlot]; } // 0x0076856C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetAmiiboScanned() const { return amiiboScanned.playerStats[SvGetCurrentTownSlot()]; } // 0x00768578
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetEarnedBadges() const { return earnedBadges.playerStats[SvGetCurrentTownSlot()]; } // 0x00768590
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTotalBingoWins() const { return totalBingoWins.playerStats[SvGetCurrentTownSlot()]; } // 0x007685A8
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTurnipExpensesTotal() const { return turnipExpenses.totalPlayerStat; } // 0x007685C0
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFestivalFeathersCaughtForSlot(u32 townSlot) const { return festivalFeathersCaught.playerStats[townSlot]; } // 0x007685C8
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTourneyInsectCaughtTotal() const { return tourneyInsectCaught.totalPlayerStat; } // 0x007685D4
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetBugsCaughtForSlot(u32 townSlot) const { return bugsCaught.playerStats[townSlot]; } // 0x007685DC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetVillagersVisitedYouTotal() const { return villagersVisitedYou.totalPlayerStat; } // 0x007685E8
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetDreamTownsVisitedForSlot(u32 townSlot) const { return dreamTownsVisited.playerStats[townSlot]; } // 0x007685F0
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFireworkDesignsGivenToIsabelleForSlot(u32 townSlot) const { return fireworkDesignsGivenToIsabelle.playerStats[townSlot]; } // 0x007685FC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetHelpedKatieTravel() const { return helpedKatieTravel.playerStats[SvGetCurrentTownSlot()]; } // 0x00768608
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetUnk0x550ForSlot(u32 townSlot) const { return unk_0x550.playerStats[townSlot]; } // 0x00768620
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetShampoodleVisits() const { return shampoodleVisits.playerStats[SvGetCurrentTownSlot()]; } // 0x0076862C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetKicksExpensesForSlot(u32 townSlot) const { return kicksExpenses.playerStats[townSlot]; } // 0x00768644
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetShrunkSketchesAttended() const { return shrunkSketchesAttended.playerStats[SvGetCurrentTownSlot()]; } // 0x00768650
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFruitGrownTotal() const { return fruitGrown.totalPlayerStat; } // 0x00768668
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetKkSliderConcertsAttendedForSlot(u32 townSlot) const { return kkSliderConcertsAttended.playerStats[townSlot]; } // 0x00768670
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTownsVisited() const { return townsVisited.playerStats[SvGetCurrentTownSlot()]; } // 0x0076867C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFlowersWateredForSlot(u32 townSlot) const { return flowersWatered.playerStats[townSlot]; } // 0x00768694
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFossilsAnalyzedForSlot(u32 townSlot) const { return fossilsAnalyzed.playerStats[townSlot]; } // 0x007686A0
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFoundImposterBlancaForSlot(u32 townSlot) const { return foundImposterBlanca.playerStats[townSlot]; } // 0x007686AC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetGivenToyDayPresentsForSlot(u32 townSlot) const { return givenToyDayPresents.playerStats[townSlot]; } // 0x007686B8
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetProDesignsCreatedForSlot(u32 townSlot) const { return proDesignsCreated.playerStats[townSlot]; } // 0x007686C4
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetReceivedCandyOnHalloweenForSlot(u32 townSlot) const { return receivedCandyOnHalloween.playerStats[townSlot]; } // 0x007686D0
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetIslandVisits() const { return islandVisits.playerStats[SvGetCurrentTownSlot()]; } // 0x007686DC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetShootingStarWishes() const { return shootingStarWishes.playerStats[SvGetCurrentTownSlot()]; } // 0x007686F4
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetSaharaRedecorations() const { return saharaRedecorations.playerStats[SvGetCurrentTownSlot()]; } // 0x0076870C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFurnitureCustomized() const { return furnitureCustomized.playerStats[SvGetCurrentTownSlot()]; } // 0x00768724
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTurnipProfitsTotal() const { return turnipProfits.totalPlayerStat; } // 0x0076873C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetGardeningShopExpensesTotal() const { return gardeningShopExpenses.totalPlayerStat; } // 0x00768744
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetGraciesExpensesForSlot(u32 townSlot) const { return graciesExpenses.playerStats[townSlot]; } // 0x0076874C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetNooklingsExpensesTotal() const { return nooklingsExpenses.totalPlayerStat; } // 0x00768758
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetBalloonsPopped() const { return balloonsPopped.playerStats[SvGetCurrentTownSlot()]; } // 0x00768760
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetNpcPicturesReceived() const { return npcPicturesReceived.playerStats[SvGetCurrentTownSlot()]; } // 0x00768778
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTurnipsBoughtForSlot(u32 townSlot) const { return turnipsBought.playerStats[townSlot]; } // 0x00768790
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFamousMushroomsEatenTotal() const { return famousMushroomsEaten.totalPlayerStat; } // 0x0076879C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetBoughtArtAtRedds() const { return boughtArtAtRedds.playerStats[SvGetCurrentTownSlot()]; } // 0x007687A4
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetLoanPaidForSlot(u32 townSlot) const { return loanPaid.playerStats[townSlot]; } // 0x007687BC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetResetSurveillanceCenterVisits() const { return resetSurveillanceCenterVisits.playerStats[SvGetCurrentTownSlot()]; } // 0x007687C8
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetScallopsGivenToPascal() const { return scallopsGivenToPascal.playerStats[SvGetCurrentTownSlot()]; } // 0x007687E0
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetAbleSistersExpensesForSlot(u32 townSlot) const { return ableSistersExpenses.playerStats[townSlot]; } // 0x007687F8
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTownVisitors() const { return townVisitors.playerStats[SvGetCurrentTownSlot()]; } // 0x00768804
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetBellsSpentForSlot(u32 townSlot) const { return bellsSpent.playerStats[townSlot]; } // 0x0076881C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetDreamTownsVisited() const { return dreamTownsVisited.playerStats[SvGetCurrentTownSlot()]; } // 0x00768828
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTourneyFishCaughtForSlot(u32 townSlot) const { return tourneyFishCaught.playerStats[townSlot]; } // 0x00768840
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTownInitiativesCompletedForSlot(u32 townSlot) const { return townInitiativesCompleted.playerStats[townSlot]; } // 0x0076884C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetLostItemsGivenBackForSlot(u32 townSlot) const { return lostItemsGivenBack.playerStats[townSlot]; } // 0x00768858
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetUnk0x550() const { return unk_0x550.playerStats[SvGetCurrentTownSlot()]; } // 0x00768864
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTurnipsSoldForSlot(u32 townSlot) const { return turnipsSold.playerStats[townSlot]; } // 0x0076887C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetKatrinaVisitsForSlot(u32 townSlot) const { return katrinaVisits.playerStats[townSlot]; } // 0x00768888
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetKicksExpenses() const { return kicksExpenses.playerStats[SvGetCurrentTownSlot()]; } // 0x00768894
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetSnowflakesCaughtForSlot(u32 townSlot) const { return snowflakesCaught.playerStats[townSlot]; } // 0x007688AC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetUnk0x564ForSlot(u32 townSlot) const { return unk_0x564.playerStats[townSlot]; } // 0x007688B8
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetKkSliderConcertsAttended() const { return kkSliderConcertsAttended.playerStats[SvGetCurrentTownSlot()]; } // 0x007688C4
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetYouVisitedVillagersForSlot(u32 townSlot) const { return youVisitedVillagers.playerStats[townSlot]; } // 0x007688DC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFossilsAnalyzed() const { return fossilsAnalyzed.playerStats[SvGetCurrentTownSlot()]; } // 0x007688E8
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTurnipExpensesForSlot(u32 townSlot) const { return turnipExpenses.playerStats[townSlot]; } // 0x00768900
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTourneyInsectCaughtForSlot(u32 townSlot) const { return tourneyInsectCaught.playerStats[townSlot]; } // 0x0076890C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetVillagersVisitedYouForSlot(u32 townSlot) const { return villagersVisitedYou.playerStats[townSlot]; } // 0x00768918
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetProDesignsCreated() const { return proDesignsCreated.playerStats[SvGetCurrentTownSlot()]; } // 0x00768924
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetGraciesExpenses() const { return graciesExpenses.playerStats[SvGetCurrentTownSlot()]; } // 0x0076893C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetTurnipProfitsForSlot(u32 townSlot) const { return turnipProfits.playerStats[townSlot]; } // 0x00768954
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetGardeningShopExpensesForSlot(u32 townSlot) const { return gardeningShopExpenses.playerStats[townSlot]; } // 0x00768960
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetNooklingsExpensesForSlot(u32 townSlot) const { return nooklingsExpenses.playerStats[townSlot]; } // 0x0076896C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFamousMushroomsEatenForSlot(u32 townSlot) const { return famousMushroomsEaten.playerStats[townSlot]; } // 0x00768978
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetLostItemsGivenBack() const { return lostItemsGivenBack.playerStats[SvGetCurrentTownSlot()]; } // 0x00768984
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetKatrinaVisits() const { return katrinaVisits.playerStats[SvGetCurrentTownSlot()]; } // 0x0076899C
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetSnowflakesCaught() const { return snowflakesCaught.playerStats[SvGetCurrentTownSlot()]; } // 0x007689B4
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetUnk0x564() const { return unk_0x564.playerStats[SvGetCurrentTownSlot()]; } // 0x007689CC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetYouVisitedVillagers() const { return youVisitedVillagers.playerStats[SvGetCurrentTownSlot()]; } // 0x007689E4
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetVillagersVisitedYou() const { return villagersVisitedYou.playerStats[SvGetCurrentTownSlot()]; } // 0x007689FC
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetGardeningShopExpenses() const { return gardeningShopExpenses.playerStats[SvGetCurrentTownSlot()]; } // 0x00768A14
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetFamousMushroomsEaten() const { return famousMushroomsEaten.playerStats[SvGetCurrentTownSlot()]; } // 0x00768A2C
};
ASSERT_SIZE(SvCensus, 0x1414);

// the census with its checksum (CRC-32 MSB first, nn::util::Crc32Msb), in the shared data (name is ours)
// the census of the save data in memory: the same as GetCensus (dSvApi.h), but through GardenPlus (name is ours)
SvCensus* GetCensusOfGardenPlus(); // 0x006B618C

// The census menu (unused, see SvPlayerFlags::canUseCensusMenu; names are ours). The menu shows its rows in the order of
// GetCensusValueOfRow; a value index (SvCensusValueId) selects the value, its title (STR_Kokusei_Title) and unit
// (STR_Kokusei_Unit).
// whether the row of a value is shown for the current player: the money and nature values always, the others once the
// player has done it (value > 0), the event values while their event takes place
bool IsCensusValueShown(u32 index); // 0x005D10F8
// (game code, not decompiled yet; name after the address) a bit of the four player entries of GardenPlus; the bingo row
// of the census menu depends on it
bool Func226D48(); // 0x00226D48
// the value index of a row of the menu (no range check)
u8 GetCensusValueOfRow(u32 row); // 0x005D1908
// the value of a player by its town slot; the values without player values give the total
s32 GetCensusValueForSlot(u32 index, u32 townSlot); // 0x005D1918
// the total of a value (0 for an unknown index)
s32 GetCensusValueTotal(u32 index); // 0x005D2010
// the message index of the unit (STR_Kokusei_Unit) and of the title (STR_Kokusei_Title) of a value (0 out of range)
u32 GetCensusUnitIndex(u32 index); // 0x005D27A8
u32 GetCensusTitleIndex(u32 index); // 0x005D27C0

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

