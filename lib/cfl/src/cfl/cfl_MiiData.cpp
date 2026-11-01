#include "cfl/cfl_Api.h"
#include "nn/cfg/CTR/CTR_Api.h"
#include "nn/ubl/ubl_Api.h"

#include <cstring>

namespace {
// replaces names with characters the font cannot show ('%', '\\', '@', U+FF20, the private use area
// U+E000-U+F8FF) or with more than maxNumbers digits (inline, used twice by CFLi_UnpackRFLMiiDataCore; name is ours)
inline bool ContainsInvalidCharacter(const u16* name, int length, int maxNumbers)
{
    int numbers = 0;
    for (int i = 0; i < length; i++) {
        u16 c = name[i];
        if (c == 0) {
            break;
        }
        if (c == '%' || c == '\\' || c == '@' || c == 0xFF20 || static_cast<u32>(c - 0xE000) < 0x1900) {
            return true;
        }
        if (CFLi_IsNumberCharCode(c) && ++numbers > maxNumbers) {
            return true;
        }
    }
    return false;
}

// the texture of the face (wrinkles, makeup) for the face features of the Wii (name is ours)
// 0x008C2A84
const u8 s_RFLFaceFeatureTable[12][2] = {
    {0, 0}, {0, 1}, {0, 6}, {0, 9}, {5, 0}, {2, 0}, {3, 0}, {7, 0}, {8, 0}, {0, 10}, {9, 0}, {11, 0},
};

// the name of a Mii whose name was rejected (name is ours)
// 0x008C2A9C
const u16 s_InvalidName[] = {'?', '?', '?', 0};

// the six Miis of the Wii format that CFL makes when there is none (CFLi_GetDefaultMiiDataCore; name is ours)
// 0x008BAC64
const u8 s_DefaultRFLMiiData[6][sizeof(CFLiRFLMiiData)] = {
    {0x00, 0x08, 0x00, 0x6E, 0x00, 0x6F, 0x00, 0x20, 0x00, 0x6E, 0x00, 0x61, 0x00, 0x6D, 0x00, 0x65, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x04, 0x88, 0x00,
     0x31, 0x80, 0x08, 0xA2, 0x08, 0x8C, 0x08, 0x58, 0x14, 0x4A, 0xB8, 0x8D, 0x00, 0x8A, 0x00, 0x8A, 0x25, 0x04},
    {0x00, 0x0A, 0x00, 0x6E, 0x00, 0x6F, 0x00, 0x20, 0x00, 0x6E, 0x00, 0x61, 0x00, 0x6D, 0x00, 0x65, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x6F, 0x80,
     0x31, 0x80, 0xC8, 0xA2, 0x08, 0x8C, 0x88, 0x58, 0x14, 0x4A, 0xB8, 0x8D, 0x00, 0x8A, 0x00, 0x8A, 0x25, 0x04},
    {0x00, 0x00, 0x00, 0x6E, 0x00, 0x6F, 0x00, 0x20, 0x00, 0x6E, 0x00, 0x61, 0x00, 0x6D, 0x00, 0x65, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x42, 0x40,
     0x31, 0x80, 0x28, 0xA2, 0x08, 0x8C, 0x08, 0x58, 0x14, 0x4A, 0xB8, 0x8D, 0x00, 0x8A, 0x00, 0x8A, 0x25, 0x04},
    {0x40, 0x04, 0x00, 0x6E, 0x00, 0x6F, 0x00, 0x20, 0x00, 0x6E, 0x00, 0x61, 0x00, 0x6D, 0x00, 0x65, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x30, 0x00,
     0x01, 0x80, 0x08, 0xA2, 0x10, 0x6C, 0x08, 0x58, 0x14, 0x4A, 0xB8, 0x8D, 0x00, 0x8A, 0x00, 0x8A, 0x25, 0x04},
    {0x40, 0x0C, 0x00, 0x6E, 0x00, 0x6F, 0x00, 0x20, 0x00, 0x6E, 0x00, 0x61, 0x00, 0x6D, 0x00, 0x65, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x1D, 0xC0,
     0x01, 0x80, 0xE8, 0xA2, 0x10, 0x6C, 0xA8, 0x58, 0x14, 0x4A, 0xB8, 0x8D, 0x00, 0x8A, 0x00, 0x8A, 0x25, 0x04},
    {0x40, 0x0E, 0x00, 0x6E, 0x00, 0x6F, 0x00, 0x20, 0x00, 0x6E, 0x00, 0x61, 0x00, 0x6D, 0x00, 0x65, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x18, 0x40,
     0x01, 0x80, 0x28, 0xA2, 0x10, 0x6C, 0x08, 0x58, 0x14, 0x4A, 0xB8, 0x8D, 0x00, 0x8A, 0x00, 0x8A, 0x25, 0x04},
};

// the template of the IDs of the default Miis; byte 3 takes the index (name is ours)
// 0x008BAE1E
const CFLiCreateID s_DefaultCreateID = {{0x0080, 0x0000, 0xFFEC, 0xD282, 0x0000}};

// the names a prohibited Mii gets (names are ours)
// 0x008A3CA0
const u16 s_ReplacedName[] = {'M', 'i', 'i', 0};
// 0x008A3CA8
const u16 s_ReplacedCreatorName[] = {0};
} // namespace

