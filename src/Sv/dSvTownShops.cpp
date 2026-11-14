#include "Sv/dSvTown.h"

#include "Sv/dSvCommon.h"

// 0x005CE224 (name is ours)
void SvTownDataUnkEE2C::Clear()
{
    shopZakka.Clear();
    Func314144(&unk_0x179A4);
    Func109FF4(ablesItems);
    Func2A809C(labellesItems);
    Func10B38C(&unk_0x132C0);
    Func10A7FC(reddItems);
    Func108B54(unk_0x133F8);
    Func20D8E4(unk_0x13428);
    Func10AB28(reTailPremiumItems);
    Func10AF5C(&museumShopUnlockState);
    Func10B6CC(nooksHomeItems);
    Func108A98(&gracieUnlockStatus);
    Func10A3FC(&clubLolUnlockState);
    Func108444(&dreamSuiteUnlockStatus);
    Func10A4A4(&fortuneTellerUnlockStatus);
    Func10B438(&unk_0x17981[0xB]);
    Func20D628(islandShopItems);
    Func10B1E0(&unk_0x17A24);
    Func106BAC(campgroundShopItems);
    unk_0x17A4C = SvDate();
    u32* words = reinterpret_cast<u32*>(unk_0x17A50);
    words[0] = 0;
    for (u32 i = 0; i < 16; i++) {
        words[1 + i] = 0;
    }
}
