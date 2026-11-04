#pragma once

#include "decomp.h"
#include "sead/seadPtrArrayImpl.h"
#include <new>

namespace sead {
// A free list over a block of equally big objects (the first word links them; names are ours)
class FreeList
{
public:
    FreeList() : mFree(nullptr), mWork(nullptr) {}

    void init(void* work, s32 elementSize, s32 num)
    {
        void** element = static_cast<void**>(work);
        s32 words = elementSize / sizeof(void*);
        mFree = work;
        for (s32 i = 0; i < num - 1; i++) {
            void** next = element + words;
            *element = next;
            element = next;
        }
        *element = nullptr;
        mWork = work;
    }
    void* get()
    {
        void* element = mFree;
        if (element != nullptr) {
            mFree = *static_cast<void**>(element);
        }
        return element;
    }
    void put(void* element)
    {
        *static_cast<void**>(element) = mFree;
        mFree = element;
    }

    void* mFree; // 0x0
    void* mWork; // 0x4
};
ASSERT_SIZE(FreeList, 8);

// An array of objects with pointers into a free list (class names from the reference symbols, the rest is ours)
template <typename T>
class ObjArray : public PtrArrayImpl
{
public:
    typedef s32 (*CompareCallback)(const T* element, const void* key);

    s32 size() const { return mPtrNum; }
    T* at(s32 index) const { return static_cast<T*>(mPtrs[index]); }

    // the first element that compares equal, -1 if there is none
    s32 search(const void* key, CompareCallback cmp) const
    {
        for (s32 i = 0; i < mPtrNum; i++) {
            if (cmp(static_cast<T*>(mPtrs[i]), key) == 0) {
                return i;
            }
        }
        return -1;
    }
    // a new element at the end (NULL if the array is full)
    T* emplaceBack()
    {
        if (mPtrNum >= mPtrNumMax) {
            return nullptr;
        }
        void* memory = mFreeList.get();
        T* element = memory != nullptr ? new (memory) T() : nullptr;
        if (mPtrNum < mPtrNumMax) {
            mPtrs[mPtrNum] = element;
            mPtrNum++;
        }
        return element;
    }
    void erase(s32 pos)
    {
        destroy(at(pos));
        PtrArrayImpl::erase(pos, 1);
    }
    void clear()
    {
        for (s32 i = 0; i < mPtrNum; i++) {
            destroy(at(i));
        }
        mPtrNum = 0;
    }

protected:
    void destroy(T* element)
    {
        element->~T();
        mFreeList.put(element);
    }

    FreeList mFreeList; // 0xC
};

template <typename T, s32 N>
class FixedObjArray : public ObjArray<T>
{
public:
    FixedObjArray()
    {
        this->mFreeList.init(mWork, sizeof(T), N);
        this->setBuffer(N, mPtrWork);
    }

    u8 mWork[N * sizeof(T)];
    void* mPtrWork[N];
};
} // namespace sead
