#include "Sound/dSoundDJPlayer.h"
#include "Sound/dSoundLabel.h"

// (in a file of their own: GCC would otherwise fold the tables, the frames per beat are the same for both tempos)
// 0x0083A1D8
const u8 SoundDJPlayer::s_FixedSongs[2] = {SoundSongQueue::SONG_FLAG | 90, SoundSongQueue::SONG_FLAG | 94};
// 0x0083A1DA
const s16 SoundDJPlayer::s_Tempos[2] = {130, 140};
// 0x0083A1E0
const u32 SoundDJPlayer::s_FirstSongs[2] = {SEQ_BGM_DJ01_HS_SANBIKA, SEQ_BGM_DJ01_FC_SANBIKA};
// 0x0083A1E8
const u32 SoundDJPlayer::s_BreakSounds[2][2] = {
    {SEQ_BGM_DJ_HS_MURAMELO, SEQ_BGM_DJ_FC_MURAMELO},
    {SEQ_BGM_DJ_HS_HANIWA, SEQ_BGM_DJ_FC_HANIWA},
};
