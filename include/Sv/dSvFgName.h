#pragma once

// SvFgName - an item / field object ("Fg" = foreground of the field) as stored in the
// save data: a 16 bit item ID plus 16 bits of flags (4 bytes in total).
//
// Class name, the nested Name / ID types and the methods are from the binary.

#include "decomp.h"

class SvFgName
{
public:
    // Item ID. 0x7FFE is the empty item: the game uses (SvFgName::Name)32766 as
    // template argument of ExchangeBankKeywordFgNameEx and the save constructors fill
    // empty slots with it. The other values are not named yet.
    enum Name {
        NAME_EMPTY = 0x7FFE,
    };

    struct ID { u32 _unknown; }; // TODO: real type unknown (placeholder)

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

    SvFgName(); // TODO: default ctor added so derived stubs compile - may not exist
    void IsEmpty() const; // 0x002FCB84 | libgarden [tier A]
    void IsUsual() const; // 0x002FCBA0 | libgarden [tier A]
    SvFgName(SvFgName::ID); // 0x002FCC14 | libgarden [tier A]
    void GetFgKind() const; // 0x002FCCD0 | libgarden [tier A]
    void GetName(void*) const; // 0x002FEA78 | libgarden [tier A]
    void GetIconName() const; // 0x005367DC | libgarden [tier A]
    void GetIndex() const; // 0x00769DBC | libgarden [tier A]

    /* 0x0 */ u16 id;    // SvFgName::Name
    /* 0x2 */ u16 flags;
};
ASSERT_SIZE(SvFgName, 0x4);
