#pragma once

// Forward declarations of every known class / template and
// placeholder definitions for types that are only known from function signatures.
#include "types.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/os/os_Types.h"


class AStarForeachCB;
class AStarForeachPreSisyoCB;
class AcArtPlate;
class AcAutoCampBird;
class AcAutoCampEntrySignboard;
class AcBalloon;
class AcBrochure;
class AcCockroach;
class AcFishCommon;
class AcFishFieldBase;
class AcFishMuseumBase;
class AcFishMuseumRiver;
class AcFishMuseumSea;
class AcFossilPartsPlate;
class AcFossilSinglePlate;
class AcFsFdShadow;
class AcFsMuBaseMove;
class AcFsMuBaseMoveSea;
class AcFsMuCarp;
class AcFsMuCylinder;
class AcFsMuDefault;
class AcFsMuEel;
class AcFsMuFast;
class AcFsMuFlatfish;
class AcFsMuFloat;
class AcFsMuFrog;
class AcFsMuKeepSwimming;
class AcFsMuMoray;
class AcFsMuOarfish;
class AcFsMuObjColBase;
class AcFsMuObjLotus;
class AcFsMuObjSimple;
class AcFsMuPiranha;
class AcFsMuPufferfish;
class AcFsMuRay;
class AcFsMuSeaConger;
class AcFsMuSeaCrab;
class AcFsMuSeaCrabSp;
class AcFsMuSeaLobster;
class AcFsMuSeaMollusc;
class AcFsMuSeaScallop;
class AcFsMuSeaShrimp;
class AcFsMuSeaSimple;
class AcFsMuShark;
class AcFsMuSoftShelledTurtle;
class AcFsMuSquid;
class AcFsMuSunfish;
class AcFsMuWander;
class AcFtr2Way;
class AcFtr;
class AcFtrATM;
class AcFtrAudio;
class AcFtrBarley;
class AcFtrBed;
class AcFtrBromide;
class AcFtrChair;
class AcFtrChest;
class AcFtrChirp;
class AcFtrClock;
class AcFtrClockDigital;
class AcFtrCloth;
class AcFtrCompass;
class AcFtrFireWater;
class AcFtrFlip;
class AcFtrFlipSwitch;
class AcFtrGossipStone;
class AcFtrHaniwa;
class AcFtrHouseParts;
class AcFtrInstrument;
class AcFtrKiller;
class AcFtrLoop;
class AcFtrLoopTrigger;
class AcFtrMannequin;
class AcFtrManualBook;
class AcFtrMetronome;
class AcFtrMiniGame3DS;
class AcFtrMiniGameWiiU;
class AcFtrMonitor;
class AcFtrMusicJacket;
class AcFtrMyDesignObject;
class AcFtrOnlyCatalog;
class AcFtrPigBank;
class AcFtrPushcart;
class AcFtrRemakeOrgel;
class AcFtrRoller;
class AcFtrStand;
class AcFtrSwitch2;
class AcFtrSwitch;
class AcFtrTV;
class AcFtrTVFlip;
class AcFtrTelephone;
class AcFtrTheremin;
class AcFtrTrashBox;
class AcFtrTrigger;
class AcFtrTrophy;
class AcFtrUmbrella;
class AcFtrYutaroLamp;
class AcHomeTicketMachine;
class AcInsectCommon;
class AcInsectFieldBase;
class AcInsectFieldFly;
class AcInsectFieldFlyPursue;
class AcInsectFieldRestless;
class AcInsectFieldStraight;
class AcInsectFieldSwim;
class AcInsectFishPlate;
class AcInsectMuseumBase;
class AcIsFdAnt;
class AcIsFdBeetle;
class AcIsFdButterfly;
class AcIsFdCastOffSkin;
class AcIsFdCicada;
class AcIsFdDragonfly;
class AcIsFdFeather;
class AcIsFdFirefly;
class AcIsFdFlea;
class AcIsFdFlower;
class AcIsFdFly;
class AcIsFdHermitCrab;
class AcIsFdHoneyBee;
class AcIsFdHornet;
class AcIsFdLeaf;
class AcIsFdLocust;
class AcIsFdMoleCricket;
class AcIsFdMosquito;
class AcIsFdMoth;
class AcIsFdPillBug;
class AcIsFdScorpion;
class AcIsFdSeaSlater;
class AcIsFdSnowCrystal;
class AcIsFdSpider;
class AcIsFdStump;
class AcIsFdTigerBeetle;
class AcIsFdTumblebug;
class AcIsFdWaterBeetle;
class AcIsFdWaterStrider;
class AcIsMuAnt;
class AcIsMuBeetle;
class AcIsMuButterfly;
class AcIsMuCastOffSkin;
class AcIsMuCicada;
class AcIsMuDragonfly;
class AcIsMuFeather;
class AcIsMuFirefly;
class AcIsMuFlea;
class AcIsMuFlower;
class AcIsMuFly;
class AcIsMuHermitCrab;
class AcIsMuHoneyBee;
class AcIsMuHornet;
class AcIsMuLeaf;
class AcIsMuLocust;
class AcIsMuMoleCricket;
class AcIsMuMosquito;
class AcIsMuMoth;
class AcIsMuPillBug;
class AcIsMuScorpion;
class AcIsMuSeaSlater;
class AcIsMuSnowCrystal;
class AcIsMuSpider;
class AcIsMuStump;
class AcIsMuTigerBeetle;
class AcIsMuTumblebug;
class AcIsMuWaterBeetle;
class AcIsMuWaterStrider;
class AcNpc;
class AcNpcApril;
class AcNpcAutoCamp;
class AcNpcAutoCampOut;
class AcNpcBoardingDemoDollPlayer;
class AcNpcBoardingDemoDollSp;
class AcNpcCamp;
class AcNpcDemoDoll;
class AcNpcDemoDollAmiiboCamera;
class AcNpcDemoDollAmiiboCameraBase;
class AcNpcDemoDollAmiiboCameraPlayer;
class AcNpcDemoDollAmiiboCameraSp;
class AcNpcDemoDollBase;
class AcNpcDemoDollPlayer;
class AcNpcDemoDollSp;
class AcNpcDt;
class AcNpcIn;
class AcNpcKotobukiDemoDollSp;
class AcNpcNml;
class AcNpcOut;
class AcNpcPlayerGhost;
class AcNpcSp;
class AcNpcSpAsami;
class AcNpcSpBerlina;
class AcNpcSpCafeTalk;
class AcNpcSpCleaning;
class AcNpcSpCleaningVisit;
class AcNpcSpDJKK;
class AcNpcSpDream;
class AcNpcSpExchangeMaster;
class AcNpcSpExchangeReset;
class AcNpcSpExhibition;
class AcNpcSpExhibitionIn;
class AcNpcSpExhibitionOut;
class AcNpcSpFortuneteller;
class AcNpcSpFrankrin;
class AcNpcSpFuta;
class AcNpcSpGraceOut;
class AcNpcSpHonma;
class AcNpcSpJingle;
class AcNpcSpJonny;
class AcNpcSpKaburiba;
class AcNpcSpKameyama;
class AcNpcSpKappei;
class AcNpcSpKappeisKidOne;
class AcNpcSpKotobukiAnnounce;
class AcNpcSpKotobukiBalloon;
class AcNpcSpKotobukiBase;
class AcNpcSpKotobukiBorrowingRace;
class AcNpcSpKotobukiDiving;
class AcNpcSpKotobukiFish;
class AcNpcSpKotobukiFlower;
class AcNpcSpKotobukiFossilDig;
class AcNpcSpKotobukiHideAndSeek;
class AcNpcSpKotobukiInsect;
class AcNpcSpKotobukiMaze;
class AcNpcSpKotobukiMining;
class AcNpcSpKotobukiPikoHan;
class AcNpcSpKotobukiTreasureHunt;
class AcNpcSpMaiko;
class AcNpcSpManualImport;
class AcNpcSpMaster;
class AcNpcSpMysteryCat;
class AcNpcSpPalloncino;
class AcNpcSpPeriko;
class AcNpcSpPerimi;
class AcNpcSpPerioNormal;
class AcNpcSpPerioSpecial;
class AcNpcSpPerioTutorial;
class AcNpcSpPerioWarning;
class AcNpcSpPlSelect;
class AcNpcSpPostOffice;
class AcNpcSpPreSisyo;
class AcNpcSpPrologueTanukichi;
class AcNpcSpPumpking;
class AcNpcSpPumpkingPre;
class AcNpcSpPyontarou;
class AcNpcSpRacketsanIn;
class AcNpcSpRakosuke;
class AcNpcSpResetsan;
class AcNpcSpResetsanIn;
class AcNpcSpRollan;
class AcNpcSpSecretaryAnniversary;
class AcNpcSpSecretaryCeremony;
class AcNpcSpSecretaryCompass;
class AcNpcSpSecretaryEvent;
class AcNpcSpSecretaryGpEvent;
class AcNpcSpSecretaryIn;
class AcNpcSpSecretaryOut;
class AcNpcSpSecretaryPWorks;
class AcNpcSpSecretaryPlantTree;
class AcNpcSpSecretaryTakumiImport;
class AcNpcSpSecretaryTutorial;
class AcNpcSpSecretaryTutorialWP;
class AcNpcSpSecretaryUpdateTrain;
class AcNpcSpSeiichi;
class AcNpcSpSeniorTakumiImport;
class AcNpcSpShop;
class AcNpcSpShopCamp;
class AcNpcSpShopCatherine;
class AcNpcSpShopFuko;
class AcNpcSpShopGardening;
class AcNpcSpShopGrace;
class AcNpcSpShopIsland;
class AcNpcSpShopKate;
class AcNpcSpShopKinuyo;
class AcNpcSpShopKodanuki;
class AcNpcSpShopMamekichi;
class AcNpcSpShopPolice;
class AcNpcSpShopPoliceA;
class AcNpcSpShopPoliceB;
class AcNpcSpShopRecycle;
class AcNpcSpShopRemake;
class AcNpcSpShopShoes;
class AcNpcSpShopTanukichi;
class AcNpcSpShopTubukichi;
class AcNpcSpShopTunekichi;
class AcNpcSpSisyo;
class AcNpcSpStationMaster;
class AcNpcSpTakumiChipTutorial;
class AcNpcSpTakumiRealEstate;
class AcNpcSpTotakeke;
class AcNpcSpTourDesk;
class AcNpcSpTrain;
class AcNpcSpTunekichiEvent;
class AcNpcSpUomasa;
class AcNpcSpYutarou;
class AcNpcSpYutarouVisit;
class AcNpcTopsChangeDemoDollSp;
class AcNpcTopsChangeDemoDollSpBase;
class AcNpcTour;
class AcObjUnitCursor;
class AcObjWallHangBack;
class AcObjWallHangUnitCursor;
class AcObjectBase;
class AcObjectShow;
class AcPhotoMgr;
class AcPikoHanObj;
class AcPlayer;
class AcRobjCatalogMachine;
class AcRobjCoolerBox;
class AcRobjGate;
class AcRobjHaniwa;
class AcRobjHighTech;
class AcRobjLiveChair;
class AcRobjPermaMachine;
class AcRobjPinLight;
class AcRobjPlayerSelect;
class AcRobjPoster;
class AcRobjProjector;
class AcRobjRailway;
class AcRobjResetChair;
class AcRobjResetMachine;
class AcRobjResetTV;
class AcRobjSewingMachine;
class AcRobjSpeaker;
class AcRobjTrain;
class AcSimpleTalkBase;
class AcSnowBall;
class AcSnowBallHIO;
class AcStrc;
class AcStrcBbs;
class AcStrcBbsBird;
class AcStrcBoat;
class AcStrcBridge;
class AcStrcCampingCar;
class AcStrcCarnivalStage;
class AcStrcCoolerBox;
class AcStrcCountDownBBS;
class AcStrcCrossing;
class AcStrcDemoPier;
class AcStrcDonationBox;
class AcStrcDowntownAtt;
class AcStrcDowntownBench;
class AcStrcDowntownLamp;
class AcStrcDowntownObj;
class AcStrcFacility;
class AcStrcFieldObj;
class AcStrcGeyser;
class AcStrcIslandHut;
class AcStrcLightHouse;
class AcStrcModelHouse;
class AcStrcModelHousePlate;
class AcStrcMyDesignSign;
class AcStrcNpcHouse;
class AcStrcOffice;
class AcStrcPhotoBox;
class AcStrcPier;
class AcStrcPlayerHouse;
class AcStrcPost;
class AcStrcRecycleShop;
class AcStrcRentalHaniwa;
class AcStrcReset;
class AcStrcScreen;
class AcStrcSymboltree;
class AcStrcTableHarvest;
class AcStrcTrain;
class AcStrcTrashBox;
class AcTourBalloon;
class AcVersatileFlow;
class AcVersatilePlate;
class Actor;
class AddressBook;
class AddressSelectWindow;
class AmiiboPhotoBG;
class AreaList;
class ArtPlateTalkRecept;
class AtmWindow;
class AudioObjFall;
class AudioObjFgBase;
class AudioObjFurniture;
class AudioObjFurnitureAudio;
class AudioObjFurnitureBase;
class AudioObjFurnitureInsect;
class AudioObjFurnitureInst;
class AudioObjFurnitureOrgel;
class AudioObjHaniwa;
class AudioObjPond;
class AudioObjRiver;
class AudioPlayerContent;
class AutoCampBirdAction;
class AutoCampBirdHeadCtrl;
class AutoCampBook;
class BadgeExplainWindow;
class BalloonLine;
class BalloonString;
class BankTableBase;
class BankTablePrivate;
class Base;
class BingoMassBase;
class BingoMassCenter;
class BingoMassNormal;
class BiquadNwAdaptor;
class BkUnitSearchCandCb;
class BlackToBtn;
class BlackWait;
class BoardRecept;
class BookIconBase;
class BookParameterAccess;
class BossMgr;
class BossSys;
class BothHandPartsAnimSetter;
class BrochureTalkRecept;
class BsAmiiboCamera;
class BsAmiiboPhotoMgr;
class BsAquariumMgr;
class BsArtPlateMgr;
class BsAudioLoadSignal;
class BsAutoCampBirdMgr;
class BsBalloonChat;
class BsBgSeMgr;
class BsBuoyMgr;
class BsBuoyString;
class BsCafeCupIn;
class BsCamera;
class BsCfl;
class BsCharRoomViewMgr;
class BsClub444ViewMgr;
class BsCockroachMgr;
class BsCommonDialog;
class BsDemoCancelMgr;
class BsEscapeFogMgr;
class BsEventMgr;
class BsFireworkMgr;
class BsFireworkMgrHioNode;
class BsFish0AquariumMgr;
class BsFishFieldMgr;
class BsFishMuseumMgr;
class BsFlag;
class BsFogMgr;
class BsFogMgrHioNode;
class BsFortuneViewMgr;
class BsFossilPlateMgr;
class BsFtrMgr;
class BsGeneralObj;
class BsGrRenderer;
class BsGrowUp;
class BsHandCursor;
class BsHandItem;
class BsIncludeSave;
class BsIndoorDoorSE;
class BsIndoorPlateMgr;
class BsIndoorViewMgr;
class BsInsectFieldMgr;
class BsInsectMuseumMgr;
class BsLightAmbient;
class BsLightAmbientCatalog;
class BsLightAmbientClub;
class BsLightAmbientFix;
class BsLightAmbientFixDemo;
class BsLightBase;
class BsLightDiffuseBase;
class BsLightDiffuseCatalog;
class BsLightDiffuseClub;
class BsLightDiffuseDarkroom;
class BsLightDiffuseInBg;
class BsLightDiffuseInObj;
class BsLightDiffuseOutBg;
class BsLightDiffuseOutObj;
class BsLightDiffuseShowroom;
class BsLightFixBase;
class BsLightFixDemo;
class BsLightFixParam;
class BsLightHemiSphereBase;
class BsLightHemiSphereCatalog;
class BsLightHemiSphereClub;
class BsLightHemiSphereDarkroom;
class BsLightHemiSphereFix;
class BsLightHemiSphereFixDemo;
class BsLightHemiSphereIn;
class BsLightHemiSphereOut;
class BsLightHemiSphereShowroom;
class BsLightMgr;
class BsLightPointBase;
class BsLightPointFix;
class BsLightSpot;
class BsMenuAmiiboPhoto;
class BsMenuAmiiboShutter;
class BsMenuAppraiser;
class BsMenuAreaResult;
class BsMenuAreaSelect;
class BsMenuAtm;
class BsMenuAudioPlayer;
class BsMenuAudioPlayerMgr;
class BsMenuBbsEditer;
class BsMenuBbsMgr;
class BsMenuBestFriendList;
class BsMenuBestFriendListMgr;
class BsMenuBestFriendRegister;
class BsMenuBg;
class BsMenuBingoCard;
class BsMenuBook;
class BsMenuBorrowThingList;
class BsMenuBulletinBoard;
class BsMenuCamper;
class BsMenuCatalog;
class BsMenuChat;
class BsMenuChest;
class BsMenuCoolbox;
class BsMenuCredit;
class BsMenuEdit;
class BsMenuExplainPlate;
class BsMenuFriendChat;
class BsMenuFriendChatLog;
class BsMenuInputComment;
class BsMenuInteriorEditor;
class BsMenuItem;
class BsMenuItemChange;
class BsMenuItemMannequin;
class BsMenuItemSelect;
class BsMenuLetter;
class BsMenuLetterEditer;
class BsMenuLetterMgr;
class BsMenuLetterStock;
class BsMenuMailSelect;
class BsMenuMailbox;
class BsMenuMain;
class BsMenuMap;
class BsMenuMapBase;
class BsMenuMapIsland;
class BsMenuMapList;
class BsMenuMapLobby;
class BsMenuMapModelHome;
class BsMenuMapSelect;
class BsMenuMapShoppingStreet;
class BsMenuMapVillage;
class BsMenuMelody;
class BsMenuMgr;
class BsMenuMiiSelect;
class BsMenuModelHomeBoard;
class BsMenuMyDesign;
class BsMenuMyDesignDreamPresent;
class BsMenuMyDesignKeep;
class BsMenuMyDesignSelect;
class BsMenuNfcReader;
class BsMenuNumberInput;
class BsMenuPassport;
class BsMenuPassportForFriend;
class BsMenuPassportForGhost;
class BsMenuPassportForPhoto;
class BsMenuQREncoder;
class BsMenuQRReader;
class BsMenuRandomHeadsUp;
class BsMenuReaction;
class BsMenuRentalHaniwa;
class BsMenuRoomLightSwitch;
class BsMenuRoomMgr;
class BsMenuSantaBagSelect;
class BsMenuSantaBagView;
class BsMenuSendMailLand;
class BsMenuSendMailMe;
class BsMenuSetupTime;
class BsMenuSignList;
class BsMenuTab;
class BsMenuTicketExchange;
class BsMenuTourSelect;
class BsMenuWhatTreeIsThisTree;
class BsMiniGame0Mgr;
class BsMiniGame1Mgr;
class BsModelEffectMgr;
class BsModelEffectMgrHioNode;
class BsMyDesignEditModel;
class BsNameWindow;
class BsNpcMgr;
class BsObjCheckMgr;
class BsObjControl;
class BsOutdoorViewMgr;
class BsPikoHanObjMgr;
class BsPlayerMgr;
class BsReturnSaveMgr;
class BsRoot;
class BsScene;
class BsSeadParticleDraw2D;
class BsSeadParticleDrawBase;
class BsSeadParticleDrawBaseHioNode;
class BsSeadParticleDrawDefault;
class BsSeadParticleDrawFireworks;
class BsSeadParticleDrawFootmark;
class BsSeadParticleDrawMinigame;
class BsSeadParticleMgr;
class BsSeadParticleMgrHioNode;
class BsSealifeMuseumMgr;
class BsShadowMapMgr;
class BsShadowViewMgr;
class BsShootingStarMgr;
class BsShootingStarMgrHioNode;
class BsShowMgr;
class BsSkb;
class BsSnowBallMgr;
class BsStrcCatalog;
class BsStrcMgr;
class BsSvDemo;
class BsSvMgr;
class BsTakumiFlowLoader;
class BsTelop;
class BsThunderMgr;
class BsThunderMgrHioNode;
class BsTimeBelWindow;
class BsTitle;
class BsTourBalloonMgr;
class BsTourFieldResMgr;
class BsTourMgr;
class BsTripList;
class BsUpdateSelect;
class BsVillageBalloonMgr;
class BsVrBoxSkyHioNode;
class BsVrboxAurora;
class BsVrboxAuroraHioNode;
class BsVrboxCloud;
class BsVrboxCloudHioNode;
class BsVrboxCumulonimbus;
class BsVrboxCumulonimbusHioNode;
class BsVrboxForest;
class BsVrboxForestHioNode;
class BsVrboxRainbow;
class BsVrboxRainbowHioNode;
class BsVrboxSky;
class BsWaveMgr;
class BsWeatherPaper;
class BsWeatherPaperHioNode;
class BsWeatherRain;
class BsWeatherRainHioNode;
class BsWeatherSakura;
class BsWeatherSakuraHioNode;
class BsWeatherSnow;
class BsWeatherSnowHioNode;
class BuoyStringFunctor;
class ButtonActionAllGroupBindNode;
class ButtonActionControl;
class ButtonActionControlChoice;
class ButtonActionControlEx;
class ButtonActionControlKeep;
class ButtonActionControlTouchOkAnmSwitch;
class ButtonActionCstmNode;
class ButtonActionNode;
class ButtonActionNodeEx;
class CFLiRecentDBFile;
class CFLiResShape;
class CFLiTexHandle;
class Cabinet;
class CafeArbeitSeqTalkRecept;
class CameraBase;
class CameraCatalog;
class CameraGame;
class CameraShutter;
class CampingCarTextureBank;
class CatalogBase;
class CatalogList;
class Caution;
class CensusBG;
class CensusContentList;
class CensusWindow;
class ChangeListBase;
class ChangeMenuWindow;
class ChangeRentalBase;
class ChangeStockCooler;
class ChangeStockItem;
class ChangeStockLetter;
class ChangeStockMyDesign;
class ChangeStockRental;
class ChangeStockWarehouse;
class CheckEnterStrc;
class Chip;
class ChoiceStandardWin;
class ClothesList;
class CoffeePartsAnimSetter;
class CollectChip;
class ComButton1Lyt;
class ComButton2Lyt;
class ComButton3Lyt;
class ComButtonLytBase;
class ComDialogA;
class ComTitle;
class CommandConstSubroutine;
class CommandReuserBase;
class CommonButtonSingle;
class CommonScrollBar;
class CommonScrollBarWidth;
class ContentList;
class ContentNode;
class Control1Button;
class Control2Button;
class Control3Button;
class ControlButtonBase;
class Controller;
class CursorObjList;
class CursorObjListNode;
class DLLRoMgr;
class DataStore;
class DemoActor;
class DonationBoxTalkRecept;
class DowntownBook;
class DragIconControl;
class DragIconGroup;
class DragIconNode;
class DreamGhostCandCB;
class DreamGhostSetForeachCB;
class DtWanderSearchCandCB;
class DuckingFader;
class EmoticonAction;
class EscapeSelectWindow;
class EventBook;
class EventDemoStationSeqRecepter;
class EventSecretarySearchFunc;
class ExcavateList;
class ExchangeBankBase;
class ExchangeBankKeywordCampingCarTexture;
class ExchangeModelBankBase;
class ExhibitionEastBook;
class ExhibitionEntranceBook;
class ExhibitionNorthBook;
class ExhibitionOtherEastBook;
class ExhibitionOtherNorthBook;
class ExhibitionOtherWestBook;
class ExhibitionParkInfo;
class ExhibitionWestBook;
class ExplainBookContent;
class ExplainPlate;
class ExplainPlateBase;
class ExplainWindow;
class Fade;
class FaderColor;
class FaderMoment;
class FaderWipe;
class FdBkSearchCand;
class FdInfo;
class Field;
class FieldBuilder;
class FieldRect;
class FishAwardSeqTalkRecept;
class FishingCandCB;
class FlagFunctor;
class FootMark;
class ForeachOnFallToPitfall;
class ForeachOnMoveOutPitfall;
class ForeachOnPlayerEndEmoticon;
class ForeachOnPlayerStartEmoticon;
class ForeachOnStrugglePitfall;
class FortuneCamera;
class FossilPartsPlateTalkRecept;
class FossilSinglePlateTalkRecept;
class FrameBufferShadow;
class FriendChatLogText;
class FriendList;
class FriendListText;
class FtrList;
class GameIcon;
class GetVisitNpcIdxCB;
class GmoLoader;
class GridButtonNode;
class HalloweenWalk;
class HandCursorHioNode;
class HomeBtnProhibition;
class HumanFaceAnimControl;
class HumanModel;
class HumanResTextureAnimBank;
class ICameraUpdater;
class IconBase;
class InOutWindow;
class InsectAwardSeqTalkRecept;
class IslandData;
class IslandHutPlateTalkRecept;
class ItemDragIconGroup;
class ItemDragIconNode;
class ItemDragWindow;
class ItemMultiDragIconGroup;
class ItemMultiDragIconNode;
class ItemSelectNameWindow;
class ItemSelectWindow;
class Jingle;
class JmpBlock;
class Layout;
class LetterDragIconGroup;
class LetterDragIconNode;
class LetterDragItemWindow;
class LetterDragPostWindow;
class LetterDragStockWindow;
class LetterDragWindowBase;
class LetterList;
class LifeSupportExplainWindow;
class LifeSupportWindow;
class LightIcon;
class ListId;
class ListNodeId;
class ListNodePriority;
class LobbyIslandBook;
class LytAppraiser;
class LytNfcTouch;
class LytReactionButton;
class LytTicketExchangeBg;
class LytTicketExchangeButton;
class LytTicketExchangeIcon;
class LytTicketExchangeId;
class LytTicketExchangeLinkSeq;
class LytTicketExchangeList;
class LytTicketExchangeListTop;
class LytTicketExchangeMsg;
class LytTicketExchangeReceiptSeq;
class LytTicketExchangeResultTop;
class LytTicketExchangeSetupMenu;
class LytTicketExchangeSetupSeq;
class LytTicketExchangeSmaphoLinkSeq;
class LytTicketExchangeTax;
class LytTicketExchangeTaxPaymentsSeq;
class LytTicketExchangeTopMenu;
class LytTicketExchangeTopMenuSeq;
class LytTicketExchangeWait;
class LytTicketExchangeWaitTop;
class MacroRoadSearch;
class MailLoadAdapt;
class MailWork;
class MapFaceIcon;
class MapIcon;
class MenuBase;
class MenuItemHioNode;
class MigrationNetModule;
class MigrationReceiver;
class MoveFromTalkRecept;
class MoveFromTalkReceptBase;
class MsgWindow;
class MuseumListRecept;
class MuseumPlateBaseTalkRecept;
class MusicJacketTextureBank;
class MusicList;
class MyButtonActionControl;
class MyButtonActionNode;
class MyDesignDragIconNode;
class MyDesignProDragIconNode;
class NameList;
class NfcErrorRecept;
class NoticeLoadAdapt;
class NoticeWork;
class NpcAction;
class NpcAutoCampAStarForeachCB;
class NpcBoardingSimpleModel;
class NpcBuildStrEscapeSetupCandCB;
class NpcBuildStrcEscapeForeachCB;
class NpcCalcMtxRequestFunction;
class NpcCampAStarForeachCB;
class NpcCampingCarInfoLoader;
class NpcDJCtrl;
class NpcDcoAnimModel;
class NpcDemoDollTalkRecept;
class NpcDtAStarForeachCB;
class NpcHeadCtrl;
class NpcHousePlateTalkRecept;
class NpcInAStarForeachCB;
class NpcKameyamaModel;
class NpcKotobukiModel;
class NpcLockPlayerPermitBit;
class NpcModel;
class NpcModelBase;
class NpcMove;
class NpcNetPacketMgr;
class NpcNetShareInfo;
class NpcOnBuildFunc;
class NpcOnPreviewEndFunc;
class NpcOnPreviewFunc;
class NpcOutAStarForeachCB;
class NpcOutAStarForeachCBWithGoal;
class NpcOutAStarSetupCand6CB;
class NpcOutAStarSetupCandCB;
class NpcOutAStarSetupCandCDBoardCB;
class NpcOutMacroRoadSearchCB;
class NpcPanielModel;
class NpcPermitBitMgr;
class NpcRakosukeModel;
class NpcResetChairSimpleModel;
class NpcResetModel;
class NpcSimpleModel;
class NpcSpTalkRecept;
class NpcTalkRecept;
class NpcTopsBank;
class NpcTopsChangeModel;
class NpcWanderTalkRecept;
class NpcYutarouModel;
class ObjTalkRecept;
class ObjcBody;
class ObjectResource;
class OrderList;
class OtherNpcSearchCB;
class PWorkDeleteList;
class PWorkList;
class Palette;
class PaneponBG;
class PaneponCmnBLayout;
class PaneponConfigure;
class PaneponCutin;
class PaneponDemoSeqTalkRecept;
class PaneponDialog;
class PaneponFaceIcon;
class PaneponHelpWin;
class PaneponMenu;
class PaneponNfp;
class PaneponSvData;
class PaneponTelopFinal;
class ParameterHIO;
class PartsAnimSetter;
class PartsDragIconNode;
class Passport;
class PatchFileDevice;
class PcbdayGuestCB;
class PhotoBoxFrame;
class PhotoBoxRecept;
class PhotoExplainWindow;
class PhotoFrame;
class PhotoTimer;
class PickerBase;
class PickerMono;
class PickerParts;
class PlHouseGhostSetForeachCB;
class PlayerAcceBank;
class PlayerBoardingModel;
class PlayerBottomsBank;
class PlayerCapBank;
class PlayerFaceBank;
class PlayerHeadBank;
class PlayerLegsBank;
class PlayerModel;
class PlayerShoesBank;
class PlayerSimpleMessage;
class PlayerTopsBank;
class PostDragWindow;
class PosterTalkRecept;
class PresentRootCandCB;
class ProButtonNode;
class ProcMgr;
class ProcThread;
class RandomPlacer;
class ReactionDragIconNode;
class RecycleAStarForeachCB;
class RecycleSignTalkRecept;
class RentalHaniwaTalkRecept;
class ResourceGetMaterial;
class ResourceGetSkeletal;
class ResourceGetSklMat;
class ResourceGetSklVis;
class ResourceGetSklVisMat;
class ResourceLoadAsyncSkeletal;
class ResourceLoadSkeletal;
class ResourceNone;
class ResultList;
class RhandPartsAnimSetter;
class RoModule;
class RoadSearchAgent;
class RoadSearchAgentHasGoal;
class RoadSearchCandCore;
class RoadSearchDefaultAgent;
class RoadSearchForeachNodeCB;
class RoadSearchSetupCandCB;
class RollText;
class RollTextDate;
class RollTextHistory;
class RollTextLicence;
class RollTextName;
class RollTextOfficial;
class RollanDemoCamera;
class RoomLayoutWindow;
class RootTask;
class SantaBagIcon;
class SantaBagWindow;
class SaveProcess;
class ScBoot;
class ScStage;
class ScStageMiniGame0;
class ScStageMiniGame1;
class ScreenRecept;
class SeaDemoCamera;
class SeaDepartureSeqTalkRecept;
class SeadParticle;
class SeadParticleHandle;
class SearchCandCore;
class SearchCandNearestUnitFromPlayer;
class SearchCandXZCore;
class SearchLamp;
class SearchLampSwOn;
class SearchList;
class SearchMasterCB;
class SearchMysteryCatCB;
class SearchNpcByProductIdCB;
class SelectBase;
class SelectCursor;
class SelectWindow;
class ServeTimeRecept;
class SetDateTimeBg;
class ShopAgent;
class SignRecept;
class SimpleCommentWindow;
class SkbHioNode;
class SoPaCaWindowBase;
class SoundBarcarollePlayer;
class SoundBgmMgr;
class SoundBiquadFilterWithFade;
class SoundConductor;
class SoundDJPlayer;
class SoundDataLoadMgr;
class SoundDataLoadRequest;
class SoundDataLoadThread;
class SoundDoubutsugoMgr;
class SoundEnvMgr;
class SoundFader;
class SoundFaderEx;
class SoundFaderPlayer;
class SoundFurnitureMgr;
class SoundFxMgr;
class SoundHandleBindSeq;
class SoundHaniwaMgr;
class SoundHistoryMgr;
class SoundIHaniwaControl;
class SoundKKInfo;
class SoundLivePlayer;
class SoundMelodyMgr;
class SoundMelodyPlayerBase;
class SoundMelodyPlayerOneSeqBase;
class SoundMelodyPlayerOneSeqPos;
class SoundMelodyPlayerOneSeqTimeTone;
class SoundMelodyPlayerPos;
class SoundMgr;
class SoundMicMgr;
class SoundMuInfo;
class SoundObjBase;
class SoundObjCar;
class SoundObjExtVolume;
class SoundObjFall;
class SoundObjFieldObj;
class SoundObjFurniture;
class SoundObjFurnitureAudio;
class SoundObjFurnitureInsect;
class SoundObjFurnitureInst;
class SoundObjFurnitureOrgel;
class SoundObjHaniwa;
class SoundObjHold;
class SoundObjInsectFish;
class SoundObjNpc;
class SoundObjPlayer;
class SoundObjPond;
class SoundObjRiver;
class SoundObjSimple;
class SoundObjSymbolTree;
class SoundObjWithMelody;
class SoundRiverMgr;
class SoundRoomMusicPlayer;
class SoundSeaMgr;
class SoundSeqParser;
class SoundSharedObjMgr;
class SoundTv00Storm;
class SoundTv01Color;
class SoundTv02News;
class SoundTv03Drama;
class SoundTv04Song;
class SoundTv05Quiz;
class SoundTv06Travel;
class SoundTv07Hero;
class SoundTv08Variety;
class SoundTv09Movie;
class SoundTv10Soccer;
class SoundTv11Talk;
class SoundTv12Anime;
class SoundTv13Cooking;
class SoundTv14Exercize;
class SoundTv15AppleCM;
class SoundTv16SingCM;
class SoundTv17SnowCM;
class SoundTv18UfoCM;
class SoundTv19ShoppingCM;
class SoundTv20JuiceCM;
class SoundTv21LaundryCM;
class SoundTv255Weather;
class SoundTvBase;
class SoundVolumeMgr;
class StationComeHomeSeqTalkRecept;
class StationEntrySeqTalkRecept;
class StationPrologueSeqTalkRecept;
class StationRetireSeqTalkRecept;
class SvBestFriendList;
class SvFgName;
class SvPlayerInventory;
class SvProc;
class SvStepInner;
class SystemTask;
class TTKKSearchCB;
class TakeOtherDelegate;
class TakeToolDelegate;
class TaxList;
class TenkeyPad;
class TicketList;
class TimeBellHioNode;
class ToolBank;
class ToolButtonNode;
class TotakekeLiveCamera;
class TourData;
class TourHideInfo;
class TourIslandBook;
class TourList;
class TourResult;
class TourResultSeqTalkRecept;
class TrainCamera;
class TrainSearchPartnerFunction;
class TripList;
class TryLockPlayerBase;
class TryLockPlayerSyncSpeak;
class TryLockPlayerSyncSpeakOnBoat;
class UlcdFramework;
class VersatileFlowTalkRecept;
class VersatilePlateTalkRecept;
class VillageBook;
class VillageSpBook;
class VisitedPlayerData;
class VisitorLocalInfo;
class VolumeHIO;
class WaitIcon;
class WanderAgent2;
class WanderAgent;
class WanderSearchMaxMinCandCB;
class WaterCurtain;
class Wipe;
class dLoadSplit;
struct CFLProgramContext { u32 _unknown; }; // placeholder, real type unknown
struct CFLTexFmt { u32 _unknown; }; // placeholder, real type unknown
struct CFLTexWrap { u32 _unknown; }; // placeholder, real type unknown
struct CFLiTexHeader { u32 _unknown; }; // placeholder, real type unknown
struct FieldName { u32 _unknown; }; // placeholder, real type unknown
struct PicaDataColor { u32 _unknown; }; // placeholder, real type unknown
struct PicaDataDepth { u32 _unknown; }; // placeholder, real type unknown
struct PicaDataDrawMode { u32 _unknown; }; // placeholder, real type unknown
struct PlayerCapBankKeyword { u32 _unknown; }; // placeholder, real type unknown
struct PlayerHeadBankKeyword { u32 _unknown; }; // placeholder, real type unknown
struct PlayerNumber { u32 _unknown; }; // placeholder, real type unknown
struct PlayerSaveNumber { u32 _unknown; }; // placeholder, real type unknown
struct PlayerState { u32 _unknown; }; // placeholder, real type unknown
struct ProcName { u32 _unknown; }; // placeholder, real type unknown
struct ProcType { u32 _unknown; }; // placeholder, real type unknown
struct RandomPlacerUsual { u32 _unknown; }; // placeholder, real type unknown
struct ScrollBarDescription { u32 _unknown; }; // placeholder, real type unknown
struct SeID { u32 _unknown; }; // placeholder, real type unknown
struct TourName { u32 _unknown; }; // placeholder, real type unknown
struct nnacConfig { u32 _unknown; }; // placeholder, real type unknown
enum nnerrFatalErrType : s32; // nn/err/CTR/CTR_Api.h
struct nnfriendsFriendKey { u32 _unknown; }; // placeholder, real type unknown
struct nnfriendsMyPresence { u32 _unknown; }; // placeholder, real type unknown
struct nnfriesndsGameAuthenticationData { u32 _unknown; }; // placeholder, real type unknown
struct nnfriesndsServiceLocatorData { u32 _unknown; }; // placeholder, real type unknown
struct nngxlowInterrupt { u32 _unknown; }; // placeholder, real type unknown
struct u { u32 _unknown; }; // placeholder, real type unknown
template <auto T0, auto T1, auto T2, auto T3, auto T4> struct BankVramConfig { u32 _unknown; }; // placeholder
template <auto T0, auto T1, auto T2, typename T3> class Bank;
template <auto T0, auto T1, typename T2> class DoubleBank;
template <auto T0, auto T1, typename T2> class RoadSearchCand;
template <auto T0, auto T1> class SearchCandXZ;
template <auto T0, typename T1, typename T2, auto T3> class ExchangeBank;
template <auto T0, typename T1, typename T2, typename T3, auto T4, auto T5> class ExchangeModelBank;
template <auto T0, typename T1, typename T2, typename T3, auto T4, auto T5> class ExchangeResBank;
template <auto T0> class InOutWindowInButton;
template <auto T0> class InstCatalog;
template <auto T0> class InstSelect;
template <auto T0> class SearchCand;
template <auto T0> class SoundObj;
template <auto T0> class Tab;
template <auto T0> class mySearchCand;
template <auto T0> struct ExchangeBankKeywordFgNameEx { u32 _unknown; }; // placeholder
template <auto T0> struct ExchangeBankKeywordMyDesignFgNameEx { u32 _unknown; }; // placeholder
template <typename T0, auto T1> class BankTable;
template <typename T0, typename T1> class SvStep;
template <typename T0> class AcSimpleTalk;
template <typename T0> class BsPartsSaveMgr;
template <typename T0> class EmoticonActionEx;
template <typename T0> class NpcAmbCamModelBase;
template <typename T0> class ObjectState;
template <typename T0> class UtlBase;

