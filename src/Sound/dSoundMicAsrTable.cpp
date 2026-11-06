#include "Sound/dSoundMicMgr.h"

// 0x0083C46C
const SoundMicAsr::UpdateFunc SoundMicAsr::s_UpdateFuncs[6] = {
    &SoundMicAsr::UpdateNothing, &SoundMicAsr::UpdateLoading,   &SoundMicAsr::UpdateNothing,
    &SoundMicAsr::UpdateWaitSound, &SoundMicAsr::UpdateWaitTimer, &SoundMicAsr::UpdateRecognizing,
};
// 0x0083C49C
const s32 SoundMicAsr::s_FileIds[FILE_NUM] = {1, 2, 3, 4, 5, 6};
