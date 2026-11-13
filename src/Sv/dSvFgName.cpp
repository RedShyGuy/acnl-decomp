#include "Sv/dSvFgName.h"
#include "Item/item_Api.h"
#include "Other/dCatalogList.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvCommon.h"
#include "Sv/dSvFgNameExtern.h"
#include "Sv/dSvGardenPlus.h"
#include "Sv/dSvMail.h"
#include "netgame/dGameFunctions.h"
#include "Sv/dSvPlayer.h"
#include "script/dScriptExtern.h"
#include "script/dWordFix.h"
#include "time/time_Api.h"

namespace {
// the icon files of the items, by ParamItemEntry::iconID (name is ours)
const u32 ICON_COUNT = 0x1FB;
// the icons of KIND_AXE whose file follows the state of the item (GetIconOffset)
const u16 ICON_AXE_0 = 0x136;
const u16 ICON_AXE_1 = 0x151;
// 0x00879998
const char* const s_IconNames[ICON_COUNT] = {
    "icn_000",
    "icn_001",
    "icn_001",
    "icn_001",
    "icn_001",
    "icn_001",
    "icn_001",
    "icn_001",
    "icn_001",
    "icn_001",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_010",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_039",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_069",
    "icn_109",
    "icn_109",
    "icn_109",
    "icn_109",
    "icn_109",
    "icn_109",
    "icn_109",
    "icn_109",
    "icn_109",
    "icn_109",
    "icn_119",
    "icn_127",
    "icn_128",
    "icn_129",
    "icn_130",
    "icn_131",
    "icn_132",
    "icn_133",
    "icn_134",
    "icn_135",
    "icn_136",
    "icn_137",
    "icn_138",
    "icn_139",
    "icn_140",
    "icn_141",
    "icn_142",
    "icn_143",
    "icn_144",
    "icn_145",
    "icn_146",
    "icn_147",
    "icn_148",
    "icn_149",
    "icn_150",
    "icn_151",
    "icn_152",
    "icn_153",
    "icn_154",
    "icn_155",
    "icn_156",
    "icn_157",
    "icn_158",
    "icn_159",
    "icn_160",
    "icn_161",
    "icn_162",
    "icn_163",
    "icn_164",
    "icn_165",
    "icn_166",
    "icn_167",
    "icn_168",
    "icn_169",
    "icn_170",
    "icn_171",
    "icn_172",
    "icn_173",
    "icn_174",
    "icn_175",
    "icn_176",
    "icn_177",
    "icn_178",
    "icn_179",
    "icn_180",
    "icn_181",
    "icn_182",
    "icn_183",
    "icn_184",
    "icn_185",
    "icn_186",
    "icn_187",
    "icn_188",
    "icn_189",
    "icn_190",
    "icn_191",
    "icn_192",
    "icn_193",
    "icn_194",
    "icn_195",
    "icn_196",
    "icn_197",
    "icn_198",
    "icn_199",
    "icn_200",
    "icn_201",
    "icn_202",
    "icn_203",
    "icn_204",
    "icn_205",
    "icn_206",
    "icn_207",
    "icn_208",
    "icn_209",
    "icn_210",
    "icn_211",
    "icn_212",
    "icn_213",
    "icn_214",
    "icn_215",
    "icn_216",
    "icn_217",
    "icn_218",
    "icn_219",
    "icn_220",
    "icn_221",
    "icn_222",
    "icn_223",
    "icn_224",
    "icn_225",
    "icn_226",
    "icn_227",
    "icn_228",
    "icn_230",
    "icn_231",
    "icn_232",
    "icn_233",
    "icn_234",
    "icn_235",
    "icn_236",
    "icn_237",
    "icn_238",
    "icn_239",
    "icn_240",
    "icn_241",
    "icn_242",
    "icn_243",
    "icn_245",
    "icn_246",
    "icn_247",
    "icn_248",
    "icn_249",
    "icn_250",
    "icn_251",
    "icn_252",
    "icn_253",
    "icn_254",
    "icn_255",
    "icn_256",
    "icn_257",
    "icn_258",
    "icn_259",
    "icn_260",
    "icn_261",
    "icn_262",
    "icn_263",
    "icn_264",
    "icn_265",
    "icn_266",
    "icn_267",
    "icn_268",
    "icn_269",
    "icn_270",
    "icn_271",
    "icn_272",
    "icn_273",
    "icn_274",
    "icn_275",
    "icn_276",
    "icn_277",
    "icn_278",
    "icn_279",
    "icn_280",
    "icn_281",
    "icn_282",
    "icn_283",
    "icn_284",
    "icn_285",
    "icn_286",
    "icn_287",
    "icn_288",
    "icn_289",
    "icn_290",
    "icn_291",
    "icn_292",
    "icn_293",
    "icn_294",
    "icn_295",
    "icn_296",
    "icn_297",
    "icn_298",
    "icn_299",
    "icn_300",
    "icn_301",
    "icn_302",
    "icn_303",
    "icn_304",
    "icn_305",
    "icn_306",
    "icn_307",
    "icn_308",
    "icn_309",
    "icn_310",
    "icn_311",
    "icn_312",
    "icn_313",
    "icn_314",
    "icn_315",
    "icn_316",
    "icn_317",
    "icn_318",
    "icn_319",
    "icn_320",
    "icn_321",
    "icn_322",
    "icn_323",
    "icn_324",
    "icn_325",
    "icn_326",
    "icn_327",
    "icn_328",
    "icn_329",
    "icn_330",
    "icn_331",
    "icn_332",
    "icn_333",
    "icn_334",
    "icn_335",
    "icn_337",
    "icn_338",
    "icn_339",
    "icn_342",
    "icn_343",
    "icn_345",
    "icn_346",
    "icn_347",
    "icn_348",
    "icn_349",
    "icn_360",
    "icn_361",
    "icn_362",
    "icn_351",
    "icn_352",
    "icn_353",
    "icn_354",
    "icn_355",
    "icn_356",
    "icn_357",
    "icn_358",
    "icn_359",
    "icn_363",
    "icn_364",
    "icn_365",
    "icn_368",
    "icn_370",
    "icn_371",
    "icn_372",
    "icn_373",
    "icn_374",
    "icn_375",
    "icn_376",
    "icn_377",
    "icn_378",
    "icn_379",
    "icn_380",
    "icn_381",
    "icn_382",
    "icn_383",
    "icn_384",
    "icn_385",
    "icn_386",
    "icn_387",
    "icn_388",
    "icn_389",
    "icn_390",
    "icn_391",
    "icn_392",
    "icn_394",
    "icn_395",
    "icn_396",
    "icn_397",
    "icn_398",
    "icn_399",
    "icn_400",
    "icn_401",
    "icn_402",
    "icn_403",
    "icn_404",
    "icn_405",
    "icn_406",
    "icn_407",
    "icn_408",
    "icn_409",
    "icn_410",
    "icn_411",
    "icn_412",
    "icn_413",
    "icn_414",
    "icn_415",
    "icn_416",
    "icn_417",
    "icn_418",
    "icn_419",
    "icn_420",
    "icn_421",
    "icn_422",
    "icn_423",
    "icn_424",
    "icn_425",
    "icn_426",
    "icn_427",
    "icn_428",
    "icn_429",
    "icn_430",
    "icn_431",
    "icn_432",
    "icn_433",
    "icn_434",
    "icn_435",
    "icn_436",
    "icn_437",
    "icn_438",
    "icn_439",
    "icn_440",
    "icn_441",
    "icn_442",
    "icn_443",
    "icn_444",
    "icn_446",
    "icn_447",
    "icn_448",
    "icn_449",
    "icn_450",
    "icn_451",
    "icn_452",
    "icn_453",
    "icn_454",
    "icn_455",
    "icn_456",
    "icn_457",
    "icn_458",
    "icn_459",
    "icn_460",
    "icn_462",
    "icn_463",
    "icn_464",
    "icn_465",
    "icn_466",
    "icn_467",
    "icn_469",
    "icn_470",
    "icn_471",
    "icn_472",
    "icn_473",
    "icn_474",
    "icn_475",
    "icn_476",
    "icn_477",
    "icn_478",
    "icn_479",
    "icn_480",
    "icn_481",
    "icn_482",
    "icn_483",
    "icn_484",
    "icn_485",
    "icn_486",
    "icn_487",
    "icn_488",
    "icn_489",
    "icn_490",
    "icn_491",
    "icn_492",
    "icn_493",
    "icn_494",
    "icn_495",
    "icn_496",
    "icn_497",
    "icn_498",
    "icn_499",
    "icn_500",
    "icn_501",
    "icn_502",
    "icn_503",
    "icn_504",
    "icn_505",
    "icn_506",
    "icn_507",
    "icn_001",
    "icn_509",
    "icn_510",
    "icn_511",
    "icn_512",
    "icn_513",
    "icn_514",
    "icn_515",
    "icn_516",
    "icn_517",
    "icn_518",
    "icn_519",
    "icn_520",
    "icn_521",
    "icn_393",
    "icn_508",
    "icn_468",
    "icn_369",
    "icn_366",
    "icn_522",
};
} // namespace

