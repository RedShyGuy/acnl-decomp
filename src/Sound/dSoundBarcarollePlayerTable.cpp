#include "Sound/dSoundBarcarollePlayer.h"
#include "Sound/dSoundLabel.h"

// (in a file of their own: GCC would otherwise fold the tables and resolve the call in Update)
// the frames before the first song starts
// 0x0083A000
const u8 SoundBarcarollePlayer::s_StartWaits[KIND_NUM] = {75, 45};
// the frames after the first song
// 0x0083A002
const u8 SoundBarcarollePlayer::s_FirstEndWaits[KIND_NUM] = {45, 45};
// the frames before the second song starts
// 0x0083A004
const u8 SoundBarcarollePlayer::s_SecondWaits[KIND_NUM] = {35, 0};
// the frames after the second song
// 0x0083A006
const u8 SoundBarcarollePlayer::s_SecondEndWaits[KIND_NUM] = {30, 30};
// from here on Kapp'n sings in the first song
// 0x0083A008
const u32 SoundBarcarollePlayer::s_SingTicks[KIND_NUM] = {1, 432};
// 0x0083A010
const u32 SoundBarcarollePlayer::s_LookaheadTicks[KIND_NUM] = {4, 5};
// the shortest gap between two notes of the random matching song
// 0x0083A018
const u32 SoundBarcarollePlayer::s_MatchingGaps[KIND_NUM] = {36, 72};
// the first of the 30 songs; the second songs follow at + 30
// 0x0083A020
const u32 SoundBarcarollePlayer::s_FirstSounds[KIND_NUM] = {SEQ_BGM_KAPPEI_GO_A_MALE01, SEQ_BGM_KAPPEI_BACK_A_MALE01};
// 0x0083A028
const SoundBarcarollePlayer::PhraseCount SoundBarcarollePlayer::s_PhraseCounts[KIND_NUM] = {{2, 4}, {2, 4}};
// 0x0083A030
const u32 SoundBarcarollePlayer::s_MatchingSounds[KIND_NUM] = {SEQ_BGM_KAPPEI_GO_RANDOMMATCHING,
                                                                SEQ_BGM_KAPPEI_BACK_RANDOMMATCHING};
// the loop of the random matching song
// 0x0083A038
const u32 SoundBarcarollePlayer::s_LoopStarts[KIND_NUM] = {48, 6936};
// 0x0083A040
const u32 SoundBarcarollePlayer::s_LoopEnds[KIND_NUM] = {11472, 11544};
// 0x0083A048
void (SoundBarcarollePlayer::*const SoundBarcarollePlayer::s_UpdateFuncs[STATE_NUM])() = {
    &SoundBarcarollePlayer::UpdateNone,        &SoundBarcarollePlayer::UpdateFirstSong,
    &SoundBarcarollePlayer::UpdateNone,        &SoundBarcarollePlayer::UpdateSecondSong,
    &SoundBarcarollePlayer::UpdateRandomMatching,
};
// the ticks at which a phrase ends ([phrase + 1])
// 0x0083A070
const u32 SoundBarcarollePlayer::s_PhraseTicks[KIND_NUM][5] = {
    {0, 730, 1714, 0, 890},
    {192, 1129, 1896, 48, 1271},
};
// from here on the part changes (first song, second song)
// 0x0083A098
const u32 SoundBarcarollePlayer::s_PartTicks[KIND_NUM][2] = {{386, 482}, {768, 648}};
// 0x0083A0A8
const s16 SoundBarcarollePlayer::s_NoteKinds[NOTE_TABLE_NUM] = {2, 5, 3, 1, 4, -1};
