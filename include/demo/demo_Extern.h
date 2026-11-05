#pragma once

#include "decomp.h"
#include "netgame/netgame_Api.h"

class AcPlayer;
class DemoActor;

// Functions of the game that the orders use (not decompiled yet; names are ours, FuncXXXXXX while the purpose is unknown).
namespace demo {
class Order;

// the functions of the kinds of orders (s_OrderFunctions)
bool Func165688(Order* order); // 0x00165688
bool Func16571C(Order* order); // 0x0016571C
bool Func165724(Order* order); // 0x00165724
bool Func165734(Order* order); // 0x00165734
bool Func16573C(Order* order); // 0x0016573C
bool Func165744(Order* order); // 0x00165744
bool Func16574C(Order* order); // 0x0016574C
bool Func165750(Order* order); // 0x00165750
bool Func165B1C(Order* order); // 0x00165B1C
bool Func165B20(Order* order); // 0x00165B20
bool Func165B28(Order* order); // 0x00165B28
bool Func165B58(Order* order); // 0x00165B58
bool Func165BB4(Order* order); // 0x00165BB4
bool Func165BBC(Order* order); // 0x00165BBC
bool Func165BDC(Order* order); // 0x00165BDC
bool Func165C90(Order* order); // 0x00165C90
bool Func165ECC(Order* order); // 0x00165ECC
bool Func165ED4(Order* order); // 0x00165ED4
bool Func166270(Order* order); // 0x00166270
bool Func166288(Order* order); // 0x00166288
bool Func1662A8(Order* order); // 0x001662A8
bool Func1662B0(Order* order); // 0x001662B0
bool Func16635C(Order* order); // 0x0016635C
bool Func16636C(Order* order); // 0x0016636C
bool Func167F10(Order* order); // 0x00167F10
bool Func168DD4(Order* order); // 0x00168DD4
bool Func168E10(Order* order); // 0x00168E10
bool Func168EFC(Order* order); // 0x00168EFC
bool Func169B54(Order* order); // 0x00169B54
bool Func169B80(Order* order); // 0x00169B80
bool Func16C9E0(Order* order); // 0x0016C9E0
bool Func16CB28(Order* order); // 0x0016CB28
bool Func16DDB0(Order* order); // 0x0016DDB0
bool Func1894A4(Order* order); // 0x001894A4
bool Func1894B4(Order* order); // 0x001894B4
bool Func1894B8(Order* order); // 0x001894B8
bool Func1894BC(Order* order); // 0x001894BC
bool Func1894C4(Order* order); // 0x001894C4
bool Func1894C8(Order* order); // 0x001894C8
bool Func189A5C(Order* order); // 0x00189A5C
bool Func189B3C(Order* order); // 0x00189B3C
bool Func189B4C(Order* order); // 0x00189B4C
bool Func189B5C(Order* order); // 0x00189B5C
bool Func189B64(Order* order); // 0x00189B64
bool Func189B74(Order* order); // 0x00189B74
bool Func189BF4(Order* order); // 0x00189BF4
bool Func189BF8(Order* order); // 0x00189BF8
bool Func189C00(Order* order); // 0x00189C00
bool Func189CC4(Order* order); // 0x00189CC4
bool Func189CD4(Order* order); // 0x00189CD4
bool Func189CF4(Order* order); // 0x00189CF4
bool Func5C1534(Order* order); // 0x005C1534

// PlayerGetActor with a removed call in front (falls into it)
AcPlayer* PlayerGetActorEntry(netgame::PlayerNo player, bool flag); // 0x005C3E9C
// the demo actor with the id (NULL: none)
DemoActor* FindDemoActor(u32 id); // 0x006E5218
void Func5C3998(); // 0x005C3998
bool Func5C32D8(); // 0x005C32D8
void Func5C2CC0(); // 0x005C2CC0
u32 Func241598(); // 0x00241598
} // namespace demo
