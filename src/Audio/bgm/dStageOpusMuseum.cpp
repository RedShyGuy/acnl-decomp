#include "Audio/bgm/dStageOpusMuseum.h"
#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Sound/dSoundLabel.h"

namespace audio {
namespace bgm {
namespace {

// the room of the museum (1: the room with the mix 48)
inline u8 GetMuseumRoom(stage::Name stage)
{
    if (stage == 73) {
        return 0;
    }
    if (static_cast<u32>(stage - 74) <= 2) {
        return 2;
    }
    if (static_cast<u32>(stage - 77) <= 2) {
        return 1;
    }
    if (static_cast<u32>(stage - 80) <= 2) {
        return 3;
    }
    if (static_cast<u32>(stage - 83) <= 1) {
        return 4;
    }
    return stage == 85 ? 5 : 6;
}

} // namespace
} // namespace bgm
} // namespace audio

// 0x005871AC
void audio::bgm::StageOpusMuseum::Setup(stage::Name stage)
{
    u8 room = GetMuseumRoom(stage);
    m_Room = room;
    m_IsReady = 0;
    if (room == 1) {
        GetTrackMixer()->AddMix(48);
    }
    s_ClockState.isShop = 1;
}

// 0x00587240
void audio::bgm::StageOpusMuseum::Leave(stage::Name)
{
    GetTrackMixer()->RemoveMix(48);
    Stop();
}

// 0x00587268
void audio::bgm::StageOpusMuseum::SetupKeep(stage::Name stage)
{
    u8 room = GetMuseumRoom(stage);
    if (m_Room == room) {
        return;
    }
    if (m_Room == 1) {
        GetTrackMixer()->RemoveMix(48);
    }
    if (room == 1) {
        GetTrackMixer()->AddMix(48);
    }
    m_Room = room;
}

// 0x0058730C
void audio::bgm::StageOpusMuseum::EnterKeep()
{
    GetTrackMixer()->m_IsSequenceAlt = 1;
}

// 0x00587320
void audio::bgm::StageOpusMuseum::OnStart()
{
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(KIND_SHOP, STRM_BGM_IND_MUSEUM, 20, times);
}

// 0x00587358
void audio::bgm::StageOpusMuseum::Update()
{
    if (m_IsCurrent || m_IsReady || !IsPlayerReady()) {
        return;
    }
    m_IsReady = 1;
    Start();
}

// 0x005873A0
void audio::bgm::StageOpusMuseum::OnStop()
{
    GetRequestMgr()->SetFade(KIND_SHOP, STRM_BGM_IND_MUSEUM, 0, 0, 0);
}

// 0x005873D8
// 0x005873D4 (deleting dtor)
audio::bgm::StageOpusMuseum::~StageOpusMuseum()
{
    // empty (in the original too)
}
