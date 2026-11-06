#include "Sound/dSoundDoubutsugoMgr.h"
#include "Sound/dSoundLabel.h"

// the tables of SoundDoubutsugoMgr (in a file of their own, GCC folds them otherwise)

// 0x0083A20C
const f32 SoundDoubutsugoMgr::s_LevelVolumes[LEVEL_NUM] = {0.3f, 0.65f, 1.0f, 1.0f, 1.0f};

// 0x0083A220
const f32 SoundDoubutsugoMgr::s_ExtraPitches[EXTRA_VOICE_NUM] = {1.2f, 0.9f, 0.8f};

// 0x0083A22C
const f32 SoundDoubutsugoMgr::s_FxSends[SoundFxMgr::FX_NUM] = {0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f};

// 0x0083A254
const u32 SoundDoubutsugoMgr::s_VoiceGroups[VOICE_NUM] = {
    0, 1, 1, 1, 1, 0, 0, 2, 0, 3, 4, 1, 0, 1, 0,
    1, 2, 2, 2, 1, 0, 0, 1, 0, 0, 2, 0, 2, 1,
};

// 0x0083A2C8
const u16 SoundDoubutsugoMgr::s_ExtendedChars[EXTENDED_CHAR_NUM] = {
    'a', 'a', 'a', 'a', 'a', 'a', 'a', 'e', 'c', 'e', 'e', 'e',
    'e', 'i', 'i', 'i', 'i', 'd', 'n', 'o', 'o', 'o', 'o', 'o',
    0, 0, 'u', 'u', 'u', 'u', 0, 0, 155, 'y', 157, 158,
    159, 0, 0, 0, 0, 0, 0, 0, 0, 47, 32, 15,
    25, 2, 57, 12, 39, 4, 28, 8, 38, 4, 11, 73,
    64, 44, 22, 10, 49, 77, 25, 23, 31, 70, 47, 11,
    21, 10, 47, 56, 56, 50, 59, 53, 59, 50, 4, 6,
    47, 56, 56, 50, 53, 21, 50, 53, 59, 53, 59, 0,
};

// 0x0083A3D0
const u16 SoundDoubutsugoMgr::s_CompatibilityJamo[51] = {
    16, 15, 16, 83, 83, 83, 41, 36, 75, 75, 75, 75, 75, 75, 75, 75, 64,
    54, 55, 54, 25, 25, 0, 24, 33, 33, 15, 36, 55, 53, 2, 2, 68, 8,
    8, 8, 8, 8, 10, 79, 79, 10, 72, 6, 10, 8, 4, 70, 8, 6, 4,
};

// 0x0083A436
const u16 SoundDoubutsugoMgr::s_InitialJamo[HANGUL_INITIAL_NUM] = {
    16, 15, 83, 41, 36, 75, 64, 54, 55, 25, 25, 0, 24, 33, 33, 15, 36, 55, 53,
};

// 0x0083A45C
const u16 SoundDoubutsugoMgr::s_MedialJamo[HANGUL_MEDIAL_NUM] = {
    2, 2, 68, 8, 8, 8, 8, 8, 10, 79, 79, 10, 72, 6, 10, 8, 4, 70, 8, 6, 4,
};

// 0x0083A486
const u16 SoundDoubutsugoMgr::s_FinalJamo[27] = {
    16, 15, 16, 83, 83, 83, 41, 75, 75, 75, 75, 75, 75, 75, 75, 64, 54, 54, 25, 25, 0, 24, 33, 15, 36, 55, 53,
};

