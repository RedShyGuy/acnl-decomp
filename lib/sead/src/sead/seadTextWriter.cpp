#include "sead/seadTextWriter.h"
#include "sead/seadFontBase.h"
#include "sead/seadGraphicsContext.h"
#include "sead/seadMathCalcCommon.h"
#include "sead/seadOrthoCamera.h"
#include "sead/seadOrthoProjection.h"
#include "sead/seadStringUtil.h"

#include <cmath>

namespace {
// RTTI N42_GLOBAL__N_18_seadTextWriter_cpp_06da2534DummyFontE
// vtable 0x008C3098 (vptr 0x008C30A0), offset_to_top 0, 10 entries
//
// The font of the program before one is set: draws nothing.
class DummyFont : public sead::FontBase
{
public:
    virtual ~DummyFont(); // 0x00528F08 slot 0x00
    // 0x00528F04 slot 0x04 (deleting dtor)
    virtual f32 getHeight() const; // 0x00748CFC slot 0x08
    virtual f32 getWidth() const; // 0x00748CF0 slot 0x0C
    virtual f32 getCharWidth(wchar_t) const; // 0x00748CD0 slot 0x10
    virtual s32 getEncoding() const; // 0x00748CC8 slot 0x14
    virtual s32 getMaxDrawNum() const; // 0x00748CDC slot 0x18
    virtual void begin() const; // 0x00748CE8 slot 0x1C
    virtual void end() const; // 0x00748CE4 slot 0x20
    virtual void print(const sead::Projection*, const sead::Camera*, const sead::Matrix34<f32>&, const sead::Color4f&,
                       const wchar_t*, s32) const; // 0x00748CEC slot 0x24
};

// 0x00748CFC slot 0x08
inline f32 DummyFont::getHeight() const
{
    return 16.0f;
}

// 0x00748CF0 slot 0x0C
inline f32 DummyFont::getWidth() const
{
    return 12.0f;
}

// 0x00748CD0 slot 0x10
inline f32 DummyFont::getCharWidth(wchar_t) const
{
    return 12.0f;
}

// 0x00748CC8 slot 0x14
inline s32 DummyFont::getEncoding() const
{
    return 2;
}

// 0x00748CDC slot 0x18
inline s32 DummyFont::getMaxDrawNum() const
{
    return 128;
}

// 0x00748CE8 slot 0x1C
inline void DummyFont::begin() const
{
    // empty (in the original too)
}

// 0x00748CE4 slot 0x20
inline void DummyFont::end() const
{
    // empty (in the original too)
}

// 0x00748CEC slot 0x24
inline void DummyFont::print(const sead::Projection*, const sead::Camera*, const sead::Matrix34<f32>&,
                             const sead::Color4f&, const wchar_t*, s32) const
{
    // empty (in the original too)
}

// 0x00528F08 slot 0x00
// 0x00528F04 (deleting dtor)
DummyFont::~DummyFont()
{
    // empty (in the original too)
}

// RTTI N42_GLOBAL__N_18_seadTextWriter_cpp_06da2534GraphicsContextForTextWriterE
// vtable 0x008C30C8 (vptr 0x008C30D0), offset_to_top 0, 2 entries
//
// The render states of text: no culling, no depth test, no depth write.
class GraphicsContextForTextWriter : public sead::GraphicsContext
{
public:
    GraphicsContextForTextWriter() : GraphicsContext()
    {
        mCullingMode = -16;
        mRenderState.m_Culling.m_IsEnable = false;
        mDepthTestEnable = false;
        mRenderState.m_DepthTest.m_IsEnable = false;
        mDepthWriteEnable = false;
        mRenderState.m_DepthTest.m_IsEnableWrite = false;
    }
    virtual ~GraphicsContextForTextWriter(); // 0x00528EF4 slot 0x00
    // 0x00528EE4 slot 0x04 (deleting dtor)
};

// 0x00528EF4 slot 0x00
// 0x00528EE4 (deleting dtor)
GraphicsContextForTextWriter::~GraphicsContextForTextWriter()
{
    // (only the bases are destroyed)
}

// 0x00982D94
DummyFont s_DummyFont;
// 0x00AF64E8
GraphicsContextForTextWriter s_GraphicsContextForTextWriter;
} // namespace

// the static initializer of the dummy font and the render states
// 0x00798EC8 (static initializer)

