#include "field/dFieldFactory.h"
#include "field/dField.h"
#include "field/dFieldBuilderUtil.h"
#include "field/field_Extern.h"
#include "g3d/dResourceLoader.h"
#include "Item/item_Api.h"
#include "mutil/dBaseResourceMgr.h"
#include "Other/dFieldBuilder.h"
#include "Other/dMailLoadAdapt.h"
#include "Other/dTicketList.h"
#include "Other/dHeapInit.h"
#include "sead/seadExpHeap.h"
#include "sead/seadHeapMgr.h"
#include "sead/seadNew.h"
#include "sead/seadRandom.h"
#include "stage/stage_Api.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvFgBlock.h"
#include "Sv/dSvFgName.h"
#include "Sv/dSvGardenPlus.h"
#include "Sv/dSvMuseum.h"
#include "Sv/dSvNpc.h"
#include "Sv/dSvShopZakkaExtern.h"
#include "Sound/sound_Extern.h"

namespace {
// the size of a block in the item buffers of the camping cars and the villager houses (one layer each)
const s32 FG_BLOCK_SIZE = UNIT_BASE_NUM;

// a field from a stage layout: the grounds of its blocks and their heights
Field* CreateField(sead::Heap* heap, const stage::FieldLayout* layout);

// the items of both blocks into the block 0, 0 of the field
inline void CopyBlocks(Field* field, SvFgBlock* layer0, SvFgBlock* layer1)
{
    BgInfo* bg = field->GetBg(0, 0);
    if (bg == NULL) {
        return;
    }
    for (s32 z = 0; z < FG_BLOCK_SIZE; z++) {
        for (s32 x = 0; x < FG_BLOCK_SIZE; x++) {
            bg->SetFg(*layer0->GetFgChecked(x, z), x, z, 0);
            bg->SetFg(*layer1->GetFgChecked(x, z), x, z, 1);
        }
    }
}

// The base of the builders here: the steps that most fields share (class name from the RTTI; it has no vtable of its
// own in the binary, its functions are the slots of the builders that do not override them)
class BasicBuilder : public ::FieldBuilder
{
public:
    BasicBuilder(Field* field, const FgDataLoader* layout) : FieldBuilder(field), m_pLayout(layout) {}

    void SetupHeights() override;
    void Setup10() override;
    void ClearItems() override;
    void SetupItems() override;
    void Setup1C() override;
    void Setup20() override;
    void Setup24() override;
    void SetupGrass() override;

    const FgDataLoader* m_pLayout; // 0x8 the items of the grounds (NULL: none)
};

// the camping ground: the tent or the igloo
class CampNpcBuilder : public BasicBuilder
{
public:
    CampNpcBuilder(Field* field, const FgDataLoader* layout) : BasicBuilder(field, layout) {}
    ~CampNpcBuilder() override;

    void SetupBlocks() override;
};

// the grounds of the blocks from a stage layout
class ElmFieldBuilder : public BasicBuilder
{
public:
    ElmFieldBuilder(Field* field, const FgDataLoader* layout, const stage::FieldLayout* fieldLayout)
        : BasicBuilder(field, layout), m_pFieldLayout(fieldLayout)
    {
    }
    ~ElmFieldBuilder() override;

    void SetupBlocks() override;

    const stage::FieldLayout* m_pFieldLayout; // 0xC
};

// the shop of Gracie: the usual room or the bargain room
class ShopGraceBuilder : public BasicBuilder
{
public:
    ShopGraceBuilder(Field* field, const FgDataLoader* layout) : BasicBuilder(field, layout) {}
    ~ShopGraceBuilder() override;

    void SetupBlocks() override;
};

// the Happy Home Showcase: the flowers of the layout in the colors of the section
class ExhibitionBuilder : public ElmFieldBuilder
{
public:
    ExhibitionBuilder(Field* field, const FgDataLoader* layout, const stage::FieldLayout* fieldLayout)
        : ElmFieldBuilder(field, layout, fieldLayout)
    {
    }
    ~ExhibitionBuilder() override;

    void SetupItems() override;
};

// the fossil rooms of the museum: the stands of the fossils that are not donated
class MuseumFossilBuilder : public ElmFieldBuilder
{
public:
    MuseumFossilBuilder(Field* field, const FgDataLoader* layout, const stage::FieldLayout* fieldLayout)
        : ElmFieldBuilder(field, layout, fieldLayout)
    {
    }
    ~MuseumFossilBuilder() override;

    void SetupItems() override;
};

// the art rooms of the museum: only the donated sculptures
class MuseumPictureBuilder : public ElmFieldBuilder
{
public:
    MuseumPictureBuilder(Field* field, const FgDataLoader* layout, const stage::FieldLayout* fieldLayout)
        : ElmFieldBuilder(field, layout, fieldLayout)
    {
    }
    ~MuseumPictureBuilder() override;

    void SetupItems() override;
};

// a room of a house of the players (the last structure that was entered): its ground and its items
class ModelHouseFieldBuilder : public BasicBuilder
{
public:
    ModelHouseFieldBuilder(Field* field, const FgDataLoader* layout) : BasicBuilder(field, layout) {}
    ~ModelHouseFieldBuilder() override;

