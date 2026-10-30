#include <cmath>
#include <cstring>

#include "mw/qrd/qrd_Internal.h"

// The second sampling of the reader: the outer corners of the finder patterns give a grid of module lines,
// which is moved onto the alignment patterns.

namespace mw {
namespace qrd {
namespace detail {

// 0x0097F8D0
SampleInfo s_SampleInfo;
// 0x00AF5980
FinderArea s_FinderAreas[3];
// 0x00AF5AD0
SampleSteps s_SampleSteps;

inline s32 GetSampleSize()
{
    s32 size = s_SampleInfo.version == -1 ? -1 : s_SampleInfo.version * 4 + 17;
    Yield();
    return size;
}

inline Point2d& RowLine(s32 size, s32 line, s32 index)
{
    return s_SampleInfo.rowLines[line * size + index];
}

inline Point2d& ColumnLine(s32 size, s32 line, s32 index)
{
    return s_SampleInfo.columnLines[line * size + index];
}

inline Point2f ToPoint2f(const Point2d& point)
{
    Point2f result;
    result.x = point.x;
    result.y = point.y;
    return result;
}

// the crossing of two lines of the grid
inline void IntersectInto(Point2d* out, Point2f* point, const Point2d& a1, const Point2d& a2, const Point2d& b1,
                          const Point2d& b2)
{
    IntersectLines(point, ToPoint2f(a1), ToPoint2f(a2), ToPoint2f(b1), ToPoint2f(b2));
    out->x = point->x;
    out->y = point->y;
}

// the outer lines of the grid from the corners of the finder patterns
// 0x00172018
bool BuildSampleLines()
{
    s32 size = GetSampleSize();
    if (size == -1)
    {
        return false;
    }

    const FinderArea* areas = s_FinderAreas;
    const SampleSteps& steps = s_SampleSteps;
    const f64 STEP = 1.0 / 7.0;

    // the module steps at each finder pattern without the change over its 7 modules
    f64 ux = (static_cast<f64>(areas[0].corners[1].x - areas[0].corners[0].x) - steps.deltaX.x * 21.0) * STEP;
    f64 vx = (static_cast<f64>(areas[0].corners[2].x - areas[0].corners[0].x) - steps.deltaY.x * 21.0) * STEP;
    f64 uy = (static_cast<f64>(areas[0].corners[1].y - areas[0].corners[0].y) - steps.deltaX.y * 21.0) * STEP;
    f64 vy = (static_cast<f64>(areas[0].corners[2].y - areas[0].corners[0].y) - steps.deltaY.y * 21.0) * STEP;
    Point2d& first = RowLine(size, 0, 0);
    first.x = areas[0].corners[0].x + vx * 0.5 + ux * 0.5;
    ColumnLine(size, 0, 0).x = first.x;
    first.y = areas[0].corners[0].y + uy * 0.5 + vy * 0.5;
    ColumnLine(size, 0, 0).y = first.y;
    RowLine(size, 1, 0).x = areas[0].corners[2].x + ux * 0.5 - vx * 0.5 - steps.deltaY.x * 3.0;
    RowLine(size, 1, 0).y = areas[0].corners[2].y + uy * 0.5 - vy * 0.5 - steps.deltaY.y * 3.0;
    ColumnLine(size, 1, 0).x = areas[0].corners[1].x - ux * 0.5 + vx * 0.5 - steps.deltaX.x * 3.0;
    ColumnLine(size, 1, 0).y = areas[0].corners[1].y - uy * 0.5 + vy * 0.5 - steps.deltaX.y * 3.0;

    ux = (static_cast<f64>(areas[1].corners[1].x - areas[1].corners[0].x) - steps.deltaX.x * 21.0) * STEP;
    vx = (static_cast<f64>(areas[1].corners[2].x - areas[1].corners[0].x) - steps.deltaY.x * 21.0) * STEP;
    uy = (static_cast<f64>(areas[1].corners[1].y - areas[1].corners[0].y) - steps.deltaX.y * 21.0) * STEP;
    vy = (static_cast<f64>(areas[1].corners[2].y - areas[1].corners[0].y) - steps.deltaY.y * 21.0) * STEP;
    RowLine(size, 0, size - 1).x = areas[1].corners[1].x - ux * 0.5 + vx * 0.5 - steps.deltaX.x * 3.0;
    RowLine(size, 0, size - 1).y = areas[1].corners[1].y - uy * 0.5 + vy * 0.5 - steps.deltaX.y * 3.0;
    RowLine(size, 1, size - 1).x =
        areas[1].corners[3].x - ux * 0.5 - vx * 0.5 - steps.deltaX.x * 3.0 - steps.deltaY.x * 3.0;
    RowLine(size, 1, size - 1).y =
        areas[1].corners[3].y - uy * 0.5 - vy * 0.5 - steps.deltaX.y * 3.0 - steps.deltaY.y * 3.0;

    ux = (static_cast<f64>(areas[2].corners[1].x - areas[2].corners[0].x) - steps.deltaX.x * 21.0) * STEP;
    vx = (static_cast<f64>(areas[2].corners[2].x - areas[2].corners[0].x) - steps.deltaY.x * 21.0) * STEP;
    uy = (static_cast<f64>(areas[2].corners[1].y - areas[2].corners[0].y) - steps.deltaX.y * 21.0) * STEP;
    vy = (static_cast<f64>(areas[2].corners[2].y - areas[2].corners[0].y) - steps.deltaY.y * 21.0) * STEP;
    ColumnLine(size, 0, size - 1).x = areas[2].corners[2].x + ux * 0.5 - vx * 0.5 - steps.deltaY.x * 3.0;
    ColumnLine(size, 0, size - 1).y = areas[2].corners[2].y + uy * 0.5 - vy * 0.5 - steps.deltaY.y * 3.0;
    ColumnLine(size, 1, size - 1).x =
        areas[2].corners[3].x - ux * 0.5 - vx * 0.5 - steps.deltaX.x * 3.0 - steps.deltaY.x * 3.0;
    ColumnLine(size, 1, size - 1).y =
        areas[2].corners[3].y - uy * 0.5 - vy * 0.5 - steps.deltaX.y * 3.0 - steps.deltaY.y * 3.0;

    InterpolateRowLine(0, size - 1, 0);
    InterpolateColumnLine(0, size - 1, 0);
    InterpolateRowLine(0, size - 1, 1);
    InterpolateColumnLine(0, size - 1, 1);
    if (static_cast<u32>(s_SampleInfo.version) < 7)
    {
        return true;
    }

    // move the lines to the alignment patterns along the finder pattern rows
    s32 previous = 0;
    for (s32 i = 0; i < s_AlignmentCentres[s_SampleInfo.version - 1][0] - 1; i++)
    {
        s32 position = s_AlignmentCentres[s_SampleInfo.version - 1][1 + i];
        Point2d old = RowLine(size, 1, position);
        RefineAlignment(&RowLine(size, 1, position));
        RowLine(size, 0, position).x += RowLine(size, 1, position).x - old.x;
        RowLine(size, 0, position).y += RowLine(size, 1, position).y - old.y;
        InterpolateRowLine(previous, position, 0);
        InterpolateRowLine(previous, position, 1);
        ColumnLine(size, i + 2, 0) = RowLine(size, 0, position);
        ColumnLine(size, i + 2, 6) = RowLine(size, 1, position);
        InterpolateColumnLine(0, 6, i + 2);

        old = ColumnLine(size, 1, position);
        RefineAlignment(&ColumnLine(size, 1, position));
        ColumnLine(size, 0, position).x += ColumnLine(size, 1, position).x - old.x;
        ColumnLine(size, 0, position).y += ColumnLine(size, 1, position).y - old.y;
        InterpolateColumnLine(previous, position, 0);
        InterpolateColumnLine(previous, position, 1);
        RowLine(size, i + 2, 0) = ColumnLine(size, 0, position);
        RowLine(size, i + 2, 6) = ColumnLine(size, 1, position);
        InterpolateRowLine(0, 6, i + 2);
        previous = position;
    }
    InterpolateRowLine(previous, size - 1, 0);
    InterpolateRowLine(previous, size - 1, 1);
    InterpolateColumnLine(previous, size - 1, 0);
    InterpolateColumnLine(previous, size - 1, 1);
    return true;
}

// fills a row line between two of its points (the step changes by deltaX per module)
// 0x0016CE68
void InterpolateRowLine(s32 from, s32 to, s32 line)
{
    Yield();
    if (s_SampleInfo.version == -1)
    {
        return;
    }
    s32 size = s_SampleInfo.version * 4 + 17;
    if (size == -1)
    {
        return;
    }
    s32 num = to - from;
    Point2d* start = &s_SampleInfo.rowLines[size * line + from];
    const Point2d& end = s_SampleInfo.rowLines[size * line + to];
    s32 triangle = num * (num - 1) / 2;
    f64 stepX = (end.x - start->x - s_SampleSteps.deltaX.x * triangle) / num;
    f64 stepY = (end.y - start->y - s_SampleSteps.deltaX.y * triangle) / num;
    for (s32 i = 1; i < num; i++)
    {
        start[i].x = start->x + i * stepX + s_SampleSteps.deltaX.x * (i * (i - 1) / 2);
        start[i].y = start->y + i * stepY + s_SampleSteps.deltaX.y * (i * (i - 1) / 2);
    }
}

// 0x0016CFA0
void InterpolateColumnLine(s32 from, s32 to, s32 line)
{
    Yield();
    if (s_SampleInfo.version == -1)
    {
        return;
    }
    s32 size = s_SampleInfo.version * 4 + 17;
    if (size == -1)
    {
        return;
    }
    s32 num = to - from;
    Point2d* start = &s_SampleInfo.columnLines[size * line + from];
    const Point2d& end = s_SampleInfo.columnLines[size * line + to];
    s32 triangle = num * (num - 1) / 2;
    f64 stepX = (end.x - start->x - s_SampleSteps.deltaY.x * triangle) / num;
    f64 stepY = (end.y - start->y - s_SampleSteps.deltaY.y * triangle) / num;
    for (s32 i = 1; i < num; i++)
    {
        start[i].x = start->x + i * stepX + s_SampleSteps.deltaY.x * (i * (i - 1) / 2);
        start[i].y = start->y + i * stepY + s_SampleSteps.deltaY.y * (i * (i - 1) / 2);
    }
}

// continues a row line from the points at from and to up to target
// 0x00173610
void ExtrapolateRowLine(s32 line, s32 from, s32 to, s32 target)
{
    if (s_SampleInfo.version == -1)
    {
        return;
    }
    s32 size = s_SampleInfo.version * 4 + 17;
    if (size == -1)
    {
        return;
    }
    s32 num = target - to;
    s32 triangle = num * (num + 1) / 2;
    const Point2d& a = s_SampleInfo.rowLines[line * size + from];
    const Point2d& b = s_SampleInfo.rowLines[line * size + to];
    Point2d& out = s_SampleInfo.rowLines[line * size + target];
    s32 distance = to - from;
    f64 stepY = (b.y - a.y) / distance;
    out.y = stepY * num + triangle * s_SampleSteps.deltaX.y + b.y;
    f64 stepX = (b.x - a.x) / distance;
    out.x = stepX * num + triangle * s_SampleSteps.deltaX.x + b.x;
}

// 0x001736EC
void ExtrapolateColumnLine(s32 line, s32 from, s32 to, s32 target)
{
    if (s_SampleInfo.version == -1)
    {
        return;
    }
    s32 size = s_SampleInfo.version * 4 + 17;
    if (size == -1)
    {
        return;
    }
    s32 num = target - to;
    s32 triangle = num * (num + 1) / 2;
    const Point2d& a = s_SampleInfo.columnLines[line * size + from];
    const Point2d& b = s_SampleInfo.columnLines[line * size + to];
    Point2d& out = s_SampleInfo.columnLines[line * size + target];
    s32 distance = to - from;
    f64 stepY = (b.y - a.y) / distance;
    out.y = stepY * num + triangle * s_SampleSteps.deltaY.y + b.y;
    f64 stepX = (b.x - a.x) / distance;
    out.x = stepX * num + triangle * s_SampleSteps.deltaY.x + b.x;
}

// moves an estimated alignment pattern centre onto the real one: searches in a spiral for a dark pixel with
// light pixels around it and centres it on its dark run
// 0x00189060
void RefineAlignment(Point2d* point)
{
    Point2d candidate;
    s32 i;

    Yield();
    for (i = 0; i < 121; i++)
    {
        s32 dx = s_SpiralOffsets[i][0];
        s32 dy = s_SpiralOffsets[i][1];
        candidate.x = point->x + (s_SampleSteps.stepX.x * dx + dy * s_SampleSteps.stepY.x) * 0.3;
        candidate.y = point->y + (s_SampleSteps.stepX.y * dx + dy * s_SampleSteps.stepY.y) * 0.3;
        s32 x = candidate.x + 0.499999;
        s32 y = candidate.y + 0.499999;
        Yield();
        if (IsDark(x, y) != 1)
        {
            continue;
        }

        // light in every direction somewhere between 0.6 and 1.4 modules
        for (s32 a = -1; a <= 1; a++)
        {
            for (s32 b = -1; b <= 1; b++)
            {
                if ((a | b) == 0)
                {
                    continue;
                }
                f64 offsetX = s_SampleSteps.stepY.x * b + s_SampleSteps.stepX.x * a;
                f64 offsetY = s_SampleSteps.stepY.y * b + s_SampleSteps.stepX.y * a;
                if (IsDark(static_cast<s32>(candidate.x + offsetX + 0.499999),
                           static_cast<s32>(candidate.y + offsetY + 0.499999)) == 1 &&
                    IsDark(static_cast<s32>(candidate.x + offsetX * 1.4f + 0.499999),
                           static_cast<s32>(candidate.y + offsetY * 1.4f + 0.499999)) == 1 &&
                    IsDark(static_cast<s32>(candidate.x + offsetX * 0.6f + 0.499999),
                           static_cast<s32>(candidate.y + offsetY * 0.6f + 0.499999)) == 1)
                {
                    goto next;
                }
            }
        }
        break;
    next:;
    }
    if (i == 121)
    {
        return;
    }

    *point = candidate;
    for (s32 k = 0; k < 2; k++)
    {
        s32 x = point->x + 0.499999;
        s32 y = point->y + 0.499999;
        Yield();
        s32 left = x - 1;
        while (left > 0)
        {
            if (IsDark(left, y) != 1)
            {
                left++;
                break;
            }
            left--;
        }
        s32 right = x + 1;
        while (right < s_SampleInfo.width - 1 && IsDark(right, y) == 1)
        {
            right++;
        }
        point->x = left + (right - left) * 0.5;

        x = point->x + 0.499999;
        y = point->y + 0.499999;
        Yield();
        s32 top = y - 1;
        while (top > 0)
        {
            if (IsDark(x, top) != 1)
            {
                top++;
                break;
            }
            top--;
        }
        // (the original stops at the image width here too)
        s32 bottom = y + 1;
        while (bottom < s_SampleInfo.width - 1 && IsDark(x, bottom) == 1)
        {
            bottom++;
        }
        point->y = top + (bottom - top) * 0.5;
    }
}

// the grid lines through all alignment patterns (versions 7 and up)
// 0x0017B124
bool CorrectSampleLinesLarge()
{
    u8 p[8] = {6}; // 0x008BC4E4
    s32 num = s_AlignmentCentres[s_SampleInfo.version - 1][0] + 1;
    Point2f point;

    Yield();
    point.x = 0.0f;
    point.y = 0.0f;
    if (s_SampleInfo.version == -1)
    {
        return false;
    }
    s32 size = s_SampleInfo.version * 4 + 17;
    if (size == -1)
    {
        return false;
    }

    s32 count = s_AlignmentCentres[s_SampleInfo.version - 1][0];
    s32 k;
    if (count != 0)
    {
        std::memcpy(&p[1], &s_AlignmentCentres[s_SampleInfo.version - 1][1], count);
        if (count >= 2)
        {
            for (k = 2; k < num; k++)
            {
                s32 a = p[k - 1];
                IntersectInto(&RowLine(size, 2, a), &point, RowLine(size, 0, a), RowLine(size, 1, a),
                              ColumnLine(size, k - 2, p[1]), ColumnLine(size, k - 1, p[1]));
                RefineAlignment(&RowLine(size, 2, a));
                InterpolateRowLine(p[k - 2], p[k - 1], 2);
                ColumnLine(size, k, p[1]) = RowLine(size, 2, a);
                InterpolateColumnLine(p[0], p[1], k);
                ExtrapolateColumnLine(k, p[1], p[0], 0);
                InterpolateColumnLine(0, p[0], k);
                RowLine(size, 0, a) = ColumnLine(size, k, 0);
                InterpolateRowLine(p[k - 2], p[k - 1], 0);
            }
            InterpolateRowLine(p[k - 2], p[k - 1], 0);
            ExtrapolateRowLine(2, p[1], p[0], 0);
            InterpolateRowLine(0, p[0], 2);
            ColumnLine(size, 0, p[1]) = RowLine(size, 2, 0);
            InterpolateColumnLine(p[0], p[1], 0);
        }
    }

    if (num - 1 > 2)
    {
        for (k = 3; k < num; k++)
        {
            point.x = 0.0f;
            point.y = 0.0f;
            s32 a = p[k - 1];
            IntersectInto(&ColumnLine(size, 2, a), &point, ColumnLine(size, 0, a), ColumnLine(size, 1, a),
                          RowLine(size, k - 2, p[1]), RowLine(size, k - 1, p[1]));
            RefineAlignment(&ColumnLine(size, 2, a));
            InterpolateColumnLine(p[k - 2], p[k - 1], 2);
            RowLine(size, k, p[1]) = ColumnLine(size, 2, a);
            InterpolateRowLine(p[0], p[1], k);
            ExtrapolateRowLine(k, p[1], p[0], 0);
            InterpolateRowLine(0, p[0], k);
            ColumnLine(size, 0, a) = RowLine(size, k, 0);
            InterpolateColumnLine(p[k - 2], p[k - 1], 0);
        }
        InterpolateColumnLine(p[k - 2], p[k - 1], 0);
    }

    s32 m = 3;
    if (s_AlignmentCentres[s_SampleInfo.version - 1][0] > 2 && num > 3)
    {
        for (; m < num; m++)
        {
            for (k = 2; k < num - 1; k++)
            {
                point.x = 0.0f;
                point.y = 0.0f;
                IntersectInto(&RowLine(size, m, p[k]), &point, RowLine(size, m, p[k - 2]), RowLine(size, m, p[k - 1]),
                              ColumnLine(size, k + 1, p[m - 3]), ColumnLine(size, k + 1, p[m - 2]));
                RefineAlignment(&RowLine(size, m, p[k]));
                InterpolateRowLine(p[k - 1], p[k], m);
                ColumnLine(size, k + 1, p[m - 1]) = RowLine(size, m, p[k]);
                InterpolateColumnLine(p[m - 2], p[m - 1], k + 1);
            }
        }
    }

    ExtrapolateRowLine(2, p[m - 3], p[m - 2], p[m - 1]);
    RefineAlignment(&RowLine(size, 2, p[m - 1]));
    InterpolateRowLine(p[m - 2], p[m - 1], 2);
    IntersectInto(&RowLine(size, 1, p[m - 1]), &point, RowLine(size, 1, p[m - 2]), RowLine(size, 1, size - 1),
                  RowLine(size, 0, p[m - 1]), RowLine(size, 2, p[m - 1]));
    InterpolateRowLine(p[m - 2], p[m - 1], 1);
    InterpolateRowLine(p[m - 1], size - 1, 1);
    ColumnLine(size, m, 0) = RowLine(size, 0, p[m - 1]);
    ColumnLine(size, m, 6) = RowLine(size, 1, p[m - 1]);
    ColumnLine(size, m, p[1]) = RowLine(size, 2, p[m - 1]);
    InterpolateColumnLine(0, 6, m);
    InterpolateColumnLine(6, p[1], m);
    ExtrapolateColumnLine(2, p[m - 3], p[m - 2], p[m - 1]);
    RefineAlignment(&ColumnLine(size, 2, p[m - 1]));
    InterpolateColumnLine(p[m - 2], p[m - 1], 2);
    IntersectInto(&ColumnLine(size, 1, p[m - 1]), &point, ColumnLine(size, 1, p[m - 2]),
                  ColumnLine(size, 1, size - 1), ColumnLine(size, 0, p[m - 1]), ColumnLine(size, 2, p[m - 1]));
    InterpolateColumnLine(p[m - 2], p[m - 1], 1);
    InterpolateColumnLine(p[m - 1], size - 1, 1);
    RowLine(size, m, 0) = ColumnLine(size, 0, p[m - 1]);
    RowLine(size, m, 6) = ColumnLine(size, 1, p[m - 1]);
    RowLine(size, m, p[1]) = ColumnLine(size, 2, p[m - 1]);
    InterpolateRowLine(0, 6, m);
    InterpolateRowLine(6, p[1], m);

    if (s_AlignmentCentres[s_SampleInfo.version - 1][0] > 2 && num > 3)
    {
        for (s32 q = 3; q < num; q++)
        {
            point.x = 0.0f;
            point.y = 0.0f;
            IntersectInto(&RowLine(size, q, p[num - 1]), &point, ColumnLine(size, num - 2, p[q - 1]),
                          ColumnLine(size, num - 1, p[q - 1]), ColumnLine(size, num, p[q - 3]),
                          ColumnLine(size, num, p[q - 2]));
            RefineAlignment(&RowLine(size, q, p[num - 1]));
            InterpolateRowLine(p[num - 2], p[num - 1], q);
            ColumnLine(size, num, p[q - 1]) = RowLine(size, q, p[num - 1]);
            InterpolateColumnLine(p[q - 2], p[q - 1], num);
            IntersectInto(&ColumnLine(size, q, p[num - 1]), &point, RowLine(size, num - 2, p[q - 1]),
                          RowLine(size, num - 1, p[q - 1]), RowLine(size, num, p[q - 3]), RowLine(size, num, p[q - 2]));
            RefineAlignment(&ColumnLine(size, q, p[num - 1]));
            InterpolateColumnLine(p[num - 2], p[num - 1], q);
            RowLine(size, num, p[q - 1]) = ColumnLine(size, q, p[num - 1]);
            InterpolateRowLine(p[q - 2], p[q - 1], num);
        }
    }

    IntersectInto(&RowLine(size, num, p[num - 1]), &point, RowLine(size, num, p[num - 3]),
                  RowLine(size, num, p[num - 2]), ColumnLine(size, num, p[num - 3]), ColumnLine(size, num, p[num - 2]));
    RefineAlignment(&RowLine(size, num, p[num - 1]));
    InterpolateRowLine(p[num - 2], p[num - 1], num);
    ColumnLine(size, num, p[num - 1]) = RowLine(size, num, p[num - 1]);
    InterpolateColumnLine(p[num - 2], p[num - 1], num);
    ExtrapolateRowLine(2, p[num - 2], p[num - 1], size - 1);
    InterpolateRowLine(p[num - 1], size - 1, 2);
    IntersectInto(&RowLine(size, 1, size - 1), &point, RowLine(size, 1, p[num - 1]), RowLine(size, 1, size - 1),
                  RowLine(size, 0, size - 1), RowLine(size, 2, size - 1));
    InterpolateRowLine(p[num - 2], p[num - 1], 1);
    InterpolateRowLine(p[num - 1], size - 1, 1);
    ColumnLine(size, num + 1, 0) = RowLine(size, 0, size - 1);
    ColumnLine(size, num + 1, p[0]) = RowLine(size, 1, size - 1);
    ColumnLine(size, num + 1, p[1]) = RowLine(size, 2, size - 1);
    InterpolateColumnLine(0, p[0], num + 1);
    InterpolateColumnLine(p[0], p[1], num + 1);
    ExtrapolateColumnLine(2, p[num - 2], p[num - 1], size - 1);
    InterpolateColumnLine(p[num - 1], size - 1, 2);
    IntersectInto(&ColumnLine(size, 1, size - 1), &point, ColumnLine(size, 1, p[num - 1]),
                  ColumnLine(size, 1, size - 1), ColumnLine(size, 0, size - 1), ColumnLine(size, 2, size - 1));
    InterpolateColumnLine(p[num - 2], p[num - 1], 1);
    InterpolateColumnLine(p[num - 1], size - 1, 1);
    RowLine(size, num + 1, 0) = ColumnLine(size, 0, size - 1);
    RowLine(size, num + 1, p[0]) = ColumnLine(size, 1, size - 1);
    RowLine(size, num + 1, p[1]) = ColumnLine(size, 2, size - 1);
    InterpolateRowLine(0, p[0], num + 1);
    InterpolateRowLine(p[0], p[1], num + 1);

    if (num > 2 && num + 1 > 3)
    {
        for (s32 q = 3; q < num + 1; q++)
        {
            ExtrapolateRowLine(q, p[num - 2], p[num - 1], size - 1);
            InterpolateRowLine(p[num - 1], size - 1, q);
            ColumnLine(size, num + 1, p[q - 1]) = RowLine(size, q, size - 1);
            InterpolateColumnLine(p[q - 2], p[q - 1], num + 1);
            ExtrapolateColumnLine(q, p[num - 2], p[num - 1], size - 1);
            InterpolateColumnLine(p[num - 1], size - 1, q);
            RowLine(size, num + 1, p[q - 1]) = ColumnLine(size, q, size - 1);
            InterpolateRowLine(p[q - 2], p[q - 1], num + 1);
        }
    }

    IntersectInto(&RowLine(size, num + 1, size - 1), &point, RowLine(size, num + 1, p[num - 2]),
                  RowLine(size, num + 1, p[num - 1]), ColumnLine(size, num + 1, p[num - 2]),
                  ColumnLine(size, num + 1, p[num - 1]));
    ColumnLine(size, num + 1, size - 1).x = point.x;
    ColumnLine(size, num + 1, size - 1).y = point.y;
    InterpolateRowLine(p[num - 1], size - 1, num + 1);
    InterpolateColumnLine(p[num - 1], size - 1, num + 1);
    return true;
}

// the grid lines through the single alignment pattern (versions 2 to 6)
// 0x0017CB0C
bool CorrectSampleLinesSmall()
{
    Point2f point;

    Yield();
    point.x = 0.0f;
    point.y = 0.0f;
    if (s_SampleInfo.version == -1)
    {
        return false;
    }
    s32 size = s_SampleInfo.version * 4 + 17;
    if (size == -1)
    {
        return false;
    }

    s32 position = s_AlignmentCentres[s_SampleInfo.version - 1][1];
    IntersectInto(&RowLine(size, 2, position), &point, ColumnLine(size, 0, position), ColumnLine(size, 1, position),
                  RowLine(size, 0, position), RowLine(size, 1, position));
    RefineAlignment(&RowLine(size, 2, position));
    ColumnLine(size, 2, position) = RowLine(size, 2, position);

    IntersectInto(&RowLine(size, 1, position), &point, RowLine(size, 1, 6), RowLine(size, 1, size - 1),
                  RowLine(size, 0, position), RowLine(size, 2, position));
    ColumnLine(size, 2, 6).x = RowLine(size, 1, position).x;
    ColumnLine(size, 2, 6).y = RowLine(size, 1, position).y;
    ColumnLine(size, 2, 0) = RowLine(size, 0, position);
    InterpolateColumnLine(0, 6, 2);
    InterpolateColumnLine(6, position, 2);
    InterpolateRowLine(6, position, 1);
    InterpolateRowLine(position, size - 1, 1);

    IntersectInto(&ColumnLine(size, 1, position), &point, ColumnLine(size, 1, 6), ColumnLine(size, 1, size - 1),
                  ColumnLine(size, 0, position), ColumnLine(size, 2, position));
    RowLine(size, 2, 6).x = ColumnLine(size, 1, position).x;
    RowLine(size, 2, 6).y = ColumnLine(size, 1, position).y;
    RowLine(size, 2, 0) = ColumnLine(size, 0, position);
    InterpolateRowLine(0, 6, 2);
    InterpolateRowLine(6, position, 2);
    InterpolateColumnLine(6, position, 1);
    InterpolateColumnLine(position, size - 1, 1);
    ExtrapolateRowLine(2, 6, position, size - 1);
    InterpolateRowLine(position, size - 1, 2);

    ColumnLine(size, 3, 0) = RowLine(size, 0, size - 1);
    ColumnLine(size, 3, 6) = RowLine(size, 1, size - 1);
    ColumnLine(size, 3, position) = RowLine(size, 2, size - 1);
    InterpolateColumnLine(0, 6, 3);
    InterpolateColumnLine(6, position, 3);
    ExtrapolateColumnLine(2, 6, position, size - 1);
    InterpolateColumnLine(position, size - 1, 2);
    RowLine(size, 3, 0) = ColumnLine(size, 0, size - 1);
    RowLine(size, 3, 6) = ColumnLine(size, 1, size - 1);
    RowLine(size, 3, position) = ColumnLine(size, 2, size - 1);
    InterpolateRowLine(0, 6, 3);
    InterpolateRowLine(6, position, 3);

    IntersectInto(&RowLine(size, 3, size - 1), &point, RowLine(size, 3, 6), RowLine(size, 3, position),
                  ColumnLine(size, 3, 6), ColumnLine(size, 3, position));
    ColumnLine(size, 3, size - 1) = RowLine(size, 3, size - 1);
    InterpolateRowLine(position, size - 1, 3);
    InterpolateColumnLine(position, size - 1, 3);
    return true;
}

// the intersection of the line through a1 and a2 with the line through b1 and b2
// 0x00169F70
bool IntersectLines(Point2f* out, Point2f a1, Point2f a2, Point2f b1, Point2f b2)
{
    f32 slopeA = 0.0f;
    f32 slopeB = 0.0f;
    f32 offsetB = 0.0f;
    f32 offsetA = 0.0f;
    out->y = -1.0f;
    out->x = -1.0f;
    if (a1.x == a2.x && b1.x == b2.x)
    {
        return false;
    }
    if (a1.y == a2.y && b1.y == b2.y)
    {
        return false;
    }
    if (a1.x != a2.x)
    {
        slopeA = (a2.y - a1.y) / (a2.x - a1.x);
        offsetA = a1.y - a1.x * slopeA;
        if (b1.x == b2.x)
        {
            out->x = b1.x;
            out->y = offsetA + slopeA * b1.x;
            return true;
        }
    }
    if (b1.x != b2.x)
    {
        slopeB = (b2.y - b1.y) / (b2.x - b1.x);
        offsetB = b1.y - b1.x * slopeB;
        if (a1.x == a2.x)
        {
            out->x = a1.x;
            out->y = offsetB + slopeB * a1.x;
            return true;
        }
    }
    if (slopeA == slopeB)
    {
        return false;
    }
    out->x = (offsetB - offsetA) / (slopeA - slopeB);
    if (std::fabs(slopeA) > std::fabs(slopeB))
    {
        out->y = offsetB + slopeB * out->x;
    }
    else
    {
        out->y = offsetA + slopeA * out->x;
    }
    return true;
}

// walks from the centre of a finder pattern along the angle over its dark centre, the light ring and the
// dark ring and returns the first light pixel outside (or the last dark pixel when going backwards)
// 0x00169298
bool FindEdge(Point2i* out, Point2f centre, f32 angle)
{
    const f64 ROUND = 0.499999;
    s32 x = centre.x + ROUND;
    s32 y = centre.y + ROUND;
    f64 cosine = std::cos(angle);
    Yield();

    if (std::fabs(cosine) < 1e-5)
    {
        // vertical
        if (std::sin(angle) > 0.0)
        {
            s32 i = y - 1;
            if (i < 0)
            {
                return false;
            }
            while (IsDark(x, i))
            {
                if (--i < 0)
                {
                    return false;
                }
            }
            if (i <= 0)
            {
                return false;
            }
            do
            {
                if (--i < 0)
                {
                    return false;
                }
            } while (IsDark(x, i) != 1);
            if (i < 0)
            {
                return false;
            }
            while (--i >= 0 && IsDark(x, i))
            {
            }
            out->x = x;
            out->y = i + 1;
        }
        else
        {
            s32 i = y + 1;
            while (i < s_SampleInfo.height && IsDark(x, i))
            {
                i++;
            }
            if (i >= s_SampleInfo.height - 1)
            {
                return false;
            }
            i++;
            while (i < s_SampleInfo.height && IsDark(x, i) != 1)
            {
                i++;
            }
            if (i > s_SampleInfo.height - 1)
            {
                return false;
            }
            i++;
            while (i < s_SampleInfo.height && IsDark(x, i))
            {
                i++;
            }
            out->y = i;
            out->x = x;
        }
    }
    else
    {
        f64 tangent = std::tan(angle);
        if (static_cast<s32>(std::fabs(tangent) * 10000.0 + ROUND) > 10000)
        {
            // steep: x = b + k * y
            f64 k = -1.0 / tangent;
            f64 b = centre.x - centre.y * k;
            if (std::sin(angle) > 0.0f)
            {
                s32 i = y - 1;
                if (i <= 0)
                {
                    return false;
                }
                for (;;)
                {
                    s32 xx = b + k * i + ROUND;
                    if (!(xx > 0 && s_SampleInfo.width > xx))
                    {
                        return false;
                    }
                    if (!IsDark(xx, i))
                    {
                        break;
                    }
                    if (--i <= 0)
                    {
                        return false;
                    }
                }
                if (i <= 0 || --i <= 0)
                {
                    return false;
                }
                for (;;)
                {
                    s32 xx = b + k * i + ROUND;
                    if (!(xx > 0 && s_SampleInfo.width > xx))
                    {
                        return false;
                    }
                    if (IsDark(xx, i) == 1)
                    {
                        break;
                    }
                    if (--i <= 0)
                    {
                        return false;
                    }
                }
                if (i <= 0 || --i <= 0)
                {
                    return false;
                }
                for (;;)
                {
                    s32 xx = b + k * i + ROUND;
                    if (!(xx > 0 && s_SampleInfo.width > xx))
                    {
                        return false;
                    }
                    if (!IsDark(xx, i))
                    {
                        break;
                    }
                    if (--i <= 0)
                    {
                        return false;
                    }
                }
                if (i <= 0)
                {
                    return false;
                }
                out->x = b + k * (i + 1) + ROUND;
                out->y = i + 1;
            }
            else
            {
                s32 i = y + 1;
                for (; i < s_SampleInfo.height; i++)
                {
                    s32 xx = b + k * i + ROUND;
                    if (!(xx > 0 && s_SampleInfo.width > xx))
                    {
                        return false;
                    }
                    if (!IsDark(xx, i))
                    {
                        break;
                    }
                }
                if (s_SampleInfo.height <= i)
                {
                    return false;
                }
                for (i++; i < s_SampleInfo.height; i++)
                {
                    s32 xx = b + k * i + ROUND;
                    if (!(xx > 0 && s_SampleInfo.width > xx))
                    {
                        return false;
                    }
                    if (IsDark(xx, i) == 1)
                    {
                        break;
                    }
                }
                if (s_SampleInfo.height <= i)
                {
                    return false;
                }
                for (i++; i < s_SampleInfo.height; i++)
                {
                    s32 xx = b + k * i + ROUND;
                    if (!(xx > 0 && s_SampleInfo.width > xx))
                    {
                        return false;
                    }
                    if (!IsDark(xx, i))
                    {
                        break;
                    }
                }
                if (s_SampleInfo.height <= i)
                {
                    return false;
                }
                out->x = b + k * (i - 1) + ROUND;
                out->y = i - 1;
            }
        }
        else
        {
            // flat: y = b + k * x
            f64 k = -tangent;
            f64 b = centre.y - centre.x * k;
            if (0.0 > cosine)
            {
                s32 i = x - 1;
                if (i <= 0)
                {
                    return false;
                }
                for (;;)
                {
                    s32 yy = b + k * i + ROUND;
                    if (!(yy > 0 && s_SampleInfo.height > yy))
                    {
                        return false;
                    }
                    if (!IsDark(i, yy))
                    {
                        break;
                    }
                    if (--i <= 0)
                    {
                        return false;
                    }
                }
                if (i <= 0 || --i <= 0)
                {
                    return false;
                }
                for (;;)
                {
                    s32 yy = b + k * i + ROUND;
                    if (!(yy > 0 && s_SampleInfo.height > yy))
                    {
                        return false;
                    }
                    if (IsDark(i, yy) == 1)
                    {
                        break;
                    }
                    if (--i <= 0)
                    {
                        return false;
                    }
                }
                if (i <= 0 || --i <= 0)
                {
                    return false;
                }
                for (;;)
                {
                    s32 yy = b + k * i + ROUND;
                    if (!(yy > 0 && s_SampleInfo.height > yy))
                    {
                        return false;
                    }
                    if (!IsDark(i, yy))
                    {
                        break;
                    }
                    if (--i <= 0)
                    {
                        return false;
                    }
                }
                if (i <= 0)
                {
                    return false;
                }
                out->x = i + 1;
                out->y = b + k * (i + 1) + ROUND;
            }
            else
            {
                s32 i = x + 1;
                s32 yy = 0;
                for (; i < s_SampleInfo.width; i++)
                {
                    yy = b + k * i + ROUND;
                    if (!(yy > 0 && s_SampleInfo.height > yy))
                    {
                        return false;
                    }
                    if (!IsDark(i, yy))
                    {
                        break;
                    }
                }
                if (s_SampleInfo.width <= i)
                {
                    return false;
                }
                for (i++; i < s_SampleInfo.width; i++)
                {
                    yy = b + k * i + ROUND;
                    if (!(yy > 0 && s_SampleInfo.height > yy))
                    {
                        return false;
                    }
                    if (IsDark(i, yy) == 1)
                    {
                        break;
                    }
                }
                if (s_SampleInfo.width <= i)
                {
                    return false;
                }
                for (i++; i < s_SampleInfo.width; i++)
                {
                    yy = b + k * i + ROUND;
                    if (!(yy > 0 && s_SampleInfo.height > yy))
                    {
                        return false;
                    }
                    if (!IsDark(i, yy))
                    {
                        break;
                    }
                }
                if (s_SampleInfo.width <= i)
                {
                    return false;
                }
                out->y = yy;
                out->x = i;
            }
        }
    }

    return out->x >= 0 && s_SampleInfo.width > out->x && out->y >= 0 && s_SampleInfo.height > out->y;
}

// the angle of the line from a to b (radians, 0 to 2 pi)
inline f32 LineAngle(Point2f a, Point2f b)
{
    f32 angle = 0.0f;
    if (a.y != b.y)
    {
        f32 t = (b.x - a.x) / (b.y - a.y);
        angle = std::asin(std::sqrt(1.0f / (1.0f + t * t)));
    }
    f32 degree = angle * 57.2957795f;
    if (a.x < b.x)
    {
        if (a.y < b.y)
        {
            degree = 360.0f - degree;
        }
    }
    else if (a.y >= b.y)
    {
        degree = 180.0f - degree;
    }
    else
    {
        degree = 180.0f + degree;
    }
    return degree * 0.0174532924f;
}

inline f32 EdgeDistance(Point2f a, Point2f b)
{
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

inline Point2f ToPoint2f(Point2i point)
{
    Point2f result;
    result.x = point.x;
    result.y = point.y;
    return result;
}

// the version from the distance between two finder pattern centres (4 * version + 10 modules)
inline s32 VersionFromDistance(f32 distance, f32 width0, f32 width1)
{
    f32 moduleSize = (width0 + width1) * (1.0f / 14.0f);
    return (distance / moduleSize - 10.0f) * 0.25f + 0.499999;
}

// the outer corners of the three finder patterns from the edges along both symbol axes
// 0x0017F6C8
bool LocateCorners()
{
    FinderArea* areas = s_FinderAreas;
    Point2i pixel;
    Point2f point;

    Yield();
    pixel.x = 0;
    pixel.y = 0;
    point.x = 0.0f;
    point.y = 0.0f;

    // the axis from top left to top right
    f32 angleH = LineAngle(areas[0].centre, areas[1].centre);
    f32 backH = angleH + 3.14159274f;
    if (!FindEdge(&areas[0].edgePixels[0], areas[0].centre, backH))
    {
        return false;
    }
    areas[0].edges[0] = ToPoint2f(areas[0].edgePixels[0]);
    if (!FindEdge(&areas[0].edgePixels[1], areas[0].centre, angleH))
    {
        return false;
    }
    areas[0].edges[1] = ToPoint2f(areas[0].edgePixels[1]);
    areas[0].widths[0] = EdgeDistance(areas[0].edges[0], areas[0].edges[1]);
    if (!FindEdge(&areas[1].edgePixels[0], areas[1].centre, backH))
    {
        return false;
    }
    areas[1].edges[0] = ToPoint2f(areas[1].edgePixels[0]);
    if (!FindEdge(&areas[1].edgePixels[1], areas[1].centre, angleH))
    {
        return false;
    }
    areas[1].edges[1] = ToPoint2f(areas[1].edgePixels[1]);
    areas[1].widths[0] = EdgeDistance(areas[1].edges[0], areas[1].edges[1]);
    f32 distanceH = EdgeDistance(areas[0].centre, areas[1].centre);
    s_SampleInfo.versionH = VersionFromDistance(distanceH, areas[0].widths[0], areas[1].widths[0]);
    if (static_cast<u32>(s_SampleInfo.versionH) >= 45)
    {
        return false;
    }

    // the axis from top left to bottom left, nearly at a right angle
    f32 angleV = LineAngle(areas[0].centre, areas[2].centre);
    f32 backV = angleV + 3.14159274f;
    if (std::sin(angleH - angleV) <= 0.866025388f)
    {
        return false;
    }
    if (!FindEdge(&areas[0].edgePixels[2], areas[0].centre, backV))
    {
        return false;
    }
    areas[0].edges[2] = ToPoint2f(areas[0].edgePixels[2]);
    if (!FindEdge(&areas[0].edgePixels[3], areas[0].centre, angleV))
    {
        return false;
    }
    areas[0].edges[3] = ToPoint2f(areas[0].edgePixels[3]);
    areas[0].widths[1] = EdgeDistance(areas[0].edges[2], areas[0].edges[3]);
    if (!FindEdge(&areas[2].edgePixels[2], areas[2].centre, backV))
    {
        return false;
    }
    areas[2].edges[2] = ToPoint2f(areas[2].edgePixels[2]);
    if (!FindEdge(&areas[2].edgePixels[3], areas[2].centre, angleV))
    {
        return false;
    }
    areas[2].edges[3] = ToPoint2f(areas[2].edgePixels[3]);
    areas[2].widths[1] = EdgeDistance(areas[2].edges[2], areas[2].edges[3]);
    f32 distanceV = EdgeDistance(areas[0].centre, areas[2].centre);
    f32 ratio = distanceH > distanceV ? distanceH / distanceV : distanceV / distanceH;
    if (ratio > 2.0f)
    {
        return false;
    }
    s_SampleInfo.versionV = VersionFromDistance(distanceV, areas[0].widths[1], areas[2].widths[1]);
    if (static_cast<u32>(s_SampleInfo.versionV) >= 45)
    {
        return false;
    }
    if (s_SampleInfo.versionH > 6 && s_SampleInfo.versionV > 6)
    {
        // (the original takes the absolute value of the comparison)
        if (std::fabs(static_cast<f32>(s_SampleInfo.versionH - s_SampleInfo.versionV > 4)) != 0.0f)
        {
            return false;
        }
    }
    else if (s_SampleInfo.versionH != s_SampleInfo.versionV)
    {
        return false;
    }

    // the remaining edges
    if (!FindEdge(&areas[1].edgePixels[2], areas[1].centre, backV))
    {
        return false;
    }
    areas[1].edges[2] = ToPoint2f(areas[1].edgePixels[2]);
    if (!FindEdge(&areas[1].edgePixels[3], areas[1].centre, angleV))
    {
        return false;
    }
    areas[1].edges[3] = ToPoint2f(areas[1].edgePixels[3]);
    areas[1].widths[1] = EdgeDistance(areas[1].edges[2], areas[1].edges[3]);
    if (!FindEdge(&areas[2].edgePixels[0], areas[2].centre, backH))
    {
        return false;
    }
    areas[2].edges[0] = ToPoint2f(areas[2].edgePixels[0]);
    if (!FindEdge(&areas[2].edgePixels[1], areas[2].centre, angleH))
    {
        return false;
    }
    areas[2].edges[1] = ToPoint2f(areas[2].edgePixels[1]);
    areas[2].widths[0] = EdgeDistance(areas[2].edges[0], areas[2].edges[1]);

    // the corners of the top left pattern: where its edge lines cross
    if (!IntersectLines(&areas[0].corners[0], areas[0].edges[2], areas[1].edges[2], areas[0].edges[0],
                        areas[2].edges[0]) ||
        !IntersectLines(&areas[0].corners[1], areas[0].edges[2], areas[1].edges[2], areas[0].edges[1],
                        areas[2].edges[1]) ||
        !IntersectLines(&areas[0].corners[3], areas[0].edges[3], areas[1].edges[3], areas[0].edges[1],
                        areas[2].edges[1]) ||
        !IntersectLines(&areas[0].corners[2], areas[0].edges[3], areas[1].edges[3], areas[0].edges[0],
                        areas[2].edges[0]))
    {
        return false;
    }

    // the outer corners of the other two by following their contours
    TraceCornerA(areas[1].edgePixels[0], &pixel, areas[1].centre);
    areas[1].corners[0] = ToPoint2f(pixel);
    TraceCornerB(areas[1].edgePixels[0], &pixel, areas[1].centre);
    areas[1].corners[2] = ToPoint2f(pixel);
    if (!IntersectLines(&point, areas[0].edges[2], areas[1].edges[2], areas[1].corners[0], areas[1].corners[2]))
    {
        return false;
    }
    areas[1].corners[0] = point;
    if (!IntersectLines(&point, areas[0].edges[3], areas[1].edges[3], areas[1].corners[0], areas[1].corners[2]))
    {
        return false;
    }
    areas[1].corners[2] = point;
    TraceCornerA(areas[1].edgePixels[1], &pixel, areas[1].centre);
    areas[1].corners[3] = ToPoint2f(pixel);
    TraceCornerB(areas[1].edgePixels[1], &pixel, areas[1].centre);
    areas[1].corners[1] = ToPoint2f(pixel);
    if (!IntersectLines(&point, areas[0].edges[2], areas[1].edges[2], areas[1].corners[1], areas[1].corners[3]))
    {
        return false;
    }
    areas[1].corners[1] = point;
    if (!IntersectLines(&point, areas[0].edges[3], areas[1].edges[3], areas[1].corners[1], areas[1].corners[3]))
    {
        return false;
    }
    areas[1].corners[3] = point;

    TraceCornerA(areas[2].edgePixels[2], &pixel, areas[2].centre);
    areas[2].corners[1] = ToPoint2f(pixel);
    TraceCornerB(areas[2].edgePixels[2], &pixel, areas[2].centre);
    areas[2].corners[0] = ToPoint2f(pixel);
    if (!IntersectLines(&point, areas[0].edges[0], areas[2].edges[0], areas[2].corners[0], areas[2].corners[1]))
    {
        return false;
    }
    areas[2].corners[0] = point;
    if (!IntersectLines(&point, areas[0].edges[1], areas[2].edges[1], areas[2].corners[0], areas[2].corners[1]))
    {
        return false;
    }
    areas[2].corners[1] = point;
    TraceCornerA(areas[2].edgePixels[3], &pixel, areas[2].centre);
    areas[2].corners[2] = ToPoint2f(pixel);
    TraceCornerB(areas[2].edgePixels[3], &pixel, areas[2].centre);
    areas[2].corners[3] = ToPoint2f(pixel);
    if (!IntersectLines(&point, areas[0].edges[0], areas[2].edges[0], areas[2].corners[2], areas[2].corners[3]))
    {
        return false;
    }
    areas[2].corners[2] = point;
    if (!IntersectLines(&point, areas[0].edges[1], areas[2].edges[1], areas[2].corners[2], areas[2].corners[3]))
    {
        return false;
    }
    areas[2].corners[3] = point;
    return true;
}

// follows the dark contour from an edge pixel to the corner (two variants that turn the other way)
// 0x00181CA4
void TraceCornerA(Point2i start, Point2i* out, Point2f centre)
{
    Yield();
    s32 x = start.x;
    s32 y = start.y;
    if (std::fabs(static_cast<f32>(start.x) - centre.x) >= std::fabs(static_cast<f32>(start.y) - centre.y))
    {
        if (static_cast<f32>(start.x) > centre.x)
        {
            if (IsDark(x, y))
            {
                x--;
            }
            while (y < s_SampleInfo.height - 1)
            {
                if (IsDark(x, y + 1) == 1)
                {
                    y++;
                    if (IsDark(x + 1, y) == 1)
                    {
                        x++;
                    }
                    if (IsDark(x + 1, y) == 1)
                    {
                        x++;
                    }
                    continue;
                }
                if (x > start.x)
                {
                    break;
                }
                if (IsDark(x - 1, y + 1) == 1)
                {
                    x--;
                }
                else if (IsDark(x - 2, y + 1) == 1)
                {
                    x -= 2;
                }
                else
                {
                    break;
                }
                y++;
            }
            x++;
        }
        else
        {
            if (!IsDark(x, y))
            {
                x++;
            }
            while (y > 0)
            {
                if (IsDark(x, y - 1) == 1)
                {
                    y--;
                    if (IsDark(x - 1, y) == 1)
                    {
                        x--;
                    }
                    if (IsDark(x - 1, y) == 1)
                    {
                        x--;
                    }
                    continue;
                }
                if (start.x > x)
                {
                    break;
                }
                if (IsDark(x + 1, y - 1) == 1)
                {
                    x++;
                }
                else if (IsDark(x + 2, y - 1) == 1)
                {
                    x += 2;
                }
                else
                {
                    break;
                }
                y--;
            }
        }
    }
    else if (static_cast<f32>(start.y) > centre.y)
    {
        if (!IsDark(x, y))
        {
            y--;
        }
        while (x > 0)
        {
            if (IsDark(x - 1, y) == 1)
            {
                x--;
                if (IsDark(x - 1, y + 1) == 1)
                {
                    y++;
                }
                if (IsDark(x - 1, y + 1) == 1)
                {
                    y++;
                }
                continue;
            }
            if (y > start.y)
            {
                break;
            }
            if (IsDark(x - 1, y - 1) == 1)
            {
                y--;
            }
            else if (IsDark(x - 1, y - 2) == 1)
            {
                y -= 2;
            }
            else
            {
                break;
            }
            x--;
        }
        y++;
    }
    else
    {
        if (!IsDark(x, y))
        {
            y++;
        }
        while (x < s_SampleInfo.width - 1)
        {
            if (IsDark(x + 1, y) == 1)
            {
                x++;
                if (IsDark(x + 1, y - 1) == 1)
                {
                    y--;
                }
                if (IsDark(x + 1, y - 1) == 1)
                {
                    y--;
                }
                continue;
            }
            if (start.y > y)
            {
                break;
            }
            if (IsDark(x + 1, y + 1) == 1)
            {
                y++;
            }
            else if (IsDark(x + 1, y + 2) == 1)
            {
                y += 2;
            }
            else
            {
                break;
            }
            x++;
        }
    }
    out->y = y;
    out->x = x;
}

// 0x00183874
void TraceCornerB(Point2i start, Point2i* out, Point2f centre)
{
    Yield();
    s32 x = start.x;
    s32 y = start.y;
    if (std::fabs(static_cast<f32>(x) - centre.x) >= std::fabs(static_cast<f32>(y) - centre.y))
    {
        if (static_cast<f32>(start.x) > centre.x)
        {
            if (!IsDark(x, y))
            {
                x--;
            }
            while (y > 0)
            {
                if (IsDark(x, y - 1) == 1)
                {
                    y--;
                    if (IsDark(x + 1, y) == 1)
                    {
                        x++;
                    }
                    if (IsDark(x + 1, y) == 1)
                    {
                        x++;
                    }
                    continue;
                }
                if (x > start.x)
                {
                    break;
                }
                if (IsDark(x - 1, y - 1) == 1)
                {
                    x--;
                }
                else if (IsDark(x - 2, y - 1) == 1)
                {
                    x -= 2;
                }
                else
                {
                    break;
                }
                y--;
            }
            x++;
        }
        else
        {
            if (!IsDark(x, y))
            {
                x++;
            }
            while (y < s_SampleInfo.height - 1)
            {
                if (IsDark(x, y + 1) == 1)
                {
                    y++;
                    if (IsDark(x - 1, y) == 1)
                    {
                        x--;
                    }
                    if (IsDark(x - 1, y) == 1)
                    {
                        x--;
                    }
                    continue;
                }
                if (start.x > x)
                {
                    break;
                }
                if (IsDark(x + 1, y + 1) == 1)
                {
                    x++;
                }
                else if (IsDark(x + 2, y + 1) == 1)
                {
                    x += 2;
                }
                else
                {
                    break;
                }
                y++;
            }
        }
    }
    else if (static_cast<f32>(start.y) > centre.y)
    {
        if (!IsDark(x, y))
        {
            y--;
        }
        while (x < s_SampleInfo.width - 1)
        {
            if (IsDark(x + 1, y) == 1)
            {
                x++;
                if (IsDark(x + 1, y + 1) == 1)
                {
                    y++;
                }
                if (IsDark(x + 1, y + 1) == 1)
                {
                    y++;
                }
                continue;
            }
            if (y > start.y)
            {
                break;
            }
            if (IsDark(x + 1, y - 1) == 1)
            {
                y--;
            }
            else if (IsDark(x + 1, y - 2) == 1)
            {
                y -= 2;
            }
            else
            {
                break;
            }
            x++;
        }
        y++;
    }
    else
    {
        if (!IsDark(x, y))
        {
            y++;
        }
        while (x > 0)
        {
            if (IsDark(x - 1, y) == 1)
            {
                x--;
                if (IsDark(x - 1, y - 1) == 1)
                {
                    y--;
                }
                if (IsDark(x - 1, y - 1) == 1)
                {
                    y--;
                }
                continue;
            }
            if (start.y > y)
            {
                break;
            }
            if (IsDark(x - 1, y + 1) == 1)
            {
                y++;
            }
            else if (IsDark(x - 1, y + 2) == 1)
            {
                y += 2;
            }
            else
            {
                break;
            }
            x--;
        }
    }
    out->y = y;
    out->x = x;
}

// samples the 6x3 version information block next to a finder pattern (false when it leaves the image)
inline bool SampleVersionBlock(u32* bits, f64 centreX, f64 centreY, f64 rowX, f64 rowY, f64 columnX, f64 columnY)
{
    for (s32 row = -3; row < 3; row++)
    {
        f64 baseX = centreX + rowX * row;
        f64 baseY = centreY + rowY * row;
        for (s32 column = -7; column < -4; column++)
        {
            s32 x = baseX + columnX * column + 0.499999;
            if (!(x >= 0 && s_SampleInfo.width > x))
            {
                return false;
            }
            s32 y = baseY + columnY * column + 0.499999;
            if (!(y >= 0 && s_SampleInfo.height > y))
            {
                return false;
            }
            if (IsDark(x, y))
            {
                *bits |= 1 << (row * 3 + column + 16);
            }
        }
    }
    return true;
}

// the version whose BCH(18, 6) code differs in at most two bits, -1 if there is none
inline s32 MatchVersion(u32 bits)
{
    Yield();
    for (s32 version = 7; version <= 40; version++)
    {
        s32 code = version << 12;
        for (s32 i = 0; i < 6; i++)
        {
            if (code & (1 << (17 - i)))
            {
                code ^= 0x1F25 << (5 - i);
            }
        }
        code += version << 12;
        s32 errors = 0;
        for (s32 i = 0; i < 18; i++)
        {
            if ((bits ^ code) & (1 << i))
            {
                errors++;
            }
        }
        if (errors <= 2)
        {
            return code >> 12;
        }
    }
    return -1;
}

inline void FreeSampleLines()
{
    if (s_SampleInfo.rowLines != NULL)
    {
        Free(s_SampleInfo.rowLines);
        s_SampleInfo.rowLines = NULL;
    }
    if (s_SampleInfo.columnLines != NULL)
    {
        Free(s_SampleInfo.columnLines);
        s_SampleInfo.columnLines = NULL;
    }
}

// the second sampling: the module grid from the outer corners of the finder patterns, corrected at the
// alignment patterns
// 0x00177188
u8* SampleByPerspective()
{
    u8* modules = NULL;

    Yield();
    s_SampleInfo.version = -1;
    if (!LocateCorners())
    {
        return modules;
    }

    if (s_SampleInfo.versionH > 6 && s_SampleInfo.versionV > 6)
    {
        // read the version information next to the top right pattern, else next to the bottom left one
        const FinderArea& topRight = s_FinderAreas[1];
        u32 bits = 0;
        f64 rowX = (topRight.corners[2].x - topRight.corners[0].x) * (1.0f / 7.0f);
        f64 rowY = (topRight.corners[2].y - topRight.corners[0].y) * (1.0f / 7.0f);
        f64 columnX = (static_cast<f64>(topRight.corners[1].x - topRight.corners[0].x) +
                       (topRight.corners[3].x - topRight.corners[2].x)) *
                      (1.0 / 14.0);
        f64 columnY = (static_cast<f64>(topRight.corners[3].y - topRight.corners[2].y) +
                       (topRight.corners[1].y - topRight.corners[0].y)) *
                      (1.0 / 14.0);
        Yield();
        s_SampleInfo.version = -1;
        if (!SampleVersionBlock(&bits, topRight.centre.x, topRight.centre.y, rowX, rowY, columnX, columnY))
        {
            goto checkVersion;
        }
        s32 version = MatchVersion(bits);
        if (version != -1)
        {
            s_SampleInfo.version = version;
            goto checkVersion;
        }

        const FinderArea& bottomLeft = s_FinderAreas[2];
        bits = 0;
        rowX = (bottomLeft.corners[1].x - bottomLeft.corners[0].x) * (1.0f / 7.0f);
        rowY = (bottomLeft.corners[1].y - bottomLeft.corners[0].y) * (1.0f / 7.0f);
        columnX = (static_cast<f64>(bottomLeft.corners[2].x - bottomLeft.corners[0].x) +
                   (bottomLeft.corners[3].x - bottomLeft.corners[1].x)) *
                  (1.0 / 14.0);
        columnY = (static_cast<f64>(bottomLeft.corners[3].y - bottomLeft.corners[1].y) +
                   (bottomLeft.corners[2].y - bottomLeft.corners[0].y)) *
                  (1.0 / 14.0);
        if (!SampleVersionBlock(&bits, bottomLeft.centre.x, bottomLeft.centre.y, rowX, rowY, columnX, columnY))
        {
            goto checkVersion;
        }
        version = MatchVersion(bits);
        if (version != -1)
        {
            s_SampleInfo.version = version;
        }
    }
    else
    {
        s_SampleInfo.version = s_SampleInfo.versionV;
    }

checkVersion:
    // the version has to fit the distance of the finder patterns
    if (std::fabs(static_cast<f32>(s_SampleInfo.version - s_SampleInfo.versionH)) >= 5.0f)
    {
        s_SampleInfo.version = -1;
    }
    {
        s32 size = GetSampleSize();
        if (size == -1)
        {
            goto failure;
        }
        s32 between = size - 7;

        if (s_SampleInfo.rowLines != NULL)
        {
            Free(s_SampleInfo.rowLines);
        }
        if (s_SampleInfo.columnLines != NULL)
        {
            Free(s_SampleInfo.columnLines);
        }
        s_SampleInfo.rowLines = static_cast<Point2d*>(Allocate(size * 9 * sizeof(Point2d)));
        if (s_SampleInfo.rowLines == NULL)
        {
            goto failure;
        }
        s_SampleInfo.columnLines = static_cast<Point2d*>(Allocate(size * 9 * sizeof(Point2d)));
        if (s_SampleInfo.columnLines == NULL)
        {
            goto failure;
        }
        std::memset(s_SampleInfo.rowLines, 0, size * 9 * sizeof(Point2d));
        std::memset(s_SampleInfo.columnLines, 0, size * 9 * sizeof(Point2d));

        // module steps along the top and the left side and how they change over the symbol
        const FinderArea* areas = s_FinderAreas;
        f32 stepX0 = (areas[0].corners[1].x - areas[0].corners[0].x) * (1.0f / 7.0f);
        s_SampleSteps.deltaX.x =
            ((areas[1].corners[1].x - areas[1].corners[0].x) - (areas[0].corners[1].x - areas[0].corners[0].x)) *
            (1.0f / 7.0f) / static_cast<f32>(between);
        f32 stepY0 = (areas[0].corners[1].y - areas[0].corners[0].y) * (1.0f / 7.0f);
        s_SampleSteps.deltaX.y =
            ((areas[1].corners[1].y - areas[1].corners[0].y) - (areas[0].corners[1].y - areas[0].corners[0].y)) *
            (1.0f / 7.0f) / static_cast<f32>(between);
        f32 stepX1 = (areas[0].corners[2].x - areas[0].corners[0].x) * (1.0f / 7.0f);
        s_SampleSteps.deltaY.x =
            ((areas[2].corners[2].x - areas[2].corners[0].x) - (areas[0].corners[2].x - areas[0].corners[0].x)) *
            (1.0f / 7.0f) / static_cast<f32>(between);
        f32 stepY1 = (areas[0].corners[2].y - areas[0].corners[0].y) * (1.0f / 7.0f);
        s_SampleSteps.deltaY.y =
            ((areas[2].corners[2].y - areas[2].corners[0].y) - (areas[0].corners[2].y - areas[0].corners[0].y)) *
            (1.0f / 7.0f) / static_cast<f32>(between);
        s_SampleSteps.stepY.y = stepY1;
        s_SampleSteps.stepX.x = stepX0;
        s_SampleSteps.stepX.y = stepY0;
        s_SampleSteps.stepY.x = stepX1;

        if (!BuildSampleLines())
        {
            goto failure;
        }
        if (static_cast<u32>(s_SampleInfo.version) >= 7)
        {
            if (!CorrectSampleLinesLarge())
            {
                goto failure;
            }
        }
        else
        {
            if (static_cast<u32>(s_SampleInfo.version) <= 1 || !CorrectSampleLinesSmall())
            {
                goto failure;
            }
        }

        // the lines through the alignment patterns divide the symbol into cells
        u8 positions[12] = {0, 6, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF, 0xFF}; // 0x008BC6F8
        Yield();
        size = GetSampleSize();
        if (size == -1)
        {
            goto done;
        }
        modules = static_cast<u8*>(Allocate(size * size));
        if (modules == NULL)
        {
            goto done;
        }
        std::memset(modules, 0, size * size);

        const u8* alignment = s_AlignmentCentres[s_SampleInfo.version - 1];
        s32 lineNum = 2;
        if (alignment[0] != 0)
        {
            for (s32 i = 0; i < alignment[0]; i++)
            {
                positions[2 + i] = alignment[1 + i];
            }
            lineNum = s_AlignmentCentres[s_SampleInfo.version - 1][0] + 2;
        }
        positions[lineNum] = size - 1;

        for (s32 i = 0; i < lineNum; i++)
        {
            for (s32 j = 0; j < lineNum; j++)
            {
                for (s32 y = positions[i]; y <= positions[i + 1]; y++)
                {
                    for (s32 x = positions[j]; x <= positions[j + 1]; x++)
                    {
                        Point2f point;
                        point.x = 0.0f;
                        point.y = 0.0f;
                        const Point2d& a1 = s_SampleInfo.columnLines[size * j + y];
                        const Point2d& a2 = s_SampleInfo.columnLines[size * (j + 1) + y];
                        const Point2d& b1 = s_SampleInfo.rowLines[size * i + x];
                        const Point2d& b2 = s_SampleInfo.rowLines[size * (i + 1) + x];
                        IntersectLines(&point, ToPoint2f(a1), ToPoint2f(a2), ToPoint2f(b1), ToPoint2f(b2));
                        s32 px = point.x;
                        s32 py = point.y;
                        if (!(px >= 0 && s_SampleInfo.width > px && py >= 0 && s_SampleInfo.height > py &&
                              px >= 1 && s_SampleInfo.width - 1 > px && py >= 1 && s_SampleInfo.height - 1 > py))
                        {
                            continue;
                        }

                        // the majority of nine pixels around the module centre
                        s32 right = point.x + 0.35f;
                        s32 left = point.x - 0.35f;
                        s32 bottom = point.y + 0.35f;
                        s32 top = point.y - 0.35f;
                        s32 darkNum = 0;
                        if (IsDark(px, py) == 1)
                        {
                            darkNum = 1;
                        }
                        if (IsDark(right, py) == 1)
                        {
                            darkNum++;
                        }
                        if (IsDark(left, py) == 1)
                        {
                            darkNum++;
                        }
                        if (IsDark(px, bottom) == 1)
                        {
                            darkNum++;
                        }
                        if (IsDark(px, top) == 1)
                        {
                            darkNum++;
                        }
                        if (IsDark(right, bottom) == 1)
                        {
                            darkNum++;
                        }
                        if (IsDark(left, bottom) == 1)
                        {
                            darkNum++;
                        }
                        if (IsDark(right, top) == 1)
                        {
                            darkNum++;
                        }
                        if (IsDark(left, top) == 1)
                        {
                            darkNum++;
                        }
                        if (darkNum >= 5)
                        {
                            modules[y * size + x] = 0xFF;
                        }
                    }
                }
            }
        }
    }

done:
    FreeSampleLines();
    return modules;

failure:
    FreeSampleLines();
    return NULL;
}

} // namespace detail
} // namespace qrd
} // namespace mw
