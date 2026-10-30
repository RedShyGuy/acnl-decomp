#include <cmath>
#include <cstdlib>
#include <cstring>

#include "mw/qrd/qrd_Internal.h"

namespace mw {
namespace qrd {
namespace detail {

// 0x00982E90
AlignmentFinder s_AlignmentFinder;
// the finder pattern centres for CompareAlignmentCandidates
// 0x00AF7530
PatternPoint s_FinderCentres[3];

namespace {

inline PatternPoint* GetPatternListItem(PatternList* list, s32 index)
{
    if (list == NULL || list->count <= index)
    {
        return NULL;
    }
    return &list->items[index];
}

} // namespace

// scans the rows around the estimate for a 1:1:1 dark-light-dark run
// 0x00166434
s32 SearchAlignment(const FinderPattern* finder0, const FinderPattern* finder1, const FinderPattern* finder2,
                    PatternPoint* out)
{
    s32 startX = s_AlignmentFinder.startX;
    s32 height = s_AlignmentFinder.height;
    s32 maxJ = startX + s_AlignmentFinder.width;
    s32 middleI = s_AlignmentFinder.startY + (height >> 1);
    PatternPoint candidate;

    Yield();
    s32 counts[3];
    counts[0] = 0;
    counts[1] = 0;
    counts[2] = 0;
    for (s32 iGen = 0; iGen < height; iGen++)
    {
        // from the middle outwards
        s32 i = middleI + ((iGen & 1) == 0 ? (iGen + 1) >> 1 : -((iGen + 1) >> 1));
        counts[0] = 0;
        counts[1] = 0;
        counts[2] = 0;
        s32 state = 0;
        s32 j = startX;
        while (j < maxJ && IsLight(j, i))
        {
            j++;
        }
        for (; j < maxJ; j++)
        {
            if (IsLight(j, i))
            {
                if (state == 1)
                {
                    state = 2;
                }
                counts[state]++;
            }
            else if (state == 1)
            {
                counts[1]++;
            }
            else if (state == 2)
            {
                if (FoundAlignmentCross(counts))
                {
                    HandleAlignmentCenter(counts, i, j, &candidate);
                }
                counts[0] = counts[2];
                counts[1] = 1;
                counts[2] = 0;
                state = 1;
            }
            else
            {
                state++;
                counts[state]++;
            }
        }
        if (FoundAlignmentCross(counts))
        {
            HandleAlignmentCenter(counts, i, maxJ, &candidate);
        }
    }

    PatternList* list = s_AlignmentFinder.candidates;
    if (list->count == 0)
    {
        Free(list->items);
        Free(list);
        return 0;
    }

    // the best candidate fits the distances of the finder patterns
    s_FinderCentres[0].x = finder0->x;
    s_FinderCentres[0].y = finder0->y;
    s_FinderCentres[1].x = finder1->x;
    s_FinderCentres[1].y = finder1->y;
    s_FinderCentres[2].x = finder2->x;
    s_FinderCentres[2].y = finder2->y;
    std::qsort(list->items, list->count, sizeof(PatternPoint), CompareAlignmentCandidates);
    list = s_AlignmentFinder.candidates;
    *out = *GetPatternListItem(list, 0);
    Free(list->items);
    Free(list);
    return 1;
}

// 0x001756C0
bool FoundAlignmentCross(const s32* counts)
{
    f32 moduleSize = s_AlignmentFinder.moduleSize;
    f32 maxVariance = moduleSize * 0.751879692f;
    for (s32 i = 0; i < 3; i++)
    {
        if (std::fabs(moduleSize - counts[i]) >= maxVariance)
        {
            // still fine when the runs are even and their mean is near the module size
            f32 mean = (counts[0] + counts[1] + counts[2]) * (1.0f / 3.0f);
            f32 tolerance = (mean > moduleSize ? mean : moduleSize) * 0.4f;
            if (std::fabs(static_cast<f32>(counts[0] - counts[2])) < moduleSize * 0.25f &&
                std::fabs(mean - moduleSize) < tolerance)
            {
                return true;
            }
            return false;
        }
    }
    return true;
}

// 0x00177D10
s32 FindAlignmentPattern(s32 x, s32 y, const FinderPattern* finder0, const FinderPattern* finder1,
                         const FinderPattern* finder2, PatternPoint* alignment, f32 moduleSize, f32 radius)
{
    s32 allowance = radius * moduleSize;
    s32 left = x - allowance;
    s32 right = x + allowance;
    s32 top = y - allowance;
    s32 bottom = y + allowance;
    if (left < 0)
    {
        left = 0;
    }
    if (s_Context.bitImage.width - 1 < right)
    {
        right = s_Context.bitImage.width - 1;
    }
    if (top < 0)
    {
        top = 0;
    }
    if (s_Context.bitImage.height - 1 < bottom)
    {
        bottom = s_Context.bitImage.height - 1;
    }
    Yield();
    InitializeAlignmentFinder(left, top, right - left, bottom - top, moduleSize);
    return SearchAlignment(finder0, finder1, finder2, alignment);
}

// checks a candidate across its column and keeps it (0 = kept)
// 0x0017A5FC
s32 HandleAlignmentCenter(const s32* counts, s32 i, s32 j, PatternPoint* point)
{
    f32 centerJ = static_cast<f32>(j - counts[2]) - counts[1] * 0.5f;
    s32 column = centerJ;
    s32 height = s_Context.bitImage.height;
    f32 centerI;

    Yield();
    {
        s32 c[3];
        c[0] = 0;
        c[1] = 0;
        c[2] = 0;
        s32 k = i;
        while (k >= 0 && !IsLight(column, k) && c[1] <= counts[1] * 2.0f)
        {
            c[1]++;
            k--;
        }
        if (k < 0 || c[1] > counts[1] * 2.0f)
        {
            goto notFound;
        }
        while (k >= 0 && IsLight(column, k) && c[0] <= counts[0] * 2.0f)
        {
            c[0]++;
            k--;
        }
        if (c[0] > counts[0] * 2.0f)
        {
            goto notFound;
        }
        k = i + 1;
        while (k < height && !IsLight(column, k) && c[1] <= counts[1] * 2.0f)
        {
            c[1]++;
            k++;
        }
        if (k == height || c[1] > counts[1] * 2.0f)
        {
            goto notFound;
        }
        while (k < height && IsLight(column, k) && c[2] <= counts[2] * 2.0f)
        {
            c[2]++;
            k++;
        }
        if (c[2] > counts[2] * 2.0f || !FoundAlignmentCross(c))
        {
            goto notFound;
        }
        centerI = static_cast<f32>(k - c[2]) - c[1] * 0.5f;
        goto checked;
    }
notFound:
    centerI = -0.0f;
checked:
    Yield();
    point->x = 0.0f;
    point->y = 0.0f;
    point->moduleSize = 0.0f;
    point->count = 0;
    if (centerI == -0.0f)
    {
        return 1;
    }

    f32 moduleSize = (counts[0] + counts[1] + counts[2]) * (1.0f / 3.0f);
    PatternList* list = s_AlignmentFinder.candidates;
    for (s32 k = 0; k < list->count; k++)
    {
        PatternPoint* candidate = GetPatternListItem(list, k);
        if (std::fabs(centerI - candidate->y) <= moduleSize && std::fabs(centerJ - candidate->x) <= moduleSize)
        {
            f32 difference = std::fabs(moduleSize - candidate->moduleSize);
            if (difference <= 1.0f || difference / candidate->moduleSize <= 0.1f)
            {
                point->x = centerJ;
                point->y = centerI;
                point->moduleSize = moduleSize;
                return 0;
            }
        }
    }
    if (std::fabs(s_AlignmentFinder.moduleSize - moduleSize) < 1000.0f)
    {
        point->x = centerJ;
        point->y = centerI;
        point->moduleSize = moduleSize;
        SetPatternListItem(list, point, list->count);
        return 0;
    }
    return 1;
}

// 0x0017EF64
void InitializeAlignmentFinder(s32 startX, s32 startY, s32 width, s32 height, f32 moduleSize)
{
    Yield();
    s_AlignmentFinder.startX = startX;
    s_AlignmentFinder.startY = startY;
    s_AlignmentFinder.width = width;
    s_AlignmentFinder.height = height;
    s_AlignmentFinder.moduleSize = moduleSize;
    s_AlignmentFinder.candidates = CreatePatternList(1);
}

// 0x00187334
DECOMP_NOINLINE f32 PointDistance(Point2f a, Point2f b)
{
    f32 dx = std::fabs(a.x - b.x);
    f32 dy = std::fabs(a.y - b.y);
    return std::sqrt(dx * dx + dy * dy);
}

// the candidate whose distances to the finder patterns fit a parallelogram best comes first (qsort)
// 0x00189D74
int CompareAlignmentCandidates(const void* a, const void* b)
{
    const PatternPoint* pointA = static_cast<const PatternPoint*>(a);
    const PatternPoint* pointB = static_cast<const PatternPoint*>(b);
    s32 invalid = -1;
    if (pointA == NULL || pointB == NULL || std::memcmp(&pointA->x, &invalid, 4) == 0 ||
        std::memcmp(&pointA->y, &invalid, 4) == 0 || std::memcmp(&pointB->x, &invalid, 4) == 0 ||
        std::memcmp(&pointB->y, &invalid, 4) == 0)
    {
        return -1;
    }

    Point2f c0 = {s_FinderCentres[0].x, s_FinderCentres[0].y};
    Point2f c1 = {s_FinderCentres[1].x, s_FinderCentres[1].y};
    Point2f c2 = {s_FinderCentres[2].x, s_FinderCentres[2].y};
    Point2f pa = {pointA->x, pointA->y};
    Point2f pb = {pointB->x, pointB->y};
    f32 errorA = std::fabs(PointDistance(c0, c2) - PointDistance(c1, pa)) +
                 std::fabs(PointDistance(c0, c1) - PointDistance(c2, pa));
    f32 errorB = std::fabs(PointDistance(c0, c2) - PointDistance(c1, pb)) +
                 std::fabs(PointDistance(c0, c1) - PointDistance(c2, pb));
    Yield();
    return errorA >= errorB ? 1 : -1;
}

} // namespace detail
} // namespace qrd
} // namespace mw
