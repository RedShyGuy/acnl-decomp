#include "nw/anim/res/res_Api.h"

#include <cmath>

#include "nn/math/math_ARMv6.h"

namespace nw {
namespace anim {
namespace res {
namespace {
typedef f32 (*RepeatFunc)(f32 frame, f32 startFrame, f32 endFrame);

// (inline) the frame inside the curve, the curve repeated
inline f32 RepeatFrame(f32 frame, f32 startFrame, f32 endFrame)
{
    const f32 length = endFrame - startFrame;
    const s32 count = static_cast<s32>(std::floor((frame - startFrame) / length));
    return frame - static_cast<f32>(count) * length;
}

// (inline) the frame inside the curve, the curve played back and forth
inline f32 MirrorFrame(f32 frame, f32 startFrame, f32 endFrame)
{
    const f32 length = endFrame - startFrame;
    const s32 count = static_cast<s32>(std::floor((frame - startFrame) / length));
    f32 result = frame - static_cast<f32>(count) * length;
    if (count & 1) {
        result = (startFrame + endFrame) - result;
    }
    return result;
}

// the repeat methods before the start of a curve (names are ours)
// 0x004D5FFC (name is ours)
f32 PreClampFrame(f32 frame, f32 startFrame, f32 /*endFrame*/)
{
    return (frame < startFrame) ? startFrame : frame;
}

// 0x004D606C (name is ours)
f32 PreRepeatFrame(f32 frame, f32 startFrame, f32 endFrame)
{
    return RepeatFrame(frame, startFrame, endFrame);
}

// 0x004D601C (name is ours)
f32 PreMirrorFrame(f32 frame, f32 startFrame, f32 endFrame)
{
    return MirrorFrame(frame, startFrame, endFrame);
}

// the repeat methods behind the end of a curve
// 0x004D600C (name is ours)
f32 PostClampFrame(f32 frame, f32 /*startFrame*/, f32 endFrame)
{
    return (frame > endFrame) ? endFrame : frame;
}

// 0x004D60F0 (name is ours)
f32 PostRepeatFrame(f32 frame, f32 startFrame, f32 endFrame)
{
    return RepeatFrame(frame, startFrame, endFrame);
}

// 0x004D60A0 (name is ours)
f32 PostMirrorFrame(f32 frame, f32 startFrame, f32 endFrame)
{
    return MirrorFrame(frame, startFrame, endFrame);
}

// (inline) the segment of the frame (the last one behind all)
inline const ResFloatSegmentData* FindSegment(const ResFloatCurveData* pCurve, f32 frame)
{
    if (pCurve->flags & ResFloatCurveData::FLAG_SINGLE_SEGMENT) {
        return pCurve->segments[0].To<ResFloatSegmentData>();
    }
    const ut::Offset* pEnd = pCurve->segments + pCurve->segmentCount;
    for (const ut::Offset* pOffset = pCurve->segments; pOffset < pEnd; ++pOffset) {
        const ResFloatSegmentData* pSegment = pOffset->To<ResFloatSegmentData>();
        if (pSegment->endFrame > frame) {
            return pSegment;
        }
    }
    return (pEnd - 1)->To<ResFloatSegmentData>();
}

// by RepeatMethod
// 0x008A3990 (name is ours)
const RepeatFunc s_PreRepeatFuncs[] = {PreClampFrame, PreRepeatFrame, PreMirrorFrame};
// 0x008A399C (name is ours)
const RepeatFunc s_PostRepeatFuncs[] = {PostClampFrame, PostRepeatFrame, PostMirrorFrame};
} // namespace

namespace internal {
// The keys of a segment (the names of the formats and all other names are ours). A segment starts
// with the number of keys and the factor that turns a frame into an estimated key index; the
// quantized formats have the scale and offset of their values and the scale of their frames.
struct KeysHeader
{
    u16 keyCount;     // 0x0
    u8 padding[2];    // 0x2
    f32 invDuration;  // 0x4 1 / the length of the segment in frames
};
ASSERT_SIZE(KeysHeader, 0x8);

struct QuantizedKeysHeader : public KeysHeader
{
    f32 valueScale;  // 0x8
    f32 valueOffset; // 0xC
    f32 frameScale;  // 0x10
};
ASSERT_SIZE(QuantizedKeysHeader, 0x14);

// format 0: float frame, value and slopes
struct HermiteKeyF32
{
    typedef KeysHeader Header;
    typedef f32 Frame;
    typedef f32 Value;

