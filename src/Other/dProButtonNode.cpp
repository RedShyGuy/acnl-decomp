#include "Other/dProButtonNode.h"

#include "Bs/dBsMenuEdit.h"
#include "Bs/dBsMyDesignEditModel.h"
#include "Item/dItemSelectWindow.h"
#include "Other/dHeapInit.h"
#include "Other/dPaneVisible.h"
#include "Other/dTextureImage.h"
#include "Sound/dSoundLabel.h"
#include "Sv/dSvMyDesign.h"
#include "nw/gfx/res/gfx_ResTexture.h"
#include "nw/lyt/lyt_Bounding.h"
#include "nw/lyt/lyt_Pane.h"
#include "nw/lyt/lyt_TexMap.h"
#include "sead/seadExpHeap.h"
#include "sead/seadFixedSafeString.h"
#include "ssys/ma/lyt/dArcResAccReader.h"

namespace {
// the command cache size of the layout
const u32 CMD_CACHE_SIZE = 0x10000;
// the layer of the layout
const u8 LAYOUT_LAYER = 156;
// the letter of the layout names (edit_pro_M_*)
const char LAYOUT_LETTER = 'M';
// the frames of the cursor before repeating
const s32 CURSOR_TIMER = 9;
// the pattern types of the shirts (below: one-piece dresses)
const s32 PATTERN_TYPE_SHIRT = 3;
// the sleeve kind without sleeves
const s32 SLEEVE_NONE = 2;
} // namespace

// the frame of edit_pro_M_kind of each pattern type
// 0x0084AA14
extern const f32 s_ProKindFrames[ProButtonNode::PATTERN_TYPE_NUM] = {4.0f, 3.0f, 5.0f, 1.0f, 0.0f, 2.0f};
// the texture sizes of the front and the back (one-piece, shirt), of the sleeves (long, short)
// 0x0084AA2C
extern const u32 s_ProBodyTextureSizes[2][2] = {{32, 64}, {32, 32}};
// 0x0084AA3C
extern const u32 s_ProSleeveTextureSizes[2][2] = {{16, 32}, {16, 16}};
// the frame of edit_pro_M_crsl_kind of each part per pattern type
// 0x0084A700
extern const f32 s_ProCursorFrames[ProButtonNode::PATTERN_TYPE_NUM][ProButtonNode::PART_NUM] = {
    {0.0f, 0.0f, 2.0f, 2.0f}, {0.0f, 0.0f, 3.0f, 3.0f}, {0.0f, 0.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 2.0f, 2.0f}, {1.0f, 1.0f, 3.0f, 3.0f}, {1.0f, 1.0f, 0.0f, 0.0f},
};
// the layout kind of the sleeve icons (long, short)
// 0x00849F58
extern const u8 s_ProSleeveIconKinds[2] = {MyDesignProDragIconNode::KIND_16X32, MyDesignProDragIconNode::KIND_16X16};

// the states with their names (Japanese, debug); the machine starts idle
// 0x0084A968
const ProButtonNode::StateInfo ProButtonNode::s_StateInfos[5] = {
    {"アイドル", &ProButtonNode::Enter_Idle, &ProButtonNode::State_Idle},
    {"入場処理", &ProButtonNode::Enter_In, &ProButtonNode::State_In},
    {"退場処理", &ProButtonNode::Enter_Out, &ProButtonNode::State_Out},
    {"実行処理", &ProButtonNode::Enter_Exec, &ProButtonNode::State_Exec},
    {"選択肢処理", &ProButtonNode::Enter_Choice, &ProButtonNode::State_Choice},
};

// 0x009509B0
const char* ProButtonNode::s_IconNames[PART_NUM] = {"P_icon_F", "P_icon_B", "P_icon_L", "P_icon_R"};
// 0x009509C0
const char* ProButtonNode::s_BodyButtonNames[2][2] = {{"B_onep_F", "B_onep_B"}, {"B_body_F", "B_body_B"}};
// 0x009509D0
const char* ProButtonNode::s_SleeveButtonNames[2][2] = {{"B_long_L", "B_long_R"}, {"B_short_L", "B_short_R"}};
// 0x00950950
const char* ProButtonNode::s_CursorPaneNames[PATTERN_TYPE_NUM][PART_NUM] = {
    {"P_Base_06", "P_Base_07", "P_Base_03", "P_Base_02"}, {"P_Base_06", "P_Base_07", "P_Base_05", "P_Base_04"},
    {"P_Base_06", "P_Base_07", NULL, NULL},               {"P_Base_00", "P_Base_01", "P_Base_03", "P_Base_02"},
    {"P_Base_00", "P_Base_01", "P_Base_05", "P_Base_04"}, {"P_Base_00", "P_Base_01", NULL, NULL},
};

