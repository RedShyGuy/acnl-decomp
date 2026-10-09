#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace ro {
// the kinds of segments of a module (3dbrew "CRO0")
enum SegmentType : u8
{
    SEGMENT_TYPE_CODE = 0,
    SEGMENT_TYPE_RODATA = 1,
    SEGMENT_TYPE_DATA = 2,
    SEGMENT_TYPE_BSS = 3
};

// an entry of the segment table; the offset is an address once the module is loaded
struct Segment
{
    uptr m_Offset;          // 0x0
    size_t m_Size;          // 0x4
    u8 m_Type;              // 0x8, SegmentType
    u8 m_Padding9[3];       // 0x9
};
ASSERT_SIZE(Segment, 0xC);

// the region of a module in memory (names are ours)
struct RegionInfo
{
    const class Module* m_Module;   // 0x00
    size_t m_Size;                  // 0x04, the fixed size
    uptr m_LoadAddress;             // 0x08
    size_t m_FixedSize;             // 0x0C
    uptr m_DataAddress;             // 0x10, .data and .bss
    size_t m_DataSize;              // 0x14
    uptr m_CodeAddress;             // 0x18
    size_t m_CodeSize;              // 0x1C
};
ASSERT_SIZE(RegionInfo, 0x20);

// the sizes a module can be fixed to (by fix level), and the size of .data and .bss (names
// are ours)
struct SizeInfo
{
    size_t m_FixedSizes[4];         // 0x00, the ends of the module from fix level 0 on
    size_t m_BufferSize;            // 0x10, .data and .bss
};
ASSERT_SIZE(SizeInfo, 0x14);

// A module (CRO): the module image itself, with its header at 0x80 (3dbrew "CRO0"; the member
// names follow it). The names of the functions are ours.
class Module
{
public:
    class EnumerateCallback;

    static const u32 MAGIC = 0x304F5243; // "CRO0"
    static const size_t HEADER_END = 0x138;

    // a function behind a segment offset (segment index in the low 4 bits): 0 if there is none
    uptr GetSegmentAddress(u32 segmentOffset) const
    {
        u32 index = segmentOffset & 0xF;
        u32 offset = segmentOffset >> 4;
        if (index < m_SegmentTableCount) {
            const Segment& segment = reinterpret_cast<const Segment*>(m_SegmentTableOffset)[index];
            if (offset < segment.m_Size) {
                return segment.m_Offset + offset;
            }
        }
        return 0;
    }

    DECOMP_NOIPA static void Enumerate(EnumerateCallback* callback); // 0x00129B18 (name is ours)
    nn::Result Unload(); // 0x0013B1B4 (name is ours)
    // the sizes for Load (cro: the module file)
    static nn::Result GetSizeInfo(SizeInfo* info, const void* cro); // 0x0034CC64 (name is ours)
    // loads the module file cro; buffer gets .data and .bss (SizeInfo::m_BufferSize)
    static Module* Load(const void* cro, size_t croSize, void* buffer, size_t bufferSize, bool isAutoLink, u32 fixLevel, uptr crr); // 0x0034CB08 (name is ours)
    // OnExit and the destructors of the global objects
    bool DoFinalize(); // 0x0034CD7C (name is ours)
    // the constructors of the global objects and OnLoad
    bool DoInitialize(); // 0x0034CE10 (name is ours)
    void GetRegionInfo(RegionInfo* info) const; // 0x0034CF54 (name is ours)

    u8 m_Hashes[0x80];                  // 0x000
    u32 m_Magic;                        // 0x080
    u32 m_NameOffset;                   // 0x084
    Module* m_Next;                     // 0x088
    Module* m_Previous;                 // 0x08C
    size_t m_FileSize;                  // 0x090
    size_t m_BssSize;                   // 0x094
    size_t m_FixedSize;                 // 0x098
    u32 m_Unknown9C;                    // 0x09C
    u32 m_ControlObject;                // 0x0A0, segment offset
    u32 m_OnLoad;                       // 0x0A4
    u32 m_OnExit;                       // 0x0A8
    u32 m_OnUnresolved;                 // 0x0AC
    uptr m_CodeOffset;                  // 0x0B0
    size_t m_CodeSize;                  // 0x0B4
    uptr m_DataOffset;                  // 0x0B8
    size_t m_DataSize;                  // 0x0BC
    uptr m_ModuleNameOffset;            // 0x0C0
    size_t m_ModuleNameSize;            // 0x0C4
    uptr m_SegmentTableOffset;          // 0x0C8
    s32 m_SegmentTableCount;            // 0x0CC
    uptr m_NamedExportTableOffset;      // 0x0D0
    u32 m_NamedExportTableCount;        // 0x0D4
    uptr m_IndexedExportTableOffset;    // 0x0D8
    u32 m_IndexedExportTableCount;      // 0x0DC
    uptr m_ExportStringsOffset;         // 0x0E0
    size_t m_ExportStringsSize;         // 0x0E4
    uptr m_ExportTrieOffset;            // 0x0E8
    u32 m_ExportTrieCount;              // 0x0EC
    uptr m_ImportModuleTableOffset;     // 0x0F0
    u32 m_ImportModuleTableCount;       // 0x0F4
    uptr m_ImportRelocationsOffset;     // 0x0F8
    u32 m_ImportRelocationsCount;       // 0x0FC
    uptr m_NamedImportTableOffset;      // 0x100
    u32 m_NamedImportTableCount;        // 0x104
    uptr m_IndexedImportTableOffset;    // 0x108
    u32 m_IndexedImportTableCount;      // 0x10C
    uptr m_AnonymousImportTableOffset;  // 0x110
    u32 m_AnonymousImportTableCount;    // 0x114
    uptr m_ImportStringsOffset;         // 0x118
    size_t m_ImportStringsSize;         // 0x11C
    uptr m_Unknown120Offset;            // 0x120, 8 byte entries
    u32 m_Unknown120Count;              // 0x124
    uptr m_InternalRelocationsOffset;   // 0x128
    u32 m_InternalRelocationsCount;     // 0x12C
    uptr m_UnknownRelocationsOffset;    // 0x130
    u32 m_UnknownRelocationsCount;      // 0x134
};
ASSERT_SIZE(Module, 0x138);

// A registration list (CRR, 3dbrew "CRR0"): the hashes of the modules that may be loaded.
// The names are ours (after 3dbrew).
class RegistrationList
{
public:
    // unregisters the list (the previous or next one is the current list then)
    nn::Result Unregister(); // 0x0013B170 (name is ours)

    u32 m_Magic;                        // 0x000
    u32 m_Reserved4;                    // 0x004
    RegistrationList* m_Next;           // 0x008
    RegistrationList* m_Previous;       // 0x00C
    u32 m_DebugInfoOffset;              // 0x010
    size_t m_DebugInfoSize;             // 0x014
    u8 m_Unknown18[0x350 - 0x18];       // 0x018, ids, keys, signatures
    u32 m_HashTableOffset;              // 0x350
    s32 m_HashCount;                    // 0x354
    u32 m_PlainRegionOffset;            // 0x358
    size_t m_PlainRegionSize;           // 0x35C
};
ASSERT_SIZE(RegistrationList, 0x360);

// sets up nn::ro with the static module (CRS) of the program
nn::Result Initialize(uptr crs, size_t size); // 0x0011FC70 (name is ours)
// registers the registration list (CRR) crr; null if it fails
RegistrationList* RegisterList(uptr crr, size_t size); // 0x0011FC9C (name is ours)
} // namespace ro
} // namespace nn