    static f32 QuantizeFrame(const Header* /*pHeader*/, f32 frame) { return frame; }
    f32 GetRawFrame() const { return frame; }
    f32 GetFrame(const Header* /*pHeader*/) const { return frame; }
    f32 GetValue(const Header* /*pHeader*/) const { return value; }
    f32 GetInSlope() const { return inSlope; }
    f32 GetOutSlope() const { return outSlope; }

    f32 frame;    // 0x0
    f32 value;    // 0x4
    f32 inSlope;  // 0x8
    f32 outSlope; // 0xC
};

// format 1: a 12 bit frame and a 20 bit value, slopes in 1/256
struct HermiteKeyQ64
{
    typedef QuantizedKeysHeader Header;
    typedef u32 Frame;
    typedef f32 Value;

    static u32 QuantizeFrame(const Header* pHeader, f32 frame) { return static_cast<u32>(std::floor(frame / pHeader->frameScale + 0.5f)); }
    u32 GetRawFrame() const { return frameValue & 0xFFF; }
    f32 GetFrame(const Header* pHeader) const { return pHeader->frameScale * static_cast<f32>(GetRawFrame()); }
    f32 GetValue(const Header* pHeader) const { return pHeader->valueOffset + static_cast<f32>(frameValue >> 12) * pHeader->valueScale; }
    f32 GetInSlope() const { return static_cast<f32>(inSlope) * (1.0f / 256.0f); }
    f32 GetOutSlope() const { return static_cast<f32>(outSlope) * (1.0f / 256.0f); }

    u32 frameValue; // 0x0
    s16 inSlope;    // 0x4
    s16 outSlope;   // 0x6
};

// format 2: an 8 bit frame, a 16 bit value and two 12 bit slopes in 1/32
struct HermiteKeyQ48
{
    typedef QuantizedKeysHeader Header;
    typedef u32 Frame;
    typedef f32 Value;

    static u32 QuantizeFrame(const Header* pHeader, f32 frame) { return static_cast<u32>(std::floor(frame / pHeader->frameScale + 0.5f)); }
    u32 GetRawFrame() const { return data[0]; }
    f32 GetFrame(const Header* pHeader) const { return pHeader->frameScale * static_cast<f32>(GetRawFrame()); }
    f32 GetValue(const Header* pHeader) const { return pHeader->valueOffset + static_cast<f32>(data[1] + (data[2] << 8)) * pHeader->valueScale; }
    f32 GetInSlope() const { return static_cast<f32>(data[3] + (static_cast<s8>(data[4] << 4) << 4)) * (1.0f / 32.0f); }
    f32 GetOutSlope() const { return static_cast<f32>((static_cast<s8>(data[5]) << 4) + (data[4] >> 4)) * (1.0f / 32.0f); }

    u8 data[6];
};

// format 3: float frame, value and one slope
struct UnifiedHermiteKeyF32
{
    typedef KeysHeader Header;
    typedef f32 Frame;
    typedef f32 Value;

    static f32 QuantizeFrame(const Header* /*pHeader*/, f32 frame) { return frame; }
    f32 GetRawFrame() const { return frame; }
    f32 GetFrame(const Header* /*pHeader*/) const { return frame; }
    f32 GetValue(const Header* /*pHeader*/) const { return value; }
    f32 GetInSlope() const { return slope; }
    f32 GetOutSlope() const { return slope; }

    f32 frame; // 0x0
    f32 value; // 0x4
    f32 slope; // 0x8
};

// format 4: a 16 bit frame in 1/32, a 16 bit value and a slope in 1/256
struct UnifiedHermiteKeyQ48
{
    typedef QuantizedKeysHeader Header;
    typedef s32 Frame;
    typedef f32 Value;

