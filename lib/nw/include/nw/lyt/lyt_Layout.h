#pragma once

#include <new>

#include "decomp.h"
#include "nw/font/font_TagProcessorBase.h"
#include "nw/lyt/lyt_Types.h"
#include "nw/os/os_IAllocator.h"
#include "nw/ut/ut_LinkList.h"
#include "nw/ut/ut_Rect.h"

namespace nw {
namespace lyt {
class AnimResource;
class AnimTransform;
class DrawInfo;
class GroupContainer;
class Pane;
class ResourceAccessor;

// RTTI N2nw3lyt6LayoutE @ 0x008D0888
// vtable 0x00902C44 (vptr 0x00902C4C), offset_to_top 0, 17 entries
//
// A layout built from a BCLYT file: the pane tree, the groups and the animations. The memory of
// all layout objects comes from the allocators (AllocMemory / AllocDeviceMemory). The names of the
// functions beside the reference ones and of the members are ours.
class Layout
{
public:
    typedef ut::LinkList<AnimTransform, 4> AnimTransformList;

    Layout(); // 0x001322C8 | nintendogs:bytes [tier A]
    virtual ~Layout(); // 0x004BAA68 slot 0x00 | nintendogs:bytes
    // 0x004BAA54 (deleting dtor)
    virtual bool Build(const void* lytResBuf, ResourceAccessor* pResAcsr); // 0x004BA794 slot 0x08 | nintendogs:bytes
    // a new animation in the list
    virtual AnimTransform* CreateAnimTransform(); // 0x004BA678 slot 0x0C | nintendogs:bytes
    virtual AnimTransform* CreateAnimTransform(const void* animResBuf, ResourceAccessor* pResAcsr); // 0x004BA5F0 slot 0x10 | nintendogs:bytes
    virtual AnimTransform* CreateAnimTransform(const AnimResource& animRes, ResourceAccessor* pResAcsr); // 0x004BA62C slot 0x14 | libgarden
    virtual void DeleteAnimTransform(AnimTransform* pAnimTrans); // 0x004BA6D0 slot 0x18 (name is ours)
    virtual void BindAnimation(AnimTransform* pAnimTrans); // 0x004BA200 slot 0x1C | nintendogs:bytes
    virtual void UnbindAnimation(AnimTransform* pAnimTrans); // 0x004BA224 slot 0x20 | nintendogs:bytes
    virtual void UnbindAllAnimation(); // 0x00137B18 slot 0x24
    // a new animation bound to the groups of the tag of the resource (all panes without a tag) and
    // to the panes that share animations; false if a group or pane is missing
    virtual bool BindAnimationAuto(const AnimResource& animRes, ResourceAccessor* pResAcsr); // 0x004BA29C slot 0x28 (name is ours)
    virtual void SetAnimationEnable(AnimTransform* pAnimTrans, bool bEnable); // 0x004BA5D0 slot 0x2C (name is ours)
    virtual void Animate(u32 option); // 0x004BAA38 slot 0x30 (name is ours)
    virtual void CalculateMtx(const DrawInfo& drawInfo); // 0x004BA1C0 slot 0x34
    // the panes with GL
    virtual void Draw(DrawInfo& drawInfo); // 0x004BA730 slot 0x38 (name is ours)
    // the tag processor of all text boxes
    virtual void SetTagProcessor(font::TagProcessorBase<wchar_t>* pTagProcessor); // 0x004B7118 slot 0x3C (name is ours)
    virtual Pane* BuildPaneObj(s32 kind, const void* dataPtr, const ResBlockSet& resBlockSet); // 0x004BA034 slot 0x40 | nintendogs:callseq

    // the rectangle of the layout around the origin (a copy out of line for the game)
    ut::Rect GetLayoutRect() const; // 0x0073D320 (name is ours)

    // the allocators
    static void SetAllocator(os::IAllocator* pAllocator); // 0x004BA1DC (name is ours)
    static void SetDeviceMemoryAllocator(os::IAllocator* pAllocator); // 0x004BA720 (name is ours)
    static void* AllocMemory(u32 size, u8 alignment = 4); // 0x004BA010 | libgarden [tier A]
    static void FreeMemory(void* ptr); // 0x0013BB1C | libgarden [tier A]
    static void* AllocDeviceMemory(u32 size, u8 alignment); // 0x004BA278 | nintendogs:bytes [tier A]
    static void FreeDeviceMemory(void* ptr); // 0x004BA25C (name is ours)

    // (inline) an array of num default constructed objects (NULL: no memory)
    template <typename T>
    static T* NewArray(u32 num)
    {
        T* pArray = static_cast<T*>(AllocMemory(sizeof(T) * num));
        if (pArray == nullptr) {
            return nullptr;
        }
        for (u32 i = 0; i < num; ++i) {
            T* p = &pArray[i];
            if (p != nullptr) {
                new (p) T();
            }
        }
        return pArray;
    }
    // (inline) a new object in the layout memory (NULL: no memory); the arguments are copies (the
    // binary passes copies of the ResBlockSet to the constructors)
    template <typename T>
    static T* NewObj()
    {
        void* pMem = AllocMemory(sizeof(T));
        if (pMem == nullptr) {
            return nullptr;
        }
        return new (pMem) T();
    }
    template <typename T, typename P1>
    static T* NewObj(P1 param1)
    {
        void* pMem = AllocMemory(sizeof(T));
        if (pMem == nullptr) {
            return nullptr;
        }
        return new (pMem) T(param1);
    }
    template <typename T, typename P1, typename P2>
    static T* NewObj(P1 param1, P2 param2)
    {
        void* pMem = AllocMemory(sizeof(T));
        if (pMem == nullptr) {
            return nullptr;
        }
        return new (pMem) T(param1, param2);
    }
    // (inline) the object is destructed and its memory freed
    template <typename T>
    static void DeleteObj(T* pObj)
    {
        if (pObj != nullptr) {
            pObj->~T();
            FreeMemory(pObj);
        }
    }

    // (inline)
    Pane* GetRootPane() const { return m_pRootPane; }
    GroupContainer* GetGroupContainer() const { return m_pGroupContainer; }

    static os::IAllocator* s_pAllocator;            // the layout memory
    static os::IAllocator* s_pDeviceMemoryAllocator;

    AnimTransformList m_AnimTransList;      // 0x04
    Pane* m_pRootPane;                      // 0x10
    GroupContainer* m_pGroupContainer;      // 0x14
    Size m_LayoutSize;                      // 0x18
};
ASSERT_SIZE(Layout, 0x20);

// the allocators of all layouts (name is ours)
void Initialize(os::IAllocator* pAllocator, os::IAllocator* pDeviceAllocator); // 0x004B4AFC (name is ours)
} // namespace lyt
} // namespace nw