namespace sead {
// 0x00982D90
FontBase* TextWriter::s_pDefaultFont = nullptr;
// 0x00982ED8
FontBase* TextWriter::s_pDefaultFontAlternative = nullptr;

// 0x0053FC60 (name is ours)
void sead::TextWriter::setupGraphics()
{
    s_GraphicsContextForTextWriter.apply();
}

// 0x0053FC74 (name is ours)
void sead::TextWriter::endDraw()
{
    mCallFontBeginEnd = true;
    mFont->end();
}

// 0x0053FCA8 (name is ours)
void sead::TextWriter::beginDraw()
{
    mFont->begin();
    mCallFontBeginEnd = false;
}

// 0x0053FCCC (name is ours)
sead::TextWriter::TextWriter()
    : mViewport(nullptr), mProjection(nullptr), mCamera(nullptr),
      mFont(s_pDefaultFont != nullptr ? s_pDefaultFont : s_pDefaultFontAlternative), mCursor(0.0f, 0.0f),
      mScale(1.0f, 1.0f), field_0x34(0.0f), mLineSpace(0.0f), mModelMtx(nullptr), mMaxWidth(INFINITY),
      mBuffer(nullptr), mBufferSize(0), mCallFontBeginEnd(true)
{
    mColor.r = 1.0f;
    mColor.g = 1.0f;
    mColor.b = 1.0f;
    mColor.a = 1.0f;
}

// 0x0053FD68 slot 0x00
// 0x0053FD64 (deleting dtor)
sead::TextWriter::~TextWriter()
{
    // empty (in the original too)
}

// 0x0053F7B8 (name is ours)
void sead::TextWriter::printImpl_(const char* str, s32 length, bool doDraw, BoundBox2<f32>* boundBox)
{
    wchar_t stackBuffer[512];
    wchar_t* buffer = mBuffer;
    s32 bufferSize;
    if (buffer != nullptr) {
        bufferSize = mBufferSize;
    } else {
        buffer = stackBuffer;
        bufferSize = 512;
    }
    if (length >= 0 && length + 1 <= bufferSize) {
        bufferSize = length + 1;
    }
    StringUtil::convertUtf8ToUtf16(buffer, bufferSize, str, bufferSize - 1);
    printImpl_(buffer, -1, doDraw, boundBox);
}

// 0x0053F830 (name is ours)
void sead::TextWriter::printImpl_(const wchar_t* str, s32 length, bool doDraw, BoundBox2<f32>* boundBox)
{
    if ((mProjection == nullptr || mCamera == nullptr) && doDraw) {
        // an orthographic projection of the viewport
        OrthoProjection projection(1.0f, 2000.0f, *mViewport);
        OrthoCamera camera(projection);
        camera.doUpdateMatrix(&camera.mMatrix);
        printImpl_(str, length, doDraw, boundBox, &projection, &camera);
        return;
    }
    printImpl_(str, length, doDraw, boundBox, mProjection, mCamera);
}

namespace {
// why a line of the text ended
enum LineEnd
{
    LINE_END_TEXT = 0,      // the end of the text
    LINE_END_NEWLINE = 1,   // '\n'
    LINE_END_WIDTH = 2,     // the maximal width
    LINE_END_DRAW_NUM = 3,  // the most characters of one print call of the font
};
} // namespace

// 0x0053F914 (name is ours)
void sead::TextWriter::printImpl_(const wchar_t* str, s32 length, bool doDraw, BoundBox2<f32>* boundBox,
                                  const Projection* projection, const Camera* camera)
{
    s32 start = 0;
    Matrix34<f32> local(mScale.x, 0.0f, 0.0f, 0.0f, 0.0f, mScale.y, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    s32 maxDrawNum = mFont->getMaxDrawNum();
    f32 x = mCursor.x;
    f32 y = mCursor.y;
    f32 startX = x;
    f32 lineHeight = mFont->getHeight() * mScale.y;
    f32 right = mMaxWidth + x;
    if (boundBox != nullptr) {
        boundBox->set(x, y - lineHeight, x, y);
    }
    if (mCallFontBeginEnd && doDraw) {
        mFont->begin();
    }

    f32 width;
    while (true) {
        width = 0.0f;
        s32 count = 0;
        LineEnd end = LINE_END_TEXT;
        while (true) {
            s32 index = start + count;
            wchar_t c = str[index];
            if (c == L'\0' || (length >= 0 && index >= length)) {
                break;
            }
            if (c == L'\n') {
                end = LINE_END_NEWLINE;
                break;
            }
            if (count >= maxDrawNum) {
                end = LINE_END_DRAW_NUM;
                break;
            }
            f32 charWidth = mFont->getCharWidth(c) * mScale.x;
            if (mMaxWidth != INFINITY && x + width + charWidth > right) {
                end = LINE_END_WIDTH;
                break;
            }
            width += charWidth;
            count++;
        }

        if (doDraw && count > 0) {
            local.m[0][3] = x;
            local.m[1][3] = y - lineHeight;
            local.m[2][3] = 0.0f;
            if (mModelMtx != nullptr) {
                Matrix34<f32> model;
                Matrix34CalcCtr<f32>::multiply(model, *mModelMtx, local);
                mFont->print(projection, camera, model, mColor, str + start, count);
            } else {
                mFont->print(projection, camera, local, mColor, str + start, count);
            }
        }
        if (boundBox != nullptr && count > 0) {
            f32 lineRight = x + width;
            f32 lineBottom = y - lineHeight;
            if (boundBox->mMin.x > lineRight) {
                boundBox->mMin.x = lineRight;
            }
            if (boundBox->mMin.y > lineBottom) {
                boundBox->mMin.y = lineBottom;
            }
            if (!(boundBox->mMax.x > lineRight)) {
                boundBox->mMax.x = lineRight;
            }
            if (!(boundBox->mMax.y > lineBottom)) {
                boundBox->mMax.y = lineBottom;
            }
        }

        if (end == LINE_END_TEXT) {
            break;
        }
        if (end == LINE_END_DRAW_NUM) {
            // the line goes on with the next characters
            x += width;
            start += count;
            continue;
        }
        x = startX;
        if (end == LINE_END_NEWLINE) {
            start = start + count + 1;
        } else {
            start += count;
        }
        y -= mLineSpace + lineHeight;
    }

    f32 endX = x + width;
    if (doDraw) {
        if (mCallFontBeginEnd) {
            mFont->end();
        }
        mCursor.x = endX;
        mCursor.y = y;
    }
}
} // namespace sead
