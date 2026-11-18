#pragma once

#include "decomp.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadHeapMgr.h"
#include "sead/seadStringFormatter.h"
#include "ssys/ma/dHeapAllocator.h"
#include "ssys/st/dList.h"
#include "ssys/st/dListNode.h"
#include "Sv/dSvChecksum.h"
#include <cstring>

namespace mutil {
// The key of a resource of a BaseResourceMgr: the CRC-32C of its path (the class has no RTTI; all names are ours).
class ResourceKey
{
public:
    ResourceKey(); // 0x005B2504
    // (the flag is not copied)
    ResourceKey(const ResourceKey& other); // 0x005B24F0
    // the key of a hash
    void Set(u32 hash); // 0x005B24D0
    void Clear(); // 0x005B24E0
    ResourceKey& operator=(const ResourceKey& other); // 0x005B2514
    // (a copy in the shared area of the inline functions)
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool operator==(const ResourceKey& other) const // 0x00756AF0
    {
        return m_Hash == other.m_Hash;
    }

    // whether the key belongs to a file (a copy in the shared area)
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsValid() const { return m_Hash != 0; } // 0x00756AE0

    u32 m_Hash;  // 0x0
    bool m_Flag; // 0x4 (always false in the known code)
};
ASSERT_SIZE(ResourceKey, 0x8);

// A fixed number of resources (T = a loader with LoadAsync(path, heap, alignment) and Unload) in their own heap, found by the
// CRC of their path. The class name and the instances are from the RTTI of ResourceNode; the members and functions are
// ours. Instantiations found in the binary (ResourceNode typeinfo / vtable):
//   <FgDataLoader, 18u>  0x008D2D04 / 0x0090918C
//   <g3d::ResourceLoader, 13u>  0x008D2D10 / 0x0090919C
//   <g3d::ResourceLoader, 15u>  0x008D2D1C / 0x009091AC
//   <g3d::ResourceLoader, 1u>  0x008D2D28 / 0x009091BC
//   <g3d::ResourceLoader, 2u>  0x008D2D34 / 0x009091CC
//   <g3d::ResourceLoader, 3u>  0x008D2D40 / 0x009091DC
//   <g3d::ResourceLoader, 4u>  0x008D2D4C / 0x009091EC
//   <g3d::ResourceLoader, 64u>  0x008D2D58 / 0x009091FC
//   <mutil::LoaderWrapper, 110u>  0x008D2D64 / 0x0090920C
//   <mutil::LoaderWrapper, 18u>  0x008D2D70 / 0x0090921C
//   <mutil::LoaderWrapper, 20u>  0x008D2D7C / 0x0090922C
//   <mutil::LoaderWrapper, 31u>  0x008D2D88 / 0x0090923C
//   <mutil::LoaderWrapper, 3u>  0x008D2D94 / 0x0090924C
//   <mutil::LoaderWrapper, 62u>  0x008D2DA0 / 0x0090925C
// The manager itself is inline everywhere (e.g. in FieldFactory::Create for the exhibition grass).
template <typename T, u32 N>
class BaseResourceMgr
{
public:
    // one resource with its key, in one of the lists of the manager
    class ResourceNode : public ::ssys::st::ListNode
    {
    public:
        ResourceNode();
        virtual ~ResourceNode();

        ResourceKey m_Key; // 0x0C
        T m_Resource;      // 0x14
    };

    // the size of the heap of the manager
    static const u32 DEFAULT_HEAP_SIZE = 0x19000;
    // the alignment of the loaded files
    static const u32 LOAD_ALIGNMENT = 0x80;

    // (not user-provided: the house objects of hobj value-initialize the manager, i.e. clear it first; its owners call
    // Finalize themselves)
    BaseResourceMgr() = default;
    ~BaseResourceMgr() = default;

    // all is freed first, then the heap of the manager in parent (nothing without parent); some instances have
    // copies out of line (explicit specializations in dResourceLoaderMgr.h)
    DECOMP_ALWAYS_INLINE void Initialize(sead::Heap* parent, u32 size) { InitializeImpl(parent, size); }
    DECOMP_ALWAYS_INLINE void InitializeImpl(sead::Heap* parent, u32 size)
    {
        Finalize();
        if (parent != NULL) {
            m_Allocator.CreateExpHeap(size, parent, sead::SafeStringBase<char>("ResourceMgr"),
                                      sead::Heap::HEAP_DIRECTION_FORWARD, false);
        }
    }

