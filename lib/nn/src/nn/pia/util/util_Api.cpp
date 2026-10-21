#include "nn/pia/util/util_Api.h"

#include "nn/pia/inet/inet_NexMatchmakeSession.h"

namespace nn {
namespace pia {
namespace util {
namespace {
// the error code of all other failures (error 018-0000; name is ours)
const u32 ERROR_CODE_BASE = 180000;
} // namespace

// 0x00413978 | fefates:bytes [tier B]
bool IsPiaResult(const nn::Result& result)
{
    return result.GetModule() == RESULT_MODULE_PIA;
}

// The three functions below are only called by the game. The meaning of the kinds is not known;
// the values are written as in the original (the names of the codes are in the headers of the
// layers, e.g. common_Result.h).

// 0x00413990 (name is ours)
s32 GetResultKind(const nn::Result& result)
{
    if (result.GetModule() != RESULT_MODULE_PIA) {
        return 0;
    }
    switch (result.GetPrintableBits()) {
    case 0xD8A14825:
    case 0xD8A148C1:
    case 0xD8A148C3:
    case 0xD8A148E1:
    case 0xD8A148E7:
    case 0xD8A148EA:
    case 0xD8A148EB:
    case 0xD8A148EC:
    case 0xD8A148ED:
    case 0xD8A148F2:
    case 0xD8A148F7:
    case 0xF8A148C4:
        return 1;
    case 0xD8A148E0:
    case 0xD8A148EE:
    case 0xD8A148EF:
    case 0xD8A148F0:
    case 0xD8A148F1:
        return 2;
    default:
        return 3;
    }
}

// 0x00413A68 (name is ours)
s32 GetResultCategory(const nn::Result& result)
{
    if (result.GetModule() != RESULT_MODULE_PIA) {
        return 0;
    }
    switch (result.GetPrintableBits()) {
    case 0xC8A14831:
    case 0xD0814864:
    case 0xD0814866:
    case 0xD0A14867:
    case 0xD1614828:
    case 0xD8814829:
    case 0xD8A14884:
    case 0xD9614822:
        return 1;
    case 0xD0A14830:
    case 0xD82148A4:
    case 0xD861482C:
    case 0xD8A14981:
        return 2;
    case 0xD8A14868:
    case 0xD8A1486A:
    case 0xD8A1486B:
    case 0xD8A1486C:
    case 0xD8A1486D:
    case 0xD8A148C1:
    case 0xD8A148C3:
    case 0xD8A148E0:
    case 0xD8A148E1:
    case 0xD8A148E5:
    case 0xD8A148E7:
    case 0xD8A148E8:
    case 0xD8A148E9:
    case 0xD8A148EA:
    case 0xD8A148EB:
    case 0xD8A148EC:
    case 0xD8A148ED:
    case 0xD8A148EE:
    case 0xD8A148EF:
    case 0xD8A148F0:
    case 0xD8A148F1:
    case 0xD8A148F2:
    case 0xD8A148F3:
    case 0xD8A148F4:
    case 0xD8A148F5:
    case 0xD8A148F6:
    case 0xD8A148F8:
    case 0xD8A148F9:
    case 0xD8A148FA:
    case 0xD8A148FB:
    case 0xD8A148FC:
    case 0xD8A148FD:
    case 0xD8A14900:
    case 0xD8A14901:
    case 0xD8A14902:
    case 0xD8A14904:
    case 0xD9214824:
    case 0xD941482D:
        return 3;
    case 0xD8A148F7:
    case 0xD8A14903:
        return 4;
    case 0xD9414869:
        return 5;
    case 0xD8A148FE:
    case 0xD8A14905:
        return 6;
    case 0xD8A14825:
        return 7;
    case 0xD8A148E4:
    case 0xF8A148C4:
        return 8;
    case 0xD82148AA:
    case 0xD8A1482E:
    case 0xD8A14832:
    case 0xD8A14883:
    case 0xD8A148E6:
    case 0xD8A14906:
    case 0xE0614820:
    case 0xE0A14821:
    case 0xE0A14827:
    case 0xE0A1482B:
    case 0xE0E14823:
    case 0xE0E14826:
    case 0xE1614840:
        return 9;
    default:
        return 0;
    }
}

// the error code shown for a result: 0 on success, 018-xxxx for those of pia, the code of the
// server for 0xF8A148C4 (NexMatchmakeSession::s_NetworkErrorCode)
// 0x00413DC8 (name is ours)
u32 GetErrorCode(const nn::Result& result)
{
    if (result.IsSuccess()) {
        return 0;
    }
    if (!IsPiaResult(result)) {
        return ERROR_CODE_BASE;
    }
    bit32 value = result.GetPrintableBits();
    if (value == 0xF8A148C4) {
        if (inet::NexMatchmakeSession::s_NetworkErrorCode != 0) {
            return inet::NexMatchmakeSession::s_NetworkErrorCode;
        }
        return ERROR_CODE_BASE;
    } else if (value == 0xE0614820) {
        return ERROR_CODE_BASE + 1;
    } else if (value == 0xE0A14821) {
        return ERROR_CODE_BASE + 2;
    } else if (value == 0xE0E14823) {
        return ERROR_CODE_BASE + 3;
    } else if (value == 0xD9614822) {
        return ERROR_CODE_BASE + 4;
    } else if (value == 0xD9214824) {
        return ERROR_CODE_BASE + 5;
    } else if (value == 0xD8A14825) {
        return ERROR_CODE_BASE + 6;
    } else if (value == 0xE0E14826) {
        return ERROR_CODE_BASE + 7;
    } else if (value == 0xE0A14827) {
        return ERROR_CODE_BASE + 8;
    } else if (value == 0xD1614828) {
        return ERROR_CODE_BASE + 9;
    } else if (value == 0xD8814829) {
        return ERROR_CODE_BASE + 10;
    } else if (value == 0xE161482A) {
        return ERROR_CODE_BASE + 11;
    } else if (value == 0xE0A1482B) {
        return ERROR_CODE_BASE + 12;
    } else if (value == 0xD861482C) {
        return ERROR_CODE_BASE + 13;
    } else if (value == 0xD941482D) {
        return ERROR_CODE_BASE + 14;
    } else if (value == 0xD8A1482E) {
        return ERROR_CODE_BASE + 15;
    } else if (value == 0xD8A1482F) {
        return ERROR_CODE_BASE + 16;
    } else if (value == 0xD0A14830) {
        return ERROR_CODE_BASE + 17;
    } else if (value == 0xC8A14831) {
        return ERROR_CODE_BASE + 18;
    } else if (value == 0xD8A14832) {
        return ERROR_CODE_BASE + 19;
    } else if (value == 0xE1614840) {
        return ERROR_CODE_BASE + 101;
    } else if (value == 0xD8A148C1) {
        return ERROR_CODE_BASE + 201;
    } else if (value == 0xD8A148C2) {
        return ERROR_CODE_BASE + 202;
    } else if (value == 0xD8A148C3) {
        return ERROR_CODE_BASE + 203;
    } else if (value == 0xD0814864) {
        return ERROR_CODE_BASE + 301;
    } else if (value == 0xD8814865) {
        return ERROR_CODE_BASE + 302;
    } else if (value == 0xD0814866) {
        return ERROR_CODE_BASE + 303;
    } else if (value == 0xD0A14867) {
        return ERROR_CODE_BASE + 304;
    } else if (value == 0xD8A14868) {
        return ERROR_CODE_BASE + 305;
    } else if (value == 0xD9414869) {
        return ERROR_CODE_BASE + 306;
    } else if (value == 0xD8A1486A) {
        return ERROR_CODE_BASE + 307;
    } else if (value == 0xD8A1486B) {
        return ERROR_CODE_BASE + 308;
    } else if (value == 0xD8A1486C) {
        return ERROR_CODE_BASE + 309;
    } else if (value == 0xD8A1486D) {
        return ERROR_CODE_BASE + 310;
    } else if (value == 0xD9614881) {
        return ERROR_CODE_BASE + 401;
    } else if (value == 0xD9614882) {
        return ERROR_CODE_BASE + 402;
    } else if (value == 0xD8A14883) {
        return ERROR_CODE_BASE + 403;
    } else if (value == 0xD8A14884) {
        return ERROR_CODE_BASE + 404;
    } else if (value == 0xD8A14885) {
        return ERROR_CODE_BASE + 405;
    } else if (value == 0xD8A148E0) {
        return ERROR_CODE_BASE + 501;
    } else if (value == 0xD8A148E1) {
        return ERROR_CODE_BASE + 502;
    } else if (value == 0xD8A148E2) {
        return ERROR_CODE_BASE + 503;
    } else if (value == 0xD8A148E3) {
        return ERROR_CODE_BASE + 504;
    } else if (value == 0xD8A148E4) {
        return ERROR_CODE_BASE + 505;
    } else if (value == 0xD8A148E5) {
        return ERROR_CODE_BASE + 506;
    } else if (value == 0xD8A148E6) {
        return ERROR_CODE_BASE + 507;
    } else if (value == 0xD8A148E7) {
        return ERROR_CODE_BASE + 510;
    } else if (value == 0xD8A148E8) {
        return ERROR_CODE_BASE + 511;
    } else if (value == 0xD8A148E9) {
        return ERROR_CODE_BASE + 512;
    } else if (value == 0xD8A148EA) {
        return ERROR_CODE_BASE + 513;
    } else if (value == 0xD8A148ED) {
        return ERROR_CODE_BASE + 514;
    } else if (value == 0xD8A148EB) {
        return ERROR_CODE_BASE + 515;
    } else if (value == 0xD8A148EC) {
        return ERROR_CODE_BASE + 516;
    } else if (value == 0xD8A148EE) {
        return ERROR_CODE_BASE + 517;
    } else if (value == 0xD8A148EF) {
        return ERROR_CODE_BASE + 518;
    } else if (value == 0xD8A148F0) {
        return ERROR_CODE_BASE + 519;
    } else if (value == 0xD8A148F1) {
        return ERROR_CODE_BASE + 520;
    } else if (value == 0xD8A148F2) {
        return ERROR_CODE_BASE + 521;
    } else if (value == 0xD8A148F3) {
        return ERROR_CODE_BASE + 541;
    } else if (value == 0xD8A148F4) {
        return ERROR_CODE_BASE + 542;
    } else if (value == 0xD8A148F5) {
        return ERROR_CODE_BASE + 543;
    } else if (value == 0xD8A148F6) {
        return ERROR_CODE_BASE + 544;
    } else if (value == 0xD8A148F9) {
        return ERROR_CODE_BASE + 545;
    } else if (value == 0xD8A148FA) {
        return ERROR_CODE_BASE + 546;
    } else if (value == 0xD8A148F8) {
        return ERROR_CODE_BASE + 547;
    } else if (value == 0xD8A148FB) {
        return ERROR_CODE_BASE + 548;
    } else if (value == 0xD8A148FC) {
        return ERROR_CODE_BASE + 549;
    } else if (value == 0xD8A148FD) {
        return ERROR_CODE_BASE + 550;
    } else if (value == 0xD8A148FE) {
        return ERROR_CODE_BASE + 551;
    } else if (value == 0xD8A14900) {
        return ERROR_CODE_BASE + 552;
    } else if (value == 0xD8A14901) {
        return ERROR_CODE_BASE + 553;
    } else if (value == 0xD8A14902) {
        return ERROR_CODE_BASE + 554;
    } else if (value == 0xD8A14904) {
        return ERROR_CODE_BASE + 555;
    } else if (value == 0xD8A14905) {
        return ERROR_CODE_BASE + 556;
    } else if (value == 0xD8A14906) {
        return ERROR_CODE_BASE + 557;
    } else if (value == 0xD8A148F7) {
        return ERROR_CODE_BASE + 561;
    } else if (value == 0xD8A14903) {
        return ERROR_CODE_BASE + 562;
    } else if (value == 0xD82148A4) {
        return ERROR_CODE_BASE + 602;
    } else if (value == 0xD82148AA) {
        return ERROR_CODE_BASE + 606;
    } else if (value == 0xD8A14981) {
        return ERROR_CODE_BASE + 801;
    }
    return ERROR_CODE_BASE;
}
} // namespace util
} // namespace pia
} // namespace nn