    void SetupBlocks() override;
    void SetupItems() override;
};

// the town at the start of the game: the grounds, the items and the grass of the town map
class VillagePrologueBuilder : public BasicBuilder
{
public:
    VillagePrologueBuilder(Field* field, const FgDataLoader* layout) : BasicBuilder(field, layout) {}
    ~VillagePrologueBuilder() override;

    void SetupBlocks() override;
    void SetupItems() override;
    void SetupGrass() override;
};

// the items of the layout with the flowers of the section (ExhibitionBuilder)
struct ExhibitionItemsSetter
{
    ExhibitionItemsSetter(Field* field, const FgDataLoader* layout) : m_pField(field), m_pLayout(layout) {}

    void operator()(const Vector2i& block)
    {
        if (m_pField == NULL) {
            return;
        }
        BgInfo* bg = m_pField->GetBg(block.x, block.y);
        if (!m_pLayout->IsLoaded()) {
            return;
        }
        const u32 num = m_pLayout->GetItems(m_Items, sizeof(m_Items), bg->GetName());
        for (u32 i = 0; i < num; i++) {
            const FgDataItem& entry = m_Items[i];
            u32 id = entry.id;
            // the west section has red, the east section yellow flowers
            switch (SvFgName(static_cast<SvFgName::ID>(id)).GetFgKind()) {
            case SvFgName::FG_KIND_TULIP:
                if (stage::GetCurrent() == stage::NAME_EXHIBITION_WEST) {
                    id = SvFgName::NAME_RED_TULIPS_009F;
                } else if (stage::GetCurrent() == stage::NAME_EXHIBITION_EAST) {
                    id = SvFgName::NAME_YELLOW_TULIPS_00A1;
                }
                break;
            case SvFgName::FG_KIND_PANSY:
                if (stage::GetCurrent() == stage::NAME_EXHIBITION_WEST) {
                    id = SvFgName::NAME_RED_PANSIES_00A8;
                } else if (stage::GetCurrent() == stage::NAME_EXHIBITION_EAST) {
                    id = SvFgName::NAME_YELLOW_PANSIES_00A7;
                }
                break;
            case SvFgName::FG_KIND_COSMOS:
                if (stage::GetCurrent() == stage::NAME_EXHIBITION_WEST) {
                    id = SvFgName::NAME_RED_COSMOS_00AD;
                } else if (stage::GetCurrent() == stage::NAME_EXHIBITION_EAST) {
                    id = SvFgName::NAME_YELLOW_COSMOS_00AE;
                }
                break;
            case SvFgName::FG_KIND_ROSE:
                if (stage::GetCurrent() == stage::NAME_EXHIBITION_WEST) {
                    id = SvFgName::NAME_RED_ROSES_00B2;
                } else if (stage::GetCurrent() == stage::NAME_EXHIBITION_EAST) {
                    id = SvFgName::NAME_YELLOW_ROSES_00B4;
                }
                break;
            default:
                break;
            }
            bg->SetFg(SvFgName(static_cast<SvFgName::ID>(id)), entry.x, entry.z, 0);
        }
    }

    Field* m_pField;
    const FgDataLoader* m_pLayout;
    FgDataItem m_Items[FgDataLoader::ITEM_BUFFER_SIZE / sizeof(FgDataItem)];
};

// the stand of a fossil that is not donated: the podium of the small fossils, the 1x2 stand of the parts of the
// Ptera, the 2x2 stand of the other ones
inline u32 GetFossilStand(u32 id)
{
    const u16 smallFossils[] = {
        SvFgName::NAME_AMBER,         SvFgName::NAME_AMMONITE,      SvFgName::NAME_COPROLITE,
        SvFgName::NAME_DINOSAUR_EGG,  SvFgName::NAME_FERN_FOSSIL,   SvFgName::NAME_DINOSAUR_TRACK,
        SvFgName::NAME_ARCHAEOPTERYX, SvFgName::NAME_PEKING_MAN,    SvFgName::NAME_SHARK_TOOTH,
        SvFgName::NAME_TRILOBITE,
    };
    u32 stand = (id == SvFgName::NAME_PTERA_SKULL || id == SvFgName::NAME_PTERA_RIGHT_WING ||
                 id == SvFgName::NAME_PTERA_LEFT_WING)
                    ? SvFgName::NAME_1X2_MUSEUM_ITEM_STAND_DINOSAURS
                    : SvFgName::NAME_2X2_MUSEUM_ITEM_STAND_DINOSAURS;
    for (u32 i = 0; i < sizeof(smallFossils) / sizeof(smallFossils[0]); i++) {
        if (smallFossils[i] == id) {
            stand = SvFgName::NAME_1X1_MUSEUM_ITEM_PODIUM_FOSSILS;
            break;
        }
    }
    return stand;
}

// the items of the layout: donated fossils stay, the others become their stands (MuseumFossilBuilder)
struct MuseumFossilItemsSetter
{
    MuseumFossilItemsSetter(Field* field, const FgDataLoader* layout) : m_pField(field), m_pLayout(layout) {}