// 0x0023E9CC
void ProButtonNode::SetupParts(ssys::ma::lyt::ArcResAccReader* reader)
{
    m_pBackPane = m_ProLayout.FindPane("P_back_00");
    SetFlagVisible(m_pBackPane, false);
    m_pEditingGroup = m_ProLayout.FindGroup("G_editing");
    m_pCursorPane = m_ProLayout.FindPane("P_editing_crsl");
    m_pCursorPane->m_Alpha = 255;
    SetFlagVisible(m_pCursorPane, true);
    m_pAllPane = m_ProLayout.FindPane("N_All");
    m_pSetPane = m_ProLayout.FindPane("N_set");
    m_pPosPane = m_ProLayout.FindPane("N_pos");
    for (s32 i = 0; i < PART_NUM; i++) {
        m_pIcons[i] = m_ProLayout.FindPane(s_IconNames[i]);
        SetFlagVisible(m_pIcons[i], m_EditedPart == i);
    }
    m_SleeveKind = m_PatternType % 3;
    m_ProLayout.BindAnimation(&m_KindAnim);
    m_KindAnim.SetFrame(s_ProKindFrames[m_PatternType]);
    m_ProLayout.Animate();
    m_ProLayout.UnbindAnimation(&m_KindAnim);
    SetFlagVisible(m_ProLayout.FindPane("N_long_R"), m_SleeveKind == 0);
    SetFlagVisible(m_ProLayout.FindPane("N_long_L"), m_SleeveKind == 0);
    SetFlagVisible(m_ProLayout.FindPane("N_short_R"), m_SleeveKind == 1);
    SetFlagVisible(m_ProLayout.FindPane("N_short_L"), m_SleeveKind == 1);

    nw::gfx::res::ResImageTexture::DynamicDescription description;
    description.height = 0;
    description.width = 0;
    description.mipmapLevels = 1;
    description.locationFlag = 0x2020000;
    description.pLocationAddress = NULL;
    description.format = 4;
    description.isCleared = false;
    description.isAllocated = true;
    m_PartNum = m_SleeveKind == SLEEVE_NONE ? 2 : PART_NUM;
    m_Allocator.CreateExpHeap(51200, HeapInit::s_pStageHeap, sead::SafeStringBase<char>("ProButtonNode"),
                              sead::Heap::HEAP_DIRECTION_FORWARD, false);
    // (not used)
    nw::lyt::TextureInfo info;
    info.physicalAddress = 0;
    info.size = nw::lyt::TexSize(8, 8);
    info.realSize = nw::lyt::TexSize(8, 8);
    info.format = nw::lyt::TEXFORMAT_RGBA4;
    nw::lyt::TexMap texMap(info);

    // the front and the back
    const bool isShirt = m_PatternType >= PATTERN_TYPE_SHIRT;
    const char* const* bodyNames = s_BodyButtonNames[isShirt];
    const u32* bodySize = s_ProBodyTextureSizes[isShirt];
    for (s32 i = 0; i < 2; i++) {
        MyDesignProDragIconNode* part = &m_Parts[i];
        if (m_PatternType >= PATTERN_TYPE_SHIRT) {
            part->Setup(i, MyDesignProDragIconNode::KIND_ICON, reader);
        } else {
            part->Setup(i, MyDesignProDragIconNode::KIND_32X48, reader);
        }
        description.width = bodySize[0];
        description.height = bodySize[1];
        part->m_pTexture = nw::gfx::res::ResImageTexture::CreateDynamic(description, &m_Allocator).ptr();
        part->UpdatePicture();
        part->SetSeOk(SE_SYS_DECIDE_S);
        part->SetOkSoundEnabled(true);
        m_PartGroup.AddNode(part, m_ProLayout.FindPane(bodyNames[i]));
    }
    // the sleeves
    nw::lyt::Pane* sleeveBase = m_ProLayout.FindPane("N_B");
    const u32* sleeveSize = s_ProSleeveTextureSizes[m_SleeveKind];
    for (s32 i = 2; i < static_cast<s32>(m_PartNum); i++) {
        MyDesignProDragIconNode* part = &m_Parts[i];
        part->Setup(i, s_ProSleeveIconKinds[m_SleeveKind], reader);
        description.width = sleeveSize[0];
        description.height = sleeveSize[1];
        part->m_pTexture = nw::gfx::res::ResImageTexture::CreateDynamic(description, &m_Allocator).ptr();
        part->UpdatePicture();
        part->SetSeOk(SE_SYS_DECIDE_S);
        part->SetOkSoundEnabled(true);
        nw::lyt::Pane* button = m_ProLayout.FindPane(s_SleeveButtonNames[m_SleeveKind][i - 2]);
        button->m_Translate.x -= sleeveBase->m_Translate.x;
        button->m_Flag &= ~nw::lyt::Pane::FLAG_MTX_CONDITION_MASK;
        button->m_pParent->RemoveChild(button);
        sleeveBase->AppendChild(button);
        m_PartGroup.AddNode(part, button);
    }
    m_pCursorGroup = m_ProLayout.FindGroup("G_crsl");
    m_TargetPart = -1;
}

