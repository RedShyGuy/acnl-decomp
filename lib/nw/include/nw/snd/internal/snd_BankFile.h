#pragma once

#include "decomp.h"
#include "nw/snd/internal/Util/snd_BitFlag.h"
#include "nw/snd/internal/snd_BinaryTypes.h"
#include "nw/ut/ut_Api.h"

namespace nw {
namespace snd {
namespace internal {
// The parts of a bank file (BCBNK; the class names are from the reference symbols, the members are
// ours): instruments by program number, their key regions and velocity regions, which name a wave
// of a wave archive.
class BankFile
{
public:
    static const u32 SIGNATURE_FILE = 0x4B4E4243; // 'CBNK'
    static const u32 VERSION_MIN = 0x01000000;
    static const u32 VERSION_MAX = 0x01000100;
    static const u16 TYPE_INFO_BLOCK = 0x5800;
    static const u16 TYPE_INSTRUMENT = 0x5900;
    static const u16 TYPE_NULL_INSTRUMENT = 0x5903;
    // how the regions are found by key / velocity
    static const u16 TYPE_DIRECT_REFERENCE_TABLE = 0x6000; // one region
    static const u16 TYPE_RANGE_REFERENCE_TABLE = 0x6001;  // the upper ends of the ranges
    static const u16 TYPE_INDEX_REFERENCE_TABLE = 0x6002;  // a region per value from a minimum

    class FileHeader;
    class InfoBlockBody;
    class Instrument;
    class KeyRegion;
    class VelocityRegion;

    // (inline) the region of a value in a region table (NULL: none)
    static const void* GetRegion(const BinaryReference& regionTable, const void* pBase, u32 value)
    {
        const u8* pTable = static_cast<const u8*>(GetReferedPointer(pBase, regionTable));
        switch (regionTable.typeId) {
        case TYPE_DIRECT_REFERENCE_TABLE:
            return GetReferedPointer(pTable, *reinterpret_cast<const BinaryReference*>(pTable));
        case TYPE_RANGE_REFERENCE_TABLE: {
            const u32 count = *reinterpret_cast<const u32*>(pTable);
            for (u32 i = 0; i < count; ++i) {
                if (pTable[sizeof(u32) + i] >= value) {
                    const BinaryReference* pReferences = reinterpret_cast<const BinaryReference*>(pTable + sizeof(u32) + ut::RoundUp(count, 4));
                    return GetReferedPointer(pTable, pReferences[i]);
                }
            }
            return nullptr;
        }
        case TYPE_INDEX_REFERENCE_TABLE: {
            const u8 min = pTable[0];
            const u8 max = pTable[1];
            if (value < min || value > max) {
                return nullptr;
            }
            const BinaryReference* pReferences = reinterpret_cast<const BinaryReference*>(pTable + sizeof(u32));
            return GetReferedPointer(pTable, pReferences[value - min]);
        }
        default:
            return nullptr;
        }
    }
};

// what a velocity region of a bank gives (the struct name is from the reference symbols, the members
// are ours)
struct VelocityRegionInfo
{
    u32 waveArchiveId;        // 0x00
    u32 waveIndex;            // 0x04
    f32 pitch;                // 0x08
    AdshrCurve adshrCurve;    // 0x0C
    u8 originalKey;           // 0x11
    u8 volume;                // 0x12
    u8 pan;                   // 0x13
    bool isIgnoreNoteOff;     // 0x14
    u8 keyGroup;              // 0x15
    u8 interpolationType;     // 0x16
};
ASSERT_SIZE(VelocityRegionInfo, 0x18);
} // namespace internal
} // namespace snd
} // namespace nw
