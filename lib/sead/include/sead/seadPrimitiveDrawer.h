#pragma once

#include "decomp.h"
#include "sead/seadColor4f.h"
#include "sead/seadMatrix.h"
#include "sead/seadVector2.h"
#include "sead/seadVector3.h"

namespace sead {
class Camera;
class Projection;

// The renderer behind the PrimitiveDrawer (only its slots are in the binary; the instance pointer is
// set from outside sead). The class and slot names are ours.
class PrimitiveRendererBase
{
public:
    virtual ~PrimitiveRendererBase() {}
    virtual void begin(const Matrix34<f32>& view, const Matrix44<f32>& projection) = 0; // slot 0x08
    virtual void end() = 0; // slot 0x0C
    // a unit quad in the xy plane around the origin, the colors from top to bottom
    virtual void drawQuad(const Matrix34<f32>& model, const Color4f& color0, const Color4f& color1) = 0; // slot 0x10
};

// RTTI N4sead15PrimitiveDrawerE @ 0x008D18A8
// vtable 0x00905B24 (vptr 0x00905B2C), offset_to_top 0, 2 entries
//
// Draws primitives with the matrices of a camera and a projection (names are ours).
class PrimitiveDrawer
{
public:
    // a quad: the center, the size and the colors of its two sides
    class QuadArg
    {
    public:
        // (the original copies x and y of Vector3::ones for the size)
        QuadArg()
            : mCenter(Vector3<f32>::zero), mSize(Vector3<f32>::ones.x, Vector3<f32>::ones.y), mColor0(Color4f::white),
              mColor1(Color4f::white), mHorizontal(false)
        {
        }

        // color0 left, color1 right
        void setColorHorizontal(const Color4f& color0, const Color4f& color1); // 0x005463DC
        // color0 top, color1 bottom
        void setColorVertical(const Color4f& color0, const Color4f& color1); // 0x00546400

        Vector3<f32> mCenter; // 0x00
        Vector2<f32> mSize;   // 0x0C
        Color4f mColor0;      // 0x14
        Color4f mColor1;      // 0x24
        bool mHorizontal;     // 0x34
    };

    // the identities
    PrimitiveDrawer(); // 0x005464F8
    virtual ~PrimitiveDrawer(); // 0x00546528 slot 0x00
    // 0x00546524 slot 0x04 (deleting dtor)

    void setCamera(const Camera* camera); // 0x005464EC
    void setProjection(const Projection* projection); // 0x00546328
    void begin(); // 0x005463B8
    void end(); // 0x00546348
    void drawQuad(const QuadArg& arg); // 0x00546424

    static PrimitiveRendererBase* s_pRenderer;

    const Matrix34<f32>* mModelMtx;      // 0x04
    const Matrix34<f32>* mViewMtx;       // 0x08
    const Matrix44<f32>* mProjectionMtx; // 0x0C
};
ASSERT_SIZE(PrimitiveDrawer, 0x10);
} // namespace sead
