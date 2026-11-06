#include "Audio/bgm/dStageOpusTakumiImport.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Sound/dSoundLabel.h"

// 0x005891FC
void audio::bgm::StageOpusTakumiImport::OnStart()
{
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(KIND_SHOP, WSD_BGM_TAKUMI_IMPORT, 20, times);
}

// 0x00589234
void audio::bgm::StageOpusTakumiImport::OnStop()
{
    GetRequestMgr()->SetFade(KIND_SHOP, WSD_BGM_TAKUMI_IMPORT, 0, 0, 0);
}

// 0x0058926C
// 0x00589268 (deleting dtor)
audio::bgm::StageOpusTakumiImport::~StageOpusTakumiImport()
{
    // empty (in the original too)
}
