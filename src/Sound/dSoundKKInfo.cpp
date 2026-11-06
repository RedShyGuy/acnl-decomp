#include "Sound/dSoundKKInfo.h"
#include "Sound/dSoundKKInfo_SingletonDisposer_.h"
#include "sead/seadNew.h"

// 0x0094E004
SoundKKInfo* SoundKKInfo::s_pInstance = NULL;
// 0x0094E008
SoundKKInfo::SingletonDisposer_* SoundKKInfo::s_pSingletonDisposer = NULL;

// 0x0083B980
const SoundKKInfo::Song SoundKKInfo::s_Songs[SONG_NUM] = {
    {"kk01_Sanbika", 4, 3, 2, 0x1E5F, 0xFB4, 32, 40, 16},
    {"kk02_March", 4, 1, 2, 0x3263, 0xFB4, 16, 32, 16},
    {"kk03_Waltz", 3, 12, 2, 0x3887, 0xF5A, 12, 48, 12},
    {"kk04_Swing", 4, 2, 2, 0x2891, 0xF96, 16, 64, 16},
    {"kk05_Jazz", 4, 2, 4, 0x4A3F, 0xF96, 16, 128, 16},
    {"kk06_Fusion", 4, 11, 4, 0x578F, 0xF5A, 32, 64, 16},
    {"kk07_Etude", 4, 3, 2, 0x221F, 0xF00, 16, 68, 16},
    {"kk08_Lullaby", 4, 2, 2, 0x209F, 0xFF0, 16, 64, 16},
    {"kk09_Maria", 4, 0, 2, 0x233F, 0xFD2, 16, 64, 16},
    {"kk10_Samba", 4, 0, 4, 0x5813, 0xF1E, 16, 64, 16},
    {"kk11_Bossa", 4, 1, 1, 0x1A3F, 0xF5A, 32, 64, 16},
    {"kk12_Caripso", 4, 0, 4, 0x4BE3, 0xF1E, 16, 32, 16},
    {"kk13_Salsa", 4, 2, 4, 0x4DE7, 0xEE2, 16, 64, 16},
    {"kk14_Mambo", 4, 3, 2, 0x2EFD, 0xEE2, 16, 64, 16},
    {"kk15_Reggae", 4, 3, 2, 0x3003, 0xF3C, 16, 64, 16},
    {"kk16_Ska", 4, 7, 2, 0x3483, 0xEC4, 16, 48, 16},
    {"kk17_Tango", 4, 21, 2, 0x2E9B, 0xF5A, 16, 32, 16},
    {"kk18_Haisai", 4, 8, 2, 0x2F85, 0xF3C, 16, 32, 16},
    {"kk19_Aloha", 4, 0, 2, 0x2705, 0xF3C, 16, 96, 16},
    {"kk20_Irish", 4, 13, 2, 0x2E67, 0xF3C, 16, 96, 16},
    {"kk21_Peru", 4, 13, 4, 0x4B78, 0xF5A, 16, 56, 16},
    {"kk22_Cossack", 4, 12, 2, 0x311F, 0xF78, 16, 68, 16},
    {"kk23_China", 4, 13, 2, 0x237D, 0xF3C, 16, 40, 16},
    {"kk24_Turkey", 4, 12, 2, 0x2C3F, 0xF96, 16, 72, 16},
    {"kk25_Afro", 4, 21, 2, 0x347F, 0xFD2, 16, 60, 16},
    {"kk26_Minyo", 4, 12, 2, 0x28D3, 0xEC4, 16, 38, 16},
    {"kk27_Rock", 4, 20, 2, 0x23A1, 0xFD2, 16, 48, 16},
    {"kk28_RocknRoll", 4, 4, 2, 0x421F, 0xFB4, 16, 80, 16},
    {"kk29_RagTime", 4, 2, 2, 0x463B, 0xF78, 16, 64, 16},
    {"kk30_NewOrleans", 4, 14, 2, 0x443F, 0xF1E, 32, 64, 16},
    {"kk31_Funk", 4, 0, 2, 0x3A37, 0xF96, 16, 64, 16},
    {"kk32_Blues", 4, 0, 2, 0x263F, 0xF96, 16, 48, 16},
    {"kk33_Gospel", 3, 2, 2, 0x290F, 0xFD2, 24, 96, 12},
    {"kk34_Soul", 4, 0, 2, 0x26FF, 0xF3C, 16, 64, 16},
    {"kk35_Urban", 4, 18, 4, 0x4CD6, 0x104A, 16, 64, 16},
    {"kk36_LoveSong", 4, 2, 2, 0x1B8F, 0xF3C, 32, 68, 16},
    {"kk37_DrumnBass", 4, 1, 2, 0x3E44, 0xF1E, 32, 64, 16},
    {"kk38_TechnoBeat", 4, 12, 2, 0x3543, 0xF96, 32, 64, 16},
    {"kk39_EuroBeat", 4, 13, 2, 0x399B, 0xF96, 16, 64, 16},
    {"kk40_OnlyMe", 4, 2, 2, 0x1D9F, 0x1068, 16, 40, 16},
    {"kk41_Country", 4, 3, 2, 0x2BAF, 0xFB4, 16, 32, 16},
    {"kk42_Eleki", 4, 14, 2, 0x3D07, 0xF78, 16, 32, 16},
    {"kk43_Ballad", 4, 0, 2, 0x209F, 0xFD2, 16, 40, 16},
    {"kk44_ShowaKayo", 4, 22, 2, 0x275F, 0xFD2, 32, 32, 16},
    {"kk45_Enka", 4, 15, 2, 0x1E8F, 0xF96, 16, 32, 16},
    {"kk46_KekeRider", 4, 17, 2, 0x39BF, 0xF78, 16, 64, 16},
    {"kk47_KowaiUta", 4, 21, 2, 0x1AFF, 0xF3C, 16, 32, 16},
    {"kk48_Western", 4, 12, 2, 0x38FF, 0xF3C, 16, 64, 16},
    {"kk49_Sensei", 4, 0, 2, 0x2F3F, 0xF96, 16, 64, 16},
    {"kk50_Paris", 3, 2, 3, 0x575F, 0xF78, 12, 192, 12},
    {"kk51_Parade", 4, 0, 2, 0x2CDC, 0xF96, 16, 64, 16},
    {"kk52_Senor", 3, 4, 2, 0x401F, 0xF78, 12, 105, 12},
    {"kk53_KekeSong", 4, 0, 2, 0x222F, 0xF78, 16, 48, 16},
    {"kk54_DaiSuki", 4, 0, 2, 0x299F, 0xF96, 16, 66, 16},
    {"kk55_Ototoi", 4, 2, 2, 0x245F, 0xFD2, 16, 64, 16},
    {"kk56_BokuNoBasho", 4, 3, 2, 0x20FF, 0xFD2, 16, 32, 16},
    {"kk57_MoriNoSeikatsu", 4, 2, 2, 0x2D2F, 0xF5A, 16, 64, 16},
    {"kk58_NamiNami", 4, 3, 2, 0x323F, 0x10C2, 16, 64, 16},
    {"kk59_KangaeChu", 4, 3, 2, 0x1C43, 0xF96, 16, 32, 16},
    {"kk60_Dixie", 4, 0, 4, 0x4A43, 0xF78, 16, 32, 16},
    {"kk61_Gamelan", 4, 4, 1, 0x174B, 0xFD2, 16, 64, 16},
    {"kk62_Daimyo", 4, 23, 2, 0x1CDF, 0xFD2, 16, 64, 16},
    {"kk63_Alpine", 3, 0, 2, 0x3CEF, 0xF78, 12, 96, 12},
    {"kk64_HunaUta2001", 4, 2, 1, 0x10D1, 0xF96, 16, 52, 16},
    {"kk65_Napolitan", 4, 2, 1, 0x17E8, 0xF5A, 12, 96, 12},
    {"kk66_NiDanZaka", 4, 22, 1, 0xFCE, 0xF5A, 16, 68, 16},
    {"kk67_KekeBilly", 4, 0, 2, 0x4D43, 0xF96, 16, 96, 16},
    {"kk68_Keiji", 4, 22, 4, 0x69C8, 0x102C, 16, 72, 16},
    {"kk69_Ondo", 4, 16, 2, 0x30B6, 0xF78, 16, 60, 16},
    {"kk70_Metal", 4, 21, 2, 0x3B40, 0xF78, 16, 68, 16},
    {"kk71_BlueOnigiri", 4, 3, 2, 0x11FF, 0xFD2, 16, 60, 16},
    {"kk72_HaruNoKomorebi", 4, 11, 2, 0x42C5, 0xF96, 16, 64, 16},
    {"kk73_Horo", 4, 0, 2, 0x14A8, 0xFB4, 12, 57, 12},
    {"kk74_House", 4, 3, 2, 0x3251, 0xF78, 32, 64, 16},
    {"kk75_Sonata", 4, 16, 2, 0x22E3, 0x104A, 16, 32, 16},
    {"kk76_UtataneNoYume", 3, 4, 1, 0x1142, 0xF96, 12, 48, 12},
    {"kk77_Osanpo", 4, 4, 2, 0x315E, 0xF78, 16, 80, 16},
    {"kk78_DoubutsuNoShima", 4, 2, 2, 0x16F4, 0xFB4, 16, 64, 16},
    {"kk79_Minimal", 4, 1, 2, 0x24E1, 0xEA6, 16, 32, 16},
    {"kk80_Hollywood", 4, 23, 2, 0x2401, 0xEE2, 16, 38, 16},
    {"kk81_Maharaja", 4, 12, 2, 0x24BF, 0xEA6, 16, 48, 16},
    {"kk82_Roma", 3, 23, 2, 0x3E31, 0xF3C, 24, 48, 12},
    {"kk83_Milonga", 4, 23, 2, 0x24B8, 0xEA6, 16, 64, 16},
    {"kk84_RareGroove", 4, 2, 2, 0x271A, 0xF00, 16, 52, 16},
    {"kk85_Jongara", 4, 14, 2, 0x30AB, 0xEC4, 16, 56, 16},
    {"kk86_Flamenco", 3, 13, 2, 0x3B19, 0xF1E, 12, 48, 12},
    {"kk87_Bolero", 4, 1, 2, 0x1F4F, 0xFB4, 16, 64, 16},
    {"kk88_Idol", 4, 1, 2, 0x3F21, 0xFF0, 16, 64, 16},
    {"kk89_Electronica", 4, 9, 2, 0x319F, 0xF96, 16, 64, 16},
    {"kk90_Disco", 4, 9, 2, 0x34B2, 0x102C, 16, 72, 16},
    {"kk91_BirthdaySong", 3, 11, 2, 0x2595, 0xFD2, 16, 52, 16},
    {"kk92_Town", 3, 0, 2, 0x350E, 0xF1E, 0, 0, 0},
    {"kk93_Sayonara", 4, 11, 2, 0x1CD8, 0xFD2, 0, 0, 0},
    {"kk94_Drive", 4, 7, 2, 0x3C7C, 0xF5A, 0, 0, 0},
};

// 0x001232F0
SoundKKInfo* SoundKKInfo::createInstance(sead::Heap* heap)
{
    if (s_pInstance == NULL) {
        u8* buffer = new (heap, 4) u8[sizeof(SoundKKInfo)];
        SingletonDisposer_* disposer = NULL;
        if (buffer != NULL) {
            disposer = new (buffer) SingletonDisposer_(heap);
        }
        s_pSingletonDisposer = disposer;
        s_pInstance = reinterpret_cast<SoundKKInfo*>(buffer);
    }
    return s_pInstance;
}

// 0x0070F72C
s32 SoundKKInfo::ConvertKey(u32 key) const
{
    switch (key) {
    case 7:
    case 16:
        return 1;
    case 2:
    case 23:
        return 2;
    case 9:
    case 18:
        return 3;
    case 4:
    case 13:
        return 4;
    case 11:
    case 20:
        return 5;
    case 6:
    case 15:
        return 6;
    case 1:
    case 22:
        return 7;
    case 8:
    case 17:
        return 8;
    case 3:
    case 12:
        return 9;
    case 10:
    case 19:
        return 10;
    case 5:
    case 14:
        return 11;
    }
    return 0;
}