// 0x0095CFFC
SvFgName SvFgName::s_Empty;

// 0x0088F650
const SvFgName::InitFlagsFunction SvFgName::s_InitFlagsFunctions[2] = {&SvFgName::InitFgFlags, &SvFgName::InitItemFlags};

// 0x002FCC14 | libgarden [tier A]
SvFgName::SvFgName(SvFgName::ID name)
{
    flags = 0;
    id = name & ~0x8000;
    u32 group = (id >> 13) & 3;
    if (group < 2) {
        (this->*s_InitFlagsFunctions[group])();
    }
}

// 0x002FC9C4
SvFgName::SvFgName(const SvFgName& other)
{
    id = other.id;
    flags = other.flags;
}

// 0x002FC978
SvFgName& SvFgName::operator=(const SvFgName& other)
{
    id = other.id;
    flags = other.flags;
    return *this;
}

// 0x002FCB84 | libgarden [tier A]
bool SvFgName::IsEmpty() const
{
    return (id & ~0x8000) == NAME_EMPTY;
}

// 0x002FCBA0 | libgarden [tier A]
bool SvFgName::IsUsual() const
{
    return static_cast<u32>((id & ~0x8000) - USUAL_BEGIN) < USUAL_COUNT;
}

// 0x002FCBC4 (name after libgarden's SvFgName header)
item::Kind SvFgName::GetKind() const
{
    const item::ParamItemEntry* entry = item::GetParamItemEntry(*this);
    if (entry == NULL) {
        return item::KIND_END;
    }
    return entry->kind < item::KIND_END ? static_cast<item::Kind>(entry->kind) : item::KIND_DEFAULT;
}

// 0x002FCBE8 (name is ours)
bool SvFgName::IsName(u16 name) const
{
    return (id & ~0x8000) == name;
}

// 0x002FCC00 (name after libgarden's SvFgName::Flags)
bool SvFgName::IsBuried() const
{
    return flags >> 15;
}

// 0x002FCCB8 (name is ours)
bool SvFgName::IsFg() const
{
    return (id & ~0x8000) <= FG_LAST;
}

// 0x002FCCD0 | libgarden [tier A]
u8 SvFgName::GetFgKind() const
{
    if ((id & ~0x8000) > FG_LAST) {
        return FG_KIND_INVALID;
    }
    const item::ParamFgEntry* entry = item::GetParamFgEntry(*this);
    if (entry == NULL) {
        return 0;
    }
    return entry->fgKind < FG_KIND_INVALID ? entry->fgKind : 0;
}

// 0x002FEA78 | libgarden [tier A]
script::WordFix<33> SvFgName::GetName(void* myDesign) const
{
    script::WordFix<33> word;
    item::GetName(&word, *this, myDesign);
    return word;
}

// 0x005367DC | libgarden [tier A]
const char* SvFgName::GetIconName() const
{
    const item::ParamItemEntry* entry = item::GetParamItemEntry(*this);
    if (entry != NULL) {
        u32 icon = entry->iconID;
        if (icon >= ICON_COUNT) {
            icon = 0;
        } else if (icon == ICON_AXE_0 || icon == ICON_AXE_1) {
            u32 index = static_cast<u32>(item::GetIconOffset(*this)) + icon;
            if (index < ICON_COUNT) {
                return s_IconNames[index];
            }
            return s_IconNames[0];
        }
        return s_IconNames[icon];
    }
    return s_IconNames[0];
}

