#include "script/dDialogCompo.h"
#include "script/dDialogCompo_PageAnalyze.h"
#include "script/dDialogCompo_PagePut.h"
#include "script/dDialogCompo_TextRecept.h"
#include "script/dMsgEngineModify.h"
#include "script/dStrMgr.h"
#include "script/dUiMgr.h"
#include "script/dWordRes.h"

namespace script {
// the arguments of the dialog texts (copied and completed)
// 0x008941D8
const DialogCompo::Arg DialogCompo::s_IndexArg = {"SYS_2D_Dialog_Text", 0, -1, NULL, NULL, 5};
// 0x008941F0
const DialogCompo::Arg DialogCompo::s_NumberArg = {"SYS_2D_Dialog_Text", -1, 0, NULL, NULL, 5};

// 0x005D5E44 (name is ours)
void DialogCompo::ComposeByNumber(s32 number, Callback callback, void* callbackArg)
{
    Arg arg = s_NumberArg;
    arg.number = number;
    arg.callback = callback;
    arg.callbackArg = callbackArg;
    Compose(arg);
}

// 0x005D5E80 (name is ours)
void DialogCompo::NextPage()
{
    if (m_Page < m_PageNum - 1) {
        m_Page++;
        PutPage();
    }
}

// 0x005D5EA4 (name is ours)
void DialogCompo::ComposeByIndex(s32 index, Callback callback, void* callbackArg)
{
    Arg arg = s_IndexArg;
    arg.index = index;
    arg.callback = callback;
    arg.callbackArg = callbackArg;
    Compose(arg);
}

// 0x005D5EE4 (name is ours)
DialogCompo::~DialogCompo()
{
    // (only the members are destroyed)
}

// 0x005D9210 (name is ours)
void DialogCompo::PutPage()
{
    const char16* begin = m_PageStarts[m_Page];
    const char16* end = m_PageEnds[m_Page];
    PagePut put;
    IWord* word = &m_PageText;
    put.m_pWrite = MsgEngineModify::s_pNoStop;
    put.m_pLast = MsgEngineModify::s_pNoStop;
    if (word != NULL) {
        put.m_pWord = word;
        put.m_pWrite = word->GetWord();
        put.m_pLast = put.m_pWrite;
        put.m_pLast = put.m_pLast + word->GetSize() - 1;
        put.m_pWord->Clear();
    }
    put.m_IsOk = 1;
    put.SetText(&begin, &end);
    put.SetText2(&MsgEngine::s_pNoStop);
}

// 0x005D950C (name is ours)
void DialogCompo::Compose(const Arg& arg)
{
    for (u32 i = 0; i < PAGE_NUM; i++) {
        m_PageStarts[i] = NULL;
        m_PageEnds[i] = NULL;
    }
    m_PageNum = 0;
    m_Page = 0;
    m_Info.values[0] = 0;
    m_Info.values[1] = 0;
    m_Info.kind = -1;
    m_Info.unknown8 = 0;
    m_Info.flag = -1;
    m_Info.value = 5;
    m_Attribute.Clear();
    static_cast<IWord&>(m_Text).Clear();
    static_cast<IWord&>(m_PageText).Clear();

    TextRecept recept(this);
    StrMgr* strMgr = StrMgr::s_pInstance;
    UiMgr* ui = UiMgr::s_pInstance;
    ui->Begin(&recept);
    if (arg.callback != NULL) {
        arg.callback(ui, arg.callbackArg);
    }
    bool isOk = false;
    if (arg.index != -1) {
        if (ui->ReadWord(&m_Text, arg.name, arg.index, 1)) {
            isOk = true;
        }
        // the attribute of the string
        StrMgr::File* file = strMgr->m_Table.Find(arg.name);
        const void* data;
        if (file != NULL && (data = file->m_pData) != NULL) {
            strMgr->m_Table.m_CS.lock();
            libms::Message message;
            message.Initialize(data);
            m_Attribute.Clear();
            m_Attribute.pData = static_cast<const u8*>(message.GetAttribute(arg.index));
            message.Finalize();
            strMgr->m_Table.m_CS.unlock();
        }
    } else if (arg.number != -1) {
        UiSystem* system = &ui->m_System;
        WordRes res;
        bool isFound = StrMgr::s_pInstance->FindNumbered(&res, arg.name, arg.number);
        if (isFound) {
            system->m_Text.Copy(res);
        } else {
            system->m_Text.Clear();
        }
        if (isFound && system->m_pSeq->Run(&m_Text, &system->m_Text, 1)) {
            isOk = true;
        }
        strMgr->GetAttribute(&m_Attribute, arg.name, arg.number);
    }
    ui->End();
    if (!isOk) {
        return;
    }

    PageAnalyze analyze(arg.lines, &m_Info, m_PageStarts, m_PageEnds);
    m_Info.values[0] = 0;
    m_Info.values[1] = 0;
    m_Info.kind = -1;
    m_Info.unknown8 = 0;
    m_Info.flag = -1;
    m_Info.value = 5;
    const char16* text = static_cast<const IWord&>(m_Text).GetWord();
    m_PageStarts[0] = text;
    m_PageEnds[0] = text;
    analyze.m_PageNum++;
    analyze.SetText(&text, &MsgEngine::s_pNoStop);
    analyze.SetText2(&MsgEngine::s_pNoStop);
    m_PageNum = analyze.m_PageNum;
    PutPage();
}
} // namespace script