    void operator()(const Vector2i& block)
    {
        if (m_pField == NULL) {
            return;
        }
        BgInfo* bg = m_pField->GetBg(block.x, block.y);
        if (!m_pLayout->IsLoaded()) {
            return;
        }
        const u32 num = m_pLayout->GetItems(m_Items, sizeof(m_Items), bg->GetName());
        for (u32 i = 0; i < num; i++) {
            const FgDataItem& entry = m_Items[i];
            u32 id = entry.id;
            if (SvFgName(static_cast<SvFgName::ID>(id)).GetKind() == item::KIND_FOSSIL) {
                if (!GetGardenPlus()->data.townData.museum.IsDonated(SvFgName(static_cast<SvFgName::ID>(id)))) {
                    id = GetFossilStand(id);
                }
            } else {
                id = SvFgName::NAME_2X2_MUSEUM_ITEM_STAND_DINOSAURS;
            }
            SvFgName item(static_cast<SvFgName::ID>(id));
            if (!item.IsEmpty() && Func30B658(&item)) {
                Func311860(&item, SvFgName(static_cast<SvFgName::ID>(id)), entry.flags >> 14);
            }
            bg->SetFg(item, entry.x, entry.z, 0);
        }
    }

    Field* m_pField;
    const FgDataLoader* m_pLayout;
    FgDataItem m_Items[FgDataLoader::ITEM_BUFFER_SIZE / sizeof(FgDataItem)];
};

// the items of the layout: only donated sculptures stay (MuseumPictureBuilder)
struct MuseumPictureItemsSetter
{
    MuseumPictureItemsSetter(Field* field, const FgDataLoader* layout) : m_pField(field), m_pLayout(layout) {}

    void operator()(const Vector2i& block)
    {
        if (m_pField == NULL) {
            return;
        }
        BgInfo* bg = m_pField->GetBg(block.x, block.y);
        if (!m_pLayout->IsLoaded()) {
            return;
        }
        const u32 num = m_pLayout->GetItems(m_Items, sizeof(m_Items), bg->GetName());
        for (u32 i = 0; i < num; i++) {
            const FgDataItem& entry = m_Items[i];
            u32 id = entry.id;
            if (SvFgName(static_cast<SvFgName::ID>(id)).GetKind() != item::KIND_SCULPTURE ||
                !GetGardenPlus()->data.townData.museum.IsDonated(SvFgName(static_cast<SvFgName::ID>(id)))) {
                id = SvFgName::NAME_EMPTY;
            }
            SvFgName item(static_cast<SvFgName::ID>(id));
            if (!item.IsEmpty() && Func30B658(&item)) {
                Func311860(&item, SvFgName(static_cast<SvFgName::ID>(id)), entry.flags >> 14);
            }
            bg->SetFg(item, entry.x, entry.z, 0);
        }
    }

    Field* m_pField;
    const FgDataLoader* m_pLayout;
    FgDataItem m_Items[FgDataLoader::ITEM_BUFFER_SIZE / sizeof(FgDataItem)];
};
} // namespace

