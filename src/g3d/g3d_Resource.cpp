#include "g3d/g3d_System.h"
#include "g3d/dModel.h"
#include "g3d/dResourceLoader.h"
#include "nw/gfx/gfx_Model.h"
#include "nw/gfx/res/gfx_ResFragmentShader.h"
#include "nw/gfx/res/gfx_ResMaterial.h"
#include "nw/gfx/res/gfx_ResSkeleton.h"
#include "nw/ut/ut_FloatColor.h"
#include <cstring>
#include "nonameg3d/dCharTreeNode.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResLookupTable.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "nw/gfx/res/gfx_ResPrimitive.h"
#include "nw/gfx/res/gfx_ResPrimitiveSet.h"
#include "nw/gfx/res/gfx_ResShape.h"
#include "nw/gfx/res/gfx_ResTexture.h"
#include "nw/gfx/res/gfx_ResVertexAttribute.h"
#include "sead/seadTreeMapNode.h"

namespace g3d {
namespace {
// the length of a mesh node name FindMeshByNodeName compares
const u32 MESH_NODE_NAME_LENGTH = 64;
// a color component of 0..255 as a float
const f32 COLOR_COMPONENT_SCALE = 1.0f / 255.0f;

// the table set whose tables ResolveLookupTable resolves without resolveAll
const char* const TAKUMI_TABLE_SET = "Lut_Takumi";
} // namespace

// 0x004F1110 (name is ours)
void ResolveLookupTable(nw::gfx::res::ResLightingLookupTableData* sampler, bool resolveAll)
{
    if (sampler == NULL) {
        return;
    }
    nw::gfx::res::ResReferenceLookupTableData* table = sampler->sampler.To<nw::gfx::res::ResReferenceLookupTableData>();
    if (table->target.ToPtr() != NULL) {
        return;
    }
    if (!resolveAll && std::strcmp(TAKUMI_TABLE_SET, table->path.To<char>()) != 0) {
        return;
    }
    nonameg3d::Char key(table->tableName.To<char>());
    sead::TreeMapNode<nonameg3d::Char>* node = s_LookupTables.mRoot;
    while (node != NULL) {
        s32 cmp = key.Compare(node->mKey);
        if (cmp == 0) {
            table->target.Set(static_cast<nonameg3d::CharTreeNode*>(node)->m_pValue);
            return;
        }
        node = (cmp > 0) ? node->mRight : node->mLeft;
    }
}

// 0x004F1C10 (name is ours)
void SetupModelBuffers(const nw::gfx::res::ResModelData* model, u32 vertexArea, u32 indexArea)
{
    for (s32 i = 0; i < model->shapes.count; i++) {
        nw::gfx::res::ResSeparateDataShapeData* shape = static_cast<nw::gfx::res::ResSeparateDataShapeData*>(model->shapes.GetData(i));
        if (shape->typeInfo == nw::gfx::res::ResSeparateDataShapeData::TYPE) {
            for (s32 j = 0; j < shape->vertexAttributes.count; j++) {
                nw::gfx::res::ResVertexStreamBaseData* attribute =
                    static_cast<nw::gfx::res::ResVertexStreamBaseData*>(shape->vertexAttributes.GetData(j));
                if (!(attribute->flags & nw::gfx::res::ResVertexAttributeData::FLAG_CONSTANT)) {
                    attribute->locationFlag = vertexArea | LOCATION_FLAG;
                }
            }
        }
        SetupShapeBuffers(shape, indexArea);
    }
}

// 0x004F23F4 (name is ours)
void SetupShapeBuffers(const nw::gfx::res::ResSeparateDataShapeData* shape, u32 area)
{
    u32 locationFlag = area | LOCATION_FLAG;
    for (s32 i = 0; i < shape->primitiveSets.count; i++) {
        nw::gfx::res::ResPrimitiveSetData* primitiveSet =
            static_cast<nw::gfx::res::ResPrimitiveSetData*>(shape->primitiveSets.GetData(i));
        nw::ut::Offset* primitives = primitiveSet->primitives.array.To<nw::ut::Offset>();
        nw::ut::Offset* primitivesEnd = primitives + primitiveSet->primitives.count;
        for (; primitives != primitivesEnd; primitives++) {
            nw::gfx::res::ResPrimitiveData* primitive = primitives->To<nw::gfx::res::ResPrimitiveData>();
            for (u32 j = 0; j < static_cast<u32>(primitive->indexStreams.count); j++) {
                static_cast<nw::gfx::res::ResIndexStreamData*>(primitive->indexStreams.GetData(j))->locationFlag = locationFlag;
            }
        }
    }
}

// 0x004F2894 (name is ours)
void SetupTexture(const nw::gfx::res::ResTextureData* texture, u32 area)
{
    while (true) {
        switch (texture->typeInfo) {
        case nw::gfx::res::ResTexture::TYPE_REFERENCE: {
            const nw::gfx::res::ResReferenceTextureData* reference = static_cast<const nw::gfx::res::ResReferenceTextureData*>(texture);
            texture = reference->target.To<nw::gfx::res::ResTextureData>();
            if (texture == NULL) {
                return;
            }
            continue;
        }
        case nw::gfx::res::ResTexture::TYPE_CUBE:
            break;
        case nw::gfx::res::ResTexture::TYPE_IMAGE:
            texture = nw::gfx::res::DynamicCastData<nw::gfx::res::ResTextureData>(texture, nw::gfx::res::ResTexture::TYPE_IMAGE);
            break;
        default:
            return;
        }
        const_cast<nw::gfx::res::ResPixelBasedTextureData*>(static_cast<const nw::gfx::res::ResPixelBasedTextureData*>(texture))
            ->locationFlag = area | LOCATION_FLAG;
        return;
    }
}

// 0x004F2918 (name is ours)
void CopyTextureImage(nw::gfx::res::ResImageTextureData* dst, const nw::gfx::res::ResImageTextureData* src)
{
    dst = const_cast<nw::gfx::res::ResImageTextureData*>(
        nw::gfx::res::DynamicCastData<nw::gfx::res::ResImageTextureData>(dst, nw::gfx::res::ResTexture::TYPE_IMAGE));
    src = nw::gfx::res::DynamicCastData<nw::gfx::res::ResImageTextureData>(src, nw::gfx::res::ResTexture::TYPE_IMAGE);
    nw::gfx::res::ResImageData* dstImage = dst->image.To<nw::gfx::res::ResImageData>();
    const nw::gfx::res::ResImageData* srcImage = src->image.To<nw::gfx::res::ResImageData>();
    dstImage->data.Set(srcImage->data.ToPtr());
    dst->image.To<nw::gfx::res::ResImageData>()->width = src->image.To<nw::gfx::res::ResImageData>()->width;
    dst->width = src->width;
    dst->image.To<nw::gfx::res::ResImageData>()->height = src->image.To<nw::gfx::res::ResImageData>()->height;
    dst->height = src->height;
    dst->format = src->format;
    dst->glType = src->glType;
    dst->mipmapLevels = src->mipmapLevels;
}

// 0x004F2A20 (name is ours)
void SetTextureImage(nw::gfx::res::ResImageTextureData* texture, const void* data, u32 width, u32 height)
{
    texture = const_cast<nw::gfx::res::ResImageTextureData*>(
        nw::gfx::res::DynamicCastData<nw::gfx::res::ResImageTextureData>(texture, nw::gfx::res::ResTexture::TYPE_IMAGE));
    texture->image.To<nw::gfx::res::ResImageData>()->data.Set(data);
    if (width != 0) {
        texture->image.To<nw::gfx::res::ResImageData>()->width = width;
        texture->width = width;
    }
    if (height != 0) {
        texture->image.To<nw::gfx::res::ResImageData>()->height = height;
        texture->height = height;
    }
    if (width != 0 || height != 0) {
        texture->image.To<nw::gfx::res::ResImageData>()->size = 0;
    }
}

// 0x004F1B38 (name is ours)
bool GetBoneTranslate(nn::math::VEC3* translate, const nw::gfx::res::ResSkeletalModelData* model, s32 boneIdx)
{
    const nw::gfx::res::ResSkeletonData* skeleton = static_cast<const nw::gfx::res::ResSkeletonData*>(model->skeleton.ToPtr());
    nw::ut::ResDicPatricia bones = {skeleton->boneDic.To<nw::ut::ResDicPatricia::Data>()};
    const nw::gfx::res::ResBoneData* bone = static_cast<const nw::gfx::res::ResBoneData*>(bones.GetDataByIndex(boneIdx));
    translate->x = bone->transform.translate.x;
    translate->y = bone->transform.translate.y;
    translate->z = bone->transform.translate.z;
    return true;
}

// 0x004F1B98 (name is ours)
s32 GetBoneIndex(const nw::gfx::res::ResSkeletalModelData* model, const char* boneName)
{
    const nw::gfx::res::ResSkeletonData* skeleton = static_cast<const nw::gfx::res::ResSkeletonData*>(model->skeleton.ToPtr());
    nw::ut::ResDicPatricia bones = {skeleton->boneDic.To<nw::ut::ResDicPatricia::Data>()};
    return bones.GetIndex(boneName);
}

// 0x004F1CD0 (name is ours)
bool SetConstantColor(const nw::gfx::res::ResModelData* model, const char* materialName, s32 colorIdx, s32 r, s32 g, s32 b, s32 a)
{
    nw::gfx::res::ResMaterialData* material = static_cast<nw::gfx::res::ResMaterialData*>(model->materials.GetData(materialName));
    if (material == NULL) {
        return false;
    }
    return SetConstantColor(material, colorIdx, r, g, b, a);
}

// 0x004F1E08 (name is ours)
void GetTextureTranslate(nn::math::VEC2* translate, const nw::gfx::res::ResModelData* model, const char* materialName, s32 coordinatorIdx)
{
    if (translate == NULL || model == NULL) {
        return;
    }
    const nw::gfx::res::ResMaterialData* material =
        static_cast<const nw::gfx::res::ResMaterialData*>(model->materials.GetData(materialName));
    if (material == NULL) {
        return;
    }
    translate->x = material->textureCoordinators[coordinatorIdx].translate.x;
    translate->y = material->textureCoordinators[coordinatorIdx].translate.y;
}

// 0x004F1EB0 (name is ours)
nw::gfx::res::ResMeshData* FindMeshByNodeName(const nw::gfx::res::ResModelData* model, const char* meshNodeName)
{
    nw::gfx::res::ResMeshData* found = NULL;
    if (model == NULL || meshNodeName == NULL) {
        return found;
    }
    const nw::ut::Offset* meshes = model->meshes.array.To<nw::ut::Offset>();
    const u32 meshCount = (meshes + model->meshes.count) - meshes;
    for (u32 i = 0; i < meshCount; i++) {
        const nw::gfx::res::ResMeshData* mesh = static_cast<const nw::gfx::res::ResMeshData*>(meshes[i].ToPtr());
        if (mesh->meshNodeVisibilityIndex < 0) {
            continue;
        }
        const nw::gfx::res::ResMeshNodeVisibilityData* visibility =
            static_cast<const nw::gfx::res::ResMeshNodeVisibilityData*>(model->meshNodeVisibilities.GetData(mesh->meshNodeVisibilityIndex));
        if (visibility == NULL) {
            continue;
        }
        if (std::strncmp(meshNodeName, visibility->name.To<char>(), MESH_NODE_NAME_LENGTH) == 0) {
            found = static_cast<nw::gfx::res::ResMeshData*>(const_cast<void*>(meshes[i].ToPtr()));
        }
    }
    return found;
}

// 0x004F1FA0 (name is ours)
void SetAmbientAndDiffuse(const nw::gfx::res::ResModelData* model, const nw::ut::FloatColor& color)
{
    for (s32 i = 0; i < model->materials.count; i++) {
        nw::gfx::res::ResMaterialData* material = static_cast<nw::gfx::res::ResMaterialData*>(model->materials.GetData(i));
        nw::gfx::res::ResMaterialColorData* colors = &material->materialColor;
        if (colors == NULL) {
            continue;
        }
        colors->ambient.r = color.r;
        colors->ambient.g = color.g;
        colors->ambient.b = color.b;
        colors->ambientU8 = colors->ambient.ToPicaU32();
        colors->diffuse.r = color.r;
        colors->diffuse.g = color.g;
        colors->diffuse.b = color.b;
        colors->diffuse.a = 1.0f;
        colors->diffuseU8 = colors->diffuse.ToPicaU32();
        material->materialColorHash = 0;
    }
}

// 0x004F21EC (name is ours)
void SetAmbientAndDiffuse(const nw::gfx::res::ResModelData* model, f32 value)
{
    nw::ut::FloatColor color;
    color.r = value;
    color.g = value;
    color.b = value;
    color.a = value;
    SetAmbientAndDiffuse(model, color);
}

// 0x004F2214 (name is ours)
void SetEmission(const nw::gfx::res::ResModelData* model, const nw::ut::FloatColor& color)
{
    for (s32 i = 0; i < model->materials.count; i++) {
        nw::gfx::res::ResMaterialData* material = static_cast<nw::gfx::res::ResMaterialData*>(model->materials.GetData(i));
        nw::gfx::res::ResMaterialColorData* colors = &material->materialColor;
        if (colors == NULL) {
            continue;
        }
        colors->emission.r = color.r;
        colors->emission.g = color.g;
        colors->emission.b = color.b;
        colors->emission.a = 1.0f;
        colors->emissionU8 = colors->emission.ToPicaU32();
        material->materialColorHash = 0;
    }
}

// 0x004F2364 (name is ours)
void SetEmission(const nw::gfx::res::ResModelData* model, f32 value)
{
    nw::ut::FloatColor color;
    color.r = value;
    color.g = value;
    color.b = value;
    color.a = value;
    SetEmission(model, color);
}

// 0x004F238C (name is ours)
void EnableFog(const nw::gfx::res::ResModelData* model)
{
    for (s32 i = 0; i < model->materials.count; i++) {
        nw::gfx::res::ResMaterialData* material = static_cast<nw::gfx::res::ResMaterialData*>(model->materials.GetData(i));
        material->flags |= nw::gfx::res::ResMaterialData::FLAG_FOG;
    }
}

// 0x004F3108 (name is ours)
bool SetCombinerBufferColor(nw::gfx::res::ResMaterialData* material, s32 r, s32 g, s32 b, s32 a)
{
    if (material == NULL) {
        return false;
    }
    nw::gfx::res::ResFragmentShaderData* fragmentShader = material->fragmentShader.To<nw::gfx::res::ResFragmentShaderData>();
    if (fragmentShader == NULL) {
        return false;
    }
    nw::ut::FloatColor color = fragmentShader->bufferColor;
    if (r >= 0) {
        color.r = r * COLOR_COMPONENT_SCALE;
    }
    if (g >= 0) {
        color.g = g * COLOR_COMPONENT_SCALE;
    }
    if (b >= 0) {
        color.b = b * COLOR_COMPONENT_SCALE;
    }
    if (a >= 0) {
        color.a = a * COLOR_COMPONENT_SCALE;
    }
    fragmentShader->bufferColor = color;
    fragmentShader->bufferColorCommand[0] = fragmentShader->bufferColor.ToPicaU32();
    material->textureCombinersHash = 0;
    return true;
}

// 0x004F31C4 (name is ours)
void GetDiffuse(nw::ut::FloatColor* color, const nw::gfx::res::ResMaterialData* material)
{
    if (material == NULL) {
        return;
    }
    const nw::gfx::res::ResMaterialColorData* colors = &material->materialColor;
    if (colors == NULL) {
        return;
    }
    *color = colors->diffuse;
}

// 0x004F31E0 (name is ours)
void SetDiffuse(nw::gfx::res::ResMaterialData* material, const nw::ut::FloatColor& color)
{
    if (material == NULL) {
        return;
    }
    nw::gfx::res::ResMaterialColorData* colors = &material->materialColor;
    if (colors == NULL) {
        return;
    }
    colors->diffuse = color;
    colors->diffuseU8 = colors->diffuse.ToPicaU32();
    material->materialColorHash = 0;
}

// 0x004F3214 (name is ours)
void GetConstantColor(nw::ut::FloatColor* color, const nw::gfx::res::ResMaterialData* material, s32 colorIdx)
{
    switch (colorIdx) {
    case 0:
        *color = material->materialColor.constant[0];
        break;
    case 1:
        *color = material->materialColor.constant[1];
        break;
    case 2:
        *color = material->materialColor.constant[2];
        break;
    case 3:
        *color = material->materialColor.constant[3];
        break;
    case 4:
        *color = material->materialColor.constant[4];
        break;
    case 5:
        *color = material->materialColor.constant[5];
        break;
    }
}

// 0x004F3298 (name is ours)
void SetConstantColor(nw::gfx::res::ResMaterialData* material, const nw::ut::FloatColor& color, s32 colorIdx)
{
    nw::gfx::res::ResMaterialColorData& colors = material->materialColor;
    switch (colorIdx) {
    case 0:
        colors.constant[0] = color;
        colors.constantU8[0] = colors.constant[0].ToPicaU32();
        break;
    case 1:
        colors.constant[1] = color;
        colors.constantU8[1] = colors.constant[1].ToPicaU32();
        break;
    case 2:
        colors.constant[2] = color;
        colors.constantU8[2] = colors.constant[2].ToPicaU32();
        break;
    case 3:
        colors.constant[3] = color;
        colors.constantU8[3] = colors.constant[3].ToPicaU32();
        break;
    case 4:
        colors.constant[4] = color;
        colors.constantU8[4] = colors.constant[4].ToPicaU32();
        break;
    case 5:
        colors.constant[5] = color;
        colors.constantU8[5] = colors.constant[5].ToPicaU32();
        break;
    }
    material->textureCombinersHash = 0;
}

// 0x004F33D0 (name is ours)
bool SetConstantColor(nw::gfx::res::ResMaterialData* material, s32 colorIdx, s32 r, s32 g, s32 b, s32 a)
{
    if (material == NULL) {
        return false;
    }
    nw::ut::FloatColor color;
    GetConstantColor(&color, material, colorIdx);
    if (static_cast<u32>(r) < 256) {
        color.r = r * COLOR_COMPONENT_SCALE;
    }
    if (static_cast<u32>(g) < 256) {
        color.g = g * COLOR_COMPONENT_SCALE;
    }
    if (static_cast<u32>(b) < 256) {
        color.b = b * COLOR_COMPONENT_SCALE;
    }
    if (static_cast<u32>(a) < 256) {
        color.a = a * COLOR_COMPONENT_SCALE;
    }
    SetConstantColor(material, color, colorIdx);
    return true;
}

// 0x004F34A0 (name is ours)
bool ReplaceTexture(Model* model, ResourceLoader* loader, nw::os::IAllocator* allocator, const char* materialName, const char* textureName)
{
    loader->InitializeCgfx(allocator, NULL, true);
    nw::gfx::res::ResTexture texture(loader->GetGraphicsFile().ptr()->textures.GetData(0));
    const nw::gfx::res::ResModelData* resModel = model->GetModel()->GetResModel().ptr();
    nw::gfx::res::ResMaterial material(resModel->materials.GetData(materialName));
    return SetTexture(material, textureName, texture);
}

// 0x004F3578 (name is ours)
bool SetTexture(nw::gfx::res::ResMaterial material, const char* textureName, nw::gfx::res::ResTexture texture)
{
    if (!material.IsValid()) {
        return false;
    }
    material.ForceSetupTexture(textureName, texture);
    return true;
}

// 0x004F359C (name is ours)
void GetTextureTranslate(nn::math::VEC2* translate, const nw::gfx::res::ResMaterialData* material, s32 coordinatorIdx)
{
    if (translate == NULL || material == NULL) {
        return;
    }
    translate->x = material->textureCoordinators[coordinatorIdx].translate.x;
    translate->y = material->textureCoordinators[coordinatorIdx].translate.y;
}

// 0x004F35D0 (name is ours)
f32 GetPolygonOffsetUnits(const nw::gfx::res::ResMaterialData* material)
{
    if (material == NULL) {
        return 0.0f;
    }
    return material->polygonOffsetUnits;
}

// 0x004F35E8 (name is ours)
f32 GetEnabledPolygonOffsetUnits(const nw::gfx::res::ResMaterialData* material)
{
    f32 units = 0.0f;
    if (material == NULL) {
        return units;
    }
    if (material->rasterizationFlags & nw::gfx::res::ResMaterialData::RASTERIZATION_POLYGON_OFFSET) {
        units = material->polygonOffsetUnits;
    }
    return units;
}

// 0x00317EE8 (name is ours)
void SetLightSetIndex(const nw::gfx::res::ResModelData* model, s32 lightSetIndex)
{
    for (s32 i = 0; i < model->materials.count; i++) {
        nw::gfx::res::ResMaterialData* material = static_cast<nw::gfx::res::ResMaterialData*>(model->materials.GetData(i));
        if (material == NULL || lightSetIndex < 0 || material->lightSetIndex == lightSetIndex) {
            continue;
        }
        material->lightSetIndex = lightSetIndex;
        material->fragmentLightingHash = 0;
        material->fragmentLightingTableHash = 0;
        material->fragmentLightingTableHashSeed = 0;
    }
}

} // namespace g3d
