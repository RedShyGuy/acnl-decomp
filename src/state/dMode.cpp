#include "state/dMode.h"

#include "Bs/dBsCommonDialog.h"
#include "Bs/dBsHandCursor.h"
#include "Bs/dBsMenuAtm.h"
#include "Bs/dBsMenuBbsEditer.h"
#include "Bs/dBsMenuBbsMgr.h"
#include "Bs/dBsMenuBestFriendList.h"
#include "Bs/dBsMenuBestFriendListMgr.h"
#include "Bs/dBsMenuBestFriendRegister.h"
#include "Bs/dBsMenuBg.h"
#include "Bs/dBsMenuBingoCard.h"
#include "Bs/dBsMenuBook.h"
#include "Bs/dBsMenuBorrowThingList.h"
#include "Bs/dBsMenuBulletinBoard.h"
#include "Bs/dBsMenuCatalog.h"
#include "Bs/dBsMenuChat.h"
#include "Bs/dBsMenuChest.h"
#include "Bs/dBsMenuCoolbox.h"
#include "Bs/dBsMenuEdit.h"
#include "Bs/dBsMenuExplainPlate.h"
#include "Bs/dBsMenuFriendChatLog.h"
#include "Bs/dBsMenuItem.h"
#include "Bs/dBsMenuItemChange.h"
#include "Bs/dBsMenuItemSelect.h"
#include "Bs/dBsMenuLetter.h"
#include "Bs/dBsMenuLetterEditer.h"
#include "Bs/dBsMenuLetterMgr.h"
#include "Bs/dBsMenuLetterStock.h"
#include "Bs/dBsMenuMailSelect.h"
#include "Bs/dBsMenuMailbox.h"
#include "Bs/dBsMenuMain.h"
#include "Bs/dBsMenuMap.h"
#include "Bs/dBsMenuMapVillage.h"
#include "Bs/dBsMenuMelody.h"
#include "Bs/dBsMenuMgr.h"
#include "Bs/dBsMenuMiiSelect.h"
#include "Bs/dBsMenuModelHomeBoard.h"
#include "Bs/dBsMenuMyDesign.h"
#include "Bs/dBsMenuMyDesignDreamPresent.h"
#include "Bs/dBsMenuMyDesignKeep.h"
#include "Bs/dBsMenuMyDesignSelect.h"
#include "Bs/dBsMenuNumberInput.h"
#include "Bs/dBsMenuPassport.h"
#include "Bs/dBsMenuPassportForFriend.h"
#include "Bs/dBsMenuPassportForGhost.h"
#include "Bs/dBsMenuPassportForPhoto.h"
#include "Bs/dBsMenuRandomHeadsUp.h"
#include "Bs/dBsMenuReaction.h"
#include "Bs/dBsMenuRentalHaniwa.h"
#include "Bs/dBsMenuRoomLightSwitch.h"
#include "Bs/dBsMenuRoomMgr.h"
#include "Bs/dBsMenuSantaBagSelect.h"
#include "Bs/dBsMenuSendMailLand.h"
#include "Bs/dBsMenuSendMailMe.h"
#include "Bs/dBsMenuSetupTime.h"
#include "Bs/dBsMenuSignList.h"
#include "Bs/dBsMenuTab.h"
#include "Bs/dBsMenuTourSelect.h"
#include "Bs/dBsMenuWhatTreeIsThisTree.h"
#include "Bs/dBsSkb.h"
#include "Bs/dBsSvDemo.h"
#include "Bs/dBsSvMgr.h"
#include "Bs/dBsTelop.h"
#include "Bs/dBsTimeBelWindow.h"
#include "Bs/dBsTimeBelWindow_MoneyProc.h"
#include "Camera/dCameraShutter.h"
#include "Change/dChangeListBase.h"
#include "Change/dChangeRentalBase.h"
#include "Change/dChangeStockLetter.h"
#include "Change/dChangeStockMyDesign.h"
#include "Item/dItemDragWindow.h"
#include "Item/dItemSelectNameWindow.h"
#include "Letter/dLetterDragItemWindow.h"
#include "Letter/dLetterDragPostWindow.h"
#include "Letter/dLetterDragStockWindow.h"
#include "Other/dAddressSelectWindow.h"
#include "Other/dAtmWindow.h"
#include "Other/dBlackToBtn.h"
#include "Other/dBlackWait.h"
#include "Other/dBookIconBase.h"
#include "Other/dCaution.h"
#include "Other/dCensusWindow.h"
#include "Other/dComDialogA.h"
#include "Other/dComTitle.h"
#include "Other/dControlButtonBase.h"
#include "Other/dExplainBookContent.h"
#include "Other/dFade.h"
#include "Other/dHomeBtnProhibition.h"
#include "Other/dLifeSupportWindow.h"
#include "Other/dPalette.h"
#include "Other/dPassport.h"
#include "Other/dPostDragWindow.h"
#include "Other/dProButtonNode.h"
#include "Other/dRollText.h"
#include "Other/dRollTextLicence.h"
#include "Other/dSantaBagWindow.h"
#include "Other/dSetDateTimeBg.h"
#include "Other/dTenkeyPad.h"
#include "Other/dToolButtonNode.h"
#include "Other/dWipe.h"
#include "Photo/dPhotoBoxFrame.h"
#include "nfp/dFramework.h"

