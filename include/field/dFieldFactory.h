#pragma once

#include "decomp.h"
#include "stage/stage_Api.h"

class Field;
namespace sead {
class Heap;
class Random;
} // namespace sead

// Makes the field of a stage: its blocks with grounds, heights and items, for most stages through a FieldBuilder of
// dFieldFactory.cpp (no RTTI; the class name is ours after that source file, the original name of the file comes from
// the RTTI of its builders). The object of the game at 0x009536FC is passed as this; the functions do not use it.
// Besides the fields, the file has the helpers of the acre maps of the island builders: 7 x 6 bytes, one acre kind
// (Func6060E8) for each block, row by row.
class FieldFactory
{
public:
    // the width and height of an acre map in blocks
    static const s32 ACRE_MAP_WIDTH = 7;
    static const s32 ACRE_MAP_HEIGHT = 6;
    // the cells of a source map that ReplacePlaceholders fills from the list
    static const u8 ACRE_KIND_PLACEHOLDER = 120;
    // the last acre id (the acres 0..360 have a kind)
    static const u32 ACRE_MAX = 360;

    // (empty; the static initializer of the object at 0x009536FC registers it, armlink removed the call)
    ~FieldFactory(); // 0x00203788

    // the field of the stage on the stage heap (NULL for the player houses, without a field or on failure)
    Field* Create(stage::Name name); // 0x002012C8

    // a field of width x height blocks on heap: only the blocks and heights / with grass / with the item layers
    // 0..maxLayer / with both (NULL on failure; the field is finalized, not freed)
    static Field* CreateBase(sead::Heap* heap, s32 width, s32 height); // 0x0020255C
    static Field* CreateWithGrass(sead::Heap* heap, s32 width, s32 height); // 0x00202B78
    DECOMP_NOINLINE static Field* CreateWithFg(sead::Heap* heap, s32 width, s32 height, u32 maxLayer); // 0x00202CA0
    static Field* CreateWithFgAndGrass(sead::Heap* heap, s32 width, s32 height, u32 maxLayer); // 0x0020372C

    // a random acre of the kind (one of the first 7 acres of that kind)
    static u16 GetRandomAcre(u8 kind, sead::Random* random); // 0x002025B0

    // map takes over source; the placeholder cells get the kinds of the list one after the other
    static void ReplacePlaceholders(u8* map, const u16* kinds, const u8* source); // 0x00202634
    // the right neighbor of each cell of kind 33 becomes 61, the left neighbor of each cell of kind 32 becomes 71
    static void SetNeighbors(u8* map); // 0x00202790
    // one random cell of the kind becomes newKind
    static void ReplaceRandomCell(u8* map, u8 kind, u8 newKind, sead::Random* random); // 0x0020292C
    // one random cell of the kinds 8..19 becomes the kind + 12 (8: 20)
    static void ReplaceRandomCell8To19(u8* map, sead::Random* random); // 0x00202CFC
    // one random cell of the kinds 43 / 38 (and 34, 35, 39 / 44, 42 in the next two) becomes the kind of the
    // conversion 34 -> 52, 35 -> 55, 38 -> 56, 39 -> 57, 42 -> 58, 43 -> 53, 44 -> 54
    static void ReplaceRandomCell43Or38(u8* map, sead::Random* random); // 0x00202FB8
    static void ReplaceRandomCell34Or35Or39(u8* map, sead::Random* random); // 0x00203234
    static void ReplaceRandomCell44Or42(u8* map, sead::Random* random); // 0x002034B0
};
