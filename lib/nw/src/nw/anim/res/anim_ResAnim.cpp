#include "nw/anim/res/anim_ResAnim.h"

#include "nw/anim/res/anim_ResTextureAnim.h"

#include <cstring>

namespace nw {
namespace anim {
namespace res {
namespace {
// a self-relative offset copied to another place, still pointing to the same data
inline void CopyOffset(s32* pDst, const s32* pSrc)
{
    const u8* p = *pSrc != 0 ? reinterpret_cast<const u8*>(pSrc) + *pSrc : nullptr;
    *pDst = p != nullptr ? p - reinterpret_cast<const u8*>(pDst) : 0;
}
} // namespace

// 0x004D5850 (name is ours)
nw::anim::res::ResAnim nw::anim::res::ResAnim::Clone(os::IAllocator* pAllocator, const ResAnim& source)
{
    ResAnimData* pData = static_cast<ResAnimData*>(pAllocator->Alloc(sizeof(ResAnimData), 4));
    pData->signature = source.ptr()->signature;
    pData->typeInfo = source.ptr()->typeInfo;
    pData->name = source.ptr()->name;
    pData->m_Unknown0xC = source.ptr()->m_Unknown0xC;
    pData->loopMode = source.ptr()->loopMode;
    pData->frameSize = source.ptr()->frameSize;
    pData->memberAnimCount = source.ptr()->memberAnimCount;
    pData->userDataCount = source.ptr()->userDataCount;
    pData->userDataDic = source.ptr()->userDataDic;

    // the dictionary (the header and count + 1 nodes)
    const s32 count = source.ptr()->memberAnimCount;
    const u32 dicSize = sizeof(ut::ResDicPatricia::Data) + count * sizeof(ut::ResDicPatricia::Node);
    ut::ResDicPatricia::Data* pDic = static_cast<ut::ResDicPatricia::Data*>(pAllocator->Alloc(dicSize, 4));
    std::memcpy(pDic, source.ptr()->memberAnimDic.ToPtr(), dicSize);
    const ut::ResDicPatricia::Data* pSourceDic = source.ptr()->memberAnimDic.To<ut::ResDicPatricia::Data>();
    for (int i = 0; i < count; i++) {
        ut::ResDicPatricia::Node* pNode = &pDic->nodes[i + 1];
        CopyOffset(&pNode->ofsName, &pSourceDic->nodes[i + 1].ofsName);
        CopyOffset(&pNode->ofsData, &pSourceDic->nodes[i + 1].ofsData);
        if (source.GetMemberAnim(i).GetPrimitiveType() != PRIMITIVE_TEXTURE) {
            continue;
        }

        // a texture pattern: its own texture references
        const ResTextureAnimData* pSourceMember = ResTextureAnim(source.GetMemberAnim(i).ptr()).ptr();
        const s32 textureCount = pSourceMember->textureCount;
        ut::Offset* pTextureOffsets = static_cast<ut::Offset*>(pAllocator->Alloc(textureCount * sizeof(ut::Offset), 4));
        gfx::res::ResReferenceTextureData* pTextures = static_cast<gfx::res::ResReferenceTextureData*>(
            pAllocator->Alloc(textureCount * sizeof(gfx::res::ResReferenceTextureData), 4));
        for (int j = 0; j < textureCount; j++) {
            const gfx::res::ResReferenceTextureData* pSourceTexture =
                pSourceMember->textures.To<ut::Offset>()[j].To<gfx::res::ResReferenceTextureData>();
            gfx::res::ResReferenceTextureData* pTexture = &pTextures[j];
            pTexture->typeInfo = pSourceTexture->typeInfo;
            pTexture->signature = pSourceTexture->signature;
            pTexture->m_Unknown0x8 = pSourceTexture->m_Unknown0x8;
            pTexture->name = pSourceTexture->name;
            pTexture->userDataCount = pSourceTexture->userDataCount;
            pTexture->userDataDic = pSourceTexture->userDataDic;
            pTexture->path = pSourceTexture->path;
            pTexture->target.m_Offset = 0;
            pTextureOffsets[j].m_Offset = reinterpret_cast<u8*>(pTexture) - reinterpret_cast<u8*>(&pTextureOffsets[j]);
        }
        ResTextureAnimData* pMember = static_cast<ResTextureAnimData*>(pAllocator->Alloc(sizeof(ResTextureAnimData), 4));
        if (pSourceMember->flags & ResMemberAnimData::FLAG_CONSTANT) {
            pMember->index.constantValue = pSourceMember->index.constantValue;
        } else {
            pMember->index.curve = pSourceMember->index.curve;
        }
        pMember->textureCount = textureCount;
        pMember->textures.Set(pTextureOffsets);
        pMember->flags = pSourceMember->flags;
        pMember->path = pSourceMember->path;
        pMember->primitiveType = pSourceMember->primitiveType;
        pNode->ofsData = pMember != nullptr ? reinterpret_cast<u8*>(pMember) - reinterpret_cast<u8*>(&pNode->ofsData) : 0;
    }
    pData->memberAnimDic.Set(pDic);
    return ResAnim(pData);
}

// 0x004D5E5C (name is ours)
void nw::anim::res::ResAnim::DestroyClone(os::IAllocator* pAllocator, const ResAnim& anim)
{
    ResAnimData* pData = const_cast<ResAnimData*>(anim.ptr());
    if (pData == nullptr) {
        return;
    }
    for (int i = 0; i < pData->memberAnimCount; i++) {
        if (anim.GetMemberAnim(i).GetPrimitiveType() != PRIMITIVE_TEXTURE) {
            continue;
        }
        ResTextureAnimData* pMember = const_cast<ResTextureAnimData*>(ResTextureAnim(anim.GetMemberAnim(i).ptr()).ptr());
        pAllocator->Free(pMember->textures.To<ut::Offset>()->ToPtr());
        pAllocator->Free(pMember->textures.ToPtr());
        pAllocator->Free(pMember);
    }
    pAllocator->Free(pData->memberAnimDic.ToPtr());
    pAllocator->Free(pData);
}

// 0x001385DC | nintendogs:bytes [tier A]
void nw::anim::res::ResAnim::Cleanup()
{
    for (int i = 0; i < GetMemberAnimCount(); ++i) {
        ResMemberAnim memberAnim = GetMemberAnim(i);
        if (memberAnim.GetPrimitiveType() != PRIMITIVE_TEXTURE) {
            continue;
        }
        ResTextureAnim textureAnim(memberAnim.ptr());
        for (int j = 0; j < textureAnim.GetTextureCount(); ++j) {
            textureAnim.GetTexture(j)->target.m_Offset = 0;
        }
    }
}

// 0x004D6124 | nintendogs:bytes [tier A]
u32 nw::anim::res::ResAnim::Setup(os::IAllocator* pAllocator, gfx::res::ResGraphicsFile graphicsFile)
{
    u32 result = gfx::RESULT_SUCCESS;
    for (int i = 0; i < GetMemberAnimCount(); ++i) {
        ResMemberAnim memberAnim = GetMemberAnim(i);
        result |= memberAnim.GetPrimitiveType() == PRIMITIVE_TEXTURE ? ResTextureAnim(memberAnim.ptr()).Setup(pAllocator, graphicsFile) : 0;
    }
    return result;
}

} // namespace res
} // namespace anim
} // namespace nw