// The copies of state::Mode<T>::GetName per T in the template area (the classes whose machine is a secondary base or
// that do not override GetName).
// 0x0082A680 slot 0x08
template const char* state::Mode<BlackToBtn>::GetName(Func func) const;
// 0x0082A6F8 slot 0x08
template const char* state::Mode<BsMenuBook>::GetName(Func func) const;
// 0x0082A770 slot 0x08
template const char* state::Mode<BsMenuChat>::GetName(Func func) const;
// 0x0082A7E8 slot 0x08
template const char* state::Mode<BsMenuEdit>::GetName(Func func) const;
// 0x0082A860 slot 0x08
template const char* state::Mode<BsMenuItem>::GetName(Func func) const;
// 0x0082A8D8 slot 0x08
template const char* state::Mode<BsMenuMain>::GetName(Func func) const;
// 0x0082A950 slot 0x08
template const char* state::Mode<ComDialogA>::GetName(Func func) const;
// 0x0082A9C8 slot 0x08
template const char* state::Mode<BsMenuChest>::GetName(Func func) const;
// 0x0082AAB8 slot 0x08
template const char* state::Mode<BookIconBase>::GetName(Func func) const;
// 0x0082AB30 slot 0x08
template const char* state::Mode<BsHandCursor>::GetName(Func func) const;
// 0x0082ABA8 slot 0x08
template const char* state::Mode<BsMenuBbsMgr>::GetName(Func func) const;
// 0x0082AC20 slot 0x08
template const char* state::Mode<BsMenuLetter>::GetName(Func func) const;
// 0x0082AC98 slot 0x08
template const char* state::Mode<BsMenuMelody>::GetName(Func func) const;
// 0x0082AD10 slot 0x08
template const char* state::Mode<CensusWindow>::GetName(Func func) const;
// 0x0082AE00 slot 0x08
template const char* state::Mode<BsMenuCatalog>::GetName(Func func) const;
// 0x0082AE78 slot 0x08
template const char* state::Mode<BsMenuCoolbox>::GetName(Func func) const;
// 0x0082AEF0 slot 0x08
template const char* state::Mode<BsMenuMailbox>::GetName(Func func) const;
// 0x0082AF68 slot 0x08
template const char* state::Mode<BsMenuRoomMgr>::GetName(Func func) const;
// 0x0082AFE0 slot 0x08
template const char* state::Mode<CameraShutter>::GetName(Func func) const;
// 0x0082B058 slot 0x08
template const char* state::Mode<PhotoBoxFrame>::GetName(Func func) const;
// 0x0082B0D0 slot 0x08
template const char* state::Mode<ProButtonNode>::GetName(Func func) const;
// 0x0082B148 slot 0x08
template const char* state::Mode<SetDateTimeBg>::GetName(Func func) const;
// 0x0082B1C0 slot 0x08
template const char* state::Mode<BsCommonDialog>::GetName(Func func) const;
// 0x0082B238 slot 0x08
template const char* state::Mode<BsMenuMyDesign>::GetName(Func func) const;
// 0x0082B2B0 slot 0x08
template const char* state::Mode<BsMenuPassport>::GetName(Func func) const;
// 0x0082B328 slot 0x08
template const char* state::Mode<BsMenuReaction>::GetName(Func func) const;
// 0x0082B3A0 slot 0x08
template const char* state::Mode<BsMenuSignList>::GetName(Func func) const;
// 0x0082B418 slot 0x08
template const char* state::Mode<ChangeListBase>::GetName(Func func) const;
// 0x0082B490 slot 0x08
template const char* state::Mode<ItemDragWindow>::GetName(Func func) const;
// 0x0082B508 slot 0x08
template const char* state::Mode<PostDragWindow>::GetName(Func func) const;
// 0x0082B580 slot 0x08
template const char* state::Mode<SantaBagWindow>::GetName(Func func) const;
// 0x0082B5F8 slot 0x08
template const char* state::Mode<ToolButtonNode>::GetName(Func func) const;
// 0x0082B670 slot 0x08
template const char* state::Mode<BsMenuBbsEditer>::GetName(Func func) const;
// 0x0082B6E8 slot 0x08
template const char* state::Mode<BsMenuBingoCard>::GetName(Func func) const;
// 0x0082B760 slot 0x08
template const char* state::Mode<BsMenuLetterMgr>::GetName(Func func) const;
// 0x0082B7D8 slot 0x08
template const char* state::Mode<BsMenuMiiSelect>::GetName(Func func) const;
// 0x0082B850 slot 0x08
template const char* state::Mode<BsMenuSetupTime>::GetName(Func func) const;
// 0x0082B8C8 slot 0x08
template const char* state::Mode<BsTimeBelWindow>::GetName(Func func) const;
// 0x0082B940 slot 0x08
template const char* state::Mode<RollTextLicence>::GetName(Func func) const;
// 0x0082B9B8 slot 0x08
template const char* state::Mode<BsMenuItemChange>::GetName(Func func) const;
// 0x0082BA30 slot 0x08
template const char* state::Mode<BsMenuItemSelect>::GetName(Func func) const;
// 0x0082BAA8 slot 0x08
template const char* state::Mode<BsMenuMailSelect>::GetName(Func func) const;
// 0x0082BB20 slot 0x08
template const char* state::Mode<BsMenuMapVillage>::GetName(Func func) const;
// 0x0082BB98 slot 0x08
template const char* state::Mode<BsMenuSendMailMe>::GetName(Func func) const;
// 0x0082BC10 slot 0x08
template const char* state::Mode<BsMenuTourSelect>::GetName(Func func) const;
// 0x0082BC88 slot 0x08
template const char* state::Mode<ChangeRentalBase>::GetName(Func func) const;
// 0x0082BD78 slot 0x08
template const char* state::Mode<BsMenuLetterStock>::GetName(Func func) const;
// 0x0082BDF0 slot 0x08
template const char* state::Mode<BsMenuNumberInput>::GetName(Func func) const;
// 0x0082BE68 slot 0x08
template const char* state::Mode<ChangeStockLetter>::GetName(Func func) const;
// 0x0082BEE0 slot 0x08
template const char* state::Mode<ControlButtonBase>::GetName(Func func) const;
// 0x0082BF58 slot 0x08
template const char* state::Mode<LifeSupportWindow>::GetName(Func func) const;
// 0x0082BFD0 slot 0x08
template const char* state::Mode<BsMenuExplainPlate>::GetName(Func func) const;
// 0x0082C048 slot 0x08
template const char* state::Mode<BsMenuLetterEditer>::GetName(Func func) const;
// 0x0082C0C0 slot 0x08
template const char* state::Mode<BsMenuMyDesignKeep>::GetName(Func func) const;
// 0x0082C138 slot 0x08
template const char* state::Mode<BsMenuRentalHaniwa>::GetName(Func func) const;
// 0x0082C1B0 slot 0x08
template const char* state::Mode<BsMenuSendMailLand>::GetName(Func func) const;
// 0x0082C228 slot 0x08
template const char* state::Mode<ExplainBookContent>::GetName(Func func) const;
// 0x0082C2A0 slot 0x08
template const char* state::Mode<HomeBtnProhibition>::GetName(Func func) const;
// 0x0082C318 slot 0x08
template const char* state::Mode<AddressSelectWindow>::GetName(Func func) const;
// 0x0082C390 slot 0x08
template const char* state::Mode<BsMenuBulletinBoard>::GetName(Func func) const;
// 0x0082C408 slot 0x08
template const char* state::Mode<BsMenuFriendChatLog>::GetName(Func func) const;
// 0x0082C480 slot 0x08
template const char* state::Mode<BsMenuRandomHeadsUp>::GetName(Func func) const;
// 0x0082C4F8 slot 0x08
template const char* state::Mode<ChangeStockMyDesign>::GetName(Func func) const;
// 0x0082C570 slot 0x08
template const char* state::Mode<BsMenuBestFriendList>::GetName(Func func) const;
// 0x0082C5E8 slot 0x08
template const char* state::Mode<BsMenuModelHomeBoard>::GetName(Func func) const;
// 0x0082C660 slot 0x08
template const char* state::Mode<BsMenuMyDesignSelect>::GetName(Func func) const;
// 0x0082C6D8 slot 0x08
template const char* state::Mode<BsMenuSantaBagSelect>::GetName(Func func) const;
// 0x0082C750 slot 0x08
template const char* state::Mode<ItemSelectNameWindow>::GetName(Func func) const;
// 0x0082C7C8 slot 0x08
template const char* state::Mode<LetterDragItemWindow>::GetName(Func func) const;
// 0x0082C840 slot 0x08
template const char* state::Mode<LetterDragPostWindow>::GetName(Func func) const;
// 0x0082C8B8 slot 0x08
template const char* state::Mode<BsMenuBorrowThingList>::GetName(Func func) const;
// 0x0082C930 slot 0x08
template const char* state::Mode<BsMenuRoomLightSwitch>::GetName(Func func) const;
// 0x0082C9A8 slot 0x08
template const char* state::Mode<LetterDragStockWindow>::GetName(Func func) const;
// 0x0082CA20 slot 0x08
template const char* state::Mode<BsMenuPassportForGhost>::GetName(Func func) const;
// 0x0082CA98 slot 0x08
template const char* state::Mode<BsMenuPassportForPhoto>::GetName(Func func) const;
// 0x0082CB10 slot 0x08
template const char* state::Mode<BsMenuBestFriendListMgr>::GetName(Func func) const;
// 0x0082CB88 slot 0x08
template const char* state::Mode<BsMenuPassportForFriend>::GetName(Func func) const;
// 0x0082CC00 slot 0x08
template const char* state::Mode<BsMenuBestFriendRegister>::GetName(Func func) const;
// 0x0082CC78 slot 0x08
template const char* state::Mode<BsMenuWhatTreeIsThisTree>::GetName(Func func) const;
// 0x0082CCF0 slot 0x08
template const char* state::Mode<BsMenuMyDesignDreamPresent>::GetName(Func func) const;
// 0x0082CED0 slot 0x08
template const char* state::Mode<Fade>::GetName(Func func) const;
// 0x0082CF48 slot 0x08
template const char* state::Mode<Wipe>::GetName(Func func) const;
// 0x0082CFC0 slot 0x08
template const char* state::Mode<BsSkb>::GetName(Func func) const;
// 0x0082D038 slot 0x08
template const char* state::Mode<BsSvMgr>::GetName(Func func) const;
// 0x0082D0B0 slot 0x08
template const char* state::Mode<BsTelop>::GetName(Func func) const;
// 0x0082D128 slot 0x08
template const char* state::Mode<Caution>::GetName(Func func) const;
// 0x0082D1A0 slot 0x08
template const char* state::Mode<Palette>::GetName(Func func) const;
// 0x0082D218 slot 0x08
template const char* state::Mode<BsMenuBg>::GetName(Func func) const;
// 0x0082D290 slot 0x08
template const char* state::Mode<BsSvDemo>::GetName(Func func) const;
// 0x0082D308 slot 0x08
template const char* state::Mode<ComTitle>::GetName(Func func) const;
// 0x0082D380 slot 0x08
template const char* state::Mode<Passport>::GetName(Func func) const;
// 0x0082D3F8 slot 0x08
template const char* state::Mode<RollText>::GetName(Func func) const;
// 0x0082D4E8 slot 0x08
template const char* state::Mode<AtmWindow>::GetName(Func func) const;
// 0x0082D560 slot 0x08
template const char* state::Mode<BlackWait>::GetName(Func func) const;
// 0x0082D5D8 slot 0x08
template const char* state::Mode<BsMenuAtm>::GetName(Func func) const;
// 0x0082D650 slot 0x08
template const char* state::Mode<BsMenuMap>::GetName(Func func) const;
// 0x0082D6C8 slot 0x08
template const char* state::Mode<BsMenuMgr>::GetName(Func func) const;
// 0x0082D740 slot 0x08
template const char* state::Mode<BsMenuTab>::GetName(Func func) const;
// 0x0082D830 slot 0x08
template const char* state::Mode<TenkeyPad>::GetName(Func func) const;
// 0x0082D8A8 slot 0x08
template const char* state::Mode<BsTimeBelWindow::MoneyProc>::GetName(Func func) const;
// 0x0082D920 slot 0x08
template const char* state::Mode<nfp::Framework>::GetName(Func func) const;

// The destructors of state::Mode<T> that ARMCC emitted (the deleting one is `b operator delete`)
// 0x00818150
// 0x0081814C (deleting dtor)
template state::Mode<CensusWindow>::~Mode();
// 0x0081A2B8
// 0x0081A2B4 (deleting dtor)
template state::Mode<LifeSupportWindow>::~Mode();
// 0x0081C744
// 0x0081C740 (deleting dtor)
template state::Mode<Passport>::~Mode();
