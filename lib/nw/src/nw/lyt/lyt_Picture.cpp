#include "nw/lyt/lyt_Picture.h"

#include "nw/lyt/internal/internal_Api.h"
#include "nw/lyt/lyt_DrawInfo.h"
#include "nw/lyt/lyt_Drawer.h"
#include "nw/lyt/lyt_Layout.h"
#include "nw/lyt/lyt_Material.h"
#include "nw/lyt/lyt_ResourceFormat.h"
#include "nw/lyt/lyt_TexMap.h"

namespace nw {
namespace lyt {
// 0x00976168 (name is ours)
const PaneTypeInfo nw::lyt::Picture::s_TypeInfo(&Pane::s_TypeInfo);

// sets s_TypeInfo
// 0x00793010 (static initializer)

// 0x004BBD88 | nintendogs:bytes-fuzzy [tier A]
nw::lyt::Picture::Picture(const res::Picture* pResPic, const ResBlockSet& resBlockSet) : Pane(pResPic)
{
    u8 texCoordNum = pResPic->texCoordNum;
    if (texCoordNum > Material::TEXMAP_MAX) {
        texCoordNum = Material::TEXMAP_MAX;
    }
    if (texCoordNum != 0) {
        m_TexCoordAry.Reserve(texCoordNum);
    }

    m_IsTexCoordCached = false;
    for (int i = 0; i < 4; ++i) {
        m_VtxColors[i] = pResPic->vtxCols[i];
    }

    if (texCoordNum != 0 && m_TexCoordAry.m_Cap != 0) {
        m_TexCoordAry.Copy(pResPic + 1, texCoordNum);
    }

    const res::MaterialList* pMaterialList = resBlockSet.pMaterialList;
    const res::Material* pResMaterial =
        reinterpret_cast<const res::Material*>(reinterpret_cast<uptr>(pMaterialList) + pMaterialList->materialOffsets[pResPic->materialIdx]);
    m_pMaterial = Layout::NewObj<Material>(pResMaterial, resBlockSet);
}

// 0x004BBF38 slot 0x00 (name is ours)
// 0x004BBECC (deleting dtor)
nw::lyt::Picture::~Picture()
{
    if (m_pMaterial != nullptr && !m_pMaterial->IsUserAllocated()) {
        Layout::DeleteObj(m_pMaterial);
        m_pMaterial = nullptr;
    }
    m_TexCoordAry.Free();
}

// 0x0073E394 slot 0x08 (name is ours)
const PaneTypeInfo* nw::lyt::Picture::GetRuntimeTypeInfo() const
{
    return &s_TypeInfo;
}

// 0x0073E374 slot 0x0C
ut::Color8 nw::lyt::Picture::GetVtxColor(u32 idx) const
{
    return m_VtxColors[idx];
}

// 0x004BBB70 slot 0x10
void nw::lyt::Picture::SetVtxColor(u32 idx, ut::Color8 value)
{
    m_VtxColors[idx] = value;
}

// 0x0073E3A0 slot 0x1C
u8 nw::lyt::Picture::GetVtxColorElement(u32 idx) const
{
    return reinterpret_cast<const u8*>(&m_VtxColors[idx / 4])[idx % 4];
}

// 0x004BBB80 slot 0x20
void nw::lyt::Picture::SetVtxColorElement(u32 idx, u8 value)
{
    reinterpret_cast<u8*>(&m_VtxColors[idx / 4])[idx % 4] = value;
}

// 0x0073E384 slot 0x24
u8 nw::lyt::Picture::GetMaterialNum() const
{
    return m_pMaterial != nullptr ? 1 : 0;
}

// 0x0073E364 slot 0x28 | libgarden
Material* nw::lyt::Picture::GetMaterial(u32 idx)
{
    return idx == 0 ? m_pMaterial : nullptr;
}

// 0x004BBD08 slot 0x64 (name is ours)
void nw::lyt::Picture::DrawSelf(DrawInfo& drawInfo)
{
    if (m_pMaterial == nullptr) {
        return;
    }
    LoadMtx(drawInfo);
    m_pMaterial->SetupGL(drawInfo, m_GlbAlpha, true);
    internal::DrawQuad(drawInfo, GetVtxPos(), m_Size, m_TexCoordAry.GetSize(), m_TexCoordAry.GetArray(), m_VtxColors);
}

// 0x0073E3B8 slot 0x68 | nintendogs:callseq
u32* nw::lyt::Picture::MakeUniformDataSelf(u32* pCommand, DrawInfo& /*drawInfo*/, Drawer& drawer)
{
    pCommand = drawer.SetUpTexEnv(pCommand, m_pMaterial);
    pCommand = drawer.SetUpTextures(pCommand, m_pMaterial, true);
    if (!m_IsTexCoordCached || !m_pMaterial->IsCacheValid()) {
        m_TexCoordNum = drawer.CalcTextureCoords(m_pMaterial, m_TexCoordAry.GetArray(), m_TexCoords);
        m_IsTexCoordCached = true;
        m_pMaterial->SetCacheValid();
    }
    pCommand = drawer.SetUpTextureCoords(pCommand, m_TexCoords, m_TexCoordNum);
    drawer.SetUpMtx(m_GlbMtx);
    drawer.SetUpQuad(m_Size, GetVtxPos());
    drawer.SetUpVtxColors(m_VtxColors, m_GlbAlpha);
    return drawer.AddQuad(pCommand);
}

// 0x004BBBC8 slot 0x70 | libgarden
void nw::lyt::Picture::Append(const TexMap& texMap)
{
    if (m_pMaterial->GetTexMapNum() >= m_pMaterial->m_MemCap.texMap) {
        return;
    }
    if (m_pMaterial->GetTexCoordGenNum() >= m_pMaterial->m_MemCap.texCoordGen) {
        return;
    }

    const u8 texIdx = m_pMaterial->GetTexMapNum();
    m_pMaterial->SetTexMapNum(texIdx + 1);
    m_pMaterial->SetTexMap(texIdx, &texMap);
    m_pMaterial->SetTexCoordGenNum(m_pMaterial->GetTexMapNum());
    TexCoordGen& texCoordGen = m_pMaterial->GetTexCoordGenAry()[texIdx];
    texCoordGen.genType = 0;
    texCoordGen.texGenSrc = 0;
    m_pMaterial->SetCacheDirty();
    m_TexCoordAry.SetSize(m_pMaterial->GetTexMapNum());

    if (m_Size.width == 0.0f && m_Size.height == 0.0f && m_pMaterial->GetTexMapNum() == 1) {
        const TexSize texSize = m_pMaterial->GetTexMapAry()[0].m_Size;
        m_Size = Size(texSize.width, texSize.height);
    }
}

// 0x004BBB98 slot 0x74 (name is ours)
void nw::lyt::Picture::Append(const TextureInfo& textureInfo)
{
    Append(TexMap(textureInfo));
}

// 0x004BBB44 (name is ours)
void nw::lyt::Picture::SetTexCoord(u32 idx, const nn::math::VEC2* coords)
{
    m_TexCoordAry.SetCoord(idx, coords);
    if (m_pMaterial != nullptr) {
        m_pMaterial->SetCacheDirty();
    }
}

} // namespace lyt
} // namespace nw