// 0x002012C8 (name is ours)
Field* FieldFactory::Create(stage::Name name)
{
    if (stage::IsKind(stage::KIND_PLAYER_HOUSE, name)) {
        return NULL;
    }
    const stage::FieldLayout* layout = stage::GetInfo(name)->field;
    if (layout == NULL) {
        return NULL;
    }
    sead::Heap* heap = HeapInit::s_pStageHeap;
    if (heap == NULL) {
        return NULL;
    }

    if (stage::IsKind(stage::KIND_NPC_HOUSE, name)) {
        // the house of a villager: the items of the villager
        const s32 value = Func31336C(stage::GetCurrent());
        Func51E1D4(value);
        Field* field = CreateField(heap, layout);
        if (field == NULL) {
            return NULL;
        }
        SvFgBlock* layer0 = new (heap, 4) SvFgBlock();
        if (layer0 == NULL) {
            return NULL;
        }
        SvFgBlock* layer1 = new (heap, 4) SvFgBlock();
        if (layer1 == NULL) {
            delete layer0;
            return NULL;
        }
        layer0->Clear();
        layer1->Clear();
        SvNpc* npc = Func305AC0(GetGardenData()->GetNpcData()->npcs, value);
        if (npc != NULL) {
            if (Func11A3A8()) {
                FgDataItem items[7];
                Func6E5698(items, sizeof(items), Func2FE510(npc));
                Func11A818(npc, items, layer0, layer1);
            } else {
                FgDataItem items[12];
                Func318FDC(items, sizeof(items), npc->roomLayout);
                Func1199C4(npc, items, layer0, layer1);
            }
        }
        CopyBlocks(field, layer0, layer1);
        delete layer0;
        delete layer1;
        return field;
    }

    if (name == stage::NAME_CAMPING_CAR_A || name == stage::NAME_CAMPING_CAR_B) {
        // a camping car: the items of its object in the scene
        const u32 index = name == stage::NAME_CAMPING_CAR_A ? 0 : 1;
        Field* field = CreateField(heap, layout);
        if (field == NULL) {
            return NULL;
        }
        SvFgBlock* layer0 = new (heap, 4) SvFgBlock();
        if (layer0 == NULL) {
            return NULL;
        }
        SvFgBlock* layer1 = new (heap, 4) SvFgBlock();
        if (layer1 == NULL) {
            heap->free(layer0);
            return NULL;
        }
        layer0->Clear();
        layer1->Clear();
        ScStage* scene = stage::GetScene();
        if (scene != NULL) {
            const void* car = Func75E984(scene, index);
            for (s32 i = 0; i < 24; i++) {
                const FgPlacement placement = car != NULL ? Func71FF94(car, i) : Func2CFE88(i);
                if (placement.item.IsUsual()) {
                    (placement.layer == 0 ? layer0 : layer1)->SetFg(placement.item, placement.x, placement.z);
                }
            }
        }
        CopyBlocks(field, layer0, layer1);
        heap->free(layer0);
        heap->free(layer1);
        return field;
    }

    if (stage::IsKind(stage::KIND_DOWNTOWN, name)) {
        // the downtown blocks for the ocean side of the town
        u16 names[8];
        stage::FieldLayout downtown = {names, 4, 2, 0};
        Func6FB288(names, sizeof(names), GetGardenPlus()->data.townData.unk_0x5C.oceanSide);
        const FgDataLoader* fgLayout = Func6E565C(0);
        Field* field = CreateWithFg(heap, downtown.width, downtown.height, downtown.lightingType);
        ElmFieldBuilder builder(field, fgLayout, &downtown);
        FieldDirector director;
        director.Construct(&builder);
        return field;
    }

    Field* field;
    if (stage::IsKind(stage::KIND_TRAIN_STATION_ENVIRONMENT, name)) {
        // the train station in the variant of the town
        const FieldRect blocks(Vector2i::Zero(), Vector2i(layout->width - 1, layout->height - 1));
        field = CreateWithFg(heap, blocks.GetWidth(), blocks.GetHeight(), layout->lightingType);
        if (field == NULL) {
            return NULL;
        }
        BgInfo* bg = field->GetBg(0, 0);
        if (bg != NULL) {
            const SvBgName names[] = {
                BG_NAME_IDR_STATION_A,
                BG_NAME_IDR_STATION_B,
                BG_NAME_IDR_STATION_C,
                BG_NAME_IDR_STATION_D,
            };
            bg->SetName(names[Func6CBA48()]);
        }
    } else {
        switch (name) {
        case stage::NAME_OFFICE: {
            // the town hall in the variant of the town
            const FieldRect blocks(Vector2i::Zero(), Vector2i(layout->width - 1, layout->height - 1));
            field = CreateWithFg(heap, blocks.GetWidth(), blocks.GetHeight(), layout->lightingType);
            if (field == NULL) {
                return NULL;
            }
            BgInfo* bg = field->GetBg(0, 0);
            if (bg != NULL) {
                const SvBgName names[] = {
                    BG_NAME_IDR_OFFICE_A,
                    BG_NAME_IDR_OFFICE_B,
                    BG_NAME_IDR_OFFICE_C,
                    BG_NAME_IDR_OFFICE_D,
                };
                bg->SetName(names[Func6CBA2C()]);
            }
            break;
        }
        case stage::NAME_DEPARTMENT_STORE_3F: {
            const FgDataLoader* fgLayout = Func6E565C(1);
            field = CreateWithFg(heap, layout->width, layout->height, layout->lightingType);
            ShopGraceBuilder builder(field, fgLayout);
            FieldDirector director;
            director.Construct(&builder);
            return field;
        }
        case stage::NAME_VILLAGE_PROLOGUE: {
            const FgDataLoader* fgLayout = Func6E565C(0);
            field = new (heap, 4) Field();
            if (field == NULL) {
                return NULL;
            }
            if (!field->InitializeBase(heap, layout->width, layout->height)) {
                field->Finalize();
                return NULL;
            }
            VillagePrologueBuilder builder(field, fgLayout);
            FieldDirector director;
            director.Construct(&builder);
            return field;
        }
        case stage::NAME_CAMP_NPC: {
            const FgDataLoader* fgLayout = Func6E565C(1);
            field = CreateWithFg(heap, layout->width, layout->height, layout->lightingType);
            CampNpcBuilder builder(field, fgLayout);
            FieldDirector director;
            director.Construct(&builder);
            return field;
        }
        case stage::NAME_MUSEUM_FOSSIL_0:
        case stage::NAME_MUSEUM_FOSSIL_1:
        case stage::NAME_MUSEUM_FOSSIL_2: {
            const FgDataLoader* fgLayout = Func6E565C(1);
            field = CreateWithFg(heap, layout->width, layout->height, layout->lightingType);
            MuseumFossilBuilder builder(field, fgLayout, layout);
            FieldDirector director;
            director.Construct(&builder);
            return field;
        }
        case stage::NAME_MUSEUM_PICTURE_0:
        case stage::NAME_MUSEUM_PICTURE_1: {
            const FgDataLoader* fgLayout = Func6E565C(1);
            field = CreateWithFg(heap, layout->width, layout->height, layout->lightingType);
            MuseumPictureBuilder builder(field, fgLayout, layout);
            FieldDirector director;
            director.Construct(&builder);
            return field;
        }
        default:
            if (stage::IsKind(stage::KIND_MODEL_HOUSE, name)) {
                field = CreateWithFg(heap, layout->width, layout->height, layout->lightingType);
                ModelHouseFieldBuilder builder(field, NULL);
                FieldDirector director;
                director.Construct(&builder);
                return field;
            }
            if (stage::IsKind(stage::KIND_EXHIBITION, name)) {
                const FgDataLoader* fgLayout = Func6E565C(0);
                field = CreateWithFgAndGrass(heap, layout->width, layout->height, layout->lightingType);
                {
                    ExhibitionBuilder builder(field, fgLayout, layout);
                    FieldDirector director;
                    director.Construct(&builder);
                }
                // the worn grass of the showcase
                sead::Heap* current = sead::HeapMgr::s_pInstance->getCurrentHeap();
                mutil::BaseResourceMgr<g3d::ResourceLoader, 1> resources;
                resources.Initialize(current, mutil::BaseResourceMgr<g3d::ResourceLoader, 1>::DEFAULT_HEAP_SIZE);
                const mutil::ResourceKey key = resources.Load("Bg/Outdoor/Textures/GrassBaldness.bcres");
                Func6F9A68(field, resources.Get(key));
                return field;
            }
            // the other stages: the grounds and heights of the stage layout, indoors also the items
            field = CreateField(heap, layout);
            if (field == NULL) {
                return NULL;
            }
            if (stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent())) {
                ForEachBlock(field->GetBlockBounds(), BlockItemsCleaner(field));
                ForEachBlock(field->GetBlockBounds(), BlockItemsSetter(field, Func6E565C(1)));
            }
            return field;
        }
    }
    // (the train station and the town hall)
    ForEachBlock(field->GetBlockBounds(), BlockHeightsSetter(field));
    ForEachBlock(field->GetBlockBounds(), BlockItemsCleaner(field));
    ForEachBlock(field->GetBlockBounds(), BlockItemsSetter(field, Func6E565C(1)));
    return field;
}

