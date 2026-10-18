#include "nn/nex/nex_ErrorCodeConverter.h"

namespace nn {
namespace nex {
namespace {
// the hundreds of the network error codes per group
const u32 GROUP_FACTOR = 100;
// the network error codes of the system errors (from the first one): the errors of module 0 (and
// the facade), of module 1, and the end of their codes
const u32 CORE_ERROR_CODES = 900;
const u32 MODULE1_ERROR_CODES = 950;
const u32 LAST_SYSTEM_ERROR_CODE = 1000;
} // namespace

// the first network error code and the offsets of the groups (names are ours)
// 0x0096B098
u32 g_NetworkErrorCodeTable[21] = {60000, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1};

// 0x003879A4 | fefates:callgraph [tier C]
u32 nn::nex::ErrorCodeConverter::ConvertToNetworkErrorCode(const qResult& result)
{
    s32 code = result.m_Code;
    u32 module = result.GetModule();
    if ((code & 0x80000000) == 0) {
        return 0;
    }
    u32 group;
    s32 description;
    switch (module) {
    case 1:
        group = 1;
        description = code - static_cast<s32>(0x80010001);
        break;
    case 2:
        group = 2;
        description = code - static_cast<s32>(0x80020001);
        break;
    case 3:
        group = 3;
        if (code < static_cast<s32>(0x80030064)) {
            description = code - static_cast<s32>(0x80030001);
        } else if (code < static_cast<s32>(0x800300C8)) {
            description = code - static_cast<s32>(0x80030062);
        } else {
            description = code - static_cast<s32>(0x800300B3);
        }
        break;
    case 4:
        group = 4;
        description = code - static_cast<s32>(0x80040001);
        break;
    case 5:
        group = 5;
        description = code - static_cast<s32>(0x80050001);
        break;
    case 6:
        group = 6;
        description = code - static_cast<s32>(0x80060001);
        break;
    case 101:
        group = 7;
        description = code - static_cast<s32>(0x80650000);
        break;
    case 103:
        group = 11;
        description = code - static_cast<s32>(0x80670001);
        break;
    case 104:
        group = 8;
        description = code - static_cast<s32>(0x80680001);
        break;
    case 105:
        group = 12;
        description = code - static_cast<s32>(0x80690001);
        break;
    case 108:
        group = 15;
        description = code - static_cast<s32>(0x806C0001);
        break;
    case 111:
        group = 18;
        description = code - static_cast<s32>(0x806F0001);
        break;
    case 112:
        group = 19;
        description = code - static_cast<s32>(0x80700001);
        break;
    case 113:
        group = 20;
        description = code - static_cast<s32>(0x80710001);
        break;
    default:
        return g_NetworkErrorCodeTable[0];
    }
    return description + group * GROUP_FACTOR + g_NetworkErrorCodeTable[0] + g_NetworkErrorCodeTable[group];
}

// 0x00387BFC (name is ours)
u32 nn::nex::ErrorCodeConverter::ConvertSystemErrorToNetworkErrorCode(u32 error)
{
    u32 base = g_NetworkErrorCodeTable[0];
    u32 code;
    switch (error) {
    case 0xE0100000:
        code = base + CORE_ERROR_CODES + 1;
        break;
    case 0xE0000000:
        code = base + CORE_ERROR_CODES + 2;
        break;
    case 0xE0000001:
        code = base + CORE_ERROR_CODES + 3;
        break;
    case 0xE0000002:
        code = base + CORE_ERROR_CODES + 4;
        break;
    case 0xE0000003:
        code = base + CORE_ERROR_CODES + 5;
        break;
    case 0xA0000004:
        code = base + CORE_ERROR_CODES + 6;
        break;
    case 0xA0000005:
        code = base + CORE_ERROR_CODES + 7;
        break;
    case 0xA0000006:
        code = base + CORE_ERROR_CODES + 8;
        break;
    case 0xA0000007:
        code = base + CORE_ERROR_CODES + 9;
        break;
    case 0xE0000008:
        code = base + CORE_ERROR_CODES + 10;
        break;
    case 0xE0000009:
        code = base + CORE_ERROR_CODES + 11;
        break;
    case 0xA000000A:
        code = base + CORE_ERROR_CODES + 12;
        break;
    case 0xA000000B:
        code = base + CORE_ERROR_CODES + 13;
        break;
    case 0xE000000C:
        code = base + CORE_ERROR_CODES + 14;
        break;
    case 0xA000000D:
        code = base + CORE_ERROR_CODES + 15;
        break;
    case 0xE000000E:
        code = base + CORE_ERROR_CODES + 16;
        break;
    case 0xE000000F:
        code = base + CORE_ERROR_CODES + 17;
        break;
    case 0xE0000010:
        code = base + CORE_ERROR_CODES + 18;
        break;
    case 0xE0000011:
        code = base + CORE_ERROR_CODES + 19;
        break;
    case 0xE0000012:
        code = base + CORE_ERROR_CODES + 20;
        break;
    case 0xE0000013:
        code = base + CORE_ERROR_CODES + 21;
        break;
    case 0xE0000014:
        code = base + CORE_ERROR_CODES + 22;
        break;
    case 0xE0000015:
        code = base + CORE_ERROR_CODES + 23;
        break;
    case 0xE0000016:
        code = base + CORE_ERROR_CODES + 24;
        break;
    case 0xE0000017:
        code = base + CORE_ERROR_CODES + 25;
        break;
    case 0xE0000018:
        code = base + CORE_ERROR_CODES + 26;
        break;
    case 0xE0000019:
        code = base + CORE_ERROR_CODES + 27;
        break;
    case 0xE000001A:
        code = base + CORE_ERROR_CODES + 28;
        break;
    case 0xE000001B:
        code = base + CORE_ERROR_CODES + 29;
        break;
    case 0xE000001C:
        code = base + CORE_ERROR_CODES + 30;
        break;
    case 0xA000001D:
        code = base + CORE_ERROR_CODES + 31;
        break;
    case 0xE000001E:
        code = base + CORE_ERROR_CODES + 32;
        break;
    case 0xE0010000:
        code = base + MODULE1_ERROR_CODES + 1;
        break;
    case 0xE0010001:
        code = base + MODULE1_ERROR_CODES + 2;
        break;
    case 0xE0010002:
        code = base + MODULE1_ERROR_CODES + 3;
        break;
    case 0xA0010003:
        code = base + MODULE1_ERROR_CODES + 4;
        break;
    case 0xA0010004:
        code = base + MODULE1_ERROR_CODES + 5;
        break;
    case 0xE0010005:
        code = base + MODULE1_ERROR_CODES + 6;
        break;
    case 0xE0010006:
        code = base + MODULE1_ERROR_CODES + 7;
        break;
    case 0xE0010007:
        code = base + MODULE1_ERROR_CODES + 8;
        break;
    case 0xE0010008:
        code = base + MODULE1_ERROR_CODES + 9;
        break;
    case 0xE0010009:
        code = base + MODULE1_ERROR_CODES + 10;
        break;
    case 0xE001000A:
        code = base + MODULE1_ERROR_CODES + 11;
        break;
    case 0xE001000B:
        code = base + MODULE1_ERROR_CODES + 12;
        break;
    case 0xE001000C:
        code = base + MODULE1_ERROR_CODES + 13;
        break;
    case 0xE001000D:
        code = base + MODULE1_ERROR_CODES + 14;
        break;
    case 0xE001000E:
        code = base + MODULE1_ERROR_CODES + 15;
        break;
    case 0xA001000F:
        code = base + MODULE1_ERROR_CODES + 16;
        break;
    case 0xE0010010:
        code = base + MODULE1_ERROR_CODES + 17;
        break;
    case 0xE0010011:
        code = base + MODULE1_ERROR_CODES + 18;
        break;
    case 0xE0010012:
        code = base + MODULE1_ERROR_CODES + 19;
        break;
    case 0xE0010013:
        code = base + MODULE1_ERROR_CODES + 20;
        break;
    case 0x60010014:
        code = base + MODULE1_ERROR_CODES + 21;
        break;
    case 0xE0010015:
        code = base + MODULE1_ERROR_CODES + 22;
        break;
    case 0xE0010016:
        code = base + MODULE1_ERROR_CODES + 23;
        break;
    case 0xE0010017:
        code = base + MODULE1_ERROR_CODES + 24;
        break;
    case 0xE0010018:
        code = base + MODULE1_ERROR_CODES + 25;
        break;
    case 0xE0010019:
        code = base + MODULE1_ERROR_CODES + 26;
        break;
    case 0xA001001B:
        code = base + MODULE1_ERROR_CODES + 28;
        break;
    default:
        return base;
    }
    if (code > base + CORE_ERROR_CODES && code < base + LAST_SYSTEM_ERROR_CODE) {
        return code;
    }
    return base;
}

} // namespace nex
} // namespace nn
