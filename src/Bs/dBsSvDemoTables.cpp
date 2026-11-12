#include "Bs/dBsSvDemo.h"

// (in a file of its own: GCC would fold the table in Initialize, the original reads it)

// the modes (names from the binary)
// 0x00855798
const BsSvDemo::Info BsSvDemo::s_ModeInfos[21] = {
    {"待機", &BsSvDemo::Enter_Wait, &BsSvDemo::State_Wait},
    {"セーブ不可", &BsSvDemo::Enter_CannotSave1, &BsSvDemo::State_CannotSave1},
    {"セーブ不可", &BsSvDemo::Enter_CannotSave2, &BsSvDemo::State_CannotSave2},
    {"セーブ不可", &BsSvDemo::Enter_CannotSave3, &BsSvDemo::State_CannotSave3},
    {"セーブ不可", &BsSvDemo::Enter_CannotSave4, &BsSvDemo::State_CannotSave4},
    {"セーブ不可", &BsSvDemo::Enter_CannotSave5, &BsSvDemo::State_CannotSave5},
    {"wait", &BsSvDemo::Enter_WaitQuestion, &BsSvDemo::State_WaitQuestion},
    {"wait", &BsSvDemo::Enter_WaitChoice, &BsSvDemo::State_WaitChoice},
    {"out", &BsSvDemo::Enter_Out, &BsSvDemo::State_Out},
    {"save", &BsSvDemo::Enter_Save, &BsSvDemo::State_Save},
    {"継続セーブ", &BsSvDemo::Enter_SaveContinue, &BsSvDemo::State_SaveContinue},
    {"通信時待機", &BsSvDemo::Enter_WaitNet, &BsSvDemo::State_WaitNet},
    {"選択終了", &BsSvDemo::Enter_EndChoice, &BsSvDemo::State_EndChoice},
    {"ロック失敗", &BsSvDemo::Enter_LockFailed, &BsSvDemo::State_LockFailed},
    {"ロック", &BsSvDemo::Enter_Lock, &BsSvDemo::State_Lock},
    {"ロック結果", &BsSvDemo::Enter_LockResult, &BsSvDemo::State_LockResult},
    {"ロック成功", &BsSvDemo::Enter_LockSucceeded, &BsSvDemo::State_LockSucceeded},
    {"ロック", &BsSvDemo::Enter_LockContinue, &BsSvDemo::State_LockContinue},
    {"ロック結果", &BsSvDemo::Enter_LockContinueResult, &BsSvDemo::State_LockContinueResult},
    {"ロック成功", &BsSvDemo::Enter_LockContinueSucceeded, &BsSvDemo::State_LockContinueSucceeded},
    {"なにもしない", &BsSvDemo::Enter_DoNothing, &BsSvDemo::State_DoNothing},
};
