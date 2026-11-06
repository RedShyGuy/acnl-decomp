#include "Sound/dSoundDoubutsugoMgr.h"
#include "Sound/dSoundLabel.h"

// the pairs of letters of the languages (generated from the original by emulating it for all pairs of characters)

// 0x00721904
u32 SoundDoubutsugoMgr::GetEnglishSound(u32 c, u32 next) const
{
    switch (c) {
        case 'a':
            switch (next) {
                case 'c':
                    return SE_DOU_BOY_000_A;
                case 'i':
                case 'y':
                    return SE_DOU_BOY_600_AI;
                case 'n':
                    return SE_DOU_BOY_601_AN;
            }
            return 0;
        case 'b':
            switch (next) {
                case 'e':
                    return SE_DOU_BOY_253_BE;
                case 'i':
                    return SE_DOU_BOY_251_BI;
                case 'o':
                    return SE_DOU_BOY_254_BO;
                case 'u':
                    return SE_DOU_BOY_252_BU;
            }
            return 0;
        case 'c':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_010_KA;
                case 'e':
                    return SE_DOU_BOY_023_SE;
                case 'i':
                    return SE_DOU_BOY_603_CI;
                case 'o':
                    return SE_DOU_BOY_014_KO;
                case 'u':
                    return SE_DOU_BOY_012_KU;
            }
            return 0;
        case 'd':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_230_DA;
                case 'i':
                    return SE_DOU_BOY_231_DI;
                case 'o':
                    return SE_DOU_BOY_234_DO;
            }
            return 0;
        case 'e':
            switch (next) {
                case 'e':
                    return SE_DOU_BOY_604_EE;
                case 'f':
                    return SE_DOU_BOY_605_EF;
                case 'h':
                    return SE_DOU_BOY_003_E;
                case 'i':
                    return SE_DOU_BOY_606_EI;
                case 'm':
                    return SE_DOU_BOY_607_EM;
                case 'n':
                    return SE_DOU_BOY_608_EN;
                case 'r':
                    return SE_DOU_BOY_000_A;
                case 's':
                    return SE_DOU_BOY_609_ES;
            }
            return 0;
        case 'f':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_050_HA;
                case 'e':
                    return SE_DOU_BOY_053_HE;
                case 'i':
                    return SE_DOU_BOY_051_HI;
                case 'o':
                    return SE_DOU_BOY_054_HO;
                case 'u':
                    return SE_DOU_BOY_052_HU;
            }
            return 0;
        case 'g':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_210_GA;
                case 'e':
                    return SE_DOU_BOY_213_GE;
                case 'i':
                    return SE_DOU_BOY_211_GI;
                case 'n':
                    return SE_DOU_BOY_105_NN;
                case 'o':
                    return SE_DOU_BOY_214_GO;
                case 'u':
                    return SE_DOU_BOY_212_GU;
                case 'y':
                    return SE_DOU_BOY_610_GY;
            }
            return 0;
        case 'h':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_050_HA;
                case 'e':
                    return SE_DOU_BOY_053_HE;
                case 'i':
                    return SE_DOU_BOY_051_HI;
                case 'o':
                    return SE_DOU_BOY_054_HO;
                case 'u':
                    return SE_DOU_BOY_052_HU;
            }
            return 0;
        case 'i':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_070_YA;
                case 'e':
                    return SE_DOU_BOY_001_I;
                case 'o':
                    return SE_DOU_BOY_074_YO;
                case 'u':
                    return SE_DOU_BOY_072_YU;
                case 'x':
                    return SE_DOU_BOY_611_IX;
            }
            return 0;
        case 'j':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_220_ZA;
                case 'e':
                    return SE_DOU_BOY_612_JE;
                case 'i':
                    return SE_DOU_BOY_221_ZI;
                case 'o':
                    return SE_DOU_BOY_224_ZO;
                case 'u':
                    return SE_DOU_BOY_222_ZU;
            }
            return 0;
        case 'k':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_010_KA;
                case 'e':
                    return SE_DOU_BOY_013_KE;
                case 'i':
                    return SE_DOU_BOY_011_KI;
                case 'o':
                    return SE_DOU_BOY_014_KO;
                case 'u':
                    return SE_DOU_BOY_012_KU;
            }
            return 0;
        case 'l':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_080_RA;
                case 'e':
                    return SE_DOU_BOY_083_RE;
                case 'i':
                    return SE_DOU_BOY_081_RI;
                case 'o':
                    return SE_DOU_BOY_084_RO;
                case 'u':
                    return SE_DOU_BOY_082_RU;
            }
            return 0;
        case 'm':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_060_MA;
                case 'e':
                    return SE_DOU_BOY_063_ME;
                case 'i':
                    return SE_DOU_BOY_061_MI;
                case 'o':
                    return SE_DOU_BOY_064_MO;
                case 'u':
                    return SE_DOU_BOY_062_MU;
            }
            return 0;
        case 'n':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_040_NA;
                case 'e':
                    return SE_DOU_BOY_043_NE;
                case 'i':
                    return SE_DOU_BOY_041_NI;
                case 'o':
                    return SE_DOU_BOY_044_NO;
                case 'u':
                    return SE_DOU_BOY_042_NU;
            }
            return 0;
        case 'o':
            switch (next) {
                case 'o':
                    return SE_DOU_BOY_613_OO;
            }
            return 0;
        case 'p':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_350_PA;
                case 'e':
                    return SE_DOU_BOY_353_PE;
                case 'h':
                    return SE_DOU_BOY_052_HU;
                case 'i':
                    return SE_DOU_BOY_351_PI;
                case 'o':
                    return SE_DOU_BOY_354_PO;
                case 'u':
                    return SE_DOU_BOY_352_PU;
            }
            return 0;
        case 'q':
            switch (next) {
                case 'u':
                    return SE_DOU_BOY_614_QU;
            }
            return 0;
        case 'r':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_080_RA;
                case 'e':
                    return SE_DOU_BOY_083_RE;
                case 'i':
                    return SE_DOU_BOY_081_RI;
                case 'o':
                    return SE_DOU_BOY_084_RO;
                case 'u':
                    return SE_DOU_BOY_082_RU;
                case 'y':
                    return SE_DOU_BOY_615_RY;
            }
            return 0;
        case 's':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_020_SA;
                case 'e':
                    return SE_DOU_BOY_023_SE;
                case 'i':
                    return SE_DOU_BOY_021_SI;
                case 'o':
                    return SE_DOU_BOY_024_SO;
                case 'u':
                    return SE_DOU_BOY_022_SU;
            }
            return 0;
        case 't':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_030_TA;
                case 'e':
                    return SE_DOU_BOY_033_TE;
                case 'i':
                    return SE_DOU_BOY_031_TI;
                case 'o':
                    return SE_DOU_BOY_034_TO;
                case 'u':
                    return SE_DOU_BOY_032_TU;
                case 'y':
                    return SE_DOU_BOY_617_TY;
            }
            return 0;
        case 'u':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_090_WA;
                case 'u':
                    return SE_DOU_BOY_002_U;
            }
            return 0;
        case 'v':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_250_BA;
                case 'e':
                    return SE_DOU_BOY_253_BE;
                case 'i':
                    return SE_DOU_BOY_618_VI;
                case 'o':
                    return SE_DOU_BOY_254_BO;
                case 'u':
                    return SE_DOU_BOY_252_BU;
            }
            return 0;
        case 'w':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_090_WA;
                case 'e':
                    return SE_DOU_BOY_003_E;
                case 'i':
                    return SE_DOU_BOY_001_I;
                case 'o':
                    return SE_DOU_BOY_004_O;
                case 'u':
                    return SE_DOU_BOY_002_U;
            }
            return 0;
        case 'x':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_220_ZA;
                case 'e':
                    return SE_DOU_BOY_223_ZE;
                case 'u':
                    return SE_DOU_BOY_012_KU;
            }
            return 0;
        case 'y':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_070_YA;
                case 'o':
                    return SE_DOU_BOY_074_YO;
                case 'u':
                    return SE_DOU_BOY_072_YU;
            }
            return 0;
        case 'z':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_220_ZA;
                case 'e':
                    return SE_DOU_BOY_223_ZE;
                case 'i':
                    return SE_DOU_BOY_221_ZI;
                case 'o':
                    return SE_DOU_BOY_224_ZO;
                case 'u':
                    return SE_DOU_BOY_222_ZU;
            }
            return 0;
    }
    return 0;
}

