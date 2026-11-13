#pragma once

// SvFgName - an item / field object ("Fg" = foreground of the field) as stored in the
// save data: a 16 bit item ID plus 16 bits of flags (4 bytes in total).
//
// Class name, the nested Name / ID types and the methods are from the binary.

#include "decomp.h"

namespace item {
enum Kind : u8;
} // namespace item
namespace script {
template <u32 N>
class WordFix;
} // namespace script

class SvFgName
{
public:
    // Item ID. The game has this enumeration (it uses (SvFgName::Name)32766, the empty item, as template argument of
    // ExchangeBankKeywordFgNameEx; the save constructors fill empty slots with it), the binary has no identifiers: the
    // enumerators are generated from the item list of the project (tools/build/gen_item_names.py).
    enum Name {
#include "Sv/dSvFgNameList.inc"
    };

    // an item ID as the game passes it to the constructor (bit 15 is dropped)
    enum ID : u16 {};

    // the usual items: GetIndex counts from USUAL_BEGIN (names are ours)
    static const u16 USUAL_BEGIN = 0x2000;
    static const u16 USUAL_COUNT = 0x172B;
    // the field objects: the names up to FG_LAST have a FgKind (name is ours)
    static const u16 FG_LAST = 0xFD;

    // Result of GetFgKind(). Enumerator names are descriptive (research), not from the binary.
    enum FgKind {
        FG_KIND_HOLE_OR_BROKEN = 0,   // hole, seed, broken flowers / holes
        FG_KIND_WILTED_PLANT,         // wilted bush, trees, cedar, palm, bamboo
        FG_KIND_STUMP_OR_CUT_TREE,    // cut trees / cedars / palms / bamboo regrowing + all stumps
        FG_KIND_TREE_SAPLING,         // normal / fruit / money tree, growth stage 1
        FG_KIND_TREE,                 // normal / fruit / money tree, growth stage 2-4 + fully grown
        FG_KIND_PALM_SAPLING,         // coconut / banana palm, growth stage 1
        FG_KIND_PALM_TREE,            // coconut / banana palm, growth stage 2-4 + fully grown
        FG_KIND_CEDAR_SAPLING,        // cedar, growth stage 1
        FG_KIND_CEDAR_TREE,           // cedar, growth stage 2-4 + fully grown
        FG_KIND_WEED,
        FG_KIND_ROCK,
        FG_KIND_TULIP,
        FG_KIND_PANSY,
        FG_KIND_COSMOS,
        FG_KIND_ROSE,
        FG_KIND_GOLD_ROSE,
        FG_KIND_CARNATION,
        FG_KIND_JACOBS_LADDER,
        FG_KIND_RAFFLESIA,            // incl. wilted rafflesia
        FG_KIND_WILTED_FLOWER,        // all wilted flowers except rafflesia
        FG_KIND_DANDELION,            // dandelions + dandelion puffs
        FG_KIND_CLOVER,               // lucky clovers + clover weeds
        FG_KIND_BUSH_SAPLING,         // all bushes, growth stage 1
        FG_KIND_BUSH,                 // all bushes, growth stage 2 + fully grown
        FG_KIND_PATTERN,
        FG_KIND_BAMBOO,               // bamboo growth stage 2-3 + fully grown
        FG_KIND_BAMBOO_SHOOT,         // bamboo growth stage 1
        FG_KIND_LILY,
        FG_KIND_VIOLET,
        FG_KIND_INVALID,
    };

    // the empty item (inline after libgarden's SvFgName header)
    constexpr SvFgName() : id(NAME_EMPTY), flags(0) {}
    explicit SvFgName(SvFgName::ID); // 0x002FCC14 | libgarden [tier A]
    SvFgName(const SvFgName& other); // 0x002FC9C4
    SvFgName& operator=(const SvFgName& other); // 0x002FC978

