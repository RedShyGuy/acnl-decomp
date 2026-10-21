#include "nn/pl/CTR/pl_GameCoin.h"
#include "nn/err/CTR/CTR_Api.h"
#include "nn/fnd/fnd_DateTime.h"
#include "nn/fs/detail/detail_Api.h"
#include "nn/fs/fs_Api.h"
#include "nn/os/os_Thread.h"
#include "nn/pl/CTR/CTR_Api.h"
#include "nn/ptm/CTR/CTR_Api.h"

namespace nn {
namespace pl {
namespace CTR {
namespace {
// the shared extra save data of the play coins
const u32 GAME_COIN_SAVE_ID = 0xF000000B;
const u32 MAGIC = 0x4F00;
const u16 MAX_COINS = 300;
const u16 MAX_COINS_PER_DAY = 10;
const u32 STEPS_PER_COIN = 100;
const u32 HOURS_PER_DAY = 24;
// GetGameCoinCount: the file was made new (level success, module 69, description 100)
const bit32 RESULT_GAME_COIN_CREATED = 0x00011464;

// the results of fs that the functions handle (module 17; the ranges of the descriptions are from
// the code, the names are ours, after what the code does with them)
const bit32 MODULE_FS = 17;
inline bool IsFsResultInRange(nn::Result result, int begin, int end)
{
    return result.GetModule() == MODULE_FS && begin <= static_cast<int>(result.GetDescription()) &&
           static_cast<int>(result.GetDescription()) <= end;
}
inline bool IsNotFound(nn::Result result)
{
    return IsFsResultInRange(result, 100, 179);
}
inline bool IsAlreadyExisting(nn::Result result)
{
    return IsFsResultInRange(result, 180, 199);
}
// (the file is opened again 10 ms later)
inline bool IsBusy(nn::Result result)
{
    return IsFsResultInRange(result, 230, 339);
}
// (the file is deleted and made new)
inline bool IsBroken(nn::Result result)
{
    return IsFsResultInRange(result, 360, 389) || IsFsResultInRange(result, 390, 399);
}

// the steps of the 24 hours from start
inline u32 GetStepsOfDay(s64 start)
{
    u16 steps[HOURS_PER_DAY];
    GetStepHistory(steps, HOURS_PER_DAY, start);
    u32 sum = 0;
    for (u32 i = 0; i < HOURS_PER_DAY; i++) {
        sum += steps[i];
    }
    return sum;
}

// 00:00 of today
inline nn::fnd::DateTime GetToday(nn::fnd::DateTimeParameters* pParameters)
{
    *pParameters = nn::fnd::DateTime::GetNow().GetParameters();
    pParameters->hour = 0;
    pParameters->minute = 0;
    pParameters->second = 0;
    pParameters->milliSecond = 0;
    return nn::fnd::DateTime::FromParameters(*pParameters);
}
} // namespace

// (names are ours)
// 0x0097F01C
const char* s_GameCoinMountName = "$coin";
// 0x0097F020
const wchar_t* s_GameCoinPath = L"$coin:/gamecoin.dat";
// 0x00975F96
bool s_IsGameCoinInitialized;

namespace detail {
// 0x0011F700 (name is ours)
void MountGameCoin()
{
    nn::Result result = nn::fs::MountSharedExtSaveData(s_GameCoinMountName, GAME_COIN_SAVE_ID);
    nn::err::CTR::ThrowFatalErrLoggedIfFailure(result);
}

// 0x001365F4 (name is ours)
void UnmountGameCoin()
{
    nn::Result result = nn::fs::Unmount(s_GameCoinMountName);
    nn::err::CTR::ThrowFatalErrLoggedIfFailure(result);
}

// 0x0011F738 (name is ours)
void UpdateGameCoin(GameCoinData* data)
{
    nn::fnd::DateTimeParameters parameters;
    nn::fnd::DateTime today = GetToday(&parameters);
    nn::fnd::DateTime last = nn::fnd::DateTime::FromParameters(data->year, data->month, data->day, 0, 0, 0, 0);
    u32 totalSteps = GetTotalStepCount();

    // a new day, a new counter or a full purse: the counting starts again from now
    if (today < last || data->totalSteps > totalSteps || data->coins >= MAX_COINS ||
        (today == last && data->coinsToday >= MAX_COINS_PER_DAY)) {
        if (today != last) {
            data->coinsToday = 0;
        }
        data->totalSteps = totalSteps;
        data->todaySteps = GetStepsOfDay(today.GetMilliSecondsSince2000());
    } else {
        if (totalSteps - data->totalSteps < STEPS_PER_COIN) {
            return;
        }
        // the coins of the days since the last update, day by day
        for (nn::fnd::DateTime date = last; !(today < date); date += nn::fnd::TimeSpan::FromMilliSeconds(24 * 60 * 60 * 1000)) {
            u32 daySteps = GetStepsOfDay(date.GetMilliSecondsSince2000());
            u32 countedSteps = 0;
            u16 coinsOfDay = 0;
            if (date == last) {
                countedSteps = static_cast<u16>(data->todaySteps);
                coinsOfDay = data->coinsToday;
            }
            s64 newSteps = static_cast<s64>(daySteps) - countedSteps;
            u32 steps = newSteps > 0 ? daySteps - countedSteps : 0;
            u16 coins = steps / STEPS_PER_COIN;
            u32 rest = steps % STEPS_PER_COIN;
            if (coinsOfDay + coins >= MAX_COINS_PER_DAY) {
                s32 free = MAX_COINS_PER_DAY - coinsOfDay;
                coins = free > 0 ? free : 0;
                rest = 0;
            }
            if (data->coins + coins >= MAX_COINS) {
                s32 free = MAX_COINS - data->coins;
                coins = free > 0 ? free : 0;
                rest = 0;
            }
            if (date == today) {
                // the steps of a part coin are counted again next time
                data->coinsToday = coinsOfDay + coins;
                data->totalSteps = static_cast<s64>(totalSteps) - rest > 0 ? totalSteps - rest : 0;
                data->todaySteps = static_cast<s64>(daySteps) - rest > 0 ? daySteps - rest : 0;
            }
            data->coins += coins;
        }
    }
    data->year = parameters.year;
    data->month = parameters.month;
    data->day = parameters.day;
    if (data->coins > MAX_COINS) {
        data->coins = MAX_COINS;
    }
}

// 0x00123E00 (name is ours)
bool OpenGameCoinFile(nn::fs::FileStream* pStream)
{
    bool isCreated = false;
    while (true) {
        nn::Result result = pStream->TryInitialize(s_GameCoinPath, nn::fs::OPEN_MODE_READ | nn::fs::OPEN_MODE_WRITE);
        if (result.IsSuccess()) {
            return isCreated;
        }
        if (IsBusy(result)) {
            nn::os::Thread::SleepImpl(nn::fnd::TimeSpan::FromMilliSeconds(10));
            continue;
        }
        if (IsNotFound(result)) {
            CreateGameCoinFile();
            isCreated = true;
            continue;
        }
        if (IsBroken(result)) {
            nn::Result deleteResult = nn::fs::detail::GetGlobalFileSystemBase()->TryDeleteFile(s_GameCoinPath);
            if (!IsNotFound(deleteResult)) {
                nn::err::CTR::ThrowFatalErrLoggedIfFailure(deleteResult);
            }
            CreateGameCoinFile();
            isCreated = true;
            continue;
        }
        nn::err::CTR::ThrowFatalErrLoggedIfFailure(result);
    }
}

// 0x0011FB04 (name is ours)
bool ReadGameCoinFile(nn::fs::FileStream* pStream, GameCoinData* data)
{
    bool isCreated = OpenGameCoinFile(pStream);
    s32 readSize;
    nn::Result result = pStream->TryRead(&readSize, data, sizeof(GameCoinData));
    if (result.IsFailure()) {
        pStream->Finalize();
        nn::err::CTR::ThrowFatalErrLoggedIfFailure(result);
    }
    if (data->magic != MAGIC) {
        // a broken file: a new one
        pStream->Finalize();
        nn::Result deleteResult = nn::fs::detail::GetGlobalFileSystemBase()->TryDeleteFile(s_GameCoinPath);
        if (!IsNotFound(deleteResult)) {
            nn::err::CTR::ThrowFatalErrLoggedIfFailure(deleteResult);
        }
        CreateGameCoinFile();
        isCreated = true;
        OpenGameCoinFile(pStream);
        result = pStream->TryRead(&readSize, data, sizeof(GameCoinData));
        if (result.IsFailure()) {
            pStream->Finalize();
            nn::err::CTR::ThrowFatalErrLoggedIfFailure(result);
        }
    }
    return isCreated;
}

// 0x00123F78 (name is ours)
void WriteGameCoinFile(nn::fs::FileStream* pStream, const GameCoinData* data)
{
    nn::Result result = pStream->TrySeek(0, nn::fs::POSITION_BASE_BEGIN);
    if (result.IsFailure()) {
        pStream->Finalize();
        nn::err::CTR::ThrowFatalErrLoggedIfFailure(result);
    }
    s32 writtenSize;
    result = pStream->TryWrite(&writtenSize, data, sizeof(GameCoinData), true);
    if (result.IsFailure()) {
        pStream->Finalize();
        nn::err::CTR::ThrowFatalErrLoggedIfFailure(result);
    }
    pStream->Finalize();
}

// 0x00124078 (name is ours)
void CreateGameCoinFile()
{
    nn::Result result = nn::fs::detail::GetGlobalFileSystemBase()->TryCreateFile(s_GameCoinPath, sizeof(GameCoinData));
    if (result.IsFailure()) {
        if (!IsAlreadyExisting(result)) {
            nn::err::CTR::ThrowFatalErrLoggedIfFailure(result);
        }
        return;
    }
    nn::fnd::DateTimeParameters parameters;
    nn::fnd::DateTime today = GetToday(&parameters);
    GameCoinData data;
    data.totalSteps = GetTotalStepCount();
    data.coinsToday = 0;
    data.todaySteps = GetStepsOfDay(today.GetMilliSecondsSince2000());
    data.year = parameters.year;
    data.month = parameters.month;
    data.day = parameters.day;
    data.magic = MAGIC;
    data.coins = 0;

    nn::fs::FileStream stream;
    OpenGameCoinFile(&stream);
    WriteGameCoinFile(&stream, &data);
}
} // namespace detail

// 0x0011DFB8 (name is ours)
nn::Result GetGameCoinCount(u16* pCount)
{
    nn::fs::FileStream stream;
    GameCoinData data;
    bool isCreated = detail::ReadGameCoinFile(&stream, &data);
    detail::UpdateGameCoin(&data);
    detail::WriteGameCoinFile(&stream, &data);
    *pCount = data.coins;
    if (isCreated) {
        return RESULT_GAME_COIN_CREATED;
    }
    return nn::Result();
}

// 0x0011E078 (name is ours)
void InitializeGameCoin()
{
    if (!s_IsGameCoinInitialized) {
        detail::MountGameCoin();
        detail::CreateGameCoinFile();
        s_IsGameCoinInitialized = true;
    }
}

// 0x00130964 (name is ours)
void FinalizeGameCoin()
{
    if (s_IsGameCoinInitialized) {
        detail::UnmountGameCoin();
        s_IsGameCoinInitialized = false;
    }
}

// 0x0013111C (name is ours)
nn::Result Finalize()
{
    FinalizeGameCoin();
    return nn::ptm::CTR::Finalize();
}
} // namespace CTR
} // namespace pl
} // namespace nn
