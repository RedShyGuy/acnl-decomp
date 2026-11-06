#include "Sound/dSoundSeaMgr.h"

// (in a file of its own: GCC would otherwise resolve the call in Update)
// 0x0083ED78
void (SoundSeaMgr::*const SoundSeaMgr::s_CalcFuncs[TYPE_NUM])() = {
    &SoundSeaMgr::CalcVillage, &SoundSeaMgr::CalcDownTown, &SoundSeaMgr::CalcTour1,
    &SoundSeaMgr::CalcTour2,   &SoundSeaMgr::CalcIsland,
};