// 0x00769DBC | libgarden [tier A]
s32 SvFgName::GetIndex() const
{
    u32 name = id & ~0x8000;
    if (name - USUAL_BEGIN < USUAL_COUNT) {
        return name - USUAL_BEGIN;
    }
    return -1;
}

// --- The item functions of the save data (0x006BA2A8..0x006BC774; names are ours unless stated) ---

namespace {
// the first name of the flowers among the field objects (the names 159.. are the flowers of the KIND_FLOWER items)
const u16 FG_FLOWER_FIRST = 159;

// the name without bit 15 / it changes and bit 15 stays
inline u16 GetPlainName(const SvFgName* item)
{
    return item->id & ~0x8000;
}
inline void SetPlainName(SvFgName* item, u32 name)
{
    item->id = (item->id & 0x8000) | (name & ~0x8000);
}
// the item bits (Flags::unique) / the common bits (Flags::common)
inline u16 GetUniqueBits(const SvFgName* item)
{
    return item->flags & ~SvFgName::FLAGS_COMMON;
}
inline void SetUniqueBits(SvFgName* item, u32 value)
{
    item->flags = (item->flags & SvFgName::FLAGS_COMMON) | (value & ~SvFgName::FLAGS_COMMON);
}
inline u32 GetCommonBits(const SvFgName* item)
{
    return item->flags >> 14;
}
inline void SetCommonBits(SvFgName* item, u32 value)
{
    item->flags = (item->flags & ~SvFgName::FLAGS_COMMON) | (value << 14);
}

// the fruit trees: a perfect fruit (bit 3) and its decay (bits 4..6, -1 if the fruit is not perfect)
inline s32 GetPerfectFruitDecay(const SvFgName* item)
{
    if ((item->flags & 8) == 0) {
        return -1;
    }
    return (item->flags >> 4) & 7;
}

// the axes that break after a while: the axe and the silver axe (the rental axe and the golden axe do not count here) (UseBreakableAxe)
inline bool IsBreakableAxe(const SvFgName* item)
{
    return GetPlainName(item) == SvFgName::NAME_AXE || GetPlainName(item) == SvFgName::NAME_SILVER_AXE;
}
inline u32 GetBreakableAxeUses(const SvFgName* item)
{
    return IsBreakableAxe(item) ? item->flags & 0xF : 0;
}

// the items with Func7694C0: the low ten item bits and the bits 10..13 (0 if out of range)
inline u32 GetLowBits(const SvFgName* item)
{
    if (item->Func7694C0()) {
        u32 value = item->flags & 0x3FF;
        if (value < 0x400) {
            return value;
        }
    }
    return 0;
}
inline u32 GetHighBits(const SvFgName* item)
{
    if (item->Func7694C0()) {
        u32 value = (item->flags >> 10) & 0xF;
        if (value <= 13) {
            return value;
        }
    }
    return 0;
}

// the count of a paper (item bits + 1; 0 for other items)
inline s32 GetPaperCount(const SvFgName* item)
{
    if (item->GetKind() != item::KIND_PAPER) {
        return 0;
    }
    return GetUniqueBits(item) + 1;
}

// the four clocks / TVs with a second name (the name after the first one)
inline bool ToNextClockName(SvFgName* item)
{
    switch (GetPlainName(item) - SvFgName::NAME_HELLO_KITTY_CLOCK_3057) {
    case 0:
        SetPlainName(item, SvFgName::NAME_HELLO_KITTY_CLOCK_3058);
        return true;
    case SvFgName::NAME_MY_MELODY_CLOCK_305A - SvFgName::NAME_HELLO_KITTY_CLOCK_3057:
        SetPlainName(item, SvFgName::NAME_MY_MELODY_CLOCK_305B);
        return true;
    case SvFgName::NAME_POMPOMPURIN_TV_3064 - SvFgName::NAME_HELLO_KITTY_CLOCK_3057:
        SetPlainName(item, SvFgName::NAME_POMPOMPURIN_TV_3065);
        return true;
    case SvFgName::NAME_KIKI_AND_LALA_CLOCK_3067 - SvFgName::NAME_HELLO_KITTY_CLOCK_3057:
        SetPlainName(item, SvFgName::NAME_KIKI_AND_LALA_CLOCK_3068);
        return true;
    }
    return false;
}
inline bool ToPreviousClockName(SvFgName* item)
{
    switch (GetPlainName(item) - SvFgName::NAME_HELLO_KITTY_CLOCK_3058) {
    case 0:
        SetPlainName(item, SvFgName::NAME_HELLO_KITTY_CLOCK_3057);
        return true;
    case SvFgName::NAME_MY_MELODY_CLOCK_305B - SvFgName::NAME_HELLO_KITTY_CLOCK_3058:
        SetPlainName(item, SvFgName::NAME_MY_MELODY_CLOCK_305A);
        return true;
    case SvFgName::NAME_POMPOMPURIN_TV_3065 - SvFgName::NAME_HELLO_KITTY_CLOCK_3058:
        SetPlainName(item, SvFgName::NAME_POMPOMPURIN_TV_3064);
        return true;
    case SvFgName::NAME_KIKI_AND_LALA_CLOCK_3068 - SvFgName::NAME_HELLO_KITTY_CLOCK_3058:
        SetPlainName(item, SvFgName::NAME_KIKI_AND_LALA_CLOCK_3067);
        return true;
    }
    return false;
}

// the flowers whose genes are in the item bits (tulip, pansy, cosmos, rose, lily, violet, carnation)
inline bool HasFlowerGenes(u8 fgKind)
{
    return fgKind == SvFgName::FG_KIND_TULIP || fgKind == SvFgName::FG_KIND_PANSY ||
           fgKind == SvFgName::FG_KIND_COSMOS || fgKind == SvFgName::FG_KIND_ROSE ||
           fgKind == SvFgName::FG_KIND_LILY || fgKind == SvFgName::FG_KIND_VIOLET ||
           fgKind == SvFgName::FG_KIND_CARNATION;
}
} // namespace