namespace PlayerAction { 
    struct Name { u32 _unknown; }; // placeholder, real type unknown
}

namespace ambcam { 
    class CameraThread;
}

namespace applet { 
    class ErrEulaMgr;
}

namespace appraiser { 
    class LytAppraiserHIO;
}

namespace aquarium { 
    class AABB;
    class Capsule;
    class CisternBase;
    class CisternBox;
    class CisternBoxFan;
    class CisternCylinder;
    class Cylinder;
}

namespace asami { 
    class AcNpcSpAsamiHioNode;
}

namespace audio { namespace bgm { 
    class EvOpusBase;
    class EvOpusCarnival;
    class EvOpusCleaning;
    class EvOpusCountdown;
    class EvOpusEaster;
    class EvOpusFireworks;
    class EvOpusHalloween;
    class EvOpusHarvest;
    class EvOpusHide;
    class EvOpusNewYear;
    class EvOpusNewYearNext;
    class EvOpusXmasEve;
    class OpusBase;
    class StageOpusAblesisters;
    class StageOpusAward;
    class StageOpusBarcarolle;
    class StageOpusBase;
    class StageOpusBeauty;
    class StageOpusBirthday;
    class StageOpusCafe;
    class StageOpusCafeStaff;
    class StageOpusCamping;
    class StageOpusCeremony;
    class StageOpusChipTutorial;
    class StageOpusClub444;
    class StageOpusClub444Live;
    class StageOpusComeHomeDemo;
    class StageOpusConvenience;
    class StageOpusDepart;
    class StageOpusDream;
    class StageOpusEntryDemo;
    class StageOpusEvidence;
    class StageOpusExchange;
    class StageOpusExhibition;
    class StageOpusFortune;
    class StageOpusGallery;
    class StageOpusGrace;
    class StageOpusGrowUp;
    class StageOpusHomeCenter;
    class StageOpusImpro;
    class StageOpusLoad;
    class StageOpusLobbyIsland;
    class StageOpusMuseum;
    class StageOpusNull;
    class StageOpusOffice;
    class StageOpusPavilion;
    class StageOpusPlantDemo;
    class StageOpusPoliceBox;
    class StageOpusPostOffice;
    class StageOpusPrologue1;
    class StageOpusPrologue2;
    class StageOpusPrologue3;
    class StageOpusPrologue4;
    class StageOpusRealEstate;
    class StageOpusRecycle;
    class StageOpusResult;
    class StageOpusRetireDemo;
    class StageOpusSave;
    class StageOpusSelect;
    class StageOpusShoes;
    class StageOpusShop;
    class StageOpusShopBase;
    class StageOpusShowroom;
    class StageOpusStation;
    class StageOpusSuper;
    class StageOpusTakumiImport;
    class StageOpusTitle;
    class StageOpusTour;
    class StageOpusTourDesk;
    class StageOpusTrain;
    class StageOpusUpdatePrologue;
    class StageOpusVillageDowntown;
}}