    bool IsEmpty() const; // 0x002FCB84 | libgarden [tier A]
    bool IsUsual() const; // 0x002FCBA0 | libgarden [tier A]
    // the kind of the item (KIND_END without parameters)
    item::Kind GetKind() const; // 0x002FCBC4 (name after libgarden's SvFgName header)
    // the name without bit 15 is the given one
    bool IsName(u16 name) const; // 0x002FCBE8 (name is ours)
    // bit 15 of the flags (name after libgarden's SvFgName::Flags)
    bool IsBuried() const; // 0x002FCC00
    // a field object: one of the names that have a FgKind
    bool IsFg() const; // 0x002FCCB8 (name is ours)
    u8 GetFgKind() const; // 0x002FCCD0 | libgarden [tier A]
    script::WordFix<33> GetName(void* myDesign) const; // 0x002FEA78 | libgarden [tier A]
    const char* GetIconName() const; // 0x005367DC | libgarden [tier A]
    s32 GetIndex() const; // 0x00769DBC | libgarden [tier A]
    // whether SvPlayerInventory::GetItem returns the item (names 0x4000..0x5FFF or the flag of its kind)
    bool CanBeInInventory() const; // 0x0076A85C (name is ours)
    // bit 0 of the flags (meaning unknown; SvPlayerInventory::SetItem counts money with it as earned bells)
    bool GetUnkFlag0() const; // 0x0076D690 (name is ours)
    void ClearUnkFlag0(); // 0x006BA464 (name is ours)
    // prepares the item for the inventory (clears the flag bits 14 and 15, converts field objects); false if it
    // cannot go there
    bool PrepareForInventory(); // 0x006BAC68 (name is ours)

    // The flags (after libgarden's SvFgName::Flags): bits 0..13 depend on the item ("unique"; for the fruit trees the
    // growth of the fruit 0..2, a perfect fruit 3, its decay 4..6 and the place of a perfect fruit 7..9), bits 14..15
    // are common to all items (the orientation inside, bit 15 buried outside).
    static const u16 FLAGS_COMMON = 0xC000;

    // the item from the name in the low and the flags in the high half (after libgarden's SvFgName)
    explicit SvFgName(u32 data); // 0x006BC564
    // the name of the item of a kind at an index (the empty name if the index is too large) (name is ours)
    static u16 GetNameOfKind(u8 kind, u32 index); // 0x006BA680
    // the item of a kind gets the name of another kind at the same index (true if it was of the kind) (name is ours)
    DECOMP_NOINLINE bool ChangeKind(u8 from, u8 to); // 0x006BA3A8
    // the flag bits of the item / the common flag bits are cleared / the item bits get a value (names are ours, after
    // libgarden's Flags::unique and Flags::common)
    void ClearUnique(); // 0x006BA454
    void ClearCommon(); // 0x006BA474
    void SetUnique(u16 value); // 0x006BA89C