// 0x0083A4BC
const u16 SoundDoubutsugoMgr::s_HangulSyllables[HANGUL_INITIAL_NUM][HANGUL_MEDIAL_NUM] = {
    {12, 12, 12, 18, 20, 18, 20, 18, 20, 12, 18, 18, 20, 16, 20, 18, 14, 16, 16, 14, 14},
    {11, 11, 11, 17, 19, 17, 19, 17, 19, 11, 17, 17, 19, 15, 19, 17, 13, 15, 15, 13, 13},
    {42, 42, 42, 45, 46, 45, 46, 45, 46, 42, 45, 45, 46, 44, 46, 45, 43, 44, 44, 43, 43},
    {32, 32, 32, 39, 41, 39, 41, 39, 41, 32, 39, 39, 41, 26, 41, 39, 24, 26, 26, 24, 24},
    {31, 31, 31, 38, 40, 38, 40, 38, 40, 31, 38, 38, 40, 36, 40, 38, 33, 36, 36, 33, 33},
    {73, 73, 73, 76, 77, 76, 77, 76, 77, 73, 76, 76, 77, 75, 77, 76, 74, 75, 75, 74, 74},
    {62, 62, 62, 65, 66, 65, 66, 65, 66, 62, 65, 65, 66, 64, 66, 65, 63, 64, 64, 63, 63},
    {48, 48, 48, 57, 60, 57, 60, 57, 60, 48, 57, 57, 60, 54, 60, 57, 51, 54, 54, 51, 51},
    {49, 49, 49, 58, 61, 58, 61, 58, 61, 49, 58, 58, 61, 55, 61, 58, 52, 55, 55, 52, 52},
    {21, 21, 21, 27, 29, 27, 29, 27, 29, 21, 27, 27, 29, 25, 29, 27, 23, 25, 25, 23, 23},
    {21, 21, 21, 27, 29, 27, 29, 27, 29, 21, 27, 27, 29, 25, 29, 27, 23, 25, 25, 23, 23},
    {2, 2, 68, 8, 10, 8, 10, 8, 10, 79, 8, 8, 72, 6, 10, 8, 4, 70, 6, 4, 4},
    {22, 22, 22, 28, 30, 28, 30, 28, 30, 22, 28, 28, 30, 26, 30, 28, 24, 26, 26, 24, 24},
    {31, 31, 31, 38, 40, 38, 40, 38, 40, 31, 38, 38, 40, 36, 40, 38, 33, 36, 36, 33, 33},
    {31, 31, 31, 38, 40, 38, 40, 38, 40, 31, 38, 38, 40, 36, 40, 38, 33, 36, 36, 33, 33},
    {11, 11, 11, 17, 19, 17, 19, 17, 19, 11, 17, 17, 19, 15, 19, 17, 13, 15, 15, 13, 13},
    {31, 31, 31, 38, 40, 38, 40, 38, 40, 31, 38, 38, 40, 36, 40, 38, 33, 36, 36, 33, 33},
    {49, 49, 49, 58, 61, 58, 61, 58, 61, 49, 58, 58, 61, 55, 61, 58, 52, 55, 55, 52, 52},
    {47, 47, 47, 56, 59, 56, 59, 56, 59, 47, 56, 56, 59, 54, 59, 56, 50, 54, 54, 50, 50},
};

// 0x0083A7DC
const u32 SoundDoubutsugoMgr::s_DigitSounds[LANGUAGE_NUM][10] = {
    // Japanese
    {
        SE_DOU_BOY_223_ZE, SE_DOU_BOY_001_I,
        SE_DOU_BOY_041_NI, SE_DOU_BOY_020_SA,
        SE_DOU_BOY_074_YO, SE_DOU_BOY_214_GO,
        SE_DOU_BOY_084_RO, SE_DOU_BOY_040_NA,
        SE_DOU_BOY_050_HA, SE_DOU_BOY_012_KU,
    },
    // English
    {
        SE_DOU_BOY_223_ZE, SE_DOU_BOY_090_WA,
        SE_DOU_BOY_032_TU, SE_DOU_BOY_081_RI,
        SE_DOU_BOY_054_HO, SE_DOU_BOY_050_HA,
        SE_DOU_BOY_021_SI, SE_DOU_BOY_023_SE,
        SE_DOU_BOY_606_EI, SE_DOU_BOY_040_NA,
    },
    // German
    {
        SE_DOU_BOY_042_NU, SE_DOU_BOY_600_AI,
        SE_DOU_BOY_524_Y, SE_DOU_BOY_232_DU,
        SE_DOU_BOY_521_V, SE_DOU_BOY_052_HU,
        SE_DOU_BOY_223_ZE, SE_DOU_BOY_221_ZI,
        SE_DOU_BOY_000_A, SE_DOU_BOY_044_NO,
    },
    // French
    {
        SE_DOU_BOY_023_SE, SE_DOU_BOY_601_AN,
        SE_DOU_BOY_232_DU, SE_DOU_BOY_034_TO,
        SE_DOU_BOY_010_KA, SE_DOU_BOY_020_SA,
        SE_DOU_BOY_021_SI, SE_DOU_BOY_023_SE,
        SE_DOU_BOY_072_YU, SE_DOU_BOY_042_NU,
    },
    // Italian
    {
        SE_DOU_BOY_223_ZE, SE_DOU_BOY_002_U,
        SE_DOU_BOY_232_DU, SE_DOU_BOY_033_TE,
        SE_DOU_BOY_010_KA, SE_DOU_BOY_031_TI,
        SE_DOU_BOY_600_AI, SE_DOU_BOY_023_SE,
        SE_DOU_BOY_004_O, SE_DOU_BOY_044_NO,
    },
    // Spanish
    {
        SE_DOU_BOY_023_SE, SE_DOU_BOY_002_U,
        SE_DOU_BOY_234_DO, SE_DOU_BOY_083_RE,
        SE_DOU_BOY_010_KA, SE_DOU_BOY_031_TI,
        SE_DOU_BOY_023_SE, SE_DOU_BOY_021_SI,
        SE_DOU_BOY_004_O, SE_DOU_BOY_043_NE,
    },
    // Korean
    {
        SE_DOU_BOY_074_YO, SE_DOU_BOY_001_I,
        SE_DOU_BOY_504_E, SE_DOU_BOY_403_3,
        SE_DOU_BOY_020_SA, SE_DOU_BOY_514_O,
        SE_DOU_BOY_520_U, SE_DOU_BOY_031_TI,
        SE_DOU_BOY_350_PA, SE_DOU_BOY_012_KU,
    },
};

