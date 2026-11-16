#pragma once

#include "decomp.h"

class TourData;

// The island huts are the small houses on the tour island that the players can enter (stages Island_Hut0..9). Only two
// tours use them (Nookipedia "Scavenger Hunt", Thonky "Island Tours"):
// - the Fossil Finder's / Fossil Hunter's Tour (isfactory::TOUR_TYPE_FOSSIL_DIG): two houses per player, the left one
//   shows fossils as a template, the right one is empty; the players dig up the same fossils on the island and put
//   them into the empty house in the same arrangement (8 huts = 4 players x 2),
// - the Scavenger Tour (isfactory::TOUR_TYPE_BORROWING_RACE): five vacant houses with furniture; the players look for
//   the furniture of a list in them.
namespace ishutfact2 {
// the number of the huts of the tours of other types than 9 (the five houses of the Scavenger Tour; the original calls
// a function of the tour data that returns 5, armlink replaced the call by its result)
const u32 HUT_NUM = 5;
// the huts of the Fossil Finder's Tour (Island_Hut0..7: a template hut and an empty hut for each of the 4 players)
const u32 FOSSIL_HUT_NUM = 8;

// the fields of the island huts of the tour (name is ours): for the Fossil Finder's Tour the even huts get the template
// fossils and the odd ones stay empty, for the Scavenger Tour each hut gets the room of a villager (or the items of a
// layout at level 2 = Elite), the huts of the other tours stay empty
void BuildFields(TourData* tourData); // 0x002B8938
} // namespace ishutfact2
