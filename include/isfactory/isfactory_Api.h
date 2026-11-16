#pragma once

#include "decomp.h"
#include "field/dFieldTypes.h"

class Field;
class FieldBuilder;
class TourData;
namespace sead {
class Heap;
class Random;
} // namespace sead

// The builders of the fields of the tours of the island (namespace and class names from the RTTI; the rest is ours).
namespace isfactory {
// the types of the tours (GetTourType; names after the builders that CreateBuilder makes for them, the comments give the
// English names of the tours after Nookipedia "Island Tours" / Thonky; each type has three levels Easy/Official/Elite)
enum TourType : s8
{
    TOUR_TYPE_HIDE_AND_SEEK = 0,    // Hide-and-Seek Tour
    TOUR_TYPE_FLOWER_PICKING = 1,   // Gardening Tour
    TOUR_TYPE_FISHING = 2,          // the fishing tours
    TOUR_TYPE_INSECT_CATCHING = 3,  // the bug-catching tours
    TOUR_TYPE_BALLOON = 4,          // Balloon-Hunt Tour
    TOUR_TYPE_TREASURE_HUNT = 5,    // probably the Item-Matching Tour (each treasure is buried twice)
    TOUR_TYPE_PIKOHAN = 6,          // Hammer Tour (pikohan = toy hammer)
    TOUR_TYPE_MAZE = 7,             // Labyrinth Tour
    TOUR_TYPE_MINING = 8,           // Ore-Hunter Tour
    TOUR_TYPE_FOSSIL_DIG = 9,       // Fossil Finder's / Fossil Hunter's Tour (with huts, see ishutfact2)
    TOUR_TYPE_DIVING = 10,          // the free-diving tours
    TOUR_TYPE_BORROWING_RACE = 11,  // Scavenger Tour (with huts, see ishutfact2)
};

// the builder of the field of the tour of tourData on heap (NULL without heap or for other types)
FieldBuilder* CreateBuilder(sead::Heap* heap, Field* field, sead::Random* random, TourData* tourData); // 0x007034A8

// the keys of the fossil layout for the four even huts of the tour of type 9: the four keys of the level of the tour
// (level 0: 0..3, 1: 4..7, 2: 8..11) in a random order (keys has 12 bytes, all cleared first)
void GetHutFossilKeys(u8* keys, TourData* tourData, sead::Random* random); // 0x00707DBC

// one random cell of the kind in the blocks 2..4 x 2..3 of the acre map becomes newKind (false if there is none)
bool ReplaceRandomInnerCell(u8* map, u8 kind, u8 newKind, sead::Random* random); // 0x00706EB8

// the units at the west edge of the block column left, the east edge of the column right and the north edge of the
// block row 1 get other attributes (soil 0x14 -> 0x32, sand 0x16 -> 0x80; not in this file in the original)
void SetEdgeAttributes(Field* field, s32 left, s32 right); // 0x005279A4

// the number of the players (1 without multiplayer; game code, not decompiled yet)
u32 Func6E1510(); // 0x006E1510
} // namespace isfactory
