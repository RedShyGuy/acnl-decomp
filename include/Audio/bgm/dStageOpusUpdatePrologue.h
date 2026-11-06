#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Sound/dSoundLabel.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvPlayer.h"
#include "stage/stage_Api.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm23StageOpusUpdatePrologueE @ 0x008D28A0
// vtable 0x00908910 (vptr 0x00908918), offset_to_top 0, 18 entries
class StageOpusUpdatePrologue : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusUpdatePrologue() {
        m_Kind = KIND_COMMAND;
        m_Fade = 5;
        m_BgmId = STRM_BGM_PROLOGUE_08;
    }
    virtual ~StageOpusUpdatePrologue();
    virtual void OnStart(); // 0x0058929C slot 0x0C
    virtual void OnStop(); // 0x005892D4 slot 0x10
    virtual void EnterKeep(); // 0x00589288 slot 0x28
    virtual void Leave(stage::Name next); // 0x0058927C slot 0x2C
    virtual void Ready(); // 0x00589270 slot 0x38
    virtual bool CanKeep(stage::Name next, bool) // 0x00753FBC slot 0x40
    {
        SvPlayer* player = SvGetCurrentPlayer();
        bool result = false;
        if (player != NULL && next == stage::NAME_VILLAGE) {
            result = player->playerFlags.unk_0x2B_7;
        }
        return result;
    }
};
ASSERT_SIZE(StageOpusUpdatePrologue, 0x20);
} // namespace bgm
} // namespace audio
