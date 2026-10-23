#include "nw/gfx/gfx_ParticleUtil.h"

#include "nn/nstd/nstd_String.h"

#include <cstring>

namespace nw {
namespace gfx {
namespace {
// the pieces of a copy are aligned to 4 bytes
const u32 PIECE_ALIGNMENT = 4;

// (inline) the size of a piece rounded up to the alignment of the pieces
inline u32 AlignPiece(u32 size)
{
    return (size + PIECE_ALIGNMENT - 1) & ~(PIECE_ALIGNMENT - 1);
}

// the sizes of the pieces of an updater (names are ours)
struct UpdaterSizes
{
    u32 updater;
    s32 name;
    s32 volume;
    s32 secondPart;
    u32 animation;
    s32 keys;
    s32 values;
    u32 interpolator;
};

// (inline) the sizes of the updater, of the name and the parts of a user updater and of the animation
DECOMP_ALWAYS_INLINE void GetUpdaterSizes(UpdaterSizes* pSizes, const res::ResParticleUpdaterData* pData)
{
    pSizes->name = 0;
    pSizes->volume = 0;
    pSizes->secondPart = 0;
    pSizes->updater = res::GetParticleUpdaterSize(pData->typeInfo);
    if (pData->typeInfo == res::ResParticleUserUpdaterData::TYPE) {
        const res::ResParticleUserUpdaterData* pUser =
            res::DynamicCastData<res::ResParticleUserUpdaterData>(pData, res::ResParticleUserUpdaterData::TYPE);
        pSizes->name = std::strlen(pUser->name.To<char>()) + 1;
        const u32* pVolume = pUser->volume.To<u32>();
        const u32* pSecondPart = pUser->secondPart.To<u32>();
        if (pVolume != nullptr) {
            pSizes->volume = res::ResParticleVolumeData::GetSize(*pVolume);
        }
        if (pSecondPart != nullptr) {
            pSizes->secondPart = res::ResParticleUserUpdaterData::GetSecondPartSize(*pSecondPart);
        }
    }

    pSizes->animation = 0;
    pSizes->keys = 0;
    pSizes->values = 0;
    pSizes->interpolator = 0;
    const res::ResParticleAnimationData* pAnimation = pData->animation.To<res::ResParticleAnimationData>();
    if (pAnimation != nullptr) {
        pSizes->keys = pAnimation->keyCount;
        pSizes->values = pAnimation->valueCount * sizeof(u32);
        const u32* pInterpolator = pAnimation->interpolator.To<u32>();
        if (pInterpolator != nullptr) {
            pSizes->interpolator = res::ResParticleAnimationData::GetInterpolatorSize(*pInterpolator);
        }
        // (the keys are counted as words here, but copied as bytes)
        pSizes->animation = sizeof(res::ResParticleAnimationData) + pAnimation->keyCount * sizeof(u32) + pAnimation->valueCount * sizeof(u32);
    }
}

// (inline) a piece added to the size with the alignment of the pieces
inline void AddPiece(os::MemorySizeCalculator* pSize, u32 size)
{
    const u32 alignment = pSize->GetAlignment();
    pSize->SetAlignment(PIECE_ALIGNMENT);
    pSize->Add(size);
    pSize->SetAlignment(alignment);
}
} // namespace

// 0x0048EA38 | nintendogs:bytes [tier A]
res::ResParticleUpdaterData* nw::gfx::ParticleUtil::DuplicateResParticleUpdater(const res::ResParticleUpdater* pUpdater, os::IAllocator* pAllocator)
{
    UpdaterSizes sizes;
    GetUpdaterSizes(&sizes, pUpdater->ptr());

    const u32 size = AlignPiece(sizes.updater) + AlignPiece(sizes.animation) + AlignPiece(sizes.keys) + AlignPiece(sizes.values) +
                     AlignPiece(sizes.interpolator) + AlignPiece(sizes.name) + AlignPiece(sizes.volume) + AlignPiece(sizes.secondPart);
    u8* pBuffer = static_cast<u8*>(pAllocator->Alloc(size, PIECE_ALIGNMENT));
    if (pBuffer == nullptr) {
        return nullptr;
    }

    res::ResParticleUpdaterData* pCopy = reinterpret_cast<res::ResParticleUpdaterData*>(pBuffer);
    nnnstdMemCpy(pCopy, pUpdater->ptr(), sizes.updater);
    u8* pPiece = pBuffer + AlignPiece(sizes.updater);

    if (sizes.animation != 0) {
        res::ResParticleAnimationData* pAnimation = reinterpret_cast<res::ResParticleAnimationData*>(pPiece);
        const res::ResParticleAnimationData* pSource = pUpdater->ptr()->animation.To<res::ResParticleAnimationData>();
        nnnstdMemCpy(pAnimation, pSource, sizes.animation);
        pCopy->animation.Set(pAnimation);
        pPiece += AlignPiece(sizes.animation);
        if (sizes.keys > 0) {
            nnnstdMemCpy(pPiece, pSource->keys.ToPtr(), sizes.keys);
            pAnimation->keys.Set(pPiece);
            pPiece += AlignPiece(sizes.keys);
        }
        if (sizes.values > 0) {
            nnnstdMemCpy(pPiece, pSource->values.ToPtr(), sizes.values);
            pAnimation->values.Set(pPiece);
            pPiece += AlignPiece(sizes.values);
        }
        if (sizes.interpolator != 0) {
            nnnstdMemCpy(pPiece, pSource->interpolator.ToPtr(), sizes.interpolator);
            pAnimation->interpolator.Set(pPiece);
            pPiece += AlignPiece(sizes.interpolator);
        }
    }

    const res::ResParticleUserUpdaterData* pSource = static_cast<const res::ResParticleUserUpdaterData*>(pUpdater->ptr());
    if (pSource->typeInfo == res::ResParticleUserUpdaterData::TYPE) {
        res::ResParticleUserUpdaterData* pUser = static_cast<res::ResParticleUserUpdaterData*>(pCopy);
        if (sizes.name > 0) {
            nnnstdMemCpy(pPiece, pSource->name.ToPtr(), sizes.name);
            pUser->name.Set(pPiece);
            pPiece += AlignPiece(sizes.name);
        }
        if (sizes.volume > 0) {
            nnnstdMemCpy(pPiece, pSource->volume.ToPtr(), sizes.volume);
            pUser->volume.Set(pPiece);
            pPiece += AlignPiece(sizes.volume);
        }
        if (sizes.secondPart > 0) {
            nnnstdMemCpy(pPiece, pSource->secondPart.ToPtr(), sizes.secondPart);
            pUser->secondPart.Set(pPiece);
        }
    }
    return pCopy;
}

// 0x0048EFC0 | nintendogs:bytes [tier A]
res::ResParticleInitializerData* nw::gfx::ParticleUtil::DuplicateResParticleInitializer(const res::ResParticleInitializer* pInitializer,
                                                                                       os::IAllocator* pAllocator)
{
    const u32 size = res::GetParticleInitializerSize(pInitializer->ptr()->typeInfo);
    res::ResParticleInitializerData* pCopy = static_cast<res::ResParticleInitializerData*>(pAllocator->Alloc(AlignPiece(size), PIECE_ALIGNMENT));
    if (pCopy == nullptr) {
        return nullptr;
    }
    nnnstdMemCpy(pCopy, pInitializer->ptr(), size);
    return pCopy;
}

// 0x0048F0BC | nintendogs:bytes [tier B]
void nw::gfx::ParticleUtil::GetMemorySizeForDuplicateResParticleUpdaterInternal(os::MemorySizeCalculator* pSize, const res::ResParticleUpdater* pUpdater)
{
    UpdaterSizes sizes;
    GetUpdaterSizes(&sizes, pUpdater->ptr());
    AddPiece(pSize, sizes.updater);
    AddPiece(pSize, sizes.animation);
    AddPiece(pSize, sizes.keys);
    AddPiece(pSize, sizes.values);
    AddPiece(pSize, sizes.interpolator);
    AddPiece(pSize, sizes.name);
    AddPiece(pSize, sizes.volume);
    AddPiece(pSize, sizes.secondPart);
}

// 0x0048F574 | nintendogs:bytes [tier A]
void nw::gfx::ParticleUtil::GetMemorySizeForDuplicateResParticleInitializerInternal(os::MemorySizeCalculator* pSize,
                                                                                    const res::ResParticleInitializer* pInitializer)
{
    AddPiece(pSize, res::GetParticleInitializerSize(pInitializer->ptr()->typeInfo));
}

} // namespace gfx
} // namespace nw