// 0x002FF76C (name after libgarden's item header)
s32 item::GetIndex(const SvFgName& item)
{
    const ParamItemEntry* entry = GetParamItemEntry(item);
    if (entry == NULL) {
        return -1;
    }
    const u8 kind = entry->kind < KIND_END ? entry->kind : KIND_DEFAULT;
    return (item.id & ~0x8000) - Func5360A8(kind)->first;
}

// 0x006BA2A8 (name is ours)
SvMonthDay::SvMonthDay()
{
    month = time::GetFirstDate().month;
    day = time::GetFirstDate().day;
}

// 0x006BA2E8
bool SvFgName::ConvertFromWallKind()
{
    switch (GetKind()) {
    case item::KIND_PICTURE_WALL:
        return ChangeKind(item::KIND_PICTURE_WALL, item::KIND_PICTURE);
    case item::KIND_FAKE_PICTURE_WALL:
        return ChangeKind(item::KIND_FAKE_PICTURE_WALL, item::KIND_FAKE_PICTURE);
    case item::KIND_MUSIC_WALL:
        return ChangeKind(item::KIND_MUSIC_WALL, item::KIND_MUSIC);
    case item::KIND_TOPS_WALL:
        return ChangeKind(item::KIND_TOPS_WALL, item::KIND_TOPS);
    case item::KIND_ONE_PIECE_WALL:
        return ChangeKind(item::KIND_ONE_PIECE_WALL, item::KIND_ONE_PIECE_OUTFIT);
    default:
        break;
    }
    return false;
}

// 0x006BA3A8 (name is ours)
bool SvFgName::ChangeKind(u8 from, u8 to)
{
    if (GetKind() != from) {
        return false;
    }
    SetPlainName(this, GetNameOfKind(to, item::GetIndex(*this)));
    return true;
}

// 0x006BA454 (name is ours)
void SvFgName::ClearUnique()
{
    flags &= FLAGS_COMMON;
}

// 0x006BA464 (name is ours)
void SvFgName::ClearUnkFlag0()
{
    flags &= ~1;
}

// 0x006BA474 (name is ours)
void SvFgName::ClearCommon()
{
    flags &= ~FLAGS_COMMON;
}

// 0x006BA484
void SvFgName::ResetFruitAfterHarvest()
{
    if (!IsFruitTree()) {
        return;
    }
    if (GetPlainName(this) == NAME_MONEY_TREE) {
        SetPlainName(this, NAME_TREE);
        return;
    }
    SetUniqueBits(this, (GetUniqueBits(this) & ~0x387) | 3);
}

// 0x006BA4D8
bool SvFgName::BreedFlower(const SvFgName& a, const SvFgName& b)
{
    const u8 fgKind = a.GetFgKind();
    if (!HasFlowerGenes(fgKind)) {
        return false;
    }
    const u8 genes = Func1BA2D0(a.flags & 0xFF, b.flags & 0xFF);
    SetPlainName(this, Func1BA0A0(fgKind, genes));
    SetUniqueBits(this, (GetUniqueBits(this) & ~0xFF) | genes);
    return true;
}

// 0x006BA584
SvFgName SvFgName::GetPickedItem() const
{
    if (GetPlainName(this) > FG_LAST) {
        return *this;
    }
    const u8 fgKind = GetFgKind();
    if (HasFlowerGenes(fgKind) || fgKind == FG_KIND_GOLD_ROSE || fgKind == FG_KIND_JACOBS_LADDER ||
        fgKind == FG_KIND_DANDELION || GetPlainName(this) == NAME_LUCKY_CLOVERS_00C9) {
        SvFgName flower(*this);
        SetPlainName(&flower, GetNameOfKind(item::KIND_FLOWER, GetPlainName(this) - FG_FLOWER_FIRST));
        return flower;
    }
    return s_Empty;
}

// 0x006BA680 (name is ours)
u16 SvFgName::GetNameOfKind(u8 kind, u32 index)
{
    const ItemNameRange* range = Func5360A8(kind);
    if (index < range->count) {
        return range->first + index;
    }
    return NAME_EMPTY;
}

// 0x006BA6AC
void SvFgName::ClearBuried()
{
    SetCommonBits(this, GetCommonBits(this) & ~2);
}

// 0x006BA6C8
bool SvFgName::ToSmallerTurnipStack()
{
    if (GetKind() != item::KIND_KABU || GetPlainName(this) == NAME_10_TURNIPS) {
        return false;
    }
    const s32 index = item::GetIndex(*this);
    if (index <= 0) {
        return false;
    }
    SetPlainName(this, GetNameOfKind(item::KIND_KABU, index - 1));
    return true;
}

// 0x006BA788
bool SvFgName::Func6BA788(u32 count)
{
    if (count == 0 || GetPlainName(this) != NAME_BAMBOO) {
        return false;
    }
    const u32 value = flags & 7;
    const u16 next = value >= count ? value - count : 2;
    SetUniqueBits(this, (GetUniqueBits(this) & ~7) | next);
    return next == 2;
}

// 0x006BA7E8
SvFgName SvFgName::Func6BA7E8() const
{
    const s32 index = item::GetIndex(*this);
    SvFgNameSet set;
    Func6F4004(&set, 14);
    return Func1AD37C(index, &set);
}

// 0x006BA864
void SvFgName::Func6BA864(u16 value)
{
    SetUniqueBits(this, value | 0x800);
}

// 0x006BA880
void SvFgName::Func6BA880(u16 value)
{
    SetUniqueBits(this, value | 0x400);
}

// 0x006BA89C (name is ours)
void SvFgName::SetUnique(u16 value)
{
    SetUniqueBits(this, value);
}

// 0x006BA8B4
void SvFgName::Func6BA8B4()
{
    SetUniqueBits(this, 0x200);
}

// 0x006BA8C8
bool SvFgName::IsSaplingFgKind(u8 fgKind)
{
    return fgKind == FG_KIND_TREE_SAPLING || fgKind == FG_KIND_CEDAR_SAPLING || fgKind == FG_KIND_PALM_SAPLING ||
           fgKind == FG_KIND_BUSH_SAPLING || fgKind == FG_KIND_BAMBOO_SHOOT;
}

// 0x006BA8F4
void SvFgName::SetOrientation(SvFgName* item, u32 orientation)
{
    if (item == NULL) {
        return;
    }
    const item::ParamItemEntry* entry = item::GetParamItemEntry(*item);
    if (entry != NULL) {
        Func749394(entry);
    }
    item->flags &= ~FLAGS_COMMON;
    if (orientation < 4) {
        SetCommonBits(item, orientation);
    }
}

