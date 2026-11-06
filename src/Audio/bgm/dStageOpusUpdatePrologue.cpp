#include "Audio/bgm/dStageOpusUpdatePrologue.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Audio/bgm/dBgmRequestMgr.h"

// 0x00589270
void audio::bgm::StageOpusUpdatePrologue::Ready()
{
    Start();
}

// 0x0058927C
void audio::bgm::StageOpusUpdatePrologue::Leave(stage::Name)
{
    Stop();
}

// 0x00589288
void audio::bgm::StageOpusUpdatePrologue::EnterKeep()
{
    GetTrackMixer()->m_IsSequenceAlt = 1;
}

// 0x0058929C
void audio::bgm::StageOpusUpdatePrologue::OnStart()
{
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(m_Kind, m_BgmId, 20, times);
}

// 0x005892D4
void audio::bgm::StageOpusUpdatePrologue::OnStop()
{
    GetRequestMgr()->SetFade(m_Kind, m_BgmId, m_Fade, 0, 0);
}

// 0x0058930C
// 0x00589308 (deleting dtor)
audio::bgm::StageOpusUpdatePrologue::~StageOpusUpdatePrologue()
{
    // empty (in the original too)
}
