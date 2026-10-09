#pragma once

#include "decomp.h"

namespace nn {
namespace CTR {
// the icon file of a title, "SMDH" (the type name is from the reference symbols; the layout and
// the field names after 3dbrew "SMDH")
struct SystemMenuData
{
    struct ApplicationTitle
    {
        u16 shortDescription[0x40]; // 0x000, UTF-16
        u16 longDescription[0x80];  // 0x080
        u16 publisher[0x40];        // 0x180
    };

    struct ApplicationSettings
    {
        u8 ratings[0x10];          // 0x00, region specific game ratings
        u32 regionLockout;         // 0x10
        u8 matchMakerIds[0xC];     // 0x14
        u32 flags;                 // 0x20
        u8 eulaVersionMinor;       // 0x24
        u8 eulaVersionMajor;       // 0x25
        u8 reserved26[2];          // 0x26
        f32 optimalAnimationFrame; // 0x28
        u32 cecId;                 // 0x2C
    };

    static const u32 TITLE_NUM = 16;

    u32 magic;                            // 0x0000, "SMDH"
    u16 version;                          // 0x0004
    u8 reserved6[2];                      // 0x0006
    ApplicationTitle titles[TITLE_NUM];   // 0x0008
    ApplicationSettings settings;         // 0x2008
    u8 reserved2038[8];                   // 0x2038
    u8 iconGraphics[0x1680];              // 0x2040
};
ASSERT_SIZE(SystemMenuData::ApplicationTitle, 0x200);
ASSERT_SIZE(SystemMenuData::ApplicationSettings, 0x30);
ASSERT_OFFSET(SystemMenuData, settings, 0x2008);
ASSERT_SIZE(SystemMenuData, 0x36C0);
} // namespace CTR
} // namespace nn