// 0x006BA938
bool SvFgName::IsBushFgKind(u8 fgKind)
{
    return fgKind == FG_KIND_BUSH;
}

// 0x006BA948
void SvFgName::ClearOrientationIfFurniture()
{
    const item::ParamItemEntry* entry = item::GetParamItemEntry(*this);
    if (entry != NULL && Func749394(entry)) {
        flags &= ~FLAGS_COMMON;
    }
}

// 0x006BA978
void SvFgName::GrowFruit(u32 count, u32 mode)
{
    if (!IsFruitTree()) {
        return;
    }
    const u32 growth = flags & 7;
    if (growth == 0) {
        return;
    }
    u16 next = growth <= count ? 0 : growth - count;
    if (next == 0 && Func76BFA4()) {
        if (Func76BFA4() && (flags & 8) != 0) {
            // the perfect fruit decays
            if (mode == 0 || (mode == 1 && Func2FC2EC(2) == 0)) {
                if (Func76BFA4() && (GetUniqueBits(this) & 8) != 0) {
                    SetUniqueBits(this, (GetUniqueBits(this) & ~0x70) | (((GetPerfectFruitDecay(this) + 1) & 7) << 4));
                }
            }
        } else {
            // a tree of the fruit of the town may get a perfect fruit
            SvFgName fruit;
            if (IsFruitTree()) {
                const SvFgName name(static_cast<ID>(item::GetParamFgEntry(*this)->unk_0x2));
                if (name.IsUsual()) {
                    fruit = name;
                }
            }
            if (item::GetIndex(GetGardenPlus()->data.townData.townFruit) == item::GetIndex(fruit)) {
                s32 chance = 256;
                if (mode == 1) {
                    chance = 32;
                } else if (mode == 2) {
                    chance = 16;
                }
                if (Func2FC2EC(chance) == 0) {
                    SetPerfectFruitPlace(Func2FC2EC(3));
                }
            }
        }
    }
    SetUniqueBits(this, (GetUniqueBits(this) & ~7) | next);
}

// 0x006BAC1C
bool SvFgName::Func6BAC1C(u32 high, u32 low)
{
    if (!Func7694C0()) {
        return false;
    }
    SetUniqueBits(this, low + (high << 10));
    return true;
}

// 0x006BAC58
void SvFgName::Func6BAC58()
{
    flags &= FLAGS_COMMON;
}

// 0x006BAC68 (name is ours)
bool SvFgName::PrepareForInventory()
{
    flags &= ~FLAGS_COMMON;
    if (GetPlainName(this) <= FG_LAST) {
        *this = GetPickedItem();
    }
    if (GetKind() == item::KIND_MUSH_FTR) {
        *this = Func6BA7E8();
    }
    if (GetPlainName(this) == NAME_MII_MASK) {
        *this = s_Empty;
    }
    Func6BB17C();
    if (GetPlainName(this) == NAME_EMPTY || (id & 0x6000) == 0x4000) {
        flags &= ~FLAGS_COMMON;
        return true;
    }
    const item::KindParamEntry* param = item::GetKindParamEntry(*this);
    if (param != NULL && ((param->flags1 >> 4) & 1) != 0) {
        flags &= ~FLAGS_COMMON;
        return true;
    }
    return false;
}

// 0x006BADE4
s32 SvFgName::Func6BADE4() const
{
    switch (GetKind()) {
    case item::KIND_TOPS:
        return item::GetIndex(*this) + 8;
    case item::KIND_ONE_PIECE_OUTFIT:
        return item::GetIndex(*this) + 701;
    case item::KIND_MY_DESIGN_TEXTURE:
        switch ((flags >> 8) & 0xF) {
        case 0: {
            const s32 base = Func76CAE0();
            const u32 value = (flags >> 4) & 0xF;
            s32 index = -1;
            if (value < 8) {
                index = value;
            }
            return base + index * 10 + 976;
        }
        case 3:
            return Func76CAE0() + 1016;
        }
        break;
    default:
        break;
    }
    return -1;
}

// 0x006BAF54
bool SvFgName::MakeMusicBox(const SvFgName& shell, const SvFgName& song)
{
    if (GetPlainName(&shell) != NAME_GIANT_CLAM_SHELL || song.GetKind() != item::KIND_MUSIC) {
        return false;
    }
    SetPlainName(this, GetNameOfKind(item::KIND_ORGEL, item::GetIndex(song)));
    return true;
}

// 0x006BB018
bool SvFgName::ConvertToWall()
{
    bool changed;
    switch (GetKind()) {
    case item::KIND_PICTURE:
        changed = ChangeKind(item::KIND_PICTURE, item::KIND_PICTURE_WALL);
        break;
    case item::KIND_FAKE_PICTURE:
        changed = ChangeKind(item::KIND_FAKE_PICTURE, item::KIND_FAKE_PICTURE_WALL);
        break;
    case item::KIND_MUSIC:
        changed = ChangeKind(item::KIND_MUSIC, item::KIND_MUSIC_WALL);
        break;
    case item::KIND_TOPS:
        changed = ChangeKind(item::KIND_TOPS, item::KIND_TOPS_WALL);
        break;
    case item::KIND_ONE_PIECE_OUTFIT:
        changed = ChangeKind(item::KIND_ONE_PIECE_OUTFIT, item::KIND_ONE_PIECE_WALL);
        break;
    default:
        changed = false;
        break;
    }
    if (changed) {
        return true;
    }
    return ToNextClockName(this);
}

// 0x006BB164
SvFgName Func6BB164(const void* object)
{
    return *reinterpret_cast<const SvFgName*>(static_cast<const u8*>(object) + 0x5E4);
}

// 0x006BB17C
bool SvFgName::Func6BB17C()
{
    if (ConvertFromWallKind()) {
        return true;
    }
    return ToPreviousClockName(this);
}

