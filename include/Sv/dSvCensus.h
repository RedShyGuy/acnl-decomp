#pragma once

// Census (statistics) data of all players.
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"

#pragma pack(push, 1)

struct SvCensusValue {
    /* 0x0000 */ u32 totalPlayerStat; // All player stats combined
    /* 0x0004 */ u32 playerStats[4]; // For each player
};
ASSERT_SIZE(SvCensusValue, 0x14);

struct SvInsectCatchCounts {
    /* 0x0000 */ u32 commonButterflyCaught;
    /* 0x0004 */ u32 yellowButterflyCaught;
    /* 0x0008 */ u32 tigerButterflyCaught;
    /* 0x000C */ u32 peacockButterflyCaught;
    /* 0x0010 */ u32 monarchButterflyCaught;
    /* 0x0014 */ u32 emperorButterflyCaught;
    /* 0x0018 */ u32 agriasButterflyCaught;
    /* 0x001C */ u32 rajaBCaught;
    /* 0x0020 */ u32 birdwingButterflyCaught;
    /* 0x0024 */ u32 mothCaught;
    /* 0x0028 */ u32 oakSilkMothCaught;
    /* 0x002C */ u32 honeybeeCaught;
    /* 0x0030 */ u32 beeCaught;
    /* 0x0034 */ u32 longLocustCaught;
    /* 0x0038 */ u32 migratoryLocustCaught;
    /* 0x003C */ u32 riceGrasshopperCaught;
    /* 0x0040 */ u32 mantisCaught;
    /* 0x0044 */ u32 orchidMantisCaught;
    /* 0x0048 */ u32 brownCicadaCaught;
    /* 0x004C */ u32 robustCicadaCaught;
    /* 0x0050 */ u32 giantCicadaCaught;
    /* 0x0054 */ u32 walkerCicadaCaught;
    /* 0x0058 */ u32 eveningCicadaCaught;
    /* 0x005C */ u32 cicadaShellCaught;
    /* 0x0060 */ u32 lanternFlyCaught;
    /* 0x0064 */ u32 redDragonflyCaught;
    /* 0x0068 */ u32 darnerDragonflyCaught;
    /* 0x006C */ u32 bandedDragonflyCaught;
    /* 0x0070 */ u32 petaltailDragonflyCaught;
    /* 0x0074 */ u32 antCaught;
    /* 0x0078 */ u32 pondskaterCaught;
    /* 0x007C */ u32 divingBeetleCaught;
    /* 0x0080 */ u32 stinkbugCaught;
    /* 0x0084 */ u32 snailCaught;
    /* 0x0088 */ u32 cricketCaught;
    /* 0x008C */ u32 bellCricketCaught;
    /* 0x0090 */ u32 grasshopperCaught;
    /* 0x0094 */ u32 moleCricketCaught;
    /* 0x0098 */ u32 walkingLeafCaught;
    /* 0x009C */ u32 walkingStickCaught;
    /* 0x00A0 */ u32 bagwormCaught;
    /* 0x00A4 */ u32 ladybugCaught;
    /* 0x00A8 */ u32 violinBeetleCaught;
    /* 0x00AC */ u32 longhornBeetleCaught;
    /* 0x00B0 */ u32 tigerBeetleCaught;
    /* 0x00B4 */ u32 dungBeetleCaught;
    /* 0x00B8 */ u32 wharfRoachCaught;
    /* 0x00BC */ u32 hermitCrabCaught;
    /* 0x00C0 */ u32 fireflyCaught;
    /* 0x00C4 */ u32 fruitBeetleCaught;
    /* 0x00C8 */ u32 scarabBeetleCaught;
    /* 0x00CC */ u32 jewelBeetleCaught;
    /* 0x00D0 */ u32 miyamaStagCaught;
    /* 0x00D4 */ u32 sawStagCaught;
    /* 0x00D8 */ u32 giantStagCaught;
    /* 0x00DC */ u32 rainbowStagCaught;
    /* 0x00E0 */ u32 cyclommatusStagCaught;
    /* 0x00E4 */ u32 goldenStagCaught;
    /* 0x00E8 */ u32 hornedDynastidCaught;
    /* 0x00EC */ u32 hornedAtlasCaught;
    /* 0x00F0 */ u32 hornedElephantCaught;
    /* 0x00F4 */ u32 hornedHerculesCaught;
    /* 0x00F8 */ u32 goliathBeetleCaught;
    /* 0x00FC */ u32 fleaCaught;
    /* 0x0100 */ u32 pillBugCaught;
    /* 0x0104 */ u32 mosquitoCaught;
    /* 0x0108 */ u32 flyCaught;
    /* 0x010C */ u32 houseCentipedeCaught;
    /* 0x0110 */ u32 centipedeCaught;
    /* 0x0114 */ u32 spiderCaught;
    /* 0x0118 */ u32 tarantulaCaught;
    /* 0x011C */ u32 scorpionCaught;
};
ASSERT_SIZE(SvInsectCatchCounts, 0x120);

