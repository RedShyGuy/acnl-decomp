#include "Audio/bgm/dEvOpusBase.h"

// 0x00584958
void audio::bgm::EvOpusBase::Begin()
{
    // empty (in the original too)
}

// 0x0058495C
void audio::bgm::EvOpusBase::Resume()
{
    // empty (in the original too)
}

// 0x00584960
bool audio::bgm::EvOpusBase::StartRequest(u8, u32)
{
    return false;
}

// 0x00584968
bool audio::bgm::EvOpusBase::StopRequest(u8, u32)
{
    return false;
}

// 0x00584970
void audio::bgm::EvOpusBase::Update()
{
    // empty (in the original too)
}

// 0x00584974
void audio::bgm::EvOpusBase::End(bool)
{
    // empty (in the original too)
}
