// the table of the kinds of orders (in its own file: GCC would fold it into the functions)
#include "demo/demo_Api.h"
#include "demo/demo_Extern.h"

namespace demo {
// 0x0085F250
extern const OrderFunctions s_OrderFunctions[ORDER_ID_NUM] = {
    {Func1662A8, Func165ED4, Func189CD4, Func189BF8}, // ORDER_0
    {Func165BDC, Func165750, Func189B74, Func1894C8}, // INTERACT
    {Func1662B0, Func16574C, Func189CF4, Func189C00}, // DEFAULT_TALK
    {Func165B20, Func16571C, Func189A5C, Func1894A4}, // ORDER_3
    {Func165BB4, Func16573C, Func189B5C, Func1894BC}, // WARP
    {Func16DDB0, Func16CB28, Func168E10, Func167F10}, // ORDER_5
    {Func16DDB0, Func16CB28, Func168E10, Func167F10}, // CREDITS
    {Func165B58, Func165734, Func189B4C, Func1894B8}, // PLAYER_THINK
    {Func16C9E0, Func169B80, Func16635C, Func166270}, // ISLAND_FORCE_LEAVE_THINK
    {Func168DD4, Func16636C, Func165688, Func5C1534}, // ORDER_9
    {Func165BBC, Func165744, Func189B64, Func1894C4}, // SAVE
    {Func166288, Func165ECC, Func189CC4, Func189BF4}, // ORDER_11
    {Func169B54, Func168EFC, Func165C90, Func165B1C}, // NETGAME_LOCK
    {Func165B28, Func165724, Func189B3C, Func1894B4}, // ORDER_13
};
} // namespace demo
