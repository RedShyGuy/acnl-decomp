#include "sead/seadPrimitiveDrawer.h"
#include "sead/seadCamera.h"
#include "sead/seadMathCalcCommon.h"
#include "sead/seadProjection.h"

namespace sead {
// 0x00982EE0
PrimitiveRendererBase* PrimitiveDrawer::s_pRenderer;

// 0x00546348 (name is ours)
void sead::PrimitiveDrawer::end()
{
    s_pRenderer->end();
}

// 0x005463B8 (name is ours)
void sead::PrimitiveDrawer::begin()
{
    s_pRenderer->begin(*mViewMtx, *mProjectionMtx);
}

// 0x005463DC (name is ours)
void sead::PrimitiveDrawer::QuadArg::setColorHorizontal(const Color4f& color0, const Color4f& color1)
{
    mHorizontal = true;
    mColor0 = color0;
    mColor1 = color1;
}

// 0x00546400 (name is ours)
void sead::PrimitiveDrawer::QuadArg::setColorVertical(const Color4f& color0, const Color4f& color1)
{
    mHorizontal = false;
    mColor0 = color0;
    mColor1 = color1;
}

// 0x00546424 (name is ours)
void sead::PrimitiveDrawer::drawQuad(const QuadArg& arg)
{
    Matrix34<f32> local;
    if (arg.mHorizontal) {
        // the colors from left to right: the quad turned by 90 degrees
        Vector3<f32> scale(arg.mSize.y, arg.mSize.x, 1.0f);
        Vector3<f32> rotate(0.0f, 0.0f, 3.14159265f / 2.0f);
        Matrix34CalcCtr<f32>::makeSRT(local, toVEC3(scale), toVEC3(rotate), toVEC3(arg.mCenter));
    } else {
        Vector3<f32> scale(arg.mSize.x, arg.mSize.y, 1.0f);
        Matrix34CalcCtr<f32>::makeST(local, toVEC3(scale), toVEC3(arg.mCenter));
    }
    Matrix34<f32> model;
    Matrix34CalcCtr<f32>::multiply(model, *mModelMtx, local);
    s_pRenderer->drawQuad(model, arg.mColor0, arg.mColor1);
}

// 0x005464EC (name is ours)
void sead::PrimitiveDrawer::setCamera(const Camera* camera)
{
    mViewMtx = &camera->mMatrix;
}

// 0x00546328 (name is ours)
void sead::PrimitiveDrawer::setProjection(const Projection* projection)
{
    mProjectionMtx = &projection->getDeviceProjectionMatrix();
}

// 0x005464F8 (name is ours)
sead::PrimitiveDrawer::PrimitiveDrawer()
    : mModelMtx(&Matrix34<f32>::ident), mViewMtx(&Matrix34<f32>::ident), mProjectionMtx(&Matrix44<f32>::ident)
{
}

// 0x00546528 slot 0x00
// 0x00546524 (deleting dtor)
sead::PrimitiveDrawer::~PrimitiveDrawer()
{
    // empty (in the original too)
}
} // namespace sead

// 0x00814484 (name is ours)
template void sead::Matrix34CalcCtr<f32>::makeST(nn::math::MTX34& o, const nn::math::VEC3& s, const nn::math::VEC3& t);