// 0x007228FC
u32 SoundDoubutsugoMgr::GetGermanSound(u32 c, u32 next) const
{
    switch (c) {
        case 'a':
            switch (next) {
                case 'i':
                case 'y':
                    return SE_DOU_BOY_600_AI;
                case 'n':
                    return SE_DOU_BOY_601_AN;
            }
            return 0;
        case 'b':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_250_BA;
                case 'e':
                    return SE_DOU_BOY_253_BE;
                case 'i':
                    return SE_DOU_BOY_251_BI;
                case 'o':
                    return SE_DOU_BOY_254_BO;
                case 'u':
                    return SE_DOU_BOY_252_BU;
            }
            return 0;
        case 'c':
            switch (next) {
                case 'h':
                    return SE_DOU_BOY_051_HI;
                case 'i':
                    return SE_DOU_BOY_603_CI;
                case 'k':
                    return SE_DOU_BOY_012_KU;
            }
            return 0;
        case 'd':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_230_DA;
                case 'e':
                    return SE_DOU_BOY_233_DE;
                case 'i':
                    return SE_DOU_BOY_231_DI;
                case 'o':
                    return SE_DOU_BOY_234_DO;
                case 's':
                    return SE_DOU_BOY_032_TU;
                case 't':
                    return SE_DOU_BOY_034_TO;
            }
            return 0;
        case 'e':
            switch (next) {
                case 'e':
                    return SE_DOU_BOY_604_EE;
                case 'f':
                    return SE_DOU_BOY_605_EF;
                case 'i':
                case 'y':
                    return SE_DOU_BOY_600_AI;
                case 'm':
                    return SE_DOU_BOY_607_EM;
                case 'n':
                    return SE_DOU_BOY_608_EN;
                case 's':
                    return SE_DOU_BOY_609_ES;
            }
            return 0;
        case 'f':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_050_HA;
                case 'e':
                    return SE_DOU_BOY_053_HE;
                case 'i':
                    return SE_DOU_BOY_051_HI;
                case 'o':
                    return SE_DOU_BOY_054_HO;
                case 'u':
                    return SE_DOU_BOY_052_HU;
            }
            return 0;
        case 'g':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_210_GA;
                case 'e':
                    return SE_DOU_BOY_213_GE;
                case 'i':
                    return SE_DOU_BOY_211_GI;
                case 'o':
                    return SE_DOU_BOY_214_GO;
                case 'u':
                    return SE_DOU_BOY_212_GU;
                case 'y':
                    return SE_DOU_BOY_610_GY;
            }
            return 0;
        case 'h':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_050_HA;
                case 'e':
                    return SE_DOU_BOY_053_HE;
                case 'i':
                    return SE_DOU_BOY_051_HI;
                case 'o':
                    return SE_DOU_BOY_054_HO;
                case 'u':
                    return SE_DOU_BOY_052_HU;
            }
            return 0;
        case 'i':
            switch (next) {
                case 'e':
                    return SE_DOU_BOY_001_I;
                case 'x':
                    return SE_DOU_BOY_611_IX;
            }
            return 0;
        case 'j':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_070_YA;
                case 'e':
                    return SE_DOU_BOY_612_JE;
                case 'i':
                    return SE_DOU_BOY_221_ZI;
                case 'o':
                    return SE_DOU_BOY_074_YO;
                case 'u':
                    return SE_DOU_BOY_072_YU;
            }
            return 0;
        case 'k':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_010_KA;
                case 'e':
                    return SE_DOU_BOY_013_KE;
                case 'i':
                    return SE_DOU_BOY_011_KI;
                case 'o':
                    return SE_DOU_BOY_014_KO;
                case 'u':
                    return SE_DOU_BOY_012_KU;
            }
            return 0;
        case 'l':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_080_RA;
                case 'e':
                    return SE_DOU_BOY_083_RE;
                case 'i':
                    return SE_DOU_BOY_081_RI;
                case 'o':
                    return SE_DOU_BOY_084_RO;
                case 'u':
                    return SE_DOU_BOY_082_RU;
            }
            return 0;
        case 'm':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_060_MA;
                case 'e':
                    return SE_DOU_BOY_063_ME;
                case 'i':
                    return SE_DOU_BOY_061_MI;
                case 'o':
                    return SE_DOU_BOY_064_MO;
                case 'u':
                    return SE_DOU_BOY_062_MU;
            }
            return 0;
        case 'n':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_040_NA;
                case 'e':
                    return SE_DOU_BOY_043_NE;
                case 'i':
                    return SE_DOU_BOY_041_NI;
                case 'o':
                    return SE_DOU_BOY_044_NO;
                case 'u':
                    return SE_DOU_BOY_042_NU;
            }
            return 0;
        case 'o':
            switch (next) {
                case 'h':
                case 'o':
                    return SE_DOU_BOY_613_OO;
            }
            return 0;
        case 'p':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_350_PA;
                case 'e':
                    return SE_DOU_BOY_353_PE;
                case 'i':
                    return SE_DOU_BOY_351_PI;
                case 'o':
                    return SE_DOU_BOY_354_PO;
                case 'u':
                    return SE_DOU_BOY_352_PU;
            }
            return 0;
        case 'q':
            switch (next) {
                case 'u':
                    return SE_DOU_BOY_614_QU;
            }
            return 0;
        case 'r':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_080_RA;
                case 'e':
                    return SE_DOU_BOY_083_RE;
                case 'i':
                    return SE_DOU_BOY_081_RI;
                case 'o':
                    return SE_DOU_BOY_084_RO;
                case 'u':
                    return SE_DOU_BOY_082_RU;
                case 'y':
                    return SE_DOU_BOY_615_RY;
            }
            return 0;
        case 's':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_220_ZA;
                case 'e':
                    return SE_DOU_BOY_223_ZE;
                case 'i':
                    return SE_DOU_BOY_221_ZI;
                case 'o':
                    return SE_DOU_BOY_224_ZO;
                case 'u':
                    return SE_DOU_BOY_222_ZU;
            }
            return 0;
        case 't':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_030_TA;
                case 'e':
                    return SE_DOU_BOY_033_TE;
                case 'i':
                    return SE_DOU_BOY_031_TI;
                case 'o':
                    return SE_DOU_BOY_034_TO;
                case 's':
                case 'u':
                case 'z':
                    return SE_DOU_BOY_032_TU;
                case 'y':
                    return SE_DOU_BOY_617_TY;
            }
            return 0;
        case 'u':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_090_WA;
            }
            return 0;
        case 'v':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_050_HA;
                case 'i':
                    return SE_DOU_BOY_618_VI;
                case 'o':
                    return SE_DOU_BOY_054_HO;
            }
            return 0;
        case 'w':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_090_WA;
                case 'i':
                    return SE_DOU_BOY_618_VI;
                case 'o':
                    return SE_DOU_BOY_094_WO;
            }
            return 0;
        case 'y':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_070_YA;
                case 'o':
                    return SE_DOU_BOY_074_YO;
                case 'u':
                    return SE_DOU_BOY_072_YU;
            }
            return 0;
        case 'z':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_220_ZA;
                case 'e':
                    return SE_DOU_BOY_223_ZE;
                case 'i':
                    return SE_DOU_BOY_221_ZI;
                case 'o':
                    return SE_DOU_BOY_224_ZO;
                case 'u':
                    return SE_DOU_BOY_222_ZU;
            }
            return 0;
    }
    return 0;
}

