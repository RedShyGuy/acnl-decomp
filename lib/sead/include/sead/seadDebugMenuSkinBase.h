#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"
#include "sead/seadColor4f.h"
#include "sead/seadSafeStringBase.h"
#include "sead/seadVector2.h"
#include "sead/seadVector3.h"

namespace sead {
class Viewport;

// RTTI N4sead17DebugMenuSkinBaseE @ 0x008D1C0C
// The look of the debug menu (an interface).
class DebugMenuSkinBase
{
public:
    DebugMenuSkinBase() {}
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009834C4 (guard 0x009834C0)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~DebugMenuSkinBase() {} // slot 0x08
    // the frame of the debug menu: sets up the camera and the projection for viewport (names of all
    // slots are ours)
    virtual void begin(const Viewport& viewport) = 0; // slot 0x10
    virtual void end() = 0; // slot 0x14
    // the background of the menu, of an item and of the selected item (pos: the top left)
    virtual void drawBackground(const Vector3<f32>& pos, const Vector2<f32>& size) = 0; // slot 0x18
    virtual void drawItemBackground(const Vector3<f32>& pos, const Vector2<f32>& size) = 0; // slot 0x1C
    virtual void drawSelectedBackground(const Vector3<f32>& pos, const Vector2<f32>& size) = 0; // slot 0x20
    // a bar over the part from..to (0..1) of the height
    virtual void drawScrollBar(const Vector3<f32>& pos, const Vector2<f32>& size, f32 from, f32 to) = 0; // slot 0x24
    // color0 to color1 from top to bottom (or left to right)
    virtual void drawQuad(const Vector3<f32>& pos, const Vector2<f32>& size, const Color4f& color0,
                          const Color4f& color1, bool horizontal) = 0; // slot 0x28
    virtual void beginText(const Viewport* viewport) = 0; // slot 0x2C
    virtual void endText() = 0; // slot 0x30
    // text in white (two slots of the same code)
    virtual void vf_0x34(const Vector2<f32>& pos, const SafeStringBase<char>& text) = 0; // slot 0x34
    virtual void vf_0x38(const Vector2<f32>& pos, const SafeStringBase<char>& text) = 0; // slot 0x38
    virtual void drawCursor(const Vector2<f32>& pos) = 0; // slot 0x3C
    virtual void vf_0x40() = 0; // slot 0x40
    virtual void drawText(const Vector2<f32>& pos, const SafeStringBase<char>& text, const Color4f& color) = 0; // slot 0x44
    virtual f32 getLineHeight() const = 0; // slot 0x48
    // the width of 'm'
    virtual f32 getCharWidth() const = 0; // slot 0x4C
};
} // namespace sead
