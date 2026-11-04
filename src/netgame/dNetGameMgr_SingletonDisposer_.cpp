#include "netgame/dNetGameMgr_SingletonDisposer_.h"

#include "sead/seadNew.h"
#include <new>

// 0x0095464C
netgame::NetGameMgr::SingletonDisposer_* netgame::NetGameMgr::s_pSingletonDisposer;

// 0x00618294 (name after sead)
netgame::NetGameMgr* netgame::NetGameMgr::createInstance(sead::Heap* heap)
{
    if (s_pInstance == nullptr) {
        u8* buffer = new (heap, 4) u8[sizeof(NetGameMgr)];
        NetGameMgr* instance = nullptr;
        if (buffer == nullptr) {
            s_pSingletonDisposer = nullptr;
        } else {
            s_pSingletonDisposer = new (buffer) SingletonDisposer_(heap);
            instance = new (buffer) NetGameMgr();
        }
        s_pInstance = instance;
    }
    return s_pInstance;
}

// 0x00618BF4 slot 0x00 (name after the RTTI)
// 0x00618BA4 (deleting dtor)
netgame::NetGameMgr::SingletonDisposer_::~SingletonDisposer_()
{
    if (this == s_pSingletonDisposer) {
        s_pSingletonDisposer = nullptr;
        s_pInstance->~NetGameMgr();
        s_pInstance = nullptr;
    }
}