namespace berlina { 
    class AcNpcSpBerlinaHioNode;
}

namespace bg { 
    template <typename T0, auto T1> class ExchangeBank;
}

namespace bgcheck { 
    class DynamicCylinder;
    class GuardBox;
    class MoveBg;
}

namespace bsobjchkmgr { 
    class ActorBody;
}

namespace buoymgr { 
    class Buoy;
}

namespace cafetalk { 
    class AcNpcSpCafeTalkHioNode;
}

namespace cafetalk2 { 
    class GetMasterFunction;
}

namespace cfl { 
    class MiiIcon;
    class MiiModelBase;
    class MiiModelSimple;
}

namespace collision { 
    class AABB;
    class Capsule;
    class Cylinder;
    class Shape;
    class Sphere;
    class Triangle;
    class World;
}

namespace compass { 
    class Mgr;
}

namespace demo { 
    class Order;
    class OrderList;
    class Proc;
    struct OrderID { u32 _unknown; }; // placeholder, real type unknown
}

namespace djkk { 
    class AcNpcSpDJKKHioNode;
}

namespace esc { 
    class AcNpcEscape;
    class BsEscModelBase;
    class BsEscUkiFishShadowModel;
    class BsEscUkiModel;
    class EscFollowBase;
    class EscFollowParticle;
    class EscFollowSe;
    class EscModel;
    class EscParticle;
    class EscProjectionShadowModelMgr;
    class EscTransform;
    class EscapeBgm;
    class NpcEscapeModel;
    class ParamLoader;
}

namespace escape { 
    class ActionChaiPitfallControl;
    class ActionChainBase;
    class ActionChainBeeControl;
    class ActionChainBegin;
    class ActionChainDispControl;
    class ActionChainEAct;
    class ActionChainEffect;
    class ActionChainEvWorldFunc;
    class ActionChainFishControl;
    class ActionChainGenMotion;
    class ActionChainModelFall;
    class ActionChainModelFly;
    class ActionChainSe;
    class ActionChainVAct;
    class ActionChainVActEx;
    class ActionChainVillaggerMove;
    class ActionChainVillaggerTurn;
    class ActionChainWait;
    class BreakTop;
    class BsActionChainManeger;
    class BsBeeEffTask;
    class BsBoatModel;
    class BsBuildRaftModel;
    class BsCameraManager;
    class BsCharaHexSelector;
    class BsCombineScene;
    class BsEnemyModel;
    class BsEscLayoutManager;
    class BsEscPauseLayout_BG;
    class BsEscPauseLayout_HaveCoin;
    class BsEscPauseLayout_NeedCoin;
    class BsEscPauseLayout_Terop;
    class BsEscPauseManager;
    class BsEscapeBgmManager;
    class BsEscapeBullet;
    class BsEscapeBulletManager;
    class BsEscapeEventBase;
    class BsEscapeEventBattle;
    class BsEscapeEventBattleEnd;
    class BsEscapeEventBattleStart;
    class BsEscapeEventBeforeMove;
    class BsEscapeEventCampMenu;
    class BsEscapeEventCheck;
    class BsEscapeEventCombineTime;
    class BsEscapeEventCure;
    class BsEscapeEventDayEnd;
    class BsEscapeEventDayStart;
    class BsEscapeEventDispConditions;
    class BsEscapeEventEncountEnemy;
    class BsEscapeEventFishShadow;
    class BsEscapeEventFishing;
    class BsEscapeEventForceMove;
    class BsEscapeEventGameClear;
    class BsEscapeEventGameOver;
    class BsEscapeEventGetItem;
    class BsEscapeEventMakeTool;
    class BsEscapeEventManager;
    class BsEscapeEventMove;
    class BsEscapeEventMoveDrawAttack;
    class BsEscapeEventMoveWait;
    class BsEscapeEventOpening;
    class BsEscapeEventRest;
    class BsEscapeEventSelectMoveDir;
    class BsEscapeEventSettingTrap;
    class BsEscapeEventTrap;
    class BsEscapeEventTurnEnd;
    class BsEscapeEventTurnStart;
    class BsEscapeEventUseMedecine2;
    class BsEscapeEventWait;
    class BsEscapeEventWindow;
    class BsEscapeFishingManager;
    class BsEscapeGameMgr;
    class BsEscapeLocator;
    class BsEscapeMap;
    class BsEscapeModelBoatShadow;
    class BsEscapeModelCursor;
    class BsEscapeModelEvent;
    class BsEscapeModelPickup;
    class BsEscapeModelPickupEvent;
    class BsEscapeModelPickupFlyingItem;
    class BsEscapeModelPickupKeyItem;
    class BsEscapeModelPriorityPlayer;
    class BsEscapeNetManager;
    class BsEscapeRouletteTotalManager;
    class BsEscapeScoreManager;
    class BsEscapeStage;
    class BsEscapeStageFish;
    class BsEscapeStageFishEvent;
    class BsEscapeTitleMgr;
    class BsEscapeWindowWorldManager;
    class BsEventWorld;
    class BsFreeHexCursor;
    class BsFreeHexSelector;
    class BsGamepadMenu;
    class BsPitfallModel;
    class BsPlayerBase;
    class BsPlayerManager;
    class BsPosMoverAsist;
    class EscLayout_BACK_BTN;
    class EscLayout_BG;
    class EscLayout_CHRSELECT_CHR;
    class EscLayout_CHRSELECT_CHR_BTN;
    class EscLayout_CHRSELECT_CHR_NFC;
    class EscLayout_CHRSELECT_HAVECOIN;
    class EscLayout_CHRSELECT_NEEDCOIN;
    class EscLayout_CHRSELECT_SORT;
    class EscLayout_CHRSELECT_SORTHELP;
    class EscLayout_CHRSELECT_SORTICON;
    class EscLayout_CHRSELECT_SORTPLATE;
    class EscLayout_CHRSELECT_STATUS;
    class EscLayout_CHRSELECT_STATUS_PLATE;
    class EscLayout_CMN_BG;
    class EscLayout_CMN_BTN;
    class EscLayout_Combine;
    class EscLayout_CombineBalloon;
    class EscLayout_CombineConfi;
    class EscLayout_CombineIcon;
    class EscLayout_ComboPoint;
    class EscLayout_FishingButton;
    class EscLayout_LytNfcTouch;
    class EscLayout_MaterialIcon;
    class EscLayout_MedicBalloon;
    class EscLayout_Member;
    class EscLayout_MemberDetail;
    class EscLayout_Raft;
    class EscLayout_Raft_Icon;
    class EscLayout_Raft_PartsIcon;
    class EscLayout_Roulette;
    class EscLayout_STGSELECT_DIFF;
    class EscLayout_STGSELECT_RANK;
    class EscLayout_STGSELECT_RANK_CROWN;
    class EscLayout_STGSELECT_RANK_PLATE;
    class EscLayout_STGSELECT_SCORE;
    class EscLayout_STGSELECT_STAGE;
    class EscLayout_STGSELECT_STAGE_BTN;
    class EscLayout_STGSELECT_TAB;
    class EscLayout_ScorePoint;
    class EscLayout_SearchMarker;
    class EscLayout_TAB;
    class EscLayout_TITLE_LOGO;
    class EscLayout_TITLE_TEXT;
    class EscLayout_Terop;
    class EscLayout_Tool;
    class EscLayout_ToolIcon;
    class EscLayout_ToolName;
    class EscapeFaceIcon;
    class EscapeSmokeIcon;
    class EscapeStageButterfly;
    class EscapeStageIcon;
    class EscapeStageIconManager;
    class EscapeStageObjManager;
    class EscapeStageSupport;
    class EscapeWindowWorldBase;
    class EscapeWindowWorldBattle;
    class EscapeWindowWorldBattleDrawAttack;
    class EscapeWindowWorldBattleStealth;
    class EscapeWindowWorldBattleTrap;
    class EscapeWindowWorldBeeDrawAttack;
    class EscapeWindowWorldBreakTool;
    class EscapeWindowWorldCampBase;
    class EscapeWindowWorldCampMenu;
    class EscapeWindowWorldCampOnly;
    class EscapeWindowWorldCampOpen;
    class EscapeWindowWorldCatch;
    class EscapeWindowWorldCatchWithNet;
    class EscapeWindowWorldCatchWithSkill;
    class EscapeWindowWorldCheckCamp;
    class EscapeWindowWorldCheckCancelTrap;
    class EscapeWindowWorldCheckMakeTool;
    class EscapeWindowWorldCheckSetTrap;
    class EscapeWindowWorldCheckSkill;
    class EscapeWindowWorldCure;
    class EscapeWindowWorldDayEndLonerParty;
    class EscapeWindowWorldDemeritSkill;
    class EscapeWindowWorldDoneMes;
    class EscapeWindowWorldEscape;
    class EscapeWindowWorldFishing;
    class EscapeWindowWorldFishingWithSkill;
    class EscapeWindowWorldFishingWithSkillAndRod;
    class EscapeWindowWorldFruit;
    class EscapeWindowWorldGameClear;
    class EscapeWindowWorldGameOver;
    class EscapeWindowWorldGathering;
    class EscapeWindowWorldGetFoodMap;
    class EscapeWindowWorldGetItem;
    class EscapeWindowWorldGuid01;
    class EscapeWindowWorldGuidBase;
    class EscapeWindowWorldInvocationSkill;
    class EscapeWindowWorldMakeTool;
    class EscapeWindowWorldMyTrap;
    class EscapeWindowWorldNoEventWindow;
    class EscapeWindowWorldNoFishing;
    class EscapeWindowWorldOpening;
    class EscapeWindowWorldPickupTrap;
    class EscapeWindowWorldRouletteSkill;
    class EscapeWindowWorldTrap;
    class EscapeWindowWorldTrapDrawAttack;
    class EscapeWindowWorldUseMedecine;
    class EscapeWindowWorldUseMedecineLone;
    class EscapeWindowWorldWithDice;
    class IEscLayout_Base;
    class IEscLayout_Button;
    class MapCloudModel;
    class MapFogModel;
    class MapOverFogModel;
    class StageHexCover;
    class StageHexModel;
    class StageObjModel;
}

namespace esctitle { 
    class BsEscapeTitleModel;
    class BsEscapeTitleModelBoat;
}

namespace evdmcafearbeit { 
    class mySpecialCand;
}

namespace event { 
    class EventAnniversary;
    class EventBase;
    class EventDate;
    class EventEveryDate;
    class EventMonthlyWeekday;
    class EventNpcBirthday;
    class EventPlayerBirthday;
}

namespace fgobj { 
    class DirectPlacePacket;
    class DrawList;
    class DrawListNode;
    class FieldBit;
    class ObjectBase;
    class ObjectBury;
    class ObjectCoinStone;
    class ObjectDeco;
    class ObjectDecoSwing;
    class ObjectDig;
    class ObjectExe;
    class ObjectFall;
    class ObjectFruit;
    class ObjectHanabi;
    class ObjectHoneycomb;
    class ObjectLeaf;
    class ObjectList;
    class ObjectMove;
    class ObjectOther;
    class ObjectPlant;
    class ObjectPresent;
    class ObjectScale;
    class ObjectStump;
    class ObjectStumpAnim;
    class ObjectSwing;
    class ObjectThrow;
    class ObjectThrowHole;
    class ObjectTimer;
    class Packet;
    class Proc;
    struct FindUnitForPlayerCheck { u32 _unknown; }; // placeholder, real type unknown
    struct PlaceType { u32 _unknown; }; // placeholder, real type unknown
}

namespace font { 
    class Base;
    class Mgr;
    struct FontID { u32 _unknown; }; // placeholder, real type unknown
}

namespace foruneteller { 
    class AcNpcSpFortunetellerHioNode;
}

namespace frankrin1 { 
    class AcNpcSpFrankrinHioNode;
}

namespace ftr { 
    class BaseModelResLoader;
    class HousePartsTexLoader;
    class MannequinResLoader;
    class RemakeTexLoader;
    class ResLoader;
}

namespace futa { 
    class AcNpcSpFutaHioNode;
}

namespace g3d { 
    class AmbientLight;
    class AnimPack;
    class BaseAnim;
    class BaseLight;
    class CalcRatio;
    class Camera;
    class DirectionalFragmentLight;
    class Fog;
    class FragmentLight;
    class FrameController;
    class Func;
    class FuncNode;
    class GroupSkeletalAnim;
    class GroupTransformAnim;
    class HemiSphereLight;
    class MaterialAnim;
    class MaterialAnimPack;
    class Model;
    class MorphSkeletalAnim;
    class MorphTransformAnimEvaluator;
    class PointFragmentLight;
    class ResourceLoader;
    class ResourceSet;
    class SkeletalAnim;
    class SkeletalModel;
    class SpotFragmentLight;
    class TransformNode;
    class VisibilityAnim;
}

