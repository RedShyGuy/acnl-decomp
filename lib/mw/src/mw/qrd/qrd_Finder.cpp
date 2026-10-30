#include <cmath>
#include <cstdlib>
#include <cstring>

#include "mw/qrd/qrd_Internal.h"

namespace mw {
namespace qrd {
namespace detail {

// 0x00AF5B10
FinderPattern s_FinderPatterns[3];

namespace {

inline f32 Distance(const PatternPoint* a, const PatternPoint* b)
{
    f32 dx = a->x - b->x;
    f32 dy = a->y - b->y;
    return std::sqrt(dx * dx + dy * dy);
}

// the angle at a between b and c
inline f32 Angle(const PatternPoint* a, const PatternPoint* b, const PatternPoint* c)
{
    f32 bx = b->x - a->x;
    f32 by = b->y - a->y;
    f32 cx = c->x - a->x;
    f32 cy = c->y - a->y;
    return std::acos((bx * cx + by * cy) / (std::sqrt(bx * bx + by * by) * std::sqrt(cx * cx + cy * cy)));
}

// counts the dark and light runs through (x, y) to find a 1:1:3:1:1 pattern; returns the centre or -0.0f
inline f32 CrossCheckVertical(s32 x, s32 y, s32 maxCount, s32 height)
{
    s32 counts[5] = {};
    s32 i = y;

    while (i >= 0 && !IsLight(x, i))
    {
        counts[2]++;
        i--;
    }
    if (i < 0)
    {
        return -0.0f;
    }
    while (i >= 0 && IsLight(x, i) && counts[1] <= maxCount)
    {
        counts[1]++;
        i--;
    }
    if (i < 0 || counts[1] > maxCount)
    {
        return -0.0f;
    }
    while (i >= 0 && !IsLight(x, i) && counts[0] <= maxCount)
    {
        counts[0]++;
        i--;
    }
    if (counts[0] > maxCount)
    {
        return -0.0f;
    }

    i = y + 1;
    while (i < height && !IsLight(x, i))
    {
        counts[2]++;
        i++;
    }
    if (i == height)
    {
        return -0.0f;
    }
    while (i < height && IsLight(x, i) && counts[3] < maxCount)
    {
        counts[3]++;
        i++;
    }
    if (i == height || counts[3] >= maxCount)
    {
        return -0.0f;
    }
    while (i < height && !IsLight(x, i) && counts[4] < maxCount)
    {
        counts[4]++;
        i++;
    }
    if (counts[4] >= maxCount)
    {
        return -0.0f;
    }
    if (!CheckRatios(counts))
    {
        return -0.0f;
    }
    return static_cast<f32>(i - counts[4] - counts[3]) - counts[2] * 0.5f;
}

inline f32 CrossCheckHorizontal(s32 x, s32 y, s32 maxCount, s32 width, s32* counts)
{
    s32 i = x;

    while (i >= 0 && !IsLight(i, y))
    {
        counts[2]++;
        i--;
    }
    if (i < 0)
    {
        return -0.0f;
    }
    while (i >= 0 && IsLight(i, y) && counts[1] <= maxCount)
    {
        counts[1]++;
        i--;
    }
    if (i < 0 || counts[1] > maxCount)
    {
        return -0.0f;
    }
    while (i >= 0 && !IsLight(i, y) && counts[0] <= maxCount)
    {
        counts[0]++;
        i--;
    }
    if (counts[0] > maxCount)
    {
        return -0.0f;
    }

    i = x + 1;
    while (i < width && !IsLight(i, y))
    {
        counts[2]++;
        i++;
    }
    if (i == width)
    {
        return -0.0f;
    }
    while (i < width && IsLight(i, y) && counts[3] < maxCount)
    {
        counts[3]++;
        i++;
    }
    if (i == width || counts[3] >= maxCount)
    {
        return -0.0f;
    }
    while (i < width && !IsLight(i, y) && counts[4] < maxCount)
    {
        counts[4]++;
        i++;
    }
    if (counts[4] >= maxCount)
    {
        return -0.0f;
    }
    if (!CheckRatios(counts))
    {
        return -0.0f;
    }
    return static_cast<f32>(i - counts[4] - counts[3]) - counts[2] * 0.5f;
}

// centres a pattern on the middle of its five runs (0 = moved, 1 = stays, 2 = left the image)
inline s32 RefineCenterX(PatternPoint* point)
{
    s32 x = point->x;
    s32 y = point->y;
    s32 color = 1;
    s32 transitions = 0;

    Yield();
    // back to the start of the pattern
    for (;;)
    {
        if (!IsLight(x, y) == color)
        {
            if (--x < 0)
            {
                return 2;
            }
            continue;
        }
        color = !color;
        if (++transitions >= 3)
        {
            break;
        }
    }

    s32 start = ++x;
    s32 length = 0;
    color = 1;
    transitions = 0;
    for (;;)
    {
        if (!IsLight(x, y) == color)
        {
            x++;
            length++;
            if (x > s_Context.bitImage.width)
            {
                return 2;
            }
            continue;
        }
        color = !color;
        if (++transitions >= 5)
        {
            break;
        }
    }

    f32 center = start + length * 0.5f;
    if (point->x == center)
    {
        return 1;
    }
    point->x = center;
    point->moduleSize = length * (1.0f / 7.0f);
    return 0;
}

inline s32 RefineCenterY(PatternPoint* point)
{
    s32 x = point->x;
    s32 y = point->y;
    s32 color = 1;
    s32 transitions = 0;

    Yield();
    for (;;)
    {
        if (!IsLight(x, y) == color)
        {
            if (--y < 0)
            {
                return 2;
            }
            continue;
        }
        color = !color;
        if (++transitions >= 3)
        {
            break;
        }
    }

    s32 start = ++y;
    s32 length = 0;
    color = 1;
    transitions = 0;
    for (;;)
    {
        if (!IsLight(x, y) == color)
        {
            y++;
            length++;
            if (y > s_Context.bitImage.height)
            {
                return 2;
            }
            continue;
        }
        color = !color;
        if (++transitions >= 5)
        {
            break;
        }
    }

    f32 center = start + length * 0.5f;
    if (point->y == center)
    {
        return 1;
    }
    point->y = center;
    point->moduleSize = length * (1.0f / 7.0f);
    return 0;
}

inline void FreePatternList(PatternList* list)
{
    Free(list->items);
    Free(list);
}

} // namespace

// 0x0016613C
bool SetPatternListItem(PatternList* list, const PatternPoint* point, s32 index)
{
    Yield();
    if (list == NULL || point == NULL || index > list->count)
    {
        return false;
    }
    if (list->capacity <= index)
    {
        s32 capacity = (index + 32) & ~31;
        PatternPoint* items = static_cast<PatternPoint*>(Allocate(capacity * sizeof(PatternPoint)));
        if (items == NULL)
        {
            return false;
        }
        std::memcpy(items, list->items, list->capacity * sizeof(PatternPoint));
        std::memset(items + list->capacity, 0, (capacity - list->capacity) * sizeof(PatternPoint));
        Free(list->items);
        list->capacity = capacity;
        list->items = items;
    }
    list->items[index] = *point;
    if (list->count == index)
    {
        list->count++;
    }
    return true;
}

// 0x00168FA0
DECOMP_NOINLINE PatternList* CreatePatternList(s32 capacity)
{
    PatternList* list = static_cast<PatternList*>(Allocate(sizeof(PatternList)));
    list->items = NULL;
    list->capacity = 0;
    list->count = 0;
    capacity = (capacity + 31) & ~31;
    Yield();
    list->items = static_cast<PatternPoint*>(Allocate(capacity * sizeof(PatternPoint)));
    std::memset(list->items, 0, capacity * sizeof(PatternPoint));
    list->capacity = capacity;
    list->count = 0;
    return list;
}

// sorts by the number of rows that found the pattern, most first (qsort)
// 0x0016CCB8
int ComparePatternCount(const void* a, const void* b)
{
    s32 countA = static_cast<const PatternPoint*>(a)->count;
    s32 countB = static_cast<const PatternPoint*>(b)->count;
    if (countA == countB)
    {
        return 0;
    }
    return countA > countB ? -1 : 1;
}

// runs of 1:1:3:1:1 modules (with tolerance)?
// 0x00175794
bool CheckRatios(const s32* counts)
{
    s32 total = 0;
    for (s32 i = 0; i < 5; i++)
    {
        if (counts[i] == 0)
        {
            return false;
        }
        total += counts[i];
    }
    if (total < 7)
    {
        return false;
    }
    f32 moduleSize = total * (1.0f / 7.0f);
    f32 maxVariance = moduleSize * 0.704225361f;
    return std::fabs(moduleSize - counts[0]) < maxVariance && std::fabs(moduleSize - counts[1]) < maxVariance &&
           std::fabs(3.0f * moduleSize - counts[2]) < 3.0f * maxVariance &&
           std::fabs(moduleSize - counts[3]) < maxVariance && std::fabs(moduleSize - counts[4]) < maxVariance;
}

// the three candidates that form the best right isosceles triangle are moved to the front
// 0x00178208
void SelectTriangle(PatternList* list, s32 num, s32 width, s32 height)
{
    PatternPoint* items = list->items;
    s32 best = 0xFFFF;
    s32 threshold = (items[0].count + items[num - 1].count + 1) / 2;
    s32 minCount = items[3].count;
    if (minCount > threshold)
    {
        minCount = threshold;
    }
    f32 size = width > height ? width : height;
    s32 indices[3] = {0, 1, 2};

    for (s32 i = 1; i < num; i++)
    {
        if (items[i].count < minCount)
        {
            num = i;
            break;
        }
    }

    for (s32 a = 0; a < num; a++)
    {
        f32 minScore = 65535.0f;
        s32 bestA = 0;
        s32 bestB = 0;
        s32 bestC = 0;
        f32 ax = items[a].x;
        f32 ay = items[a].y;
        for (s32 b = 0; b < num; b++)
        {
            if (b == a)
            {
                continue;
            }
            f32 bx = items[b].x;
            f32 by = items[b].y;
            for (s32 c = 0; c < num; c++)
            {
                if (c == a || c == b)
                {
                    continue;
                }
                f32 cx = items[c].x;
                f32 cy = items[c].y;
                f32 d;
                d = std::fabs(ax - bx);
                f32 ab = d * d;
                d = std::fabs(ay - by);
                ab += d * d;
                d = std::fabs(ax - cx);
                f32 ac = d * d;
                d = std::fabs(ay - cy);
                ac += d * d;
                d = std::fabs(bx - cx);
                f32 bc = d * d;
                d = std::fabs(by - cy);
                bc += d * d;

                // the hypotenuse and the two legs (squared)
                f32 hypotenuse;
                f32 leg1;
                f32 leg2;
                if (ac <= ab && ab >= bc)
                {
                    hypotenuse = ab;
                    leg1 = ac;
                    leg2 = bc;
                }
                else if (ac >= ab && ac >= bc)
                {
                    hypotenuse = ac;
                    leg1 = ab;
                    leg2 = bc;
                }
                else
                {
                    hypotenuse = bc;
                    leg1 = ab;
                    leg2 = ac;
                }
                f32 score = (std::fabs(leg1 - leg2) + std::fabs(hypotenuse - (leg1 + leg2))) *
                            (size / std::sqrt(hypotenuse));
                if (score < minScore)
                {
                    minScore = score;
                    bestA = a;
                    bestB = b;
                    bestC = c;
                }
            }
        }

        f32 sizeA = items[bestA].moduleSize;
        f32 sizeB = items[bestB].moduleSize;
        f32 sizeC = items[bestC].moduleSize;
        s32 value =
            (std::fabs(sizeA - sizeB) + std::fabs(sizeA - sizeC) + std::fabs(sizeB - sizeC) + 1.0f) * minScore;
        if (value < best)
        {
            best = value;
            indices[0] = bestA;
            indices[1] = bestB;
            indices[2] = bestC;
        }
    }

    if (best > size * size * 0.1f)
    {
        return;
    }
    PatternPoint temp = list->items[0];
    list->items[0] = list->items[indices[0]];
    list->items[indices[0]] = temp;
    temp = list->items[1];
    list->items[1] = list->items[indices[1]];
    list->items[indices[1]] = temp;
    temp = list->items[2];
    list->items[2] = list->items[indices[2]];
    list->items[indices[2]] = temp;
}

// 0x001786C8
bool SelectBestPatterns(PatternList* candidates, PatternList* out, s32 width, s32 height)
{
    s32 num = candidates->count;
    s32 validNum = 0;

    Yield();
    std::qsort(candidates->items, num, sizeof(PatternPoint), ComparePatternCount);
    for (s32 i = 0; i < num; i++)
    {
        if (candidates->items[i].count < 1)
        {
            break;
        }
        validNum++;
    }
    if (validNum < 3)
    {
        return false;
    }

    if (validNum > 3)
    {
        SelectTriangle(candidates, validNum, width, height);
        PatternPoint* items = candidates->items;

        // one of the three may be wrong: the one whose module size is far from the other two
        f32 size0 = items[0].moduleSize;
        f32 size1 = items[1].moduleSize;
        f32 size2 = items[2].moduleSize;
        f32 diff01 = std::fabs(size0 - size1);
        f32 diff12 = std::fabs(size1 - size2);
        f32 diff02 = std::fabs(size0 - size2);
        f32 mean;
        f32 tolerance;
        s32 first;
        s32 second;
        if (diff12 <= diff01 && diff12 <= diff02)
        {
            mean = (size1 + size2) * 0.5f;
            tolerance = mean * 0.3f;
            if (std::fabs(size0 - mean) <= tolerance)
            {
                goto takeAll;
            }
            first = 1;
            second = 2;
        }
        else if (diff02 <= diff01 && diff02 <= diff12)
        {
            mean = (size0 + size2) * 0.5f;
            tolerance = mean * 0.3f;
            if (std::fabs(size1 - mean) <= tolerance)
            {
                goto takeAll;
            }
            first = 0;
            second = 2;
        }
        else if (diff01 <= diff02 && diff01 <= diff12)
        {
            mean = (size0 + size1) * 0.5f;
            tolerance = mean * 0.3f;
            if (std::fabs(size2 - mean) <= tolerance)
            {
                goto takeAll;
            }
            first = 0;
            second = 1;
        }
        else
        {
            goto takeAll;
        }

        SetPatternListItem(out, &items[first], out->count);
        SetPatternListItem(out, &candidates->items[second], out->count);
        // a replacement with a similar module size that makes a right angle
        for (s32 i = 3; i < validNum; i++)
        {
            items = candidates->items;
            if (std::fabs(items[i].moduleSize - mean) >= tolerance)
            {
                continue;
            }
            PatternPoint* a = &items[first];
            PatternPoint* b = &items[second];
            PatternPoint* c = &items[i];
            f32 angleA = Angle(a, c, b);
            f32 angleB = Angle(b, a, c);
            f32 angleC = Angle(c, a, b);
            f32 maxAngle = angleA > angleB ? angleA : angleB;
            maxAngle = angleC > maxAngle ? angleC : maxAngle;
            if (std::fabs(maxAngle - 1.57079637f) < 0.224399477f)
            {
                SetPatternListItem(out, &candidates->items[i], out->count);
                return true;
            }
        }
        SetPatternListItem(out, &candidates->items[2], out->count);
        return true;
    }

takeAll:
    SetPatternListItem(out, &candidates->items[0], out->count);
    SetPatternListItem(out, &candidates->items[1], out->count);
    SetPatternListItem(out, &candidates->items[2], out->count);
    return true;
}

// 0x0017AA90
bool HandlePossibleCenter(const s32* counts, s32 y, s32 end, const BitImage* image, PatternList* list)
{
    s32 maxCount = counts[2];
    s32 centerX = static_cast<f32>(end - counts[4] - counts[3]) - counts[2] * 0.5f;
    f32 centerY = CrossCheckVertical(centerX, y, maxCount, image->height);
    if (centerY == -0.0f)
    {
        return false;
    }

    s32 horizontalCounts[5] = {};
    f32 refinedX = CrossCheckHorizontal(centerX, centerY, counts[2], image->width, horizontalCounts);
    if (refinedX == -0.0f)
    {
        return false;
    }

    s32 total = horizontalCounts[0] + horizontalCounts[1] + horizontalCounts[2] + horizontalCounts[3] +
                horizontalCounts[4];
    f32 moduleSize = total * (1.0f / 7.0f);
    for (s32 i = 0; i < list->count; i++)
    {
        PatternPoint* item = &list->items[i];
        if (std::fabs(centerY - item->y) <= moduleSize && std::fabs(refinedX - item->x) <= moduleSize)
        {
            f32 difference = std::fabs(moduleSize - item->moduleSize);
            if (difference <= 1.0f || difference / item->moduleSize <= 0.1f)
            {
                item->count++;
                return true;
            }
        }
    }

    PatternPoint point;
    point.x = refinedX;
    point.y = centerY;
    point.moduleSize = moduleSize;
    point.count = 1;
    SetPatternListItem(list, &point, list->count);
    return true;
}

// 0x001884E0
s32 SearchFinderPatterns(const BitImage* image, FinderPattern** finders, s32* count)
{
    s32 skip = 1;

    Yield();
    PatternList* candidates = CreatePatternList(3);
    for (s32 y = 0; y < image->height; y += skip)
    {
        s32 counts[5] = {};
        s32 state = 0;
        for (s32 x = 0; x < image->width; x++)
        {
            if (IsDark(x, y))
            {
                if (state & 1)
                {
                    state++;
                }
                counts[state]++;
            }
            else if ((state & 1) == 0)
            {
                if (state == 4)
                {
                    if (CheckRatios(counts) && HandlePossibleCenter(counts, y, x, image, candidates))
                    {
                        skip = 1;
                        if (counts[2] < 0)
                        {
                            x -= counts[2] + 1;
                            y = image->width - 1;
                        }
                        state = 0;
                        counts[0] = 0;
                        counts[1] = 0;
                        counts[2] = 0;
                        counts[3] = 0;
                        counts[4] = 0;
                    }
                    else
                    {
                        counts[0] = counts[2];
                        counts[1] = counts[3];
                        counts[2] = counts[4];
                        counts[3] = 1;
                        counts[4] = 0;
                        state = 3;
                    }
                }
                else
                {
                    state++;
                    counts[state]++;
                }
            }
            else
            {
                counts[state]++;
            }
        }
        if (CheckRatios(counts) && HandlePossibleCenter(counts, y, image->width, image, candidates))
        {
            skip = counts[0];
        }
    }

    PatternList* best = CreatePatternList(3);
    if (!SelectBestPatterns(candidates, best, image->width, image->height))
    {
        goto failure;
    }

    {
        PatternPoint* items = best->items;
        f32 distance01 = Distance(&items[0], &items[1]);
        f32 distance12 = Distance(&items[1], &items[2]);
        f32 distance02 = Distance(&items[0], &items[2]);
        Yield();

        // the top left pattern is opposite the longest side
        if (!(distance12 >= distance01 && distance12 >= distance02))
        {
            if (distance02 >= distance12 && distance02 >= distance01)
            {
                PatternList* temp = CreatePatternList(1);
                SetPatternListItem(temp, &best->items[0], 0);
                SetPatternListItem(best, &best->items[1], 0);
                SetPatternListItem(best, &temp->items[0], 1);
                FreePatternList(temp);
            }
            else
            {
                PatternList* temp = CreatePatternList(1);
                SetPatternListItem(temp, &best->items[0], 0);
                SetPatternListItem(best, &best->items[2], 0);
                SetPatternListItem(best, &best->items[1], 2);
                SetPatternListItem(best, &temp->items[0], 1);
                FreePatternList(temp);
            }
        }

        // top right and bottom left in clockwise order
        items = best->items;
        if ((items[2].y - items[0].y) * (items[1].x - items[0].x) <
            (items[2].x - items[0].x) * (items[1].y - items[0].y))
        {
            PatternList* temp = CreatePatternList(1);
            SetPatternListItem(temp, &best->items[1], 0);
            SetPatternListItem(best, &best->items[2], 1);
            SetPatternListItem(best, &temp->items[0], 2);
            FreePatternList(temp);
        }
    }

    for (s32 i = 0; i < best->count; i++)
    {
        PatternPoint* point = &best->items[i];
        for (s32 iteration = 0; iteration < 8; iteration++)
        {
            s32 resultX = RefineCenterX(point);
            s32 resultY = RefineCenterY(point);
            if (resultX == 2 || resultY == 2)
            {
                goto failure;
            }
            if (resultX != 0 && resultY != 0)
            {
                break;
            }
        }
    }

    (*finders)[0].x = best->items[0].x;
    (*finders)[0].y = best->items[0].y;
    (*finders)[0].moduleSize = best->items[0].moduleSize;
    (*finders)[0]._10 = best->items[0].moduleSize;
    (*finders)[1].x = best->items[1].x;
    (*finders)[1].y = best->items[1].y;
    (*finders)[1].moduleSize = best->items[1].moduleSize;
    (*finders)[1]._10 = best->items[1].moduleSize;
    (*finders)[2].x = best->items[2].x;
    (*finders)[2].y = best->items[2].y;
    (*finders)[2].moduleSize = best->items[2].moduleSize;
    (*finders)[2]._10 = best->items[2].moduleSize;
    FreePatternList(best);
    FreePatternList(candidates);
    return 1;

failure:
    FreePatternList(best);
    FreePatternList(candidates);
    return 0;
}

// 0x00188E08
bool IsLight(s32 x, s32 y)
{
    const BitImage& image = s_Context.bitImage;
    if (x < 0 || x > image.width || y < 0 || y > image.height)
    {
        return false;
    }
    s32 index = y * image.width + x;
    return (image.bits[static_cast<u32>(index) >> 5] & (1 << (index & 31))) != 0;
}

// 0x00188E64
bool IsDark(s32 x, s32 y)
{
    const BitImage& image = s_Context.bitImage;
    if (x < 0 || x > image.width || y < 0 || y > image.height)
    {
        return true;
    }
    s32 index = y * image.width + x;
    return (image.bits[static_cast<u32>(index) >> 5] & (1 << (index & 31))) == 0;
}

} // namespace detail
} // namespace qrd
} // namespace mw
