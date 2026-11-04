#pragma once

#include "decomp.h"
#include "sead/seadListImpl.h"
#include "sead/seadNew.h"
#include <cstddef>
#include <new>

namespace sead {
// A ListImpl of objects of type T with a buffer of its own: an element holds the object and behind it
// its ListNode, the free elements are linked through their first word. In the binary it is only
// inline (net::boss::ProcessNsData); the class, function and member names are ours.
template <typename T>
class PooledList : public ListImpl
{
public:
    struct Element
    {
        T mObj;
        ListNode mNode;
    };

    PooledList() : mFreeList(nullptr), mBuffer(nullptr), mMaxNum(0) {}
    ~PooledList() { freeBuffer(); }

    // a buffer for num objects (nothing if it cannot be allocated)
    void allocBuffer(s32 num, Heap* heap, s32 alignment)
    {
        if (num <= 0) {
            return;
        }
        Element* buffer = static_cast<Element*>(::operator new(num * sizeof(Element), heap, alignment, std::nothrow));
        if (buffer == nullptr) {
            return;
        }
        mFreeList = buffer;
        for (s32 i = 0; i < num - 1; i++) {
            *reinterpret_cast<void**>(&buffer[i]) = &buffer[i + 1];
        }
        *reinterpret_cast<void**>(&buffer[num - 1]) = nullptr;
        mBuffer = buffer;
        mMaxNum = num;
    }
    void freeBuffer()
    {
        if (mBuffer == nullptr) {
            return;
        }
        clear();
        ::operator delete[](mBuffer);
        mMaxNum = 0;
        mFreeList = nullptr;
        mBuffer = nullptr;
    }

    // a new object at the end (NULL when the buffer is full)
    T* birthBack()
    {
        if (mCount >= mMaxNum) {
            return nullptr;
        }
        void* memory = mFreeList;
        if (memory != nullptr) {
            mFreeList = *static_cast<void**>(memory);
        }
        T* obj = new (memory) T();
        mStartEnd.insertFront_(objToListNode(obj));
        mCount++;
        return obj;
    }
    // takes obj out and gives its element back
    void erase(T* obj)
    {
        objToListNode(obj)->erase_();
        mCount--;
        obj->~T();
        *reinterpret_cast<void**>(obj) = mFreeList;
        mFreeList = obj;
    }
    void clear()
    {
        ListNode* node = mStartEnd.mNext;
        while (node != &mStartEnd) {
            ListNode* next = node->mNext;
            erase(listNodeToObj(node));
            node = next;
        }
    }

    T* front() const { return listNodeToObj(mStartEnd.mNext); }
    T* endObj() const { return listNodeToObj(const_cast<ListNode*>(&mStartEnd)); }
    T* next(T* obj) const { return listNodeToObj(objToListNode(obj)->mNext); }
    void sort(s32 (*cmp)(const void* a, const void* b)) { ListImpl::sort(offsetof(Element, mNode), cmp); }

private:
    static ListNode* objToListNode(T* obj)
    {
        return reinterpret_cast<ListNode*>(reinterpret_cast<u8*>(obj) + offsetof(Element, mNode));
    }
    static T* listNodeToObj(ListNode* node)
    {
        return reinterpret_cast<T*>(reinterpret_cast<u8*>(node) - offsetof(Element, mNode));
    }

    void* mFreeList; // 0x0C
    Element* mBuffer; // 0x10
    s32 mMaxNum;      // 0x14
};
} // namespace sead