// 0x00722160
u32 SoundDoubutsugoMgr::GetFrenchSound(u32 c, u32 next) const
{
    switch (c) {
        case 'a':
            switch (next) {
                case 'i':
                    return SE_DOU_BOY_003_E;
                case 'n':
                    return SE_DOU_BOY_601_AN;
                case 'u':
                    return SE_DOU_BOY_004_O;
            }
            return 0;
        case 'b':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_253_BE;
                case 'i':
                    return SE_DOU_BOY_251_BI;
                case 'o':
                    return SE_DOU_BOY_254_BO;
                case 'u':
                    return SE_DOU_BOY_252_BU;
            }
            return 0;
        case 'c':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_013_KE;
                case 'e':
                    return SE_DOU_BOY_024_SO;
                case 'h':
                    return SE_DOU_BOY_602_CH;
                case 'i':
                    return SE_DOU_BOY_011_KI;
                case 'o':
                    return SE_DOU_BOY_014_KO;
                case 'u':
                    return SE_DOU_BOY_012_KU;
            }
            return 0;
        case 'd':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_233_DE;
                case 'e':
                    return SE_DOU_BOY_230_DA;
                case 'i':
                    return SE_DOU_BOY_231_DI;
                case 'o':
                    return SE_DOU_BOY_234_DO;
            }
            return 0;
        case 'e':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_004_O;
                case 'e':
                    return SE_DOU_BOY_604_EE;
                case 'f':
                    return SE_DOU_BOY_605_EF;
                case 'i':
                    return SE_DOU_BOY_003_E;
                case 'm':
                    return SE_DOU_BOY_607_EM;
                case 'n':
                    return SE_DOU_BOY_608_EN;
                case 's':
                    return SE_DOU_BOY_609_ES;
                case 'u':
                    return SE_DOU_BOY_002_U;
            }
            return 0;
        case 'f':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_050_HA;
                case 'e':
                    return SE_DOU_BOY_053_HE;
                case 'i':
                    return SE_DOU_BOY_051_HI;
                case 'o':
                    return SE_DOU_BOY_054_HO;
                case 'u':
                    return SE_DOU_BOY_052_HU;
            }
            return 0;
        case 'g':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_213_GE;
                case 'e':
                    return SE_DOU_BOY_210_GA;
                case 'i':
                    return SE_DOU_BOY_211_GI;
                case 'n':
                    return SE_DOU_BOY_042_NU;
                case 'o':
                    return SE_DOU_BOY_214_GO;
                case 'u':
                    return SE_DOU_BOY_212_GU;
                case 'y':
                    return SE_DOU_BOY_610_GY;
            }
            return 0;
        case 'i':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_070_YA;
                case 'm':
                    return SE_DOU_BOY_000_A;
                case 'o':
                    return SE_DOU_BOY_074_YO;
                case 'u':
                    return SE_DOU_BOY_072_YU;
                case 'x':
                    return SE_DOU_BOY_611_IX;
            }
            return 0;
        case 'j':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_223_ZE;
                case 'e':
                    return SE_DOU_BOY_612_JE;
                case 'i':
                    return SE_DOU_BOY_221_ZI;
                case 'o':
                    return SE_DOU_BOY_224_ZO;
                case 'u':
                    return SE_DOU_BOY_222_ZU;
            }
            return 0;
        case 'k':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_010_KA;
            }
            return 0;
        case 'l':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_080_RA;
                case 'e':
                    return SE_DOU_BOY_083_RE;
                case 'i':
                    return SE_DOU_BOY_081_RI;
                case 'o':
                    return SE_DOU_BOY_084_RO;
                case 'u':
                    return SE_DOU_BOY_082_RU;
            }
            return 0;
        case 'm':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_063_ME;
                case 'e':
                    return SE_DOU_BOY_060_MA;
                case 'i':
                    return SE_DOU_BOY_061_MI;
                case 'o':
                    return SE_DOU_BOY_064_MO;
                case 'u':
                    return SE_DOU_BOY_062_MU;
            }
            return 0;
        case 'n':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_043_NE;
                case 'e':
                    return SE_DOU_BOY_040_NA;
                case 'i':
                    return SE_DOU_BOY_041_NI;
                case 'o':
                    return SE_DOU_BOY_044_NO;
                case 'u':
                    return SE_DOU_BOY_042_NU;
            }
            return 0;
        case 'o':
            switch (next) {
                case 'e':
                case 'u':
                    return SE_DOU_BOY_002_U;
                case 'h':
                case 'o':
                    return SE_DOU_BOY_613_OO;
                case 'i':
                    return SE_DOU_BOY_090_WA;
                case 'm':
                    return SE_DOU_BOY_105_NN;
            }
            return 0;
        case 'p':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_353_PE;
                case 'e':
                    return SE_DOU_BOY_350_PA;
                case 'h':
                    return SE_DOU_BOY_052_HU;
                case 'i':
                    return SE_DOU_BOY_351_PI;
                case 'o':
                    return SE_DOU_BOY_354_PO;
                case 'u':
                    return SE_DOU_BOY_352_PU;
            }
            return 0;
        case 'q':
            switch (next) {
                case 'u':
                    return SE_DOU_BOY_614_QU;
            }
            return 0;
        case 'r':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_080_RA;
                case 'e':
                case 'u':
                    return SE_DOU_BOY_082_RU;
                case 'i':
                    return SE_DOU_BOY_081_RI;
                case 'o':
                    return SE_DOU_BOY_084_RO;
                case 'y':
                    return SE_DOU_BOY_615_RY;
            }
            return 0;
        case 's':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_020_SA;
                case 'e':
                    return SE_DOU_BOY_023_SE;
                case 'i':
                    return SE_DOU_BOY_021_SI;
                case 'u':
                    return SE_DOU_BOY_022_SU;
            }
            return 0;
        case 't':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_033_TE;
                case 'e':
                    return SE_DOU_BOY_030_TA;
                case 'i':
                    return SE_DOU_BOY_031_TI;
                case 'o':
                    return SE_DOU_BOY_034_TO;
                case 'u':
                    return SE_DOU_BOY_032_TU;
                case 'y':
                    return SE_DOU_BOY_617_TY;
            }
            return 0;
        case 'u':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_090_WA;
                case 'i':
                    return SE_DOU_BOY_001_I;
            }
            return 0;
        case 'v':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_253_BE;
                case 'e':
                    return SE_DOU_BOY_250_BA;
                case 'i':
                    return SE_DOU_BOY_618_VI;
                case 'o':
                    return SE_DOU_BOY_254_BO;
                case 'u':
                    return SE_DOU_BOY_252_BU;
            }
            return 0;
        case 'w':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_090_WA;
            }
            return 0;
        case 'y':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_070_YA;
                case 'o':
                    return SE_DOU_BOY_074_YO;
                case 'u':
                    return SE_DOU_BOY_072_YU;
            }
            return 0;
        case 'z':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_223_ZE;
                case 'e':
                    return SE_DOU_BOY_220_ZA;
                case 'i':
                    return SE_DOU_BOY_221_ZI;
                case 'o':
                    return SE_DOU_BOY_224_ZO;
                case 'u':
                    return SE_DOU_BOY_222_ZU;
            }
            return 0;
    }
    return 0;
}