// 0x0023EF64
bool ProButtonNode::Setup(const SetupArg& arg)
{
    if (!m_Choice.SetupTextBoxes(0)) {
        return false;
    }
    m_ProLayout.m_pArcReader = arg.reader;
    m_ProLayout.Build("edit_pro_M.bclyt", NULL, CMD_CACHE_SIZE);
    m_ProLayout.m_Layer = LAYOUT_LAYER;
    ButtonActionNode::Info info = {NULL, NULL, nn::math::VEC2(0.0f, 0.0f), 0, BUTTON_INDEX_SELF, NULL};
    info.pane = m_ProLayout.FindBounding("B_btn");
    info.group = m_ProLayout.FindGroup("G_btn");
    info.pos = nn::math::VEC2(0.0f, 0.0f);
    Initialize(info);
    m_PatternType = BsMenuEdit::s_EditDesign.patternType;
    sead::FixedSafeString<64> name;
    name.format("edit_pro_%c_in.bclan", LAYOUT_LETTER);
    m_InAnim.Initialize(name.getCString(), arg.reader);
    name.format("edit_pro_%c_out.bclan", LAYOUT_LETTER);
    m_OutAnim.Initialize(name.getCString(), arg.reader);
    name.format("edit_pro_%c_kind.bclan", LAYOUT_LETTER);
    m_KindAnim.Initialize(name.getCString(), arg.reader);
    name.format("edit_pro_%c_crsl_kind.bclan", LAYOUT_LETTER);
    m_CursorKindAnim.Initialize(name.getCString(), arg.reader);
    name.format("edit_pro_%c_touch.bclan", LAYOUT_LETTER);
    m_Touch.Initialize(name.getCString(), arg.reader);
    name.format("edit_pro_%c_select.bclan", LAYOUT_LETTER);
    m_Select.Initialize(name.getCString(), arg.reader);
    name.format("edit_pro_%c_touch_ok.bclan", LAYOUT_LETTER);
    m_TouchOk.Initialize(name.getCString(), arg.reader);
    name.format("edit_pro_%c_select_ok.bclan", LAYOUT_LETTER);
    m_SelectOk.Initialize(name.getCString(), arg.reader);
    // the parts start hidden
    m_ProLayout.BindAnimation(&m_OutAnim);
    m_OutAnim.SetLastFrame();
    m_ProLayout.Animate();
    m_ProLayout.UnbindAnimation(&m_OutAnim);
    m_pBasePane = arg.pBasePane;
    if (!m_IsAttached) {
        arg.pBasePane->AppendChild(m_ProLayout.GetRootPane());
        m_IsAttached = true;
    }
    DragIconGroup::SetupArg groupArg = {NULL,
                                        NULL,
                                        NULL,
                                        NULL,
                                        "edit_pro_M_on_itm_hole.bclan",
                                        NULL,
                                        "m_design_icon_touch.bclan",
                                        "m_design_icon_select.bclan",
                                        "m_design_icon_touch_ok.bclan",
                                        "m_design_icon_select_ok.bclan",
                                        "m_design_icon_catch.bclan"};
    groupArg.markerResAcc = arg.reader;
    groupArg.layout = &m_ProLayout;
    groupArg.marker = m_ProLayout.FindPane("N_crsl");
    groupArg.markerGroup = m_ProLayout.FindGroup("G_crsl");
    groupArg.resAcc = arg.reader;
    m_PartGroup.Setup(groupArg);
    SetupParts(arg.reader);
    m_DragControl.InitializeDrag();
    m_DragControl.AddGroup(&m_PartGroup);
    m_DragControl.m_Flags2 = ButtonActionControl::ENABLE_TOUCH;
    // swap / copy / cancel
    m_ChoiceLabels[0].label = "SYS_2D_UI";
    m_ChoiceLabels[0].number = 52;
    m_ChoiceLabels[0].value = 0;
    m_ChoiceLabels[0].sound = 0;
    m_ChoiceLabels[0].kind = 7;
    m_ChoiceLabels[1].label = "SYS_2D_UI";
    m_ChoiceLabels[1].number = 51;
    m_ChoiceLabels[1].value = 0;
    m_ChoiceLabels[1].sound = 0;
    m_ChoiceLabels[1].kind = 9;
    m_ChoiceLabels[2].label = "SYS_2D_UI";
    m_ChoiceLabels[2].number = 11;
    m_ChoiceLabels[2].value = 0;
    m_ChoiceLabels[2].sound = 0;
    m_ChoiceLabels[2].kind = 1;
    m_Choice.SetItems(m_ChoiceLabels, 3, CHOICE_CANCEL);
    Start("ProButtonNode", s_StateInfos, 5);
    SetSeOk(SE_SYS_EDIT_PARTS_WIN_OPEN);
    SetOkSoundEnabled(true);
    SetSe(SE_SYS_EDIT_BTN_ACTIVE);
    SetSoundEnabled(true);
    m_SoundMode = true;
    return true;
}