struct SvFishCatchCounts {
    /* 0x0000 */ u32 bitterlingCaught;
    /* 0x0004 */ u32 paleChubCaught;
    /* 0x0008 */ u32 crucianCarpCaught;
    /* 0x000C */ u32 daceCaught;
    /* 0x0010 */ u32 barbelSteedCaught;
    /* 0x0014 */ u32 carpCaught;
    /* 0x0018 */ u32 koiCaught;
    /* 0x001C */ u32 goldfishCaught;
    /* 0x0020 */ u32 popEyedGoldfishCaught;
    /* 0x0024 */ u32 killifishCaught;
    /* 0x0028 */ u32 crawfishCaught;
    /* 0x002C */ u32 softShelledTurtleCaught;
    /* 0x0030 */ u32 tadpoleCaught;
    /* 0x0034 */ u32 frogCaught;
    /* 0x0038 */ u32 freshwaterGobyCaught;
    /* 0x003C */ u32 loachCaught;
    /* 0x0040 */ u32 catfishCaught;
    /* 0x0044 */ u32 eelCaught;
    /* 0x0048 */ u32 giantSnakeheadCaught;
    /* 0x004C */ u32 bluegillCaught;
    /* 0x0050 */ u32 yellowPerchCaught;
    /* 0x0054 */ u32 blackBassCaught;
    /* 0x0058 */ u32 pikeCaught;
    /* 0x005C */ u32 pondSmeltCaught;
    /* 0x0060 */ u32 sweetfishCaught;
    /* 0x0064 */ u32 cherrySalmonCaught;
    /* 0x0068 */ u32 charCaught;
    /* 0x006C */ u32 rainbowTroutCaught;
    /* 0x0070 */ u32 stringfishCaught;
    /* 0x0074 */ u32 salmonCaught;
    /* 0x0078 */ u32 kingSalmonCaught;
    /* 0x007C */ u32 mittenCrabCaught;
    /* 0x0080 */ u32 guppyCaught;
    /* 0x0084 */ u32 nibbleFishCaught;
    /* 0x0088 */ u32 angelfishCaught;
    /* 0x008C */ u32 neonTetraCaught;
    /* 0x0090 */ u32 piranhaCaught;
    /* 0x0094 */ u32 arowanaCaught;
    /* 0x0098 */ u32 doradoCaught;
    /* 0x009C */ u32 garCaught;
    /* 0x00A0 */ u32 arapaimaCaught;
    /* 0x00A4 */ u32 saddledBichirCaught;
    /* 0x00A8 */ u32 seaButterflyCaught;
    /* 0x00AC */ u32 seaHorseCaught;
    /* 0x00B0 */ u32 clownFishCaught;
    /* 0x00B4 */ u32 surgeonfishCaught;
    /* 0x00B8 */ u32 butterflyFishCaught;
    /* 0x00BC */ u32 napoleonfishCaught;
    /* 0x00C0 */ u32 zebraTurkeyfishCaught;
    /* 0x00C4 */ u32 blowfishCaught;
    /* 0x00C8 */ u32 pufferFishCaught;
    /* 0x00CC */ u32 horsemackerelCaught;
    /* 0x00D0 */ u32 barredKnifejawCaught;
    /* 0x00D4 */ u32 seaBassCaught;
    /* 0x00D8 */ u32 redSnapperCaught;
    /* 0x00DC */ u32 dabCaught;
    /* 0x00E0 */ u32 oliveFlounderCaught;
    /* 0x00E4 */ u32 squidCaught;
    /* 0x00E8 */ u32 morayEelCaught;
    /* 0x00EC */ u32 ribbonEelCaught;
    /* 0x00F0 */ u32 footballFishCaught;
    /* 0x00F4 */ u32 tunaCaught;
    /* 0x00F8 */ u32 blueMarlinCaught;
    /* 0x00FC */ u32 giantTrevallyCaught;
    /* 0x0100 */ u32 rayCaught;
    /* 0x0104 */ u32 oceanSunfishCaught;
    /* 0x0108 */ u32 hammerheadSharkCaught;
    /* 0x010C */ u32 sharkCaught;
    /* 0x0110 */ u32 sawSharkCaught;
    /* 0x0114 */ u32 whaleSharkCaught;
    /* 0x0118 */ u32 oarfishCaught;
    /* 0x011C */ u32 coelacanthCaught;
};
ASSERT_SIZE(SvFishCatchCounts, 0x120);

