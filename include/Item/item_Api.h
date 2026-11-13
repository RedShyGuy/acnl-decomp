#pragma once

#include "decomp.h"

class SvFgName;
namespace script {
class WordPtr;
} // namespace script

namespace item {
// the kind of an item (names and values after libgarden's item::Kind)
enum Kind : u8
{
    KIND_DEFAULT = 0,
    KIND_MII = 0,
    KIND_CLEAR_FTR = 1,
    KIND_MONEY = 2,
    KIND_DUMMY_WALL = 3,
    KIND_CARPET = 4,
    KIND_DUMMY_FTR = 5,
    KIND_TOPS = 6,
    KIND_ONE_PIECE_OUTFIT = 7,
    KIND_TOPS_WET_SUIT = 8,
    KIND_BOTTOMS = 9,
    KIND_SOCKS = 10,
    KIND_SHOES = 11,
    KIND_CAP = 12,
    KIND_ACCE = 13,
    KIND_AXE = 14,
    KIND_NET = 15,
    KIND_ROD = 16,
    KIND_SHOVEL = 17,
    KIND_SLINGSHOT = 18,
    KIND_WATERING_CAN = 19,
    KIND_HAMMER = 20,
    KIND_MEGAPHONE = 21,
    KIND_TIMER = 22,
    KIND_FIREWORK = 23,
    KIND_PUT_HANABI = 24,
    KIND_UMBRELLA = 25,
    KIND_CRACKER = 26,
    KIND_SOAP_BUBBLE = 27,
    KIND_BALLOON = 28,
    KIND_WINDMILL = 29,
    KIND_BEANS = 30,
    KIND_RICE_ROLL = 31,
    KIND_SOFT_CREAM = 32,
    KIND_CANDY_CANE = 33,
    KIND_GLOW_STICK = 34,
    KIND_BLOW_ROLL_ITEM = 35,
    KIND_INSECT = 36,
    KIND_FEATHER = 37,
    KIND_SNOW_CRYSTAL = 38,
    KIND_FISH = 39,
    KIND_DIVE_FISH = 40,
    KIND_PAPER = 41,
    KIND_MELODY_PAPER = 42,
    KIND_BEFORE_FOSSIL = 43,
    KIND_FOSSIL = 44,
    KIND_REMAKE_FOSSIL = 45,
    KIND_HANIWA = 46,             // gyroids (the English name in the game)
    KIND_MUSIC = 47,
    KIND_MUSIC_MISS = 48,
    KIND_MUSIC_WALL = 49,
    KIND_ORGEL = 50,
    KIND_FRUIT = 51,
    KIND_GOOD_FRUIT = 52,
    KIND_BAD_FRUIT = 53,
    KIND_SEED = 54,
    KIND_SEEDLING = 55,
    KIND_SEEDLING_BUSH = 56,
    KIND_KABU = 57,
    KIND_BAD_KABU = 58,
    KIND_SOLD_OUT = 59,
    KIND_SHELL = 60,
    KIND_DUST = 61,
    KIND_MUSHROOM = 62,
    KIND_MUSH_FTR = 63,
    KIND_FLOWER = 64,
    KIND_PITFALL_SEED = 65,
    KIND_ORE = 66,
    KIND_RUDE_ORE = 67,
    KIND_FOODSTUFF = 68,
    KIND_HONEYCOMB = 69,
    KIND_MANURE = 70,
    KIND_MY_DESIGN_TEXTURE = 71,
    KIND_MY_DESIGN_OBJECT = 72,
    KIND_WRAPPING_PAPER = 73,
    KIND_MEDICINE = 74,
    KIND_SIGNATURE_SISYO = 75,
    KIND_SIGNATURE_QUEST = 76,
    KIND_SIGNATURE_QUEST_DUST = 77,
    KIND_LOST_QUEST = 78,
    KIND_LOST_QUEST_DUST = 79,
    KIND_SANTA_SACK = 80,
    KIND_SANTA_SACK_DUST = 81,
    KIND_CANDY = 82,
    KIND_LOLLIPOP = 83,
    KIND_PICTURE = 84,
    KIND_FAKE_PICTURE = 85,
    KIND_PICTURE_WALL = 86,
    KIND_FAKE_PICTURE_WALL = 87,
    KIND_SCULPTURE = 88,
    KIND_FAKE_SCULPTURE = 89,
    KIND_FORTUNE_COOKIE = 90,
    KIND_PAPER_FORTUNE = 91,
    KIND_BADGE = 92,
    KIND_HOUSE_POST = 93,
    KIND_HOUSE_CIRCLE_DOOR = 94,
    KIND_HOUSE_RECTANGLE_DOOR = 95,
    KIND_HOUSE_ROOF = 96,
    KIND_HOUSE_FENCE = 97,
    KIND_HOUSE_WALL = 98,
    KIND_HOUSE_MAT = 99,
    KIND_HOUSE_MODEL = 100,
    KIND_FISH_EGG = 101,
    KIND_DIVE_FISH_EGG = 102,
    KIND_BURY_EGG = 103,
    KIND_TREE_EGG = 104,
    KIND_ROCK_EGG = 105,
    KIND_BALLOON_EGG = 106,
    KIND_PRIZE_TICKET = 107,
    KIND_BIG_PRIZE_TICKET = 108,
    KIND_PRESENT_BOX = 109,
    KIND_MAP = 110,
    KIND_DUMMY_TROPHY = 111,
    KIND_PUT_HANABI_AFTER = 112,
    KIND_COFFEE_BEANS = 113,
    KIND_COIN_CHOCO = 114,
    KIND_COFFEE = 115,
    KIND_SPARKLING_SODA = 116,
    KIND_MT_FRUIT = 117,
    KIND_GOOD_MT_FRUIT = 118,
    KIND_CARDBOARD = 119,
    KIND_BINGO_CARD = 120,
    KIND_BINGO_CARD_DUST = 121,
    KIND_TIME_CAPSULE = 122,
    KIND_MANNEQUIN = 123,
    KIND_SOLD_OUT_RECYCLE = 124,
    KIND_SO_PA_CA = 125,
    KIND_DUMMY_LOCKER = 126,
    KIND_CAN_SIT_CHAIR_IN_ALL_STAGE = 127,
    KIND_TOUR_FLOWER_DUMMY = 128,
    KIND_TOWEL = 129,
    KIND_TOPS_DEFAULT = 130,
    KIND_BOTTOMS_DEFAULT = 131,
    KIND_BROMIDE = 132,
    KIND_SOLD_OUT_WALL = 133,
    KIND_NETACHO = 134,
    KIND_CHOCOLATE = 135,
    KIND_RENTAL_LIST = 136,
    KIND_HAND_LETTER = 137,
    KIND_PRESENT_BOX_QUEST = 138,
    KIND_SOLD_OUT_TSUNEKICHI = 139,
    KIND_DUMMY_WALL_2 = 140,
    KIND_FORTUNE_TSUNEKICHI = 141,
    KIND_PAPER_TSUNEKICHI = 142,
    KIND_DUMMY_FTR_2 = 143,
    KIND_RANDOM_TICKET = 144,
    KIND_MANNEQUIN_GRACE = 145,
    KIND_PLANT = 146,
    KIND_TOPS_WALL = 147,
    KIND_ONE_PIECE_WALL = 148,
    KIND_YUTARO_LAMP = 149,
    KIND_REMAKE_LARGE_FTR = 150,
    KIND_FRYING_PAN = 151,
    KIND_AMULET = 152,
    KIND_TROPHY = 153,
    KIND_SMP = 154,
    KIND_END = 155,
};

// the parameters of an item (layout and names after libgarden's item::ParamItemEntry)
struct ParamItemEntry
{
    u16 iconID;                 // 0x00
    u16 price;                  // 0x02
    u8 roomOrientation[6];      // 0x04
    u8 kind;                    // 0x0A Kind
    u8 modelID;                 // 0x0B
    u8 unk_0xC;                 // 0x0C
    u8 group;                   // 0x0D Group: the group of the item for the random item sets (name is ours)
    s8 priceMultiplier;         // 0x0E
    u8 unk_0xF[2];              // 0x0F
    u8 ftr;                     // 0x11
    u8 unk_0x12[7];             // 0x12
    u8 type;                    // 0x19
    u8 inventoryActionFlags;    // 0x1A
    u8 unk_0x1B[3];             // 0x1B
};
ASSERT_SIZE(ParamItemEntry, 0x1E);

// The groups of the items for the random item sets (ParamItemEntry::group; SetupFgNameSet and 0x002FECF8 take them).
// The game has no names for them: the enumerators are ours, after the items of the group in romfs Item/Param/Item.bin;
// only the groups that the decompiled code uses are listed so far.
enum Group : u8 {
    GROUP_SPECIAL_1 = 1,                         // no items: 0x002FECF8 treats it specially (with a category: all of it?)
    GROUP_FEATURED_FURNITURE = 9,                // covered wagon, moon, garden gnome, king, queen, ... (the featured item
                                                 // of the Nooklings)
    GROUP_STATIONERY = 25,                       // the mail papers
    GROUP_MELODY_PAPER = 26,
    GROUP_MEDICINE = 27,
    GROUP_WRAPPING_PAPER = 29,
    GROUP_PARTY_POPPER_AND_GLOW_WANDS = 30,
    GROUP_BEANS_AND_GOOD_LUCK_ROLL = 31,
    GROUP_CANDY = 32,
    GROUP_FORTUNE_COOKIE = 34,
    GROUP_JINGLE_SERIES = 35,
    GROUP_SPOOKY_SERIES = 36,
    GROUP_HARVEST_SERIES = 37,
    GROUP_HINANINGYO_AND_BLOSSOM_LANTERN = 38,
    GROUP_CARP_BANNER = 39,
    GROUP_NET_ROD_SHOVEL = 42,                   // net, fishing rod, shovel
    GROUP_SLINGSHOT_MEGAPHONE_TIMER = 43,        // slingshot, megaphone, timer
    GROUP_WALL_FURNITURE = 51,                   // wall shelves, wall clocks, wall lamps, ...
    GROUP_KK_SONGS_1 = 64,                       // K.K. March, K.K. Samba, K.K. Calypso, ...
    GROUP_KK_SONGS_2 = 65,                       // K.K. Tango, Lucky K.K., K.K. Steppe, ...
    GROUP_KK_SONGS_3 = 66,                       // K.K. Swing, K.K. Jazz, K.K. Fusion, ...
    GROUP_KK_SONGS_4 = 67,                       // K.K. Chorale, K.K. Waltz, K.K. Etude, ...
    GROUP_KK_SONGS_5 = 68,                       // K.K. Folk, K.K. Gumbo, The K. Funk, ...
    GROUP_FLOWERS = 74,                          // red, white, yellow tulips/pansies/cosmos/..., the first colors
    GROUP_FIREWORKS = 131,                       // sparkler, roman candle, fountain firework
    GROUP_KK_SONGS_6 = 155,                      // K.K. Groove, Bubblegum K.K., K.K. Disco
    GROUP_KK_SONGS_7 = 156,                      // K.K. Adventure, K.K. Bazaar, K.K. Jongara, K.K. Flamenco
    GROUP_KK_SONGS_8 = 157,                      // K.K. Moody, K.K. Synth
    GROUP_KK_SONGS_9 = 158,                      // K.K. Milonga
    GROUP_KK_SONGS_10 = 159,                     // Space K.K., K.K. Oasis
};

// the parameters of a kind of item (names are ours)
struct KindParamEntry
{
    u8 unk_0x0;
    u8 flags1;  // 0x1 bit 4: the item may be in the inventory (SvFgName::CanBeInInventory)
    u8 flags2;  // 0x2 bit 7: the item goes into the inventory as a letter (SvPlayerInventory::SetItem)
};
ASSERT_SIZE(KindParamEntry, 0x3);

// the parameters of a field object (names are ours; only the kind is known)
struct ParamFgEntry
{
    u8 unk_0x0[10];
    u8 fgKind;      // 0xA SvFgName::FgKind
};

const ParamItemEntry* GetParamItemEntry(const SvFgName& item); // 0x00536854 (name after libgarden's item header)
// the entry of the kind of the item (NULL if it has none)
const KindParamEntry* GetKindParamEntry(const SvFgName& item); // 0x005368C0 (name is ours)
// the entry of a field object (the names up to 0xFD)
const ParamFgEntry* GetParamFgEntry(const SvFgName& item); // 0x002FCFB4 (name is ours)
void GetName(script::WordPtr* out, const SvFgName& item, void* myDesign); // 0x0056E93C (name after libgarden's item header)
// whether the item is of KIND_MONEY
bool IsMoney(const SvFgName& item); // 0x002FE6CC (name is ours)
// the price of the item (ParamItemEntry::price times priceMultiplier, with special cases for fruit)
s32 GetPrice(const SvFgName& item); // 0x003055C4 (name is ours)
// the offset of the icon of an item of KIND_AXE (0 for other kinds)
f32 GetIconOffset(const SvFgName& item); // 0x006BFCDC (name is ours)
// the index of the item among the items of its kind (-1: no item; not decompiled yet)
s32 GetIndex(const SvFgName& item); // 0x002FF76C (name after libgarden's item header)
// the id of a creature in the list of the insects and fish (insects, fish, dive fish, and the kinds counted with them;
// 0: other kinds)
u32 ToInsectFishID(const SvFgName& item); // 0x006D9BA8 | libgarden
} // namespace item
