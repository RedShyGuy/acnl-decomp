#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadDebugMenuSkinBase.h"
#include "sead/seadOrthoCamera.h"
#include "sead/seadOrthoProjection.h"
#include "sead/seadPrimitiveDrawer.h"
#include "sead/seadTextWriter.h"

namespace sead {
// RTTI N4sead20DebugMenuSkinDefaultE @ 0x008D1E40
// vtable 0x00906430 (vptr 0x00906438), offset_to_top 0, 20 entries
// The look of the debug menu with quads and the text writer (member names are ours).
class DebugMenuSkinDefault : public ::sead::DebugMenuSkinBase
{
public:
    DebugMenuSkinDefault()
        : DebugMenuSkinBase(), mProjection(), mCamera(Vector2<f32>(0.0f, 0.0f), 1.0f), mTextWriter(), mPrimitiveDrawer()
    {
    }
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074D148 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return DebugMenuSkinBase::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074D0FC (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009834FC (guard 0x009834F8)
        static const RuntimeTypeInfo::Derive<DebugMenuSkinBase> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~DebugMenuSkinDefault(); // 0x0054AA68 slot 0x08
    // 0x0054AA38 slot 0x0C (deleting dtor)
    virtual void begin(const Viewport& viewport); // 0x00546360 slot 0x10
    virtual void end(); // 0x00546340 slot 0x14
    // the background of the menu, of an item and of the selected item (pos: the top left)
    virtual void drawBackground(const Vector3<f32>& pos, const Vector2<f32>& size); // 0x0054A848 slot 0x18
    virtual void drawItemBackground(const Vector3<f32>& pos, const Vector2<f32>& size); // 0x0054A8D0 slot 0x1C
    virtual void drawSelectedBackground(const Vector3<f32>& pos, const Vector2<f32>& size); // 0x0054A898 slot 0x20
    // a bar over the part from..to (0..1) of the height
    virtual void drawScrollBar(const Vector3<f32>& pos, const Vector2<f32>& size, f32 from, f32 to); // 0x0054A760 slot 0x24
    // color0 to color1 from top to bottom (or left to right)
    virtual void drawQuad(const Vector3<f32>& pos, const Vector2<f32>& size, const Color4f& color0,
                          const Color4f& color1, bool horizontal); // 0x0054A904 slot 0x28
    virtual void beginText(const Viewport* viewport); // 0x0053FC8C slot 0x2C
    virtual void endText(); // 0x0053FC6C slot 0x30
    // text in white (two slots of the same code)
    virtual void vf_0x34(const Vector2<f32>& pos, const SafeStringBase<char>& text); // 0x0054A810 slot 0x34
    virtual void vf_0x38(const Vector2<f32>& pos, const SafeStringBase<char>& text); // 0x0054A724 slot 0x38
    virtual void drawCursor(const Vector2<f32>& pos); // 0x0054A87C slot 0x3C
    virtual void vf_0x40(); // 0x0054A8CC slot 0x40
    virtual void drawText(const Vector2<f32>& pos, const SafeStringBase<char>& text, const Color4f& color); // 0x0054A9E0 slot 0x44
    virtual f32 getLineHeight() const; // 0x0054A824 slot 0x48
    // the width of 'm'
    virtual f32 getCharWidth() const; // 0x0054A738 slot 0x4C

    OrthoProjection mProjection;       // 0x004
    OrthoCamera mCamera;               // 0x0B0
    TextWriter mTextWriter;            // 0x108
    PrimitiveDrawer mPrimitiveDrawer;  // 0x158
};
ASSERT_SIZE(DebugMenuSkinDefault, 0x168);
} // namespace sead
