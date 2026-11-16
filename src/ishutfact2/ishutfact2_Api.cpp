#include "ishutfact2/ishutfact2_Api.h"
#include "event/event_Extern.h"
#include "field/field_Extern.h"
#include "isfactory/isfactory_Api.h"
#include "ishutfact2/dBasicBuilder.h"
#include "ishutfact2/dBorrowingBuilder.h"
#include "ishutfact2/dFossilTemplateBuilder.h"
#include "Other/dFieldBuilder.h"
#include "Other/dTourData.h"
#include "sead/seadObjArray.h"
#include "sead/seadRandom.h"
#include "stage/stage_Api.h"
#include "stage/stage_Extern.h"

namespace {
// the villagers of the huts: shuffled, one for each hut
template <typename T>
inline void TakeEntries(u32* entries, T* candidates, sead::Random* random)
{
    candidates->shuffle(random);
    for (u32 i = 0; i < ishutfact2::HUT_NUM; i++) {
        entries[i] = *candidates->front();
        candidates->erase(0);
    }
}
} // namespace

namespace ishutfact2 {
// 0x002B8938 (name is ours)
void BuildFields(TourData* tourData)
{
    sead::Random random;
    random.init(tourData->m_Seed);
    FieldDirector director;
    switch (GetTourType(tourData->m_Tour)) {
    case 9: {
        // fossils in the even huts
        u8 keys[12];
        isfactory::GetHutFossilKeys(keys, tourData, &random);
        for (u32 i = FIELD_NAME_ISLAND_HUT_0; i < FIELD_NAME_ISLAND_HUT_0 + FOSSIL_HUT_NUM; i++) {
            const FieldName hut = static_cast<FieldName>(i);
            Field* field = stage::GetField(hut);
            if (field == NULL) {
                continue;
            }
            if (i & 1) {
                BasicBuilder builder(field, &random, tourData);
                director.Construct(&builder);
            } else {
                FossilTemplateBuilder builder(field, &random, tourData, hut, keys);
                director.Construct(&builder);
            }
        }
        break;
    }
    case 11: {
        // a villager for each hut (at level 2 a key 0..7 of the layout)
        u32 entries[BorrowingBuilder::ENTRY_NUM];
        if (Func76F7E4(&tourData->m_Seed) == 2) {
            sead::FixedObjArray<u32, 8> keys;
            for (u32 i = 0; i < 8; i++) {
                keys.pushBack(i);
            }
            TakeEntries(entries, &keys, &random);
        } else {
            sead::FixedObjArray<u32, 399> villagers;
            for (const u8* const* entry = event::g_UnknownA84AEC.entries; entry != event::g_UnknownA84AEC.entries + 399;
                 entry++) {
                if (((*entry)[0x1C] & 0xF) == 0) {
                    villagers.pushBack(*reinterpret_cast<const u16*>(*entry));
                }
            }
            TakeEntries(entries, &villagers, &random);
        }
        for (u32 i = 0; i < HUT_NUM; i++) {
            const FieldName hut = static_cast<FieldName>(FIELD_NAME_ISLAND_HUT_0 + i);
            Field* field = stage::GetField(hut);
            if (field == NULL) {
                continue;
            }
            BorrowingBuilder builder(field, &random, tourData, entries, hut);
            director.Construct(&builder);
        }
        Func6C5A20(&TourData::s_pInstance->m_Seed, random.getU32());
        break;
    }
    default:
        for (u32 i = 0; i < HUT_NUM; i++) {
            Field* field = stage::GetField(static_cast<FieldName>(FIELD_NAME_ISLAND_HUT_0 + i));
            if (field == NULL) {
                continue;
            }
            BasicBuilder builder(field, &random, tourData);
            director.Construct(&builder);
        }
        break;
    }
}
} // namespace ishutfact2
