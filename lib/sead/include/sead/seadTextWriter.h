#pragma once

#include "decomp.h"
#include "sead/seadBoundBox2.h"
#include "sead/seadColor4f.h"
#include "sead/seadMatrix.h"
#include "sead/seadVector2.h"

namespace sead {
class Camera;
class FontBase;
class Projection;
class Viewport;

// RTTI N4sead10TextWriterE @ 0x008D13D4
// vtable 0x00904F48 (vptr 0x00904F50), offset_to_top 0, 2 entries
//
// Writes text with a font at a cursor that moves on (lines break at '\n', at the maximal width and
// after the most characters of one font call). Without a projection and a camera it draws with an
// orthographic projection of the viewport. All names are ours.
class TextWriter
{
public:
    // the default font, the alternative font when there is none
    TextWriter(); // 0x0053FCCC
    virtual ~TextWriter(); // 0x0053FD68 slot 0x00
    // 0x0053FD64 slot 0x04 (deleting dtor)

    // the render states for the text
    static void setupGraphics(); // 0x0053FC60
    // calls begin/end of the font once for all following prints
    void beginDraw(); // 0x0053FCA8
    void endDraw(); // 0x0053FC74

    // str (UTF-8; length < 0: up to the null character); draws it if doDraw, adds its area to boundBox
    void printImpl_(const char* str, s32 length, bool doDraw, BoundBox2<f32>* boundBox); // 0x0053F7B8
    void printImpl_(const wchar_t* str, s32 length, bool doDraw, BoundBox2<f32>* boundBox); // 0x0053F830
    void printImpl_(const wchar_t* str, s32 length, bool doDraw, BoundBox2<f32>* boundBox,
                    const Projection* projection, const Camera* camera); // 0x0053F914

    static FontBase* s_pDefaultFont;
    // (nothing in the binary sets it)
    static FontBase* s_pDefaultFontAlternative;

    const Viewport* mViewport;      // 0x04
    const Projection* mProjection;  // 0x08
    const Camera* mCamera;          // 0x0C
    FontBase* mFont;                // 0x10
    Vector2<f32> mCursor;           // 0x14, the top left of the next character
    Vector2<f32> mScale;            // 0x1C
    Color4f mColor;                 // 0x24
    f32 field_0x34;                 // 0x34
    f32 mLineSpace;                 // 0x38, added to the height of a line
    const Matrix34<f32>* mModelMtx; // 0x3C
    f32 mMaxWidth;                  // 0x40, infinite: no limit
    wchar_t* mBuffer;               // 0x44, NULL: 512 characters on the stack
    s32 mBufferSize;                // 0x48
    bool mCallFontBeginEnd;         // 0x4C, each print calls begin/end of the font
};
ASSERT_SIZE(TextWriter, 0x50);
} // namespace sead