// 0x0083A8F4
const u32 SoundDoubutsugoMgr::s_LetterSounds[LANGUAGE_NUM][26] = {
    // Japanese
    {
        SE_DOU_BOY_606_EI, SE_DOU_BOY_251_BI,
        SE_DOU_BOY_021_SI, SE_DOU_BOY_231_DI,
        SE_DOU_BOY_001_I, SE_DOU_BOY_605_EF,
        SE_DOU_BOY_221_ZI, SE_DOU_BOY_606_EI,
        SE_DOU_BOY_600_AI, SE_DOU_BOY_612_JE,
        SE_DOU_BOY_013_KE, SE_DOU_BOY_003_E,
        SE_DOU_BOY_607_EM, SE_DOU_BOY_608_EN,
        SE_DOU_BOY_613_OO, SE_DOU_BOY_351_PI,
        SE_DOU_BOY_614_QU, SE_DOU_BOY_000_A,
        SE_DOU_BOY_609_ES, SE_DOU_BOY_617_TY,
        SE_DOU_BOY_072_YU, SE_DOU_BOY_618_VI,
        SE_DOU_BOY_230_DA, SE_DOU_BOY_003_E,
        SE_DOU_BOY_090_WA, SE_DOU_BOY_223_ZE,
    },
    // English
    {
        SE_DOU_BOY_606_EI, SE_DOU_BOY_250_BA,
        SE_DOU_BOY_602_CH, SE_DOU_BOY_233_DE,
        SE_DOU_BOY_003_E, SE_DOU_BOY_052_HU,
        SE_DOU_BOY_610_GY, SE_DOU_BOY_053_HE,
        SE_DOU_BOY_001_I, SE_DOU_BOY_612_JE,
        SE_DOU_BOY_010_KA, SE_DOU_BOY_082_RU,
        SE_DOU_BOY_062_MU, SE_DOU_BOY_105_NN,
        SE_DOU_BOY_004_O, SE_DOU_BOY_351_PI,
        SE_DOU_BOY_614_QU, SE_DOU_BOY_080_RA,
        SE_DOU_BOY_022_SU, SE_DOU_BOY_032_TU,
        SE_DOU_BOY_002_U, SE_DOU_BOY_618_VI,
        SE_DOU_BOY_002_U, SE_DOU_BOY_611_IX,
        SE_DOU_BOY_617_TY, SE_DOU_BOY_222_ZU,
    },
    // German
    {
        SE_DOU_BOY_000_A, SE_DOU_BOY_253_BE,
        SE_DOU_BOY_033_TE, SE_DOU_BOY_233_DE,
        SE_DOU_BOY_500_A, SE_DOU_BOY_605_EF,
        SE_DOU_BOY_213_GE, SE_DOU_BOY_050_HA,
        SE_DOU_BOY_001_I, SE_DOU_BOY_074_YO,
        SE_DOU_BOY_010_KA, SE_DOU_BOY_511_L,
        SE_DOU_BOY_607_EM, SE_DOU_BOY_608_EN,
        SE_DOU_BOY_514_O, SE_DOU_BOY_353_PE,
        SE_DOU_BOY_012_KU, SE_DOU_BOY_511_L,
        SE_DOU_BOY_518_S, SE_DOU_BOY_033_TE,
        SE_DOU_BOY_002_U, SE_DOU_BOY_050_HA,
        SE_DOU_BOY_253_BE, SE_DOU_BOY_523_X,
        SE_DOU_BOY_001_I, SE_DOU_BOY_223_ZE,
    },
    // French
    {
        SE_DOU_BOY_000_A, SE_DOU_BOY_253_BE,
        SE_DOU_BOY_023_SE, SE_DOU_BOY_233_DE,
        SE_DOU_BOY_002_U, SE_DOU_BOY_605_EF,
        SE_DOU_BOY_612_JE, SE_DOU_BOY_518_S,
        SE_DOU_BOY_001_I, SE_DOU_BOY_221_ZI,
        SE_DOU_BOY_010_KA, SE_DOU_BOY_511_L,
        SE_DOU_BOY_607_EM, SE_DOU_BOY_608_EN,
        SE_DOU_BOY_514_O, SE_DOU_BOY_353_PE,
        SE_DOU_BOY_614_QU, SE_DOU_BOY_511_L,
        SE_DOU_BOY_518_S, SE_DOU_BOY_033_TE,
        SE_DOU_BOY_072_YU, SE_DOU_BOY_253_BE,
        SE_DOU_BOY_522_W, SE_DOU_BOY_523_X,
        SE_DOU_BOY_083_RE, SE_DOU_BOY_223_ZE,
    },
    // Italian
    {
        SE_DOU_BOY_000_A, SE_DOU_BOY_501_B,
        SE_DOU_BOY_502_C, SE_DOU_BOY_503_D,
        SE_DOU_BOY_500_A, SE_DOU_BOY_505_F,
        SE_DOU_BOY_506_G, SE_DOU_BOY_000_A,
        SE_DOU_BOY_504_E, SE_DOU_BOY_001_I,
        SE_DOU_BOY_010_KA, SE_DOU_BOY_511_L,
        SE_DOU_BOY_607_EM, SE_DOU_BOY_608_EN,
        SE_DOU_BOY_613_OO, SE_DOU_BOY_515_P,
        SE_DOU_BOY_012_KU, SE_DOU_BOY_511_L,
        SE_DOU_BOY_518_S, SE_DOU_BOY_519_T,
        SE_DOU_BOY_002_U, SE_DOU_BOY_252_BU,
        SE_DOU_BOY_234_DO, SE_DOU_BOY_523_X,
        SE_DOU_BOY_083_RE, SE_DOU_BOY_223_ZE,
    },
    // Spanish
    {
        SE_DOU_BOY_000_A, SE_DOU_BOY_253_BE,
        SE_DOU_BOY_023_SE, SE_DOU_BOY_233_DE,
        SE_DOU_BOY_500_A, SE_DOU_BOY_505_F,
        SE_DOU_BOY_053_HE, SE_DOU_BOY_000_A,
        SE_DOU_BOY_001_I, SE_DOU_BOY_054_HO,
        SE_DOU_BOY_010_KA, SE_DOU_BOY_511_L,
        SE_DOU_BOY_607_EM, SE_DOU_BOY_608_EN,
        SE_DOU_BOY_613_OO, SE_DOU_BOY_353_PE,
        SE_DOU_BOY_012_KU, SE_DOU_BOY_511_L,
        SE_DOU_BOY_518_S, SE_DOU_BOY_033_TE,
        SE_DOU_BOY_002_U, SE_DOU_BOY_253_BE,
        SE_DOU_BOY_234_DO, SE_DOU_BOY_523_X,
        SE_DOU_BOY_001_I, SE_DOU_BOY_023_SE,
    },
    // Korean
    {
        SE_DOU_BOY_606_EI, SE_DOU_BOY_251_BI,
        SE_DOU_BOY_021_SI, SE_DOU_BOY_231_DI,
        SE_DOU_BOY_001_I, SE_DOU_BOY_605_EF,
        SE_DOU_BOY_221_ZI, SE_DOU_BOY_606_EI,
        SE_DOU_BOY_600_AI, SE_DOU_BOY_612_JE,
        SE_DOU_BOY_013_KE, SE_DOU_BOY_003_E,
        SE_DOU_BOY_607_EM, SE_DOU_BOY_608_EN,
        SE_DOU_BOY_613_OO, SE_DOU_BOY_351_PI,
        SE_DOU_BOY_614_QU, SE_DOU_BOY_000_A,
        SE_DOU_BOY_609_ES, SE_DOU_BOY_617_TY,
        SE_DOU_BOY_072_YU, SE_DOU_BOY_618_VI,
        SE_DOU_BOY_230_DA, SE_DOU_BOY_003_E,
        SE_DOU_BOY_090_WA, SE_DOU_BOY_223_ZE,
    },
};