// 0x0023F52C
void ProButtonNode::State_In()
{
    if (!m_InAnim.IsDone()) {
        m_InAnim.IncreaseFrame();
        m_ProLayout.m_IsAnimationChanged = true;
        return;
    }
    m_ProLayout.UnbindAnimation(&m_InAnim);
    m_IsAnimating = false;
    SetFlagVisible(m_pBackPane, true);
    m_DragControl.SetFlags(ButtonActionControl::DISABLED, false);
    SetFlagVisible(m_pIcons[m_EditedPart], false);
    // the position pane moves into the set pane
    m_pPosPane->m_pParent->RemoveChild(m_pPosPane);
    const nn::math::VEC3& set = m_pSetPane->m_Translate;
    const nn::math::VEC3& pos = m_pPosPane->m_Translate;
    m_pPosPane->SetTranslate(nn::math::VEC3(pos.x - set.x, pos.y - set.y, pos.z - set.z));
    nw::lyt::Pane::PaneList::iterator it = m_pSetPane->m_ChildList.begin();
    ++it;
    nw::lyt::Pane* next = it == m_pSetPane->m_ChildList.end() ? NULL : nw::lyt::Pane::FromLink(it.GetNode());
    m_pSetPane->InsertChild(next, m_pPosPane);
    BsMyDesignEditModel::s_pInstance->Func2F507C();
    BsMyDesignEditModel::s_pInstance->Func2F50C8(m_EditedPart);
    Change(&ProButtonNode::State_Exec);
}

// 0x0023F688
void ProButtonNode::Enter_In()
{
    m_ProLayout.BindAnimation(&m_InAnim);
    m_InAnim.SetFrame(0.0f);
    m_ProLayout.m_IsAnimationChanged = true;
    m_DragControl.Reset();
    m_IsAnimating = true;
    m_IsChosen = false;
    m_IsShown = true;
    for (u32 i = 0; i < m_PartNum; i++) {
        BsMenuEdit::s_EditDesign.WriteToTexture(m_Parts[i].m_pTexture, i);
    }
    m_PartGroup.HideMarker();
    m_pCursorPane->m_pParent->RemoveChild(m_pCursorPane);
    m_ProLayout.FindPane(s_CursorPaneNames[m_PatternType][m_EditedPart])->PrependChild(m_pCursorPane);
    m_ProLayout.BindAnimation(&m_CursorKindAnim, m_pEditingGroup, false);
    m_CursorKindAnim.SetFrame(s_ProCursorFrames[m_PatternType][m_EditedPart]);
    m_ProLayout.Animate();
    m_ProLayout.UnbindAnimation(&m_CursorKindAnim, m_pEditingGroup, false);
    SetSeOk(SE_SYS_EDIT_PARTS_WIN_CLOSE);
    SetOkSoundEnabled(true);
}

