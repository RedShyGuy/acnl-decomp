#include "g3d/modelEffect/dModelEffectHandle.h"
#include "Bs/dBsModelEffectMgr.h"
#include "g3d/modelEffect/dModelEffectContainer.h"

namespace g3d {
namespace modelEffect {
// 0x004EBA28 (name is ours)
void g3d::modelEffect::ModelEffectHandle::SetMatrix(const nn::math::MTX34& matrix)
{
    ModelEffectContainer* container = GetContainer();
    if (container != NULL) {
        container->SetMatrix(matrix);
    }
}

// 0x004EBA68 (name is ours)
bool g3d::modelEffect::ModelEffectHandle::Fade()
{
    ModelEffectContainer* container = GetContainer();
    if (container == NULL) {
        return false;
    }
    if (!container->StartFade(ModelEffectContainer::FADE_FRAMES)) {
        return false;
    }
    Release();
    return true;
}

// 0x004EBAC0 (name is ours)
void g3d::modelEffect::ModelEffectHandle::SetModelMatrix(const nn::math::MTX34& matrix)
{
    ModelEffectContainer* container = GetContainer();
    if (container != NULL) {
        container->SetModelMatrix(matrix);
    }
}

// 0x004EBB00 (name is ours)
g3d::modelEffect::ModelEffectHandle::ModelEffectHandle() : m_Index(-1), m_Serial(-1)
{
}

// 0x004EBC3C slot 0x00
// 0x004EBB1C (deleting dtor)
g3d::modelEffect::ModelEffectHandle::~ModelEffectHandle()
{
    ModelEffectContainer* container = GetContainer();
    if (container != NULL && container->IsLooping()) {
        Fade();
    }
}

// 0x00746628 (name is ours; a copy for the game, inline in the original)
ModelEffectContainer* g3d::modelEffect::ModelEffectHandle::GetContainer() const
{
    if (BsModelEffectMgr::GetInstance() == NULL) {
        return NULL;
    }
    return BsModelEffectMgr::GetInstance()->FindEffect(m_Index, m_Serial);
}

} // namespace modelEffect
} // namespace g3d
