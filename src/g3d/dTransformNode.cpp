#include "g3d/dTransformNode.h"
#include "g3d/g3d_System.h"
#include "nw/gfx/gfx_SceneTraverser.h"
#include "nw/gfx/gfx_TransformNode.h"
#include "nn/math/math_ARMv6.h"

namespace g3d {
namespace {
// the smallest absolute value of a local scale component
const f32 MIN_LOCAL_SCALE = 0.01f;

// (inline) a scale component kept away from zero (name is ours)
inline f32 ClampScale(f32 value)
{
    if (value > -MIN_LOCAL_SCALE && value < MIN_LOCAL_SCALE) {
        value = (value >= 0.0f) ? MIN_LOCAL_SCALE : -MIN_LOCAL_SCALE;
    }
    return value;
}
} // namespace

// 0x004EE938 (name is ours)
void g3d::TransformNode::SetTranslate(const nn::math::VEC3& translate)
{
    nw::gfx::CalculatedTransform& transform = m_pNode->m_Transform;
    transform.SetTranslate(translate);
    transform.EnableFlags(nw::gfx::CalculatedTransform::FLAG_IS_DIRTY);
}

// 0x004EE964 (name is ours)
void g3d::TransformNode::CopyWorld(const TransformNode& other)
{
    m_pNode->m_WorldMatrix = other.m_pNode->m_WorldMatrix;
    m_pNode->m_WorldTransform = other.m_pNode->m_WorldTransform;
}

// 0x004EEA24 slot 0x0C | libgarden
void g3d::TransformNode::Finalize()
{
    nw::gfx::SafeDestroy(m_pNode);
}

// 0x004EEA70 (name is ours)
void g3d::TransformNode::SetScale(const nn::math::VEC3& scale)
{
    f32 x = ClampScale(scale.x);
    f32 y = ClampScale(scale.y);
    f32 z = ClampScale(scale.z);
    nw::gfx::CalculatedTransform& transform = m_pNode->m_Transform;
    transform.m_Scale.x = x;
    transform.m_Scale.y = y;
    transform.m_Scale.z = z;
    transform.EnableFlags(nw::gfx::CalculatedTransform::FLAG_IS_DIRTY);
}

// 0x004EEB40 slot 0x08 | libgarden
void g3d::TransformNode::Update()
{
    UpdateWorldMatrix(m_pNode);
}

// 0x004EEB50 (name is ours)
void g3d::TransformNode::SetMatrix(const nn::math::MTX34& matrix)
{
    nw::gfx::CalculatedTransform& transform = m_pNode->m_Transform;
    nn::math::ARMv6::MTX34CopyAsm(&transform.m_TransformMatrix, &matrix);
    transform.EnableFlags(nw::gfx::CalculatedTransform::FLAG_IS_DIRTY);
}

// 0x004EEB9C slot 0x00
// 0x004EEB88 (deleting dtor)
g3d::TransformNode::~TransformNode()
{
    // empty (in the original too)
}

// 0x004EE9EC (name is ours)
void g3d::TransformNode::AddToScene(s32 sceneIdx)
{
    m_pNode->Accept(s_pSceneTraversers[sceneIdx]);
}

} // namespace g3d