    static s32 QuantizeFrame(const Header* pHeader, f32 frame) { return static_cast<s32>(std::floor(frame / pHeader->frameScale * 32.0f + 0.5f)); }
    u32 GetRawFrame() const { return frame; }
    f32 GetFrame(const Header* pHeader) const { return pHeader->frameScale * (static_cast<f32>(frame) * (1.0f / 32.0f)); }
    f32 GetValue(const Header* pHeader) const { return pHeader->valueOffset + static_cast<f32>(value) * pHeader->valueScale; }
    f32 GetInSlope() const { return static_cast<f32>(slope) * (1.0f / 256.0f); }
    f32 GetOutSlope() const { return static_cast<f32>(slope) * (1.0f / 256.0f); }

    u16 frame; // 0x0
    u16 value; // 0x2
    s16 slope; // 0x4
};

// format 5: an 8 bit frame, a 12 bit value and a 12 bit slope in 1/32
struct UnifiedHermiteKeyQ32
{
    typedef QuantizedKeysHeader Header;
    typedef u32 Frame;
    typedef f32 Value;

    static u32 QuantizeFrame(const Header* pHeader, f32 frame) { return static_cast<u32>(std::floor(frame / pHeader->frameScale + 0.5f)); }
    u32 GetRawFrame() const { return data[0]; }
    f32 GetFrame(const Header* pHeader) const { return pHeader->frameScale * static_cast<f32>(GetRawFrame()); }
    f32 GetValue(const Header* pHeader) const { return pHeader->valueOffset + static_cast<f32>(data[1] + ((data[2] & 0xF) << 8)) * pHeader->valueScale; }
    f32 GetInSlope() const { return static_cast<f32>((static_cast<s8>(data[3]) << 4) + (data[2] >> 4)) * (1.0f / 32.0f); }
    f32 GetOutSlope() const { return GetInSlope(); }

    u8 data[4];
};

// format 6: float frame and value
struct LinearKeyF32
{
    typedef KeysHeader Header;
    typedef f32 Frame;
    typedef f32 Value;

    static f32 QuantizeFrame(const Header* /*pHeader*/, f32 frame) { return frame; }
    f32 GetRawFrame() const { return frame; }
    f32 GetFrame(const Header* /*pHeader*/) const { return frame; }
    f32 GetValue(const Header* /*pHeader*/) const { return value; }

    f32 frame; // 0x0
    f32 value; // 0x4
};

// format 7: a 12 bit frame and a 20 bit value
struct LinearKeyQ32
{
    typedef QuantizedKeysHeader Header;
    typedef u32 Frame;
    typedef f32 Value;

    static u32 QuantizeFrame(const Header* pHeader, f32 frame) { return static_cast<u32>(std::floor(frame / pHeader->frameScale + 0.5f)); }
    u32 GetRawFrame() const { return frameValue & 0xFFF; }
    f32 GetFrame(const Header* pHeader) const { return pHeader->frameScale * static_cast<f32>(GetRawFrame()); }
    f32 GetValue(const Header* pHeader) const { return pHeader->valueOffset + static_cast<f32>(frameValue >> 12) * pHeader->valueScale; }

    u32 frameValue; // 0x0
};

// the keys of the integer curves (step only): float frame and value
struct IntKeyF32
{
    typedef KeysHeader Header;
    typedef f32 Frame;
    typedef s32 Value;

    static f32 QuantizeFrame(const Header* /*pHeader*/, f32 frame) { return frame; }
    f32 GetRawFrame() const { return frame; }
    s32 GetValue(const Header* /*pHeader*/) const { return value; }

    f32 frame; // 0x0
    s32 value; // 0x4
};

// a 16 bit frame and value
struct IntKey16
{
    typedef KeysHeader Header;
    typedef u32 Frame;
    typedef s32 Value;