// 0x00723078
u32 SoundDoubutsugoMgr::GetItalianSound(u32 c, u32 next) const
{
    switch (c) {
        case 'a':
            switch (next) {
                case 'i':
                    return SE_DOU_BOY_600_AI;
                case 'n':
                    return SE_DOU_BOY_601_AN;
            }
            return 0;
        case 'b':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_250_BA;
                case 'e':
                    return SE_DOU_BOY_253_BE;
                case 'i':
                    return SE_DOU_BOY_251_BI;
                case 'o':
                    return SE_DOU_BOY_254_BO;
                case 'u':
                    return SE_DOU_BOY_252_BU;
            }
            return 0;
        case 'c':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_010_KA;
                case 'h':
                case 'u':
                    return SE_DOU_BOY_012_KU;
                case 'i':
                    return SE_DOU_BOY_603_CI;
                case 'o':
                    return SE_DOU_BOY_014_KO;
            }
            return 0;
        case 'd':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_230_DA;
                case 'e':
                    return SE_DOU_BOY_233_DE;
                case 'i':
                    return SE_DOU_BOY_231_DI;
                case 'o':
                    return SE_DOU_BOY_234_DO;
            }
            return 0;
        case 'e':
            switch (next) {
                case 'e':
                    return SE_DOU_BOY_604_EE;
                case 'f':
                    return SE_DOU_BOY_605_EF;
                case 'i':
                    return SE_DOU_BOY_600_AI;
                case 'm':
                    return SE_DOU_BOY_607_EM;
                case 'n':
                    return SE_DOU_BOY_608_EN;
                case 's':
                    return SE_DOU_BOY_609_ES;
            }
            return 0;
        case 'f':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_050_HA;
                case 'e':
                    return SE_DOU_BOY_053_HE;
                case 'i':
                    return SE_DOU_BOY_051_HI;
                case 'o':
                    return SE_DOU_BOY_054_HO;
                case 'u':
                    return SE_DOU_BOY_052_HU;
            }
            return 0;
        case 'g':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_210_GA;
                case 'e':
                    return SE_DOU_BOY_053_HE;
                case 'i':
                    return SE_DOU_BOY_051_HI;
                case 'o':
                    return SE_DOU_BOY_214_GO;
                case 'u':
                    return SE_DOU_BOY_212_GU;
                case 'y':
                    return SE_DOU_BOY_610_GY;
            }
            return 0;
        case 'h':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_000_A;
                case 'e':
                    return SE_DOU_BOY_003_E;
                case 'i':
                    return SE_DOU_BOY_001_I;
                case 'o':
                    return SE_DOU_BOY_004_O;
                case 'u':
                    return SE_DOU_BOY_002_U;
            }
            return 0;
        case 'i':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_070_YA;
                case 'o':
                    return SE_DOU_BOY_074_YO;
                case 'u':
                    return SE_DOU_BOY_072_YU;
                case 'x':
                    return SE_DOU_BOY_611_IX;
            }
            return 0;
        case 'j':
            switch (next) {
                case 'e':
                    return SE_DOU_BOY_053_HE;
                case 'i':
                    return SE_DOU_BOY_051_HI;
                case 'u':
                    return SE_DOU_BOY_052_HU;
            }
            return 0;
        case 'k':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_010_KA;
                case 'e':
                    return SE_DOU_BOY_013_KE;
                case 'i':
                    return SE_DOU_BOY_011_KI;
                case 'o':
                    return SE_DOU_BOY_014_KO;
                case 'u':
                    return SE_DOU_BOY_012_KU;
            }
            return 0;
        case 'l':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_080_RA;
                case 'e':
                    return SE_DOU_BOY_083_RE;
                case 'i':
                    return SE_DOU_BOY_081_RI;
                case 'o':
                    return SE_DOU_BOY_084_RO;
                case 'u':
                    return SE_DOU_BOY_082_RU;
            }
            return 0;
        case 'm':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_060_MA;
                case 'e':
                    return SE_DOU_BOY_063_ME;
                case 'i':
                    return SE_DOU_BOY_061_MI;
                case 'o':
                    return SE_DOU_BOY_064_MO;
                case 'u':
                    return SE_DOU_BOY_062_MU;
            }
            return 0;
        case 'n':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_040_NA;
                case 'e':
                    return SE_DOU_BOY_043_NE;
                case 'i':
                    return SE_DOU_BOY_041_NI;
                case 'o':
                    return SE_DOU_BOY_044_NO;
                case 'u':
                    return SE_DOU_BOY_042_NU;
            }
            return 0;
        case 'o':
            switch (next) {
                case 'h':
                case 'o':
                    return SE_DOU_BOY_613_OO;
                case 'm':
                    return SE_DOU_BOY_105_NN;
            }
            return 0;
        case 'p':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_350_PA;
                case 'e':
                    return SE_DOU_BOY_353_PE;
                case 'i':
                    return SE_DOU_BOY_351_PI;
                case 'o':
                    return SE_DOU_BOY_354_PO;
                case 'u':
                    return SE_DOU_BOY_352_PU;
            }
            return 0;
        case 'q':
            switch (next) {
                case 'u':
                    return SE_DOU_BOY_614_QU;
            }
            return 0;
        case 'r':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_080_RA;
                case 'e':
                    return SE_DOU_BOY_083_RE;
                case 'i':
                    return SE_DOU_BOY_081_RI;
                case 'o':
                    return SE_DOU_BOY_084_RO;
                case 'u':
                    return SE_DOU_BOY_082_RU;
                case 'y':
                    return SE_DOU_BOY_615_RY;
            }
            return 0;
        case 's':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_020_SA;
                case 'e':
                    return SE_DOU_BOY_023_SE;
                case 'i':
                    return SE_DOU_BOY_021_SI;
                case 'o':
                    return SE_DOU_BOY_024_SO;
                case 'u':
                    return SE_DOU_BOY_022_SU;
            }
            return 0;
        case 't':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_030_TA;
                case 'e':
                    return SE_DOU_BOY_033_TE;
                case 'i':
                    return SE_DOU_BOY_031_TI;
                case 'o':
                    return SE_DOU_BOY_034_TO;
                case 'u':
                    return SE_DOU_BOY_032_TU;
                case 'y':
                    return SE_DOU_BOY_617_TY;
            }
            return 0;
        case 'u':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_090_WA;
            }
            return 0;
        case 'v':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_250_BA;
                case 'e':
                    return SE_DOU_BOY_253_BE;
                case 'i':
                    return SE_DOU_BOY_618_VI;
                case 'o':
                    return SE_DOU_BOY_254_BO;
                case 'u':
                    return SE_DOU_BOY_252_BU;
            }
            return 0;
        case 'w':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_090_WA;
            }
            return 0;
        case 'y':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_050_HA;
                case 'o':
                    return SE_DOU_BOY_054_HO;
            }
            return 0;
        case 'z':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_020_SA;
                case 'e':
                    return SE_DOU_BOY_023_SE;
                case 'i':
                    return SE_DOU_BOY_021_SI;
                case 'o':
                    return SE_DOU_BOY_024_SO;
                case 'u':
                    return SE_DOU_BOY_022_SU;
            }
            return 0;
    }
    return 0;
}