namespace g3d { namespace modelEffect { 
    class ModelEffectContainer;
    class ModelEffectHandle;
}}

namespace gamecoin { 
    class GameCoinHIO;
}

namespace graceout { 
    class AcNpcSpGraceOutHioNode;
}

namespace graphic { 
    class CurrentSetter;
    class MemoryMgr;
}

namespace gui { 
    class ButtonBase;
    class ButtonEx2;
    class ButtonEx;
    class CellPhoneKeySet;
    class EditArea;
    class EditAreaEx;
    class GridKeySet;
    class InputButton;
    class KanaKeySet;
    class Key;
    class KeySet;
    class MulKey;
    class NrmKey;
    class QwertyKeySet;
    class RadioButton;
    class RadioButtonEx;
    class RepeatButton;
    class RepeatButtonEx2;
    class RepeatButtonEx;
    class RepeatInputButton;
    class TchOnButton;
    class TchOnButtonSp;
    class TextArea;
    class TextAreaBoard;
    class TextAreaEx;
    class TextAreaMail;
    class TglButton;
    class TglKey;
    class Widget;
}

namespace hobj { 
    class HouseBase;
    class MyHouse;
    class MyHouseMax;
    class Tent;
}

namespace honma { 
    class AcNpcSpHonmaHioNode;
    class GetTanukichiFunction;
}

namespace imgdb { 
    class ArchiveMounter;
    class Crc16;
    class Crc16Context;
    class CtrNandArchiveMounter;
    class DateTimeSeconds;
    class Directory;
    class FaceInfo;
    class FileReader;
    class FileWriter;
    class ImageCollectionTable;
    class ImageCollectionTableRecord;
    class ImageCollector;
    class ImageDatabase;
    class ImageDbImageValidatorBasic;
    class ImageDbRecord;
    class ImageDbRecordLink;
    class ImageDbRecordParam;
    class ImageDbRestore;
    class ImageFileWriter;
    class ImageInfo;
    class ImageSearcher;
    class IndexInfo;
    class JpegDecoder;
    class JpegEncoder;
    class JpegInMJpegFileReader;
    class JpegMpBaseDecoder;
    class JpegMpBaseEncoder;
    class JpegMpBaseSaver;
    class JpegSaver;
    class LegacyTable;
    class LegacyTableHeader;
    class LegacyTableRecord;
    class MpDecoder;
    class MpEncoder;
    class MpSaver;
    class PhotoFileReader;
    class PhotoHeaderFileReader;
    class PhotoHeaderImageInfoReader;
    class PhotoImageInfoReader;
    class PictureDatabase;
    class PictureDbRecord;
    class PictureTable;
    class PictureTableHeader;
    class PictureTableRecord;
    class SdArchiveMounter;
    class SysMakerNote;
    class TwlNandArchiveMounter;
    class ValidityStateTable;
    struct Allocator { u32 _unknown; }; // placeholder, real type unknown
    struct BodyIdType { u32 _unknown; }; // placeholder, real type unknown
    struct DistinctionTypeBit { u32 _unknown; }; // placeholder, real type unknown
    struct HandleTypeBit { u32 _unknown; }; // placeholder, real type unknown
    struct ImageKind { u32 _unknown; }; // placeholder, real type unknown
    struct ImageKindBit { u32 _unknown; }; // placeholder, real type unknown
    struct Result { u32 _unknown; }; // placeholder, real type unknown
    struct SaveProcessType { u32 _unknown; }; // placeholder, real type unknown
    struct ShootingType { u32 _unknown; }; // placeholder, real type unknown
    struct StorageType { u32 _unknown; }; // placeholder, real type unknown
    template <typename T0, typename T1> struct DynamicArray { u32 _unknown; }; // placeholder
    template <typename T0> class Singleton;
    template <typename T0> struct XAllocator { u32 _unknown; }; // placeholder
}

namespace imgdb { namespace util { 
    struct FileIndexInfo { u32 _unknown; }; // placeholder, real type unknown
}}

namespace indoor { 
    template <typename T0, auto T1> class ExchangeBank;
}

namespace isfactory { 
    class BalloonBuilder;
    class BasicBuilder;
    class BorrowingRaceBuilder;
    class DivingBuilder;
    class FishingBuilder;
    class FishingIslandBuilder;
    class FlowerPickingBuilder;
    class FossilDigBuilder;
    class HideAndSeekBuilder;
    class InsectCatchingButterflyBuilder;
    class InsectCatchingDefaultBuilder;
    class InsectCatchingReduceTreeBuilder;
    class InsectCatchingRiseStoneBuilder;
    class InsectCatchingRiseTreeBuilder;
    class IslandBuilder;
    class MazeBuilder;
    class MiningBuilder;
    class PeninsulaBuilder;
    class PikohanBuilder;
    class TreasureHuntBuilder;
    class VillageBuilder;
}

namespace ishutfact2 { 
    class BasicBuilder;
    class BorrowingBuilder;
    class FossilTemplateBuilder;
}

namespace item { 
    class ResLoader;
    struct FishID { u32 _unknown; }; // placeholder, real type unknown
    struct InsectID { u32 _unknown; }; // placeholder, real type unknown
}

namespace jonny { 
    class AcNpcSpJonnyHioNode;
}

namespace kameyama { 
    class AcNpcSpKameyamaHioNode;
}

namespace kodanuki { 
    class AcNpcSpShopKodanukiHioNode;
    class GetRecycleFunction;
}

namespace ktbkannounce { 
    class AcNpcSpKotobukiAnnounceHioNode;
}

namespace ktbkballon { 
    class AcNpcSpKotobukiBalloonHioNode;
}

namespace ktbkbase { 
    class AcNpcSpKotobukiBaseHioNode;
}

namespace ktbkborrowrace { 
    class AcNpcSpKotobukiBorrowingRaceHioNode;
}

namespace ktbkdiving { 
    class AcNpcSpKotobukiDivingHioNode;
}

namespace ktbkfish { 
    class AcNpcSpKotobukiFishHioNode;
}

namespace ktbkflower { 
    class AcNpcSpKotobukiFlowerHioNode;
}

namespace ktbkfossildig { 
    class AcNpcSpKotobukiFossilDigHioNode;
}

namespace ktbkhideseek { 
    class AcNpcSpKotobukiHideAndSeekHioNode;
}

namespace ktbkins { 
    class AcNpcSpKotobukiInsectHioNode;
}

namespace ktbkmaze { 
    class AcNpcSpKotobukiMazeHioNode;
}

namespace ktbkmin { 
    class AcNpcSpKotobukiMiningHioNode;
}

namespace ktbkpikohan { 
    class AcNpcSpKotobukiPikoHanHioNode;
}

namespace ktbktrhnt { 
    class AcNpcSpKotobukiTreasureHuntHioNode;
}

namespace libms { 
    class FlowNode;
    class FlowNodeBranch;
    class FlowNodeEntry;
    class FlowNodeEvent;
    class FlowNodeJump;
    class FlowNodeMessage;
}

namespace lyt { 
    class AnmObject;
    class Object;
    class ObjectEx;
    class TagProcessor;
    class TextArea;
    class TextAreaBoard;
    class TextAreaEx;
    class TextAreaMail;
    class TextAreaOneLine;
    class TextAreaOneLineBirth;
    class TextAreaOneLineChat;
    class TextAreaOneLineComTitle;
    class TextAreaOneLineComment;
    class TextAreaOneLineFriend;
    class TextAreaTagProcessor;
    class TourList;
}

namespace maiko { 
    class AcNpcSpMaikoHioNode;
}

namespace math { 
    class Angle16;
    template <typename T0, auto T1> struct Vector { u32 _unknown; }; // placeholder
}

namespace menubg { 
    class MenuBgHostIO;
}

namespace menuexch { 
    class MenuTicketExchangeIO;
}

namespace menumelody { 
    class MelodyHostIO;
}

namespace minigame0 { 
    class BaseLayout;
    class BsMiniGame0Bg;
    class CstmCamera;
    class CstmReceptor;
    class EscapeResult;
    class EscapeResultDramaBase;
    class EscapeResultDrama_FailureByFoodShortage;
    class EscapeResultDrama_FailureByTimeup;
    class EscapeResultDrama_Success;
    class LayoutBg;
    class LayoutCmnBtn;
    class LayoutCrown;
    class LayoutCurrentScoreBoard;
    class LayoutFaceIcon;
    class LayoutHaveCoin;
    class LayoutNeedCoin;
    class LayoutRankBoard;
    class LayoutRankingPos;
    class LayoutResultInfo;
    class LayoutScore;
    class LayoutTelopBase;
    class LayoutTelopFailure;
    class LayoutTelopRankingUpdate;
    class LayoutTelopSuccess;
}

namespace mutil { 
    class LoaderWrapper;
    template <typename T0, auto T1> class BaseResourceMgr;
}

