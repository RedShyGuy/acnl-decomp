#pragma once

// Meow Coupon initiatives (Welcome amiibo).
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"

#pragma pack(push, 1)

struct SvInitiative {
    enum Name {
        None = 0x0,
        FashionForward = 0x1,
        CouponKickoff = 0x2,
        GardenGrooming = 0x3,
        ClearWater = 0x4,
        PublicWorks = 0x5,
        Rainmaker = 0x6,
        TPCRefresh = 0x7,
        LookFabulous = 0x8,
        IslandTime = 0x9,
        FossilRecord = 0xA,
        RetailTherapy = 0xB,
        ProDesign = 0xC,
        GameOn = 0xD,
        ReelEm = 0xE,
        BugBoss = 0xF,
        DiveDeep = 0x10,
        RideTheRails = 0x11,
        HostTheMost = 0x12,
        StockpileSweets = 0x13,
        InteriorDesign = 0x14,
        EquityBuilding = 0x15,
        ArtAppreciation = 0x16,
        TreeHugger = 0x17,
        CicadaResearch = 0x18,
        Salmon = 0x19,
        GiantSnakehead = 0x1A,
        NibbleFish = 0x1B,
        OutsideOptions = 0x1C,
        ShipIt = 0x1D,
        FortuneSeeker = 0x1E,
        StrikeItRich = 0x1F,
        LuckyItem = 0x20,
        Beekeeper = 0x21,
        InvestInYourself = 0x22,
        LetItSnowman = 0x23,
        MoleCricket = 0x24,
        BandedDragonfly = 0x25,
        PetaltailDragonfly = 0x26,
        Tarantula = 0x27,
        Scorpion = 0x28,
        WalkingLeaf = 0x29,
        TigerBeetle = 0x2A,
        OrchidMantis = 0x2B,
        BirdwingButterfly = 0x2C,
        DivingBeetle = 0x2D,
        Stringfish = 0x2E,
        Koi = 0x2F,
        SoftShelledTurtle = 0x30,
        KingSalmon = 0x31,
        MittenCrab = 0x32,
        Pike = 0x33,
        Dorado = 0x34,
        SaddledBichir = 0x35,
        Coelacanth = 0x36,
        Tuna = 0x37,
        Napoleonfish = 0x38,
        BlueMarlin = 0x39,
        OceanSunfish = 0x3A,
        Oarfish = 0x3B,
        MorayEel = 0x3C,
        SpiderCrab = 0x3D,
        HorseshoeCrab = 0x3E,
        HQHornedBeetle = 0x3F,
        HQStagBeetle = 0x40,
        Shark = 0x41,
        GiantIsopod = 0x42,
        InGoodGracie = 0x43,
        CurbAppeal = 0x44,
        SouvenirSales = 0x45,
        PayItForward = 0x46,
        ShellingOut = 0x47,
        BaristaBooster = 0x48,
        KeeeHaMoata = 0x49,
        DreamSharing = 0x4A,
        PaybackTime = 0x4B,
        TurnipProfit = 0x4C,
        CashForClutter = 0x4D,
        LocalFruit = 0x4E,
        BugBoost = 0x4F,
        FishItForward = 0x50,
        FossilBoss = 0x51,
        JokesOnYou = 0x52,
        TropicalShop = 0x53,
        PyrotechnicPro = 0x54,
        GoodFit = 0x55,
        LocalMusic = 0x56,
        SmallTalk = 0x57,
        IslandImport = 0x58,
        RockOn = 0x59,
        ResettiOutreach = 0x5A,
        Mushroom = 0x5B,
        SnagASnowflake = 0x5C,
        TropicalGarden = 0x5D,
        GreenThumb = 0x5E,
        FlowerPower = 0x5F,
        HappiestHomes = 0x60,
        SlingshotSniper = 0x61,
        AxeCollector = 0x62,
        FertileLand = 0x63,
        FashionForward2 = 0x64,
        SmallTalk2 = 0x65,
    };
};

// Monday is ID 1
struct SvInitiativeWeek {
    /* 0x0000 */ u8 mondayInitiative; // SvInitiative::Name
    /* 0x0001 */ u8 tuesdayInitiative; // SvInitiative::Name
    /* 0x0002 */ u8 wednesdayInitiative; // SvInitiative::Name
    /* 0x0003 */ u8 thursdayInitiative; // SvInitiative::Name
    /* 0x0004 */ u8 fridayInitiative; // SvInitiative::Name
    /* 0x0005 */ u8 saturdayInitiative; // SvInitiative::Name
    /* 0x0006 */ u8 sundayInitiative; // SvInitiative::Name
};
ASSERT_SIZE(SvInitiativeWeek, 0x7);

// Initiatives
struct SvPlayerInitiative {
    /* 0x0000 */ u64 unk_0x0; // ???; Set to 0, then 0x7FFFFFFFFFFFFFFF in player ctor
    /* 0x0008 */ u8 weeklyInitiatives[2]; // SvInitiative::Name / Set to 0 in player ctor
    /* 0x000A */ SvInitiativeWeek dailyInitiatives[2]; // Set to 0 in player ctor
    /* 0x0018 */ u8 unk_0x18; // ???; Set to 0x65 in player ctor
    /* 0x0019 */ u8 unk_0x19; // ???; Set to 0 in player ctor
    /* 0x001A */ u8 unk_0x1A; // ???; Set to 0 in player ctor
    /* 0x001B */ u8 pad_0x1B; // Padding: Not set in player ctor
    /* 0x001C */ u32 initiativeProgress[0x66]; // If current progress is the same or higher than initiative goal, then initiative is completed
    /* 0x01B4 */ u8 unk_0x1B4[0x66]; // ???; 0x66 buffer size set to 0 in player ctor
    /* 0x021A */ u8 pad_0x21A; // Padding: Not set in player ctor
    /* 0x021B */ u8 pad_0x21B; // Padding: Not set in player ctor
    /* 0x021C */ u32 unk_0x21C; // ???; Set to 0 in player ctor
    /* 0x0220 */ u32 unk_0x220; // ???; Set to 0 in player ctor
    /* 0x0224 */ u32 unk_0x224; // ???; Set to 0 in player ctor
    /* 0x0228 */ u32 unk_0x228; // ???; Set to 0 in player ctor
    /* 0x022C */ u32 unk_0x22C; // ???; Set to 0 in player ctor
    /* 0x0230 */ u32 unk_0x230; // ???; Set to 0 in player ctor
    /* 0x0234 */ u32 unk_0x234; // ???; Set to 0 in player ctor
    /* 0x0238 */ u32 unk_0x238; // ???; Set to 0 in player ctor
};
ASSERT_SIZE(SvPlayerInitiative, 0x23C);

// Goal of every initiative, starting with FashionForward (index 0 = None).
static const u32 kSvInitiativeGoals[102] = {
    0, 1, 1, 20, 1, 10000, 20, 1, 1, 1, 5, 50000,
    1, 3, 20, 20, 10, 1, 1, 3, 1, 1, 1, 3,
    3, 10, 1, 1, 1, 3, 3, 8, 1, 1, 5000, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5,
    1, 1, 1, 5000, 100, 1, 5, 1, 1, 1, 1, 150,
    1, 500, 1, 5, 10, 1, 1, 1, 1, 1, 1, 1,
    3, 1, 1, 1, 1, 5,
};

#pragma pack(pop)
