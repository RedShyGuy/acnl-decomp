#include "Sv/dSvFgName.h"
#include "Item/item_Api.h"
#include "script/dWordFix.h"

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
