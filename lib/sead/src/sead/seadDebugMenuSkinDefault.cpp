#include "sead/seadDebugMenuSkinDefault.h"
#include "sead/seadFontBase.h"
#include "sead/seadViewport.h"

namespace {
// the colors of the backgrounds (names are ours)
// 0x00AF505C
const sead::Color4f s_BackgroundColor0 = {0.1328125f, 0.54296875f, 0.1328125f, 1.0f};
// 0x00AF506C
const sead::Color4f s_BackgroundColor1 = {0.0f, 0.5f, 0.0f, 1.0f};
// 0x00AF507C
const sead::Color4f s_ItemBackgroundColor0 = {0.0f, 0.39f, 0.0f, 1.0f};
// 0x00AF508C
const sead::Color4f s_ItemBackgroundColor1 = {0.0f, 0.3f, 0.0f, 1.0f};
// 0x00AF509C
const sead::Color4f s_SelectedBackgroundColor1 = {0.0328125f, 0.44296875f, 0.0328125f, 1.0f};
// 0x00AF50AC
const sead::Color4f s_ScrollBarBackgroundColor = {1.0f, 1.0f, 1.0f, 0.4f};
// 0x0097F864
const sead::SafeStringBase<char> s_CursorString(">");
// 0x00AF4EF4
sead::DebugMenuSkinDefault s_DebugMenuSkinDefault;
} // namespace

// the static initializer of the globals of the file
// 0x00797B70 (static initializer)

namespace sead {
// 0x00546360 slot 0x10 (name is ours)
void sead::DebugMenuSkinDefault::begin(const Viewport& viewport)
{
    mProjection.setByViewport(viewport);
    mCamera.setByOrthoProjection(mProjection);
    mCamera.doUpdateMatrix(&mCamera.mMatrix);
    mPrimitiveDrawer.setCamera(&mCamera);
    mPrimitiveDrawer.setProjection(&mProjection);
    TextWriter::setupGraphics();
    mPrimitiveDrawer.begin();
}

// 0x00546340 slot 0x14 (name is ours)
void sead::DebugMenuSkinDefault::end()
{
    mPrimitiveDrawer.end();
}

// 0x0054A848 slot 0x18 (name is ours)
void sead::DebugMenuSkinDefault::drawBackground(const Vector3<f32>& pos, const Vector2<f32>& size)
{
    drawQuad(pos, size, s_BackgroundColor0, s_BackgroundColor1, false);
}

// 0x0054A8D0 slot 0x1C (name is ours)
void sead::DebugMenuSkinDefault::drawItemBackground(const Vector3<f32>& pos, const Vector2<f32>& size)
{
    drawQuad(pos, size, s_ItemBackgroundColor0, s_ItemBackgroundColor1, false);
}

// 0x0054A898 slot 0x20 (name is ours)
void sead::DebugMenuSkinDefault::drawSelectedBackground(const Vector3<f32>& pos, const Vector2<f32>& size)
{
    drawQuad(pos, size, s_BackgroundColor0, s_SelectedBackgroundColor1, false);
}

// 0x0054A760 slot 0x24 (name is ours)
void sead::DebugMenuSkinDefault::drawScrollBar(const Vector3<f32>& pos, const Vector2<f32>& size, f32 from, f32 to)
{
    drawQuad(pos, size, s_ScrollBarBackgroundColor, s_ScrollBarBackgroundColor, false);
    Vector2<f32> barSize(size.x, size.y * (to - from));
    Vector3<f32> barPos(pos.x, pos.y - size.y * from, 0.0f);
    drawQuad(barPos, barSize, Color4f::white, Color4f::white, false);
}

// 0x0054A904 slot 0x28 (name is ours)
void sead::DebugMenuSkinDefault::drawQuad(const Vector3<f32>& pos, const Vector2<f32>& size, const Color4f& color0,
                                          const Color4f& color1, bool horizontal)
{
    PrimitiveDrawer::QuadArg arg;
    arg.mCenter.x = pos.x + size.x * 0.5f;
    arg.mCenter.y = pos.y - size.y * 0.5f;
    arg.mCenter.z = pos.z;
    arg.mSize.x = size.x;
    arg.mSize.y = size.y;
    if (horizontal) {
        arg.setColorHorizontal(color0, color1);
    } else {
        arg.setColorVertical(color0, color1);
    }
    mPrimitiveDrawer.drawQuad(arg);
}

// 0x0053FC8C slot 0x2C (name is ours)
void sead::DebugMenuSkinDefault::beginText(const Viewport* viewport)
{
    mTextWriter.mViewport = viewport;
    TextWriter::setupGraphics();
    mTextWriter.beginDraw();
}

// 0x0053FC6C slot 0x30 (name is ours)
void sead::DebugMenuSkinDefault::endText()
{
    mTextWriter.endDraw();
}

// 0x0054A810 slot 0x34
void sead::DebugMenuSkinDefault::vf_0x34(const Vector2<f32>& pos, const SafeStringBase<char>& text)
{
    drawText(pos, text, Color4f::white);
}

// 0x0054A724 slot 0x38
void sead::DebugMenuSkinDefault::vf_0x38(const Vector2<f32>& pos, const SafeStringBase<char>& text)
{
    drawText(pos, text, Color4f::white);
}

// 0x0054A87C slot 0x3C (name is ours)
void sead::DebugMenuSkinDefault::drawCursor(const Vector2<f32>& pos)
{
    drawText(pos, s_CursorString, Color4f::white);
}

// 0x0054A8CC slot 0x40
void sead::DebugMenuSkinDefault::vf_0x40()
{
    // empty (in the original too)
}

// 0x0054A9E0 slot 0x44 (name is ours)
void sead::DebugMenuSkinDefault::drawText(const Vector2<f32>& pos, const SafeStringBase<char>& text,
                                          const Color4f& color)
{
    mTextWriter.mCursor.x = pos.x;
    mTextWriter.mCursor.y = pos.y;
    mTextWriter.mColor = color;
    mTextWriter.printImpl_(text.getCString(), -1, true, nullptr);
}

// 0x0054A824 slot 0x48 (name is ours)
f32 sead::DebugMenuSkinDefault::getLineHeight() const
{
    return mTextWriter.mFont->getHeight() * mTextWriter.mScale.y;
}

// 0x0054A738 slot 0x4C (name is ours)
f32 sead::DebugMenuSkinDefault::getCharWidth() const
{
    return mTextWriter.mFont->getCharWidth(L'm') * mTextWriter.mScale.x;
}

// 0x0054AA68 slot 0x08
// 0x0054AA38 (deleting dtor)
sead::DebugMenuSkinDefault::~DebugMenuSkinDefault()
{
    // (only the members are destroyed)
}
} // namespace sead

// the type information of the classes below DebugMenuSkinBase
// 0x008290C8 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::DebugMenuSkinBase>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