// 0x0083ABCC
const u8 SoundDoubutsugoMgr::s_LongSounds[SYLLABLE_NUM] = {
    1, 1, 3, 3, 5, 5, 7, 7, 9, 9, 1, 1, 3, 3, 5, 5, 7, 7, 9, 9,
    1, 1, 3, 3, 5, 5, 7, 7, 9, 9, 1, 1, 3, 3, 255, 5, 5, 7, 7, 9,
    9, 1, 3, 5, 7, 9, 1, 1, 1, 3, 3, 3, 5, 5, 5, 7, 7, 7, 9, 9,
    9, 1, 3, 5, 7, 9, 1, 1, 5, 5, 9, 9, 1, 3, 5, 7, 9, 1, 1, 3,
    7, 9, 82, 5, 1, 7, 7, 3, 3, 82, 3, 9, 5, 3, 3, 5, 7, 3, 3, 3,
    3, 5, 3, 3, 3, 3, 3, 5, 82, 82, 9, 3, 5, 5, 5, 3, 5, 3, 5, 5,
    3, 3, 3, 82, 3, 3, 3, 5, 3, 82, 82, 5, 5, 5, 7, 9, 5, 3, 3, 3,
};

// 0x0083AC58
const SoundDoubutsugoMgr::VoiceParam SoundDoubutsugoMgr::s_VoiceParams[VOICE_PARAM_NUM] = {
    {0.15f, 15, 5, 0.4f, 30, 12, 1.0f, 1.0f, 0.3f, 0.5f, 3, 5, 1},
    {0.15f, 15, 5, 0.3f, 30, 12, 1.1f, 1.1f, 0.3f, 0.5f, 2, 5, 1},
    {0.05f, 15, 5, 0.1f, 30, 12, 1.1f, 1.2f, 0.3f, 0.3f, 3, 5, 1},
    {0.15f, 15, 5, 0.175f, 30, 12, 0.9f, 0.9f, 0.3f, 0.5f, 3, 5, 1},
    {0.05f, 15, 5, 0.1f, 30, 12, 1.0f, 0.95f, 0.3f, 0.1f, 3, 5, 1},
    {0.15f, 15, 5, 0.175f, 30, 12, 0.9f, 0.9f, 0.3f, 0.5f, 3, 5, 1},
    {0.15f, 15, 5, 0.4f, 30, 12, 0.85f, 1.15f, 0.3f, 0.5f, 3, 5, 1},
    {0.15f, 15, 5, 0.3f, 30, 12, 0.9f, 1.2f, 0.3f, 0.5f, 2, 5, 1},
    {0.05f, 15, 5, 0.1f, 30, 12, 0.9f, 1.3f, 0.3f, 0.3f, 3, 5, 1},
    {0.15f, 15, 5, 0.175f, 30, 12, 0.75f, 1.0f, 0.3f, 0.5f, 3, 5, 1},
    {0.05f, 15, 5, 0.1f, 30, 12, 1.0f, 1.05f, 0.3f, 0.1f, 3, 5, 1},
    {0.15f, 15, 5, 0.175f, 30, 12, 0.75f, 1.0f, 0.3f, 0.5f, 3, 5, 1},
    {0.15f, 15, 5, 0.4f, 30, 12, 1.0f, 0.82f, 0.3f, 0.2f, 3, 3, 1},
    {0.15f, 15, 5, 0.3f, 30, 12, 1.1f, 0.9f, 0.3f, 0.5f, 2, 5, 1},
    {0.05f, 15, 5, 0.1f, 30, 12, 1.1f, 1.0f, 0.3f, 0.3f, 3, 5, 1},
    {0.15f, 15, 5, 0.175f, 30, 12, 0.9f, 0.78f, 0.3f, 0.5f, 3, 5, 1},
    {0.05f, 15, 5, 0.1f, 30, 12, 1.0f, 0.73f, 0.3f, 0.1f, 3, 5, 1},
    {0.15f, 15, 5, 0.175f, 30, 12, 0.9f, 0.78f, 0.3f, 0.5f, 3, 5, 1},
    {0.15f, 15, 5, 0.2f, 30, 12, 1.0f, 0.95f, 0.3f, 0.25f, 3, 5, 1},
    {0.15f, 15, 5, 0.3f, 30, 12, 1.1f, 1.05f, 0.3f, 0.5f, 2, 5, 1},
    {0.05f, 15, 5, 0.1f, 30, 12, 1.1f, 1.15f, 0.3f, 0.3f, 3, 5, 1},
    {0.15f, 15, 5, 0.175f, 30, 12, 0.9f, 0.9f, 0.3f, 0.5f, 3, 5, 1},
    {0.05f, 15, 5, 0.1f, 30, 12, 1.0f, 0.85f, 0.3f, 0.07f, 3, 5, 1},
    {0.15f, 15, 5, 0.175f, 30, 12, 0.9f, 0.9f, 0.3f, 0.5f, 3, 5, 1},
    {0.15f, 15, 5, 0.3f, 30, 12, 1.1f, 1.1f, 0.3f, 0.5f, 2, 5, 1},
    {0.05f, 15, 5, 0.1f, 30, 12, 1.1f, 1.2f, 0.3f, 0.3f, 3, 5, 1},
    {0.05f, 15, 5, 0.1f, 30, 12, 1.0f, 0.9f, 0.3f, 0.1f, 3, 5, 1},
    {0.15f, 15, 5, 0.175f, 30, 12, 0.9f, 0.95f, 0.3f, 0.5f, 3, 5, 1},
    {0.2f, 15, 5, 0.4f, 30, 12, 1.0f, 1.1f, 0.3f, 0.4f, 3, 3, 1},
    {0.15f, 15, 5, 0.3f, 30, 12, 1.1f, 1.2f, 0.3f, 0.5f, 2, 5, 1},
    {0.05f, 15, 5, 0.1f, 30, 12, 1.1f, 1.3f, 0.3f, 0.3f, 3, 5, 1},
    {0.15f, 15, 5, 0.175f, 30, 12, 0.9f, 1.0f, 0.3f, 0.5f, 3, 5, 1},
    {0.05f, 15, 5, 0.1f, 30, 12, 1.0f, 0.9f, 0.3f, 0.1f, 3, 5, 1},
    {0.15f, 15, 5, 0.175f, 30, 12, 0.9f, 1.0f, 0.3f, 0.5f, 3, 5, 1},
    {0.1f, 15, 5, 0.2f, 30, 12, 0.9f, 1.0f, 0.3f, 0.2f, 4, 2, 1},
    {0.05f, 15, 5, 0.1f, 30, 12, 1.0f, 1.1f, 0.3f, 0.4f, 3, 5, 1},
    {0.05f, 15, 5, 0.1f, 30, 12, 1.0f, 1.15f, 0.3f, 0.4f, 3, 5, 1},
    {0.15f, 15, 5, 0.16f, 30, 12, 0.8f, 0.9f, 0.3f, 0.5f, 3, 5, 1},
    {0.05f, 15, 5, 0.07f, 30, 12, 1.0f, 0.85f, 0.3f, 0.1f, 3, 5, 1},
    {0.15f, 15, 5, 0.16f, 30, 12, 0.8f, 0.9f, 0.3f, 0.5f, 3, 5, 1},
    {0.15f, 15, 5, 0.4f, 30, 12, 1.0f, 0.9f, 0.3f, 0.75f, 3, 2, 1},
    {0.15f, 15, 5, 0.3f, 30, 12, 1.1f, 1.0f, 0.3f, 0.5f, 2, 5, 1},
    {0.05f, 15, 5, 0.1f, 30, 12, 1.1f, 1.1f, 0.3f, 0.3f, 3, 5, 1},
    {0.15f, 15, 5, 0.175f, 30, 12, 0.9f, 0.8f, 0.3f, 0.5f, 3, 5, 1},
    {0.05f, 15, 5, 0.1f, 30, 12, 1.0f, 0.85f, 0.3f, 0.1f, 3, 5, 1},
    {0.15f, 15, 5, 0.175f, 30, 12, 0.9f, 0.8f, 0.3f, 0.5f, 3, 5, 1},
    {0.1f, 15, 5, 0.3f, 30, 12, 1.0f, 1.2f, 0.3f, 0.5f, 3, 3, 1},
    {0.1f, 15, 5, 0.2f, 30, 12, 0.9f, 1.4f, 0.3f, 0.3f, 3, 3, 1},
    {0.15f, 15, 5, 0.4f, 30, 12, 1.0f, 0.7f, 0.3f, 0.5f, 3, 3, 1},
    {0.1f, 15, 5, 0.3f, 30, 12, 1.0f, 0.95f, 0.3f, 0.5f, 3, 1, 0},
    {0.2f, 15, 5, 0.4f, 30, 12, 1.0f, 0.9f, 0.3f, 0.5f, 3, 1, 0},
    {0.05f, 15, 5, 0.2f, 30, 12, 0.9f, 0.85f, 0.3f, 0.2f, 3, 3, 1},
    {0.05f, 15, 5, 0.1f, 30, 12, 0.9f, 1.1f, 0.3f, 0.1f, 4, 3, 1},
    {0.05f, 15, 5, 0.15f, 30, 12, 0.9f, 0.9f, 0.3f, 0.15f, 3, 3, 1},
    {0.15f, 15, 5, 0.4f, 30, 12, 1.0f, 0.75f, 0.3f, 0.2f, 4, 3, 1},
    {0.3f, 15, 1, 0.5f, 30, 5, 1.2f, 0.9f, 0.6f, 0.4f, 2, 1, 1},
    {0.3f, 15, 1, 0.6f, 30, 5, 1.3f, 1.0f, 0.6f, 0.65f, 1, 1, 1},
    {0.2f, 15, 1, 0.4f, 30, 5, 1.3f, 1.1f, 0.6f, 0.45f, 2, 1, 1},
    {0.3f, 15, 1, 0.475f, 30, 5, 1.1f, 0.85f, 0.6f, 0.65f, 2, 1, 1},
    {0.05f, 10, 20, 0.08f, 20, 25, 0.2f, 2.0f, 0.5f, 0.1f, 5, 1, 1},
    {0.3f, 15, 1, 0.475f, 30, 5, 1.1f, 0.85f, 0.6f, 0.65f, 2, 1, 1},
    {0.15f, 15, 4, 0.25f, 25, 8, 1.0f, 0.9f, 0.3f, 0.25f, 3, 1, 1},
    {0.2f, 15, 4, 0.4f, 25, 8, 1.1f, 1.0f, 0.3f, 0.35f, 2, 1, 1},
    {0.05f, 15, 4, 0.4f, 25, 8, 1.1f, 1.0f, 0.3f, 0.35f, 3, 2, 1},
    {0.1f, 15, 4, 0.125f, 25, 8, 0.9f, 0.85f, 0.3f, 0.15f, 3, 2, 1},
    {0.0f, 15, 4, 0.2f, 25, 8, 1.0f, 0.9f, 0.3f, 0.15f, 3, 2, 1},
    {0.1f, 15, 4, 0.125f, 25, 8, 0.9f, 0.85f, 0.3f, 0.15f, 3, 2, 1},
    {0.2f, 15, 5, 0.4f, 30, 12, 1.0f, 0.9f, 0.3f, 0.4f, 3, 5, 1},
    {0.15f, 15, 5, 0.3f, 30, 12, 1.1f, 1.0f, 0.3f, 0.5f, 2, 5, 1},
    {0.05f, 15, 5, 0.3f, 30, 12, 1.1f, 1.2f, 0.3f, 0.3f, 3, 5, 1},
    {0.15f, 15, 5, 0.175f, 30, 12, 0.9f, 0.9f, 0.3f, 0.5f, 3, 5, 1},
    {0.05f, 15, 5, 0.1f, 30, 12, 1.0f, 0.8f, 0.3f, 0.1f, 3, 5, 1},
    {0.15f, 15, 5, 0.175f, 30, 12, 0.9f, 0.9f, 0.3f, 0.5f, 3, 5, 1},
    {0.1f, 15, 5, 0.2f, 30, 12, 0.9f, 1.2f, 0.3f, 0.2f, 4, 2, 1},
    {0.05f, 15, 5, 0.1f, 30, 12, 1.0f, 1.4f, 0.3f, 0.4f, 3, 5, 1},
    {0.05f, 15, 5, 0.3f, 30, 12, 1.0f, 1.3f, 0.3f, 0.4f, 3, 5, 1},
    {0.15f, 15, 5, 0.16f, 30, 12, 0.8f, 1.1f, 0.3f, 0.5f, 3, 5, 1},
    {0.05f, 15, 5, 0.06f, 30, 12, 1.0f, 1.15f, 0.3f, 0.1f, 3, 5, 1},
    {0.15f, 15, 5, 0.16f, 30, 12, 0.8f, 1.1f, 0.3f, 0.5f, 3, 5, 1},
    {0.2f, 15, 5, 0.4f, 30, 12, 1.0f, 1.7f, 0.3f, 0.4f, 3, 5, 1},
    {0.15f, 15, 5, 0.3f, 30, 12, 1.1f, 1.9f, 0.3f, 0.5f, 2, 5, 1},
    {0.05f, 15, 5, 0.3f, 30, 12, 1.1f, 2.0f, 0.3f, 0.3f, 3, 5, 1},
    {0.15f, 15, 5, 0.175f, 30, 12, 0.9f, 1.7f, 0.3f, 0.5f, 3, 5, 1},
    {0.05f, 15, 5, 0.1f, 30, 12, 1.0f, 1.6f, 0.3f, 0.1f, 3, 5, 1},
    {0.15f, 15, 5, 0.175f, 30, 12, 0.9f, 1.7f, 0.3f, 0.5f, 3, 5, 1},
    {0.1f, 15, 5, 0.3f, 30, 12, 0.5f, 1.75f, 0.3f, 0.2f, 3, 3, 1},
    {0.05f, 15, 5, 0.15f, 30, 12, 0.9f, 1.3f, 0.3f, 0.5f, 3, 5, 1},
};