// 0x0023F7DC
void ProButtonNode::State_Out()
{
    if (!m_OutAnim.IsDone()) {
        m_OutAnim.IncreaseFrame();
        m_ProLayout.m_IsAnimationChanged = true;
        return;
    }
    m_ProLayout.UnbindAnimation(&m_OutAnim);
    m_IsAnimating = false;
    m_IsShown = false;
    Change(&ProButtonNode::State_Idle);
}

// 0x0023F83C
void ProButtonNode::Enter_Out()
{
    SetFlagVisible(m_pBackPane, false);
    SetFlagVisible(m_pIcons[m_EditedPart], true);
    m_ProLayout.BindAnimation(&m_OutAnim);
    m_OutAnim.SetFrame(0.0f);
    m_DragControl.SetFlags(ButtonActionControl::DISABLED, true);
    m_IsAnimating = true;
    // the position pane goes back to the root
    m_pPosPane->m_pParent->RemoveChild(m_pPosPane);
    const nn::math::VEC3& set = m_pSetPane->m_Translate;
    const nn::math::VEC3& pos = m_pPosPane->m_Translate;
    m_pPosPane->SetTranslate(nn::math::VEC3(pos.x + set.x, pos.y + set.y, pos.z + set.z));
    m_pAllPane->AppendChild(m_pPosPane);
    m_DragControl.Reset();
    BsMyDesignEditModel::s_pInstance->Func2F4E90();
    SetSeOk(SE_SYS_EDIT_PARTS_WIN_OPEN);
    SetOkSoundEnabled(true);
}

// 0x0023F940
void ProButtonNode::State_Idle()
{
    // empty (in the original too)
}

// 0x0023F944
void ProButtonNode::State_Exec()
{
    if (m_IsChosen) {
        return;
    }
    // the part under the dragged one
    const s32 target = m_DragControl.GetHoverIndex();
    if (m_TargetPart != target) {
        if (target == -1) {
            m_TargetPart = -1;
        } else {
            m_TargetPart = target;
            m_ProLayout.BindAnimation(&m_CursorKindAnim, m_pCursorGroup, false);
            m_CursorKindAnim.SetFrame(s_ProCursorFrames[m_PatternType][m_TargetPart]);
            m_ProLayout.Animate();
            m_ProLayout.UnbindAnimation(&m_CursorKindAnim, m_pCursorGroup, false);
        }
    }
    // the edited part follows the selection
    if ((m_DragControl.IsSelectOkDone() || m_DragControl.IsSelectOkOngoing()) && !m_DragControl.m_IsDragging) {
        const s32 selected = m_DragControl.GetSelectedIndex();
        if (m_EditedPart != selected) {
            m_TargetPart = selected;
            m_EditedPart = selected;
            m_pCursorPane->m_pParent->RemoveChild(m_pCursorPane);
            m_ProLayout.FindPane(s_CursorPaneNames[m_PatternType][m_EditedPart])->PrependChild(m_pCursorPane);
            BsMyDesignEditModel::s_pInstance->Func2F50C8(m_EditedPart);
            m_ProLayout.BindAnimation(&m_CursorKindAnim, m_pEditingGroup, false);
            m_CursorKindAnim.SetFrame(s_ProCursorFrames[m_PatternType][m_EditedPart]);
            m_ProLayout.Animate();
            m_ProLayout.UnbindAnimation(&m_CursorKindAnim, m_pEditingGroup, false);
        }
    }
    if (!m_DragControl.IsSelectOkDone()) {
        return;
    }
    if (m_DragControl.IsSelectOkDone() && m_DragControl.m_pDropTarget != NULL) {
        Change(&ProButtonNode::State_Choice);
        return;
    }
    m_EditedPart = m_DragControl.GetPressedIndex();
    m_IsChosen = true;
}

// 0x0023FB58
void ProButtonNode::Enter_Idle()
{
    // empty (in the original too)
}

// 0x0023FB5C
void ProButtonNode::Enter_Exec()
{
    // empty (in the original too)
}

