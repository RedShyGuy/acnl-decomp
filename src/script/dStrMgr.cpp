#include "script/dStrMgr.h"
#include "Other/dHeapInit.h"
#include "Other/dProcMgr.h"
#include "Other/dRegion.h"
#include "libms/libms.h"
#include "oml/framework/dProcess.h"
#include "script/dCmpEngine.h"
#include "script/dCountReturn.h"
#include "script/dMgr.h"
#include "script/dStrMgr_SingletonDisposer_.h"
#include "script/dStrNicknameEngine.h"
#include "script/dStrRemakeColor.h"
#include "script/dTalkSystem.h"
#include "script/dWordRes.h"
#include "sead/seadExpHeap.h"
#include "sead/seadNew.h"
#include "sead/seadStringFormatter.h"
#include "stage/stage_Api.h"
#include <cstring>

namespace script {
// 0x0095EEDC
StrMgr* StrMgr::s_pInstance;
// 0x0095EEE0 (name is ours)
StrMgr::SingletonDisposer_* StrMgr::s_pSingletonDisposer;

namespace {
// the label of GetLabel (built by a static initializer of several files)
// 0x00AD69D4
sead::FixedSafeString<256> s_StrLabelBuffer;
} // namespace

// (inline)
inline void StrMgr::File::Set(const sead::SafeStringBase<char>& name, sead::Heap* heap)
{
    GetRegion();  // (the result is not used)
    m_Name.copy(name, -1);
    sead::StringFormatter formatter(&m_Path);
    formatter.append("Script/Str/%s.umsbt").appendValue(name).flush();
    m_pHeap = heap;
}

// (inline)
inline bool StrMgr::File::Load(sead::Heap* tempHeap)
{
    if (m_pData != NULL) {
        return true;
    }
    bool isLoaded = m_Loader.Load(m_Path, tempHeap, 0);
    if (!isLoaded) {
        return isLoaded;
    }
    // the message binary of the language (the file starts with {offset, size} per language)
    const u8* data = static_cast<const u8*>(m_Loader.GetData());
    const void* message = NULL;
    if (data != NULL) {
        s32 language = GetRegion();
        language = static_cast<u32>(language) < 10 ? TalkFile::s_RegionLanguages[language] : 0;
        message = data + reinterpret_cast<const u32*>(data)[language * 2];
    }
    data = static_cast<const u8*>(m_Loader.GetData());
    u32 size = 0;
    if (data != NULL) {
        s32 language = GetRegion();
        language = static_cast<u32>(language) < 10 ? TalkFile::s_RegionLanguages[language] : 0;
        size = reinterpret_cast<const u32*>(data)[language * 2 + 1];
    }
    m_pData = m_pHeap->tryAlloc(size, 16);
    if (m_pData != NULL) {
        std::memcpy(m_pData, message, size);
        m_Size = size;
    }
    m_Loader.Unload();
    return isLoaded;
}

// 0x005DFFBC (name is ours)
void StrMgr::MainGroup::AddFiles()
{
    sead::Heap* heap = Mgr::s_pDataHeap;
    for (const ListInfo* info = s_Lists; info->name != NULL; info++) {
        if (info->mask != 0) {
            continue;
        }
        File* file = new (heap, 4) File();
        file->Set(sead::SafeStringBase<char>(info->name), heap);
        m_Files.PushBack(file);
    }
}

// 0x005DFE70 (name is ours)
bool StrMgr::MainGroup::LoadFiles()
{
    bool isLoaded = true;
    for (FileList::iterator it = m_Files.begin(); it != m_Files.end(); ++it) {
        isLoaded &= FileList::GetPointerFromNode(it.GetNode())->Load(m_pTempHeap);
    }
    return isLoaded;
}

// 0x005D8370 (name is ours)
void StrMgr::SceneGroup::AddFiles()
{
    sead::Heap* heap = HeapInit::s_pStageHeap;
    for (const ListInfo* info = s_Lists; info->name != NULL; info++) {
        if ((info->mask & m_Mask) == 0) {
            continue;
        }
        File* file = new (heap, 4) File();
        file->Set(sead::SafeStringBase<char>(info->name), heap);
        m_Files.PushBack(file);
    }
}

// 0x005D8224 (name is ours)
bool StrMgr::SceneGroup::LoadFiles()
{
    bool isLoaded = true;
    for (FileList::iterator it = m_Files.begin(); it != m_Files.end(); ++it) {
        isLoaded &= FileList::GetPointerFromNode(it.GetNode())->Load(m_pTempHeap);
    }
    return isLoaded;
}

// 0x005D8008 (name is ours)
bool StrMgr::SceneGroup::Load(const oml::framework::Process* scene)
{
    switch (m_State) {
    case 0: {
        m_Mask = 0;
        ProcName index = static_cast<ProcName>(scene->m_Index);
        if (index == PROC_SC_STAGE) {
            stage::Name stage = stage::GetCurrent();
            if (stage::IsNooklingStore(stage)) {
                m_Mask |= 1;
            }
            if (stage == stage::NAME_TOUR_DESK) {
                m_Mask |= 0x100;
            } else if (stage == stage::NAME_CLUB444_LIVE) {
                switch (GetRegionIndex()) {
                case 0:
                    m_Mask |= 0x10;
                    break;
                case 1:
                    m_Mask |= 0x20;
                    break;
                case 2:
                    m_Mask |= 0x40;
                    break;
                case 3:
                    m_Mask |= 0x80;
                    break;
                }
            } else if (stage == stage::NAME_CAMPING_CAR_A || stage == stage::NAME_CAMPING_CAR_B) {
                m_Mask |= 0x200;
            }
        } else if (index == PROC_SC_STAGE_MINI_GAME_0) {
            m_Mask |= 6;
        } else if (index == PROC_SC_STAGE_MINI_GAME_1) {
            m_Mask |= 10;
        }
        if (m_Mask == 0) {
            m_State = 5;
            return true;
        }
        m_State = 1;
    }
    // fall through
    case 1:
        AddFiles();
        m_State = 2;
        // fall through
    case 2:
        m_pTempHeap = sead::ExpHeap::create((m_Mask & 1) ? 0x177000 : 0x32000, sead::SafeStringBase<char>("StrTmpHeap"),
                                            HeapInit::s_pStageHeap, sead::Heap::HEAP_DIRECTION_REVERSE, true);
        m_State = 3;
        // fall through
    case 3:
        if (LoadFiles()) {
            m_State = 4;
        }
        break;
    }
    switch (m_State) {
    case 4:
        if (m_pTempHeap != NULL) {
            m_pTempHeap->destroy();
            m_pTempHeap = NULL;
        }
        m_State = 5;
        return true;
    case 5:
        return true;
    default:
        return false;
    }
}

// 0x0075BDE8 (name is ours)
StrMgr::File* StrMgr::Table::Find(const char* name)
{
    File* found = NULL;
    for (FileList::iterator it = m_Main.m_Files.begin(); it != m_Main.m_Files.end(); ++it) {
        File* file = FileList::GetPointerFromNode(it.GetNode());
        if (file->m_Name.isEqual(sead::SafeStringBase<char>(name))) {
            found = file;
            break;
        }
    }
    if (found != NULL) {
        return found;
    }
    if (m_Scene.m_Files.size() != 0) {
        for (FileList::iterator it = m_Scene.m_Files.begin(); it != m_Scene.m_Files.end(); ++it) {
            File* file = FileList::GetPointerFromNode(it.GetNode());
            if (file->m_Name.isEqual(sead::SafeStringBase<char>(name))) {
                found = file;
                break;
            }
        }
    }
    return found;
}

// 0x005F3DC8 (name after sead)
StrMgr* StrMgr::createInstance(sead::Heap* heap)
{
    if (s_pInstance == NULL) {
        u8* buffer = new (heap, 4) u8[sizeof(StrMgr)];
        StrMgr* instance = NULL;
        if (buffer == NULL) {
            s_pSingletonDisposer = NULL;
        } else {
            s_pSingletonDisposer = new (buffer) SingletonDisposer_(heap);
            instance = new (buffer) StrMgr();
        }
        s_pInstance = instance;
    }
    return s_pInstance;
}

// 0x005F3F18 (name is ours)
bool StrMgr::Load()
{
    if (m_IsLoaded) {
        return m_IsLoaded;
    }
    MainGroup* main = &m_Table.m_Main;
    switch (main->m_State) {
    case 0:
        main->m_State = 1;
        // fall through
    case 1:
        main->AddFiles();
        main->m_State = 2;
        // fall through
    case 2:
        main->m_pTempHeap = sead::ExpHeap::create(0x4F7000, sead::SafeStringBase<char>("StrTmpHeapMain"),
                                                  HeapInit::s_pStageHeap, sead::Heap::HEAP_DIRECTION_REVERSE, true);
        main->m_State = 3;
        // fall through
    case 3:
        if (main->LoadFiles()) {
            main->m_State = 4;
        }
        break;
    }
    bool isLoaded;
    switch (main->m_State) {
    case 4:
        if (main->m_pTempHeap != NULL) {
            main->m_pTempHeap->destroy();
            main->m_pTempHeap = NULL;
        }
        main->m_State = 5;
        isLoaded = true;
        break;
    case 5:
        isLoaded = true;
        break;
    default:
        isLoaded = false;
        break;
    }
    if (isLoaded) {
        m_IsLoaded = 1;
    }
    return m_IsLoaded;
}

// 0x005D8000 (name is ours)
bool StrMgr::LoadScene(const oml::framework::Process* scene)
{
    // (a nop in the original that falls into SceneGroup::Load)
    return m_Table.m_Scene.Load(scene);
}

// 0x00312610 (name is ours)
bool StrMgr::GetString(IWord* word, const char* name, u32 index)
{
    WordRes res;
    bool isFound = Find(&res, name, index);
    if (isFound) {
        word->Copy(res);
    } else {
        word->Clear();
    }
    return isFound;
}

// 0x0075CE3C (name is ours)
bool StrMgr::Find(WordRes* word, const char* name, s32 index)
{
    bool isFound = false;
    if (index >= 0) {
        File* file = m_Table.Find(name);
        const void* data;
        if (file != NULL && (data = file->m_pData) != NULL) {
            m_Table.m_CS.lock();
            libms::Message message;
            message.Initialize(data);
            const void* text = message.GetText(index);
            message.Finalize();
            m_Table.m_CS.unlock();
            if (text != NULL) {
                word->m_pWord = static_cast<const char16*>(text);
                word->m_Size = WordRes::SIZE_UNLIMITED;
                return true;
            }
        }
    }
    word->m_pWord = &WORD_NULL_CHAR;
    word->m_Size = WordRes::SIZE_UNLIMITED;
    return isFound;
}

// 0x0075D364 (name is ours)
bool StrMgr::FindByLabel(WordRes* word, const char* name, const char* label)
{
    const void* text = NULL;
    File* file = m_Table.Find(name);
    const void* data;
    if (file != NULL && (data = file->m_pData) != NULL) {
        m_Table.m_CS.lock();
        libms::Message message;
        message.Initialize(data);
        text = message.GetTextByLabel(label);
        message.Finalize();
        m_Table.m_CS.unlock();
    }
    if (text != NULL) {
        word->m_pWord = static_cast<const char16*>(text);
        word->m_Size = WordRes::SIZE_UNLIMITED;
        return true;
    }
    word->m_pWord = &WORD_NULL_CHAR;
    word->m_Size = WordRes::SIZE_UNLIMITED;
    return false;
}

// 0x0075D41C (name is ours)
bool StrMgr::FindNumbered(WordRes* word, const char* name, u32 number)
{
    if (number < 1000) {
        sead::FixedSafeString<8> label;
        sead::StringFormatter formatter(&label);
        formatter.append("%03d").appendValue(static_cast<s32>(number)).flush();
        if (FindByLabel(word, name, label.getCString())) {
            return true;
        }
    }
    word->m_pWord = &WORD_NULL_CHAR;
    word->m_Size = WordRes::SIZE_UNLIMITED;
    return false;
}

// 0x0075D108 (name is ours)
bool StrMgr::GetStringNumbered(IWord* word, const char* name, u32 number)
{
    WordRes res;
    bool isFound = FindNumbered(&res, name, number);
    if (isFound) {
        word->Copy(res);
    } else {
        word->Clear();
    }
    return isFound;
}

// 0x0075D174 (name is ours)
bool StrMgr::FindNumberedIndex(s32* index, const char* name, s32 number)
{
    if (number <= 0) {
        return false;
    }
    sead::FixedSafeString<8> label;
    sead::StringFormatter formatter(&label);
    formatter.append("%03d").appendValue(number).flush();
    const char* text = label.getCString();
    bool isFound = false;
    File* file = m_Table.Find(name);
    const void* data;
    if (file != NULL && (data = file->m_pData) != NULL) {
        m_Table.m_CS.lock();
        libms::Message message;
        message.Initialize(data);
        s32 found = message.GetTextIndex(text);
        if (found >= 0) {
            *index = found;
            isFound = true;
        }
        message.Finalize();
        m_Table.m_CS.unlock();
    }
    return isFound;
}

// 0x0075CFDC (name is ours)
const void* StrMgr::GetText(const char* name, s32 index)
{
    const void* text = NULL;
    File* file = m_Table.Find(name);
    const void* data;
    if (file != NULL && (data = file->m_pData) != NULL) {
        m_Table.m_CS.lock();
        libms::Message message;
        message.Initialize(data);
        text = message.GetText(index);
        message.Finalize();
        m_Table.m_CS.unlock();
    }
    return text;
}

// 0x0075D514 (name is ours)
const void* StrMgr::GetTextByLabel(const char* name, const char* label)
{
    const void* text = NULL;
    File* file = m_Table.Find(name);
    const void* data;
    if (file != NULL && (data = file->m_pData) != NULL) {
        m_Table.m_CS.lock();
        libms::Message message;
        message.Initialize(data);
        text = message.GetTextByLabel(label);
        message.Finalize();
        m_Table.m_CS.unlock();
    }
    return text;
}

// 0x0075CDC8 (name is ours)
u32 StrMgr::GetCount(const char* name)
{
    u32 count = 0;
    File* file = m_Table.Find(name);
    const void* data;
    if (file != NULL && (data = file->m_pData) != NULL) {
        m_Table.m_CS.lock();
        libms::Message message;
        message.Initialize(data);
        count = message.GetTextNum();
        message.Finalize();
        m_Table.m_CS.unlock();
    }
    return count;
}

// 0x0075D590 (name is ours)
bool StrMgr::GetLabel(sead::BufferedSafeStringBase<char>* label, const char* name, s32 index)
{
    bool isFound = false;
    File* file = m_Table.Find(name);
    const void* data;
    if (file != NULL && (data = file->m_pData) != NULL) {
        m_Table.m_CS.lock();
        libms::Message message;
        message.Initialize(data);
        if (message.GetLabel(index, s_StrLabelBuffer.getBuffer())) {
            label->copy(s_StrLabelBuffer, -1);
            isFound = true;
        }
        message.Finalize();
        m_Table.m_CS.unlock();
    }
    return isFound;
}

// 0x0075D2B0 (name is ours)
s32 StrMgr::CountLines(const char* name, u32 number)
{
    WordRes res;
    if (!FindNumbered(&res, name, number)) {
        return 0;
    }
    CountReturn engine;
    const char16* text = static_cast<const IWord&>(res).GetWord();
    if (text != NULL) {
        engine.Run(text);
    }
    return engine.m_Count + 1;
}

// 0x0075BEB0 (name is ours)
bool StrMgr::GetAttribute(TalkAttribute* attribute, const char* name, s32 number)
{
    bool isFound = false;
    File* file = m_Table.Find(name);
    const void* data;
    if (file != NULL && (data = file->m_pData) != NULL) {
        m_Table.m_CS.lock();
        libms::Message message;
        message.Initialize(data);
        attribute->Clear();
        sead::FixedSafeString<256> label;
        FormatLabelNumber(&label, number);
        attribute->pData = static_cast<const u8*>(message.GetAttribute(message.GetTextIndex(label.getCString())));
        message.Finalize();
        isFound = true;
        m_Table.m_CS.unlock();
    }
    return isFound;
}

// 0x0075CEF8 (name is ours)
bool StrMgr::MakeNickname(IWord* nickname, s32 index, const IWord* name)
{
    WordRes res;
    bool isMade = Find(&res, "STR_Nickname", index);
    if (isMade) {
        StrNicknameEngine engine(name);
        if (!engine.Make(nickname, &res)) {
            isMade = false;
        }
    }
    if (isMade) {
        CmpEngine engine;
        if (engine.Compare(nickname, name)) {
            isMade = false;
        }
    }
    return isMade;
}

// 0x0075D058 (name is ours)
bool StrMgr::GetLine(IWord* word, const char* name, u32 number, s32 line)
{
    WordRes res;
    bool isFound = FindNumbered(&res, name, number);
    if (isFound) {
        StrRemakeColor engine;
        isFound = engine.CopyLine(word, &res, line);
        if (isFound) {
            return isFound;
        }
    }
    word->Clear();
    return isFound;
}
} // namespace script
