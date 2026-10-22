#pragma once

#include "decomp.h"
#include "nn/math/math_ARMv6.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector2.h"

namespace nw {
namespace gfx {
namespace res {
// A texture coordinator of a material: how the texture coordinates of a texture unit are made (the
// class name and its copy constructor are from the reference symbols; the members are ours, only
// the mapping method and the enable flag are understood so far).
class ResTextureCoordinatorData
{
public:
    // the mapping methods (names are ours)
    enum MappingMethod : u8
    {
        MAPPING_UV = 0,
        MAPPING_CUBE_ENV = 1,
        MAPPING_SPHERE_ENV = 2,
        MAPPING_PROJECTION = 3,
        MAPPING_SHADOW = 4,
        MAPPING_SHADOW_CUBE = 5,
    };

    // the copy (out of line; the matrix with MTX34CopyAsm)
    ResTextureCoordinatorData(const ResTextureCoordinatorData& other); // 0x004A8FCC | mk7dlp:bytes [tier B]

    MappingMethod GetMappingMethod() const { return static_cast<MappingMethod>(mappingMethod); }

    u32 m_Unknown0x0;              // 0x00
    u32 mappingMethod;             // 0x04 MappingMethod
    u32 m_Unknown0x8;              // 0x08
    u32 m_Unknown0xC;              // 0x0C
    nn::math::VEC2 m_Unknown0x10;  // 0x10
    f32 m_Unknown0x18;             // 0x18
    nn::math::VEC2 m_Unknown0x1C;  // 0x1C
    u8 m_Unknown0x24;              // 0x24
    bool isEnabled;                // 0x25 (set by ResMaterial::SetupTextures)
    u16 m_Unknown0x26;             // 0x26
    nn::math::MTX34 textureMatrix; // 0x28
};
ASSERT_SIZE(ResTextureCoordinatorData, 0x58);
} // namespace res
} // namespace gfx
} // namespace nw
