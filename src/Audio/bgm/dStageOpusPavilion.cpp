#include "Audio/bgm/dStageOpusPavilion.h"
#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Item/item_Api.h"
#include "Sound/dSoundLabel.h"
#include "Sound/sound_Extern.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"

// 0x00587B8C
void audio::bgm::StageOpusPavilion::Setup(stage::Name)
{
    // the index at npcData.unk_0x176A6 (an entry of the table 0xA8449C)
    u16 index = *reinterpret_cast<const u16*>(GetGardenPlus()->data.npcData.unk_0x176A6);
    UnknownA8449CEntry* entries = g_UnknownA8449C.entries;
    UnknownA8449CEntry* entry;
    if (entries != NULL && index < 0x18F && (entry = &entries[index]) != NULL) {
        m_IsReady = 0;
        if (Func63061C()) {
            m_PavilionKind = KIND_SHOP;
            m_PavilionBgmId = STRM_BGM_IND_IGLOO;
        } else {
            m_PavilionBgmId = item::GetIndex(*reinterpret_cast<const SvFgName*>(entry->item));
            m_PavilionKind = KIND_OBJ_SONG;
        }
    } else {
        m_IsReady = 1;
    }
    s_ClockState.isShop = 1;
}

// 0x00587C40
void audio::bgm::StageOpusPavilion::Leave(stage::Name)
{
    Stop();
}

// 0x00587C4C
void audio::bgm::StageOpusPavilion::OnStart()
{
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(m_PavilionKind, m_PavilionBgmId, 1, times);
}

// 0x00587C84
void audio::bgm::StageOpusPavilion::Update()
{
    if (m_IsCurrent || m_IsReady || !IsPlayerReady()) {
        return;
    }
    if (Start()) {
        m_IsReady = 1;
    }
}

// 0x00587CCC
void audio::bgm::StageOpusPavilion::OnStop()
{
    GetRequestMgr()->SetFade(m_PavilionKind, m_PavilionBgmId, 0, 0, 0);
}

// 0x00587D04
// 0x00587D00 (deleting dtor)
audio::bgm::StageOpusPavilion::~StageOpusPavilion()
{
    // empty (in the original too)
}
