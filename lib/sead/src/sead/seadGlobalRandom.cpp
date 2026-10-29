#include "sead/seadGlobalRandom.h"
#include "sead/seadGlobalRandom_SingletonDisposer_.h"
#include "sead/seadNew.h"

#include <new>

namespace sead {
// 0x009763E0
GlobalRandom* GlobalRandom::s_pInstance = nullptr;
// 0x009763E4
GlobalRandom::SingletonDisposer_* GlobalRandom::s_pSingletonDisposer = nullptr;

// 0x00132E64 (name is ours)
sead::GlobalRandom* sead::GlobalRandom::createInstance(Heap* heap)
{
    if (s_pInstance == nullptr) {
        u8* buffer = new (heap, 4) u8[sizeof(GlobalRandom)];
        s_pSingletonDisposer = new (buffer + offsetof(GlobalRandom, mSingletonDisposerBuffer)) SingletonDisposer_(heap);
        s_pInstance = new (buffer) GlobalRandom();
    }
    return s_pInstance;
}

// 0x0012C504 (name is ours)
void sead::GlobalRandom::deleteInstance()
{
    SingletonDisposer_* disposer = s_pSingletonDisposer;
    if (disposer == nullptr) {
        return;
    }
    s_pSingletonDisposer = nullptr;
    disposer->~SingletonDisposer_();
    delete s_pInstance;
    s_pInstance = nullptr;
}
} // namespace sead
