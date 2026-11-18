#pragma once

#include "decomp.h"
#include "ssys/ma/dLoadSplit.h"

namespace sead {
class Heap;
} // namespace sead

// Loads the camping car info of a character, "NpcCampingCarInfo/camp_<model>.bin" (the items and the layout of its
// camping car; "wol04" when the character has none). Only the class name is from the RTTI; all other names are ours.
// RTTI 23NpcCampingCarInfoLoader @ 0x008CCF2C
// vtable 0x008F72CC (vptr 0x008F72D4), offset_to_top 0, 4 entries
class NpcCampingCarInfoLoader : public ::ssys::ma::LoadSplit
{
public:
    enum State : u8
    {
        STATE_NONE = 0,
        STATE_FOUND = 1,    // the file of the character exists (an empty path when Load is called again)
        STATE_FALLBACK = 2, // the file of the character is missing: "wol04"
        STATE_LOADING = 3,  // Read was called
    };

    DECOMP_EMIT_COPY DECOMP_NOINLINE NpcCampingCarInfoLoader() : m_InfoState(STATE_NONE) {} // 0x00335A80
    virtual ~NpcCampingCarInfoLoader(); // 0x00335AB0 slot 0x00
    // 0x00335AA0 slot 0x04 (deleting dtor)

    // stops the loading (true: nothing to wait for)
    bool Cancel(); // 0x003358DC
    // starts loading the info of the model model (in heap, the stage heap if NULL; true: started or nothing to do;
    // the last argument is not used)
    bool Load(const char* model, sead::Heap* heap, u32 unused); // 0x00335914
    // the same for the character npcId
    bool LoadForNpc(u16 npcId, sead::Heap* heap, u32 unused); // 0x00335A28

    // the loaded data
    DECOMP_EMIT_COPY const void* GetData() const { return m_pData; } // 0x00726628

    u8 m_InfoState; // 0xEB State (in the tail padding of LoadSplit)
};
ASSERT_OFFSET(NpcCampingCarInfoLoader, m_InfoState, 0xEB);
ASSERT_SIZE(NpcCampingCarInfoLoader, 0xEC);