    // all resources are unloaded, all nodes are free again and the heap is destroyed
    DECOMP_ALWAYS_INLINE void Finalize()
    {
        for (ResourceNode* node = static_cast<ResourceNode*>(m_UsedList.GetFirst()); node != NULL;
             node = static_cast<ResourceNode*>(node->GetNext())) {
            node->m_Key.Clear();
            node->m_Resource.Unload();
        }
        for (ResourceNode* node = static_cast<ResourceNode*>(m_LoadingList.GetFirst()); node != NULL;
             node = static_cast<ResourceNode*>(node->GetNext())) {
            node->m_Key.Clear();
            node->m_Resource.Unload();
        }
        m_UsedList.Clear();
        m_FreeList.Clear();
        m_LoadingList.Clear();
        for (u32 i = 0; i < N; i++) {
            m_FreeList.PushBack(&m_Nodes[i]);
        }
        m_Allocator.DestroyHeap();
    }

    // starts or goes on loading the file name (T::LoadAsync): the key of the file when it is loaded, an empty key while it
    // is loading (a free node starts it and goes into the loading list; a finished one moves into the used list). The key
    // is the CRC of the name followed by a null character, "%s%c".
    DECOMP_ALWAYS_INLINE ResourceKey Load(const char* name) { return LoadImpl(name); }
    DECOMP_ALWAYS_INLINE ResourceKey LoadImpl(const char* name)
    {
        sead::FixedSafeString<80> path;
        const s8 terminator = 0;
        {
            sead::StringFormatter formatter(&path);
            formatter.append("%s%c").append(name).appendValue(terminator).flush();
        }
        sead::Heap* heap = m_Allocator.m_pHeap;
        const char* str = path.getCString();
        ResourceKey key;
        key.Set(SvCalcCrc32C(str, std::strlen(str), 0xFFFFFFFF, 0xFFFFFFFF));
        for (ResourceNode* node = static_cast<ResourceNode*>(m_UsedList.GetFirst()); node != NULL;
             node = static_cast<ResourceNode*>(node->GetNext())) {
            if (key == node->m_Key) {
                return key;
            }
        }
        for (ResourceNode* node = static_cast<ResourceNode*>(m_LoadingList.GetFirst()); node != NULL;
             node = static_cast<ResourceNode*>(node->GetNext())) {
            if (key == node->m_Key) {
                if (!node->m_Resource.LoadAsync(sead::SafeStringBase<char>(name), heap, LOAD_ALIGNMENT)) {
                    return ResourceKey();
                }
                m_LoadingList.Erase(node);
                m_UsedList.PushBack(node);
                return node->m_Key;
            }
        }
        // (the free list is not checked)
        ResourceNode* node = static_cast<ResourceNode*>(m_FreeList.GetFirst());
        node->m_Resource.LoadAsync(sead::SafeStringBase<char>(name), heap, LOAD_ALIGNMENT);
        node->m_Key = key;
        m_FreeList.Erase(node);
        m_LoadingList.PushBack(node);
        return ResourceKey();
    }

    // the resource of a key (NULL if no node has it)
    DECOMP_ALWAYS_INLINE T* Get(ResourceKey key)
    {
        for (ResourceNode* node = static_cast<ResourceNode*>(m_UsedList.GetFirst()); node != NULL;
             node = static_cast<ResourceNode*>(node->GetNext())) {
            if (key == node->m_Key) {
                return &node->m_Resource;
            }
        }
        return NULL;
    }

    ::ssys::ma::HeapAllocator m_Allocator; // 0x0
    ResourceNode m_Nodes[N];               // 0x8
    ::ssys::st::List m_UsedList;           // the loaded resources
    ::ssys::st::List m_FreeList;           // the free nodes
    ::ssys::st::List m_LoadingList;          // the resources that are still loading
};
} // namespace mutil
