#include <cmath>

#include "mw/qrd/qrd_Internal.h"

namespace mw {
namespace qrd {
namespace detail {

namespace {

// rounds a symbol size to the next possible one (4 * version + 17), 0 if it is impossible
inline s32 ToSymbolSize(s32 a, s32 b)
{
    s32 size = ((a + b) >> 1) + 7;
    Yield();
    switch (size & 3)
    {
    case 0:
        size++;
        break;
    case 2:
        size--;
        break;
    case 3:
        return 0;
    }
    return size;
}

} // namespace

// 0x00165690
void FillRect(ModuleMatrix* matrix, s32 row, s32 column, s32 rowNum, s32 columnNum)
{
    for (s32 i = row; i < row + rowNum; i++)
    {
        for (s32 j = column; j < column + columnNum; j++)
        {
            matrix->modules[GetModuleIndex(matrix, i, j)] = 0xFF;
        }
    }
}

// 0x00169B88
void ComputeBoundingBox(SymbolPosition* position, s32 width, s32 height)
{
    Yield();
    const PatternPoint* p = position->points;

    // the finder patterns reach 3.5 modules out of their centres, the alignment pattern lies inside
    s32 left0 = p[0].x - p[0].moduleSize * 5.0f;
    s32 left1 = p[1].x - p[1].moduleSize * 5.0f;
    s32 left2 = p[2].x - p[2].moduleSize * 5.0f;
    s32 left3 = p[3].x - p[3].moduleSize * 8.0f;
    s32 left01 = left0 < left1 ? left0 : left1;
    s32 left23 = left2 < left3 ? left2 : left3;
    position->left = left01 < left23 ? left01 : left23;
    if (position->left <= 0)
    {
        position->left = 0;
    }

    s32 right0 = p[0].x + p[0].moduleSize * 5.0f;
    s32 right1 = p[1].x + p[1].moduleSize * 5.0f;
    s32 right2 = p[2].x + p[2].moduleSize * 5.0f;
    s32 right3 = p[3].x + p[3].moduleSize * 8.0f;
    s32 right01 = right0 > right1 ? right0 : right1;
    s32 right23 = right2 > right3 ? right2 : right3;
    position->right = right01 > right23 ? right01 : right23;
    if (position->right >= width)
    {
        position->right = width - 1;
    }

    s32 top0 = p[0].y - p[0].moduleSize * 5.0f;
    s32 top1 = p[1].y - p[1].moduleSize * 5.0f;
    s32 top2 = p[2].y - p[2].moduleSize * 5.0f;
    s32 top3 = p[3].y - p[3].moduleSize * 8.0f;
    s32 top01 = top0 < top1 ? top0 : top1;
    s32 top23 = top2 < top3 ? top2 : top3;
    position->top = top01 < top23 ? top01 : top23;
    if (position->top <= 0)
    {
        position->top = 0;
    }

    s32 bottom0 = p[0].y + p[0].moduleSize * 5.0f;
    s32 bottom1 = p[1].y + p[1].moduleSize * 5.0f;
    s32 bottom2 = p[2].y + p[2].moduleSize * 5.0f;
    s32 bottom3 = p[3].y + p[3].moduleSize * 8.0f;
    s32 bottom01 = bottom0 > bottom1 ? bottom0 : bottom1;
    s32 bottom23 = bottom2 > bottom3 ? bottom2 : bottom3;
    position->bottom = bottom01 > bottom23 ? bottom01 : bottom23;
    if (position->bottom >= height)
    {
        position->bottom = height - 1;
    }
}

// 0x0016CCD4
s32 EstimateSymbolSize(const FinderPattern* topLeft, const FinderPattern* topRight, const FinderPattern* bottomLeft,
                       f32 moduleSize)
{
    f64 dx = topLeft->x - topRight->x;
    f64 dy = topLeft->y - topRight->y;
    s32 toTopRight = static_cast<f32>(std::sqrt(dx * dx + dy * dy)) / moduleSize + 0.5f;
    dx = topLeft->x - bottomLeft->x;
    dy = topLeft->y - bottomLeft->y;
    s32 toBottomLeft = static_cast<f32>(std::sqrt(dx * dx + dy * dy)) / moduleSize + 0.5f;

    // the mean of both distances, else each one alone
    s32 size = ToSymbolSize(toTopRight, toBottomLeft);
    if (size == 0)
    {
        size = ToSymbolSize(toTopRight, toTopRight);
    }
    if (size == 0)
    {
        size = ToSymbolSize(toBottomLeft, toBottomLeft);
    }
    return size;
}

// 0x0016F108
void SetFinderCentres(s32 width, s32 height, const FinderPattern* finder0, const FinderPattern* finder1,
                      const FinderPattern* finder2)
{
    Yield();
    s_SampleInfo.width = width;
    s_SampleInfo.height = height;
    s_FinderAreas[0].centre.x = finder0->x;
    s_FinderAreas[0].centre.y = finder0->y;
    s_FinderAreas[1].centre.x = finder1->x;
    s_FinderAreas[1].centre.y = finder1->y;
    s_FinderAreas[2].centre.x = finder2->x;
    s_FinderAreas[2].centre.y = finder2->y;
}

// 0x00172754
f32 EstimateModuleSize(const FinderPattern* topLeft, const FinderPattern* topRight, const FinderPattern* bottomLeft)
{
    return (MeasureModuleSize(topLeft, topRight) + MeasureModuleSize(topLeft, bottomLeft)) * 0.5f;
}

// length of the dark, light and dark run from (fromX, fromY) towards (toX, toY) (Bresenham)
// 0x0017C7A4
f32 MeasureRun(s32 fromX, s32 fromY, s32 toX, s32 toY)
{
    Yield();
    bool isSteep = std::fabs(static_cast<f32>(toY - fromY)) > std::fabs(static_cast<f32>(toX - fromX));
    if (isSteep)
    {
        s32 temp = fromX;
        fromX = fromY;
        fromY = temp;
        temp = toX;
        toX = toY;
        toY = temp;
    }

    s32 diffX = toX - fromX;
    s32 diffY = toY - fromY;
    s32 dx = std::fabs(static_cast<f32>(diffX));
    s32 dy = std::fabs(static_cast<f32>(diffY));
    s32 error = -dx >> 1;
    s32 xStep = fromX < toX ? 1 : -1;
    s32 yStep = fromY < toY ? 1 : -1;
    s32 state = 0;
    for (s32 x = fromX, y = fromY; x != toX; x += xStep)
    {
        s32 realX = isSteep ? y : x;
        s32 realY = isSteep ? x : y;
        if (state == 1)
        {
            if (!IsLight(realX, realY))
            {
                state = 2;
            }
        }
        else
        {
            if (IsLight(realX, realY))
            {
                state++;
            }
            if (state == 3)
            {
                return std::sqrt(static_cast<f64>((x - fromX) * (x - fromX) + (y - fromY) * (y - fromY)));
            }
        }
        error += dy;
        if (error > 0)
        {
            y += yStep;
            error -= dx;
        }
    }
    return std::sqrt(static_cast<f64>(diffX * diffX + diffY * diffY));
}

// 0x0017D738
f32 MeasureModuleSize(const FinderPattern* pattern, const FinderPattern* other)
{
    f32 size1 = MeasureRunBothWays(pattern->x, pattern->y, other->x, other->y);
    f32 size2 = MeasureRunBothWays(other->x, other->y, pattern->x, pattern->y);
    Yield();
    if (size1 == -0.0f)
    {
        return size2;
    }
    if (size2 == -0.0f)
    {
        return size1;
    }
    return (size1 + size2) * (1.0f / 14.0f);
}

// 0x00181BA0
f32 MeasureRunBothWays(s32 fromX, s32 fromY, s32 toX, s32 toY)
{
    f32 result = MeasureRun(fromX, fromY, toX, toY);
    Yield();

    // the other way, as far as the image goes
    s32 otherToX = fromX - (toX - fromX);
    if (otherToX < 0)
    {
        otherToX = 0;
    }
    else if (otherToX > s_Context.bitImage.width)
    {
        otherToX = s_Context.bitImage.width;
    }
    s32 otherToY = fromY - (toY - fromY);
    if (otherToY < 0)
    {
        otherToY = 0;
    }
    else if (otherToY > s_Context.bitImage.height)
    {
        otherToY = s_Context.bitImage.height;
    }
    result += MeasureRun(fromX, fromY, otherToX, otherToY);
    return result - 1.0f;
}

} // namespace detail
} // namespace qrd
} // namespace mw