// 0x006BB2C8
bool SvFgName::Func6BB2C8(u32 value)
{
    const u32 count = Func5360A8(item::KIND_REMAKE_FOSSIL)->count;
    for (u32 i = 0; i < count; i++) {
        const SvFgName fossil = MakeOfKind(item::KIND_REMAKE_FOSSIL, i);
        const item::ParamItemEntry* entry = item::GetParamItemEntry(fossil);
        u8 fossilValue = 0;
        if (entry != NULL) {
            fossilValue = entry->unk_0x12[0] < 31 ? entry->unk_0x12[0] : 0;
        }
        if (fossilValue == value) {
            SetPlainName(this, fossil.id);
            return true;
        }
    }
    return false;
}

// 0x006BB3D8
bool SvFgName::SpoilPerfectFruit()
{
    if (GetKind() != item::KIND_GOOD_FRUIT) {
        return false;
    }
    *this = SvFgName(static_cast<ID>(GetNameOfKind(item::KIND_BAD_FRUIT, item::GetIndex(*this))));
    return true;
}

// 0x006BB488
bool SvFgName::Wrap()
{
    const item::Kind kind = GetKind();
    if (!IsUsual()) {
        return false;
    }
    const item::KindParamEntry* param = item::GetKindParamEntry(*this);
    if (param == NULL || ((param->flags1 >> 4) & 1) == 0) {
        return false;
    }
    param = item::GetKindParamEntry(*this);
    if (param == NULL || ((param->flags1 >> 1) & 1) == 0 || kind == item::KIND_INSECT) {
        return false;
    }
    if (kind == item::KIND_FISH || kind == item::KIND_DIVE_FISH || kind == item::KIND_KABU ||
        kind == item::KIND_BAD_KABU || kind == item::KIND_WRAPPING_PAPER || kind == item::KIND_PRESENT_BOX ||
        kind == item::KIND_PRESENT_BOX_QUEST || kind == item::KIND_MANNEQUIN || kind == item::KIND_YUTARO_LAMP) {
        return false;
    }
    if (Func7694C0() && GetUniqueBits(this) != 0 && Func76CB50() == 8) {
        return false;
    }
    if (Func769644(this)) {
        return false;
    }
    id = (id & ~0x6000) | 0x4000;
    return true;
}

// 0x006BB5B4
bool SvFgName::Unwrap()
{
    if ((id & 0x6000) != 0x4000) {
        return false;
    }
    id = (id & ~0x6000) | 0x2000;
    return true;
}

// 0x006BB5DC
void SvFgName::SetPerfectFruitPlace(u32 place)
{
    if (Func76BFA4() && (flags & 8) == 0) {
        SetUniqueBits(this, (GetUniqueBits(this) & ~0x380) | (((1 << place) & 7) << 7));
    }
}

// 0x006BB62C
bool SvFgName::SpoilFruit()
{
    const u16 name = GetPlainName(this);
    if (name != NAME_APPLE && name != NAME_ORANGE && name != NAME_PEAR && name != NAME_PEACH && name != NAME_CHERRY) {
        return false;
    }
    *this = SvFgName(static_cast<ID>(GetNameOfKind(item::KIND_BAD_FRUIT, item::GetIndex(*this))));
    return true;
}

// 0x006BB6F4
bool SvFgName::UseBreakableAxe()
{
    if (!IsBreakableAxe(this)) {
        return false;
    }
    if (GetBreakableAxeUses(this) >= 8) {
        return true;
    }
    const u32 uses = GetBreakableAxeUses(this);
    const u32 hits = (flags >> 4) & 0xF;
    if (hits < 7 && netgame::Func2FF63C(8 - hits) != 0) {
        SetUniqueBits(this, (GetUniqueBits(this) & ~0xF0) | (((hits + 1) & 0xF) << 4));
    } else {
        SetUniqueBits(this, (GetUniqueBits(this) & ~0xF) | ((uses + 1) & 0xF));
        SetUniqueBits(this, GetUniqueBits(this) & ~0xF0);
    }
    return GetBreakableAxeUses(this) >= 8;
}

// 0x006BB828
void SvFgName::Func6BB828(u16 value)
{
    SetUniqueBits(this, value);
}

// 0x006BB840
bool SvFgName::ToNormalFruit()
{
    if (GetKind() != item::KIND_GOOD_FRUIT) {
        return false;
    }
    *this = SvFgName(static_cast<ID>(GetNameOfKind(item::KIND_FRUIT, item::GetIndex(*this))));
    return true;
}

// 0x006BB8F0
bool SvFgName::ToPerfectFruit()
{
    const u16 name = GetPlainName(this);
    if (name != NAME_APPLE && name != NAME_ORANGE && name != NAME_PEAR && name != NAME_PEACH && name != NAME_CHERRY) {
        return false;
    }
    *this = SvFgName(static_cast<ID>(GetNameOfKind(item::KIND_GOOD_FRUIT, item::GetIndex(*this))));
    return true;
}

// 0x006BB9B8
void SvFgName::InitFlowerGenes()
{
    if (GetKind() != item::KIND_FLOWER || (flags & 0xFF) != 0) {
        return;
    }
    const SvFgName fg(static_cast<ID>(item::GetIndex(*this) + FG_FLOWER_FIRST));
    SetUniqueBits(this, fg.flags & 0xFF);
}

// 0x006BBA4C
u32 SvFgName::Func6BBA4C(u32 value)
{
    switch (value) {
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    case 4:
        return 4;
    case 5:
        return 5;
    case 8:
        return 6;
    case 9:
    case 14:
        return 7;
    case 10:
        return 8;
    case 13:
        return 9;
    }
    return 0;
}

// 0x006BBAE4 (name is ours)
void SvFgName::InitItemFlags()
{
    u32 value = 0;
    switch (GetKind()) {
    case item::KIND_PAPER:
        value = 3;
        break;
    case item::KIND_FLOWER: {
        const SvFgName fg(static_cast<ID>(item::GetIndex(*this) + FG_FLOWER_FIRST));
        value = fg.flags & 0xFF;
        break;
    }
    case item::KIND_MT_FRUIT:
    case item::KIND_GOOD_MT_FRUIT:
        value = 1;
        break;
    default:
        break;
    }
    SetUniqueBits(this, value);
}

// 0x006BBB90
bool SvFgName::Func6BBB90(u32 value)
{
    const u32 low = GetLowBits(this);
    if (!Func7694C0()) {
        return false;
    }
    SetUniqueBits(this, low + (value << 10));
    return true;
}

