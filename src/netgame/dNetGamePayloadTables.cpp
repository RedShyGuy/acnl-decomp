// the tables of the payloads (in their own file: GCC would fold them into the functions)
#include "netgame/dNetGamePayload.h"

namespace netgame {
namespace payload {
// the functions of the game that fill and apply the payloads (not decompiled yet; named after their address)
s32 Build183E24(void* data, u32 player, u32 myNo, u32 size, u32 stage); // 0x00183E24
void Apply184DC8(u32 sender, u32 param, const void* data, u32 size, u32 stage); // 0x00184DC8
s32 Build183F74(void* data, u32 param, u32 size, u32 stage); // 0x00183F74
void Apply184FF8(u32 sender, const void* data, u32 size, u32 stage); // 0x00184FF8
s32 Build184BBC(void* data, u32 param, u32 size, u32 stage); // 0x00184BBC
void Apply185FB8(u32 sender, const void* data, u32 size, u32 stage); // 0x00185FB8
s32 Build183F7C(void* data, u32 param, u32 size, u32 stage); // 0x00183F7C
void Apply184FFC(u32 sender, const void* data, u32 size, u32 stage); // 0x00184FFC
s32 Build184BC4(void* data, u32 param, u32 size, u32 stage); // 0x00184BC4
void Apply185FBC(u32 sender, const void* data, u32 size, u32 stage); // 0x00185FBC
} // namespace payload

using namespace payload;

// 0x0087C0E0
const netgame::PayloadA::Table netgame::PayloadA::s_Table = {1, Build183E24, Apply184DC8};
// 0x0087C0EC
const netgame::PayloadB::Table netgame::PayloadB::s_Table = {1, Build183F74, Apply184FF8};
// 0x0087C0F8
const netgame::PayloadC::Table netgame::PayloadC::s_Table = {1, Build184BBC, Apply185FB8};
// 0x0087CD0C
const netgame::PayloadD::Table netgame::PayloadD::s_Table = {1, Build183F7C, Apply184FFC};
// 0x0087CD18
const netgame::PayloadE::Table netgame::PayloadE::s_Table = {1, Build184BC4, Apply185FBC};
} // namespace netgame