// 0x007237E4
u32 SoundDoubutsugoMgr::GetSpanishSound(u32 c, u32 next) const
{
    switch (c) {
        case 'a':
            switch (next) {
                case 'i':
                    return SE_DOU_BOY_600_AI;
                case 'n':
                    return SE_DOU_BOY_601_AN;
            }
            return 0;
        case 'b':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_250_BA;
                case 'e':
                    return SE_DOU_BOY_253_BE;
                case 'i':
                    return SE_DOU_BOY_251_BI;
                case 'o':
                    return SE_DOU_BOY_254_BO;
                case 'u':
                    return SE_DOU_BOY_252_BU;
            }
            return 0;
        case 'c':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_010_KA;
                case 'h':
                    return SE_DOU_BOY_602_CH;
                case 'i':
                    return SE_DOU_BOY_603_CI;
                case 'o':
                    return SE_DOU_BOY_014_KO;
                case 'u':
                    return SE_DOU_BOY_012_KU;
            }
            return 0;
        case 'd':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_230_DA;
                case 'e':
                    return SE_DOU_BOY_233_DE;
                case 'i':
                    return SE_DOU_BOY_231_DI;
                case 'o':
                    return SE_DOU_BOY_234_DO;
            }
            return 0;
        case 'e':
            switch (next) {
                case 'e':
                    return SE_DOU_BOY_604_EE;
                case 'f':
                    return SE_DOU_BOY_605_EF;
                case 'i':
                    return SE_DOU_BOY_606_EI;
                case 'm':
                    return SE_DOU_BOY_607_EM;
                case 'n':
                    return SE_DOU_BOY_608_EN;
                case 's':
                    return SE_DOU_BOY_609_ES;
            }
            return 0;
        case 'f':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_050_HA;
                case 'e':
                    return SE_DOU_BOY_053_HE;
                case 'i':
                    return SE_DOU_BOY_051_HI;
                case 'o':
                    return SE_DOU_BOY_054_HO;
                case 'u':
                    return SE_DOU_BOY_052_HU;
                case 'y':
                    return SE_DOU_BOY_610_GY;
            }
            return 0;
        case 'g':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_210_GA;
                case 'e':
                    return SE_DOU_BOY_053_HE;
                case 'i':
                    return SE_DOU_BOY_051_HI;
                case 'o':
                    return SE_DOU_BOY_214_GO;
                case 'u':
                    return SE_DOU_BOY_212_GU;
                case 'y':
                    return SE_DOU_BOY_610_GY;
            }
            return 0;
        case 'h':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_050_HA;
                case 'e':
                    return SE_DOU_BOY_053_HE;
                case 'i':
                    return SE_DOU_BOY_051_HI;
                case 'o':
                    return SE_DOU_BOY_214_GO;
                case 'u':
                    return SE_DOU_BOY_212_GU;
                case 'y':
                    return SE_DOU_BOY_610_GY;
            }
            return 0;
        case 'i':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_070_YA;
                case 'o':
                    return SE_DOU_BOY_074_YO;
                case 'u':
                    return SE_DOU_BOY_072_YU;
                case 'x':
                    return SE_DOU_BOY_611_IX;
            }
            return 0;
        case 'j':
            switch (next) {
                case 'e':
                    return SE_DOU_BOY_053_HE;
                case 'i':
                    return SE_DOU_BOY_051_HI;
                case 'u':
                    return SE_DOU_BOY_052_HU;
            }
            return 0;
        case 'k':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_010_KA;
                case 'e':
                    return SE_DOU_BOY_013_KE;
                case 'i':
                    return SE_DOU_BOY_011_KI;
                case 'o':
                    return SE_DOU_BOY_014_KO;
                case 'u':
                    return SE_DOU_BOY_012_KU;
            }
            return 0;
        case 'l':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_080_RA;
                case 'e':
                    return SE_DOU_BOY_083_RE;
                case 'i':
                    return SE_DOU_BOY_081_RI;
                case 'o':
                    return SE_DOU_BOY_084_RO;
                case 'u':
                    return SE_DOU_BOY_082_RU;
            }
            return 0;
        case 'm':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_060_MA;
                case 'e':
                    return SE_DOU_BOY_063_ME;
                case 'i':
                    return SE_DOU_BOY_061_MI;
                case 'o':
                    return SE_DOU_BOY_064_MO;
                case 'u':
                    return SE_DOU_BOY_062_MU;
            }
            return 0;
        case 'n':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_040_NA;
                case 'e':
                    return SE_DOU_BOY_043_NE;
                case 'i':
                    return SE_DOU_BOY_041_NI;
                case 'o':
                    return SE_DOU_BOY_044_NO;
                case 'u':
                    return SE_DOU_BOY_042_NU;
            }
            return 0;
        case 'o':
            switch (next) {
                case 'h':
                case 'o':
                    return SE_DOU_BOY_613_OO;
                case 'm':
                    return SE_DOU_BOY_105_NN;
            }
            return 0;
        case 'p':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_350_PA;
                case 'e':
                    return SE_DOU_BOY_353_PE;
                case 'i':
                    return SE_DOU_BOY_351_PI;
                case 'o':
                    return SE_DOU_BOY_354_PO;
                case 'u':
                    return SE_DOU_BOY_352_PU;
            }
            return 0;
        case 'q':
            switch (next) {
                case 'u':
                    return SE_DOU_BOY_614_QU;
            }
            return 0;
        case 'r':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_080_RA;
                case 'e':
                    return SE_DOU_BOY_083_RE;
                case 'i':
                    return SE_DOU_BOY_081_RI;
                case 'o':
                    return SE_DOU_BOY_084_RO;
                case 'u':
                    return SE_DOU_BOY_082_RU;
                case 'y':
                    return SE_DOU_BOY_615_RY;
            }
            return 0;
        case 's':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_020_SA;
                case 'e':
                    return SE_DOU_BOY_023_SE;
                case 'i':
                    return SE_DOU_BOY_021_SI;
                case 'o':
                    return SE_DOU_BOY_024_SO;
                case 'u':
                    return SE_DOU_BOY_022_SU;
            }
            return 0;
        case 't':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_030_TA;
                case 'e':
                    return SE_DOU_BOY_033_TE;
                case 'i':
                    return SE_DOU_BOY_031_TI;
                case 'o':
                    return SE_DOU_BOY_034_TO;
                case 'u':
                    return SE_DOU_BOY_032_TU;
                case 'y':
                    return SE_DOU_BOY_617_TY;
            }
            return 0;
        case 'u':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_090_WA;
            }
            return 0;
        case 'v':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_250_BA;
                case 'e':
                    return SE_DOU_BOY_253_BE;
                case 'i':
                    return SE_DOU_BOY_618_VI;
                case 'o':
                    return SE_DOU_BOY_254_BO;
                case 'u':
                    return SE_DOU_BOY_252_BU;
            }
            return 0;
        case 'w':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_090_WA;
                case 'o':
                    return SE_DOU_BOY_004_O;
            }
            return 0;
        case 'y':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_050_HA;
                case 'o':
                    return SE_DOU_BOY_054_HO;
            }
            return 0;
        case 'z':
            switch (next) {
                case 'a':
                    return SE_DOU_BOY_020_SA;
                case 'e':
                    return SE_DOU_BOY_023_SE;
                case 'i':
                    return SE_DOU_BOY_021_SI;
                case 'o':
                    return SE_DOU_BOY_024_SO;
                case 'u':
                    return SE_DOU_BOY_022_SU;
            }
            return 0;
    }
    return 0;
}
