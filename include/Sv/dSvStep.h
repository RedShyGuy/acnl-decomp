#pragma once

#include "decomp.h"
#include "Sv/dSvStepInner.h"
#include "state/dStep.h"

// A step sequence with a request of the save thread: the common steps (load, save, format ...) start the request and go
// to the next step once it is accepted. Class name from the RTTI; the member names are ours (the step names after the
// debug names of the step tables of BsSvMgr, translated).
// Instantiations found in the binary:
//   SvStep<BsSvMgr, BsSvMgr::SaveStep>  typeinfo 0x008CD31C
template <typename T0, typename T1>
class SvStep : public ::state::Step<T0, T1>
{
public:
    SvStep() : m_Request(SvStepInner::FLAG_KEEP_ERROR) {}
    virtual ~SvStep() {}
    // the end of a save step / of a load step (isLast: the last check of a sequence)
    virtual void EndSave() = 0;
    virtual void EndLoad(bool isLast) = 0;

    // "フォーマット" the save data archive is formatted
    void Step_Format()
    {
        m_Request.Reset();
        m_Request.m_Flags = SvStepInner::FLAG_FORMAT;
        if (m_Request.Start()) {
            this->Next();
        }
    }
    // (the step after the current one)
    void Step_Next() { this->Next(); }
    // "終了待ち" wait for the request
    void Step_WaitDone()
    {
        if (m_Request.m_IsDone) {
            this->Next();
        }
    }
    // "ロード完了？" (load done?)
    void Step_EndLoad() { EndLoad(false); }
    // "全体セーブ終了" (end of the whole save) and the other ends of saves
    void Step_EndSave() { EndSave(); }
    // "ロード開始" garden_plus.dat is loaded and checked
    void Step_StartLoad()
    {
        m_Request.Reset();
        m_Request.m_Flags = SvStepInner::FLAG_CHECK | 2;
        if (m_Request.Start()) {
            this->Next();
        }
    }
    // "ネットホスト/ビジターセーブ初期化" (initialization of the save in a network game)
    void Step_InitializeNetSave();
    // "セーブ失敗" (save failed)
    void Step_SaveFailed() {}
    // "ロード完了？" (the last check)
    void Step_EndLoadLast() { EndLoad(true); }
    // "ネットホスト/ビジターセーブ終了"
    void Step_EndNetSave() { EndSave(); }
    // "ネットホスト/ビジターセーブ開始" the prepared request is started
    void Step_StartNetSave()
    {
        if (m_Request.Start()) {
            this->Next();
        }
    }
    // "ベストフレンドデータの破棄" (the best friend data is discarded)
    void Step_DiscardBestFriendData();
    // "村＋その他データセーブ終了" (end of the save of the town and the other data)
    void Step_EndSaveTown() { EndSave(); }
    // "全体セーブ開始" the whole save starts (garden_plus.dat with the friends, without a commit)
    void Step_StartSaveAll();
    // "村＋その他データセーブ開始" the save of the garden data starts (save kind 5)
    void Step_StartSaveTown()
    {
        m_Request.Reset();
        m_Request.m_Flags = 5 << 11;
        if (m_Request.Start()) {
            this->Next();
        }
    }

    SvStepInner m_Request;  // 0x28
};

// (game code, not decompiled yet; names after the addresses) prepare a request for the save in a network game / the
// callback after the load of the whole save / discard the best friend data
void Func1E5DA4(SvStepInner* request); // 0x001E5DA4
bool Func1E5DCC(); // 0x001E5DCC
void Func1E5DDC(SvStepInner* request); // 0x001E5DDC

template <typename T0, typename T1>
void SvStep<T0, T1>::Step_InitializeNetSave()
{
    Func1E5DA4(&m_Request);
    this->Next();
}

template <typename T0, typename T1>
void SvStep<T0, T1>::Step_DiscardBestFriendData()
{
    Func1E5DDC(&m_Request);
    this->Next();
}

template <typename T0, typename T1>
void SvStep<T0, T1>::Step_StartSaveAll()
{
    m_Request.Reset();
    m_Request.m_AfterLoad = Func1E5DCC;
    m_Request.m_BeforeCommit = NULL;
    m_Request.m_OnEnd = NULL;
    m_Request.m_Flags = SvStepInner::FLAG_SAVE_FRIENDS | SvStepInner::FLAG_NO_COMMIT | SvStepInner::FLAG_EXTRA | (1 << 11);
    if (m_Request.Start()) {
        this->Next();
    }
}
