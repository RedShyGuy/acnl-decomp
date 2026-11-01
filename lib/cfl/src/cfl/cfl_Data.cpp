#include "cfl/cfl_Database.h"
#include "cfl/cfl_Model.h"
#include "cfl/cfl_Net.h"
#include "nn/cfg/CTR/CTR_Api.h"

#include <cstring>

namespace {
// the magic of a block of Miis the program passes (name is ours)
const u32 MAGIC_CFMD = 0x444D4643;

// Miis of the program with a header (source 3 of CFLi_PickupCharInfo; name is ours)
struct CFLiMiiDataBlock
{
    u32 magic;                         // 0x00 "CFMD"
    u32 _4;                            // 0x04
    u32 _8;                            // 0x08
    const CFLiMiiDataOfficial* data;   // 0x0C
    u16 num;                           // 0x10
    u16 _12;                           // 0x12
};
} // namespace

// the colors of the skin (RGBA8; name is ours)
// 0x008A45C0
extern const u32 s_FacelineColors[6] = {0xFFADD3FF, 0xFF6BB6FF, 0xFF4279DE, 0xFF8CAAFF, 0xFF2951AD, 0xFF182C63};

// a Mii of the database that may be used (name is ours)
// 0x0016CB30 (name is ours)
DECOMP_NOINLINE const CFLiMiiDataOfficial* CFLi_GetDatabaseData(u16 index, bool checkSpecial)
{
    const CFLiMiiDataOfficial* result = NULL;
    if (index >= CFL_DATABASE_OWNED_NUM || !CFL_IsAvailable()) {
        return result;
    }
    CFLiDatabase* db = s_pDatabase;
    if (db == NULL || CFLi_GetFileBroken(1) || CFLi_GetFileBroken(0)) {
        return result;
    }
    const CFLiMiiDataOfficial* data = &db->official.owned[index];
    if (CFLi_IsNullMiiID(&data->core.createID)) {
        return result;
    }
    CFLiCharInfo info;
    CFLi_UnpackMiiDataCore(&info, &data->core, true, data->creatorName);
    if (!CFLi_VerifyCharInfo(&info, true) || CFLi_IsTemporaryMiiID(&info.createID)) {
        return result;
    }
    if (checkSpecial && !CFLi_IsNormalMiiID(&info.createID) && !s_EnableSpecialMii) {
        return result;
    }
    return data;
}

