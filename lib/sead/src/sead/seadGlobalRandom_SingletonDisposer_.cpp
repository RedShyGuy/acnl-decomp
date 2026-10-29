#include "sead/seadGlobalRandom_SingletonDisposer_.h"

// 0x00540ECC slot 0x00 | slot vf_0x00 of sead::IDisposer
// 0x00540E88 (deleting dtor)
sead::GlobalRandom::SingletonDisposer_::~SingletonDisposer_()
{
    if (this != GlobalRandom::s_pSingletonDisposer) {
        return;
    }
    GlobalRandom::s_pSingletonDisposer = nullptr;
    GlobalRandom::s_pInstance->~GlobalRandom();
    GlobalRandom::s_pInstance = nullptr;
}