// 0x0094DD34
const SoundDoubutsugoMgr::VoiceParam* SoundDoubutsugoMgr::s_pVoiceParams[VOICE_NUM][MOOD_NUM] = {
    {&s_VoiceParams[0], &s_VoiceParams[1], &s_VoiceParams[2], &s_VoiceParams[3], &s_VoiceParams[4], &s_VoiceParams[5]},
    {&s_VoiceParams[0], &s_VoiceParams[1], &s_VoiceParams[2], &s_VoiceParams[3], &s_VoiceParams[4], &s_VoiceParams[5]},
    {&s_VoiceParams[6], &s_VoiceParams[7], &s_VoiceParams[8], &s_VoiceParams[9], &s_VoiceParams[10], &s_VoiceParams[11]},
    {&s_VoiceParams[12], &s_VoiceParams[13], &s_VoiceParams[14], &s_VoiceParams[15], &s_VoiceParams[16], &s_VoiceParams[17]},
    {&s_VoiceParams[18], &s_VoiceParams[19], &s_VoiceParams[20], &s_VoiceParams[21], &s_VoiceParams[22], &s_VoiceParams[23]},
    {&s_VoiceParams[0], &s_VoiceParams[24], &s_VoiceParams[25], &s_VoiceParams[3], &s_VoiceParams[26], &s_VoiceParams[27]},
    {&s_VoiceParams[28], &s_VoiceParams[29], &s_VoiceParams[30], &s_VoiceParams[31], &s_VoiceParams[32], &s_VoiceParams[33]},
    {&s_VoiceParams[34], &s_VoiceParams[35], &s_VoiceParams[36], &s_VoiceParams[37], &s_VoiceParams[38], &s_VoiceParams[39]},
    {&s_VoiceParams[40], &s_VoiceParams[41], &s_VoiceParams[42], &s_VoiceParams[43], &s_VoiceParams[44], &s_VoiceParams[45]},
    {&s_VoiceParams[46], &s_VoiceParams[46], &s_VoiceParams[46], &s_VoiceParams[3], &s_VoiceParams[46], &s_VoiceParams[46]},
    {&s_VoiceParams[0], &s_VoiceParams[0], &s_VoiceParams[0], &s_VoiceParams[3], &s_VoiceParams[0], &s_VoiceParams[0]},
    {&s_VoiceParams[6], &s_VoiceParams[7], &s_VoiceParams[8], &s_VoiceParams[9], &s_VoiceParams[10], &s_VoiceParams[11]},
    {&s_VoiceParams[47], &s_VoiceParams[1], &s_VoiceParams[2], &s_VoiceParams[3], &s_VoiceParams[4], &s_VoiceParams[5]},
    {&s_VoiceParams[48], &s_VoiceParams[1], &s_VoiceParams[2], &s_VoiceParams[3], &s_VoiceParams[4], &s_VoiceParams[5]},
    {&s_VoiceParams[49], &s_VoiceParams[1], &s_VoiceParams[2], &s_VoiceParams[3], &s_VoiceParams[4], &s_VoiceParams[5]},
    {&s_VoiceParams[50], &s_VoiceParams[1], &s_VoiceParams[2], &s_VoiceParams[3], &s_VoiceParams[4], &s_VoiceParams[5]},
    {&s_VoiceParams[51], &s_VoiceParams[1], &s_VoiceParams[2], &s_VoiceParams[3], &s_VoiceParams[4], &s_VoiceParams[5]},
    {&s_VoiceParams[52], &s_VoiceParams[1], &s_VoiceParams[2], &s_VoiceParams[3], &s_VoiceParams[4], &s_VoiceParams[5]},
    {&s_VoiceParams[53], &s_VoiceParams[1], &s_VoiceParams[2], &s_VoiceParams[3], &s_VoiceParams[4], &s_VoiceParams[5]},
    {&s_VoiceParams[54], &s_VoiceParams[1], &s_VoiceParams[2], &s_VoiceParams[3], &s_VoiceParams[4], &s_VoiceParams[5]},
    {&s_VoiceParams[28], &s_VoiceParams[1], &s_VoiceParams[30], &s_VoiceParams[31], &s_VoiceParams[32], &s_VoiceParams[33]},
    {&s_VoiceParams[0], &s_VoiceParams[24], &s_VoiceParams[25], &s_VoiceParams[3], &s_VoiceParams[26], &s_VoiceParams[27]},
    {&s_VoiceParams[55], &s_VoiceParams[56], &s_VoiceParams[57], &s_VoiceParams[58], &s_VoiceParams[59], &s_VoiceParams[60]},
    {&s_VoiceParams[61], &s_VoiceParams[62], &s_VoiceParams[63], &s_VoiceParams[64], &s_VoiceParams[65], &s_VoiceParams[66]},
    {&s_VoiceParams[67], &s_VoiceParams[68], &s_VoiceParams[69], &s_VoiceParams[70], &s_VoiceParams[71], &s_VoiceParams[72]},
    {&s_VoiceParams[73], &s_VoiceParams[74], &s_VoiceParams[75], &s_VoiceParams[76], &s_VoiceParams[77], &s_VoiceParams[78]},
    {&s_VoiceParams[79], &s_VoiceParams[80], &s_VoiceParams[81], &s_VoiceParams[82], &s_VoiceParams[83], &s_VoiceParams[84]},
    {&s_VoiceParams[85], &s_VoiceParams[85], &s_VoiceParams[85], &s_VoiceParams[85], &s_VoiceParams[85], &s_VoiceParams[85]},
    {&s_VoiceParams[86], &s_VoiceParams[86], &s_VoiceParams[86], &s_VoiceParams[86], &s_VoiceParams[86], &s_VoiceParams[86]},
};

// 0x0083C3AC
const f32 SoundDoubutsugoMgr::s_SinglePanScale = 0.8f;
