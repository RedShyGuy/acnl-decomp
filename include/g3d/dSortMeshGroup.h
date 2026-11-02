#pragma once

#include "decomp.h"
#include <new>
#include "g3d/dFunc.h"
#include "nw/gfx/res/gfx_ResMesh.h"
#include "ssys/ma/dHeapAllocator.h"

namespace nw {
namespace gfx {
class MeshRenderer;
class Model;
} // namespace gfx
} // namespace nw

namespace sead {
class Heap;
} // namespace sead

namespace g3d {
// the meshes of models drawn by the functions of a Func node instead of the render queue: the opaque ones and then
// the translucent ones, each sorted by render priority (the class name is the name of its heap; other names are ours)
class SortMeshGroup
{
public:
    // a mesh of a model
    struct Entry
    {
        nw::gfx::Model* model;          // 0x0
        nw::gfx::res::ResMesh mesh;     // 0x4
    };
    // whether a comes before b
    typedef bool (*CompareFunc)(const Entry* a, const Entry* b);
    // an array of a fixed capacity
    template <typename T>
    struct Array
    {
        void Initialize(void* buffer, s32 capacity)
        {
            m_pBegin = static_cast<T*>(buffer);
            m_pEnd = static_cast<T*>(buffer);
            m_Capacity = capacity;
        }
        void Clear() { m_pEnd = m_pBegin; }
        // (no test of the capacity)
        void PushBack(const T& value)
        {
            new (m_pEnd) T(value);
            ++m_pEnd;
        }

        T* m_pBegin;      // 0x0
        T* m_pEnd;        // 0x4
        s32 m_Capacity;   // 0x8
    };

    SortMeshGroup(); // 0x004EE8E4
    // (the implicit destructor is used by the static array: 0x004F1440)

    // the visible meshes of the model, by translucency; the first one puts the node into scene 0; always true
    bool Add(nw::gfx::Model* model); // 0x004EE45C
    // no meshes and models; the next Add puts the node into scene 0 again
    void Clear(); // 0x004EE410
    // the normal matrices of the models for the view of the top screen, then the opaque meshes
    void DrawOpaque(nw::gfx::MeshRenderer* renderer); // 0x004EE58C
    // the drawing functions of the node (userData: the group)
    static void DrawOpaqueFunc(nw::gfx::MeshRenderer* renderer, void* userData); // 0x004EE57C
    static void DrawTranslucentFunc(nw::gfx::MeshRenderer* renderer, void* userData); // 0x004EE814
    // the default order: the lower render priority first
    static bool CompareRenderPriority(const Entry* a, const Entry* b); // 0x004EE8C4
    // sorts the meshes with m_pCompareFunc (an exchange sort)
    void Sort(Array<Entry>& entries)
    {
        if (entries.m_pBegin == entries.m_pEnd) {
            return;
        }
        for (Entry* i = entries.m_pBegin; i != entries.m_pEnd - 1; ++i) {
            for (Entry* j = i + 1; j != entries.m_pEnd; ++j) {
                if (!m_pCompareFunc(i, j)) {
                    Entry tmp = *i;
                    *i = *j;
                    *j = tmp;
                }
            }
        }
    }

    Func m_Func;                                 // 0x00
    Array<Entry> m_OpaqueEntries;                // 0x08
    Array<Entry> m_TranslucentEntries;           // 0x14
    Array<nw::gfx::Model*> m_Models;             // 0x20
    CompareFunc m_pCompareFunc;                  // 0x2C
    CompareFunc m_pCompareFunc2;                 // 0x30 (set like m_pCompareFunc; never read)
    ssys::ma::HeapAllocator m_HeapAllocator;     // 0x34 the heap of the node
    bool m_IsInScene;                            // 0x3C
};
ASSERT_SIZE(SortMeshGroup, 0x40);

// the groups
const s32 SORT_MESH_GROUP_COUNT = 2;
extern SortMeshGroup s_SortMeshGroups[SORT_MESH_GROUP_COUNT];
// the meshes of a group
struct SortMeshGroupCounts
{
    u32 opaque;       // 0x0
    u32 translucent;  // 0x4
};
// (in dSortMeshGroupCounts.cpp: GCC would fold the values into InitializeSortMeshGroups)
extern const SortMeshGroupCounts s_SortMeshGroupCounts[SORT_MESH_GROUP_COUNT];
SortMeshGroup* GetSortMeshGroups(); // 0x004EF444
// the arrays of the groups and their nodes in a heap of their own each (of 800 bytes from heap)
void InitializeSortMeshGroups(SortMeshGroup* groups, sead::Heap* heap); // 0x004EF31C
} // namespace g3d