struct SvSeaCreatureCatchCounts {
    /* 0x0000 */ u32 seaweedCaught;
    /* 0x0004 */ u32 seaGrapesCaught;
    /* 0x0008 */ u32 seaUrchinCaught;
    /* 0x000C */ u32 acornBarnacleCaught;
    /* 0x0010 */ u32 oysterCaught;
    /* 0x0014 */ u32 turbanShellCaught;
    /* 0x0018 */ u32 abaloneCaught;
    /* 0x001C */ u32 earShellCaught;
    /* 0x0020 */ u32 clamCaught;
    /* 0x0024 */ u32 pearlOysterCaught;
    /* 0x0028 */ u32 scallopCaught;
    /* 0x002C */ u32 seaAnemoneCaught;
    /* 0x0030 */ u32 seaStarCaught;
    /* 0x0034 */ u32 seaCucumberCaught;
    /* 0x0038 */ u32 seaSlugCaught;
    /* 0x003C */ u32 flatwormCaught;
    /* 0x0040 */ u32 mantisShrimpCaught;
    /* 0x0044 */ u32 sweetShrimpCaught;
    /* 0x0048 */ u32 tigerPrawnCaught;
    /* 0x004C */ u32 spinyLobsterCaught;
    /* 0x0050 */ u32 lobsterCaught;
    /* 0x0054 */ u32 snowCrabCaught;
    /* 0x0058 */ u32 horsehairCrabCaught;
    /* 0x005C */ u32 redKingCrabCaught;
    /* 0x0060 */ u32 spiderCrabCaught;
    /* 0x0064 */ u32 octopusCaught;
    /* 0x0068 */ u32 spottedGardenEelCaught;
    /* 0x006C */ u32 chamberedNautilusCaught;
    /* 0x0070 */ u32 horseshoeCrabCaught;
    /* 0x0074 */ u32 giantIsopodCaught;
    /* 0x0078 */ u32 waterEggCaught; // The other eggs are not listed as a stat, idk why
};
ASSERT_SIZE(SvSeaCreatureCatchCounts, 0x7C);

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
    /* 0x0640 */ SvInsectCatchCounts insectsCaughtData;
    /* 0x0760 */ u32 unk_0x760[0x120];
    /* 0x0BE0 */ SvFishCatchCounts fishCaughtData;
    /* 0x0D00 */ u32 unk_0xD00[0x120];
    /* 0x1180 */ u32 emptyCanCaught;
    /* 0x1184 */ u32 bootCaught;
    /* 0x1188 */ u32 oldTireCaught;
    /* 0x118C */ u32 unk_0x118C[0xC];
    /* 0x11BC */ SvSeaCreatureCatchCounts seaCreaturesCaughtData;
    /* 0x1238 */ u32 unk_0x1238[0x83];
};
ASSERT_SIZE(SvCensus, 0x1444);

#pragma pack(pop)
