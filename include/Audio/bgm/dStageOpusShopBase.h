#pragma once

#include "decomp.h"
#include "Audio/bgm/dStageOpusBase.h"

namespace audio {
namespace bgm {
// RTTI N5audio3bgm17StageOpusShopBaseE @ 0x008D2780
// The music of a shop: the music of the shop until the last hour, at hh:49:50 of the last hour the closing music
// (STRM_BGM_IND_KODANUKI_CLOSE) and from hh:50 the closed shop. Member names are ours.
class StageOpusShopBase : public ::audio::bgm::StageOpusBase
{
public:
    // m_ShopState
    enum ShopState
    {
        SHOP_OPEN = 0,
        SHOP_CLOSING = 1,
        SHOP_CLOSED = 2,
        SHOP_NONE = 3,
    };

    StageOpusShopBase(u32 bgmId, u8 hasMix36, u8 hasMix36Online)
        : m_ShopBgmId(bgmId), m_HasMix36(hasMix36), m_HasMix36Online(hasMix36Online), m_ShopState(SHOP_NONE)
    {
    }

    virtual void OnStart(); // 0x00587DC0 slot 0x0C
    virtual void OnStop(); // 0x00587F38 slot 0x10
    virtual void Update(); // 0x00587E30 slot 0x1C
    virtual void EnterKeep(); // 0x00587DAC slot 0x28
    virtual void Leave(stage::Name next); // 0x00587D94 slot 0x2C
    virtual void Setup(stage::Name stage); // 0x00587D08 slot 0x30
    // the hour when the shop closes (32: never)
    virtual void InitCloseHour(); // 0x00587DA0 slot 0x48

    u32 m_ShopBgmId;     // 0x20
    u8 m_HasMix36;       // 0x24 the mix 36 is on in the shop
    u8 m_HasMix36Online; // 0x25 also in an online game
    u8 m_ShopState;      // 0x26 ShopState
    s8 m_LastHour;       // 0x27 the last hour before the closing
    s8 m_LastSecond;     // 0x28
    s8 m_CloseHour;      // 0x29 (hours after midnight count from 24)
};
ASSERT_OFFSET(StageOpusShopBase, m_ShopBgmId, 0x20);
ASSERT_SIZE(StageOpusShopBase, 0x2C);
} // namespace bgm
} // namespace audio
