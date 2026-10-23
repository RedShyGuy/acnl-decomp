#pragma once

#include <cstring>

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector3.h"
#include "nw/gfx/res/gfx_ResLookupTable.h"
#include "nw/gfx/res/gfx_ResSceneObject.h"
#include "nw/os/os_IAllocator.h"

namespace nw {
namespace gfx {
namespace res {
// Helpers for the resources that the dynamic builders make without a file (all names are ours).

// the header of such a resource: the signature reads "CTFN"
const u32 DYNAMIC_RESOURCE_SIGNATURE = 0x4E465443;
const u32 DYNAMIC_RESOURCE_REVISION = 0x07010000;
// the longest name that is copied
const u32 DYNAMIC_RESOURCE_NAME_MAX = 256;

// (inline) the header, without user data, with a copy of the name (NULL: none)
inline void SetupDynamicResSceneObject(ResSceneObjectData* pResource, u32 typeInfo, os::IAllocator* pAllocator, const char* name,
                                       u32 signature = DYNAMIC_RESOURCE_SIGNATURE, u32 revision = DYNAMIC_RESOURCE_REVISION)
{
    pResource->m_Unknown0x8 = revision;
    pResource->signature = signature;
    pResource->typeInfo = typeInfo;
    pResource->userDataCount = 0;
    pResource->userDataDic.m_Offset = 0;
    if (name != nullptr) {
        u32 length = std::strlen(name);
        if (length >= DYNAMIC_RESOURCE_NAME_MAX) {
            length = DYNAMIC_RESOURCE_NAME_MAX;
        }
        char* pName = static_cast<char*>(pAllocator->Alloc(length + 1, 4));
        std::memcpy(pName, name, length);
        pName[length] = 0;
        pResource->name.Set(pName);
    } else {
        pResource->name.m_Offset = 0;
    }
}

// (inline) a transform node at the origin, visible, without animation groups
inline void SetupDynamicResTransformNode(ResTransformNodeData* pResource)
{
    pResource->childCount = 0;
    pResource->children.m_Offset = 0;
    pResource->animGroupCount = 0;
    pResource->animGroupDic.m_Offset = 0;
    pResource->transform.scale = nn::math::VEC3(1.0f, 1.0f, 1.0f);
    pResource->transform.rotate = nn::math::VEC3(0.0f, 0.0f, 0.0f);
    pResource->transform.translate = nn::math::VEC3(0.0f, 0.0f, 0.0f);
    pResource->worldMatrix = nn::math::MTX34::Identity();
    pResource->flags |= ResSceneObjectData::FLAG_IS_BRANCH_VISIBLE;
}

// (inline) a lookup table that refers to a table without a name (NULL: no memory)
inline ResReferenceLookupTableData* CreateDynamicReferenceLut(os::IAllocator* pAllocator)
{
    ResReferenceLookupTableData* pLut = static_cast<ResReferenceLookupTableData*>(pAllocator->Alloc(sizeof(ResReferenceLookupTableData), 4));
    std::memset(pLut, 0, sizeof(ResReferenceLookupTableData));
    pLut->target.m_Offset = 0;
    pLut->typeInfo = ResLookupTable::TYPE_REFERENCE;
    pLut->path.m_Offset = 0;
    return pLut;
}

// (inline) the name of a dynamic resource freed with the resource
inline void DestroyDynamicResSceneObject(os::IAllocator* pAllocator, ResSceneObjectData* pResource)
{
    if (pResource->name.ToPtr() != nullptr) {
        pAllocator->Free(pResource->name.ToPtr());
    }
    pAllocator->Free(pResource);
}
} // namespace res
} // namespace gfx
} // namespace nw
