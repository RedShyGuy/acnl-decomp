#include "script/dToUpper.h"

// The table of ToUpper (its own file: GCC folds the values in the loop otherwise; name of the file is ours).

namespace script {
// the Greek letters with accents, Latin, Cyrillic yo
// 0x0089175C
extern const CasePair s_CasePairs[15] = {
    {0x03AC, 0x0386},
    {0x03AD, 0x0388},
    {0x03AE, 0x0389},
    {0x03AF, 0x038A},
    {0x03CC, 0x038C},
    {0x03CD, 0x038E},
    {0x03CE, 0x038F},
    {0x03CA, 0x03AA},
    {0x03CB, 0x03AB},
    {0x00FD, 0x00DD},
    {0x00FF, 0x0178},
    {0x01C6, 0x01C5},
    {0x01F3, 0x01F2},
    {0x021B, 0x021A},
    {0x0451, 0x0401},
};
} // namespace script