namespace mw { namespace pfid { namespace pfid { 
    struct t_FaceDetect { u32 _unknown; }; // placeholder, real type unknown
    struct t_FacePosition { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace mw { namespace qrd { 
    class QRDecoder;
}}

namespace mw { namespace qre { 
    class QREncoder;
}}

namespace mysterycat { 
    class AcNpcSpMysteryCatHioNode;
}

namespace net { 
    class Net;
    class ScanBufBase;
    class SearchDataResult;
    class Sender;
    class SystemCallback;
}

namespace net { namespace nex { 
    class Error;
    class Framework;
    class Impl;
}}

namespace net { namespace nex { namespace Command { 
    struct ID { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace netgame { 
    class AvatarMgr;
    class ConfirmMachine;
    class FriendListMgr;
    class IslandMgr;
    class JoinMgr;
    class LeaveMgr;
    class LockMgr;
    class MachineBitTable;
    class MatchingList;
    class NetGameMgr;
    class PacketQueue;
    class StageMgr;
    struct AvatarId { u32 _unknown; }; // placeholder, real type unknown
    struct PlayerNo { u32 _unknown; }; // placeholder, real type unknown
    struct Type { u32 _unknown; }; // placeholder, real type unknown
}

namespace netgame { namespace GateMgr { 
    struct Flag { u32 _unknown; }; // placeholder, real type unknown
}}

namespace nfp { 
    class Framework;
    class Thread;
}

namespace nmlasspacket { 
    class SearchNpcCB;
}

namespace nn { namespace CTR { 
    struct SystemMenuData { u32 _unknown; }; // placeholder, real type unknown
}}

namespace nn { namespace ac { namespace CTR { 
    struct ApType { u32 _unknown; }; // placeholder, real type unknown
    struct InfraPriority { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace ac { namespace CTR { namespace detail { 
    class Ac;
}}}}

namespace nn { namespace applet { namespace CTR { 
    class SysSleepAcceptedCallbackInfo;
    struct AppJumpType { u32 _unknown; }; // placeholder, real type unknown
    struct AppletDisplayInfo { u32 _unknown; }; // placeholder, real type unknown
    struct AppletPos { u32 _unknown; }; // placeholder, real type unknown
    enum ApplicationRunningMode : u8; // nn/applet/CTR/applet_Types.h
    struct CaptureBufferInfo { u32 _unknown; }; // placeholder, real type unknown
    struct HomeButtonState { u32 _unknown; }; // placeholder, real type unknown
    struct QueryReply { u32 _unknown; }; // placeholder, real type unknown
    struct SleepNotificationState { u32 _unknown; }; // placeholder, real type unknown
    struct TransitionType { u32 _unknown; }; // placeholder, real type unknown
    struct WakeupState { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace applet { namespace CTR { namespace detail { 
    class APPLET;
    struct OffsetTable { u32 _unknown; }; // placeholder, real type unknown
}}}}

namespace nn { namespace boss { 
    class DataStoreDownloadAction;
    class FgOnlyTask;
    class NsData;
    class NsDataIdList;
    class NsaDownloadAction;
    class Task;
    class TaskAction;
    class TaskActionBase;
    class TaskPolicy;
    class TaskStatus;
    struct HeaderInfoType { u32 _unknown; }; // placeholder, real type unknown
    struct PropertyType { u32 _unknown; }; // placeholder, real type unknown
    struct ResultCode { u32 _unknown; }; // placeholder, real type unknown
    struct StorageType { u32 _unknown; }; // placeholder, real type unknown
    struct TaskActionConfig { u32 _unknown; }; // placeholder, real type unknown
    struct TaskOption { u32 _unknown; }; // placeholder, real type unknown
    struct TaskOptionConfig { u32 _unknown; }; // placeholder, real type unknown
    struct TaskPolicyConfig { u32 _unknown; }; // placeholder, real type unknown
    struct TaskResultCode { u32 _unknown; }; // placeholder, real type unknown
    struct TaskServiceStatus { u32 _unknown; }; // placeholder, real type unknown
    struct TaskStatusInfo { u32 _unknown; }; // placeholder, real type unknown
}}

namespace nn { namespace boss { namespace detail { 
    class IpcManager;
    class Privileged;
    class User;
}}}

namespace nn { namespace camera { namespace CTR { 
    struct CameraSelect { u32 _unknown; }; // placeholder, real type unknown
    struct Context { u32 _unknown; }; // placeholder, real type unknown
    struct Flip { u32 _unknown; }; // placeholder, real type unknown
    struct PackageParameterContextDetail { u32 _unknown; }; // placeholder, real type unknown
    struct Port { u32 _unknown; }; // placeholder, real type unknown
    struct ShutterSoundType { u32 _unknown; }; // placeholder, real type unknown
    struct StereoCameraCalibrationData { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace camera { namespace CTR { namespace detail { 
    class Camera;
}}}}

namespace nn { namespace cec { namespace CTR { 
    class CecControl;
    class CecControlSys;
    class Message;
    class MessageBox;
    class MessageId;
    struct CecBoxInfoHeader { u32 _unknown; }; // placeholder, real type unknown
    struct CecBoxType { u32 _unknown; }; // placeholder, real type unknown
    struct CecMessageHeader { u32 _unknown; }; // placeholder, real type unknown
    struct MessageBoxInfo { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace cfg { namespace CTR { 
    struct CfgCountryCode { u32 _unknown; }; // placeholder, real type unknown
    struct CfgLanguageCode { u32 _unknown; }; // placeholder, real type unknown
    struct CfgRegionCode { u32 _unknown; }; // placeholder, real type unknown
    struct SimpleAddressId { u32 _unknown; }; // placeholder, real type unknown
    struct UserName; // nn/cfg/CTR/cfg_Types.h
}}}

namespace nn { namespace cfg { namespace CTR { namespace detail { 
    class IpcUser;
    struct _IPCPortType { u32 _unknown; }; // placeholder, real type unknown
}}}}

namespace nn { namespace crypto { 
    class AuthenticatedDecryptor;
    class AuthenticatedEncryptor;
    class BlockCipher;
    class CcmDecryptor;
    class CcmEncryptor;
    class CipherMode;
    class HashContextBase;
    class Md5Context;
    class Sha1Context;
    class Sha256Context;
    class ShaBlock512BitContext;
    template <auto T0> class Aes;
}}

namespace nn { namespace crypto { namespace detail { 
    class CcmMode;
}}}

namespace nn { namespace dbm { 
    class RomPathTool;
}}

namespace nn { namespace dsp { namespace CTR { 
    class DSP;
}}}

namespace nn { namespace enc { 
    struct BreakType { u32 _unknown; }; // placeholder, real type unknown
}}

namespace nn { namespace err { namespace CTR { 
    class FatalErr;
    struct FatalErrInfo { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace erreula { namespace CTR { 
    struct Parameter { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace fnd { 
    class DateTime;
    class ExpHeapBase;
    class HeapBase;
    class IAllocator;
    class TimeSpan;
    class UnitHeapBase;
    struct DateTimeParameters { u32 _unknown; }; // placeholder, real type unknown
    template <typename T0, typename T1> class IntrusiveLinkedList;
    template <typename T0> class ExpHeapTemplate;
    template <typename T0> class UnitHeapTemplate;
}}

namespace nn { namespace fnd { namespace detail { 
    struct ExpHeapImpl { u32 _unknown; }; // placeholder, real type unknown
    struct NNSFndList { u32 _unknown; }; // placeholder, real type unknown
    struct NNSiFndExpHeapHead { u32 _unknown; }; // placeholder, real type unknown
    struct NNSiFndExpHeapMBlockHead { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace friends { namespace CTR { namespace detail { 
    class Friends;
}}}}

namespace nn { namespace fs { 
    class Directory;
    class ExtSaveDataSpecifier;
    class FileInputStream;
    class FileOutputStream;
    class FileStream;
    class IInputStream;
    class IOutputStream;
    class IPositionable;
    class IStream;
    struct ArchiveResource { u32 _unknown; }; // placeholder, real type unknown
    struct Attributes { u32 _unknown; }; // placeholder, real type unknown
    struct DirectoryEntry { u32 _unknown; }; // placeholder, real type unknown
    struct MediaType { u32 _unknown; }; // placeholder, real type unknown
    struct PositionBase { u32 _unknown; }; // placeholder, real type unknown
    struct SystemMediaType { u32 _unknown; }; // placeholder, real type unknown
    struct Transaction { u32 _unknown; }; // placeholder, real type unknown
    struct WriteOption { u32 _unknown; }; // placeholder, real type unknown
}}

namespace nn { namespace fs { namespace CTR { 
    struct DataContentArchivePath { u32 _unknown; }; // placeholder, real type unknown
    struct ProgramDataPath { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace fs { namespace CTR { namespace MPCore { namespace detail { 
    class ArchiveTableEntry;
    class ContentRomFsArchive;
    class FileServerArchive;
    class IArchive;
    class IDirectory;
    class IFile;
    class RomFsArchive;
    class UserFileSystem;
}}}}}

namespace nn { namespace fs { namespace detail { 
    class DirectoryBase;
    class DirectoryBaseImpl;
    class FileBase;
    class FileBaseImpl;
}}}

namespace nn { namespace fs { namespace ipc { 
    class Directory;
    class File;
    class FileSystem;
}}}

namespace nn { namespace fslow { 
    template <typename T0, typename T1> struct LowPath { u32 _unknown; }; // placeholder
}}

namespace nn { namespace gr { namespace CTR { 
    class BindSymbolVSBool;
    class BindSymbolVSFloat;
    class Combiner;
    class CommandBufferJumpHelper;
    class FragmentLight;
    class FrameBuffer;
    class RenderState;
    class Scissor;
    class Shader;
    class Texture;
    class Vertex;
    class Viewport;
    struct BindSymbol { u32 _unknown; }; // placeholder, real type unknown
    struct BindSymbolVSInput { u32 _unknown; }; // placeholder, real type unknown
    struct CommandBufferChannel { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace gxlow { namespace CTR { 
    class CmdReqQueueTx;
    class Gpu;
    class InterruptReceiver;
    class InterruptRelayQueueRx;
    struct DisplayCaptureInfo { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace gxlow { namespace CTR { namespace detail { 
    struct CmdReq { u32 _unknown; }; // placeholder, real type unknown
}}}}

namespace nn { namespace hid { namespace CTR { 
    class AccelerometerReader;
    class AnalogStickClamper;
    class ExtraPad;
    class Gyroscope;
    class GyroscopeReader;
    class HidBase;
    class HidDevices;
    class PadReader;
    class TouchPanelReader;
    struct AccelerationFloat { u32 _unknown; }; // placeholder, real type unknown
    struct Accelerometer { u32 _unknown; }; // placeholder, real type unknown
    struct AccelerometerStatus { u32 _unknown; }; // placeholder, real type unknown
    struct GyroscopeLowStatus { u32 _unknown; }; // placeholder, real type unknown
    struct GyroscopeStatus { u32 _unknown; }; // placeholder, real type unknown
    struct Pad { u32 _unknown; }; // placeholder, real type unknown
    struct PadStatus { u32 _unknown; }; // placeholder, real type unknown
    struct TouchPanelStatus { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace hid { namespace CTR { namespace detail { 
    class Ipc;
}}}}

namespace nn { namespace hidlow { namespace CTR { 
    class AccelerometerLifoRing;
    class GyroscopeLowLifoRing;
    class PadLifoRing;
    class TouchPanelLifoRing;
}}}

namespace nn { namespace http { 
    class Connection;
    class ConnectionIpc;
    struct PostDataType { u32 _unknown; }; // placeholder, real type unknown
    struct RequestMethod { u32 _unknown; }; // placeholder, real type unknown
}}

namespace nn { namespace http { namespace detail { 
    class LibManager;
}}}

namespace nn { namespace jpeg { namespace CTR { 
    class JpegMpDecoder;
    class JpegMpEncoder;
    struct GpsData { u32 _unknown; }; // placeholder, real type unknown
    struct JpegMpDecoderContext { u32 _unknown; }; // placeholder, real type unknown
    struct JpegMpDecoderExifTagITN { u32 _unknown; }; // placeholder, real type unknown
    struct JpegMpEncoderComponentStructure { u32 _unknown; }; // placeholder, real type unknown
    struct JpegMpEncoderContext { u32 _unknown; }; // placeholder, real type unknown
    struct JpegMpEncoderIfdWorkObj { u32 _unknown; }; // placeholder, real type unknown
    struct JpegTagWorkObj { u32 _unknown; }; // placeholder, real type unknown
    struct MpEntry { u32 _unknown; }; // placeholder, real type unknown
    struct MpIndex { u32 _unknown; }; // placeholder, real type unknown
    struct MpRegionsToBuildJpegData { u32 _unknown; }; // placeholder, real type unknown
    struct PixelFormat { u32 _unknown; }; // placeholder, real type unknown
    struct PixelSampling { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace jpeg { namespace CTR { namespace detail { 
    struct App1PointerAndSize { u32 _unknown; }; // placeholder, real type unknown
    struct JpegMpDecoderTemporarySettingObj { u32 _unknown; }; // placeholder, real type unknown
    struct JpegMpEncoderTemporarySettingObj { u32 _unknown; }; // placeholder, real type unknown
    struct JpegMpEncoderWorkObj { u32 _unknown; }; // placeholder, real type unknown
}}}}

namespace nn { namespace math { 
    class MTX34;
    class MTX44;
    struct MTX33 { u32 _unknown; }; // placeholder, real type unknown
    enum PivotDirection : u8; // nn/math/math_MTX44.h
    struct QUAT { u32 _unknown; }; // placeholder, real type unknown
    struct Transform3 { u32 _unknown; }; // placeholder, real type unknown
    struct VEC2 { u32 _unknown; }; // placeholder, real type unknown
    struct VEC3; // nn/math/math_Vector3.h
    struct VEC3_ { u32 _unknown; }; // placeholder, real type unknown
    struct VEC4 { u32 _unknown; }; // placeholder, real type unknown
    template <typename T0, auto T1> struct Vector { u32 _unknown; }; // placeholder
}}

namespace nn { namespace mic { namespace CTR { 
    struct SamplingRate { u32 _unknown; }; // placeholder, real type unknown
    struct SamplingType { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace mic { namespace CTR { namespace detail { 
    class Mic;
}}}}

namespace nn { namespace ndm { namespace CTR { 
    struct DaemonName { u32 _unknown; }; // placeholder, real type unknown
    struct ExclusiveMode { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace ndm { namespace CTR { namespace detail { 
    class Interface;
}}}}

namespace nn { namespace nex { 
    class AccountManagementProtocolClient;
    class AccountManagementServiceDDLDeclarations;
    class AnyDataAdapter;
    class AnyGatheringAdapter;
    class AuthenticationClient;
    class AuthenticationFoundationDDLDeclarations;
    class AuthenticationInfo;
    class AutoMatchmakeParam;
    class BackEndServices;
    class BandwidthCounter;
    class BandwidthCounterMap;
    class BerkeleySocketDriver;
    class BinaryMessage;
    class BitStream;
    class Buffer;
    class ByteStream;
    class CTRHttpConnection;
    class CacheManager;
    class CallContext;
    class CallContextRegister;
    class CallPolicy;
    class CallProtocolMethodOperation;
    class CallbackRoot;
    class ChecksumAlgorithm;
    class Chrono;
    class ClientProtocol;
    class ClientProtocolRequestBroker;
    class ClientStreamManager;
    class ComponentState;
    class CompressionAlgorithm;
    class CompressionPlugin;
    class Condition;
    class ConnectionData;
    class ConnectionManager;
    class ConnectionOrientedStream;
    class ConnectivityManager;
    class Core;
    class CreateMatchmakeSessionParam;
    class Credentials;
    class CriticalSection;
    class DDLDeclarations;
    class Data;
    class DataStoreChangeMetaCompareParam;
    class DataStoreChangeMetaParam;
    class DataStoreChangeMetaParamV1;
    class DataStoreClientInterface;
    class DataStoreCompletePostParam;
    class DataStoreCompletePostParamV1;
    class DataStoreCompleteUpdateParam;
    class DataStoreDDLDeclarations;
    class DataStoreDeleteParam;
    class DataStoreFileServerClient;
    class DataStoreGetMetaParam;
    class DataStoreGetNewArrivedNotificationsParam;
    class DataStoreGetNotificationUrlParam;
    class DataStoreGetSpecificMetaParam;
    class DataStoreGetSpecificMetaParamV1;
    class DataStoreKeyValue;
    class DataStoreLogicServerClient;
    class DataStoreLogicServerClientInterface;
    class DataStoreMetaInfo;
    class DataStoreNotification;
    class DataStoreNotificationV1;
    class DataStorePasswordInfo;
    class DataStorePermission;
    class DataStorePersistenceInfo;
    class DataStorePersistenceInitParam;
    class DataStorePersistenceTarget;
    class DataStorePostHttpEventListener;
    class DataStorePrepareGetParam;
    class DataStorePrepareGetParamV1;
    class DataStorePreparePostParam;
    class DataStorePreparePostParamV1;
    class DataStorePrepareUpdateParam;
    class DataStoreProtocolClient;
    class DataStoreRateObjectParam;
    class DataStoreRatingInfo;
    class DataStoreRatingInfoWithSlot;
    class DataStoreRatingInitParam;
    class DataStoreRatingInitParamWithSlot;
    class DataStoreRatingLog;
    class DataStoreRatingTarget;
    class DataStoreReqGetAdditionalMeta;
    class DataStoreReqGetInfo;
    class DataStoreReqGetInfoV1;
    class DataStoreReqGetNotificationUrlInfo;
    class DataStoreReqPostInfo;
    class DataStoreReqPostInfoV1;
    class DataStoreReqUpdateInfo;
    class DataStoreSearchParam;
    class DataStoreSearchResult;
    class DataStoreSpecificMetaInfo;
    class DataStoreSpecificMetaInfoV1;
    class DateTime;
    class DynamicGathering;
    class EmulationDevice;
    class EncryptionAlgorithm;
    class EndPoint;
    class EndPointEventHandler;
    class EndPointInfoInterface;
    class ErrorDescriptionTable;
    class Event;
    class EventHandler;
    class EventLog;
    class ForcedCriticalSection;
    class GameSession;
    class Gathering;
    class GlobalNotificationEventManager;
    class GlobalVariables;
    class HMACChecksum;
    class HealthServiceDDLDeclarations;
    class HighLevelStream;
    class HighResolutionClock;
    class HttpClient;
    class HttpConnection;
    class HttpEventListener;
    class IORequestContext;
    class IndependentServer;
    class InetAddress;
    class InputEmulationDevice;
    class InstanceControl;
    class InstanceTable;
    class InstantiationContext;
    class InterfaceInfo;
    class Job;
    class JobBackEndServicesLogin;
    class JobBackEndServicesLoginWithData;
    class JobBackEndServicesLogout;
    class JobBackEndServicesTerminate;
    class JobCTRLogin;
    class JobCallContextCallback;
    class JobCheckAndRelay;
    class JobConnectEndPoint;
    class JobConnectSecureEndPoint;
    class JobCreateAccount;
    class JobDataStoreCheckNotification;
    class JobDataStoreFileServer;
    class JobDataStoreGetRating;
    class JobDataStoreHpp;
    class JobDataStoreUpdateObject;
    class JobDeriveKey;
    class JobHttp;
    class JobIndependentServer;
    class JobLogin;
    class JobLoginOrCreateAccount;
    class JobManageAccount;
    class JobNNIDLogin;
    class JobNameResolve;
    class JobProcessProtocolEvent;
    class JobProcessProtocolMessage;
    class JobResolveRelayInfo;
    class JobRetrieveGameAuthToken;
    class JobStartNATSession;
    class JobTerminate;
    class JobTicketManagerAcquireTicket;
    class JobTicketManagerLogin;
    class JobTicketManagerLoginWithData;
    class JobWaitForNotification;
    class JoinMatchmakeSessionParam;
    class KerberosAuthentication;
    class KerberosEncryption;
    class Key;
    class KeyCache;
    class KeyDerivation;
    class KeyedChecksumAlgorithm;
    class LocalClock;
    class LockChecker;
    class Log;
    class LogDevice;
    class LogDeviceConsole;
    class MD5Checksum;
    class MD5ChecksumWithKey;
    class MD5KeyDerivation;
    class MatchMakingClient;
    class MatchMakingProtocolClient;
    class MatchMakingProtocolExtClient;
    class MatchMakingServiceDDLDeclarations;
    class MatchmakeExtensionClient;
    class MatchmakeExtensionDDLDeclarations;
    class MatchmakeExtensionProtocolClient;
    class MatchmakeParam;
    class MatchmakeSession;
    class MatchmakeSessionSearchCriteria;
    class MemoryManager;
    class Message;
    class MessageDeliveryProtocolClient;
    class MessageDeliveryServer;
    class MessageRecipient;
    class MessagingClient;
    class MessagingNotificationHandler;
    class MessagingProtocolClient;
    class MessagingServiceDDLDeclarations;
    class MutexPrimitive;
    class MyNotificationEventHandler;
    class NATProperties;
    class NATRelayInterface;
    class NATTraversalDDLDeclarations;
    class NATTraversalEngine;
    class NATTraversalProtocolClient;
    class NATTraversalRelayClient;
    class NATTraversalRelayProtocol;
    class NATTraversalRelayProtocolImpl;
    class NATTraversalReportInternalProtocolClient;
    class NATTraversalStream;
    class Network;
    class NetworkEmulator;
    class NgsBridgeImp;
    class NgsBridgeInterface;
    class NgsFacade;
    class NintendoAuthenticationDDLDeclarations;
    class NintendoNotificationEvent;
    class NintendoNotificationEventManager;
    class NintendoProtocolFoundationDDLDeclarations;
    class NonCopyable;
    class NotificationEvent;
    class NotificationEventFilterInterface;
    class NotificationEventHandler;
    class NotificationEventManager;
    class ObjectThreadRoot;
    class OnlineCoreDDLDeclarations;
    class Operation;
    class OperationManager;
    class OutputEmulationDevice;
    class OutputFormat;
    class PRUDPEndPoint;
    class PRUDPEndPointMap;
    class PRUDPMessageInterface;
    class PRUDPMessageSelector;
    class PRUDPMessageV0;
    class PRUDPMessageV1;
    class PRUDPStream;
    class Pacer;
    class Packet;
    class PacketBuffer;
    class PacketBufferManager;
    class PacketBufferPacketIn;
    class PacketBufferPacketOut;
    class PacketDispatchQueue;
    class PacketEncDec;
    class PacketIn;
    class PacketOut;
    class PacketQueue;
    class PeriodicJob;
    class PersistentGathering;
    class Platform;
    class PlayingSession;
    class Plugin;
    class PluginObject;
    class PollForCompletionJob;
    class ProfilingUnit;
    class ProtectedPacketQueue;
    class Protocol;
    class ProtocolCallContext;
    class ProtocolFoundationDDLDeclarations;
    class ProtocolRegistry;
    class ProtocolRequestBrokerInterface;
    class PseudoGlobalVariableList;
    class PseudoGlobalVariableRoot;
    class PseudoSingleton;
    class QueuingSocket;
    class QueuingSocketTransportBuffer;
    class RC4Encryption;
    class RTT;
    class RVClientCore;
    class RVConnectionData;
    class RandomNumberGenerator;
    class RefCountedObject;
    class Relay;
    class RelayMessage;
    class RelayStream;
    class RendezVous;
    class RendezVousLoginOperation;
    class RendezVousLogoutOperation;
    class RendezVousOperation;
    class ResultRange;
    class RetrieveGameAuthTokenClient;
    class RootCaReader;
    class RootObject;
    class RootTransport;
    class Router;
    class RoutingStream;
    class RoutingTable;
    class STLExtDDLDeclarations;
    class Scheduler;
    class SecureConnectionClient;
    class SecureConnectionProtocolClient;
    class SecureConnectionServiceDDLDeclarations;
    class SecureEndPoint;
    class SecureStream;
    class SecurityContext;
    class SecurityContextManager;
    class ServerProtocol;
    class ServiceClient;
    class SingleThreadCallPolicy;
    class SlidingWindow;
    class Socket;
    class SocketDriver;
    class SocketTransport;
    class SpinTest;
    class StateMachine;
    class StationContactInfo;
    class StationProbe;
    class StationURL;
    class StepSequenceJob;
    class Stream;
    class StreamBundling;
    class StreamManager;
    class StreamSettings;
    class StreamTable;
    class String;
    class StringConverter;
    class StringStream;
    class SystemComponent;
    class SystemComponentGroup;
    class SystemComponents;
    class SystemSetting;
    class TextMessage;
    class ThreadVariableList;
    class ThreadVariableRoot;
    class Ticket;
    class TicketGrantingProtocolClient;
    class TicketManager;
    class Time;
    class Timeout;
    class TimeoutManager;
    class TraceLog;
    class TransportAdapter;
    class TransportBufferMultiThread;
    class TransportBufferSet;
    class TransportBufferThread;
    class TransportBufferThreadInterface;
    class TransportEventHandler;
    class TransportSignatureGenerator;
    class TransportStreamManager;
    class UDPTransport;
    class UPnPProperties;
    class URLProbe;
    class URLProbeList;
    class UdsHandle;
    class UpdateMatchmakeSessionParam;
    class UserMessage;
    class UtilityClient;
    class UtilityDDLDeclarations;
    class UtilityProtocolClient;
    class Variant;
    class VirtualFilterDevice;
    class WorkerThreads;
    class ZLibCompression;
    class ZLibPlugin;
    class _DDL_AuthenticationInfo;
    class _DDL_AutoMatchmakeParam;
    class _DDL_BinaryMessage;
    class _DDL_ConnectionData;
    class _DDL_CreateMatchmakeSessionParam;
    class _DDL_Data;
    class _DDL_DataStoreChangeMetaCompareParam;
    class _DDL_DataStoreChangeMetaParam;
    class _DDL_DataStoreChangeMetaParamV1;
    class _DDL_DataStoreCompletePostParam;
    class _DDL_DataStoreCompletePostParamV1;
    class _DDL_DataStoreCompleteUpdateParam;
    class _DDL_DataStoreDeleteParam;
    class _DDL_DataStoreGetMetaParam;
    class _DDL_DataStoreGetNewArrivedNotificationsParam;
    class _DDL_DataStoreGetNotificationUrlParam;
    class _DDL_DataStoreGetSpecificMetaParam;
    class _DDL_DataStoreGetSpecificMetaParamV1;
    class _DDL_DataStoreKeyValue;
    class _DDL_DataStoreMetaInfo;
    class _DDL_DataStoreNotification;
    class _DDL_DataStoreNotificationV1;
    class _DDL_DataStorePasswordInfo;
    class _DDL_DataStorePermission;
    class _DDL_DataStorePersistenceInfo;
    class _DDL_DataStorePersistenceInitParam;
    class _DDL_DataStorePersistenceTarget;
    class _DDL_DataStorePrepareGetParam;
    class _DDL_DataStorePrepareGetParamV1;
    class _DDL_DataStorePreparePostParam;
    class _DDL_DataStorePreparePostParamV1;
    class _DDL_DataStorePrepareUpdateParam;
    class _DDL_DataStoreRateObjectParam;
    class _DDL_DataStoreRatingInfo;
    class _DDL_DataStoreRatingInfoWithSlot;
    class _DDL_DataStoreRatingInitParam;
    class _DDL_DataStoreRatingInitParamWithSlot;
    class _DDL_DataStoreRatingLog;
    class _DDL_DataStoreRatingTarget;
    class _DDL_DataStoreReqGetAdditionalMeta;
    class _DDL_DataStoreReqGetInfo;
    class _DDL_DataStoreReqGetInfoV1;
    class _DDL_DataStoreReqGetNotificationUrlInfo;
    class _DDL_DataStoreReqPostInfo;
    class _DDL_DataStoreReqPostInfoV1;
    class _DDL_DataStoreReqUpdateInfo;
    class _DDL_DataStoreSearchParam;
    class _DDL_DataStoreSearchResult;
    class _DDL_DataStoreSpecificMetaInfo;
    class _DDL_DataStoreSpecificMetaInfoV1;
    class _DDL_DynamicGathering;
    class _DDL_GameSession;
    class _DDL_Gathering;
    class _DDL_JoinMatchmakeSessionParam;
    class _DDL_MatchmakeParam;
    class _DDL_MatchmakeSession;
    class _DDL_MatchmakeSessionSearchCriteria;
    class _DDL_MessageRecipient;
    class _DDL_NintendoNotificationEvent;
    class _DDL_NotificationEvent;
    class _DDL_PersistentGathering;
    class _DDL_PlayingSession;
    class _DDL_RVConnectionData;
    class _DDL_ResultRange;
    class _DDL_TextMessage;
    class _DDL_UpdateMatchmakeSessionParam;
    class _DDL_UserMessage;
    class _Proto_MessageDeliveryProtocolServer;
    class _Proto_NATTraversalProtocolServer;
    class _Proto_NintendoNotificationEventProtocolServer;
    class _Proto_NotificationProtocolServer;
    class qBuffer;
    class qResult;
    struct DataStorePostObjectEventListener { u32 _unknown; }; // placeholder, real type unknown
    struct DebugString { u32 _unknown; }; // placeholder, real type unknown
    struct MD5 { u32 _unknown; }; // placeholder, real type unknown
    struct MatchmakeSystemType { u32 _unknown; }; // placeholder, real type unknown
    struct NATTraversalResult { u32 _unknown; }; // placeholder, real type unknown
    struct PacketType { u32 _unknown; }; // placeholder, real type unknown
    struct RelayType { u32 _unknown; }; // placeholder, real type unknown
    struct SignatureBytes { u32 _unknown; }; // placeholder, real type unknown
    struct UserContext { u32 _unknown; }; // placeholder, real type unknown
    struct VirtualPort { u32 _unknown; }; // placeholder, real type unknown
    template <auto T0> class LiteBuffer;
    template <auto T0> class LiteByteStream;
    template <typename T0, typename T1, typename T2> class MethodCallJob;
    template <typename T0, typename T1> class AnyObjectAdapter;
    template <typename T0, typename T1> class AnyObjectHolder;
    template <typename T0, typename T1> class Callback;
    template <typename T0, typename T1> class LogicalClockTmpl;
    template <typename T0, typename T1> class ObjectThread;
    template <typename T0, typename T1> class qChain;
    template <typename T0, typename T1> struct qMap { u32 _unknown; }; // placeholder
    template <typename T0> class CustomDataHolder;
    template <typename T0> class DataStoreCheckConsistencyEventListenerAdaptor;
    template <typename T0> class DataStoreClientTemplate;
    template <typename T0> class DataStoreGetObjectEventListenerTemplate;
    template <typename T0> class Holder;
    template <typename T0> class JobDataStoreGetObject;
    template <typename T0> class JobDataStorePostObject;
    template <typename T0> class JobDataStoreReceiveNewArrivedObjects;
    template <typename T0> class MemAllocator;
    template <typename T0> class PseudoGlobalVariable;
    template <typename T0> class ThreadVariable;
    template <typename T0> class TimedQueue;
    template <typename T0> class VirtualInternet;
    template <typename T0> class VirtualModem;
    template <typename T0> class qList;
    template <typename T0> class qProtectedList;
    template <typename T0> struct ChainPolicyHistoryPacket { u32 _unknown; }; // placeholder
    template <typename T0> struct qVector { u32 _unknown; }; // placeholder
}}

namespace nn { namespace nex { namespace DataStoreConstants { 
    struct Permission { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace nfc { namespace CTR { 
    class NfcIpc;
    class NfcIpcMaster;
    struct NfcState { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace nfp { 
    struct RomInfo { u32 _unknown; }; // placeholder, real type unknown
    struct TargetConnectionStatus { u32 _unknown; }; // placeholder, real type unknown
}}

namespace nn { namespace nfp { namespace CTR { 
    struct Parameter { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace ngc { 
    class CharacterRangeList;
    class ProfanityFilterTemporaryPool;
    class RegexDfaConverter;
    class RegexDfaState;
    class RegexFastMatch;
    class RegexMatch;
    class RegexNfaParser;
    class RegexNfaStateCopier;
    class RegexScanner;
    struct BuiltInCharClassType { u32 _unknown; }; // placeholder, real type unknown
    struct RegexNfaState { u32 _unknown; }; // placeholder, real type unknown
    struct RegexToken { u32 _unknown; }; // placeholder, real type unknown
    template <typename T0> struct UnitList { u32 _unknown; }; // placeholder
}}

namespace nn { namespace ngc { namespace CTR { 
    class ProfanityFilter;
    class ProfanityFilterBase;
    struct ProfanityFilterPatternList { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace nwm { 
    class BeaconDescriptionReader;
    class BssDescriptionReaderBase;
    class ScanResultReaderBase;
    struct BssDescription { u32 _unknown; }; // placeholder, real type unknown
    struct Mac; // nn/nwm/nwm_Types.h
}}

namespace nn { namespace nwm { namespace CTR { 
    class BeaconReader;
    class BssReader;
    struct ScanParamIpc; // nn/nwm/nwm_Types.h
}}}

namespace nn { namespace os { 
    class AddressSpaceManager;
    class AutoStackManager;
    class CriticalSection;
    class EventBase;
    class HandleObject;
    class ITaskInvoker;
    class IWaitTaskInvoker;
    class InterruptEvent;
    class LightEvent;
    class LightSemaphore;
    class LockPolicy;
    class MemoryBlock;
    class ReaderWriterLock;
    class SharedMemoryBlock;
    class SimpleLock;
    class Thread;
    class ThreadLocalStorage;
    class ThreadPool;
    class TransferMemoryBlock;
    class WaitObject;
    class WaitableCounter;
    class Event; // nn/os/os_Event.h
    class MemoryBlockBase; // nn/os/os_MemoryBlockBase.h
    class Tick; // nn/os/os_Tick.h
}}

namespace nn { namespace os { namespace anonymous_namespace { 
    struct WaitMultipleObjectsArgs { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace os { namespace ipc { 
    class Session; // nn/os/os_IpcSession.h
}}}

namespace nn { namespace pia { 
    struct ModuleType { u32 _unknown; }; // placeholder, real type unknown
    struct StationId { u32 _unknown; }; // placeholder, real type unknown
    struct StationIndex { u32 _unknown; }; // placeholder, real type unknown
}}

namespace nn { namespace pia { namespace common { 
    class BackgroundScheduler;
    class ByteOrder;
    class CachedPrint;
    class CallContext;
    class ChunkOld;
    class CriticalSection;
    class CryptoSetting;
    class DateTime;
    class HashContextBase;
    class HeapManager;
    class Hmac;
    class IPacketInput;
    class IPacketOutput;
    class InetAddress;
    class Job;
    class ListBase;
    class Log;
    class Md5Context;
    class MonitoringDataSender;
    class Packet;
    class PacketOld;
    class PayloadSizeManager;
    class RootObject;
    class Scheduler;
    class SessionBeginMonitoringData;
    class SessionEndMonitoringData;
    class SignatureManager;
    class SignatureSetting;
    class StationAddress;
    class StepSequenceJob;
    class String;
    class TimeSpan;
    class Watermark;
    class WatermarkManager;
    class ZlibCompressor;
    struct ListNode { u32 _unknown; }; // placeholder, real type unknown
    struct Time { u32 _unknown; }; // placeholder, real type unknown
    template <auto T0> class SignatureSettingWithKeyBuffer;
    template <typename T0, auto T1> class FixedObjList;
    template <typename T0, auto T1> class SimpleContainer;
    template <typename T0> class ObjList;
}}}

namespace nn { namespace pia { namespace common { namespace Crypto { 
    struct Setting { u32 _unknown; }; // placeholder, real type unknown
}}}}

namespace nn { namespace pia { namespace inet { 
    class CreateMeshJob;
    class InetLeaveWithHostMigrationJob;
    class JoinMeshJob;
    class MissingStationHandler;
    class NatDetecter;
    class NatDetectionJob;
    class NatPortDetecter;
    class NatProbe;
    class NatProbeData;
    class NatProbeList;
    class NatProbeRequest;
    class NatProbeRequestList;
    class NatProperty;
    class NatPropertyDetecter;
    class NatRelayInterface;
    class NatServerAddressResolveJob;
    class NatTraversalTimeList;
    class NatTraverser;
    class NexConnectStationJob;
    class NexCreateSessionSetting;
    class NexDisconnectStationJob;
    class NexFacade;
    class NexJoinSessionSetting;
    class NexJointSessionJob;
    class NexMatchAutoMatchmakeJob;
    class NexMatchBrowseMatchmakeJob;
    class NexMatchClearSystemPasswordJob;
    class NexMatchCreateSessionJob;
    class NexMatchDestroySessionJob;
    class NexMatchGenerateSystemPasswordJob;
    class NexMatchJoinSessionJob;
    class NexMatchLeaveSessionJob;
    class NexMatchMeshLayerController;
    class NexMatchModifyAttributeJob;
    class NexMatchUpdateApplicationDataJob;
    class NexMatchUpdateSessionSettingJob;
    class NexMatchmakeSession;
    class NexMonitoringDataSender;
    class NexNatRelay;
    class NexNatRelayInterface;
    class NexNatTraversalProtocol;
    class NexNetworkFactory;
    class NexProcessHostMigrationJob;
    class NexSessionInfo;
    class NexSessionSearchCriteria;
    class NexSessionSearchCriteriaOwner;
    class NexSessionSearchCriteriaUnused;
    class Socket;
    class SocketAddress;
    class SocketInputStream;
    class SocketOutputStream;
    class SocketStreamBase;
    struct AddrInfo { u32 _unknown; }; // placeholder, real type unknown
    struct NatTraversalTime { u32 _unknown; }; // placeholder, real type unknown
    struct Setting { u32 _unknown; }; // placeholder, real type unknown
    struct SockAddrIn { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace pia { namespace local { 
    class LocalAckMessage;
    class LocalAroundNetworkSearchBackgroundJob;
    class LocalAroundNetworkSearchJob;
    class LocalAroundNetworkSearchManager;
    class LocalBackgroundProcessJob;
    class LocalConnectNetworkJob;
    class LocalConnectionStatus;
    class LocalCreateNetworkJob;
    class LocalCreateSessionSetting;
    class LocalDestroyNetworkJob;
    class LocalDestroyNetworkMessage;
    class LocalDisconnectNetworkJob;
    class LocalEventCheckBackgroundJob;
    class LocalEventJob;
    class LocalFacade;
    class LocalForceDisconnectNetworkJob;
    class LocalHostMigrationJob;
    class LocalInputStream;
    class LocalJoinSessionSetting;
    class LocalKickoutManageJob;
    class LocalLeaveWithHostMigrationJobNew;
    class LocalMatchBrowseMatchmakeJob;
    class LocalMatchCreateSessionJob;
    class LocalMatchDestroySessionJob;
    class LocalMatchJoinSessionJob;
    class LocalMatchLeaveSessionJob;
    class LocalMatchMeshLayerController;
    class LocalMatchUpdateApplicationDataJob;
    class LocalMatchmakeSession;
    class LocalMessage;
    class LocalMigrationManager;
    class LocalNetwork;
    class LocalNetworkDescription;
    class LocalNetworkFactory;
    class LocalNetworkManager;
    class LocalNetworkSetting;
    class LocalOutputStream;
    class LocalParseSystemMessageJob;
    class LocalProcessHostMigrationJobNew;
    class LocalReceiveFromJob;
    class LocalScanNetworkJob;
    class LocalSendMessageJob;
    class LocalSendSystemMessageBackgroundJob;
    class LocalSessionInfo;
    class LocalSessionSearchCriteria;
    class LocalStartHostMigrationMessage;
    class LocalStreamBase;
    class LocalUpdateSessionMessage;
    class UdsAroundNetworkSearchBackgroundJob;
    class UdsAroundNetworkSearchManager;
    class UdsBackgroundProcessJob;
    class UdsCreateSessionSetting;
    class UdsHandle;
    class UdsJoinSessionSetting;
    class UdsMatchmakeSession;
    class UdsMigrationManagerNew;
    class UdsNetworkConnectionStatus;
    class UdsNetworkDescription;
    class UdsNetworkFactory;
    class UdsNetworkManager;
    class UdsNetworkSetting;
    class UdsSessionInfo;
    struct LocalAroundNetworkSearchSetting { u32 _unknown; }; // placeholder, real type unknown
    struct LocalConnectNetworkSetting { u32 _unknown; }; // placeholder, real type unknown
    struct LocalCreateNetworkSetting { u32 _unknown; }; // placeholder, real type unknown
    struct LocalScanNetworkSetting { u32 _unknown; }; // placeholder, real type unknown
    struct LocalUpdateEvent { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace pia { namespace session { 
    class AutoMatchmakeJob;
    class BrowseMatchmakeJob;
    class ClearMatchmakeSystemPasswordJob;
    class CloseParticipationJob;
    class CommonMatchmakeSession;
    class ConfigParticipationJobBase;
    class CreateMeshJob;
    class CreateSessionJob;
    class CreateSessionSetting;
    class DestroyMeshJob;
    class DestroySessionJob;
    class GenerateMatchmakeSystemPasswordJob;
    class IMatchmakeSession;
    class ISessionInfo;
    class ISessionInfoList;
    class JoinMeshJob;
    class JoinSessionJob;
    class JoinSessionSetting;
    class JointSessionJob;
    class KickoutManageJob;
    class LeaveMeshJob;
    class LeaveSessionJob;
    class LeaveWithHostMigrationJob;
    class Mesh;
    class MeshEventListener;
    class MeshEventListenerForSession;
    class MeshLayerController;
    class MeshProtocol;
    class ModifyAttributeJob;
    class OpenParticipationJob;
    class ProcessDestroyMeshJob;
    class ProcessHostMigrationJob;
    class ProcessJoinRequestJob;
    class ProcessUpdateMeshJob;
    class RelayRouteManageJob;
    class Session;
    class SessionProtocol;
    class SessionSearchCriteria;
    class SessionStatusCheckJob;
    class SignatureSettingStorage;
    class StationIdStatusTable;
    class SyncClock;
    class SyncClockProtocol;
    class UpdateApplicationDataJob;
    class UpdateSessionSettingJob;
    template <typename T0> class SessionInfoList;
}}}

namespace nn { namespace pia { namespace transport { 
    class AnalysisPrinter;
    class AttendanceTable;
    class BandwidthCheckerProtocol;
    class ConnectStationJob;
    class ConnectionAnalysisData;
    class ConnectionAnalyzer;
    class DisconnectStationJob;
    class IdentificationInfoTable;
    class KeepAliveReceiver;
    class KeepAliveSender;
    class LatencyEmulator;
    class MissingStationHandler;
    class NetworkFactory;
    class NetworkRttManager;
    class PacketAnalysisData;
    class PacketAnalyzer;
    class PacketHandler;
    class PacketStream;
    class ProcessConnectionRequestJob;
    class Protocol;
    class ProtocolEvent;
    class ProtocolManager;
    class ProtocolMessageFilteringManager;
    class ProtocolMessageReader;
    class ProtocolMessageWriter;
    class ReceiveThreadStream;
    class RelayRouteManager;
    class ReliableProtocol;
    class ReliableSlidingWindow;
    class ResendingMessageManager;
    class RttCalculator;
    class RttProtocol;
    class SendThreadStream;
    class SequenceIdController;
    class Station;
    class StationConnectionInfo;
    class StationConnectionInfoTable;
    class StationIdTable;
    class StationLocation;
    class StationManager;
    class StationPacketHandler;
    class StationProtocol;
    class StationProtocolManager;
    class StationProtocolReliable;
    class ThreadStreamManager;
    class Transport;
    class TransportAnalysisData;
    class TransportAnalyzer;
    class TransportThreadStream;
    struct ProtocolId { u32 _unknown; }; // placeholder, real type unknown
    struct ReceivedMessageAccessor { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace ptm { namespace CTR { 
    enum TargetPlatform : u8; // nn/ptm/CTR/ptm_Types.h
}}}

namespace nn { namespace ptm { namespace CTR { namespace detail { 
    class PtmIpc;
}}}}

namespace nn { namespace ro { 
    class Module;
}}

namespace nn { namespace snd { namespace CTR { 
    class DspFxDelay;
    class DspFxManager;
    class DspFxManagerImpl;
    class DspFxReverb;
    class Dspsnd;
    class FxDelay;
    class FxReverb;
    class MasterManager;
    class MasterManagerImpl;
    class OutputCapture;
    class ThreadManager;
    class Voice;
    class VoiceImpl;
    class VoiceManager;
    struct AdpcmContext { u32 _unknown; }; // placeholder, real type unknown
    struct AdpcmParam { u32 _unknown; }; // placeholder, real type unknown
    struct AuxBusData { u32 _unknown; }; // placeholder, real type unknown
    struct AuxBusId { u32 _unknown; }; // placeholder, real type unknown
    struct BiquadFilterCoefficients { u32 _unknown; }; // placeholder, real type unknown
    struct ClippingMode { u32 _unknown; }; // placeholder, real type unknown
    struct DspFxDelayParams { u32 _unknown; }; // placeholder, real type unknown
    struct DspFxReverbParams { u32 _unknown; }; // placeholder, real type unknown
    struct DspsndAudioInfo { u32 _unknown; }; // placeholder, real type unknown
    struct FilterType { u32 _unknown; }; // placeholder, real type unknown
    struct InterpolationType { u32 _unknown; }; // placeholder, real type unknown
    struct MixParam { u32 _unknown; }; // placeholder, real type unknown
    struct MonoFilterCoefficients { u32 _unknown; }; // placeholder, real type unknown
    struct OutputMode { u32 _unknown; }; // placeholder, real type unknown
    struct SampleFormat { u32 _unknown; }; // placeholder, real type unknown
    struct SurroundSpeakerPosition { u32 _unknown; }; // placeholder, real type unknown
    struct SyncMode { u32 _unknown; }; // placeholder, real type unknown
    struct ThreadParameter { u32 _unknown; }; // placeholder, real type unknown
    struct WaveBuffer { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nn { namespace socket { 
    class Privileged;
    struct AddrInfo { u32 _unknown; }; // placeholder, real type unknown
    struct PollFd { u32 _unknown; }; // placeholder, real type unknown
}}

namespace nn { namespace socket { namespace detail { 
    class DnsUserClient;
    class SessionItem;
    class SessionPool;
    class SessionPoolAuto;
    class User;
}}}

namespace nn { namespace srv { 
    class NotificationHandler;
    template <typename T0> class EventNotificationHandlerBase;
}}

namespace nn { namespace srv { namespace detail { 
    class Service;
}}}

namespace nn { namespace ssl { 
    class ConnectionIpc;
}}

namespace nn { namespace ssl { namespace detail { 
    class LibManager;
}}}

namespace nn { namespace uds { namespace CTR { 
    class NetworkDescription;
    class NetworkDescriptionReader;
    class NetworkDescriptionReaderInternal;
    class ScanResultReader;
    class ScanResultReaderInternal;
    enum ConnectType : u8; // nn/uds/CTR/uds_Types.h
    struct ConnectionStatus; // nn/uds/CTR/uds_Types.h
    struct EndpointDescriptor;
    struct NodeInformation;
    struct ScrambledLocalFriendCode;
}}}

namespace nn { namespace uds { namespace CTR { namespace detail { 
    class Uds;
    struct NetworkDescriptionElement; // nn/uds/CTR/uds_Types.h
    struct NodeInformationElement;
    struct NodeInformationList;
    struct NodeInformationRaw;
    struct ReceiveReport;
}}}}

namespace nn { namespace ulcd { namespace CTR { 
    class StereoCamera;
}}}

namespace nn { namespace util { 
    class Crc32;
}}

namespace nn { namespace util { namespace ADLFireWall { 
    template <typename T0> class NonCopyable;
}}}

namespace nn { namespace y2r { namespace CTR { 
    enum OutputFormat : u8; // nn/y2r/CTR/y2r_Types.h
    enum Rotation : u8;
    enum StandardCoefficient : u8;
}}}

namespace nn { namespace y2r { namespace CTR { namespace detail { 
    class Y2r;
}}}}

namespace nnmfolw { 
    class FollowerAStarForeachCB;
    class FollowerRoadSearchAgent;
}

namespace nonameg3d { 
    class Char;
    class CharTreeNode;
}

namespace npcexhibition { 
    class AcNpcSpExhibitionHioNode;
}

namespace npcmaster { 
    class AcNpcSpMasterHioNode;
    class GetCafeNpcFunction;
}

namespace npcspcleaning { 
    class AcNpcSpCleaningHioNode;
}

namespace npcspcleaningvisit { 
    class AcNpcSpCleaningVisitHioNode;
}

namespace npcutil { 
    class ISearchFgFunc;
    class ISearchFgFuncGut;
    class NpcSearchCB;
}

namespace nw { namespace anim { 
    class AnimBlendOp;
    class AnimBlendOpBool;
    class AnimBlendOpFloat;
    class AnimBlendOpInt;
    class AnimBlendOpRgbaColor;
    class AnimBlendOpTexture;
    class AnimBlendOpVector2;
    class AnimBlendOpVector3;
    class AnimFrame;
    class AnimFrameController;
    class AnimResult;
}}

namespace nw { namespace anim { namespace res { 
    class ResAnim;
    class ResAnimGroupMember;
    class ResBakedTransformAnim;
    class ResFragmentLightMember;
    class ResHemiSphereLightMember;
    class ResMaterialColorMember;
    class ResMemberAnim;
    class ResProjectionUpdaterMember;
    class ResRgbaColorAnim;
    class ResTextureAnim;
    class ResTextureCoordinatorMember;
    class ResTransformAnim;
    class ResVec3Anim;
    class ResVertexLightMember;
    struct ResAnimGroup { u32 _unknown; }; // placeholder, real type unknown
    struct ResBoolCurveData { u32 _unknown; }; // placeholder, real type unknown
    struct ResFloatCurveData { u32 _unknown; }; // placeholder, real type unknown
    struct ResFullBakedCurveData { u32 _unknown; }; // placeholder, real type unknown
    struct ResIntCurveData { u32 _unknown; }; // placeholder, real type unknown
    template <typename T0> struct ResBakedCurveData { u32 _unknown; }; // placeholder
}}}

namespace nw { namespace font { 
    class CharStrmReader;
    class CharWriter;
    class Font;
    class Glyph;
    class PairFont;
    class RectDrawer;
    class ResFont;
    class ResFontBase;
    class TextWriterResource;
    template <typename T0> class TagProcessorBase;
    template <typename T0> struct TextWriterBase { u32 _unknown; }; // placeholder
}}

namespace nw { namespace font { namespace internal { 
    class TextureObject;
}}}

namespace nw { namespace gfx { 
    class AimTargetViewUpdater;
    class AmbientLight;
    class AnimBinding;
    class AnimBlendOpTransform;
    class AnimBlender;
    class AnimEvaluator;
    class AnimGroup;
    class AnimObject;
    class AnimOverrider;
    class BaseAnimEvaluator;
    class BillboardUpdater;
    class CalculatedTransform;
    class Camera;
    class CameraProjectionUpdater;
    class CameraViewUpdater;
    class Fog;
    class FragmentLight;
    class FrustumProjectionUpdater;
    class GfxObject;
    class GraphicsDevice;
    class HemiSphereLight;
    class IMaterialActivator;
    class ISceneUpdater;
    class ISceneVisitor;
    class Light;
    class LightSet;
    class LookAtTargetViewUpdater;
    class Material;
    class MaterialActivator;
    class MeshRenderer;
    class Model;
    class OrthoProjectionUpdater;
    class ParticleCollection;
    class ParticleEmitter;
    class ParticleMaterialActivator;
    class ParticleModel;
    class ParticleSet;
    class ParticleShape;
    class ParticleUtil;
    class PerspectiveProjectionUpdater;
    class RenderContext;
    class RotateViewUpdater;
    class SceneBuilder;
    class SceneContext;
    class SceneEnvironment;
    class SceneEnvironmentSetting;
    class SceneHelper;
    class SceneNode;
    class SceneObject;
    class SceneTraverser;
    class SceneUpdater;
    class ShaderBinaryInfo;
    class ShaderProgram;
    class SimpleMaterialActivator;
    class SkeletalModel;
    class Skeleton;
    class SkeletonUpdater;
    class StandardSkeleton;
    class TransformAnimBlendOp;
    class TransformAnimBlendOpAccScale;
    class TransformAnimBlendOpAccScaleQuat;
    class TransformAnimBlendOpQuat;
    class TransformAnimBlendOpStandard;
    class TransformAnimEvaluator;
    class TransformNode;
    class WorldMatrixUpdater;
    template <typename T0, typename T1, typename T2> class BasicRenderQueue;
    template <typename T0, typename T1> class PriorDepthRenderKeyFactory;
    template <typename T0, typename T1> class PriorMaterialRenderKeyFactory;
    template <typename T0> class BasicRenderKeyFactory;
    template <typename T0> struct BasicRenderElement { u32 _unknown; }; // placeholder
    template <typename T0> struct GfxPtr { u32 _unknown; }; // placeholder
}}

namespace nw { namespace gfx { namespace internal { 
    class MaterialState;
    struct CommandBufferInfo { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nw { namespace gfx { namespace res { 
    class ResFog;
    class ResFragmentLightingTable;
    class ResFragmentShader;
    class ResGraphicsFile;
    class ResLight;
    class ResLookupTable;
    class ResLookupTableSet;
    class ResMaterial;
    class ResMesh;
    class ResModel;
    class ResPixelBasedTextureMapper;
    class ResPrimitive;
    class ResPrimitiveSet;
    class ResShader;
    class ResShape;
    class ResTexture;
    class ResTextureCoordinatorData;
    class ResTextureMapper;
    class ResTextureSampler;
    class ResVertexAttribute;
    struct ResAimTargetViewUpdater { u32 _unknown; }; // placeholder, real type unknown
    struct ResAmbientLight { u32 _unknown; }; // placeholder, real type unknown
    struct ResCamera { u32 _unknown; }; // placeholder, real type unknown
    struct ResFogData { u32 _unknown; }; // placeholder, real type unknown
    struct ResFogUpdater { u32 _unknown; }; // placeholder, real type unknown
    struct ResFragmentLight { u32 _unknown; }; // placeholder, real type unknown
    struct ResFragmentLightData { u32 _unknown; }; // placeholder, real type unknown
    struct ResFrustumProjectionUpdater { u32 _unknown; }; // placeholder, real type unknown
    struct ResHemiSphereLight { u32 _unknown; }; // placeholder, real type unknown
    struct ResImageLookupTable { u32 _unknown; }; // placeholder, real type unknown
    struct ResIndexStream { u32 _unknown; }; // placeholder, real type unknown
    struct ResLightingLookupTable { u32 _unknown; }; // placeholder, real type unknown
    struct ResLookAtTargetViewUpdater { u32 _unknown; }; // placeholder, real type unknown
    struct ResMaterialColor { u32 _unknown; }; // placeholder, real type unknown
    struct ResMaterialData { u32 _unknown; }; // placeholder, real type unknown
    struct ResOrthoProjectionUpdater { u32 _unknown; }; // placeholder, real type unknown
    struct ResParticleCollection { u32 _unknown; }; // placeholder, real type unknown
    struct ResParticleInitializer { u32 _unknown; }; // placeholder, real type unknown
    struct ResParticleModel { u32 _unknown; }; // placeholder, real type unknown
    struct ResParticleSet { u32 _unknown; }; // placeholder, real type unknown
    struct ResParticleUpdater { u32 _unknown; }; // placeholder, real type unknown
    struct ResPerspectiveProjectionUpdater { u32 _unknown; }; // placeholder, real type unknown
    struct ResReferenceLookupTable { u32 _unknown; }; // placeholder, real type unknown
    struct ResReferenceShader { u32 _unknown; }; // placeholder, real type unknown
    struct ResReferenceTexture { u32 _unknown; }; // placeholder, real type unknown
    struct ResRotateViewUpdater { u32 _unknown; }; // placeholder, real type unknown
    struct ResSceneEnvironmentSetting { u32 _unknown; }; // placeholder, real type unknown
    struct ResSceneObject { u32 _unknown; }; // placeholder, real type unknown
    struct ResSeparateDataShape { u32 _unknown; }; // placeholder, real type unknown
    struct ResShaderProgramDescription { u32 _unknown; }; // placeholder, real type unknown
    struct ResShaderSymbol { u32 _unknown; }; // placeholder, real type unknown
    struct ResSkeleton { u32 _unknown; }; // placeholder, real type unknown
    struct ResTransformNode { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nw { namespace gfx { namespace res { namespace ResBone { 
    struct BillboardMode { u32 _unknown; }; // placeholder, real type unknown
}}}}

namespace nw { namespace gfx { namespace res { namespace ResTextureCoordinator { 
    struct MappingMatrixMode { u32 _unknown; }; // placeholder, real type unknown
}}}}

namespace nw { namespace io { 
    class FileStream;
    class IOStream;
}}

namespace nw { namespace lyt { 
    class AnimResource;
    class AnimTransform;
    class AnimTransformBasic;
    class ArcResourceAccessor;
    class Bounding;
    class DrawInfo;
    class Drawer;
    class GraphicsResource;
    class Group;
    class GroupContainer;
    class Layout;
    class Material;
    class Pane;
    class Picture;
    class ResourceAccessor;
    class TexMap;
    class TexResource;
    class TextBox;
    class Window;
    struct AnimationLink { u32 _unknown; }; // placeholder, real type unknown
    struct ResBlockSet { u32 _unknown; }; // placeholder, real type unknown
    struct Size { u32 _unknown; }; // placeholder, real type unknown
    struct TextureInfo { u32 _unknown; }; // placeholder, real type unknown
}}

namespace nw { namespace lyt { namespace internal { 
    class PaneBase;
    class TexCoordAry;
}}}

namespace nw { namespace lyt { namespace res { 
    struct AnimationBlock { u32 _unknown; }; // placeholder, real type unknown
    struct Bounding { u32 _unknown; }; // placeholder, real type unknown
    struct Group { u32 _unknown; }; // placeholder, real type unknown
    struct Material { u32 _unknown; }; // placeholder, real type unknown
    struct Pane { u32 _unknown; }; // placeholder, real type unknown
    struct Picture { u32 _unknown; }; // placeholder, real type unknown
    struct TextBox { u32 _unknown; }; // placeholder, real type unknown
    struct Window { u32 _unknown; }; // placeholder, real type unknown
}}}

namespace nw { namespace math { 
    class AABB;
    class PLANE;
}}

namespace nw { namespace os { 
    class IAllocator;
    struct MemorySizeCalculator { u32 _unknown; }; // placeholder, real type unknown
}}

namespace nw { namespace snd { 
    class BiquadFilterCallback;
    class FsSoundArchive;
    class FxBase;
    class FxDelay;
    class FxReverb;
    class MemorySoundArchive;
    class RomSoundArchive;
    class SoundArchive;
    class SoundArchivePlayer;
    class SoundDataManager;
    class SoundHandle;
    class SoundHeap;
    class SoundMemoryAllocatable;
    class SoundPlayer;
    class SoundStartable;
    class SoundSystem;
    class StreamSoundHandle;
    class WaveSoundHandle;
    struct AuxBus { u32 _unknown; }; // placeholder, real type unknown
    struct OutputMode { u32 _unknown; }; // placeholder, real type unknown
    struct PanCurve { u32 _unknown; }; // placeholder, real type unknown
    struct PanMode { u32 _unknown; }; // placeholder, real type unknown
    struct SampleFormat { u32 _unknown; }; // placeholder, real type unknown
    struct SeqMute { u32 _unknown; }; // placeholder, real type unknown
    struct SequenceUserprocCallbackParam { u32 _unknown; }; // placeholder, real type unknown
    struct SoundActor { u32 _unknown; }; // placeholder, real type unknown
    struct SoundThreadProfile { u32 _unknown; }; // placeholder, real type unknown
    struct StreamDataInfo { u32 _unknown; }; // placeholder, real type unknown
}}

namespace nw { namespace snd { namespace internal { 
    class BankFile;
    class BankFileReader;
    class BasicSound;
    class BiquadFilterBpf1024;
    class BiquadFilterBpf2048;
    class BiquadFilterBpf512;
    class BiquadFilterHpf;
    class BiquadFilterLpf;
    class CachedFileStream;
    class CurveAdshr;
    class CurveLfo;
    class CurveLfoParam;
    class DriverCommand;
    class DriverCommandManager;
    class ExternalSoundPlayer;
    class FileReader;
    class FrameHeap;
    class FsFileStream;
    class FsSoundArchiveBase;
    class GroupFile;
    class GroupFileReader;
    class MemoryFileStream;
    class PlayerHeap;
    class PlayerHeapDataManager;
    class PoolImpl;
    class SequenceSound;
    class SequenceSoundFile;
    class SequenceSoundFileReader;
    class SoundArchiveFile;
    class SoundArchiveFileReader;
    class SoundArchiveLoader;
    class StreamDataInfoDetail;
    class StreamSound;
    class StreamSoundFile;
    class StreamSoundFileLoader;
    class StreamSoundFileReader;
    class StreamSoundPrefetchFile;
    class StreamSoundPrefetchFileReader;
    class Task;
    class TaskManager;
    class TaskThread;
    class ThreadStack;
    class WaveArchiveFile;
    class WaveArchiveFileReader;
    class WaveFile;
    class WaveFileReader;
    class WaveSound;
    class WaveSoundFile;
    class WaveSoundFileReader;
    struct DriverCommandStreamSoundLoadData { u32 _unknown; }; // placeholder, real type unknown
    struct DriverCommandStreamSoundLoadHeader { u32 _unknown; }; // placeholder, real type unknown
    struct GroupItemLocationInfo { u32 _unknown; }; // placeholder, real type unknown
    struct LoadDataParam { u32 _unknown; }; // placeholder, real type unknown
    struct LoadItemInfo { u32 _unknown; }; // placeholder, real type unknown
    struct VelocityRegionInfo { u32 _unknown; }; // placeholder, real type unknown
    struct WaveInfo { u32 _unknown; }; // placeholder, real type unknown
    struct WaveSoundInfo { u32 _unknown; }; // placeholder, real type unknown
    struct WaveSoundNoteInfo { u32 _unknown; }; // placeholder, real type unknown
    template <typename T0> class LoaderManager;
    template <typename T0> struct SoundInstanceManager { u32 _unknown; }; // placeholder
}}}

namespace nw { namespace snd { namespace internal { namespace Util { 
    class BitFlag;
    struct PanInfo { u32 _unknown; }; // placeholder, real type unknown
}}}}

namespace nw { namespace snd { namespace internal { namespace driver { 
    class Bank;
    class BasicSoundPlayer;
    class Channel;
    class ChannelManager;
    class DisposeCallback;
    class DisposeCallbackManager;
    class HardwareManager;
    class MmlParser;
    class MmlSequenceTrack;
    class MmlSequenceTrackAllocator;
    class MultiVoice;
    class NoteOnCallback;
    class SequenceSoundLoader;
    class SequenceSoundPlayer;
    class SequenceTrack;
    class SequenceTrackAllocator;
    class SoundThread;
    class StreamBufferPool;
    class StreamChannel;
    class StreamSoundLoader;
    class StreamSoundPlayer;
    class Voice;
    class VoiceManager;
    class WaveSoundLoader;
    class WaveSoundPlayer;
    struct NoteOnInfo { u32 _unknown; }; // placeholder, real type unknown
    struct StreamTrack { u32 _unknown; }; // placeholder, real type unknown
}}}}

namespace nw { namespace ut { 
    class Color8;
    class FloatColor;
    class FrameHeap;
    class HeapBase;
    class HeapNode;
    class ResDicPatricia;
    struct BinaryBlockHeader { u32 _unknown; }; // placeholder, real type unknown
    struct BinaryFileHeader { u32 _unknown; }; // placeholder, real type unknown
    struct LinkListNode { u32 _unknown; }; // placeholder, real type unknown
    template <typename T0, auto T1> struct LinkList { u32 _unknown; }; // placeholder
    template <typename T0> struct MoveArray { u32 _unknown; }; // placeholder
}}

namespace nw { namespace ut { namespace internal { 
    class CmdCache;
    class LinkListImpl;
    struct ResArrayClassTraits { u32 _unknown; }; // placeholder, real type unknown
    template <typename T0, typename T1> struct ResArray { u32 _unknown; }; // placeholder
}}}

namespace objctrl { 
    class ObjContainer;
    class RegistObjContainerList;
    class RegistObjContainerListNode;
}

namespace oml { namespace framework { 
    class Process;
    class ProcessManager;
    struct Result { u32 _unknown; }; // placeholder, real type unknown
}}

namespace palloncino { 
    class AcNpcSpPalloncinoHioNode;
}

namespace panenpc { 
    class NpcCountFunc;
    class NpcCreateWaitFunc;
    class NpcDeleteRequestFunc;
    class NpcDemoDollSetupFunc;
}

namespace paneponbg { 
    class CstmCamera;
}

namespace pead { 
    class BinaryStreamFormat;
    class CriticalSection;
    class DelegateThread;
    class Event;
    class ExpHeap;
    class Heap;
    class HeapMgr;
    class IDisposer;
    class INamable;
    class MainThread;
    class Mutex;
    class PrintStreamSrc;
    class StreamFormat;
    class StreamSrc;
    class TextStreamFormat;
    class Thread;
    class ThreadMgr;
    template <auto T0> class FixedSafeString;
    template <typename T0, auto T1> class FixedSafeStringBase;
    template <typename T0, typename T1, typename T2> class Delegate2;
    template <typename T0, typename T1, typename T2> class DelegateBase;
    template <typename T0, typename T1> class IDelegate2;
    template <typename T0> class BufferedSafeStringBase;
    template <typename T0> class DelegateEvent;
    template <typename T0> class IDelegate1;
    template <typename T0> class SafeStringBase;
}

namespace pead { namespace PrintConfig { 
    struct PrintEventArg { u32 _unknown; }; // placeholder, real type unknown
}}

namespace pead { namespace RuntimeTypeInfo { 
    class Interface;
    class Root;
    template <typename T0> class Derive;
}}

namespace pead { namespace hostio { 
    class LifeCheckable;
    class Node;
    class NodeEventListener;
    class PropertyEventListener;
    class Reflexible;
}}

namespace perionormal { 
    class AcNpcSpPerioNormalHioNode;
}

namespace periospecial { 
    class AcNpcSpPerioSpecialHioNode;
}

namespace periotutorial { 
    class AcNpcSpPerioTutorialHioNode;
}

namespace periowarning { 
    class AcNpcSpPerioWarningHioNode;
}

namespace photo { 
    class Allocator;
    class BsInvoke;
    class Mgr;
}

namespace postoffice { 
    class AcNpcSpPostOfficeHioNode;
}

namespace presisyo { 
    class AcNpcSpPreSisyoHioNode;
}

namespace prlgtanukichi { 
    class AcNpcSpPrologueTanukichiHioNode;
}

namespace pumpking { 
    class AcNpcSpPumpkingHioNode;
}

namespace pumpkingpre { 
    class AcNpcSpPumpkingPreHioNode;
}

namespace pyontarou { 
    class AcNpcSpPyontarouHioNode;
}

namespace qrdecode { 
    class Camera;
    class Thread;
}

namespace qrenc { 
    class MenuMainHostIO;
}

namespace racketsanin { 
    class AcNpcSpRacketsanInHioNode;
    class GetResetsanFunction;
}

namespace rakosuke { 
    class AcNpcSpRakosukeHioNode;
}

namespace resetsan { 
    class AcNpcSpResetsanHioNode;
}

namespace resetsanin { 
    class AcNpcSpResetsanInHioNode;
    class GetRacketsanFunction;
}

namespace rolan { 
    class AcNpcSpRollanHioNode;
}

namespace script { 
    class Base;
    class CapitalTopEngine;
    class CatEngine;
    class CatRepelTagEngine;
    class Chip;
    class ChoiceBase;
    class ChoiceSeq;
    class ChoiceStandard;
    class ChoiceStandardItem;
    class ChoiceStandardWin;
    class CmpEngine;
    class CmpFuzzyEngine;
    class CmpPartFuzzyEngine;
    class ContainTagEngine;
    class CopyEngine;
    class CopyExcludeKanjiEngine;
    class CopyRepelTagEngine;
    class CountCode;
    class CountReturn;
    class DetectCode;
    class DialogCompo;
    class ElisionEngine;
    class EpenthesisEngine;
    class EscapeChoiceStandardWin;
    class FlowEngine;
    class FlowSeq;
    class IFlowRecept;
    class IMailRecept;
    class IMeasure;
    class IRecept;
    class ITalkRecept;
    class IUiRecept;
    class IWord;
    class InflectEngine;
    class Insert;
    class InsertEngine;
    class InsertSearcher;
    class KappeiNoteEngine;
    class LineTracer;
    class Loader;
    class MailInsert;
    class MailMeasure;
    class MailMgr;
    class MailPhrase;
    class MailSeq;
    class Mgr;
    class MsgEngine;
    class MsgEngineModify;
    class PatchimTargetEngine;
    class PeriodCountEngine;
    class Phrase;
    class Project;
    class RenderBase;
    class RenderGarden;
    class RenderMain;
    class RenderRuby;
    class RenderTrace;
    class ReplaceEngine;
    class Searcher;
    class SeqEngine;
    class StrMgr;
    class StrNicknameEngine;
    class StrRemakeColor;
    class TalkInsert;
    class TalkMeasure;
    class TalkPhrase;
    class TalkRuby;
    class TalkSeq;
    class TalkWin;
    class UiInsert;
    class UiMgr;
    class UiPhrase;
    class UiSeq;
    class WordCPtr;
    class WordCPtrSv;
    class WordNumDigit;
    class WordPtr;
    class WordPtrSv;
    class WordRes;
    template <auto T0, typename T1> class WordKey;
    template <auto T0> class WordFix;
}

namespace sead { 
    class AccelerometerAddon;
    class AllocatorNw4c;
    class AnyFileDevice;
    class Arena;
    class AudioFsSoundArchiveCtr;
    class AudioFx;
    class AudioFxCtr;
    class AudioFxHolder;
    class AudioFxMemoryMgr;
    class AudioFxMemoryMgrCtr;
    class AudioFxMgr;
    class AudioMemorySoundArchiveCtr;
    class AudioMgr;
    class AudioPlayer;
    class AudioPlayerCtr;
    class AudioResetter;
    class AudioResetterCtr;
    class AudioResourceLoader;
    class AudioResourceLoaderCtr;
    class AudioRomSoundArchiveCtr;
    class AudioSettingParameter;
    class AudioSoundArchiveBaseCtr;
    class AudioSoundDataMgrCtr;
    class AudioSoundHeapCtr;
    class AudioSubsetBase;
    class AudioSystem;
    class AudioSystemCtr;
    class BinaryStreamFormat;
    class BitFlagUtil;
    class CalculateTask;
    class Camera;
    class CameraProjectionUpdaterNw4c;
    class Color4f;
    class ControlDevice;
    class Controller;
    class ControllerAddon;
    class ControllerBase;
    class ControllerMgr;
    class ControllerWrapper;
    class ControllerWrapperBase;
    class CriticalSection;
    class CtrAccelerometerAddon;
    class CtrBackupFileDevice;
    class CtrController;
    class CtrFileDevice;
    class CtrFileStreamFileDevice;
    class CtrHidDevice;
    class DebugMenuSkinBase;
    class DebugMenuSkinDefault;
    class DefaultGfxMemoryMgrCtr;
    class DelegateThread;
    class DirectResource;
    class DirectResourceFactoryBase;
    class DoubleCmdGameFrameworkCtrNw4c;
    class DrawLockContext;
    class DualScreenMethodTreeMgr;
    class DualScreenTask;
    class ExpHeap;
    class FaderTaskBase;
    class FileDevice;
    class FileDeviceMgr;
    class FileHandle;
    class FontBase;
    class FrameBuffer;
    class FrameBufferCtr;
    class FrameHeap;
    class Framework;
    class GameFramework;
    class GameFrameworkCtrNw4c;
    class Geometry;
    class GfxMemoryMgrCtr;
    class GlobalRandom;
    class Graphics;
    class GraphicsContext;
    class GraphicsCtr;
    class GraphicsFileResNw4c;
    class HandleBase;
    class Heap;
    class HeapMgr;
    class HostIOMgr;
    class IDelegate;
    class IDisposer;
    class INamable;
    class ListImpl;
    class ListNode;
    class LogicalFrameBuffer;
    class LookAtCamera;
    class MainFileDevice;
    class MainThread;
    class MessageQueue;
    class MethodTreeMgr;
    class MethodTreeNode;
    class Mutex;
    class NullFaderTask;
    class OrthoCamera;
    class OrthoProjection;
    class PerspectiveProjection;
    class PrimitiveDrawer;
    class PrintFormatter;
    class PrintOutput;
    class PrintStreamSrc;
    class ProcessMeter;
    class Projection;
    class PtrArrayImpl;
    class Random;
    class Resource;
    class ResourceFactory;
    class ResourceMgr;
    class SoundHandle;
    class StreamFormat;
    class StreamSrc;
    class StringPrintOutput;
    class StringUtil;
    class TaskBase;
    class TaskClassID;
    class TaskEvent;
    class TaskMgr;
    class TaskParameter;
    class TextStreamFormat;
    class TextWriter;
    class Texture;
    class TextureCtr;
    class TextureCtrGR;
    class Thread;
    class ThreadMgr;
    class TickTime;
    class TreeNode;
    class UlcdDoubleCmdGameFrameworkCtrNw4c;
    class UlcdMethodTreeMgr;
    class UlcdTask;
    class UnitHeap;
    class Viewport;
    struct HeapArray { u32 _unknown; }; // placeholder, real type unknown
    struct MemBlock { u32 _unknown; }; // placeholder, real type unknown
    struct TaskConstructArg { u32 _unknown; }; // placeholder, real type unknown
    template <auto T0> class FixedSafeString;
    template <auto T0> class FormatFixedSafeString;
    template <auto T0> class WFixedSafeString;
    template <typename T0, auto T1> class FixedSafeStringBase;
    template <typename T0, typename T1, typename T2, typename T3> class Delegate2R;
    template <typename T0, typename T1, typename T2> class Delegate2;
    template <typename T0, typename T1, typename T2> class DelegateBase;
    template <typename T0, typename T1, typename T2> class IDelegate2R;
    template <typename T0, typename T1> class AudioFxHolderCtr;
    template <typename T0, typename T1> class Delegate1;
    template <typename T0, typename T1> class DelegateR;
    template <typename T0, typename T1> class IDelegate2;
    template <typename T0, typename T1> class TreeMap;
    template <typename T0> class AudioDeviceSoundArchiveBaseCtr;
    template <typename T0> class BaseVec3;
    template <typename T0> class BoundBox2;
    template <typename T0> class BufferedSafeStringBase;
    template <typename T0> class Delegate;
    template <typename T0> class DelegateEvent;
    template <typename T0> class DelegateRFunc;
    template <typename T0> class DirectResourceFactory;
    template <typename T0> class IDelegate1;
    template <typename T0> class IDelegateR;
    template <typename T0> class SafeStringBase;
    template <typename T0> class TListNode;
    template <typename T0> class TTreeNode;
    template <typename T0> class TreeMapNode;
    template <typename T0> class Vector3;
    template <typename T0> struct Matrix34 { u32 _unknown; }; // placeholder
    template <typename T0> struct Matrix44 { u32 _unknown; }; // placeholder
    template <typename T0> struct Segment { u32 _unknown; }; // placeholder
    template <typename T0> struct TList { u32 _unknown; }; // placeholder
    template <typename T0> struct Vector2 { u32 _unknown; }; // placeholder
}

namespace sead { namespace AudioGlobal { 
    struct AuxBus { u32 _unknown; }; // placeholder, real type unknown
}}

namespace sead { namespace ControllerDefine { 
    struct DeviceId { u32 _unknown; }; // placeholder, real type unknown
}}

namespace sead { namespace RuntimeTypeInfo { 
    class Interface;
    class Root;
    template <typename T0> class Derive;
}}

namespace sead { namespace hostio { 
    class ICurve;
    class LifeCheckable;
    class Node;
    class NodeEventListener;
    class PaletteEventListener;
    class PropertyEventListener;
    class Reflexible;
    template <typename T0> class Curve;
}}

namespace sead { namespace ptcl { 
    class EmitterCalc;
    class EmitterComplexCalc;
    class EmitterInstance;
    class EmitterSet;
    class EmitterSimpleCalc;
    class EmitterSimpleGPUCalc;
    class GpuPtcle;
    class PtclEditor;
    class PtclEditorCamera;
    class PtclInstance;
    class PtclLoadCheck;
    class PtclModelMgr;
    class PtclPreview;
    class PtclProject;
    class PtclProjectEmitterSet;
    class PtclRandom;
    class PtclRenderer;
    class PtclResource;
    class PtclSystem;
    class TextureData;
    struct EmitterTblData { u32 _unknown; }; // placeholder, real type unknown
    struct Handle { u32 _unknown; }; // placeholder, real type unknown
    struct PtclStripe { u32 _unknown; }; // placeholder, real type unknown
    struct ResourceBind { u32 _unknown; }; // placeholder, real type unknown
    struct SimpleEmitterData { u32 _unknown; }; // placeholder, real type unknown
}}

namespace secretaryceremony { 
    class AcNpcSpSecretaryCeremonyHioNode;
    class ForeachCrackerActionFunc;
    class ForeachEmoticonActionFunc;
    class ForeachReadyCrackerActionFunc;
}

namespace secretarycmps { 
    class AcNpcSpSecretaryCompassHioNode;
}

namespace secretaryev { 
    class AcNpcSpSecretaryEventHioNode;
}

namespace secretarygpev { 
    class AcNpcSpSecretaryGpEventHioNode;
}

namespace secretaryout { 
    class AcNpcSpSecretaryOutHioNode;
    class TurnOutFunction;
}

namespace secretaryt { 
    class AcNpcSpSecretaryTutorialHioNode;
}

namespace secretarytwp { 
    class AcNpcSpSecretaryTutorialWPHioNode;
}

namespace secretaryupdatetrain { 
    class TrainSearchBullheadFunction;
}

namespace seiichi { 
    class AcNpcSpSeiichiHioNode;
}

namespace shopcamp { 
    class AcNpcSpShopCampHioNode;
}

namespace shopcatherine { 
    class AcNpcSpShopCatherineHioNode;
}

namespace shopfollower { 
    class ShopFollowerAStarForeachCB;
    class ShopFollowerAStarForeachCBRoadOnly;
    class ShopFollowerRoadSearchAgent;
    class VisibleNpcPosGetCB;
}

namespace shopfuko { 
    class AcNpcSpShopFukoHioNode;
}

namespace shopgardening { 
    class AcNpcSpShopGardeningHioNode;
}

namespace shopgrace { 
    class AcNpcSpShopGraceHioNode;
}

namespace shopisland { 
    class AcNpcSpShopIslandHioNode;
}

namespace shopkate { 
    class AcNpcSpShopKateHioNode;
}

namespace shopkinuyo { 
    class AcNpcSpShopKinuyoHioNode;
}

namespace shopkkone { 
    class AcNpcSpKappeisKidOneHioNode;
}

namespace shoprecycle { 
    class AcNpcSpShopRecycleHioNode;
    class GetRemakeFunction;
}

namespace shopremake { 
    class AcNpcSpShopRemakeHioNode;
    class SearchRecycleFunction;
}

namespace shopshoe { 
    class AcNpcSpShopShoesHioNode;
}

namespace shoptanukichi { 
    class AcNpcSpShopTanukichiHioNode;
}

namespace shoptunekichi { 
    class AcNpcSpShopTunekichiHioNode;
}

namespace sisyo { 
    class AcNpcSpSisyoHioNode;
}

namespace ssys { namespace co { 
    class FaderBase;
}}

namespace ssys { namespace ma { 
    class Allocator;
    class FlushCache;
    class HeapAllocator;
    class LoadSplit;
    class LoadSplitMgr;
    class Vec3i;
    class VramExpHeap;
    class VramHeap;
    class VramMemBlock;
    class VramMemList;
    struct Vec3 { u32 _unknown; }; // placeholder, real type unknown
}}

namespace ssys { namespace ma { namespace lyt { 
    class Animation;
    class ArcResAcc;
    class ArcResAccReader;
    class ArcResourceAccessorVRAM;
    class Base2D;
    class DummyLayout;
    class FrameCtrl;
    class Layout;
    class LayoutList;
    class LayoutMgr;
    class ResAccInterface;
    class TexBufList;
    class TexBufListNode;
}}}

namespace ssys { namespace st { 
    class List;
    class ListNode;
    class ListNodePriorityBase;
    class ListPriority;
}}

namespace stage { 
    class TransitionMgr;
    struct FieldName { u32 _unknown; }; // placeholder, real type unknown
    struct Kind { u32 _unknown; }; // placeholder, real type unknown
    struct Name { u32 _unknown; }; // placeholder, real type unknown
}

namespace state { 
    template <typename T0, typename T1> class Step;
    template <typename T0> class Base;
    template <typename T0> class Mode;
}

namespace strcbld { 
    class Checker;
}

namespace svfscnve { 
    class UomasaForeachFunction;
}

namespace sys { 
    class LineManager;
    class TextManager;
}

namespace time { 
    class TimeHioNode;
}

namespace totakeke { 
    class AcNpcSpTotakekeHioNode;
}

namespace tsunekichiev { 
    class AcNpcSpTunekichiEventHioNode;
}

namespace ugc { 
    class Mgr;
}

namespace uomasa { 
    class AcNpcSpUomasaHioNode;
}

namespace updateselect { 
    class PNameRecept;
}

namespace ut { 
    class FrameController;
}

namespace yutarouvisit { 
    class AcNpcSpYutarouVisitHioNode;
}
