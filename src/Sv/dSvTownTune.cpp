#include "Sv/dSvTownTune.h"

#include <cstring>

#include "Sound/dSoundMelodyPlayerBase.h"
#include "Sound/dSoundMgr.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"

// the default melody that InitTownTune packs (name is ours)
// 0x00AD6394
SoundMelody g_SvDefaultTownTune;

namespace {
// (inline) a note of a melody (the first note for an index out of range)
DECOMP_ALWAYS_INLINE u8 GetNote(const SoundMelody* melody, u32 index)
{
    return index < 16 ? melody->notes[index] : melody->notes[0];
}

DECOMP_ALWAYS_INLINE void PackMelodyInline(u8* packed, const SoundMelody* melody)
{
    for (u32 i = 0; i < 8; i++) {
        *packed++ = (GetNote(melody, i * 2) & 0xF) | (GetNote(melody, i * 2 + 1) << 4);
    }
}
} // namespace

// 0x006BCB9C (name is ours)
void InitTownTune(u8* tune)
{
    if (g_SvIsSaveMemoryValid) {
        std::memcpy(tune, GetGardenPlus()->data.townData.townTune, sizeof(GetGardenPlus()->data.townData.townTune));
        return;
    }
    std::memcpy(&g_SvDefaultTownTune, SoundMgr::SGetRandomDefaultMelody(), sizeof(g_SvDefaultTownTune));
    PackMelodyInline(tune, &g_SvDefaultTownTune);
}

// 0x006BCC54 (name is ours)
void PackMelody(u8* packed, const SoundMelody* melody)
{
    PackMelodyInline(packed, melody);
}