    // (names are ours) the kinds PICTURE_WALL, FAKE_PICTURE_WALL, MUSIC_WALL, TOPS_WALL and ONE_PIECE_WALL become
    // PICTURE, FAKE_PICTURE, MUSIC, TOPS and ONE_PIECE_OUTFIT; true if the item changed
    bool ConvertFromWallKind(); // 0x006BA2E8
    // after a harvest: the money tree becomes a tree, the other fruit trees get the fruit growth 3 and lose the place
    // of a perfect fruit
    void ResetFruitAfterHarvest(); // 0x006BA484
    // a flower field object (tulip, pansy, cosmos, rose, carnation, lily, violet) from the genes of two flowers; true
    // if a is such a flower
    bool BreedFlower(const SvFgName& a, const SvFgName& b); // 0x006BA4D8
    // the item of a flower field object (the flowers 159.. of FgKind 11..17 and 20, and the lucky clover, become the
    // items of KIND_FLOWER with the same flags), the other field objects give the empty item, the others themselves
    DECOMP_NOINLINE SvFgName GetPickedItem() const; // 0x006BA584
    // bit 15 of the flags is cleared
    void ClearBuried(); // 0x006BA6AC
    // a turnip of KIND_KABU (not the first) becomes the turnip before it; true if it changed
    bool ToSmallerTurnipStack(); // 0x006BA6C8
    // bamboo: the bits 0..2 go down by count (2 if they would go below 0); true if they are 2 then
    bool Func6BA788(u32 count); // 0x006BA788
    // the mushroom furniture: the item of the same index in the set of item names 14
    SvFgName Func6BA7E8() const; // 0x006BA7E8
    // the item bits get the value with bit 11 / bit 10 / the value 0x200
    void Func6BA864(u16 value); // 0x006BA864
    void Func6BA880(u16 value); // 0x006BA880
    void Func6BA8B4(); // 0x006BA8B4
    // a FgKind of a sapling (tree, palm, cedar, bush, bamboo shoot) / of a bush
    static bool IsSaplingFgKind(u8 fgKind); // 0x006BA8C8
    static bool IsBushFgKind(u8 fgKind); // 0x006BA938
    // the orientation (bits 14..15) of an item (none if it is 4 or more)
    static void SetOrientation(SvFgName* item, u32 orientation); // 0x006BA8F4
    // the orientation is cleared when the item is furniture (Func749394: the furniture byte 0x11 of the parameters)
    void ClearOrientationIfFurniture(); // 0x006BA948
    // the fruit of a fruit tree grows (the growth goes down by count; at 0 a perfect fruit decays, or for the fruit of
    // the town a perfect fruit may appear, mode 0..2 choose the chances)
    void GrowFruit(u32 count, u32 mode); // 0x006BA978
    // with Func7694C0: the item bits become low + (high << 10); true if they changed
    bool Func6BAC1C(u32 high, u32 low); // 0x006BAC1C
    // (a second copy of ClearUnique)
    void Func6BAC58(); // 0x006BAC58
    // an index of the design of the item (KIND_TOPS: index + 8, KIND_ONE_PIECE_OUTFIT: index + 701, KIND_MY_DESIGN_TEXTURE
    // by its bits; -1: none)
    DECOMP_NOINLINE s32 Func6BADE4() const; // 0x006BADE4
    // a giant clam shell and a song become the music box of the song; true if they did
    bool MakeMusicBox(const SvFgName& shell, const SvFgName& song); // 0x006BAF54
    // the reverse of ConvertFromWallKind, and four clocks / TVs become the next name; true if the item changed
    bool ConvertToWall(); // 0x006BB018
    // ConvertFromWallKind, and the four clocks / TVs become the name before; true if the item changed
    DECOMP_NOINLINE bool Func6BB17C(); // 0x006BB17C
    // the item of KIND_REMAKE_FOSSIL whose parameter byte 0x12 is value; true if found
    bool Func6BB2C8(u32 value); // 0x006BB2C8
    // KIND_GOOD_FRUIT becomes KIND_BAD_FRUIT / the five fruits 0x2001..0x2005 become KIND_BAD_FRUIT / KIND_GOOD_FRUIT /
    // KIND_GOOD_FRUIT becomes KIND_FRUIT
    bool SpoilPerfectFruit(); // 0x006BB3D8
    bool SpoilFruit(); // 0x006BB62C
    bool ToPerfectFruit(); // 0x006BB8F0
    bool ToNormalFruit(); // 0x006BB840
    // the item is wrapped as a present: it moves to the names 0x4000.. (bits 13..14 = 2) unless it cannot be wrapped (bugs,
    // fish, sea creatures, turnips, wrapping paper, present boxes, mannequins, ...) / unwrapped (back to 0x2000..); true
    // if it changed
    bool Wrap(); // 0x006BB488
    bool Unwrap(); // 0x006BB5B4
    // the place of a perfect fruit (bits 7..9 = 1 << place) of the five fruit trees, unless the fruit is perfect
    void SetPerfectFruitPlace(u32 place); // 0x006BB5DC
    // the axes that break (the axe and the silver axe; not the rental or the golden axe): the bits 4..7 count up (unless a random number of 8 - bits 4..7 is 0, then bits 0..3
    // count up and bits 4..7 start again); true when bits 0..3 reach 8
    bool UseBreakableAxe(); // 0x006BB6F4
    // (a second copy of SetUnique)
    void Func6BB828(u16 value); // 0x006BB828
    // a flower item of KIND_FLOWER without genes gets the genes of its field object
    void InitFlowerGenes(); // 0x006BB9B8
    // a number 1..9 of a value 0..14
    static u32 Func6BBA4C(u32 value); // 0x006BBA4C
    // with Func7694C0: bits 10.. become value (the low ten bits stay)
    bool Func6BBB90(u32 value); // 0x006BBB90
    // the four clocks / TVs become the next name; true if the item changed
    bool Func6BBCF0(); // 0x006BBCF0
    // KIND_PAPER: the item bits become count - 1 (count up to 4); true if they changed
    bool SetPaperCount(u32 count); // 0x006BBD80
    // the gold and silver nugget and the four gems (other) give the bits 10.. of this item the value 8..13
    bool Func6BBDDC(const SvFgName& other); // 0x006BBDDC
    // with Func7694C0: the low bits become value (bits 10..13 stay) / the value of Func6BADE4 of other
    bool Func6BBEA8(u32 value); // 0x006BBEA8
    bool Func6BBF0C(const SvFgName& other); // 0x006BBF0C
    // KIND_MT_FRUIT / KIND_GOOD_MT_FRUIT: the item bits become count - 1 (count 2..9); count 1 calls MtFruitToFruit
    bool SetMtFruitCount(u32 count); // 0x006BBF88
    // KIND_MT_FRUIT / KIND_GOOD_MT_FRUIT become KIND_FRUIT / KIND_GOOD_FRUIT (always false)
    bool MtFruitToFruit(); // 0x006BC000
    // the wall versions of the custom designs (pattern wall canvas, the seven ..._WALL clothes) become the normal ones
    // (pattern easel, the clothes); true if the item changed
    bool ConvertCustomDesignFromWall(); // 0x006BC114
    // items with the parameter byte 0xF of 16: the item bits count up (the new value, 0 for other items)
    s32 Func6BC23C(); // 0x006BC23C
    // KIND_PAPER: the count (item bits + 1) goes down by one; the new count, 0 for other items
    s32 DecreasePaperCount(); // 0x006BC2BC
    // KIND_MT_FRUIT / KIND_GOOD_MT_FRUIT: the count (item bits + 1) goes down by one (always false)
    bool DecreaseMtFruitCount(); // 0x006BC35C
    // KIND_PICTURE, FAKE_PICTURE, SCULPTURE, FAKE_SCULPTURE: the item bits become value
    bool Func6BC410(u16 value); // 0x006BC410
    // Func304EA0 with the save number of the current player (0 if it is 8 or more)
    void Func6BC46C(u32 value); // 0x006BC46C
    // KIND_FORTUNE_COOKIE (value < 57) / KIND_FORTUNE_TSUNEKICHI (value < 9): the item bits become value
    bool SetFortuneCookieValue(u32 value); // 0x006BC4B4
    bool SetFortuneTsunekichiValue(u32 value); // 0x006BC50C
    // the money item (100 .. 99,000 Bells) for an amount: roundUp the smallest at least the amount, otherwise the largest
    // at most it; the rest goes to rest (the empty item if there is none)
    static SvFgName Func6BC580(u32 amount, bool roundUp, u32* rest); // 0x006BC580
    // the item of a kind at an index (with the default flags of its group)
    static SvFgName MakeOfKind(u8 kind, u32 index); // 0x006BC8A4
    // bit 14 of the flags is cleared
    void Func6BC70C(); // 0x006BC70C
    // a Mii mask (NAME_MII_MASK) of the Mii in the item bits
    void MakeMiiMask(u16 value); // 0x006BC728
    // bit 0 of the flags is set
    void SetUnkFlag0(); // 0x006BC764
    // the name 0x2000 becomes the empty item, KIND_SANTA_SACK becomes KIND_SANTA_SACK_DUST, the others call Func76C010
    void Func6BC820(u32 value); // 0x006BC820

    // (copies of inline functions of the item code; names are ours) the fruit trees (incl. palms and the money tree) /
    // the apple, orange, pear, peach and cherry trees / further tests and values of the parameters
    DECOMP_NOINLINE bool IsFruitTree() const; // 0x0076B388
    DECOMP_NOINLINE bool Func76BFA4() const; // 0x0076BFA4
    bool Func7694C0() const; // 0x007694C0
    s32 Func76CAE0() const; // 0x0076CAE0
    u32 Func76CB50() const; // 0x0076CB50
    SvFgName Func76C010(u32 value) const; // 0x0076C010

    // the empty item
    static SvFgName s_Empty; // (name after libgarden)

    /* 0x0 */ u16 id;    // SvFgName::Name
    /* 0x2 */ u16 flags;

private:
    // set the default flags of a new field object or item (names are ours)
    void InitFgFlags(); // 0x006BBBF0
    void InitItemFlags(); // 0x006BBAE4

    // the flag initialization per group of names (bits 13 and 14: 0 field objects, 1 items)
    typedef void (SvFgName::*InitFlagsFunction)();
    static const InitFlagsFunction s_InitFlagsFunctions[2]; // (name is ours)
};
ASSERT_SIZE(SvFgName, 0x4);
