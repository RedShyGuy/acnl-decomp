#include "nw/lyt/lyt_AnimTransformBasic.h"

#include "nw/lyt/internal/internal_Api.h"
#include "nw/lyt/lyt_Layout.h"
#include "nw/lyt/lyt_Material.h"
#include "nw/lyt/lyt_Pane.h"
#include "nw/lyt/lyt_ResourceAccessor.h"

namespace nw {
namespace lyt {
namespace {
// two key frames this close are the same frame
inline bool IsSameFrame(f32 frame1, f32 frame2)
{
    const f32 diff = frame1 - frame2;
    return -0.001f < diff && diff < 0.001f;
}

// 0x004B71C0 (name is ours)
// the value of a Hermite curve at the frame
DECOMP_NOINLINE f32 GetHermiteCurveValue(f32 frame, const res::HermiteKey* keys, u32 keySize)
{
    if (keySize == 1 || frame <= keys[0].frame) {
        return keys[0].value;
    }
    if (keys[keySize - 1].frame <= frame) {
        return keys[keySize - 1].value;
    }

    u32 ikeyL = 0;
    u32 ikeyR = keySize - 1;
    while (ikeyL != ikeyR - 1 && ikeyL != ikeyR) {
        const u32 ikeyCenter = (ikeyL + ikeyR) / 2;
        if (frame <= keys[ikeyCenter].frame) {
            ikeyR = ikeyCenter;
        } else {
            ikeyL = ikeyCenter;
        }
    }

    const res::HermiteKey& key0 = keys[ikeyL];
    const res::HermiteKey& key1 = keys[ikeyR];
    if (IsSameFrame(frame, key1.frame)) {
        // a step: two keys of the same frame
        if (ikeyR < keySize - 1 && key1.frame == keys[ikeyR + 1].frame) {
            return keys[ikeyR + 1].value;
        }
        return key1.value;
    }

    const f32 t1 = frame - key0.frame;
    const f32 t2 = 1.0f / (key1.frame - key0.frame);
    const f32 t1t1t2 = t1 * t1 * t2;
    const f32 t1t1t2t2 = t1t1t2 * t2;
    const f32 t1t1t1t2t2 = t1 * t1t1t2t2;
    const f32 t1t1t1t2t2t2 = t1t1t1t2t2 * t2;
    return key0.value * (2.0f * t1t1t1t2t2t2 - 3.0f * t1t1t2t2 + 1.0f) + key1.value * (-2.0f * t1t1t1t2t2t2 + 3.0f * t1t1t2t2) +
           key0.slope * (t1t1t1t2t2 - 2.0f * t1t1t2 + t1) + key1.slope * (t1t1t1t2t2 - t1t1t2);
}

// (inline) the value of a step curve at the frame
inline u16 GetStepCurveValue(f32 frame, const res::StepKey* keys, u32 keySize)
{
    if (keySize == 1 || frame <= keys[0].frame) {
        return keys[0].value;
    }
    if (keys[keySize - 1].frame <= frame) {
        return keys[keySize - 1].value;
    }

    int ikeyL = 0;
    int ikeyR = static_cast<int>(keySize) - 1;
    while (ikeyL != ikeyR - 1 && ikeyL != ikeyR) {
        const int ikeyCenter = (ikeyL + ikeyR) / 2;
        if (frame < keys[ikeyCenter].frame) {
            ikeyR = ikeyCenter;
        } else {
            ikeyL = ikeyCenter;
        }
    }

    if (IsSameFrame(frame, keys[ikeyR].frame)) {
        return keys[ikeyR].value;
    }
    return keys[ikeyL].value;
}

// (inline) the keys of a target
template <typename T>
inline const T* GetKeys(const res::AnimationTarget& animTarget)
{
    return reinterpret_cast<const T*>(reinterpret_cast<uptr>(&animTarget) + animTarget.keysOffset);
}
} // namespace

// 0x004B6AC0 | nintendogs:bytes [tier A]
nw::lyt::AnimTransformBasic::AnimTransformBasic() : m_pTextures(nullptr), m_pAnimLinks(nullptr), m_AnimLinkNum(0)
{
}

// 0x004B6B30 slot 0x00 | nintendogs:bytes-fuzzy
// 0x004B6AF8 (deleting dtor)
nw::lyt::AnimTransformBasic::~AnimTransformBasic()
{
    if (m_pAnimLinks != nullptr) {
        Layout::FreeMemory(m_pAnimLinks);
    }
    if (m_pTextures != nullptr) {
        Layout::FreeMemory(m_pTextures);
    }
}

// 0x004B606C slot 0x10
void nw::lyt::AnimTransformBasic::SetResource(const res::AnimationBlock* pRes, ResourceAccessor* pResAccessor)
{
    SetResource(pRes, pResAccessor, pRes->animContNum);
}

// 0x004B607C slot 0x14
void nw::lyt::AnimTransformBasic::SetResource(const res::AnimationBlock* pRes, ResourceAccessor* pResAccessor, u16 animNum)
{
    m_pRes = pRes;
    m_pTextures = nullptr;

    if (pRes->fileNum > 0) {
        m_pTextures = Layout::NewArray<TextureInfo>(pRes->fileNum);
        if (m_pTextures != nullptr) {
            const u32* fileNameOffsets = reinterpret_cast<const u32*>(pRes + 1);
            for (int i = 0; i < pRes->fileNum; ++i) {
                const char* fileName = reinterpret_cast<const char*>(reinterpret_cast<uptr>(fileNameOffsets) + fileNameOffsets[i]);
                m_pTextures[i] = pResAccessor->GetTexture(fileName);
            }
        }
    }

    m_pAnimLinks = Layout::NewArray<AnimationLink>(animNum);
    if (m_pAnimLinks != nullptr) {
        m_AnimLinkNum = animNum;
    }
}

// 0x004B61E4 slot 0x18 | nintendogs:bytes
void nw::lyt::AnimTransformBasic::Bind(Pane* pPane, bool bRecursive, bool bDisable)
{
    const res::AnimationBlock* pAnimBlock = m_pRes;
    const u32* animContOffsets = reinterpret_cast<const u32*>(reinterpret_cast<uptr>(pAnimBlock) + pAnimBlock->animContOffsetsOffset);
    AnimationLink* pCrAnimLink = nullptr;

    for (u16 i = 0; i < pAnimBlock->animContNum; ++i) {
        const res::AnimationContent& animCont = *reinterpret_cast<const res::AnimationContent*>(reinterpret_cast<uptr>(pAnimBlock) + animContOffsets[i]);
        if (animCont.type == res::ANIMCONTENTTYPE_PANE) {
            Pane* pFindPane = pPane->FindPaneByName(animCont.name, bRecursive);
            if (pFindPane != nullptr) {
                pCrAnimLink = FindUnbindLink(pCrAnimLink);
                if (pCrAnimLink == nullptr) {
                    break;
                }
                pCrAnimLink->Set(this, i, bDisable);
                pFindPane->AddAnimationLink(pCrAnimLink);
                ++pCrAnimLink;
            }
        } else {
            Material* pFindMat = pPane->FindMaterialByName(animCont.name, bRecursive);
            if (pFindMat != nullptr) {
                pCrAnimLink = FindUnbindLink(pCrAnimLink);
                if (pCrAnimLink == nullptr) {
                    break;
                }
                pCrAnimLink->Set(this, i, bDisable);
                pFindMat->AddAnimationLink(pCrAnimLink);
                ++pCrAnimLink;
            }
        }
    }
}

// 0x004B6374 slot 0x1C
void nw::lyt::AnimTransformBasic::Bind(Material* pMaterial, bool bDisable)
{
    const res::AnimationBlock* pAnimBlock = m_pRes;
    const u32* animContOffsets = reinterpret_cast<const u32*>(reinterpret_cast<uptr>(pAnimBlock) + pAnimBlock->animContOffsetsOffset);
    AnimationLink* pCrAnimLink = nullptr;

    for (u16 i = 0; i < pAnimBlock->animContNum; ++i) {
        const res::AnimationContent& animCont = *reinterpret_cast<const res::AnimationContent*>(reinterpret_cast<uptr>(pAnimBlock) + animContOffsets[i]);
        if (animCont.type == res::ANIMCONTENTTYPE_MATERIAL && internal::EqualsMaterialName(pMaterial->GetName(), animCont.name)) {
            pCrAnimLink = FindUnbindLink(pCrAnimLink);
            if (pCrAnimLink == nullptr) {
                break;
            }
            pCrAnimLink->Set(this, i, bDisable);
            pMaterial->AddAnimationLink(pCrAnimLink);
            ++pCrAnimLink;
        }
    }
}

// 0x004B6460 slot 0x08 | nintendogs:bytes
void nw::lyt::AnimTransformBasic::Animate(u32 idx, Pane* pPane)
{
    const u32* animContOffsets = reinterpret_cast<const u32*>(reinterpret_cast<uptr>(m_pRes) + m_pRes->animContOffsetsOffset);
    const res::AnimationContent& animCont = *reinterpret_cast<const res::AnimationContent*>(reinterpret_cast<uptr>(m_pRes) + animContOffsets[idx]);
    const u32* animInfoOffsets = reinterpret_cast<const u32*>(&animCont + 1);

    for (int i = 0; i < animCont.num; ++i) {
        const res::AnimationInfo& animInfo = *reinterpret_cast<const res::AnimationInfo*>(reinterpret_cast<uptr>(&animCont) + animInfoOffsets[i]);
        const u32* animTargetOffsets = reinterpret_cast<const u32*>(&animInfo + 1);

        switch (animInfo.kind) {
        case res::ANIMKIND_PANE_SRT:
            for (int j = 0; j < animInfo.num; ++j) {
                const res::AnimationTarget& animTarget = *reinterpret_cast<const res::AnimationTarget*>(reinterpret_cast<uptr>(&animInfo) + animTargetOffsets[j]);
                const f32 value = GetHermiteCurveValue(m_Frame, GetKeys<res::HermiteKey>(animTarget), animTarget.keyNum);
                (&pPane->m_Translate.x)[animTarget.target] = value;
                if (animTarget.target < Pane::ANIMTARGET_PANE_SIZE_X) {
                    pPane->m_Flag &= ~Pane::FLAG_MTX_CONDITION_MASK;
                }
            }
            break;
        case res::ANIMKIND_VISIBILITY:
            for (int j = 0; j < animInfo.num; ++j) {
                const res::AnimationTarget& animTarget = *reinterpret_cast<const res::AnimationTarget*>(reinterpret_cast<uptr>(&animInfo) + animTargetOffsets[j]);
                const u16 value = GetStepCurveValue(m_Frame, GetKeys<res::StepKey>(animTarget), animTarget.keyNum);
                pPane->m_Flag = (pPane->m_Flag & ~Pane::FLAG_VISIBLE) | (value != 0 ? Pane::FLAG_VISIBLE : 0);
            }
            break;
        case res::ANIMKIND_VERTEX_COLOR:
            for (int j = 0; j < animInfo.num; ++j) {
                const res::AnimationTarget& animTarget = *reinterpret_cast<const res::AnimationTarget*>(reinterpret_cast<uptr>(&animInfo) + animTargetOffsets[j]);
                f32 value = GetHermiteCurveValue(m_Frame, GetKeys<res::HermiteKey>(animTarget), animTarget.keyNum) + 0.5f;
                if (value > 255.0f) {
                    value = 255.0f;
                } else if (value < 0.0f) {
                    value = 0.0f;
                }
                pPane->SetColorElement(animTarget.target, static_cast<u8>(value));
            }
            break;
        }
    }
}

// 0x004B673C slot 0x0C | nintendogs:bytes
void nw::lyt::AnimTransformBasic::Animate(u32 idx, Material* pMaterial)
{
    if (m_pRes == nullptr) {
        return;
    }

    const u32* animContOffsets = reinterpret_cast<const u32*>(reinterpret_cast<uptr>(m_pRes) + m_pRes->animContOffsetsOffset);
    const res::AnimationContent& animCont = *reinterpret_cast<const res::AnimationContent*>(reinterpret_cast<uptr>(m_pRes) + animContOffsets[idx]);
    const u32* animInfoOffsets = reinterpret_cast<const u32*>(&animCont + 1);

    for (int i = 0; i < animCont.num; ++i) {
        const res::AnimationInfo& animInfo = *reinterpret_cast<const res::AnimationInfo*>(reinterpret_cast<uptr>(&animCont) + animInfoOffsets[i]);
        const u32* animTargetOffsets = reinterpret_cast<const u32*>(&animInfo + 1);

        switch (animInfo.kind) {
        case res::ANIMKIND_MATERIAL_COLOR:
            for (int j = 0; j < animInfo.num; ++j) {
                const res::AnimationTarget& animTarget = *reinterpret_cast<const res::AnimationTarget*>(reinterpret_cast<uptr>(&animInfo) + animTargetOffsets[j]);
                f32 value = GetHermiteCurveValue(m_Frame, GetKeys<res::HermiteKey>(animTarget), animTarget.keyNum) + 0.5f;
                if (value < 0.0f) {
                    value = 0.0f;
                } else if (value > 255.0f) {
                    value = 255.0f;
                }
                pMaterial->SetColorElement(animTarget.target, static_cast<u8>(value));
            }
            break;
        case res::ANIMKIND_TEXTURE_SRT:
            for (int j = 0; j < animInfo.num; ++j) {
                const res::AnimationTarget& animTarget = *reinterpret_cast<const res::AnimationTarget*>(reinterpret_cast<uptr>(&animInfo) + animTargetOffsets[j]);
                if (animTarget.id < pMaterial->m_MemCap.texSRT) {
                    const f32 value = GetHermiteCurveValue(m_Frame, GetKeys<res::HermiteKey>(animTarget), animTarget.keyNum);
                    (&pMaterial->GetTexSRTAry()[animTarget.id].translate.x)[animTarget.target] = value;
                    pMaterial->SetCacheDirty();
                }
            }
            break;
        case res::ANIMKIND_TEXTURE_PATTERN:
            if (m_pTextures != nullptr) {
                for (int j = 0; j < animInfo.num; ++j) {
                    const res::AnimationTarget& animTarget = *reinterpret_cast<const res::AnimationTarget*>(reinterpret_cast<uptr>(&animInfo) + animTargetOffsets[j]);
                    if (animTarget.id < pMaterial->GetTexMapNum()) {
                        const u16 fileIdx = GetStepCurveValue(m_Frame, GetKeys<res::StepKey>(animTarget), animTarget.keyNum);
                        if (m_pTextures[fileIdx].physicalAddress != 0) {
                            static_cast<TexMap*>(pMaterial->m_pMem)[animTarget.id].Set(m_pTextures[fileIdx]);
                            pMaterial->SetCacheDirty();
                        }
                    }
                }
            }
            break;
        }
    }
}

} // namespace lyt
} // namespace nw
