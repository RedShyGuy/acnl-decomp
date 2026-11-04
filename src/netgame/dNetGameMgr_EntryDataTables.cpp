// the tables of the EntryData (in their own file: GCC would fold them into the functions)
#include "netgame/dNetGameMgr.h"

namespace netgame {
// the frames of the telop in front of a tour (name is ours)
// 0x0087CD24
extern const u32 s_TourTelopFrames = 0xB4;
namespace packet {
// the functions of the game for the blocks (not decompiled yet; named after their address)
bool EntryBuild1849A4(void* data, u32 no, const void* param, u32 size); // 0x001849A4
bool EntryBuild185CB8(void* data, u32 no, const void* param, u32 size); // 0x00185CB8
void EntryApply185CB4(u32 sender, const void* data, u32 size); // 0x00185CB4
void EntryApply186AD0(u32 sender, const void* data, u32 size); // 0x00186AD0
void EntryFlagBuild186228(void* data, u32 no, u32 size); // 0x00186228
void EntryFlagApply186EA4(u32 no, void* data, u32 size); // 0x00186EA4
void EntrySave184374(void* data, u32 no, u32 size); // 0x00184374
void EntrySave185000(void* data, u32 no, u32 size); // 0x00185000
void EntrySave185C00(void* data, u32 no, u32 size); // 0x00185C00
void EntryLoad1852B4(u32 no, void* data, u32 size); // 0x001852B4
void EntryLoad18619C(u32 no, void* data, u32 size); // 0x0018619C
void EntryLoad186A50(u32 no, void* data, u32 size); // 0x00186A50
} // namespace packet

using namespace packet;

// 0x0087CD2C
const u32 netgame::NetGameMgr::EntryData::s_PlayerFlagSize = 1;
// 0x0087CD30
const u32 netgame::NetGameMgr::EntryData::s_BufferSize = 0x89D;
// 0x0087CD34
const u32 netgame::NetGameMgr::EntryData::s_FlagSize = 1;
// 0x0087CD38
const u32 netgame::NetGameMgr::EntryData::s_FlagOffset = 0;
// 0x0087CD3C
const netgame::NetGameMgr::EntryData::FlagBuildFunction netgame::NetGameMgr::EntryData::s_FlagBuildFunction = EntryFlagBuild186228;
// 0x0087CD40
const netgame::NetGameMgr::EntryData::FlagApplyFunction netgame::NetGameMgr::EntryData::s_FlagApplyFunction = EntryFlagApply186EA4;
// 0x0087CD44
const u32 netgame::NetGameMgr::EntryData::s_SectionSizes[2] = {1, 1};
// 0x0087CD4C
const netgame::NetGameMgr::EntryData::SectionBuildFunction netgame::NetGameMgr::EntryData::s_SectionBuildFunctions[2] = {
    EntryBuild1849A4,
    EntryBuild185CB8,
};
// 0x0087CD54
const netgame::NetGameMgr::EntryData::SectionApplyFunction netgame::NetGameMgr::EntryData::s_SectionApplyFunctions[2] = {
    EntryApply185CB4,
    EntryApply186AD0,
};
// 0x0087CD5C
const u32 netgame::NetGameMgr::EntryData::s_DataSizes[3] = {1, 0xB4, 0x7E8};
// 0x0087CD68
const u32 netgame::NetGameMgr::EntryData::s_DataOffsets[3] = {0, 1, 0xB5};
// 0x0087CD74
const netgame::NetGameMgr::EntryData::DataSaveFunction netgame::NetGameMgr::EntryData::s_DataSaveFunctions[3] = {
    EntrySave184374,
    EntrySave185000,
    EntrySave185C00,
};
// 0x0087CD80
const netgame::NetGameMgr::EntryData::DataLoadFunction netgame::NetGameMgr::EntryData::s_DataLoadFunctions[3] = {
    EntryLoad1852B4,
    EntryLoad18619C,
    EntryLoad186A50,
};
} // namespace netgame
