#include "cfl/cfl_Database.h"
#include "cfl/cfl_Model.h"
#include "nn/math/math_Api.h"
#include "nn/math/math_MTX34.h"

#include <cmath>

extern "C" {
// a rotation about the axis 'x', 'y' or 'z'
// 0x00153248 | nintendogs:bytes [tier B]
DECOMP_NOINLINE void CFL_GetRotateMatrixRadian(nn::math::MTX34* mtx, char axis, f32 radian)
{
    f32 c = cosf(radian);
    f32 s = sinf(radian);
    switch (axis) {
    case 'x':
        mtx->m[2][1] = s;
        mtx->m[1][1] = c;
        mtx->m[2][2] = c;
        mtx->m[0][0] = 1.0f;
        mtx->m[1][2] = -s;
        mtx->m[0][1] = 0.0f;
        mtx->m[0][2] = 0.0f;
        mtx->m[0][3] = 0.0f;
        mtx->m[1][0] = 0.0f;
        mtx->m[1][3] = 0.0f;
        mtx->m[2][0] = 0.0f;
        mtx->m[2][3] = 0.0f;
        break;
    case 'y':
        mtx->m[0][0] = c;
        mtx->m[2][2] = c;
        mtx->m[2][0] = -s;
        mtx->m[0][1] = 0.0f;
        mtx->m[0][2] = s;
        mtx->m[0][3] = 0.0f;
        mtx->m[1][0] = 0.0f;
        mtx->m[1][1] = 1.0f;
        mtx->m[1][2] = 0.0f;
        mtx->m[1][3] = 0.0f;
        mtx->m[2][1] = 0.0f;
        mtx->m[2][3] = 0.0f;
        break;
    case 'z':
        mtx->m[1][0] = s;
        mtx->m[0][0] = c;
        mtx->m[1][1] = c;
        mtx->m[0][1] = -s;
        mtx->m[0][2] = 0.0f;
        mtx->m[0][3] = 0.0f;
        mtx->m[1][2] = 0.0f;
        mtx->m[1][3] = 0.0f;
        mtx->m[2][0] = 0.0f;
        mtx->m[2][1] = 0.0f;
        mtx->m[2][2] = 1.0f;
        mtx->m[2][3] = 0.0f;
        break;
    }
}

// 0x00153340 | nintendogs:bytes [tier B]
DECOMP_NOINLINE void CFL_GetScaleMatrix(nn::math::MTX34* mtx, f32 x, f32 y, f32 z)
{
    mtx->m[0][0] = x;
    mtx->m[1][1] = y;
    mtx->m[0][1] = 0.0f;
    mtx->m[0][2] = 0.0f;
    mtx->m[0][3] = 0.0f;
    mtx->m[1][0] = 0.0f;
    mtx->m[1][2] = 0.0f;
    mtx->m[1][3] = 0.0f;
    mtx->m[2][0] = 0.0f;
    mtx->m[2][1] = 0.0f;
    mtx->m[2][2] = z;
    mtx->m[2][3] = 0.0f;
}

// the number of expressions in a mask (bits 0-18)
// 0x00154D84 | nintendogs:callgraph [tier C]
DECOMP_NOINLINE int CFLi_GetExpressionCount(u32 mask)
{
    int count = 0;
    for (int i = 0; i < CFL_EXPRESSION_MAX; i++) {
        if (mask & (1 << i)) {
            count++;
        }
    }
    return count;
}

// 0x0015304C | nintendogs:bytes [tier B]
u32 CFL_GetModelBufferSize(u32 expressionMask)
{
    return ((CFLi_GetExpressionCount(expressionMask) * sizeof(CFLiTexHandle) + 31) & ~31) + sizeof(CFLiCharModelData);
}

// the matrices of position and normal, in units of 1/256
// 0x00153F14 | nintendogs:bytes [tier B]
void CFL_SetMtxPosNrm(CFLCharModel* model, const nn::math::MTX34* pos, const nn::math::MTX34* nrm)
{
    nn::math::ARMv6::MTX34CopyAsm(&model->posMtx, pos);
    nn::math::ARMv6::MTX34CopyAsm(&model->nrmMtx, nrm);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            model->posMtx.m[i][j] *= 1.0f / 256.0f;
            model->nrmMtx.m[i][j] *= 1.0f / 256.0f;
        }
    }
}