// the default Miis (names are ours)

// 0x00179484 (name is ours)
u32 CFLi_GetOfficialDataNum(const CFLiOfficialData* official)
{
    return official->num;
}

// 0x00179E90 (name is ours)
const CFLiMiiDataOfficial* CFLi_GetOfficialData(const CFLiOfficialData* official, int index)
{
    if (index >= 0 && index < static_cast<int>(official->num)) {
        return &official->data[index];
    }
    return NULL;
}

// 0x008A3C38
const CFLiOfficialDataInterface s_OfficialDataInterface = {CFLi_GetOfficialDataNum, CFLi_GetOfficialData};

extern "C" {
// 0x001555D0 | nintendogs:bytes [tier B]
DECOMP_NOINLINE bool CFLi_IsHomeAuthorID(const CFLiAuthorID* id)
{
    const CFLiAuthorID* home = CFLi_GetUserID();
    return id->data[0] == home->data[0] && id->data[1] == home->data[1] && id->data[2] == home->data[2] &&
           id->data[3] == home->data[3];
}

// 0x00155628 | nintendogs:bytes [tier B]
DECOMP_NOINLINE bool CFLi_IsNTRMiiID(const CFLiCreateID* id)
{
    u8 flags = *reinterpret_cast<const u8*>(id);
    return !(flags & 0x10) && (flags & 0x40);
}

// 0x00155668 | nintendogs:bytes [tier B]
bool CFLi_IsNullAuthorID(const CFLiAuthorID* id)
{
    if (id == NULL) {
        return false;
    }
    return id->data[0] == 0 && id->data[1] == 0 && id->data[2] == 0 && id->data[3] == 0;
}

// 0x001556A4 | nintendogs:bytes [tier B]
DECOMP_NOINLINE bool CFLi_IsNullMiiID(const CFLiCreateID* id)
{
    if (id == NULL) {
        return false;
    }
    return id->data[0] == 0 && id->data[1] == 0 && id->data[2] == 0 && id->data[3] == 0 && id->data[4] == 0;
}

// 0x00155658 (name is ours)
DECOMP_NOINLINE bool CFLi_IsNormalMiiID(const CFLiCreateID* id)
{
    return (*reinterpret_cast<const u8*>(id) & 0x80) >> 7;
}

// 0x001557E4 | nintendogs:callgraph [tier C]
DECOMP_NOINLINE bool CFLi_IsTemporaryMiiID(const CFLiCreateID* id)
{
    return (*reinterpret_cast<const u8*>(id) & 0x20) >> 5;
}

// 0x001557F4 | nintendogs:bytes [tier B]
DECOMP_NOINLINE bool CFLi_IsValidMiiID(const CFLiCreateID* id)
{
    return !CFLi_IsNullMiiID(id) && !CFLi_IsTemporaryMiiID(id);
}

// 0x001556EC | nintendogs:bytes [tier B]
DECOMP_NOINLINE bool CFLi_IsSameMiiID(const CFLiCreateID* a, const CFLiCreateID* b)
{
    if (!CFLi_IsValidMiiID(a) || !CFLi_IsValidMiiID(b)) {
        return false;
    }
    return a->data[0] == b->data[0] && a->data[1] == b->data[1] && a->data[2] == b->data[2] &&
           a->data[3] == b->data[3] && a->data[4] == b->data[4];
}

// 0x00155850 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE bool CFLi_IsValidStoreDataInner(const CFLStoreData* data)
{
    bool valid = false;
    if (CFLi_GetCRC16(0, data, sizeof(CFLStoreData)) == 0) {
        CFLiCharInfo info;
        CFLi_UnpackMiiDataCore(&info, &data->official.core, true, data->official.creatorName);
        if (!CFLi_IsNullMiiID(&info.createID) && !CFLi_IsTemporaryMiiID(&info.createID)) {
            valid = CFLi_VerifyCharInfo(&info, true);
        }
    }
    return valid;
}

// 0x00154CDC | nintendogs:bytes [tier B]
DECOMP_NOINLINE void CFLi_GetDefaultCreateIDonCTR(CFLiCreateID* id, int index)
{
    if (index < 6) {
        *id = s_DefaultCreateID;
        reinterpret_cast<u8*>(id)[3] = index;
    }
}

// 0x00154D0C | nintendogs:bytes [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE void CFLi_GetDefaultMiiDataCore(CFLiMiiDataCore* data, int index)
{
    CFLiCharInfo info;
    CFLiRFLMiiData rfl;
    CFLi_SwapRFLMiiDataEndian(&rfl, s_DefaultRFLMiiData[index], sizeof(CFLiRFLMiiData));
    CFLi_UnpackRFLMiiDataCore(&info, &rfl, NULL);
    info.miiVersion = 3;
    info.copyable = true;
    info.birthPlatform = 3;
    CFLi_PackMiiDataCore(data, NULL, &info, false);
}

// 0x0015454C | nintendogs:bytes [tier B]
DECOMP_NOINLINE void CFLi_ClearCreatorName(CFLiMiiDataOfficial* data)
{
    std::memset(data->creatorName, 0, sizeof(data->creatorName));
}

// 0x0013A95C | nintendogs:bytes [tier B]
DECOMP_NOINLINE void CFLi_GetStoreDataFromPackedData(CFLStoreData* store, const CFLiMiiDataCore* core, bool withBirthday,
                                     const u16* creatorName)
{
    if (&store->official.core != core) {
        store->official.core = *core;
    }
    if (!withBirthday) {
        store->official.core.birthMonth = 0;
        store->official.core.birthDay = 0;
    }
    if (creatorName != NULL) {
        if (store->official.creatorName != creatorName) {
            std::memcpy(store->official.creatorName, creatorName, sizeof(store->official.creatorName));
        }
    } else {
        std::memset(store->official.creatorName, 0, sizeof(store->official.creatorName));
    }
    store->_5C = 0;
    CFLi_PutCRC16(0, store, sizeof(CFLStoreData));
}

// 0x00156D84 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE void CFLi_SwapRFLMiiDataEndian(void* dst, const void* src, u32 size)
{
    u16* out = static_cast<u16*>(dst);
    const u16* in = static_cast<const u16*>(src);
    const u8* base = static_cast<const u8*>(src);
    // the flags and the name
    while (in < reinterpret_cast<const u16*>(base + 0x16)) {
        *out++ = __builtin_bswap16(*in++);
    }
    // height, build and the ID are bytes
    while (in < reinterpret_cast<const u16*>(base + 0x20)) {
        *out++ = *in++;
    }
    // the parts
    while (in < reinterpret_cast<const u16*>(base + 0x36)) {
        *out++ = __builtin_bswap16(*in++);
    }
    // the name of the author follows in the full format
    const u16* end = reinterpret_cast<const u16*>(base + (size & ~1));
    while (in < end) {
        *out++ = __builtin_bswap16(*in++);
    }
}

// 0x00156E14 | nintendogs:callgraph [tier C]
DECOMP_NOINLINE void CFLi_UnpackMiiDataCore(CFLiCharInfo* info, const CFLiMiiDataCore* data, bool withBirthday,
                            const u16* creatorName)
{
    info->miiVersion = data->version;
    info->faceType = data->faceType;
    info->faceColor = data->faceColor;
    info->faceTex = data->faceTex;
    info->faceMake = data->faceMake;
    info->hairType = data->hairType;
    info->hairColor = data->hairColor;
    info->hairFlip = data->hairFlip;
    info->eyeType = data->eyeType;
    info->eyeColor = data->eyeColor;
    info->eyeScale = data->eyeScale;
    info->eyeAspect = data->eyeAspect;
    info->eyeRotate = data->eyeRotate;
    info->eyeX = data->eyeX;
    info->eyeY = data->eyeY;
    info->eyebrowType = data->eyebrowType;
    info->eyebrowColor = data->eyebrowColor;
    info->eyebrowScale = data->eyebrowScale;
    info->eyebrowAspect = data->eyebrowAspect;
    info->eyebrowRotate = data->eyebrowRotate;
    info->eyebrowX = data->eyebrowX;
    info->eyebrowY = data->eyebrowY;
    info->noseType = data->noseType;
    info->noseScale = data->noseScale;
    info->noseY = data->noseY;
    info->mouthType = data->mouthType;
    info->mouthColor = data->mouthColor;
    info->mouthScale = data->mouthScale;
    info->mouthAspect = data->mouthAspect;
    info->mouthY = data->mouthY;
    info->mustacheType = data->mustacheType;
    info->beardType = data->beardType;
    info->beardColor = data->beardColor;
    info->mustacheScale = data->mustacheScale;
    info->mustacheY = data->mustacheY;
    info->glassType = data->glassType;
    info->glassColor = data->glassColor;
    info->glassScale = data->glassScale;
    info->glassY = data->glassY;
    info->moleType = data->moleType;
    info->moleScale = data->moleScale;
    info->moleX = data->moleX;
    info->moleY = data->moleY;
    info->height = data->height;
    info->build = data->build;
    info->gender = data->gender;
    info->favoriteColor = data->favoriteColor;
    info->favorite = data->favorite;
    info->copyable = data->copyable;
    info->ngWord = data->ngWord;
    info->localOnly = data->localOnly;
    info->regionMove = data->regionMove;
    info->fontRegion = data->fontRegion;
    info->pageIndex = data->pageIndex;
    info->slotIndex = data->slotIndex;
    info->birthPlatform = data->birthPlatform;
    if (info->birthPlatform > 3) {
        info->birthPlatform = 3;
    }
    info->_114 = data->_00_24;
    info->authorID = data->authorID;
    std::memcpy(info->name, data->name, sizeof(data->name));
    info->name[CFL_NAME_LENGTH] = 0;
    info->createID = data->createID;
    if (withBirthday) {
        info->birthMonth = data->birthMonth;
        info->birthDay = data->birthDay;
    } else {
        info->birthMonth = 0;
        info->birthDay = 0;
    }
    if (creatorName != NULL && creatorName[0] != 0) {
        std::memcpy(info->creatorName, creatorName, sizeof(u16) * CFL_NAME_LENGTH);
    } else {
        std::memset(info->creatorName, 0, sizeof(u16) * CFL_NAME_LENGTH);
    }
    info->creatorName[CFL_NAME_LENGTH] = 0;
}

// 0x00155CBC | nintendogs:bytes [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE void CFLi_PackMiiDataCore(CFLiMiiDataCore* data, u16* creatorName, const CFLiCharInfo* info, bool withBirthday)
{
    data->version = info->miiVersion;
    data->copyable = info->copyable;
    data->ngWord = info->ngWord;
    data->localOnly = info->localOnly;
    data->regionMove = info->regionMove;
    data->fontRegion = info->fontRegion;
    data->_00_14 = 0;
    data->pageIndex = info->pageIndex;
    data->slotIndex = info->slotIndex;
    data->_00_24 = info->_114;
    data->birthPlatform = info->birthPlatform;
    data->_00_31 = 0;
    data->authorID = info->authorID;
    data->createID = info->createID;
    data->_16 = 0;
    data->gender = info->gender;
    if (withBirthday) {
        data->birthMonth = info->birthMonth;
        data->birthDay = info->birthDay;
    } else {
        data->birthMonth = 0;
        data->birthDay = 0;
    }
    data->favoriteColor = info->favoriteColor;
    data->favorite = info->favorite;
    std::memcpy(data->name, info->name, sizeof(data->name));
    data->height = info->height;
    data->build = info->build;
    data->faceType = info->faceType;
    data->faceColor = info->faceColor;
    data->faceTex = info->faceTex;
    data->faceMake = info->faceMake;
    data->hairType = info->hairType;
    data->hairColor = info->hairColor;
    data->hairFlip = info->hairFlip;
    data->eyeType = info->eyeType;
    data->eyeColor = info->eyeColor;
    data->eyeScale = info->eyeScale;
    data->eyeAspect = info->eyeAspect;
    data->eyeRotate = info->eyeRotate;
    data->eyeX = info->eyeX;
    data->eyeY = info->eyeY;
    data->eyebrowType = info->eyebrowType;
    data->eyebrowColor = info->eyebrowColor;
    data->eyebrowScale = info->eyebrowScale;
    data->eyebrowAspect = info->eyebrowAspect;
    data->eyebrowRotate = info->eyebrowRotate;
    data->eyebrowX = info->eyebrowX;
    data->eyebrowY = info->eyebrowY;
    data->noseType = info->noseType;
    data->noseScale = info->noseScale;
    data->noseY = info->noseY;
    data->mouthType = info->mouthType;
    data->mouthColor = info->mouthColor;
    data->mouthScale = info->mouthScale;
    data->mouthAspect = info->mouthAspect;
    data->mouthY = info->mouthY;
    data->mustacheType = info->mustacheType;
    data->beardType = info->beardType;
    data->beardColor = info->beardColor;
    data->mustacheScale = info->mustacheScale;
    data->mustacheY = info->mustacheY;
    data->glassType = info->glassType;
    data->glassColor = info->glassColor;
    data->glassScale = info->glassScale;
    data->glassY = info->glassY;
    data->moleType = info->moleType;
    data->moleScale = info->moleScale;
    data->moleX = info->moleX;
    data->moleY = info->moleY;
    data->_18_15 = 0;
    data->_32_12 = 0;
    data->_36_14 = 0;
    data->_38_15 = 0;
    data->_3A_14 = 0;
    data->_3C_14 = 0;
    data->_40_8 = 0;
    data->_42_15 = 0;
    data->_46_15 = 0;
    if (creatorName != NULL) {
        std::memcpy(creatorName, info->creatorName, sizeof(u16) * CFL_NAME_LENGTH);
    }
}

// returns whether a name was replaced
// 0x0015723C | nintendogs:bytes [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE bool CFLi_UnpackRFLMiiDataCore(CFLiCharInfo* info, const CFLiRFLMiiData* data, const u16* creatorName)
{
    info->miiVersion = 3;
    info->faceType = data->faceType;
    info->faceColor = data->faceColor;
    info->faceTex = s_RFLFaceFeatureTable[data->faceFeature][0];
    info->faceMake = s_RFLFaceFeatureTable[data->faceFeature][1];
    info->hairType = data->hairType;
    info->hairColor = data->hairColor;
    info->hairFlip = data->hairFlip;
    info->eyeType = data->eyeType;
    info->eyeColor = data->eyeColor;
    info->eyeScale = data->eyeScale;
    info->eyeAspect = 3;
    info->eyeRotate = data->eyeRotate;
    info->eyeX = data->eyeX;
    info->eyeY = data->eyeY;
    info->eyebrowType = data->eyebrowType;
    info->eyebrowColor = data->eyebrowColor;
    info->eyebrowScale = data->eyebrowScale;
    info->eyebrowAspect = 3;
    info->eyebrowRotate = data->eyebrowRotate;
    info->eyebrowX = data->eyebrowX;
    info->eyebrowY = data->eyebrowY;
    info->noseType = data->noseType;
    info->noseScale = data->noseScale;
    info->noseY = data->noseY;
    info->mouthType = data->mouthType;
    info->mouthColor = data->mouthColor;
    info->mouthScale = data->mouthScale;
    info->mouthAspect = 3;
    info->mouthY = data->mouthY;
    info->mustacheType = data->mustacheType;
    info->beardType = data->beardType;
    info->beardColor = data->beardColor;
    info->mustacheScale = data->mustacheScale;
    info->mustacheY = data->mustacheY;
    info->glassType = data->glassType;
    info->glassColor = data->glassColor;
    info->glassScale = data->glassScale;
    info->glassY = data->glassY;
    info->moleType = data->moleType;
    info->moleScale = data->moleScale;
    info->moleX = data->moleX;
    info->moleY = data->moleY;
    info->height = data->height;
    info->build = data->build;
    info->gender = data->gender;
    info->favoriteColor = data->favoriteColor;
    info->favorite = data->favorite;
    info->copyable = false;
    info->ngWord = false;
    info->localOnly = data->localOnly;
    info->regionMove = 0;
    info->fontRegion = 0;
    info->pageIndex = 0;
    info->slotIndex = 0;
    info->_114 = 0;
    std::memset(&info->authorID, 0, sizeof(info->authorID));
    std::memcpy(info->name, data->name, sizeof(data->name));
    info->name[CFL_NAME_LENGTH] = 0;

    bool nameReplaced = false;
    if (ContainsInvalidCharacter(info->name, CFL_NAME_LENGTH, 5)) {
        CFLi_CopyString16(info->name, s_InvalidName, CFL_NAME_LENGTH);
        nameReplaced = true;
    }
    std::memset(&info->createID, 0, sizeof(info->createID));
    std::memcpy(&info->createID, data->createID, sizeof(data->createID));
    info->birthMonth = data->birthMonth;
    info->birthDay = data->birthDay;
    if (creatorName != NULL) {
        std::memcpy(info->creatorName, creatorName, sizeof(u16) * CFL_NAME_LENGTH);
    } else {
        std::memset(info->creatorName, 0, sizeof(u16) * CFL_NAME_LENGTH);
    }
    info->creatorName[CFL_NAME_LENGTH] = 0;

    bool creatorReplaced = false;
    if (ContainsInvalidCharacter(info->creatorName, CFL_NAME_LENGTH, 0)) {
        CFLi_CopyString16(info->creatorName, s_InvalidName, CFL_NAME_LENGTH);
        creatorReplaced = true;
    }
    info->birthPlatform = CFLi_IsNTRMiiID(&info->createID) ? 2 : 1;
    return nameReplaced || creatorReplaced;
}

// 0x0015775C | nintendogs:bytes [tier B]
DECOMP_NOINLINE bool CFLi_VerifyCharInfo(const CFLiCharInfo* info, bool checkName)
{
    if (info->faceType > 11 || info->faceColor > 5 || info->faceTex > 11 || info->faceMake > 11 ||
        info->hairType > 131 || info->hairColor > 7 || info->hairFlip > 1 || info->eyeType > 59 ||
        info->eyeColor > 5 || info->eyeScale > 7 || info->eyeAspect > 6 || info->eyeRotate > 7 ||
        info->eyeX < 0 || info->eyeX > 12 || info->eyeY < 0 || info->eyeY > 18 || info->eyebrowType > 23 ||
        info->eyebrowColor > 7 || info->eyebrowScale > 8 || info->eyebrowAspect > 6 || info->eyebrowRotate > 11 ||
        info->eyebrowX < 0 || info->eyebrowX > 12 || info->eyebrowY < 3 || info->eyebrowY > 18 ||
        info->noseType > 17 || info->noseScale > 8 || info->noseY > 18 || info->mouthType > 35 ||
        info->mouthColor > 4 || info->mouthScale > 8 || info->mouthAspect > 6 || info->mouthY > 18 ||
        info->mustacheType > 5 || info->beardType > 5 || info->beardColor > 7 || info->mustacheScale > 8 ||
        info->mustacheY > 16 || info->glassType > 8 || info->glassColor > 5 || info->glassScale > 7 ||
        info->glassY > 20 || info->moleType > 1 || info->moleScale > 8 || info->moleX > 16 || info->moleY > 30) {
        return false;
    }
    if (checkName) {
        if (info->name[0] == 0) {
            return false;
        }
        for (int i = 0; i < CFL_NAME_LENGTH; i++) {
            u16 c = info->name[i];
            if (c == 0) {
                break;
            }
            if (c == '%' || c == '\\') {
                return false;
            }
        }
        for (int i = 0; i < CFL_NAME_LENGTH; i++) {
            u16 c = info->creatorName[i];
            if (c == 0) {
                break;
            }
            if (c == '%' || c == '\\') {
                return false;
            }
        }
    }
    if (info->height > 128 || info->build > 128 || info->gender > 1) {
        return false;
    }
    if (info->birthMonth != 0 || info->birthDay != 0) {
        if (info->birthMonth == 0 || info->birthDay == 0) {
            return false;
        }
        if (static_cast<u32>(info->birthMonth - 1) > 11) {
            return false;
        }
        if (CFLi_GetMonthOfDay(info->birthMonth) < info->birthDay) {
            return false;
        }
    }
    if (info->favoriteColor > 11 || info->regionMove > 3 || info->fontRegion > 3 || info->pageIndex > 9 ||
        info->slotIndex > 9) {
        return false;
    }
    if (static_cast<u32>(info->birthPlatform - 1) >= 3) {
        return false;
    }
    // special Miis (bit 31 of the ID clear) must stay on this console
    if (CFLi_IsValidMiiID(&info->createID) && !CFLi_IsNormalMiiID(&info->createID) && !info->localOnly) {
        return false;
    }
    return true;
}

// 0x001567B0 | nintendogs:callgraph [confirmed by mk7dlp] [tier A]
DECOMP_NOINLINE bool CFLi_ReplaceProhibitedCharInfo(CFLiCharInfo* info)
{
    bool special;
    if (!CFLi_IsValidMiiID(&info->createID) || CFLi_IsNormalMiiID(&info->createID)) {
        // a Mii of another console must exist in this one's user list
        special = false;
        if (CFLi_IsHomeAuthorID(&info->authorID)) {
            return false;
        }
        CFLiAuthorID author = info->authorID;
        if (nn::ubl::IsExist(*reinterpret_cast<u64*>(&author), 0, 0)) {
            goto replace;
        }
        return false;
    }
    special = true;
    if (s_EnableSpecialMii) {
        s32 regionMove = info->regionMove;
        if (regionMove == 0) {
            return false;
        }
        u8 region = nn::cfg::CTR::GetRegion();
        if (regionMove == 1) {
            if (region == nn::cfg::CTR::CFG_REGION_JPN) {
                return false;
            }
        } else if (regionMove == 2) {
            if (region == nn::cfg::CTR::CFG_REGION_USA) {
                return false;
            }
        } else if (regionMove == 3) {
            if (region == nn::cfg::CTR::CFG_REGION_EUR) {
                return false;
            }
        } else {
            return false;
        }
    }
replace:
    // replace the Mii with a default one of the same gender, keeping who it is
    {
        CFLiCreateID createID = info->createID;
        bool favorite = info->favorite;
        s32 pageIndex = info->pageIndex;
        s32 slotIndex = info->slotIndex;
        CFLiAuthorID authorID = info->authorID;
        int index = reinterpret_cast<const u8*>(&createID)[3] % 3;
        if (info->gender == 1) {
            index += 3;
        }
        const CFLiMiiDataOfficial* def = s_OfficialDataInterface.GetData(&s_OfficialData, index);
        CFLi_UnpackMiiDataCore(info, &def->core, true, def->creatorName);
        CFLi_CopyString16(info->name, s_ReplacedName, CFL_NAME_LENGTH + 1);
        if (!special) {
            CFLi_CopyString16(info->creatorName, s_ReplacedCreatorName, CFL_NAME_LENGTH + 1);
        }
        info->favorite = favorite;
        info->pageIndex = pageIndex;
        info->slotIndex = slotIndex;
        info->createID = createID;
        info->authorID = authorID;
    }
    return true;
}
} // extern "C"
