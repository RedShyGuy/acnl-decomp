#pragma once

// Museum exhibition rooms.
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "Sv/dSvHouse.h"
#include "Sv/dSvMyDesign.h"

#pragma pack(push, 1)

struct SvMuseumExhibit {
    // (game code, not decompiled yet; the constructor of the save data builds it)
    SvMuseumExhibit(); // 0x001B65E4

    /* 0x0000 */ s8 playerIndex; // ctor sets to 0xFF (-1: none)
    /* 0x0001 */ u8 pad_0x1;
    /* 0x0002 */ SvRoom museumRoom;
    /* 0x0304 */ SvMyDesign exhibitPoster;
    /* 0x0B74 */ char16 exhibitTitle[0x11];
    /* 0x0B96 */ u16 pad_0xB96;
};
ASSERT_SIZE(SvMuseumExhibit, 0xB98);

// the museum (one struct in the original: GetExhibit gets its address)
struct SvMuseum {
    // (inline in the constructor of the save data) the members, then Init
    SvMuseum() { Init(); }
    // (names are ours) the donations and the exhibition rooms are cleared (also when the town data is cleared)
    void Init(); // 0x006BD128
    // the exhibition room (the first one for an index of 4 or more)
    SvMuseumExhibit* GetExhibit(u32 index); // 0x006BCED8 (name is ours)
    // a player of a save number is removed: the donations of the player get the donor DONOR_FORMER, the exhibition
    // rooms of the player are cleared (SvGardenData::RemovePlayer)
    void RemovePlayer(u32 saveNo); // 0x006BCCBC
    // all donations and their dates are cleared
    void ClearDonations(); // 0x006BCF04
    // the four exhibition rooms are cleared (two copies; the second is called when a new town is set up)
    void ClearExhibits(); // 0x006BD014
    void ClearExhibits2(); // 0x006BD0F0
    // the donation of an item gets the donor DONOR_FORMER
    void SetFormerDonor(const SvFgName& item); // 0x006BD04C
    // the current player donates an item: donor = save number + 1, date = today; then the tests whether a group and the
    // whole museum are complete run (their results are not used: the code after them was removed)
    void Donate(const SvFgName& item); // 0x006BD168

    // (copies of the inline functions of the museum in the shared area; names are ours) the donor of an item (0: not
    // donated; 1..4: save number + 1, DONOR_FORMER) / the kind and index have a place in the museum / all fish and sea
    // creatures / all pictures and sculptures are donated
    DECOMP_NOINLINE u8 GetDonor(const SvFgName& item) const; // 0x0076E684
    DECOMP_NOINLINE static bool HasPlace(u8 kind, u32 index); // 0x0076E730
    DECOMP_NOINLINE bool IsFishComplete() const; // 0x0076D8D4
    DECOMP_NOINLINE bool IsArtComplete() const; // 0x0076E150

    // the donor of a donation that was made by a player who was removed from the town (name is ours)
    static const u8 DONOR_FORMER = 5;
    // the places of the groups in donationDates and donations: fossils 67, fish 72, sea creatures 30, insects 72, pictures
    // 25, sculptures 8 (names are ours)
    enum {
        FOSSIL_FIRST = 0,
        FISH_FIRST = 67,
        DIVE_FISH_FIRST = 139,
        INSECT_FIRST = 169,
        PICTURE_FIRST = 241,
        SCULPTURE_FIRST = 266,
    };

    /* 0x0000 */ SvDate donationDates[0x112];
    /* 0x0448 */ u8 donations[0x112];
    /* 0x055A */ u8 pad_0x55A[2];
    /* 0x055C */ SvMuseumExhibit exhibits[4];
};
ASSERT_SIZE(SvMuseum, 0x33BC);

// (game code, not decompiled yet; names are ours) an exhibition room is cleared / set to its defaults
void Func1B63DC(SvMuseumExhibit* exhibit); // 0x001B63DC
void Func1B6538(SvMuseumExhibit* exhibit); // 0x001B6538

#pragma pack(pop)