// 0x00202CA0 (name is ours)
Field* FieldFactory::CreateWithFg(sead::Heap* heap, s32 width, s32 height, u32 maxLayer)
{
    Field* field = new (heap, 4) Field();
    if (field == NULL) {
        return NULL;
    }
    if (!field->InitializeWithFg(heap, width, height, maxLayer)) {
        field->Finalize();
        return NULL;
    }
    return field;
}

namespace {
// the blocks of an acre map (shared by the functions here)
inline const FieldRect& GetAcreMapRect()
{
    // 0x00AF9684 (guard 0x0098361C)
    static FieldRect s_AcreMapRect(Vector2i::Zero(), Vector2i(FieldFactory::ACRE_MAP_WIDTH - 1,
                                                              FieldFactory::ACRE_MAP_HEIGHT - 1));
    return s_AcreMapRect;
}

// the cell of a block in an acre map
inline u8& GetCell(u8* map, const Vector2i& block)
{
    return map[block.y * FieldFactory::ACRE_MAP_WIDTH + block.x];
}

// the kind 8..19 + 12 (8 and the others: 20)
inline u8 ConvertKind8To19(u8 kind)
{
    switch (kind) {
    case 9:
        return 21;
    case 10:
        return 22;
    case 11:
        return 23;
    case 12:
        return 24;
    case 13:
        return 25;
    case 14:
        return 26;
    case 15:
        return 27;
    case 16:
        return 28;
    case 17:
        return 29;
    case 18:
        return 30;
    case 19:
        return 31;
    default:
        return 20;
    }
}

// the converted kind of the kinds 34..44 (34 and the others: 52)
inline u8 ConvertKind34To44(u8 kind)
{
    switch (kind) {
    case 35:
        return 55;
    case 39:
        return 57;
    case 44:
        return 54;
    case 42:
        return 58;
    case 43:
        return 53;
    case 38:
        return 56;
    default:
        return 52;
    }
}

// the blocks of the cells of one of the kinds (at most 50) in a random order
inline void CollectCells(sead::FixedObjArray<Vector2i, 50>* cells, u8* map, const u8* kinds, u32 num)
{
    const FieldRect& blocks = GetAcreMapRect();
    for (FieldRect::iterator it = blocks.begin(); it != blocks.end(); ++it) {
        for (u32 i = 0; i < num; i++) {
            if (GetCell(map, *it) == kinds[i]) {
                const Vector2i cell = *it;
                cells->pushBack(cell);
                break;
            }
        }
    }
}
} // namespace

// 0x0020255C (name is ours)
Field* FieldFactory::CreateBase(sead::Heap* heap, s32 width, s32 height)
{
    Field* field = new (heap, 4) Field();
    if (field == NULL) {
        return NULL;
    }
    if (!field->InitializeBase(heap, width, height)) {
        field->Finalize();
        return NULL;
    }
    return field;
}

