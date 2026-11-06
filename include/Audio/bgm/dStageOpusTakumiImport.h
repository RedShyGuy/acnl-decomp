#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusSelect.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm21StageOpusTakumiImportE @ 0x008D2894
// vtable 0x009088C0 (vptr 0x009088C8), offset_to_top 0, 18 entries
class StageOpusTakumiImport : public ::audio::bgm::StageOpusSelect
{
public:
    StageOpusTakumiImport() {}
    virtual ~StageOpusTakumiImport();
    virtual void OnStart(); // 0x005891FC slot 0x0C
    virtual void OnStop(); // 0x00589234 slot 0x10
};
ASSERT_SIZE(StageOpusTakumiImport, 0x20);
} // namespace bgm
} // namespace audio
