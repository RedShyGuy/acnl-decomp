#pragma once

#include "decomp.h"
#include "field/dFieldBuilderUtil.h"
#include "field/dFieldTypes.h"
#include "ishutfact2/dBasicBuilder.h"

namespace ishutfact2 {
// The template houses of the Fossil Finder's Tour (the even huts; the huts lead back to Island_FossilDig): fossils from
// the layout "FgDataTour/J0.bin"; the fossils that end up in the hut are remembered in TourData::m_HutFossils, the
// players dig up the same ones on the island and copy the arrangement into the empty hut next to it.
// RTTI N10ishutfact221FossilTemplateBuilderE @ 0x008CD8A0
// vtable 0x008FAB74 (vptr 0x008FAB7C), offset_to_top 0, 11 entries
class FossilTemplateBuilder : public ::ishutfact2::BasicBuilder
{
public:
    // the even huts (for each one a key of the layout)
    static const u32 HUT_NUM = 4;

    FossilTemplateBuilder(Field* field, sead::Random* random, TourData* tourData, FieldName hut, const u8* keys)
        : BasicBuilder(field, random, tourData), m_Hut(hut)
    {
        for (u32 i = 0; i < HUT_NUM; i++) {
            m_Keys[i] = keys[i];
        }
    }
    ~FossilTemplateBuilder() override;

    void Setup1C() override;
    void SetupGrass() override;

    FieldName m_Hut;     // 0x10
    u8 m_Keys[HUT_NUM];  // 0x11 the key of the layout for each even hut
};
ASSERT_SIZE(FossilTemplateBuilder, 0x18);

// the fossils of the layout for each block: placeholders become a random small fossil of the set or a fossil of two or
// three parts in one of four directions (the instance of ForEachBlock is called, the operator is inline in it)
struct FossilItemsSetter
{
    FossilItemsSetter(Field* field, const FgDataLoader* data, sead::Random* random, TourData* tourData, u8 key)
        : m_pField(field), m_pData(data), m_pRandom(random), m_pTourData(tourData), m_Key(key)
    {
    }

    void operator()(const Vector2i& block);

    Field* m_pField;
    const FgDataLoader* m_pData;
    FgDataItem m_Items[FgDataLoader::ITEM_BUFFER_SIZE / sizeof(FgDataItem)];
    sead::Random* m_pRandom;
    TourData* m_pTourData;
    u8 m_Key;
};
} // namespace ishutfact2

extern template ishutfact2::FossilItemsSetter ForEachBlock<ishutfact2::FossilItemsSetter>(
    const FieldRect& rect, ishutfact2::FossilItemsSetter function);
