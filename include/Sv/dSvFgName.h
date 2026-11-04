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
    // Item ID. 0x7FFE is the empty item: the game uses (SvFgName::Name)32766 as
    // template argument of ExchangeBankKeywordFgNameEx and the save constructors fill
    // empty slots with it. The other values are not named yet.
    enum Name {
        NAME_EMPTY = 0x7FFE,
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