// 0x002025B0 (name is ours)
u16 FieldFactory::GetRandomAcre(u8 kind, sead::Random* random)
{
    u16 acres[7] = {};
    u32 num = 0;
    for (u32 acre = 0; acre <= ACRE_MAX; acre++) {
        if (kind == Func6060E8(static_cast<u16>(acre)) && num < 7) {
            acres[num++] = static_cast<u16>(acre);
        }
    }
    return acres[random->getU32(num)];
}

// 0x00202634 (name is ours)
void FieldFactory::ReplacePlaceholders(u8* map, const u16* kinds, const u8* source)
{
    const FieldRect& blocks = GetAcreMapRect();
    u32 next = 0;
    for (FieldRect::iterator it = blocks.begin(); it != blocks.end(); ++it) {
        u8 kind = source[(*it).y * ACRE_MAP_WIDTH + (*it).x];
        if (kind == ACRE_KIND_PLACEHOLDER) {
            kind = static_cast<u8>(kinds[next++]);
        }
        GetCell(map, *it) = kind;
    }
}

// 0x00202790 (name is ours)
void FieldFactory::SetNeighbors(u8* map)
{
    const FieldRect& blocks = GetAcreMapRect();
    for (FieldRect::iterator it = blocks.begin(); it != blocks.end(); ++it) {
        if (GetCell(map, *it) == 33) {
            map[(*it).y * ACRE_MAP_WIDTH + (*it).x + 1] = 61;
        }
        if (GetCell(map, *it) == 32) {
            map[(*it).y * ACRE_MAP_WIDTH + (*it).x - 1] = 71;
        }
    }
}

// 0x0020292C (name is ours)
void FieldFactory::ReplaceRandomCell(u8* map, u8 kind, u8 newKind, sead::Random* random)
{
    const FieldRect& blocks = GetAcreMapRect();
    u32 num = 0;
    for (FieldRect::iterator it = blocks.begin(); it != blocks.end(); ++it) {
        if (GetCell(map, *it) == kind) {
            num++;
        }
    }
    const u32 index = random->getU32(num);
    u32 i = 0;
    for (FieldRect::iterator it = blocks.begin(); it != blocks.end(); ++it) {
        if (GetCell(map, *it) == kind) {
            if (i == index) {
                GetCell(map, *it) = newKind;
                return;
            }
            i++;
        }
    }
}

// 0x00202B78 (name is ours)
Field* FieldFactory::CreateWithGrass(sead::Heap* heap, s32 width, s32 height)
{
    Field* field = new (heap, 4) Field();
    if (field == NULL) {
        return NULL;
    }
    if (!field->Initialize(heap, width, height)) {
        field->Finalize();
        return NULL;
    }
    return field;
}

// 0x00202BCC (name is ours)
void RandomAcreSetter::operator()(const Vector2i& block)
{
    if (m_pField == NULL) {
        return;
    }
    const u8 kind = m_pMap[block.y * FieldFactory::ACRE_MAP_WIDTH + block.x];
    const u16 acre = FieldFactory::GetRandomAcre(kind, m_pRandom);
    BgInfo* bg = m_pField->GetBg(block.x, block.y);
    if (bg == NULL) {
        return;
    }
    bg->SetName(static_cast<SvBgName>(acre));
}

// 0x0080E90C
template RandomAcreSetter ForEachBlock<RandomAcreSetter>(const FieldRect& rect, RandomAcreSetter function);

