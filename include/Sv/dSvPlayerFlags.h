#pragma once

// Progress / tutorial flags of a player (0x34 bytes of bits).
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"

#pragma pack(push, 1)

// 0x57A0 -> 0x57D3
struct SvPlayerFlags {
    /* 0x0000.0 */ u8 unk_0x0_0 : 1;
    /* 0x0000.1 */ u8 unk_0x0_1 : 1;
    /* 0x0000.2 */ u8 unk_0x0_2 : 1;
    /* 0x0000.3 */ u8 unk_0x0_3 : 1;
    /* 0x0000.4 */ u8 playerSetNameAndTownName : 1; // most likely player set their name and townname
    /* 0x0000.5 */ u8 reddIntroduced : 1;
    /* 0x0000.6 */ u8 unk_0x0_6 : 1;
    /* 0x0000.7 */ u8 unk_0x0_7 : 1;
    /* 0x0001.0 */ u8 unk_0x1_0 : 1;
    /* 0x0001.1 */ u8 finishedFirstDay : 1; // Introduction day finished
    /* 0x0001.2 */ u8 unk_0x1_2 : 1; // Meet Tommy or Timmy in shop for first time
    /* 0x0001.3 */ u8 unk_0x1_3 : 1;
    /* 0x0001.4 */ u8 unk_0x1_4 : 1;
    /* 0x0001.5 */ u8 unk_0x1_5 : 1;
    /* 0x0001.6 */ u8 unk_0x1_6 : 1;
    /* 0x0001.7 */ u8 unk_0x1_7 : 1;
    /* 0x0002.0 */ u8 unk_0x2_0 : 1;
    /* 0x0002.1 */ u8 unk_0x2_1 : 1;
    /* 0x0002.2 */ u8 unk_0x2_2 : 1;
    /* 0x0002.3 */ u8 unk_0x2_3 : 1;
    /* 0x0002.4 */ u8 unk_0x2_4 : 1;
    /* 0x0002.5 */ u8 unk_0x2_5 : 1;
    /* 0x0002.6 */ u8 unk_0x2_6 : 1;
    /* 0x0002.7 */ u8 unk_0x2_7 : 1;
    /* 0x0003.0 */ u8 unk_0x3_0 : 1;
    /* 0x0003.1 */ u8 unk_0x3_1 : 1;
    /* 0x0003.2 */ u8 unk_0x3_2 : 1;
    /* 0x0003.3 */ u8 tomNookIntroduced : 1;
    /* 0x0003.4 */ u8 blathersIntroduced : 1; // somehow also set tom nook introduced to 1???
    /* 0x0003.5 */ u8 unk_0x3_5 : 1;
    /* 0x0003.6 */ u8 celesteIntroduced : 1;
    /* 0x0003.7 */ u8 cyrusIntroduced : 1;
    /* 0x0004.0 */ u8 unk_0x4_0 : 1;
    /* 0x0004.1 */ u8 unk_0x4_1 : 1; // Reese and Tommy conversation (ReTail)
    /* 0x0004.2 */ u8 unk_0x4_2 : 1;
    /* 0x0004.3 */ u8 unk_0x4_3 : 1;
    /* 0x0004.4 */ u8 tomNookDownFirstPayment : 1;
    /* 0x0004.5 */ u8 chooseHouseLocation : 1; // Tom nook first time meeting -> finding place to build house
    /* 0x0004.6 */ u8 mainStreetUnlocked : 1; // go to tom nook after talking to isabelle in town hall at start of game
    /* 0x0004.7 */ u8 houseLocationChosen : 1; // Build House (Tom Nook placed down tent)
    /* 0x0005.0 */ u8 knowIsabelleName : 1;
    /* 0x0005.1 */ u8 knowKappn : 1;
    /* 0x0005.2 */ u8 unk_0x5_2 : 1; // after repaying loan in post office
    /* 0x0005.3 */ u8 unk_0x5_3 : 1; // set birthday in town hall / received TPC
    /* 0x0005.4 */ u8 unk_0x5_4 : 1;
    /* 0x0005.5 */ u8 unk_0x5_5 : 1;
    /* 0x0005.6 */ u8 unk_0x5_6 : 1;
    /* 0x0005.7 */ u8 unk_0x5_7 : 1;
    /* 0x0006.0 */ u8 unk_0x6_0 : 1; // Met Resetti for the first time
    /* 0x0006.1 */ u8 unk_0x6_1 : 1;
    /* 0x0006.2 */ u8 unk_0x6_2 : 1;
    /* 0x0006.3 */ u8 unk_0x6_3 : 1;
    /* 0x0006.4 */ u8 unk_0x6_4 : 1;
    /* 0x0006.5 */ u8 unk_0x6_5 : 1;
    /* 0x0006.6 */ u8 unk_0x6_6 : 1;
    /* 0x0006.7 */ u8 unk_0x6_7 : 1;
    /* 0x0007.0 */ u8 unk_0x7_0 : 1;
    /* 0x0007.1 */ u8 unk_0x7_1 : 1;
    /* 0x0007.2 */ u8 unk_0x7_2 : 1;
    /* 0x0007.3 */ u8 unk_0x7_3 : 1;
    /* 0x0007.4 */ u8 unk_0x7_4 : 1; // Meet Tommy or Timmy in shop for first time
    /* 0x0007.5 */ u8 unk_0x7_5 : 1;
    /* 0x0007.6 */ u8 unk_0x7_6 : 1;
    /* 0x0007.7 */ u8 unk_0x7_7 : 1;
    /* 0x0008.0 */ u8 unk_0x8_0 : 1;
    /* 0x0008.1 */ u8 unk_0x8_1 : 1;
    /* 0x0008.2 */ u8 unk_0x8_2 : 1;
    /* 0x0008.3 */ u8 unk_0x8_3 : 1; // Porter Introduction
    /* 0x0008.4 */ u8 unk_0x8_4 : 1; // i think got map from isabelle at start of game
    /* 0x0008.5 */ u8 unk_0x8_5 : 1;
    /* 0x0008.6 */ u8 unk_0x8_6 : 1;
    /* 0x0008.7 */ u8 unk_0x8_7 : 1;
    /* 0x0009.0 */ u8 unk_0x9_0 : 1;
    /* 0x0009.1 */ u8 unk_0x9_1 : 1;
    /* 0x0009.2 */ u8 unk_0x9_2 : 1;
    /* 0x0009.3 */ u8 unk_0x9_3 : 1;
    /* 0x0009.4 */ u8 unk_0x9_4 : 1;
    /* 0x0009.5 */ u8 unk_0x9_5 : 1;
    /* 0x0009.6 */ u8 unk_0x9_6 : 1;
    /* 0x0009.7 */ u8 unk_0x9_7 : 1;
    /* 0x000A.0 */ u8 unk_0xA_0 : 1;
    /* 0x000A.1 */ u8 resetPending : 1;
    /* 0x000A.2 */ u8 unk_0xA_2 : 1; // Villager related
    /* 0x000A.3 */ u8 unk_0xA_3 : 1; // Villager related
    /* 0x000A.4 */ u8 unk_0xA_4 : 1; // Villager related
    /* 0x000A.5 */ u8 unk_0xA_5 : 1;
    /* 0x000A.6 */ u8 unk_0xA_6 : 1; // isabelle welcomed you in your tent (actually also gets set when placing any item in house for first time, if never been in tent)
    /* 0x000A.7 */ u8 peteIntroduction : 1;
    /* 0x000B.0 */ u8 unk_0xB_0 : 1; // Reese and Tommy conversation (ReTail)
    /* 0x000B.1 */ u8 unk_0xB_1 : 1;
    /* 0x000B.2 */ u8 unk_0xB_2 : 1;
    /* 0x000B.3 */ u8 exteriorRenovationsUnlocked : 1; // If house is max size
    /* 0x000B.4 */ u8 houseLoanRepaid : 1; // after repaying loan in post office = 1 | / Talking to tom nook to let him know you payed your loan = 0
    /* 0x000B.5 */ u8 unk_0xB_5 : 1;
    /* 0x000B.6 */ u8 unk_0xB_6 : 1; // Gets set when you talk to tom nook after your house was built = 0
    /* 0x000B.7 */ u8 houseUpgradeFinished : 1; // next day when house upgrade is finished = 1 | when you talk to tom nook after your house was built = 0
    /* 0x000C.0 */ u8 unk_0xC_0 : 1;
    /* 0x000C.1 */ u8 unk_0xC_1 : 1;
    /* 0x000C.2 */ u8 unk_0xC_2 : 1;
    /* 0x000C.3 */ u8 unk_0xC_3 : 1;
    /* 0x000C.4 */ u8 unk_0xC_4 : 1;
    /* 0x000C.5 */ u8 museumExhibitExplained : 1;
    /* 0x000C.6 */ u8 unk_0xC_6 : 1;
    /* 0x000C.7 */ u8 unk_0xC_7 : 1;
    /* 0x000D.0 */ u8 permitApprovalArrived : 1; // day of arrival of permit
    /* 0x000D.1 */ u8 mayorJobIntroduction : 1; // after permit
    /* 0x000D.2 */ u8 permitApproval : 1; // if approval arrived
    /* 0x000D.3 */ u8 permitIntroduction : 1; // Isabelle told you about the mayor permit
    /* 0x000D.4 */ u8 citizenSatisfactionExplained : 1;
    /* 0x000D.5 */ u8 townFlagExplained : 1;
    /* 0x000D.6 */ u8 townTuneExplained : 1;
    /* 0x000D.7 */ u8 pwpExplained : 1;
    /* 0x000E.0 */ u8 ordinanceExplained : 1;
    /* 0x000E.1 */ u8 unk_0xE_1 : 1;
    /* 0x000E.2 */ u8 unk_0xE_2 : 1;
    /* 0x000E.3 */ u8 permitFinished : 1;
    /* 0x000E.4 */ u8 unk_0xE_4 : 1;
    /* 0x000E.5 */ u8 unk_0xE_5 : 1;
    /* 0x000E.6 */ u8 hasBeeSting : 1;
    /* 0x000E.7 */ u8 unk_0xE_7 : 1;
    /* 0x000F.0 */ u8 editDesignIntro : 1;
    /* 0x000F.1 */ u8 unk_0xF_1 : 1;
    /* 0x000F.2 */ u8 unk_0xF_2 : 1;
    /* 0x000F.3 */ u8 canTravel : 1; // i.e. can use train
    /* 0x000F.4 */ u8 hasTpcPicture : 1;
    /* 0x000F.5 */ u8 unk_0xF_5 : 1; // after first time asking porter to travel with no TPC
    /* 0x000F.6 */ u8 unk_0xF_6 : 1; // First time going in able sisters (Sable and mable)
    /* 0x000F.7 */ u8 unk_0xF_7 : 1;
    /* 0x0010.0 */ u8 unk_0x10_0 : 1;
    /* 0x0010.1 */ u8 unk_0x10_1 : 1;
    /* 0x0010.2 */ u8 unk_0x10_2 : 1;
    /* 0x0010.3 */ u8 unk_0x10_3 : 1;
    /* 0x0010.4 */ u8 befriendSable1 : 1;
    /* 0x0010.5 */ u8 unk_0x10_5 : 1;
    /* 0x0010.6 */ u8 befriendSable2 : 1;
    /* 0x0010.7 */ u8 unk_0x10_7 : 1;
    /* 0x0011.0 */ u8 befriendSable3 : 1;
    /* 0x0011.1 */ u8 unk_0x11_1 : 1;
    /* 0x0011.2 */ u8 unk_0x11_2 : 1;
    /* 0x0011.3 */ u8 unk_0x11_3 : 1; // Gets set when you talk to tom nook after your house was built
    /* 0x0011.4 */ u8 unk_0x11_4 : 1;
    /* 0x0011.5 */ u8 unk_0x11_5 : 1;
    /* 0x0011.6 */ u8 unk_0x11_6 : 1;
    /* 0x0011.7 */ u8 unk_0x11_7 : 1;
    /* 0x0012.0 */ u8 unk_0x12_0 : 1;
    /* 0x0012.1 */ u8 unk_0x12_1 : 1;
    /* 0x0012.2 */ u8 talkToLyleForTheFirstTime : 1;
    /* 0x0012.3 */ u8 unk_0x12_3 : 1;
    /* 0x0012.4 */ u8 unk_0x12_4 : 1; // Gets set when you talk to tom nook after your house was built
    /* 0x0012.5 */ u8 unk_0x12_5 : 1;
    /* 0x0012.6 */ u8 unk_0x12_6 : 1;
    /* 0x0012.7 */ u8 unlockedKappn : 1; // unlocked kappn boat
    /* 0x0013.0 */ u8 lyleIntroduction : 1;
    /* 0x0013.1 */ u8 unk_0x13_1 : 1;
    /* 0x0013.2 */ u8 receivedHhsIntro : 1;
    /* 0x0013.3 */ u8 unk_0x13_3 : 1; // after repaying loan in post office
    /* 0x0013.4 */ u8 unk_0x13_4 : 1;
    /* 0x0013.5 */ u8 unk_0x13_5 : 1; // Talked to tortimer
    /* 0x0013.6 */ u8 unk_0x13_6 : 1;
    /* 0x0013.7 */ u8 unk_0x13_7 : 1;
    /* 0x0014.0 */ u8 unk_0x14_0 : 1;
    /* 0x0014.1 */ u8 unk_0x14_1 : 1;
    /* 0x0014.2 */ u8 finishedShrunkSignatures : 1;
    /* 0x0014.3 */ u8 unk_0x14_3 : 1;
    /* 0x0014.4 */ u8 unk_0x14_4 : 1;
    /* 0x0014.5 */ u8 unk_0x14_5 : 1;
    /* 0x0014.6 */ u8 unk_0x14_6 : 1;
    /* 0x0014.7 */ u8 unk_0x14_7 : 1;
    /* 0x0015.0 */ u8 unk_0x15_0 : 1;
    /* 0x0015.1 */ u8 unk_0x15_1 : 1;
    /* 0x0015.2 */ u8 unk_0x15_2 : 1;
    /* 0x0015.3 */ u8 unk_0x15_3 : 1;
    /* 0x0015.4 */ u8 isabelleWallpaperGiftForGettingHouse : 1;
    /* 0x0015.5 */ u8 kickIntroduced : 1;
    /* 0x0015.6 */ u8 unk_0x15_6 : 1;
    /* 0x0015.7 */ u8 unk_0x15_7 : 1;
    /* 0x0016.0 */ u8 unk_0x16_0 : 1;
    /* 0x0016.1 */ u8 unk_0x16_1 : 1;
    /* 0x0016.2 */ u8 unk_0x16_2 : 1;
    /* 0x0016.3 */ u8 unk_0x16_3 : 1;
    /* 0x0016.4 */ u8 unk_0x16_4 : 1;
    /* 0x0016.5 */ u8 unk_0x16_5 : 1;
    /* 0x0016.6 */ u8 unk_0x16_6 : 1;
    /* 0x0016.7 */ u8 unk_0x16_7 : 1;
    /* 0x0017.0 */ u8 unk_0x17_0 : 1;
    /* 0x0017.1 */ u8 unk_0x17_1 : 1;
    /* 0x0017.2 */ u8 unk_0x17_2 : 1;
    /* 0x0017.3 */ u8 unk_0x17_3 : 1;
    /* 0x0017.4 */ u8 unk_0x17_4 : 1;
    /* 0x0017.5 */ u8 unk_0x17_5 : 1;
    /* 0x0017.6 */ u8 unk_0x17_6 : 1;
    /* 0x0017.7 */ u8 unk_0x17_7 : 1; // Gets set and unset at a new day
    /* 0x0018.0 */ u8 unk_0x18_0 : 1; // Gets set and unset at a new day
    /* 0x0018.1 */ u8 unk_0x18_1 : 1; // Gets set and unset at a new day
    /* 0x0018.2 */ u8 unk_0x18_2 : 1;
    /* 0x0018.3 */ u8 unk_0x18_3 : 1;
    /* 0x0018.4 */ u8 unk_0x18_4 : 1;
    /* 0x0018.5 */ u8 unk_0x18_5 : 1;
    /* 0x0018.6 */ u8 unk_0x18_6 : 1;
    /* 0x0018.7 */ u8 unk_0x18_7 : 1;
    /* 0x0019.0 */ u8 unk_0x19_0 : 1;
    /* 0x0019.1 */ u8 unk_0x19_1 : 1;
    /* 0x0019.2 */ u8 unk_0x19_2 : 1;
    /* 0x0019.3 */ u8 unk_0x19_3 : 1;
    /* 0x0019.4 */ u8 unk_0x19_4 : 1;
    /* 0x0019.5 */ u8 unk_0x19_5 : 1;
    /* 0x0019.6 */ u8 unk_0x19_6 : 1;
    /* 0x0019.7 */ u8 knowsPermitRequirements : 1; // Isabelle explained how to obtain mayor permit
    /* 0x001A.0 */ u8 tomNookToldYouFirstPayment : 1;
    /* 0x001A.1 */ u8 permitPoints1 : 1; // written to bulletin board? (1 points)
    /* 0x001A.2 */ u8 permitPoints2 : 1; // mayor permit (2 points)
    /* 0x001A.3 */ u8 permitPoints3 : 1; // written to bulletin board? (4 points)
    /* 0x001A.4 */ u8 permitPoints4 : 1; // mayor permit (8 points)
    /* 0x001A.5 */ u8 permitPoints5 : 1; // mayor permit (16 points)
    /* 0x001A.6 */ u8 permitPoints6 : 1; // mayor permit (32 points)
    /* 0x001A.7 */ u8 permitPoints7 : 1; // mayor permit (64 points)
    /* 0x001B.0 */ u8 permitPoints8 : 1; // mayor permit? (128 points?)
    /* 0x001B.1 */ u8 permitPoints9 : 1; // mayor permit? (256 points?)
    /* 0x001B.2 */ u8 permitPoints10 : 1; // mayor permit? (512 points?)
    /* 0x001B.3 */ u8 unk_0x1B_3 : 1;
    /* 0x001B.4 */ u8 unk_0x1B_4 : 1;
    /* 0x001B.5 */ u8 unk_0x1B_5 : 1;
    /* 0x001B.6 */ u8 unk_0x1B_6 : 1;
    /* 0x001B.7 */ u8 unk_0x1B_7 : 1;
    /* 0x001C.0 */ u8 unk_0x1C_0 : 1;
    /* 0x001C.1 */ u8 unk_0x1C_1 : 1;
    /* 0x001C.2 */ u8 unk_0x1C_2 : 1;
    /* 0x001C.3 */ u8 unk_0x1C_3 : 1;
    /* 0x001C.4 */ u8 unk_0x1C_4 : 1;
    /* 0x001C.5 */ u8 unk_0x1C_5 : 1;
    /* 0x001C.6 */ u8 unk_0x1C_6 : 1;
    /* 0x001C.7 */ u8 unk_0x1C_7 : 1;
    /* 0x001D.0 */ u8 unk_0x1D_0 : 1;
    /* 0x001D.1 */ u8 unk_0x1D_1 : 1;
    /* 0x001D.2 */ u8 unk_0x1D_2 : 1;
    /* 0x001D.3 */ u8 unk_0x1D_3 : 1;
    /* 0x001D.4 */ u8 unk_0x1D_4 : 1;
    /* 0x001D.5 */ u8 unk_0x1D_5 : 1;
    /* 0x001D.6 */ u8 unk_0x1D_6 : 1;
    /* 0x001D.7 */ u8 unk_0x1D_7 : 1;
    /* 0x001E.0 */ u8 unk_0x1E_0 : 1;
    /* 0x001E.1 */ u8 unk_0x1E_1 : 1; // Gets set and unset at a new day
    /* 0x001E.2 */ u8 unk_0x1E_2 : 1; // Gets set and unset at a new day
    /* 0x001E.3 */ u8 unk_0x1E_3 : 1; // Gets set and unset at a new day
    /* 0x001E.4 */ u8 unk_0x1E_4 : 1; // Gets set and unset at a new day
    /* 0x001E.5 */ u8 unk_0x1E_5 : 1; // Gets set and unset at a new day
    /* 0x001E.6 */ u8 unk_0x1E_6 : 1; // Gets set and unset at a new day
    /* 0x001E.7 */ u8 unk_0x1E_7 : 1; // Gets set and unset at a new day
    /* 0x001F.0 */ u8 unk_0x1F_0 : 1; // Gets set and unset at a new day
    /* 0x001F.1 */ u8 unk_0x1F_1 : 1;
    /* 0x001F.2 */ u8 hasClubTortimerMembership : 1;
    /* 0x001F.3 */ u8 clubTortimerFirstAsked : 1; // Kappn asks user for the first time
    /* 0x001F.4 */ u8 clubTortimerRulesExplained : 1;
    /* 0x001F.5 */ u8 unk_0x1F_5 : 1;
    /* 0x001F.6 */ u8 unk_0x1F_6 : 1;
    /* 0x001F.7 */ u8 unk_0x1F_7 : 1;
    /* 0x0020.0 */ u8 unk_0x20_0 : 1;
    /* 0x0020.1 */ u8 unk_0x20_1 : 1; // lyle talked to you at your house for becoming VIP
    /* 0x0020.2 */ u8 unk_0x20_2 : 1; // maybe after first day??
    /* 0x0020.3 */ u8 unk_0x20_3 : 1;
    /* 0x0020.4 */ u8 unk_0x20_4 : 1;
    /* 0x0020.5 */ u8 unk_0x20_5 : 1;
    /* 0x0020.6 */ u8 unk_0x20_6 : 1;
    /* 0x0020.7 */ u8 unk_0x20_7 : 1;
    /* 0x0021.0 */ u8 unk_0x21_0 : 1;
    /* 0x0021.1 */ u8 unk_0x21_1 : 1;
    /* 0x0021.2 */ u8 unk_0x21_2 : 1; // maybe donated fossil?
    /* 0x0021.3 */ u8 unk_0x21_3 : 1;
    /* 0x0021.4 */ u8 unk_0x21_4 : 1;
    /* 0x0021.5 */ u8 unk_0x21_5 : 1;
    /* 0x0021.6 */ u8 unk_0x21_6 : 1;
    /* 0x0021.7 */ u8 unk_0x21_7 : 1;
    /* 0x0022.0 */ u8 unk_0x22_0 : 1; // Island People Here Tutorial
    /* 0x0022.1 */ u8 unk_0x22_1 : 1;
    /* 0x0022.2 */ u8 unk_0x22_2 : 1;
    /* 0x0022.3 */ u8 unk_0x22_3 : 1;
    /* 0x0022.4 */ u8 unk_0x22_4 : 1;
    /* 0x0022.5 */ u8 unk_0x22_5 : 1;
    /* 0x0022.6 */ u8 unk_0x22_6 : 1;
    /* 0x0022.7 */ u8 unk_0x22_7 : 1;
    /* 0x0023.0 */ u8 unk_0x23_0 : 1; // Tommy/Timmy first time selling item
    /* 0x0023.1 */ u8 unk_0x23_1 : 1;
    /* 0x0023.2 */ u8 unk_0x23_2 : 1;
    /* 0x0023.3 */ u8 unk_0x23_3 : 1; // Isabelle second advice? (Letters)
    /* 0x0023.4 */ u8 unk_0x23_4 : 1; // Isabelle second advice? (Letters)
    /* 0x0023.5 */ u8 unk_0x23_5 : 1; // Isabelle third advice (Beach)
    /* 0x0023.6 */ u8 unk_0x23_6 : 1; // Isabelle third advice (Beach)
    /* 0x0023.7 */ u8 unk_0x23_7 : 1; // Isabelle fourth advice (Timmy and Tommy)
    /* 0x0024.0 */ u8 unk_0x24_0 : 1; // Isabelle fifth advice (bury with shovel)
    /* 0x0024.1 */ u8 unk_0x24_1 : 1; // Isabelle fifth advice (bury with shovel)
    /* 0x0024.2 */ u8 unk_0x24_2 : 1; // Isabelle fifth advice (bury with shovel)
    /* 0x0024.3 */ u8 unk_0x24_3 : 1;
    /* 0x0024.4 */ u8 unk_0x24_4 : 1; // Isabelle finished fishing/catching advice (got watering can)
    /* 0x0024.5 */ u8 unk_0x24_5 : 1; // Isabelle finished fishing/catching advice (got watering can)
    /* 0x0024.6 */ u8 unk_0x24_6 : 1; // Isabelle finished fishing/catching advice (got watering can)
    /* 0x0024.7 */ u8 unk_0x24_7 : 1; // Isabelle fifth advice (bury with shovel)
    /* 0x0025.0 */ u8 unk_0x25_0 : 1;
    /* 0x0025.1 */ u8 unk_0x25_1 : 1; // Isabelle fourth advice? (Timmy and Tommy)
    /* 0x0025.2 */ u8 unk_0x25_2 : 1;
    /* 0x0025.3 */ u8 unk_0x25_3 : 1; // Isabelle ask first advice (Talk to new villager)
    /* 0x0025.4 */ u8 unk_0x25_4 : 1;
    /* 0x0025.5 */ u8 unk_0x25_5 : 1;
    /* 0x0025.6 */ u8 unk_0x25_6 : 1;
    /* 0x0025.7 */ u8 unk_0x25_7 : 1;
    /* 0x0026.0 */ u8 unk_0x26_0 : 1;
    /* 0x0026.1 */ u8 unk_0x26_1 : 1;
    /* 0x0026.2 */ u8 unk_0x26_2 : 1;
    /* 0x0026.3 */ u8 unk_0x26_3 : 1;
    /* 0x0026.4 */ u8 unk_0x26_4 : 1;
    /* 0x0026.5 */ u8 unk_0x26_5 : 1;
    /* 0x0026.6 */ u8 unk_0x26_6 : 1;
    /* 0x0026.7 */ u8 unk_0x26_7 : 1;
    /* 0x0027.0 */ u8 unk_0x27_0 : 1;
    /* 0x0027.1 */ u8 unk_0x27_1 : 1; // Nintendo 3DS Image Share used for the first time
    /* 0x0027.2 */ u8 secretStorageExplained : 1; // Tom nook tells you what the secret storage is
    /* 0x0027.3 */ u8 unk_0x27_3 : 1; // Found magic lamp for the first time
    /* 0x0027.4 */ u8 unk_0x27_4 : 1; // Talked to wisp for the first time
    /* 0x0027.5 */ u8 unk_0x27_5 : 1; // isabelle wants to hold ceremony for becoming major | received tpc?
    /* 0x0027.6 */ u8 buildingSecretStorage : 1; // Next day secret storage will be built (1 = will be built -> afterwards 0 when built)
    /* 0x0027.7 */ u8 unlockedSecretStorage : 1;
    /* 0x0028.0 */ u8 unk_0x28_0 : 1;
    /* 0x0028.1 */ u8 unk_0x28_1 : 1; // Talking to harvey for the first time
    /* 0x0028.2 */ u8 unk_0x28_2 : 1;
    /* 0x0028.3 */ u8 unk_0x28_3 : 1;
    /* 0x0028.4 */ u8 unk_0x28_4 : 1;
    /* 0x0028.5 */ u8 unk_0x28_5 : 1;
    /* 0x0028.6 */ u8 unk_0x28_6 : 1;
    /* 0x0028.7 */ u8 unk_0x28_7 : 1;
    /* 0x0029.0 */ u8 unk_0x29_0 : 1;
    /* 0x0029.1 */ u8 unk_0x29_1 : 1;
    /* 0x0029.2 */ u8 unk_0x29_2 : 1;
    /* 0x0029.3 */ u8 unk_0x29_3 : 1;
    /* 0x0029.4 */ u8 unk_0x29_4 : 1;
    /* 0x0029.5 */ u8 unk_0x29_5 : 1;
    /* 0x0029.6 */ u8 unk_0x29_6 : 1;
    /* 0x0029.7 */ u8 unk_0x29_7 : 1;
    /* 0x002A.0 */ u8 unk_0x2A_0 : 1;
    /* 0x002A.1 */ u8 unk_0x2A_1 : 1;
    /* 0x002A.2 */ u8 unk_0x2A_2 : 1;
    /* 0x002A.3 */ u8 unk_0x2A_3 : 1;
    /* 0x002A.4 */ u8 unk_0x2A_4 : 1;
    /* 0x002A.5 */ u8 unk_0x2A_5 : 1;
    /* 0x002A.6 */ u8 unk_0x2A_6 : 1;
    /* 0x002A.7 */ u8 unk_0x2A_7 : 1;
    /* 0x002B.0 */ u8 unk_0x2B_0 : 1;
    /* 0x002B.1 */ u8 unk_0x2B_1 : 1;
    /* 0x002B.2 */ u8 unk_0x2B_2 : 1;
    /* 0x002B.3 */ u8 unk_0x2B_3 : 1; // Related to scanning amiibo at wisp lamp
    /* 0x002B.4 */ u8 unk_0x2B_4 : 1;
    /* 0x002B.5 */ u8 unk_0x2B_5 : 1; // Related to finishing first day
    /* 0x002B.6 */ u8 unk_0x2B_6 : 1; // after talking to cat in train at beginning of game
    /* 0x002B.7 */ u8 unk_0x2B_7 : 1;
    /* 0x002C.0 */ u8 unk_0x2C_0 : 1;
    /* 0x002C.1 */ u8 unk_0x2C_1 : 1;
    /* 0x002C.2 */ u8 unk_0x2C_2 : 1;
    /* 0x002C.3 */ u8 unlockedHouseEditor : 1;
    /* 0x002C.4 */ u8 unk_0x2C_4 : 1;
    /* 0x002C.5 */ u8 unk_0x2C_5 : 1; // talking to harvey for the first time
    /* 0x002C.6 */ u8 unk_0x2C_6 : 1;
    /* 0x002C.7 */ u8 receivedCatIntro : 1; // at the townhall station
    /* 0x002D.0 */ u8 unk_0x2D_0 : 1;
    /* 0x002D.1 */ u8 unk_0x2D_1 : 1;
    /* 0x002D.2 */ u8 unk_0x2D_2 : 1;
    /* 0x002D.3 */ u8 unk_0x2D_3 : 1;
    /* 0x002D.4 */ u8 unk_0x2D_4 : 1;
    /* 0x002D.5 */ u8 unk_0x2D_5 : 1;
    /* 0x002D.6 */ u8 unk_0x2D_6 : 1;
    /* 0x002D.7 */ u8 unk_0x2D_7 : 1;
    /* 0x002E.0 */ u8 unk_0x2E_0 : 1;
    /* 0x002E.1 */ u8 unk_0x2E_1 : 1;
    /* 0x002E.2 */ u8 unk_0x2E_2 : 1;
    /* 0x002E.3 */ u8 unk_0x2E_3 : 1;
    /* 0x002E.4 */ u8 unk_0x2E_4 : 1;
    /* 0x002E.5 */ u8 unk_0x2E_5 : 1;
    /* 0x002E.6 */ u8 unk_0x2E_6 : 1;
    /* 0x002E.7 */ u8 unk_0x2E_7 : 1;
    /* 0x002F.0 */ u8 unk_0x2F_0 : 1;
    /* 0x002F.1 */ u8 unk_0x2F_1 : 1;
    /* 0x002F.2 */ u8 unk_0x2F_2 : 1;
    /* 0x002F.3 */ u8 unk_0x2F_3 : 1;
    /* 0x002F.4 */ u8 unk_0x2F_4 : 1;
    /* 0x002F.5 */ u8 unk_0x2F_5 : 1;
    /* 0x002F.6 */ u8 canUseCensusMenu : 1;
    /* 0x002F.7 */ u8 unk_0x2F_7 : 1;
    /* 0x0030.0 */ u8 unk_0x30_0 : 1; // finished cat entry (First intro one?)
    /* 0x0030.1 */ u8 unk_0x30_1 : 1; // finished cat entry (Second Intro one)
    /* 0x0030.2 */ u8 unk_0x30_2 : 1; // finished all intro cat entries?
    /* 0x0030.3 */ u8 unk_0x30_3 : 1;
    /* 0x0030.4 */ u8 unk_0x30_4 : 1; // TPC explained (Intro)
    /* 0x0030.5 */ u8 unk_0x30_5 : 1;
    /* 0x0030.6 */ u8 unk_0x30_6 : 1;
    /* 0x0030.7 */ u8 unk_0x30_7 : 1;
    /* 0x0031.0 */ u8 unk_0x31_0 : 1;
    /* 0x0031.1 */ u8 unk_0x31_1 : 1;
    /* 0x0031.2 */ u8 unk_0x31_2 : 1;
    /* 0x0031.3 */ u8 unk_0x31_3 : 1;
    /* 0x0031.4 */ u8 unk_0x31_4 : 1;
    /* 0x0031.5 */ u8 unk_0x31_5 : 1; // Related to finishing first day
    /* 0x0031.6 */ u8 unk_0x31_6 : 1;
    /* 0x0031.7 */ u8 unk_0x31_7 : 1;
    /* 0x0032.0 */ u8 unk_0x32_0 : 1;
    /* 0x0032.1 */ u8 unk_0x32_1 : 1; // CAT explained in TPC (Intro)
    /* 0x0032.2 */ u8 unk_0x32_2 : 1;
    /* 0x0032.3 */ u8 unk_0x32_3 : 1;
    /* 0x0032.4 */ u8 unk_0x32_4 : 1;
    /* 0x0032.5 */ u8 unk_0x32_5 : 1;
    /* 0x0032.6 */ u8 unk_0x32_6 : 1;
    /* 0x0032.7 */ u8 unk_0x32_7 : 1;
    /* 0x0033.0 */ u8 unk_0x33_0 : 1;
    /* 0x0033.1 */ u8 unk_0x33_1 : 1;
    /* 0x0033.2 */ u8 unk_0x33_2 : 1;
    /* 0x0033.3 */ u8 unk_0x33_3 : 1;
    /* 0x0033.4 */ u8 unk_0x33_4 : 1;
    /* 0x0033.5 */ u8 unk_0x33_5 : 1;
    /* 0x0033.6 */ u8 unk_0x33_6 : 1;
    /* 0x0033.7 */ u8 unk_0x33_7 : 1;
};
ASSERT_SIZE(SvPlayerFlags, 0x34);

#pragma pack(pop)