// 0x00153EB8 | nintendogs:bytes [tier B]
void CFL_SetExpression(CFLCharModel* model, int expression)
{
    if (model == NULL) {
        return;
    }
    CFLiTexHandle* texture = model->expressionTextures[expression];
    if (texture == NULL || model->expression == expression) {
        return;
    }
    CFLiCharModelData* data = model->data;
    if (data->expressionCommand != NULL) {
        *data->expressionCommand = nngxGetPhysicalAddr(texture->address) >> 3;
    }
    model->expression = expression;
}
} // extern "C"

// a translation (name is ours)
// 0x00153424 (name is ours)
DECOMP_NOINLINE void CFL_GetTranslateMatrix(nn::math::MTX34* mtx, f32 x, f32 y, f32 z)
{
    mtx->m[0][3] = x;
    mtx->m[1][3] = y;
    mtx->m[2][3] = z;
    mtx->m[0][0] = 1.0f;
    mtx->m[0][1] = 0.0f;
    mtx->m[0][2] = 0.0f;
    mtx->m[1][0] = 0.0f;
    mtx->m[1][1] = 1.0f;
    mtx->m[1][2] = 0.0f;
    mtx->m[2][0] = 0.0f;
    mtx->m[2][1] = 0.0f;
    mtx->m[2][2] = 1.0f;
}

namespace {
// the order of the pixels in an 8x8 tile of the GPU (x, y; name is ours)
// 0x008A3D84
const s32 s_TileOrder[64][2] = {
    {0, 0}, {1, 0}, {0, 1}, {1, 1}, {2, 0}, {3, 0}, {2, 1}, {3, 1},
    {0, 2}, {1, 2}, {0, 3}, {1, 3}, {2, 2}, {3, 2}, {2, 3}, {3, 3},
    {4, 0}, {5, 0}, {4, 1}, {5, 1}, {6, 0}, {7, 0}, {6, 1}, {7, 1},
    {4, 2}, {5, 2}, {4, 3}, {5, 3}, {6, 2}, {7, 2}, {6, 3}, {7, 3},
    {0, 4}, {1, 4}, {0, 5}, {1, 5}, {2, 4}, {3, 4}, {2, 5}, {3, 5},
    {0, 6}, {1, 6}, {0, 7}, {1, 7}, {2, 6}, {3, 6}, {2, 7}, {3, 7},
    {4, 4}, {5, 4}, {4, 5}, {5, 5}, {6, 4}, {7, 4}, {6, 5}, {7, 5},
    {4, 6}, {5, 6}, {4, 7}, {5, 7}, {6, 6}, {7, 6}, {6, 7}, {7, 7},
};
} // namespace

// copies a tiled image (two bytes per pixel) into a linear one, bottom row first (name is ours)
// 0x001765E4 (name is ours)
DECOMP_NOINLINE void CFLi_UntileImage(u8* dst, const u8* src, int width, int height, int, int format)
{
    int offsets[64];
    int bytesPerPixel = format == 11 ? 4 : 2;
    for (int i = 0; i < 64; i++) {
        offsets[i] = (s_TileOrder[i][0] + (7 - s_TileOrder[i][1]) * width) * bytesPerPixel;
    }
    for (int y = 0; y < height; y += 8) {
        int tiles = (width + 7) / 8;
        int rowOffset = (height - y - 8) * width * bytesPerPixel;
        const u8* tile = src;
        for (int x = 0; x < tiles; x++) {
            int offset = bytesPerPixel * 8 * x + rowOffset;
            for (int i = 0; i < 64; i++) {
                u8* p = dst + offsets[i] + offset;
                p[0] = tile[i * 2];
                p[1] = tile[i * 2 + 1];
            }
            tile += 128;
        }
        src += tiles << 7;
    }
}

