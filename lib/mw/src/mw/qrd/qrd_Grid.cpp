#include <cstring>

#include "mw/qrd/qrd_Internal.h"

namespace mw {
namespace qrd {
namespace detail {

namespace {

inline Transform BuildAdjoint(const Transform& t)
{
    Transform result;
    result.a11 = t.a22 * t.a33 - t.a23 * t.a32;
    result.a12 = t.a13 * t.a32 - t.a12 * t.a33;
    result.a13 = t.a12 * t.a23 - t.a13 * t.a22;
    result.a21 = t.a23 * t.a31 - t.a21 * t.a33;
    result.a22 = t.a11 * t.a33 - t.a13 * t.a31;
    result.a23 = t.a13 * t.a21 - t.a11 * t.a23;
    result.a31 = t.a21 * t.a32 - t.a22 * t.a31;
    result.a32 = t.a12 * t.a31 - t.a11 * t.a32;
    result.a33 = t.a11 * t.a22 - t.a12 * t.a21;
    return result;
}

inline Transform MultiplyTransforms(const Transform& a, const Transform& b)
{
    Transform result;
    result.a11 = a.a11 * b.a11 + a.a21 * b.a12 + a.a31 * b.a13;
    result.a21 = a.a11 * b.a21 + a.a21 * b.a22 + a.a31 * b.a23;
    result.a31 = a.a11 * b.a31 + a.a21 * b.a32 + a.a31 * b.a33;
    result.a12 = a.a12 * b.a11 + a.a22 * b.a12 + a.a32 * b.a13;
    result.a22 = a.a12 * b.a21 + a.a22 * b.a22 + a.a32 * b.a23;
    result.a32 = a.a12 * b.a31 + a.a22 * b.a32 + a.a32 * b.a33;
    result.a13 = a.a13 * b.a11 + a.a23 * b.a12 + a.a33 * b.a13;
    result.a23 = a.a13 * b.a21 + a.a23 * b.a22 + a.a33 * b.a23;
    result.a33 = a.a13 * b.a31 + a.a23 * b.a32 + a.a33 * b.a33;
    return result;
}

// the first point of a row may lie one pixel outside the image (false when it is further out)
inline bool NudgePoint(f32* points, f32 x, f32 y, s32 width, s32 height)
{
    s32 px = x;
    s32 py = y;
    if (!(px >= -1 && width >= px && py >= -1 && height >= py))
    {
        return false;
    }
    if (px == -1)
    {
        points[0] = 0.0f;
    }
    if (py == -1)
    {
        points[1] = 0.0f;
    }
    if (px == width)
    {
        points[0] = width - 1;
    }
    if (py == height)
    {
        points[1] = height - 1;
    }
    return true;
}

} // namespace

// samples the modules through the perspective given by the finder patterns and the alignment pattern
// 0x00164CCC
u8* SampleGrid(const FinderPattern* topLeft, const FinderPattern* topRight, const FinderPattern* bottomLeft,
               const PatternPoint* alignment, s32 symbolSize)
{
    f32 dimMinusThree = static_cast<f32>(symbolSize) - 3.5f;
    f32 bottomRightX;
    f32 bottomRightY;
    f32 sourceBottomRight;

    Yield();
    if (alignment != NULL && alignment->moduleSize > 0.0f)
    {
        bottomRightX = alignment->x;
        bottomRightY = alignment->y;
        sourceBottomRight = dimMinusThree - 3.0f;
    }
    else
    {
        bottomRightX = (static_cast<f32>(topRight->x) - static_cast<f32>(topLeft->x)) + static_cast<f32>(bottomLeft->x);
        bottomRightY = (static_cast<f32>(topRight->y) - static_cast<f32>(topLeft->y)) + static_cast<f32>(bottomLeft->y);
        sourceBottomRight = dimMinusThree;
    }

    Transform toSquare;
    SquareToQuadrilateral(&toSquare, 3.5f, 3.5f, dimMinusThree, 3.5f, sourceBottomRight, sourceBottomRight, 3.5f,
                          dimMinusThree);
    Transform adjoint = BuildAdjoint(toSquare);
    Transform toImage;
    SquareToQuadrilateral(&toImage, topLeft->x, topLeft->y, topRight->x, topRight->y, bottomRightX, bottomRightY,
                          bottomLeft->x, bottomLeft->y);
    Transform transform = MultiplyTransforms(toImage, adjoint);

    u8* bits = static_cast<u8*>(Allocate(symbolSize * symbolSize));
    if (bits == NULL)
    {
        return NULL;
    }
    std::memset(bits, 0, symbolSize * symbolSize);
    f32* points = static_cast<f32*>(Allocate(symbolSize * 8 + 4));
    if (points == NULL)
    {
        return NULL;
    }
    std::memset(points, 0, symbolSize * 8);

    for (s32 y = 0; y < symbolSize; y++)
    {
        s32 max = symbolSize * 2;
        f32 iValue = y + 0.5f;
        for (s32 x = 0; x < max; x += 2)
        {
            points[x] = (x / 2) + 0.5f;
            points[x + 1] = iValue;
        }
        for (s32 i = 0; i < max; i += 2)
        {
            f32 px = points[i];
            f32 py = points[i + 1];
            f32 denominator = transform.a13 * px + transform.a23 * py + transform.a33;
            points[i] = (transform.a11 * px + transform.a21 * py + transform.a31) / denominator;
            points[i + 1] = (transform.a12 * px + transform.a22 * py + transform.a32) / denominator;
        }

        // (the original nudges the first point for the last one too)
        s32 width = s_Context.bitImage.width;
        s32 height = s_Context.bitImage.height;
        if (!NudgePoint(points, points[0], points[1], width, height) ||
            !NudgePoint(points, points[max - 2], points[max - 1], width, height))
        {
            Free(points);
            return NULL;
        }

        for (s32 x = 0; x < max; x += 2)
        {
            if (IsDark(points[x], points[x + 1]))
            {
                bits[y * symbolSize + x / 2] = 0xFF;
            }
        }
    }
    Free(points);
    return bits;
}

// the transform that maps the unit square onto the quadrilateral (x0, y0) .. (x3, y3)
// 0x00187354
void SquareToQuadrilateral(Transform* out, f32 x0, f32 y0, f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3)
{
    f32 dy2 = y3 - y2;
    f32 dy3 = y0 - y1 + y2 - y3;
    // (the original tests dy2 here where the affine case needs dx3)
    if (dy2 == 0.0f && dy3 == 0.0f)
    {
        out->a31 = x0;
        out->a32 = y0;
        out->a13 = 0.0f;
        out->a23 = 0.0f;
        out->a33 = 1.0f;
        out->a21 = x2 - x1;
        out->a22 = y2 - y1;
        out->a11 = x1 - x0;
        out->a12 = y1 - y0;
        return;
    }

    f32 dx1 = x1 - x2;
    f32 dx2 = x3 - x2;
    f32 dx3 = x0 - x1 + x2 - x3;
    f32 dy1 = y1 - y2;
    f32 denominator = dx1 * dy2 - dx2 * dy1;
    f32 a13 = (dx3 * dy2 - dx2 * dy3) / denominator;
    f32 a23 = (dx1 * dy3 - dx3 * dy1) / denominator;
    out->a13 = a13;
    out->a23 = a23;
    out->a21 = x3 - x0 + a23 * x3;
    out->a22 = y3 - y0 + a23 * y3;
    out->a33 = 1.0f;
    out->a31 = x0;
    out->a32 = y0;
    out->a11 = x1 - x0 + a13 * x1;
    out->a12 = y1 - y0 + a13 * y1;
}

} // namespace detail
} // namespace qrd
} // namespace mw
