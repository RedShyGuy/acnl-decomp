#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/fs/fs_FileStream.h"

namespace nn {
namespace pl {
namespace CTR {
// The play coins: the shared extra save data 0xF000000B holds gamecoin.dat; the steps of the
// pedometer become coins (one per 100 steps, at most 10 a day and 300 in all). The names of the
// functions and of the members are ours.

// the file gamecoin.dat
struct GameCoinData
{
    u32 magic;        // 0x00 (MAGIC)
    u16 coins;        // 0x04, at most 300
    u16 coinsToday;   // 0x06, the coins of the day of the last update, at most 10
    u32 totalSteps;   // 0x08, the total step count at the last update (less the steps of a part coin)
    u32 todaySteps;   // 0x0C, the steps of that day that are counted
    s16 year;         // 0x10, the day of the last update
    s8 month;         // 0x12
    s8 day;           // 0x13
};
ASSERT_SIZE(GameCoinData, 0x14);

// the coins (the steps since the last call are counted first); the result tells whether the file
// was made new
nn::Result GetGameCoinCount(u16* pCount); // 0x0011DFB8 (name is ours)

// mounts the save data and makes the file if there is none (once)
void InitializeGameCoin(); // 0x0011E078 (name is ours)
DECOMP_NOINLINE void FinalizeGameCoin(); // 0x00130964 (name is ours)
// FinalizeGameCoin and ptm::CTR::Finalize
nn::Result Finalize(); // 0x0013111C (name is ours)

namespace detail {
void MountGameCoin(); // 0x0011F700 (name is ours)
void UnmountGameCoin(); // 0x001365F4 (name is ours)
// counts the steps since the last update as coins
void UpdateGameCoin(GameCoinData* data); // 0x0011F738 (name is ours)
// opens the file (made new if needed: true); retries while it is busy
bool OpenGameCoinFile(nn::fs::FileStream* pStream); // 0x00123E00 (name is ours)
// reads the file (made new if it was broken: true)
bool ReadGameCoinFile(nn::fs::FileStream* pStream, GameCoinData* data); // 0x0011FB04 (name is ours)
// writes the data and closes the file
void WriteGameCoinFile(nn::fs::FileStream* pStream, const GameCoinData* data); // 0x00123F78 (name is ours)
// makes the file for today if there is none
void CreateGameCoinFile(); // 0x00124078 (name is ours)
} // namespace detail
} // namespace CTR
} // namespace pl
} // namespace nn