// loads a texture of the resource archive; returns the buffer of the image (the texture may use it in place)
// 0x0013DE14 (name is ours)
DECOMP_NOINLINE void* CFLi_LoadArchiveTexture(CFLiTexHandle* texture, int section, int index, unsigned area)
{
    void* image;
    s32 size = CFLi_ReadArchiveImage(NULL, section, index);
    if (size <= 0) {
        image = NULL;
    } else {
        image = CFLi_AllocateMemory(size, 32);
        CFLi_ReadArchiveImage(image, section, index);
    }
    texture->CreateFromRes(static_cast<const CFLiTexHeader*>(image), area);
    if (area != 0x10000) {
        nngxSplitDrawCmdlist();
        glGetError();
        nngxRunCmdlist();
        nngxWaitCmdlistDone();
        nngxClearCmdlist();
        glGetError();
    }
    return image;
}

extern "C" {
// releases the command lists, textures and buffers of a model (name is ours, after the order of the functions)
// 0x00152D98 (name is ours)
void CFL_DeleteCharModel(CFLCharModel* model)
{
    CFLiCharModelData* data = model->data;
    if (data->opa.name != 0) {
        nngxDeleteCmdlists(1, &data->opa.name);
        data->opa.name = 0;
    }
    if (data->xlu.name != 0) {
        nngxDeleteCmdlists(1, &data->xlu.name);
        data->xlu.name = 0;
    }
    data->textures[0].Delete();
    data->textures[3].Delete();
    data->textures[2].Delete();
    data->textures[1].Delete();
    for (int i = CFL_SHAPE_NUM - 1; i >= 0; i--) {
        CFLiResShape* shape = &data->shapes[i];
        if (shape->primitiveNum == 0) {
            continue;
        }
        CFLiVertexBuffer* buffer = &shape->vertices;
        if (buffer->name != 0) {
            if (buffer->allocated) {
                GxFreeFunction freeFunction;
                nngxGetAllocator(NULL, &freeFunction);
                freeFunction(buffer->area, 0x102, buffer->name, buffer->address);
            }
            buffer->name = 0;
            buffer->allocated = false;
        }
        buffer = &shape->indices;
        if (buffer->name != 0) {
            if (buffer->allocated) {
                GxFreeFunction freeFunction;
                nngxGetAllocator(NULL, &freeFunction);
                freeFunction(buffer->area, 0x102, buffer->name, buffer->address);
            }
            buffer->name = 0;
            buffer->allocated = false;
        }
        shape->primitiveNum = 0;
    }
    for (int i = CFL_EXPRESSION_MAX - 1; i >= 0; i--) {
        CFLiTexHandle* texture = model->expressionTextures[i];
        if (texture == NULL || texture->self == NULL) {
            continue;
        }
        if (texture->allocated) {
            GxFreeFunction freeFunction;
            nngxGetAllocator(NULL, &freeFunction);
            freeFunction(texture->area, 0x101, reinterpret_cast<u32>(texture->self),
                         reinterpret_cast<void*>(texture->address));
        }
        texture->self = NULL;
        texture->width = 0;
        texture->height = 0;
        texture->allocated = false;
    }
}

// the projection and the view of an icon of the size
// 0x00154DD0 | nintendogs:callseq [confirmed by mk7dlp] [tier A]
void CFLi_GetIconMtx(nn::math::MTX44* projection, nn::math::MTX34* view, int width, int height)
{
    f32 aspect = static_cast<f32>(width) / static_cast<f32>(height);
    f32 size = 43.2f;
    if (width < height) {
        size = 43.2f / aspect;
    }
    f32 fovy = atan2f(size, 500.0f) * 360.0f * 0.15915494f * 2.0f;
    f32 sin;
    f32 cos;
    nn::math::SinCosFIdx(&sin, &cos, fovy * 2.0f * 3.1415927f * (1.0f / 360.0f) * 0.5f * (256.0f / (2.0f * 3.1415927f)));
    f32 cot = cos / sin;
    projection->m[0][1] = 0.0f;
    projection->m[0][2] = 0.0f;
    projection->m[0][3] = 0.0f;
    projection->m[1][0] = 0.0f;
    projection->m[1][2] = 0.0f;
    projection->m[1][3] = 0.0f;
    projection->m[2][0] = 0.0f;
    projection->m[2][1] = 0.0f;
    projection->m[3][0] = 0.0f;
    projection->m[3][1] = 0.0f;
    projection->m[3][2] = -1.0f;
    projection->m[3][3] = 0.0f;
    projection->m[0][0] = cot / aspect;
    projection->m[1][1] = cot;
    projection->m[2][2] = 3.5f;
    projection->m[2][3] = 1750.0f;

    f32 eye[3] = {0.0f, 34.5f, 600.0f};
    f32 up[3] = {0.0f, 1.0f, 0.0f};
    f32 target[3] = {0.0f, 34.5f, 0.0f};
    CFLi_ApplyAxis(eye);
    CFLi_ApplyAxis(up);
    CFLi_ApplyAxis(target);
    f32 zx = eye[0] - target[0];
    f32 zy = eye[1] - target[1];
    f32 zz = eye[2] - target[2];
    f32 inv = 1.0f / sqrtf(zx * zx + zy * zy + zz * zz);
    zz *= inv;
    zx *= inv;
    zy *= inv;
    f32 xx = up[1] * zz - up[2] * zy;
    f32 xy = up[2] * zx - up[0] * zz;
    f32 xz = up[0] * zy - up[1] * zx;
    inv = 1.0f / sqrtf(xx * xx + xy * xy + xz * xz);
    xz *= inv;
    xx *= inv;
    xy *= inv;
    f32 yx = zy * xz - zz * xy;
    f32 yy = zz * xx - zx * xz;
    f32 yz = zx * xy - zy * xx;
    view->m[0][0] = xx;
    view->m[0][1] = xy;
    view->m[0][2] = xz;
    view->m[1][0] = yx;
    view->m[1][1] = yy;
    view->m[1][2] = yz;
    view->m[2][2] = zz;
    view->m[0][3] = -(eye[0] * xx + eye[1] * xy + eye[2] * xz);
    view->m[2][3] = -(eye[0] * zx + eye[1] * zy + eye[2] * zz);
    view->m[1][3] = -(eye[0] * yx + eye[1] * yy + eye[2] * yz);
    view->m[2][0] = zx;
    view->m[2][1] = zy;
}
} // extern "C"

extern "C" {
// the opaque parts of the model
// 0x00127D44 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
void CFL_DrawOpa(CFLCharModel* model)
{
    CFLi_SetModelMatrices(&model->posMtx, &model->nrmMtx, true);
    CFLiSavedCmdlist* list = &model->data->opa;
    nngxUseSavedCmdlist(list->name, 0, list->size, list->requestOffset, list->requestCount, 0, list->_C);
    glGetError();
}

// the translucent parts of the model
// 0x00152F74 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
void CFL_DrawXlu(CFLCharModel* model)
{
    CFLi_SetModelMatrices(&model->posMtx, &model->nrmMtx, true);
    CFLiSavedCmdlist* list = &model->data->xlu;
    nngxUseSavedCmdlist(list->name, 0, list->size, list->requestOffset, list->requestCount, 0, list->_C);
    glGetError();
    glGetError();
}
} // extern "C"
