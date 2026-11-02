#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector3.h"

namespace nw {
namespace gfx {
class TransformNode;
} // namespace gfx
} // namespace nw

namespace g3d {
// an object of the scene: owns an nw node (names after libgarden except the setters)
// RTTI N3g3d13TransformNodeE @ 0x008D0CD0
// vtable 0x00903A1C (vptr 0x00903A24), offset_to_top 0, 4 entries
class TransformNode
{
public:
    DECOMP_EMIT_COPY DECOMP_NOINLINE TransformNode() : m_pNode(NULL) {} // 0x004EEB74
    virtual ~TransformNode(); // 0x004EEB9C slot 0x00
    // 0x004EEB88 (deleting dtor)
    // the world matrix of the node
    virtual void Update(); // 0x004EEB40 slot 0x08 | libgarden
    // destroys the node
    virtual void Finalize(); // 0x004EEA24 slot 0x0C | libgarden

    // the local scale; components near zero become +-0.01 (names are ours)
    void SetScale(const nn::math::VEC3& scale); // 0x004EEA70
    // the local matrix
    void SetMatrix(const nn::math::MTX34& matrix); // 0x004EEB50
    // the translation of the local matrix
    void SetTranslate(const nn::math::VEC3& translate); // 0x004EE938
    // the world matrix and the world transform of another node
    void CopyWorld(const TransformNode& other); // 0x004EE964

    // the node is visited by the traverser of a scene (it is in the scene context then)
    void AddToScene(s32 sceneIdx); // 0x004EE9EC

    nw::gfx::TransformNode* GetNode() const { return m_pNode; }

    nw::gfx::TransformNode* m_pNode; // 0x4
};
ASSERT_SIZE(TransformNode, 0x8);
} // namespace g3d
