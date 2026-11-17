#include "Object/dObjectResource.h"

#include "sead/seadHeapMgr.h"
#include "sead/seadSafeStringBase.h"

// 0x00270C48 (name is ours)
ObjectResource::ObjectResource() : m_pResourceLoader(NULL), m_BufferOption(0)
{
}

// 0x00270CA8 slot 0x00
// 0x00270C70 (deleting dtor)
ObjectResource::~ObjectResource()
{
    if (m_Allocator.m_pHeap != NULL) {
        m_Allocator.DestroyHeap();
    }
}

// 0x00270C40 slot 0x20 (name is ours)
g3d::Model* ObjectResource::GetModel()
{
    return NULL;
}

// 0x00270BB8 slot 0x24 (name is ours)
void ObjectResource::CreateHeap()
{
    if (m_Allocator.m_pHeap != NULL) {
        return;
    }
    m_Allocator.CreateExpHeap(0, sead::HeapMgr::s_pInstance->getCurrentHeap(), "ObjectResource",
                              sead::Heap::HEAP_DIRECTION_FORWARD, false);
}

// 0x00270C28 slot 0x28 (name is ours)
void ObjectResource::DestroyHeap()
{
    if (m_Allocator.m_pHeap != NULL) {
        m_Allocator.DestroyHeap();
    }
}