extern "C" {
// 0x0015376C | nintendogs:bytes [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE bool CFL_IsAvailableOfficialData(u16 index)
{
    bool available = false;
    if (index < CFL_DATABASE_OWNED_NUM && CFLi_GetDatabaseData(index, true) != NULL) {
        available = true;
    }
    return available;
}

// 0x0015300C | nintendogs:bytes [tier B]
u16 CFL_GetAvailableOfficialDataNum()
{
    u16 num = 0;
    for (int i = 0; i < CFL_DATABASE_OWNED_NUM; i++) {
        if (CFL_IsAvailableOfficialData(i)) {
            num++;
        }
    }
    return num;
}

// 0x00153DF4 | nintendogs:bytes-fuzzy [tier B]
DECOMP_NOINLINE bool CFL_SearchOfficialData(u16* index, const CFLiCreateID* id)
{
    bool found = false;
    if (id == NULL || !CFL_IsAvailable()) {
        return found;
    }
    CFLiDatabase* db = s_pDatabase;
    if (db == NULL || CFLi_GetFileBroken(1) || CFLi_GetFileBroken(0)) {
        return found;
    }
    for (int i = 0; i < CFL_DATABASE_OWNED_NUM; i++) {
        if (CFL_IsAvailableOfficialData(i) && CFLi_IsSameMiiID(&db->official.owned[i].core.createID, id)) {
            found = true;
            if (index != NULL) {
                *index = i;
            }
            break;
        }
    }
    return found;
}

// 0x0015337C | nintendogs:bytes [tier B]
s32 CFL_GetStoreData(CFLStoreData* store, int source, u16 index)
{
    s32 status = CFL_RESULT_NOT_AVAILABLE;
    if (!CFL_IsAvailable()) {
        return status;
    }
    const CFLiMiiDataOfficial* data;
    if (source == 0) {
        if (index >= CFL_DATABASE_OWNED_NUM) {
            return CFL_RESULT_INVALID_ARGUMENT;
        }
        data = CFLi_GetDatabaseData(index, true);
    } else if (source == 2) {
        if (index >= 6) {
            return CFL_RESULT_INVALID_ARGUMENT;
        }
        data = s_OfficialDataInterface.GetData(&s_OfficialData, index);
    } else {
        return CFL_RESULT_NOT_FOUND;
    }
    if (data == NULL) {
        return CFL_RESULT_NOT_FOUND;
    }
    CFLi_GetStoreDataFromPackedData(store, &data->core, true, data->creatorName);
    return CFL_RESULT_SUCCESS;
}

// unpacks a Mii: 0 database, 2 default Miis, 3 a CFLiMiiDataBlock, 4 store data (a Mii of the database is taken
// from there), 5 store data
// 0x00156438 | nintendogs:bytes-fuzzy [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE s32 CFLi_PickupCharInfo(CFLiCharInfo* info, int source, const void* data, u16 index)
{
    s32 status = CFL_RESULT_NOT_AVAILABLE;
    if (!CFL_IsAvailable()) {
        return status;
    }
    u32 magic = 0;
    if (data != NULL) {
        magic = *static_cast<const u32*>(data);
    }
    switch (source) {
    case 0:
    database: {
        status = CFL_RESULT_INVALID_ARGUMENT;
        if (info == NULL || index >= CFL_DATABASE_OWNED_NUM) {
            return status;
        }
        status = CFL_RESULT_INVALID_DATA;
        const CFLiMiiDataOfficial* official = CFLi_GetDatabaseData(index, true);
        if (official == NULL) {
            return status;
        }
        CFLi_UnpackMiiDataCore(info, &official->core, true, official->creatorName);
        if (!CFLi_VerifyCharInfo(info, true)) {
            return status;
        }
        status = CFL_RESULT_SUCCESS;
        break;
    }
    case 2: {
        if (index >= 6) {
            return CFL_RESULT_INVALID_ARGUMENT;
        }
        const CFLiMiiDataOfficial* official = s_OfficialDataInterface.GetData(&s_OfficialData, index);
        CFLi_UnpackMiiDataCore(info, &official->core, true, official->creatorName);
        status = CFL_RESULT_SUCCESS;
        break;
    }
    case 3: {
        if (data == NULL || magic != MAGIC_CFMD) {
            return CFL_RESULT_INVALID_ARGUMENT;
        }
        const CFLiMiiDataBlock* block = static_cast<const CFLiMiiDataBlock*>(data);
        if (block->num <= index || block->_12 == 0) {
            return CFL_RESULT_INVALID_DATA;
        }
        const CFLiMiiDataOfficial* official = &block->data[index];
        if (CFLi_IsNullMiiID(&official->core.createID)) {
            return CFL_RESULT_INVALID_DATA;
        }
        CFLi_UnpackMiiDataCore(info, &official->core, true, official->creatorName);
        status = CFL_RESULT_SUCCESS;
        break;
    }
    case 4:
    case 5: {
        if (data == NULL || magic == MAGIC_CFMD) {
            return CFL_RESULT_INVALID_ARGUMENT;
        }
        const CFLStoreData* store = static_cast<const CFLStoreData*>(data) + index;
        if (CFLi_GetCRC16(0, store, sizeof(CFLStoreData)) != 0) {
            return CFL_RESULT_INVALID_DATA;
        }
        if (source == 4) {
            u16 found;
            if (CFL_SearchOfficialData(&found, &store->official.core.createID)) {
                index = found;
                goto database;
            }
        }
        CFLi_UnpackMiiDataCore(info, &store->official.core, true, store->official.creatorName);
        status = CFL_RESULT_SUCCESS;
        break;
    }
    default:
        if (status != CFL_RESULT_SUCCESS) {
            return status;
        }
        break;
    }
    if (CFLi_IsNullMiiID(&info->createID) || !CFLi_VerifyCharInfo(info, true)) {
        return CFL_RESULT_INVALID_DATA;
    }
    return status;
}
} // extern "C"

namespace {
// replaces control characters (inline; name is ours)
inline void ReplaceControlCharacters(u16* name)
{
    for (int i = 0; i < CFL_NAME_LENGTH; i++) {
        u16 c = name[i];
        if (c == 0) {
            break;
        }
        if (c < 0x20 || c == 0x7F) {
            name[i] = '?';
        }
    }
}

// replaces characters the font of the console does not have (inline; name is ours)
inline void ReplaceNonAsciiCharacters(u16* name)
{
    for (int i = 0; i < CFL_NAME_LENGTH; i++) {
        u16 c = name[i];
        if (c == 0) {
            break;
        }
        if (c >= 0x80) {
            name[i] = '?';
        }
    }
}
} // namespace