// 0x00202CFC (name is ours)
void FieldFactory::ReplaceRandomCell8To19(u8* map, sead::Random* random)
{
    const u8 kinds[] = {8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
    sead::FixedObjArray<Vector2i, 50> cells;
    CollectCells(&cells, map, kinds, sizeof(kinds));
    if (cells.size() > 0) {
        cells.shuffle(random);
        Vector2i* cell = cells.front();
        if (cell != NULL) {
            GetCell(map, *cell) = ConvertKind8To19(GetCell(map, *cell));
        }
    }
}

// 0x00202FB8 (name is ours)
void FieldFactory::ReplaceRandomCell43Or38(u8* map, sead::Random* random)
{
    const u8 kinds[] = {43, 38};
    sead::FixedObjArray<Vector2i, 50> cells;
    CollectCells(&cells, map, kinds, sizeof(kinds));
    if (cells.size() > 0) {
        cells.shuffle(random);
        Vector2i* cell = cells.front();
        if (cell != NULL) {
            GetCell(map, *cell) = ConvertKind34To44(GetCell(map, *cell));
        }
    }
}

// 0x00203234 (name is ours)
void FieldFactory::ReplaceRandomCell34Or35Or39(u8* map, sead::Random* random)
{
    const u8 kinds[] = {34, 35, 39};
    sead::FixedObjArray<Vector2i, 50> cells;
    CollectCells(&cells, map, kinds, sizeof(kinds));
    if (cells.size() > 0) {
        cells.shuffle(random);
        Vector2i* cell = cells.front();
        if (cell != NULL) {
            GetCell(map, *cell) = ConvertKind34To44(GetCell(map, *cell));
        }
    }
}

// 0x002034B0 (name is ours)
void FieldFactory::ReplaceRandomCell44Or42(u8* map, sead::Random* random)
{
    const u8 kinds[] = {44, 42};
    sead::FixedObjArray<Vector2i, 50> cells;
    CollectCells(&cells, map, kinds, sizeof(kinds));
    if (cells.size() > 0) {
        cells.shuffle(random);
        Vector2i* cell = cells.front();
        if (cell != NULL) {
            GetCell(map, *cell) = ConvertKind34To44(GetCell(map, *cell));
        }
    }
}

// 0x0020372C (name is ours)
Field* FieldFactory::CreateWithFgAndGrass(sead::Heap* heap, s32 width, s32 height, u32 maxLayer)
{
    Field* field = new (heap, 4) Field();
    if (field == NULL) {
        return NULL;
    }
    if (!field->InitializeWithFgAndGrass(heap, width, height, maxLayer)) {
        field->Finalize();
        return NULL;
    }
    return field;
}

// 0x00203788 (name is ours)
FieldFactory::~FieldFactory()
{
    // empty (in the original too)
}

namespace {
// 0x00526190 (name is ours)
Field* CreateField(sead::Heap* heap, const stage::FieldLayout* layout)
{
    Field* field = FieldFactory::CreateWithFg(heap, layout->width, layout->height, layout->lightingType);
    if (field == NULL) {
        return NULL;
    }
    ForEachBlock(field->GetBlockBounds(), BlockNamesSetter(field, layout->acres));
    ForEachBlock(field->GetBlockBounds(), BlockHeightsSetter(field));
    return field;
}

// 0x005256C0 slot 0x0C (name is ours)
void BasicBuilder::SetupHeights()
{
    ForEachBlock(m_pField->GetBlockBounds(), BlockHeightsSetter(m_pField));
}

// 0x00525698 slot 0x10 (name is ours)
void BasicBuilder::Setup10()
{
    // empty (in the original too)
}

// 0x005256A4 slot 0x14 (name is ours)
void BasicBuilder::ClearItems()
{
    ForEachBlock(m_pField->GetBlockBounds(), BlockItemsCleaner(m_pField));
}

// 0x00525634 slot 0x18 (name is ours)
void BasicBuilder::SetupItems()
{
    if (m_pLayout != NULL) {
        ForEachBlock(m_pField->GetBlockBounds(), BlockItemsSetter(m_pField, m_pLayout));
    }
}

// 0x00525694 slot 0x1C (name is ours)
void BasicBuilder::Setup1C()
{
    // empty (in the original too)
}

// 0x0052569C slot 0x20 (name is ours)
void BasicBuilder::Setup20()
{
    // empty (in the original too)
}

// 0x005256A0 slot 0x24 (name is ours)
void BasicBuilder::Setup24()
{
    // empty (in the original too)
}

// 0x00525630 slot 0x28 (name is ours)
void BasicBuilder::SetupGrass()
{
    // empty (in the original too)
}

// 0x005256DC slot 0x08 (name is ours)
void CampNpcBuilder::SetupBlocks()
{
    const SvBgName name = Func6CC54C() ? BG_NAME_IDR_TENT : BG_NAME_IDR_IGLOO;
    if (m_pField == NULL) {
        return;
    }
    BgInfo* bg = m_pField->GetBg(0, 0);
    if (bg == NULL) {
        return;
    }
    bg->SetName(name);
}

// 0x00525734 slot 0x00
// 0x00525724 (deleting dtor)
CampNpcBuilder::~CampNpcBuilder()
{
    // empty (in the original too)
}

// 0x00525738 slot 0x08 (name is ours)
void ElmFieldBuilder::SetupBlocks()
{
    ForEachBlock(m_pField->GetBlockBounds(), BlockNamesSetter(m_pField, m_pFieldLayout->acres));
}

// 0x0052582C slot 0x00
// 0x0052581C (deleting dtor)
ElmFieldBuilder::~ElmFieldBuilder()
{
    // empty (in the original too)
}

// 0x00525830 slot 0x08 (name is ours)
void ShopGraceBuilder::SetupBlocks()
{
    SvBgName name = BG_NAME_IDR_GRACE_NORMAL;
    const u8* shop = &GetGardenPlus()->data.townData.unk_0xEE2C.gracieUnlockStatus;
    if (shop != NULL && Func302940(shop)) {
        name = BG_NAME_IDR_GRACE_BARGAIN;
    }
    if (m_pField == NULL) {
        return;
    }
    BgInfo* bg = m_pField->GetBg(0, 0);
    if (bg == NULL) {
        return;
    }
    bg->SetName(name);
}

// 0x005258A4 slot 0x00
// 0x00525894 (deleting dtor)
ShopGraceBuilder::~ShopGraceBuilder()
{
    // empty (in the original too)
}

// 0x005258A8 slot 0x18 (name is ours)
void ExhibitionBuilder::SetupItems()
{
    if (m_pField != NULL && m_pLayout != NULL) {
        ForEachBlock(m_pField->GetBlockBounds(), ExhibitionItemsSetter(m_pField, m_pLayout));
    }
}

// 0x00525B60 slot 0x00
// 0x00525B50 (deleting dtor)
ExhibitionBuilder::~ExhibitionBuilder()
{
    // empty (in the original too)
}

// 0x00525B64 slot 0x18 (name is ours)
void MuseumFossilBuilder::SetupItems()
{
    if (m_pField != NULL && m_pLayout != NULL) {
        ForEachBlock(m_pField->GetBlockBounds(), MuseumFossilItemsSetter(m_pField, m_pLayout));
    }
}

// 0x00525EB0 slot 0x00
// 0x00525EA0 (deleting dtor)
MuseumFossilBuilder::~MuseumFossilBuilder()
{
    // empty (in the original too)
}

// 0x00525EB4 slot 0x18 (name is ours)
void MuseumPictureBuilder::SetupItems()
{
    if (m_pField != NULL && m_pLayout != NULL) {
        ForEachBlock(m_pField->GetBlockBounds(), MuseumPictureItemsSetter(m_pField, m_pLayout));
    }
}

// 0x0052618C slot 0x00
// 0x0052617C (deleting dtor)
MuseumPictureBuilder::~MuseumPictureBuilder()
{
    // empty (in the original too)
}

// 0x00526298 slot 0x08 (name is ours)
void ModelHouseFieldBuilder::SetupBlocks()
{
    if (m_pField == NULL) {
        return;
    }
    const u8 structure = stage::GetLastStructure();
    if (!Func6CB818(structure)) {
        return;
    }
    u8* house = static_cast<u8*>(Func6CC658(structure));
    if (house == NULL) {
        return;
    }
    void* rooms = house + 0x10;
    const u32 room = Func6FBB80(stage::GetCurrent());
    BgInfo* bg = m_pField->GetBg(0, 0);
    if (bg == NULL) {
        return;
    }
    bg->SetName(static_cast<SvBgName>(Func6FB9E8(rooms, room)));
}

// 0x00526318 slot 0x18 (name is ours)
void ModelHouseFieldBuilder::SetupItems()
{
    if (m_pField == NULL) {
        return;
    }
    const u8 structure = stage::GetLastStructure();
    if (!Func6CB818(structure)) {
        return;
    }
    u8* house = static_cast<u8*>(Func6CC658(structure));
    if (house == NULL) {
        return;
    }
    void* room = Func30521C(house + 0x10, Func6FBB80(stage::GetCurrent()));
    BgInfo* bg = m_pField->GetBg(0, 0);
    if (bg == NULL) {
        return;
    }
    // the 10 x 10 units of layer 0 from unit 3, 4 on, the 8 x 8 units of layer 1 from unit 4, 5 on
    for (s32 z = 0; z < 10; z++) {
        for (s32 x = 0; x < 10; x++) {
            bg->SetFg(*Func721010(room, x, z, 0), x + 3, z + 4, 0);
        }
    }
    for (s32 z = 0; z < 8; z++) {
        for (s32 x = 0; x < 8; x++) {
            bg->SetFg(*Func721010(room, x, z, 1), x + 4, z + 5, 1);
        }
    }
}

// 0x00526450 slot 0x00
// 0x00526440 (deleting dtor)
ModelHouseFieldBuilder::~ModelHouseFieldBuilder()
{
    // empty (in the original too)
}

// 0x00526454 slot 0x08 (name is ours)
void VillagePrologueBuilder::SetupBlocks()
{
    if (m_pField == NULL) {
        return;
    }
    const FieldRect& blocks = m_pField->GetBlockBounds();
    u8* map = &GetGardenPlus()->data.townData.unk_0x5C.oceanSide;
    for (FieldRect::iterator it = blocks.begin(); it != blocks.end(); ++it) {
        BgInfo* bg = m_pField->GetBg((*it).x, (*it).y);
        if (bg != NULL) {
            bg->SetName(static_cast<SvBgName>(*Func75EBB4(map, (*it).x, (*it).y)));
        }
    }
}

// 0x00526554 slot 0x18 (name is ours)
void VillagePrologueBuilder::SetupItems()
{
    if (m_pField == NULL) {
        return;
    }
    const FieldRect& blocks = m_pField->GetBlockBounds();
    SvTownDataUnk5C* map = &GetGardenData()->townData.unk_0x5C;
    for (FieldRect::iterator it = blocks.begin(); it != blocks.end(); ++it) {
        if (Func6141DC((*it).x, (*it).y)) {
            // (the blocks of the map lie one block inside of the field)
            SvFgBlock* items = Func2FAECC(map, (*it).x, (*it).y);
            BgInfo* bg = m_pField->GetBg((*it).x + 1, (*it).y + 1);
            if (bg != NULL) {
                bg->SetBlocks(items, NULL);
            }
        }
    }
}

// 0x005822A4 slot 0x28 (name is ours)
void VillagePrologueBuilder::SetupGrass()
{
    m_pField->SetGrass(Func614230(&GetGardenData()->townData.unk_0x5C));
}

// 0x00526690 slot 0x00
// 0x00526680 (deleting dtor)
VillagePrologueBuilder::~VillagePrologueBuilder()
{
    // empty (in the original too)
}
} // namespace
