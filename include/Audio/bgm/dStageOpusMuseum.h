#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"
#include "stage/stage_Api.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm15StageOpusMuseumE @ 0x008D26C0
// vtable 0x00907CAC (vptr 0x00907CB4), offset_to_top 0, 18 entries
class StageOpusMuseum : public ::audio::bgm::StageOpusBase
{
public:
    StageOpusMuseum() {}
    virtual ~StageOpusMuseum();
    virtual void OnStart(); // 0x00587320 slot 0x0C
    virtual void OnStop(); // 0x005873A0 slot 0x10
    virtual void Update(); // 0x00587358 slot 0x1C
    virtual void EnterKeep(); // 0x0058730C slot 0x28
    virtual void Leave(stage::Name next); // 0x00587240 slot 0x2C
    virtual void Setup(stage::Name stage); // 0x005871AC slot 0x30
    virtual void SetupKeep(stage::Name stage); // 0x00587268 slot 0x34
    virtual bool CanKeep(stage::Name next, bool isOnline) // 0x00753E74 slot 0x40
    {
        if (stage::IsKind(0x8000, next) && !stage::IsKind(0x2000, next)) {
            return true;
        }
        return !isOnline;
    }

    u8 m_Room; // 0x1E
};
ASSERT_SIZE(StageOpusMuseum, 0x20);
} // namespace bgm
} // namespace audio