// 0x0023FB60
void ProButtonNode::State_Choice()
{
    m_Choice.Run();
    if (!m_Choice.IsIdle()) {
        return;
    }
    if (m_Choice.GetCursor() == CHOICE_SWAP) {
        const s32 current = m_DragControl.GetCurrentIndex();
        SwapParts(m_DragControl.GetDropTargetIndex(), current);
    } else if (m_Choice.GetCursor() == CHOICE_COPY) {
        const s32 current = m_DragControl.GetCurrentIndex();
        CopyPart(m_DragControl.GetDropTargetIndex(), current);
        m_DragControl.EndDrag(false, SE_SYS_ITEM_ICON_SET);
    }
    m_DragControl.EndDrag(true, SE_SYS_ITEM_ICON_SET);
    Change(&ProButtonNode::State_Exec);
}

// 0x0023FC40
void ProButtonNode::Enter_Choice()
{
    m_Choice.m_Window.m_pAttachPane = m_DragControl.GetCurrentParentPane();
    nn::math::VEC3 position;
    m_DragControl.GetCurrentPosition(&position);
    m_Choice.m_Window.m_AttachPosition = position;
    m_Choice.m_Window.SetNameVisible(false);
    m_Choice.Begin(NULL, NULL, NULL);
}

// 0x0023FCC0
void ProButtonNode::CopyPart(s32 to, s32 from)
{
    const u16 transparent = BsMenuEdit::s_EditDesign.GetPixelValue(0);
    const nw::gfx::res::ResImageTextureData*& toTexture = m_Parts[to].m_pTexture;
    TextureImage16 toImage(toTexture);
    const nw::gfx::res::ResImageTextureData*& fromTexture = m_Parts[from].m_pTexture;
    TextureImage16 fromImage(fromTexture);
    for (u32 x = 0; x < 32; x++) {
        for (u32 y = 0; y < 64; y++) {
            if (x >= toTexture->width || y >= toTexture->height) {
                continue;
            }
            if (x < fromTexture->width && y < fromTexture->height) {
                toImage.SetPixel(x, y, fromImage.GetPixel(x, y));
            } else {
                toImage.SetPixel(x, y, transparent);
            }
        }
    }
    BsMenuEdit::s_EditDesign.ReadFromTexture(toTexture, to);
}

// 0x0023FDDC
void ProButtonNode::SwapParts(s32 a, s32 b)
{
    const u16 transparent = BsMenuEdit::s_EditDesign.GetPixelValue(0);
    const nw::gfx::res::ResImageTextureData*& textureA = m_Parts[a].m_pTexture;
    TextureImage16 imageA(textureA);
    const nw::gfx::res::ResImageTextureData*& textureB = m_Parts[b].m_pTexture;
    TextureImage16 imageB(textureB);
    for (u32 x = 0; x < 32; x++) {
        for (u32 y = 0; y < 64; y++) {
            const bool isInA = x < textureA->width && y < textureA->height;
            const bool isInB = x < textureB->width && y < textureB->height;
            if (isInA && isInB) {
                const u16 valueA = imageA.GetPixel(x, y);
                const u16 valueB = imageB.GetPixel(x, y);
                imageA.SetPixel(x, y, valueB);
                imageB.SetPixel(x, y, valueA);
            } else if (isInA) {
                imageA.SetPixel(x, y, transparent);
            } else if (isInB) {
                imageB.SetPixel(x, y, transparent);
            }
        }
    }
    BsMenuEdit::s_EditDesign.ReadFromTexture(textureA, a);
    BsMenuEdit::s_EditDesign.ReadFromTexture(textureB, b);
}

// 0x0023FF54
void ProButtonNode::Update()
{
    m_DragControl.Update();
    Call();
}

// 0x0023FFEC
ProButtonNode::ProButtonNode()
    : m_pBasePane(NULL), m_pBackPane(NULL), m_pCursorPane(NULL), m_pAllPane(NULL), m_pSetPane(NULL), m_pPosPane(NULL),
      m_pCursorGroup(NULL), m_pEditingGroup(NULL), m_EditedPart(0), m_PatternType(0), m_SleeveKind(0), m_PartNum(0),
      m_TargetPart(-1), m_IsAnimating(false), m_IsChosen(false), m_IsAttached(false), m_Unknown0x100F(0),
      m_IsShown(false)
{
    m_pLayout = &m_ProLayout;
}

// 0x00240118 slot 0x00
// 0x00775E30 (thunk)
// 0x00775E1C (deleting thunk)
// 0x00240108 (deleting dtor)
ProButtonNode::~ProButtonNode()
{
    // (only the members are destroyed)
}