// the name shown for Miis with the profanity flag of other consoles (names are ours)
namespace {
const u16 s_NGWordString[] = {'?', '?', '?', 0};
} // namespace

// 0x0097F6B0
const u16* s_pNGWordName = s_NGWordString;

extern "C" {
// 0x001548FC | nintendogs:bytes [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE void CFLi_GetAdditionalInfo(CFLAdditionalInfo* out, const CFLiCharInfo* info, bool checkFont)
{
    if (info->ngWord && !CFLi_IsHomeAuthorID(&info->authorID)) {
        CFLi_CopyString16(out->name, s_pNGWordName, CFL_NAME_LENGTH + 1);
        CFLi_CopyString16(out->creatorName, s_pNGWordName, CFL_NAME_LENGTH + 1);
    } else {
        std::memcpy(out->name, info->name, sizeof(u16) * CFL_NAME_LENGTH);
        out->name[CFL_NAME_LENGTH] = 0;
        std::memcpy(out->creatorName, info->creatorName, sizeof(u16) * CFL_NAME_LENGTH);
        out->creatorName[CFL_NAME_LENGTH] = 0;
        ReplaceControlCharacters(out->name);
        ReplaceControlCharacters(out->creatorName);
        if (checkFont) {
            s32 fontRegion = info->fontRegion;
            u8 region = nn::cfg::CTR::GetRegion();
            bool shown;
            switch (fontRegion) {
            case 0:
                shown = region == nn::cfg::CTR::CFG_REGION_JPN || region == nn::cfg::CTR::CFG_REGION_USA ||
                        region == nn::cfg::CTR::CFG_REGION_EUR || region == nn::cfg::CTR::CFG_REGION_AUS;
                break;
            case 1:
                shown = region == nn::cfg::CTR::CFG_REGION_CHN;
                break;
            case 2:
                shown = region == 5;
                break;
            case 3:
                shown = region == 6;
                break;
            default:
                shown = false;
                break;
            }
            if (!shown) {
                ReplaceNonAsciiCharacters(out->name);
                ReplaceNonAsciiCharacters(out->creatorName);
            }
        }
    }
    out->createID = info->createID;
    out->gender = info->gender;
    if (info->birthMonth == 0 && info->birthDay == 0) {
        goto birthday;
    }
    if (info->birthMonth != 0 && info->birthDay != 0 && static_cast<u32>(info->birthMonth - 1) <= 11 &&
        CFLi_GetMonthOfDay(info->birthMonth) >= info->birthDay) {
    birthday:
        out->birthMonth = info->birthMonth;
        out->birthDay = info->birthDay;
    } else {
        out->birthMonth = 0;
        out->birthDay = 0;
    }
    out->favoriteColor = info->favoriteColor;
    out->favorite = info->favorite;
    u32 height = info->height & 0xFF;
    if (height > 127) {
        height = 127;
    }
    u32 build = info->build & 0xFF;
    if (build > 127) {
        build = 127;
    }
    out->height = height;
    out->build = build;
    out->ngWord = info->ngWord;
    out->fontRegion = info->fontRegion;
    out->pageIndex = info->pageIndex;
    out->slotIndex = info->slotIndex;
    out->faceType = info->faceType;
    out->hairType = info->hairType;
    out->hairFlip = info->hairFlip;
    out->facelineColor = CFLi_GetFacelineRGBA8(info->faceColor);
    out->_36_8 = 0;
    out->_3C_13 = 0;
}

// 0x00152FC4 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
s32 CFL_GetAdditionalInfo(CFLAdditionalInfo* out, int source, const void* data, u16 index, bool checkFont)
{
    CFLiCharInfo info;
    s32 status = CFLi_PickupCharInfo(&info, source, data, index);
    if (status == CFL_RESULT_SUCCESS) {
        CFLi_ReplaceProhibitedCharInfo(&info);
        CFLi_GetAdditionalInfo(out, &info, checkFont);
    }
    return status;
}

// 0x00154DB8 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE u32 CFLi_GetFacelineRGBA8(int color)
{
    if (color > 5) {
        color = 5;
    }
    return s_FacelineColors[color];
}
} // extern "C"
