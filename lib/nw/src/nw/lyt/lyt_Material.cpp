#include "nw/lyt/lyt_Material.h"

#include <cstring>

#include "nn/gx/gx_Api.h"
#include "nn/math/math_Api.h"
#include "nw/lyt/internal/internal_Api.h"
#include "nw/lyt/internal/lyt_GlConstants.h"
#include "nw/lyt/lyt_AnimTransform.h"
#include "nw/lyt/lyt_DrawInfo.h"
#include "nw/lyt/lyt_GraphicsResource.h"
#include "nw/lyt/lyt_Layout.h"
#include "nw/lyt/lyt_ResourceAccessor.h"
#include "nw/lyt/lyt_ResourceFormat.h"

namespace nw {
namespace lyt {
namespace {
// the GL enums of the settings of a material (indices: TexMap::WrapMode, BlendMode, the logical
// operations)
// GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT
// 0x008A3A6C (name is ours)
const GLint s_WrapModes[] = {0x812F, 0x2901, 0x8370};
// none, GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT
// 0x008A3A78 (name is ours)
const GLenum s_BlendOps[] = {0, 0x8006, 0x800A, 0x800B};
// GL_ZERO, GL_ONE, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
// GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA
// 0x008A3A88 (name is ours)
const GLenum s_BlendSrcFactors[] = {0x0000, 0x0001, 0x0306, 0x0307, 0x0302, 0x0303, 0x0304, 0x0305};
// GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
// GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA
// 0x008A3AA8 (name is ours)
const GLenum s_BlendDstFactors[] = {0x0000, 0x0001, 0x0300, 0x0301, 0x0302, 0x0303, 0x0304, 0x0305};
// none, GL_CLEAR, GL_AND, GL_AND_REVERSE, GL_COPY, GL_AND_INVERTED, GL_NOOP, GL_XOR, GL_OR, GL_NOR,
// GL_EQUIV, GL_INVERT, GL_OR_REVERSE, GL_COPY_INVERTED, GL_OR_INVERTED, GL_NAND, GL_SET
// 0x008A3AC8 (name is ours)
const GLenum s_LogicOps[] = {0x0000, 0x1505, 0x1500, 0x150F, 0x1503, 0x150C, 0x150A, 0x1501,
                             0x150E, 0x1507, 0x1508, 0x1506, 0x1509, 0x1502, 0x1504, 0x150B};

// the default alpha test (always) and blending (source alpha)
const u8 ALPHA_COMPARE_DEFAULT_FUNC = 7;
const u8 BLEND_DEFAULT_SRC_FACTOR = 4;
const u8 BLEND_DEFAULT_DST_FACTOR = 5;

inline f32 DegToFIdx(f32 deg)
{
    return deg * (256.0f / 360.0f);
}

// 0x004B5514 (name is ours)
// the matrix of the texture coordinates (rotation and scale around the center, the part of the
// texture the image covers, v upwards)
DECOMP_NOINLINE void CalcTextureMtx(f32 (*pMtx)[3], const TexSRT& texSRT, const TexMap& texMap)
{
    f32 sinR, cosR;
    nn::math::SinCosFIdx(&sinR, &cosR, DegToFIdx(texSRT.rotate));

    pMtx[0][0] = texSRT.scale.x * cosR;
    pMtx[0][1] = -(sinR * texSRT.scale.y);
    pMtx[0][2] = texSRT.translate.x + 0.5f - pMtx[0][0] * 0.5f - pMtx[0][1] * 0.5f;
    pMtx[1][0] = texSRT.scale.x * sinR;
    pMtx[1][1] = texSRT.scale.y * cosR;
    pMtx[1][2] = texSRT.translate.y + 0.5f - pMtx[1][0] * 0.5f - pMtx[1][1] * 0.5f;

    const f32 scaleU = static_cast<f32>(texMap.m_Size.width) / static_cast<f32>(texMap.m_RealSize.width);
    const f32 scaleV = static_cast<f32>(texMap.m_Size.height) / static_cast<f32>(texMap.m_RealSize.height);
    pMtx[0][0] *= scaleU;
    pMtx[0][1] *= scaleU;
    pMtx[0][2] *= scaleU;
    pMtx[1][0] = -(scaleV * pMtx[1][0]);
    pMtx[1][1] = -(scaleV * pMtx[1][1]);
    pMtx[1][2] = 1.0f + pMtx[1][2] * -scaleV;
}
} // namespace

// 0x004BE1C8 | nintendogs:bytes-fuzzy [tier A]
nw::lyt::Material::Material(const res::Material* pRes, const ResBlockSet& resBlockSet)
{
    Init();

    internal::CopyString(m_Name, sizeof(m_Name), pRes->name);
    for (int i = 0; i < MATCOLOR_MAX; ++i) {
        m_Colors[i] = pRes->colors[i];
    }

    // the parts of the resource
    MemSize resNum;
    std::memcpy(&resNum, &pRes->flags, sizeof(resNum));
    const res::TexMap* pResTexMap = reinterpret_cast<const res::TexMap*>(pRes + 1);
    const TexSRT* pResTexSRT = reinterpret_cast<const TexSRT*>(pResTexMap + resNum.texMap);
    const TexCoordGen* pResTexCoordGen = reinterpret_cast<const TexCoordGen*>(pResTexSRT + resNum.texSRT);
    const TevStage* pResTevStage = reinterpret_cast<const TevStage*>(pResTexCoordGen + resNum.texCoordGen);
    const AlphaCompare* pResAlphaCompare = reinterpret_cast<const AlphaCompare*>(pResTevStage + resNum.tevStage);
    const BlendMode* pResBlendMode = reinterpret_cast<const BlendMode*>(reinterpret_cast<uptr>(pResAlphaCompare) + (resNum.alpCmp ? sizeof(AlphaCompare) : 0));

    u8 texMapNum = resNum.texMap;
    if (texMapNum > TEXMAP_MAX) {
        texMapNum = TEXMAP_MAX;
    }
    u8 texSRTNum = resNum.texSRT;
    if (texSRTNum > TEXMAP_MAX) {
        texSRTNum = TEXMAP_MAX;
    }
    u8 texCoordGenNum = resNum.texCoordGen;
    if (texCoordGenNum > TEXMAP_MAX) {
        texCoordGenNum = TEXMAP_MAX;
    }
    u8 tevStageNum = resNum.tevStage;
    if (tevStageNum > TEVSTAGE_MAX) {
        tevStageNum = TEVSTAGE_MAX;
    }
    const bool allocAlpCmp = resNum.alpCmp != 0;
    const bool allocBlendMode = resNum.blendMode != 0;

    ReserveMem(texMapNum, texSRTNum, texCoordGenNum, tevStageNum, allocAlpCmp, allocBlendMode);
    m_Flag = (m_Flag & ~FLAG_TEXTURE_ONLY) | (((pRes->flags & (1 << 11)) != 0) << 1);

    if (m_pMem == nullptr) {
        return;
    }

    // the textures
    SetTexMapNum(texMapNum);
    if (texMapNum > 0) {
        const res::TextureList* pTextureList = resBlockSet.pTextureList;
        const u32* fileNameOffsets = pTextureList->nameOffsets;
        TexMap* texMaps = GetTexMapAry();
        for (u8 i = 0; i < texMapNum; ++i) {
            const char* fileName = reinterpret_cast<const char*>(reinterpret_cast<uptr>(fileNameOffsets) + fileNameOffsets[pResTexMap[i].texIdx]);
            TexMap& texMap = texMaps[i];
            texMap.Set(resBlockSet.pResAccessor->GetTexture(fileName));
            texMap.SetWrapMode(pResTexMap[i].wrapSFlag & 3, pResTexMap[i].wrapTFlag & 3);
            texMap.SetFilter((pResTexMap[i].wrapSFlag >> 2) & 3, (pResTexMap[i].wrapTFlag >> 2) & 3);
        }
    }

    // the texture SRTs
    TexSRT* texSRTs = GetTexSRTAry();
    if (texSRTNum > 0) {
        for (int i = m_MemNum.texSRT; i < texSRTNum; ++i) {
            new (&texSRTs[i]) TexSRT();
        }
    }
    m_MemNum.texSRT = texSRTNum;
    SetCacheDirty();
    for (int i = 0; i < texSRTNum; ++i) {
        texSRTs[i].translate = pResTexSRT[i].translate;
        texSRTs[i].rotate = pResTexSRT[i].rotate;
        texSRTs[i].scale = pResTexSRT[i].scale;
    }

    // the texture coordinate generators
    TexCoordGen* texCoordGens = GetTexCoordGenAry();
    SetTexCoordGenNum(texCoordGenNum);
    for (int i = 0; i < GetTexCoordGenNum(); ++i) {
        texCoordGens[i] = pResTexCoordGen[i];
    }

    // the texture combiner
    if (tevStageNum > 0) {
        SetTevStageNum(tevStageNum);
        TevStage* tevStages = const_cast<TevStage*>(GetTevStageAry());
        for (int i = 0; i < tevStageNum; ++i) {
            tevStages[i] = pResTevStage[i];
        }
    }

    if (allocAlpCmp) {
        *const_cast<AlphaCompare*>(GetAlphaComparePtr()) = *pResAlphaCompare;
    }
    if (allocBlendMode) {
        *const_cast<BlendMode*>(GetBlendModePtr()) = *pResBlendMode;
    }
}

// 0x004BE8A8 slot 0x00 | libgarden
// 0x004BE864 (deleting dtor)
nw::lyt::Material::~Material()
{
    UnbindAnimation(nullptr);
    if (m_pMem != nullptr) {
        Layout::FreeMemory(m_pMem);
        m_pMem = nullptr;
    }
}

// 0x004BE114 | nintendogs:bytes [tier A]
void nw::lyt::Material::Init()
{
    m_Colors[MATCOLOR_BLACK] = ut::Color8(0, 0, 0, 0);
    m_Colors[MATCOLOR_WHITE] = ut::Color8(0xFF, 0xFF, 0xFF, 0xFF);
    m_Colors[MATCOLOR_C0] = ut::Color8(0xFF, 0xFF, 0xFF, 0xFF);
    m_MemCap.texMap = 0;
    m_MemCap.texSRT = 0;
    m_MemCap.texCoordGen = 0;
    m_MemCap.tevStage = 0;
    m_MemCap.alpCmp = 0;
    m_MemCap.blendMode = 0;
    m_MemNum.texMap = 0;
    m_MemNum.texSRT = 0;
    m_MemNum.texCoordGen = 0;
    m_MemNum.tevStage = 0;
    m_MemNum.alpCmp = 0;
    m_MemNum.blendMode = 0;
    m_Flag = 0;
    m_pMem = nullptr;
}

// 0x004BC900 | libgarden [tier A]
void nw::lyt::Material::ReserveMem(u8 texMapNum, u8 texSRTNum, u8 texCoordGenNum, u8 tevStageNum, bool allocAlpCmp, bool allocBlendMode)
{
    if (m_MemCap.texMap >= texMapNum && m_MemCap.texSRT >= texSRTNum && m_MemCap.texCoordGen >= texCoordGenNum &&
        m_MemCap.tevStage >= tevStageNum && m_MemCap.alpCmp >= static_cast<u32>(allocAlpCmp) &&
        m_MemCap.blendMode >= static_cast<u32>(allocBlendMode)) {
        return;
    }

    if (m_pMem != nullptr) {
        Layout::FreeMemory(m_pMem);
        m_pMem = nullptr;
        m_MemCap.texMap = 0;
        m_MemCap.texSRT = 0;
        m_MemCap.texCoordGen = 0;
        m_MemCap.tevStage = 0;
        m_MemCap.alpCmp = 0;
        m_MemCap.blendMode = 0;
        m_MemNum.texMap = 0;
        m_MemNum.texSRT = 0;
        m_MemNum.texCoordGen = 0;
        m_MemNum.tevStage = 0;
        m_MemNum.alpCmp = 0;
        m_MemNum.blendMode = 0;
    }

    m_pMem = Layout::AllocMemory(sizeof(TexMap) * texMapNum + sizeof(TexSRT) * texSRTNum + sizeof(TexCoordGen) * texCoordGenNum +
                                 sizeof(AlphaCompare) * allocAlpCmp + sizeof(BlendMode) * allocBlendMode + sizeof(TevStage) * tevStageNum);
    if (m_pMem == nullptr) {
        return;
    }

    m_MemCap.texMap = texMapNum;
    m_MemCap.texSRT = texSRTNum;
    m_MemCap.texCoordGen = texCoordGenNum;
    m_MemCap.tevStage = tevStageNum;
    m_MemCap.alpCmp = allocAlpCmp;
    m_MemCap.blendMode = allocBlendMode;

    m_MemNum.texSRT = m_MemCap.texSRT;
    TexSRT* texSRTs = GetTexSRTAry();
    for (int i = 0; i < m_MemNum.texSRT; ++i) {
        texSRTs[i].translate = nn::math::VEC2(0.0f, 0.0f);
        texSRTs[i].rotate = 0.0f;
        texSRTs[i].scale = nn::math::VEC2(1.0f, 1.0f);
    }

    m_MemNum.alpCmp = m_MemCap.alpCmp;
    if (m_MemNum.alpCmp) {
        *const_cast<AlphaCompare*>(GetAlphaComparePtr()) = AlphaCompare(ALPHA_COMPARE_DEFAULT_FUNC, 0.0f);
    }

    m_MemNum.blendMode = m_MemCap.blendMode;
    if (m_MemNum.blendMode) {
        *const_cast<BlendMode*>(GetBlendModePtr()) = BlendMode(0, BLEND_DEFAULT_SRC_FACTOR, BLEND_DEFAULT_DST_FACTOR, 0);
    }
}

// 0x004BCBA8 | libgarden [tier A]
TexSRT* nw::lyt::Material::GetTexSRTAry()
{
    return reinterpret_cast<TexSRT*>(reinterpret_cast<uptr>(m_pMem) + sizeof(TexMap) * m_MemCap.texMap);
}

// 0x0073E860 (name is ours)
const TexSRT* nw::lyt::Material::GetTexSRTAry() const
{
    return reinterpret_cast<const TexSRT*>(reinterpret_cast<uptr>(m_pMem) + sizeof(TexMap) * m_MemCap.texMap);
}

// 0x004BE004 | nintendogs:callseq-callee [tier A]
TexCoordGen* nw::lyt::Material::GetTexCoordGenAry()
{
    return reinterpret_cast<TexCoordGen*>(reinterpret_cast<uptr>(GetTexSRTAry()) + sizeof(TexSRT) * m_MemCap.texSRT);
}

// 0x0073E900 (name is ours)
const TexCoordGen* nw::lyt::Material::GetTexCoordGenAry() const
{
    return reinterpret_cast<const TexCoordGen*>(reinterpret_cast<uptr>(GetTexSRTAry()) + sizeof(TexSRT) * m_MemCap.texSRT);
}

// 0x0073E928 | nintendogs:bytes [tier A]
const AlphaCompare* nw::lyt::Material::GetAlphaComparePtr() const
{
    return reinterpret_cast<const AlphaCompare*>(reinterpret_cast<uptr>(GetTexCoordGenAry()) + sizeof(TexCoordGen) * m_MemCap.texCoordGen);
}

// 0x0073E8C0 | nintendogs:bytes [tier A]
const BlendMode* nw::lyt::Material::GetBlendModePtr() const
{
    return reinterpret_cast<const BlendMode*>(reinterpret_cast<uptr>(GetAlphaComparePtr()) + sizeof(AlphaCompare) * m_MemCap.alpCmp);
}

// 0x0073E874 | nintendogs:bytes [tier A]
const TevStage* nw::lyt::Material::GetTevStageAry() const
{
    return reinterpret_cast<const TevStage*>(reinterpret_cast<uptr>(GetBlendModePtr()) + sizeof(BlendMode) * m_MemCap.blendMode);
}

// 0x004BCBBC | nintendogs:bytes [tier A]
void nw::lyt::Material::SetTexMapNum(u8 num)
{
    if (num > 0) {
        TexMap* texMaps = GetTexMapAry();
        for (int i = m_MemNum.texMap; i < num; ++i) {
            new (&texMaps[i]) TexMap();
        }
    }
    m_MemNum.texMap = num;
    SetCacheDirty();
}

// 0x004BE02C | nintendogs:bytes [tier A]
void nw::lyt::Material::SetTexCoordGenNum(u8 num)
{
    if (num > 0) {
        TexCoordGen* texCoordGens = GetTexCoordGenAry();
        for (int i = m_MemNum.texCoordGen; i < num; ++i) {
            new (&texCoordGens[i]) TexCoordGen();
        }
    }
    m_MemNum.texCoordGen = num;
    SetCacheDirty();
}

// 0x004BDE54 | nintendogs:bytes [tier A]
void nw::lyt::Material::SetTevStageNum(u8 num)
{
    if (num > 0) {
        TevStage* tevStages = const_cast<TevStage*>(GetTevStageAry());
        for (int i = m_MemNum.tevStage; i < num; ++i) {
            new (&tevStages[i]) TevStage();
        }
    }
    m_MemNum.tevStage = num;
}

// 0x004BDF9C | nintendogs:bytes [tier A]
void nw::lyt::Material::SetColorElement(u32 idx, u8 value)
{
    if (idx < ANIMTARGET_MATCOLOR_MAX) {
        ut::Color8& color = m_Colors[idx / 4];
        switch (idx % 4) {
        case 0:
            color.r = value;
            break;
        case 1:
            color.g = value;
            break;
        case 2:
            color.b = value;
            break;
        case 3:
            color.a = value;
            break;
        }
    }
}

// 0x004BE198 (name is ours)
void nw::lyt::Material::SetTexMap(u32 idx, const TexMap* pTexMap)
{
    GetTexMapAry()[idx] = *pTexMap;
    SetCacheDirty();
}

// 0x004BDFE4 | nintendogs:callgraph [tier A]
void nw::lyt::Material::AddAnimationLink(AnimationLink* pAnimationLink)
{
    m_AnimList.PushBack(pAnimationLink);
}

// 0x004BCC18 slot 0x08 (name is ours)
void nw::lyt::Material::BindAnimation(AnimTransform* pAnimTrans)
{
    pAnimTrans->Bind(this, false);
}

// 0x004BDFDC slot 0x0C
void nw::lyt::Material::UnbindAnimation(AnimTransform* pAnimTrans)
{
    internal::UnbindAnimationLink(&m_AnimList, pAnimTrans);
}

// 0x004BE104 slot 0x10 (name is ours)
void nw::lyt::Material::UnbindAllAnimation()
{
    UnbindAnimation(nullptr);
}

// 0x004BE14C slot 0x14 | nintendogs:bytes
void nw::lyt::Material::Animate()
{
    for (AnimationList::iterator it = m_AnimList.begin(); it != m_AnimList.end(); ++it) {
        AnimationLink* pLink = AnimationList::GetPointerFromNode(it.GetNode());
        if (!pLink->m_IsDisable) {
            pLink->m_pAnimTransform->Animate(pLink->m_Index, this);
        }
    }
}

// 0x004BDFF4 slot 0x18 (name is ours)
AnimationLink* nw::lyt::Material::FindAnimationLink(AnimTransform* pAnimTrans)
{
    return internal::FindAnimationLink(&m_AnimList, pAnimTrans);
}

// 0x004BDFFC slot 0x1C (name is ours)
AnimationLink* nw::lyt::Material::FindAnimationLink(const AnimResource& animRes)
{
    return internal::FindAnimationLink(&m_AnimList, animRes);
}

// 0x004BE0BC slot 0x20 (name is ours)
void nw::lyt::Material::SetAnimationEnable(AnimTransform* pAnimTrans, bool bEnable)
{
    AnimationLink* pAnimLink = FindAnimationLink(pAnimTrans);
    if (pAnimLink != nullptr) {
        pAnimLink->m_IsDisable = !bEnable;
    }
}

// 0x004BE0E0 slot 0x24 (name is ours)
void nw::lyt::Material::SetAnimationEnable(const AnimResource& animRes, bool bEnable)
{
    AnimationLink* pAnimLink = FindAnimationLink(animRes);
    if (pAnimLink != nullptr) {
        pAnimLink->m_IsDisable = !bEnable;
    }
}

// 0x004BCC34 slot 0x28 (name is ours)
void nw::lyt::Material::SetupGL(const DrawInfo& drawInfo, u8 alpha, bool bInitFrameSpec)
{
    GraphicsResource& graphicsResource = *drawInfo.m_pGraphicsResource;

    if (!IsTextureOnly()) {
        glUseProgram(graphicsResource.m_GlProgram);
        if (bInitFrameSpec) {
            glUniform4f(graphicsResource.GetUniformLocation(GraphicsResource::UNIFORM_FRAMESPEC), 0.0f, 0.0f, 0.0f, 0.0f);
        }
        // the colors of the pane shader are 0-255, the alpha of the pane scales them
        glUniform4f(graphicsResource.GetUniformLocation(GraphicsResource::UNIFORM_COLOR), 1.0f / 255.0f, 1.0f / 255.0f, 1.0f / 255.0f,
                    (1.0f / (255.0f * 255.0f)) * alpha);

        if (GetTexCoordGenNum() > 0) {
            u32 i = 0;
            for (; i < GetTexCoordGenNum(); ++i) {
                graphicsResource.m_VertexTexCoordSrc[i] = GetTexCoordGenAry()[i].texGenSrc;
            }
            for (; i < GraphicsResource::VERTEX_TEXCOORD_MAX; ++i) {
                graphicsResource.m_VertexTexCoordSrc[i] = -1;
            }
        }
    }

    // the texture matrices
    if (GetTexSRTNum() > 0) {
        u32 num = GetTexMapNum();
        if (GetTexSRTNum() <= num) {
            num = GetTexSRTNum();
        }
        if (num > TEXMAP_MAX) {
            num = TEXMAP_MAX;
        }
        for (u32 i = 0; i < num; ++i) {
            f32 texMtx[2][3];
            CalcTextureMtx(texMtx, GetTexSRTAry()[i], GetTexMapAry()[i]);
            const GLint loc = graphicsResource.GetUniformLocation(GraphicsResource::UNIFORM_TEXMTX0 + i);
            glUniform4f(loc, texMtx[0][0], texMtx[0][1], 0.0f, texMtx[0][2]);
            glUniform4f(loc + 1, texMtx[1][0], texMtx[1][1], 0.0f, texMtx[1][2]);
        }
    }

    // the textures
    if (GetTexMapNum() > 0) {
        u32 i = 0;
        for (; i < GetTexMapNum(); ++i) {
            const TexMap& texMap = GetTexMapAry()[i];
            glActiveTexture(internal::gl::TEXTURE0 + i);
            glBindTexture(internal::gl::TEXTURE_2D, texMap.m_TexObject);
            graphicsResource.SetTextureSamplerType(i, internal::gl::TEXTURE_2D);
            glTexParameteri(internal::gl::TEXTURE_2D, internal::gl::TEXTURE_WRAP_S, s_WrapModes[texMap.GetWrapModeS()]);
            glTexParameteri(internal::gl::TEXTURE_2D, internal::gl::TEXTURE_WRAP_T, s_WrapModes[texMap.GetWrapModeT()]);
            GraphicsResource::SetTexMinFilter(texMap.GetMinFilter());
            GraphicsResource::SetTexMagFilter(texMap.GetMagFilter());
            if (i == 0) {
                glUniform4f(graphicsResource.GetUniformLocation(GraphicsResource::UNIFORM_RCPTEXSIZE0), 1.0f / texMap.m_Size.width,
                            1.0f / texMap.m_Size.height, 0.0f, 0.0f);
            }
            glTexParameterf(internal::gl::TEXTURE_2D, internal::gl::TEXTURE_LOD_BIAS, 0.0f);
            glTexParameteri(internal::gl::TEXTURE_2D, internal::gl::TEXTURE_MIN_LOD, -1000);
        }
        for (; i < 4; ++i) {
            graphicsResource.SetTextureSamplerType(i, 0);
        }
    }

    // the texture combiner
    if (!IsTextureOnly()) {
        graphicsResource.SetTexEnvBufferColor(m_Colors[MATCOLOR_BLACK]);

        u32 stage = 0;
        if (GetTevStageNum() > 0) {
            graphicsResource.m_IsTexEnvModeValid = false;
            graphicsResource.m_TexEnvMode = 0;

            const u32 tevStageNum = (TEVSTAGE_MAX < GetTevStageNum()) ? TEVSTAGE_MAX : GetTevStageNum();
            for (; stage < tevStageNum; ++stage) {
                const TevStage& tevStage = GetTevStageAry()[stage];
                graphicsResource.SetTexEnvCombineRgb(stage, tevStage.rgb.combine);
                graphicsResource.SetTexEnvCombineAlpha(stage, tevStage.alpha.combine);
                graphicsResource.SetTexEnvSrcRgb(stage, tevStage.rgb.src0, tevStage.rgb.src1, tevStage.rgb.src2);
                graphicsResource.SetTexEnvSrcAlpha(stage, tevStage.alpha.src0, tevStage.alpha.src1, tevStage.alpha.src2);
                graphicsResource.SetTexEnvOperandRgb(stage, tevStage.rgb.op0, tevStage.rgb.op1, tevStage.rgb.op2);
                graphicsResource.SetTexEnvOperandAlpha(stage, tevStage.alpha.op0, tevStage.alpha.op1, tevStage.alpha.op2);
                graphicsResource.SetTexEnvScaleRgb(stage, tevStage.rgb.scale);
                graphicsResource.SetTexEnvScaleAlpha(stage, tevStage.alpha.scale);
                graphicsResource.SetTexEnvConstColor(stage, m_Colors[tevStage.konstColor.rgb], m_Colors[tevStage.konstColor.alpha]);
                if (stage >= 1 && stage <= 4) {
                    graphicsResource.SetTexEnvBufferInput(stage, tevStage.rgb.isSavePrevReg, tevStage.alpha.isSavePrevReg);
                }
            }
        } else {
            // the combiner of the number of textures (set again only when it changes)
            const bool isChanged = !(graphicsResource.m_IsTexEnvModeValid && graphicsResource.m_TexEnvMode == GetTexMapNum());
            graphicsResource.m_IsTexEnvModeValid = true;
            graphicsResource.m_TexEnvMode = GetTexMapNum();

            switch (GetTexMapNum()) {
            case 0:
                // the white color
                graphicsResource.SetTexEnvConstColor(0, m_Colors[MATCOLOR_WHITE]);
                if (isChanged) {
                    graphicsResource.SetTexEnvCombineRgb(0, 1);
                    graphicsResource.SetTexEnvCombineAlpha(0, 1);
                    graphicsResource.SetTexEnvSrcRgb(0, 4, 5, 5);
                    graphicsResource.SetTexEnvSrcAlpha(0, 4, 5, 5);
                    graphicsResource.SetTexEnvOperandRgb(0, 0, 0, 0);
                    graphicsResource.SetTexEnvOperandAlpha(0, 0, 0, 0);
                    graphicsResource.SetTexEnvScaleRgb(0, 0);
                    graphicsResource.SetTexEnvScaleAlpha(0, 0);
                }
                stage = 1;
                break;
            case 1:
                // the texture, mapped between black and white
                if (isChanged) {
                    graphicsResource.SetTexEnvCombineRgb(0, 0);
                    graphicsResource.SetTexEnvCombineAlpha(0, 0);
                    graphicsResource.SetTexEnvSrcRgb(0, 0, 0, 0);
                    graphicsResource.SetTexEnvSrcAlpha(0, 0, 0, 0);
                    graphicsResource.SetTexEnvOperandRgb(0, 0, 0, 0);
                    graphicsResource.SetTexEnvOperandAlpha(0, 0, 0, 0);
                    graphicsResource.SetTexEnvScaleRgb(0, 0);
                    graphicsResource.SetTexEnvScaleAlpha(0, 0);
                }
                graphicsResource.SetTexEnvConstColor(1, m_Colors[MATCOLOR_WHITE]);
                if (isChanged) {
                    graphicsResource.SetTexEnvCombineRgb(1, 4);
                    graphicsResource.SetTexEnvCombineAlpha(1, 4);
                    graphicsResource.SetTexEnvSrcRgb(1, 4, 7, 6);
                    graphicsResource.SetTexEnvSrcAlpha(1, 4, 7, 6);
                    graphicsResource.SetTexEnvOperandRgb(1, 0, 0, 0);
                    graphicsResource.SetTexEnvOperandAlpha(1, 0, 0, 0);
                    graphicsResource.SetTexEnvScaleRgb(1, 0);
                    graphicsResource.SetTexEnvScaleAlpha(1, 0);
                    graphicsResource.SetTexEnvCombineRgb(2, 1);
                    graphicsResource.SetTexEnvCombineAlpha(2, 1);
                    graphicsResource.SetTexEnvSrcRgb(2, 5, 6, 6);
                    graphicsResource.SetTexEnvSrcAlpha(2, 5, 6, 6);
                    graphicsResource.SetTexEnvOperandRgb(2, 0, 0, 0);
                    graphicsResource.SetTexEnvOperandAlpha(2, 0, 0, 0);
                    graphicsResource.SetTexEnvScaleRgb(2, 0);
                    graphicsResource.SetTexEnvScaleAlpha(2, 0);
                }
                stage = 3;
                break;
            case 2:
                // two textures blended by the alpha of C4
                graphicsResource.SetTexEnvConstColor(0, m_Colors[MATCOLOR_C4]);
                if (isChanged) {
                    graphicsResource.SetTexEnvCombineRgb(0, 1);
                    graphicsResource.SetTexEnvCombineAlpha(0, 1);
                    graphicsResource.SetTexEnvSrcRgb(0, 4, 0, 4);
                    graphicsResource.SetTexEnvSrcAlpha(0, 4, 0, 4);
                    graphicsResource.SetTexEnvOperandRgb(0, 2, 0, 0);
                    graphicsResource.SetTexEnvOperandAlpha(0, 0, 0, 0);
                    graphicsResource.SetTexEnvScaleRgb(0, 0);
                    graphicsResource.SetTexEnvScaleAlpha(0, 0);
                }
                graphicsResource.SetTexEnvConstColor(1, m_Colors[MATCOLOR_C4]);
                if (isChanged) {
                    graphicsResource.SetTexEnvCombineRgb(1, 7);
                    graphicsResource.SetTexEnvCombineAlpha(1, 7);
                    graphicsResource.SetTexEnvSrcRgb(1, 4, 1, 6);
                    graphicsResource.SetTexEnvSrcAlpha(1, 4, 1, 6);
                    graphicsResource.SetTexEnvOperandRgb(1, 3, 0, 0);
                    graphicsResource.SetTexEnvOperandAlpha(1, 1, 0, 0);
                    graphicsResource.SetTexEnvScaleRgb(1, 0);
                    graphicsResource.SetTexEnvScaleAlpha(1, 0);
                    graphicsResource.SetTexEnvBufferInput(1, false, false);
                }
                graphicsResource.SetTexEnvConstColor(2, m_Colors[MATCOLOR_WHITE]);
                if (isChanged) {
                    graphicsResource.SetTexEnvCombineRgb(2, 4);
                    graphicsResource.SetTexEnvCombineAlpha(2, 4);
                    graphicsResource.SetTexEnvSrcRgb(2, 4, 7, 6);
                    graphicsResource.SetTexEnvSrcAlpha(2, 4, 7, 6);
                    graphicsResource.SetTexEnvOperandRgb(2, 0, 0, 0);
                    graphicsResource.SetTexEnvOperandAlpha(2, 0, 0, 0);
                    graphicsResource.SetTexEnvScaleRgb(2, 0);
                    graphicsResource.SetTexEnvScaleAlpha(2, 0);
                    graphicsResource.SetTexEnvCombineRgb(3, 1);
                    graphicsResource.SetTexEnvCombineAlpha(3, 1);
                    graphicsResource.SetTexEnvSrcRgb(3, 5, 6, 6);
                    graphicsResource.SetTexEnvSrcAlpha(3, 5, 6, 6);
                    graphicsResource.SetTexEnvOperandRgb(3, 0, 0, 0);
                    graphicsResource.SetTexEnvOperandAlpha(3, 0, 0, 0);
                    graphicsResource.SetTexEnvScaleRgb(3, 0);
                    graphicsResource.SetTexEnvScaleAlpha(3, 0);
                }
                stage = 4;
                break;
            case 3:
                // three textures blended by the color of C4
                graphicsResource.SetTexEnvConstColor(0, m_Colors[MATCOLOR_C4]);
                if (isChanged) {
                    graphicsResource.SetTexEnvCombineRgb(0, 1);
                    graphicsResource.SetTexEnvCombineAlpha(0, 1);
                    graphicsResource.SetTexEnvSrcRgb(0, 4, 0, 4);
                    graphicsResource.SetTexEnvSrcAlpha(0, 4, 0, 4);
                    graphicsResource.SetTexEnvOperandRgb(0, 2, 0, 0);
                    graphicsResource.SetTexEnvOperandAlpha(0, 0, 0, 0);
                    graphicsResource.SetTexEnvScaleRgb(0, 0);
                    graphicsResource.SetTexEnvScaleAlpha(0, 0);
                }
                graphicsResource.SetTexEnvConstColor(1, m_Colors[MATCOLOR_C4]);
                if (isChanged) {
                    graphicsResource.SetTexEnvCombineRgb(1, 7);
                    graphicsResource.SetTexEnvCombineAlpha(1, 7);
                    graphicsResource.SetTexEnvSrcRgb(1, 4, 1, 6);
                    graphicsResource.SetTexEnvSrcAlpha(1, 4, 1, 6);
                    graphicsResource.SetTexEnvOperandRgb(1, 8, 0, 0);
                    graphicsResource.SetTexEnvOperandAlpha(1, 6, 0, 0);
                    graphicsResource.SetTexEnvScaleRgb(1, 0);
                    graphicsResource.SetTexEnvScaleAlpha(1, 0);
                    graphicsResource.SetTexEnvBufferInput(1, false, false);
                }
                graphicsResource.SetTexEnvConstColor(2, m_Colors[MATCOLOR_C4]);
                if (isChanged) {
                    graphicsResource.SetTexEnvCombineRgb(2, 7);
                    graphicsResource.SetTexEnvCombineAlpha(2, 7);
                    graphicsResource.SetTexEnvSrcRgb(2, 4, 2, 6);
                    graphicsResource.SetTexEnvSrcAlpha(2, 4, 2, 6);
                    graphicsResource.SetTexEnvOperandRgb(2, 6, 0, 0);
                    graphicsResource.SetTexEnvOperandAlpha(2, 4, 0, 0);
                    graphicsResource.SetTexEnvScaleRgb(2, 0);
                    graphicsResource.SetTexEnvScaleAlpha(2, 0);
                    graphicsResource.SetTexEnvBufferInput(2, false, false);
                }
                graphicsResource.SetTexEnvConstColor(3, m_Colors[MATCOLOR_WHITE]);
                if (isChanged) {
                    graphicsResource.SetTexEnvCombineRgb(3, 4);
                    graphicsResource.SetTexEnvCombineAlpha(3, 4);
                    graphicsResource.SetTexEnvSrcRgb(3, 4, 7, 6);
                    graphicsResource.SetTexEnvSrcAlpha(3, 4, 7, 6);
                    graphicsResource.SetTexEnvOperandRgb(3, 0, 0, 0);
                    graphicsResource.SetTexEnvOperandAlpha(3, 0, 0, 0);
                    graphicsResource.SetTexEnvScaleRgb(3, 0);
                    graphicsResource.SetTexEnvScaleAlpha(3, 0);
                    graphicsResource.SetTexEnvCombineRgb(4, 1);
                    graphicsResource.SetTexEnvCombineAlpha(4, 1);
                    graphicsResource.SetTexEnvSrcRgb(4, 5, 6, 6);
                    graphicsResource.SetTexEnvSrcAlpha(4, 5, 6, 6);
                    graphicsResource.SetTexEnvOperandRgb(4, 0, 0, 0);
                    graphicsResource.SetTexEnvOperandAlpha(4, 0, 0, 0);
                    graphicsResource.SetTexEnvScaleRgb(4, 0);
                    graphicsResource.SetTexEnvScaleAlpha(4, 0);
                }
                stage = 5;
                break;
            }
        }

        // the stages behind pass the previous result on
        const u8 prevTexEnvNum = graphicsResource.m_NumTexEnv;
        graphicsResource.m_NumTexEnv = stage;
        for (; stage < prevTexEnvNum; ++stage) {
            graphicsResource.SetTexEnvCombineRgb(stage, 0);
            graphicsResource.SetTexEnvCombineAlpha(stage, 0);
            graphicsResource.SetTexEnvSrcRgb(stage, 6, 6, 6);
            graphicsResource.SetTexEnvSrcAlpha(stage, 6, 6, 6);
            graphicsResource.SetTexEnvOperandRgb(stage, 0, 0, 0);
            graphicsResource.SetTexEnvOperandAlpha(stage, 0, 0, 0);
            graphicsResource.SetTexEnvScaleRgb(stage, 0);
            graphicsResource.SetTexEnvScaleAlpha(stage, 0);
        }
    }

    // the alpha test
    if (m_MemCap.alpCmp) {
        const AlphaCompare& alphaCompare = *GetAlphaComparePtr();
        graphicsResource.SetAlphaTestEnable(true);
        graphicsResource.SetAlphaRefValue(alphaCompare.ref);
        graphicsResource.SetAlphaTestFunc(alphaCompare.func);
    } else {
        graphicsResource.SetAlphaTestEnable(true);
        graphicsResource.SetAlphaRefValue(0.0f);
        graphicsResource.SetAlphaTestFunc(6);
    }

    // the blending
    if (IsTextureOnly()) {
        return;
    }
    if (m_MemCap.blendMode) {
        const BlendMode& blendMode = *GetBlendModePtr();
        if (blendMode.blendOp != 0) {
            glEnable(internal::gl::BLEND);
            glBlendFunc(s_BlendSrcFactors[blendMode.srcFactor], s_BlendDstFactors[blendMode.dstFactor]);
            glBlendEquation(s_BlendOps[blendMode.blendOp]);
        } else {
            glDisable(internal::gl::BLEND);
        }
        if (blendMode.logicOp != 0) {
            glEnable(internal::gl::COLOR_LOGIC_OP);
            glLogicOp(s_LogicOps[blendMode.logicOp]);
        } else {
            glDisable(internal::gl::COLOR_LOGIC_OP);
        }
    } else {
        glEnable(internal::gl::BLEND);
        glBlendFunc(internal::gl::SRC_ALPHA, internal::gl::ONE_MINUS_SRC_ALPHA);
        glDisable(internal::gl::COLOR_LOGIC_OP);
    }
}

} // namespace lyt
} // namespace nw
