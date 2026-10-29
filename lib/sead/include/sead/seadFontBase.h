#pragma once

#include "decomp.h"
#include "sead/seadColor4f.h"
#include "sead/seadMatrix.h"

namespace sead {
class Camera;
class Projection;

// RTTI N4sead8FontBaseE @ 0x008D21E4
//
// A font for the TextWriter (the interface only; the slots are known from the DummyFont of
// seadTextWriter.cpp and their calls; names are ours).
class FontBase
{
public:
    FontBase() {}
    virtual ~FontBase() {} // slot 0x00
    virtual f32 getHeight() const = 0; // slot 0x08
    virtual f32 getWidth() const = 0; // slot 0x0C
    virtual f32 getCharWidth(wchar_t c) const = 0; // slot 0x10
    virtual s32 getEncoding() const = 0; // slot 0x14
    // the most characters one print call takes
    virtual s32 getMaxDrawNum() const = 0; // slot 0x18
    virtual void begin() const = 0; // slot 0x1C
    virtual void end() const = 0; // slot 0x20
    // draws length characters of str with the model matrix
    virtual void print(const Projection* projection, const Camera* camera, const Matrix34<f32>& model,
                       const Color4f& color, const wchar_t* str, s32 length) const = 0; // slot 0x24
};
} // namespace sead
