#include "Audio/bgm/dStageOpusNull.h"

// 0x00586794
void audio::bgm::StageOpusNull::Setup(stage::Name)
{
    // empty (in the original too)
}

// 0x0058679C
// 0x00586798 (deleting dtor)
audio::bgm::StageOpusNull::~StageOpusNull()
{
    // empty (in the original too)
}
