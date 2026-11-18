#include "Npc/dNpcCampingCarInfoLoader.h"
#include "Npc/npc_Extern.h"
#include "Other/dHeapInit.h"
#include "sead/seadExpHeap.h"
#include "sead/seadFixedSafeString.h"
#include "ssys/ma/dFile.h"
#include "ssys/ma/dLoadSplit.h"

// 0x003358DC (name is ours)
bool NpcCampingCarInfoLoader::Cancel()
{
    if (m_InfoState == STATE_NONE) {
        return true;
    }
    if (!RequestCancel()) {
        return false;
    }
    m_InfoState = STATE_NONE;
    return true;
}

// 0x00335914 (name is ours)
bool NpcCampingCarInfoLoader::Load(const char* model, sead::Heap* heap, u32 unused)
{
    (void)unused;
    if (model == NULL || m_InfoState >= STATE_LOADING) {
        return true;
    }
    sead::FixedSafeString<128> path;
    switch (m_InfoState) {
    case STATE_NONE:
        path.format("NpcCampingCarInfo/camp_%s.bin", model);
        if (ssys::ma::File::IsExist(path)) {
            m_InfoState = STATE_FOUND;
            break;
        }
        m_InfoState = STATE_FALLBACK;
        // fall through
    case STATE_FALLBACK:
        path.format("NpcCampingCarInfo/camp_%s.bin", "wol04");
        break;
    }
    if (heap == NULL) {
        heap = HeapInit::s_pStageHeap;
    }
    if (!Read(path, heap)) {
        return false;
    }
    m_InfoState = STATE_LOADING;
    return true;
}

// 0x00335A28 (name is ours)
bool NpcCampingCarInfoLoader::LoadForNpc(u16 npcId, sead::Heap* heap, u32 unused)
{
    const char* model;
    if (!Func1B6350(npcId) || (model = Func1B6330(npcId)) == NULL) {
        model = "wol04";
    }
    return Load(model, heap, unused);
}

// 0x00335AB0 slot 0x00
// 0x00335AA0 (deleting dtor)
NpcCampingCarInfoLoader::~NpcCampingCarInfoLoader()
{
    // empty (in the original too)
}
