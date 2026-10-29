#include "sead/seadResourceMgr.h"
#include "sead/seadDirectResource.h"
#include "sead/seadDirectResourceFactory.h"
#include "sead/seadHeapMgr.h"
#include "sead/seadNew.h"
#include "sead/seadResourceMgr_SingletonDisposer_.h"

#include <new>

namespace sead {
// 0x009763E8
ResourceMgr* ResourceMgr::s_pInstance = nullptr;
// 0x009763EC
ResourceMgr::SingletonDisposer_* ResourceMgr::s_pSingletonDisposer = nullptr;

// 0x00132D30 (name is ours)
sead::ResourceMgr::ResourceMgr() : mFactoryList(), mDecompList(), mNullFactory(nullptr), mDefaultFactory(nullptr)
{
    if (HeapMgr::s_pInstance == nullptr) {
        return;
    }
    Heap* heap = HeapMgr::s_pInstance->findContainHeap(this);
    mNullFactory = new (heap, 4) DirectResourceFactory<DirectResource>();
    setDefaultFactory(mNullFactory);
}

// 0x0012C43C (name is ours)
sead::ResourceMgr* sead::ResourceMgr::createInstance(Heap* heap)
{
    if (s_pInstance == nullptr) {
        u8* buffer = new (heap, 4) u8[sizeof(ResourceMgr)];
        ResourceMgr* instance = nullptr;
        if (buffer == nullptr) {
            s_pSingletonDisposer = nullptr;
        } else {
            s_pSingletonDisposer = new (buffer) SingletonDisposer_(heap);
            instance = new (buffer) ResourceMgr();
        }
        s_pInstance = instance;
    }
    return s_pInstance;
}

// 0x0012C4A0 (name is ours)
void sead::ResourceMgr::deleteInstance()
{
    SingletonDisposer_* disposer = s_pSingletonDisposer;
    if (disposer == nullptr) {
        return;
    }
    s_pSingletonDisposer = nullptr;
    disposer->~SingletonDisposer_();
    if (s_pInstance != nullptr) {
        delete s_pInstance;
    }
    s_pInstance = nullptr;
}

// 0x00138AE0 (name is ours)
void sead::ResourceMgr::registerFactory(ResourceFactory* factory, const SafeStringBase<char>& name)
{
    factory->mName.copy(name);
    mFactoryList.pushBack(factory);
}

// 0x00540A8C | nintendogs:bytes [tier A]
void sead::ResourceMgr::unregisterFactory(ResourceFactory* factory)
{
    TListNode<ResourceFactory*>* node = factory;
    if (node->mList != nullptr) {
        mFactoryList.erase(node);
    }
}

// 0x00540A44 (name is ours)
sead::ResourceFactory* sead::ResourceMgr::setDefaultFactory(ResourceFactory* factory)
{
    if (factory == nullptr) {
        factory = mNullFactory;
    }
    ResourceFactory* previous = mDefaultFactory;
    mDefaultFactory = factory;
    registerFactory(mDefaultFactory, "");
    return previous;
}
} // namespace sead
