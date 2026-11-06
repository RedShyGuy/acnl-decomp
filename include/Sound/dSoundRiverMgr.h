#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundHandle.h"

namespace sead { class Heap; }
class SoundObjRiver;

// The sound of the river: one sound for all river objects near the listener, its volume is the loudest of them and its pan
// the mean weighted by their volume (the class name is from the RTTI of its disposer, everything else is ours).
class SoundRiverMgr
{
public:
    class SingletonDisposer_;

    enum
    {
        OBJ_NUM = 60,
        INVALID_INDEX = 61,
    };

    SoundRiverMgr();
    static SoundRiverMgr* createInstance(sead::Heap* heap); // 0x00247274
    static void destroyInstance(); // 0x001235B0

    // the index of the object or INVALID_INDEX
    u32 Register(SoundObjRiver* obj); // 0x002473B0
    void Update(); // 0x00247408
    void Unregister(u32 index); // 0x002474F0

    static SoundRiverMgr* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 m_SingletonDisposerBuffer[4]; // 0x00
    nw::snd::SoundHandle m_Handle;    // 0x10
    SoundObjRiver* m_pObjs[OBJ_NUM];  // 0x14
    u32 m_ObjNum;                     // 0x104
};
ASSERT_OFFSET(SoundRiverMgr, m_Handle, 0x10);
ASSERT_OFFSET(SoundRiverMgr, m_ObjNum, 0x104);
ASSERT_SIZE(SoundRiverMgr, 0x108);