    static u32 QuantizeFrame(const Header* /*pHeader*/, f32 frame) { return static_cast<u32>(frame); }
    u32 GetRawFrame() const { return frame; }
    s32 GetValue(const Header* /*pHeader*/) const { return value; }

    u16 frame; // 0x0
    s16 value; // 0x2
};

// an 8 bit frame and value
struct IntKey8
{
    typedef KeysHeader Header;
    typedef u32 Frame;
    typedef s32 Value;

    static u32 QuantizeFrame(const Header* /*pHeader*/, f32 frame) { return static_cast<u32>(frame); }
    u32 GetRawFrame() const { return frame; }
    s32 GetValue(const Header* /*pHeader*/) const { return value; }

    u8 frame; // 0x0
    s8 value; // 0x1
};

template <typename Key>
const Key* GetKeys(const typename Key::Header* pHeader)
{
    return reinterpret_cast<const Key*>(pHeader + 1);
}

// the last key at or before the frame (the first key before the first one): from an index
// estimated by the frame on
template <typename Key>
inline const Key* FindKey(const typename Key::Header* pHeader, f32 frame)
{
    const typename Key::Frame keyFrame = Key::QuantizeFrame(pHeader, frame);
    const Key* pKeys = GetKeys<Key>(pHeader);
    if (pKeys[0].GetRawFrame() > keyFrame) {
        return &pKeys[0];
    }
    const Key* pLast = &pKeys[pHeader->keyCount - 1];
    if (keyFrame >= pLast->GetRawFrame()) {
        return pLast;
    }
    const Key* pKey = &pKeys[static_cast<u32>(pHeader->invDuration * (static_cast<f32>(pHeader->keyCount - 1) * frame))];
    if (keyFrame < pKey->GetRawFrame()) {
        do {
            --pKey;
        } while (pKey->GetRawFrame() > keyFrame);
        return pKey;
    }
    do {
        ++pKey;
    } while (keyFrame >= pKey->GetRawFrame());
    return pKey - 1;
}

// the value of the key at or before the frame
template <typename Key>
typename Key::Value CalcStepSegment(const void* pKeys, f32 frame)
{
    const typename Key::Header* pHeader = static_cast<const typename Key::Header*>(pKeys);
    return FindKey<Key>(pHeader, frame)->GetValue(pHeader);
}

// between the keys around the frame
template <typename Key>
f32 CalcLinearSegment(const void* pKeys, f32 frame)
{
    const typename Key::Header* pHeader = static_cast<const typename Key::Header*>(pKeys);
    const Key* pLast = &GetKeys<Key>(pHeader)[pHeader->keyCount - 1];
    const Key* pKey = FindKey<Key>(pHeader, frame);
    const f32 keyFrame = pKey->GetFrame(pHeader);
    if (keyFrame == frame || pKey == pLast) {
        return pKey->GetValue(pHeader);
    }
    const Key* pNext = pKey + 1;
    const f32 t = (frame - keyFrame) / (pNext->GetFrame(pHeader) - keyFrame);
    return pKey->GetValue(pHeader) * (1.0f - t) + pNext->GetValue(pHeader) * t;
}

// the hermite curve of the keys around the frame
template <typename Key>
f32 CalcHermiteSegment(const void* pKeys, f32 frame)
{
    const typename Key::Header* pHeader = static_cast<const typename Key::Header*>(pKeys);
    const Key* pLast = &GetKeys<Key>(pHeader)[pHeader->keyCount - 1];
    const Key* pKey = FindKey<Key>(pHeader, frame);
    const f32 keyFrame = pKey->GetFrame(pHeader);
    if (keyFrame == frame || pKey == pLast) {
        return pKey->GetValue(pHeader);
    }
    const Key* pNext = pKey + 1;
    return nn::math::ARMv6::HermiteC_FAST(pKey->GetValue(pHeader), pKey->GetOutSlope(), pNext->GetValue(pHeader), pNext->GetInSlope(), frame - keyFrame,
                                          pNext->GetFrame(pHeader) - keyFrame);
}

// the key search of the quantized formats is out of line in the original
// 0x007E7A18 (name is ours)
template const UnifiedHermiteKeyQ32* FindKey<UnifiedHermiteKeyQ32>(const QuantizedKeysHeader* pHeader, f32 frame);
// 0x007E7AC8 (name is ours)
template const UnifiedHermiteKeyQ48* FindKey<UnifiedHermiteKeyQ48>(const QuantizedKeysHeader* pHeader, f32 frame);
// 0x007E7B8C (name is ours)
template const HermiteKeyQ48* FindKey<HermiteKeyQ48>(const QuantizedKeysHeader* pHeader, f32 frame);
// 0x007E7C44 (name is ours)
template const HermiteKeyQ64* FindKey<HermiteKeyQ64>(const QuantizedKeysHeader* pHeader, f32 frame);

// the integer curves
// 0x007E7D1C (name is ours)
template s32 CalcStepSegment<IntKey8>(const void* pKeys, f32 frame);
// 0x007E7DA0 (name is ours)
template s32 CalcStepSegment<IntKey16>(const void* pKeys, f32 frame);
// 0x007E7E24 (name is ours)
template s32 CalcStepSegment<IntKeyF32>(const void* pKeys, f32 frame);

// the float curves
// 0x007E7ECC (name is ours)
template f32 CalcStepSegment<LinearKeyQ32>(const void* pKeys, f32 frame);
// 0x007E7FBC (name is ours)
template f32 CalcStepSegment<LinearKeyF32>(const void* pKeys, f32 frame);
// 0x007E8064 (name is ours)
template f32 CalcStepSegment<UnifiedHermiteKeyQ32>(const void* pKeys, f32 frame);
// 0x007E8098 (name is ours)
template f32 CalcStepSegment<UnifiedHermiteKeyQ48>(const void* pKeys, f32 frame);
// 0x007E80C0 (name is ours)
template f32 CalcStepSegment<UnifiedHermiteKeyF32>(const void* pKeys, f32 frame);
// 0x007E8170 (name is ours)
template f32 CalcStepSegment<HermiteKeyQ48>(const void* pKeys, f32 frame);
// 0x007E81A0 (name is ours)
template f32 CalcStepSegment<HermiteKeyQ64>(const void* pKeys, f32 frame);
// 0x007E81CC (name is ours)
template f32 CalcStepSegment<HermiteKeyF32>(const void* pKeys, f32 frame);
// 0x007E8278 (name is ours)
template f32 CalcLinearSegment<LinearKeyQ32>(const void* pKeys, f32 frame);
// 0x007E83FC (name is ours)
template f32 CalcLinearSegment<LinearKeyF32>(const void* pKeys, f32 frame);
// 0x007E84F0 (name is ours)
template f32 CalcLinearSegment<UnifiedHermiteKeyQ32>(const void* pKeys, f32 frame);
// 0x007E85DC (name is ours)
template f32 CalcLinearSegment<UnifiedHermiteKeyQ48>(const void* pKeys, f32 frame);
// 0x007E86BC (name is ours)
template f32 CalcLinearSegment<UnifiedHermiteKeyF32>(const void* pKeys, f32 frame);
// 0x007E87BC (name is ours)
template f32 CalcLinearSegment<HermiteKeyQ48>(const void* pKeys, f32 frame);
// 0x007E88A8 (name is ours)
template f32 CalcLinearSegment<HermiteKeyQ64>(const void* pKeys, f32 frame);
// 0x007E897C (name is ours)
template f32 CalcLinearSegment<HermiteKeyF32>(const void* pKeys, f32 frame);
// 0x007E8A7C (name is ours)
template f32 CalcHermiteSegment<UnifiedHermiteKeyQ32>(const void* pKeys, f32 frame);
// 0x007E8B90 (name is ours)
template f32 CalcHermiteSegment<UnifiedHermiteKeyQ48>(const void* pKeys, f32 frame);
// 0x007E8C84 (name is ours)
template f32 CalcHermiteSegment<UnifiedHermiteKeyF32>(const void* pKeys, f32 frame);
// 0x007E8D88 (name is ours)
template f32 CalcHermiteSegment<HermiteKeyQ48>(const void* pKeys, f32 frame);
// 0x007E8EA0 (name is ours)
template f32 CalcHermiteSegment<HermiteKeyQ64>(const void* pKeys, f32 frame);
// 0x007E8F90 (name is ours)
template f32 CalcHermiteSegment<HermiteKeyF32>(const void* pKeys, f32 frame);

typedef f32 (*FloatSegmentFunc)(const void* pKeys, f32 frame);
typedef s32 (*IntSegmentFunc)(const void* pKeys, f32 frame);

// by InterpolationType and key format
// 0x00975FD8 (name is ours)
const FloatSegmentFunc s_FloatSegmentFuncs[3][8] = {
    {CalcStepSegment<HermiteKeyF32>, CalcStepSegment<HermiteKeyQ64>, CalcStepSegment<HermiteKeyQ48>, CalcStepSegment<UnifiedHermiteKeyF32>,
     CalcStepSegment<UnifiedHermiteKeyQ48>, CalcStepSegment<UnifiedHermiteKeyQ32>, CalcStepSegment<LinearKeyF32>, CalcStepSegment<LinearKeyQ32>},
    {CalcLinearSegment<HermiteKeyF32>, CalcLinearSegment<HermiteKeyQ64>, CalcLinearSegment<HermiteKeyQ48>, CalcLinearSegment<UnifiedHermiteKeyF32>,
     CalcLinearSegment<UnifiedHermiteKeyQ48>, CalcLinearSegment<UnifiedHermiteKeyQ32>, CalcLinearSegment<LinearKeyF32>, CalcLinearSegment<LinearKeyQ32>},
    {CalcHermiteSegment<HermiteKeyF32>, CalcHermiteSegment<HermiteKeyQ64>, CalcHermiteSegment<HermiteKeyQ48>, CalcHermiteSegment<UnifiedHermiteKeyF32>,
     CalcHermiteSegment<UnifiedHermiteKeyQ48>, CalcHermiteSegment<UnifiedHermiteKeyQ32>},
};
// by key format
// 0x00976038 (name is ours)
const IntSegmentFunc s_IntSegmentFuncs[3] = {CalcStepSegment<IntKeyF32>, CalcStepSegment<IntKey16>, CalcStepSegment<IntKey8>};
} // namespace internal

// 0x004D5FB4 (name after fefates)
f32 nw::anim::res::ResAnimCurveData::NormalizeFrame(f32 frame) const
{
    if (startFrame > frame) {
        return s_PreRepeatFuncs[preRepeatMethod](frame, startFrame, endFrame);
    }
    if (endFrame > frame) {
        return frame;
    }
    return s_PostRepeatFuncs[postRepeatMethod](frame, startFrame, endFrame);
}

// 0x004D526C | nintendogs:bytes [tier A]
s32 CalcIntCurve(const ResIntCurveData* pCurve, f32 frame)
{
    frame = pCurve->NormalizeFrame(frame);
    if (pCurve->flags & ResIntCurveData::FLAG_CONSTANT) {
        return pCurve->constantValue;
    }
    return internal::s_IntSegmentFuncs[(pCurve->flags >> ResIntCurveData::KEY_FORMAT_SHIFT) & 7](&pCurve->keyCount, frame);
}

// 0x004D52AC | nintendogs:bytes-fuzzy [tier A]
bool CalcBoolCurve(const ResBoolCurveData* pCurve, f32 frame)
{
    frame = pCurve->NormalizeFrame(frame);
    if (pCurve->flags & ResBoolCurveData::FLAG_CONSTANT) {
        return (pCurve->flags & ResBoolCurveData::FLAG_CONSTANT_VALUE) >> 1;
    }
    if (pCurve->flags & ResBoolCurveData::FLAG_BAKED) {
        f32 index = frame - pCurve->startFrame;
        if (pCurve->endFrame == frame) {
            index = std::ceil(index);
        }
        const u32 bit = static_cast<u32>(index);
        return (pCurve->bits[bit >> 3] >> (bit & 7)) & 1;
    }
    return internal::s_IntSegmentFuncs[(pCurve->flags >> ResBoolCurveData::KEY_FORMAT_SHIFT) & 7](&pCurve->keyCount, frame) != 0;
}

// 0x004D5334 | nintendogs:bytes [tier A]
f32 CalcFloatCurve(const ResFloatCurveData* pCurve, f32 frame)
{
    frame = pCurve->NormalizeFrame(frame);
    if (pCurve->flags & ResFloatCurveData::FLAG_COMPOSITE) {
        const ResFloatCurveData* pCurve0 = pCurve->firstCurve.To<ResFloatCurveData>();
        const ResFloatCurveData* pCurve1 = pCurve->secondCurve.To<ResFloatCurveData>();
        return CalcFloatCurve(pCurve0, frame) + CalcFloatCurve(pCurve1, frame);
    }
    if (pCurve->flags & ResFloatCurveData::FLAG_CONSTANT) {
        return pCurve->constantValue;
    }

    const ResFloatSegmentData* pSegment = FindSegment(pCurve, frame);
    if (pSegment->startFrame > frame) {
        frame = pSegment->startFrame;
    }
    if (pSegment->flags & ResFloatSegmentData::FLAG_CONSTANT) {
        return pSegment->constantValue;
    }
    const u32 interpolation = (pSegment->flags >> ResFloatSegmentData::INTERPOLATION_SHIFT) & 7;
    const u32 format = (pSegment->flags >> ResFloatSegmentData::KEY_FORMAT_SHIFT) & 7;
    return internal::s_FloatSegmentFuncs[interpolation][format](&pSegment->keyCount, frame - pSegment->startFrame);
}

// 0x004D5540 (name is ours)
void CalcRotateCurve(nn::math::MTX34* pMtx, u32* pFlags, const ResBakedCurveData<nn::math::QUAT>* pCurve, f32 frame)
{
    frame = pCurve->NormalizeFrame(frame);
    if (pCurve->flags & ResBakedCurveData<nn::math::QUAT>::FLAG_CONSTANT) {
        frame = pCurve->startFrame;
    }
    const u32 index = static_cast<u32>(frame);
    const f32 t = frame - static_cast<f32>(index);
    const ResBakedKey<nn::math::QUAT>* pKey = &pCurve->keys[index];

    nn::math::MTX34 mtx;
    nn::math::QUAT quat;
    if (t == 0.0f) {
        quat = pKey->value;
        nn::math::ARMv6::QUATToMTX34C_FAST(&mtx, &quat, true);
        *pFlags |= pKey->flags;
    } else if (pCurve->endFrame == frame) {
        pKey = &pCurve->keys[index + 1];
        quat = pKey->value;
        nn::math::ARMv6::QUATToMTX34C_FAST(&mtx, &quat, true);
        *pFlags |= pKey->flags;
    } else {
        const ResBakedKey<nn::math::QUAT>* pNext = &pCurve->keys[static_cast<u32>(pCurve->NormalizeFrame(frame + 1.0f))];
        quat.x = pKey->value.x + t * (pNext->value.x - pKey->value.x);
        quat.y = pKey->value.y + t * (pNext->value.y - pKey->value.y);
        quat.z = pKey->value.z + t * (pNext->value.z - pKey->value.z);
        quat.w = pKey->value.w + t * (pNext->value.w - pKey->value.w);
        nn::math::ARMv6::QUATToMTX34C_FAST(&mtx, &quat, true);
        *pFlags |= pKey->flags & pNext->flags;
    }

    // only the rotation: the translation stays
    pMtx->m[0][0] = mtx.m[0][0];
    pMtx->m[0][1] = mtx.m[0][1];
    pMtx->m[0][2] = mtx.m[0][2];
    pMtx->m[1][0] = mtx.m[1][0];
    pMtx->m[1][1] = mtx.m[1][1];
    pMtx->m[1][2] = mtx.m[1][2];
    pMtx->m[2][0] = mtx.m[2][0];
    pMtx->m[2][1] = mtx.m[2][1];
    pMtx->m[2][2] = mtx.m[2][2];
}

// 0x004D5724 | nintendogs:bytes [tier A]
void CalcVector3Curve(nn::math::VEC3* pValue, u32* pFlags, const ResBakedCurveData<nn::math::VEC3_>* pCurve, f32 frame)
{
    if (pCurve->flags & ResBakedCurveData<nn::math::VEC3_>::FLAG_CONSTANT) {
        frame = pCurve->startFrame;
    } else {
        frame = pCurve->NormalizeFrame(frame);
    }
    const u32 index = static_cast<u32>(frame);
    const f32 t = frame - static_cast<f32>(index);
    const ResBakedKey<nn::math::VEC3_>* pKey = &pCurve->keys[index];

    if (t == 0.0f) {
        *pValue = pKey->value;
        *pFlags |= pKey->flags;
        return;
    }
    if (pCurve->endFrame == frame) {
        pKey = &pCurve->keys[index + 1];
        *pValue = pKey->value;
        *pFlags |= pKey->flags;
        return;
    }
    const ResBakedKey<nn::math::VEC3_>* pNext = &pCurve->keys[static_cast<u32>(pCurve->NormalizeFrame(frame + 1.0f))];
    pValue->x = pKey->value.x + t * (pNext->value.x - pKey->value.x);
    pValue->y = pKey->value.y + t * (pNext->value.y - pKey->value.y);
    pValue->z = pKey->value.z + t * (pNext->value.z - pKey->value.z);
    *pFlags |= pKey->flags & pNext->flags;
}

// 0x004D5CA0 | nintendogs:bytes-fuzzy [tier A]
void CalcTransformCurve(nn::math::MTX34* pMtx, const ResFullBakedCurveData* pCurve, f32 frame)
{
    frame = pCurve->NormalizeFrame(frame);
    u32 index = static_cast<u32>(frame);
    if (pCurve->endFrame == frame) {
        ++index;
    }
    nn::math::ARMv6::MTX34CopyAsm(pMtx, &pCurve->keys[index]);
}

// 0x004D5CE4 | nintendogs:bytes [tier A]
void CalcTranslateCurve(nn::math::MTX34* pMtx, u32* pFlags, const ResBakedCurveData<nn::math::VEC3_>* pCurve, f32 frame)
{
    if (pCurve->flags & ResBakedCurveData<nn::math::VEC3_>::FLAG_CONSTANT) {
        frame = pCurve->startFrame;
    } else {
        frame = pCurve->NormalizeFrame(frame);
    }
    const u32 index = static_cast<u32>(frame);
    const f32 t = frame - static_cast<f32>(index);
    const ResBakedKey<nn::math::VEC3_>* pKey = &pCurve->keys[index];

    if (t == 0.0f) {
        pMtx->m[0][3] = pKey->value.x;
        pMtx->m[1][3] = pKey->value.y;
        pMtx->m[2][3] = pKey->value.z;
        *pFlags |= pKey->flags;
        return;
    }
    if (pCurve->endFrame == frame) {
        pKey = &pCurve->keys[index + 1];
        pMtx->m[0][3] = pKey->value.x;
        pMtx->m[1][3] = pKey->value.y;
        pMtx->m[2][3] = pKey->value.z;
        *pFlags |= pKey->flags;
        return;
    }
    const ResBakedKey<nn::math::VEC3_>* pNext = &pCurve->keys[static_cast<u32>(pCurve->NormalizeFrame(frame + 1.0f))];
    pMtx->m[0][3] = pKey->value.x + t * (pNext->value.x - pKey->value.x);
    pMtx->m[1][3] = pKey->value.y + t * (pNext->value.y - pKey->value.y);
    pMtx->m[2][3] = pKey->value.z + t * (pNext->value.z - pKey->value.z);
    *pFlags |= pKey->flags & pNext->flags;
}

} // namespace res
} // namespace anim
} // namespace nw