// 0x006BBBF0 (name is ours)
void SvFgName::InitFgFlags()
{
    u8 genes;
    switch (GetFgKind()) {
    case FG_KIND_COSMOS:
        genes = Func1BA270(GetPlainName(this));
        break;
    case FG_KIND_LILY:
        genes = Func1BA290(GetPlainName(this));
        break;
    case FG_KIND_TULIP:
        genes = Func1BA250(GetPlainName(this));
        break;
    case FG_KIND_PANSY:
        genes = Func1BA230(GetPlainName(this));
        break;
    case FG_KIND_ROSE:
        genes = Func1BA210(GetPlainName(this));
        break;
    case FG_KIND_CARNATION:
        genes = Func1BA2B0(GetPlainName(this));
        break;
    case FG_KIND_VIOLET:
        genes = Func1BA1F0(GetPlainName(this));
        break;
    default:
        return;
    }
    SetUniqueBits(this, (GetUniqueBits(this) & ~0xFF) | genes);
}

// 0x006BBCF0
bool SvFgName::Func6BBCF0()
{
    return ToNextClockName(this);
}

// 0x006BBD80
bool SvFgName::SetPaperCount(u32 count)
{
    if (GetKind() != item::KIND_PAPER || count > 4) {
        return false;
    }
    SetUniqueBits(this, count - 1);
    return true;
}

// 0x006BBDDC
bool SvFgName::Func6BBDDC(const SvFgName& other)
{
    u32 high;
    switch (GetPlainName(&other)) {
    case NAME_GOLD_NUGGET:
        high = 8;
        break;
    case NAME_SILVER_NUGGET:
        high = 9;
        break;
    case NAME_RUBY:
        high = 10;
        break;
    case NAME_SAPPHIRE:
        high = 11;
        break;
    case NAME_EMERALD:
        high = 12;
        break;
    case NAME_AMETHYST:
        high = 13;
        break;
    default:
        return false;
    }
    const u32 low = GetLowBits(this);
    if (!Func7694C0()) {
        return false;
    }
    SetUniqueBits(this, low + (high << 10));
    return true;
}

// 0x006BBEA8
bool SvFgName::Func6BBEA8(u32 value)
{
    const u32 high = GetHighBits(this);
    if (!Func7694C0()) {
        return false;
    }
    SetUniqueBits(this, value + (high << 10));
    return true;
}

// 0x006BBF0C
bool SvFgName::Func6BBF0C(const SvFgName& other)
{
    const s32 value = other.Func6BADE4();
    if (value == -1) {
        return false;
    }
    return Func6BBEA8(value);
}

// 0x006BBF88
bool SvFgName::SetMtFruitCount(u32 count)
{
    const item::Kind kind = GetKind();
    if (kind != item::KIND_MT_FRUIT && kind != item::KIND_GOOD_MT_FRUIT) {
        return false;
    }
    if (count - 2 <= 7) {
        SetUniqueBits(this, count - 1);
        return true;
    }
    if (count == 1) {
        return MtFruitToFruit();
    }
    return false;
}

// 0x006BC000
bool SvFgName::MtFruitToFruit()
{
    u8 target;
    switch (GetKind()) {
    case item::KIND_MT_FRUIT:
        target = item::KIND_FRUIT;
        break;
    case item::KIND_GOOD_MT_FRUIT:
        target = item::KIND_GOOD_FRUIT;
        break;
    default:
        return false;
    }
    const u16 name = GetNameOfKind(target, item::GetIndex(*this));
    if (name != NAME_EMPTY) {
        SetPlainName(this, name);
    }
    return false;
}

// 0x006BC114
bool SvFgName::ConvertCustomDesignFromWall()
{
    switch (GetPlainName(this)) {
    case NAME_PATTERN_WALL_CANVAS:
        SetPlainName(this, NAME_PATTERN_EASEL);
        return true;
    case NAME_CUSTOM_DESIGN_T_SHIRT_WALL:
        SetPlainName(this, NAME_CUSTOM_DESIGN_T_SHIRT);
        return true;
    case NAME_CUSTOM_DESIGN_TANK_SHIRT_WALL:
        SetPlainName(this, NAME_CUSTOM_DESIGN_TANK_SHIRT);
        return true;
    case NAME_CUSTOM_DESIGN_T_SHIRT_DISTORTED_SLEEVES_WALL:
        SetPlainName(this, NAME_CUSTOM_DESIGN_T_SHIRT_DISTORTED_SLEEVES);
        return true;
    case NAME_CUSTOM_DESIGN_LONG_SLEEVE_SHIRT_WALL:
        SetPlainName(this, NAME_CUSTOM_DESIGN_LONG_SLEEVE_SHIRT);
        return true;
    case NAME_CUSTOM_DESIGN_TANK_DRESS_WALL:
        SetPlainName(this, NAME_CUSTOM_DESIGN_TANK_DRESS);
        return true;
    case NAME_CUSTOM_DESIGN_SLEEVE_DRESS_WALL:
        SetPlainName(this, NAME_CUSTOM_DESIGN_SLEEVE_DRESS);
        return true;
    case NAME_CUSTOM_DESIGN_LONG_SLEEVE_DRESS_WALL:
        SetPlainName(this, NAME_CUSTOM_DESIGN_LONG_SLEEVE_DRESS);
        return true;
    }
    return false;
}

// 0x006BC23C
s32 SvFgName::Func6BC23C()
{
    const item::ParamItemEntry* entry = item::GetParamItemEntry(*this);
    if (entry == NULL || entry->unk_0xF[0] >= 50 || entry->unk_0xF[0] != 16) {
        return 0;
    }
    entry = item::GetParamItemEntry(*this);
    u32 value = 0;
    if (entry != NULL && entry->unk_0xF[0] < 50 && entry->unk_0xF[0] == 16) {
        value = GetUniqueBits(this);
    }
    value++;
    SetUniqueBits(this, value);
    return value;
}

// 0x006BC2BC
s32 SvFgName::DecreasePaperCount()
{
    const s32 count = GetPaperCount(this) - 1;
    if (count == 0 || SetPaperCount(count)) {
        return count;
    }
    return 0;
}

// 0x006BC35C
bool SvFgName::DecreaseMtFruitCount()
{
    const item::Kind kind = GetKind();
    if (kind == item::KIND_MT_FRUIT || kind == item::KIND_GOOD_MT_FRUIT) {
        const u32 count = GetUniqueBits(this) + 1;
        if (count - 2 <= 7) {
            SetMtFruitCount(count - 1);
        }
    }
    return false;
}

// 0x006BC410
bool SvFgName::Func6BC410(u16 value)
{
    const item::Kind kind = GetKind();
    if (kind != item::KIND_PICTURE && kind != item::KIND_FAKE_PICTURE && kind != item::KIND_SCULPTURE &&
        kind != item::KIND_FAKE_SCULPTURE) {
        return false;
    }
    flags = (flags & FLAGS_COMMON) | value;
    return true;
}

// 0x006BC46C
void SvFgName::Func6BC46C(u32 value)
{
    const PlayerSaveNumber saveNo = SvGetPlayerSaveNo();
    if (saveNo < 8) {
        Func304EA0(this, 0, saveNo, value);
    } else {
        Func304EA0(this, 0, 0, value);
    }
}

// 0x006BC4B4
bool SvFgName::SetFortuneCookieValue(u32 value)
{
    if (GetKind() != item::KIND_FORTUNE_COOKIE || value >= 57) {
        return false;
    }
    SetUniqueBits(this, value);
    return true;
}

// 0x006BC50C
bool SvFgName::SetFortuneTsunekichiValue(u32 value)
{
    if (GetKind() != item::KIND_FORTUNE_TSUNEKICHI || value >= 9) {
        return false;
    }
    SetUniqueBits(this, value);
    return true;
}

// 0x006BC564 (name after libgarden's SvFgName)
SvFgName::SvFgName(u32 data)
{
    id = data;
    flags = data >> 16;
}

// 0x006BC580
SvFgName SvFgName::Func6BC580(u32 amount, bool roundUp, u32* rest)
{
    u32 unused;
    if (rest == NULL) {
        rest = &unused;
    }
    SvFgName money;
    *rest = 0;
    if (roundUp) {
        money = SvFgName(static_cast<ID>(NAME_99_000_BELLS));
        if (static_cast<u32>(Func3055C8(&money)) < amount) {
            return s_Empty;
        }
        for (s32 name = NAME_100_BELLS; name <= NAME_99_000_BELLS; name++) {
            money = SvFgName(static_cast<ID>(name));
            const u32 price = Func3055C8(&money);
            if (amount <= price) {
                *rest = price - amount;
                return money;
            }
        }
    } else {
        money = SvFgName(static_cast<ID>(NAME_100_BELLS));
        if (static_cast<u32>(Func3055C8(&money)) > amount) {
            return s_Empty;
        }
        for (s32 name = NAME_99_000_BELLS; name >= NAME_100_BELLS; name--) {
            money = SvFgName(static_cast<ID>(name));
            const u32 price = Func3055C8(&money);
            if (amount >= price) {
                *rest = amount - price;
                return money;
            }
        }
    }
    return s_Empty;
}

// 0x006BC70C
void SvFgName::Func6BC70C()
{
    SetCommonBits(this, GetCommonBits(this) & ~1);
}

// 0x006BC728
void SvFgName::MakeMiiMask(u16 value)
{
    SetPlainName(this, NAME_MII_MASK);
    flags &= ~FLAGS_COMMON;
    if (GetPlainName(this) == NAME_MII_MASK) {
        SetUniqueBits(this, value);
    }
}

// 0x006BC764
void SvFgName::SetUnkFlag0()
{
    flags |= 1;
}

// 0x006BC820
void SvFgName::Func6BC820(u32 value)
{
    if (GetPlainName(this) == NAME_MII_MASK) {
        *this = s_Empty;
        return;
    }
    if (GetKind() == item::KIND_SANTA_SACK) {
        ChangeKind(item::KIND_SANTA_SACK, item::KIND_SANTA_SACK_DUST);
        return;
    }
    Func76C010(value);
}

// 0x006BC8A4
SvFgName SvFgName::MakeOfKind(u8 kind, u32 index)
{
    SvFgName item;
    SetPlainName(&item, GetNameOfKind(kind, index));
    const u32 group = (item.id >> 13) & 3;
    if (group < 2) {
        (item.*s_InitFlagsFunctions[group])();
    }
    return item;
}

// 0x0076B388 (name is ours)
bool SvFgName::IsFruitTree() const
{
    const u16 name = GetPlainName(this);
    return name == NAME_APPLE_TREE || name == NAME_ORANGE_TREE || name == NAME_PEACHES_TREE || name == NAME_PEAR_TREE ||
           name == NAME_CHERRY_TREE || name == NAME_DURIAN_TREE || name == NAME_LEMON_TREE || name == NAME_MANGO_TREE ||
           name == NAME_LYCHEE_TREE || name == NAME_PERSIMMONS_TREE || name == NAME_BANANA_PALM_TREE ||
           name == NAME_COCONUT_PALM_TREE || name == NAME_MONEY_TREE;
}

// 0x0076BFA4
bool SvFgName::Func76BFA4() const
{
    const u16 name = GetPlainName(this);
    return name == NAME_APPLE_TREE || name == NAME_ORANGE_TREE || name == NAME_PEACHES_TREE || name == NAME_PEAR_TREE ||
           name == NAME_CHERRY_TREE;
}

// (falls through into SvFgName::Func6BC820 with the attached item)
// 0x006BC774 (name is ours)
void Func6BC774(SvMail* mail, u32 a, SvPersonalId* player, u32 b)
{
    SvMailPerson* receiver = NULL;
    SvMailPerson* sender = NULL;
    const u8 receiverKind = mail->receiver.unk_0x30;
    const u8 senderKind = mail->sender.unk_0x30;
    if (receiverKind == 1 || receiverKind == 3 || receiverKind == 6) {
        receiver = &mail->receiver;
    }
    if (senderKind == 1 || senderKind == 3 || senderKind == 6) {
        sender = &mail->sender;
    }
    if (receiver != NULL && Func311B44(&receiver->id, player)) {
        Func1B50DC(&receiver->id, a);
    }
    if (sender != NULL && Func311B44(&sender->id, player)) {
        Func1B50DC(&sender->id, a);
    }
    mail->attachedItem.Func6BC820(b);
}
