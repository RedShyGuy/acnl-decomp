#include "Other/dProcMgr.h"

// the constants of the process classes (names after the process table 0x00954FD0 of the binary)
// 0x00887E32
extern const ProcName PROC_SC_STAGE = static_cast<ProcName>(1);
// 0x00887E34
extern const ProcName PROC_SC_STAGE_MINI_GAME_0 = static_cast<ProcName>(2);
// 0x00887E36
extern const ProcName PROC_SC_STAGE_MINI_GAME_1 = static_cast<ProcName>(3);
// 0x00887E38
extern const ProcName PROC_BS_MINI_GAME_0_BG = static_cast<ProcName>(4);
// 0x00887E3A
extern const ProcName PROC_BS_MINI_GAME_0_MGR = static_cast<ProcName>(5);
// 0x00887ED8
extern const ProcName PROC_BS_MINI_GAME_1_MGR = static_cast<ProcName>(84);
// 0x00887EDA
extern const ProcName PROC_BS_AUDIO_LOAD_SIGNAL = static_cast<ProcName>(85);
// 0x00887EDE
extern const ProcName PROC_BS_OUTDOOR_VIEW_MGR = static_cast<ProcName>(87);
// 0x00887EE0
extern const ProcName PROC_BS_BUOY_MGR = static_cast<ProcName>(88);
// 0x00887EE2
extern const ProcName PROC_BS_INDOOR_VIEW_MGR = static_cast<ProcName>(89);
// 0x00887EE4
extern const ProcName PROC_BS_TOUR_FIELD_RES_MGR = static_cast<ProcName>(90);
// 0x00887EE6
extern const ProcName PROC_BS_INDOOR_PLATE_MGR = static_cast<ProcName>(91);
// 0x00887EE8
extern const ProcName PROC_BS_ART_PLATE_MGR = static_cast<ProcName>(92);
// 0x00887EEA
extern const ProcName PROC_BS_FOSSIL_PLATE_MGR = static_cast<ProcName>(93);
// 0x00887EF2
extern const ProcName PROC_BS_FORTUNE_VIEW_MGR = static_cast<ProcName>(97);
// 0x00887EF4
extern const ProcName PROC_BS_CLUB_444_VIEW_MGR = static_cast<ProcName>(98);
// 0x00887EFA
extern const ProcName PROC_BS_CHAR_ROOM_VIEW_MGR = static_cast<ProcName>(101);
// 0x00887EFC
extern const ProcName PROC_BS_TOUR_MGR = static_cast<ProcName>(102);
// 0x00887EFE
extern const ProcName PROC_BS_BG_SE_MGR = static_cast<ProcName>(103);
// 0x00887F00
extern const ProcName PROC_BS_EVENT_MGR = static_cast<ProcName>(104);
// 0x00887F04
extern const ProcName PROC_BS_PLAYER_MGR = static_cast<ProcName>(106);
// 0x00887F06
extern const ProcName PROC_BS_INDOOR_DOOR_SE = static_cast<ProcName>(107);
// 0x00887F08
extern const ProcName PROC_BS_STRC_MGR = static_cast<ProcName>(108);
// 0x00887F6E
extern const ProcName PROC_BS_MY_DESIGN_EDIT_MODEL = static_cast<ProcName>(159);
// 0x00887F70
extern const ProcName PROC_BS_STRC_CATALOG = static_cast<ProcName>(160);
// 0x00887F72
extern const ProcName PROC_BS_FLAG = static_cast<ProcName>(161);
// 0x00887F76
extern const ProcName PROC_BS_OBJ_CONTROL = static_cast<ProcName>(163);
// 0x00887F7C
extern const ProcName PROC_AC_ROBJ_TRAIN = static_cast<ProcName>(166);
// 0x00887F7E
extern const ProcName PROC_AC_ROBJ_COOLER_BOX = static_cast<ProcName>(167);
// 0x00887F80
extern const ProcName PROC_AC_BROCHURE_MSG = static_cast<ProcName>(168);
// 0x00888070
extern const ProcName PROC_AC_ROBJ_PROJECTOR = static_cast<ProcName>(172);
// 0x00888072
extern const ProcName PROC_AC_ROBJ_RAILWAY = static_cast<ProcName>(173);
// 0x00888074
extern const ProcName PROC_AC_ROBJ_GATE = static_cast<ProcName>(174);
// 0x00888076
extern const ProcName PROC_AC_ROBJ_SEWING_MACHINE = static_cast<ProcName>(175);
// 0x0088807A
extern const ProcName PROC_AC_ROBJ_SPEAKER = static_cast<ProcName>(177);
// 0x0088807E
extern const ProcName PROC_AC_ROBJ_POSTER = static_cast<ProcName>(179);
// 0x00888080
extern const ProcName PROC_AC_ROBJ_CATALOG_MACHINE = static_cast<ProcName>(180);
// 0x00888082
extern const ProcName PROC_AC_ROBJ_PERMA_MACHINE = static_cast<ProcName>(181);
// 0x00888084
extern const ProcName PROC_AC_ROBJ_RESET_CHAIR = static_cast<ProcName>(182);
// 0x00888086
extern const ProcName PROC_AC_ROBJ_RESET_TV = static_cast<ProcName>(183);
// 0x00888088
extern const ProcName PROC_AC_ROBJ_RESET_MACHINE = static_cast<ProcName>(184);
// 0x0088808A
extern const ProcName PROC_AC_ROBJ_LIVE_CHAIR = static_cast<ProcName>(185);
// 0x0088808C
extern const ProcName PROC_AC_ROBJ_PLAYER_SELECT = static_cast<ProcName>(186);
// 0x008880F4
extern const ProcName PROC_BS_FTR_MGR = static_cast<ProcName>(238);
// 0x008880F6
extern const ProcName PROC_BS_TOUR_BALLOON_MGR = static_cast<ProcName>(239);
// 0x008880F8
extern const ProcName PROC_BS_VILLAGE_BALLOON_MGR = static_cast<ProcName>(240);
// 0x008880FE
extern const ProcName PROC_BS_FOG_MGR = static_cast<ProcName>(243);
// 0x00888100
extern const ProcName PROC_BS_ESCAPE_FOG_MGR = static_cast<ProcName>(244);
// 0x00888102
extern const ProcName PROC_BS_LIGHT_MGR = static_cast<ProcName>(245);
// 0x00888134
extern const ProcName PROC_BS_AMIIBO_CAMERA = static_cast<ProcName>(271);
// 0x00888136
extern const ProcName PROC_BS_AMIIBO_PHOTO_MGR = static_cast<ProcName>(272);
// 0x00888138
extern const ProcName PROC_BS_MENU_AMIIBO_SHUTTER = static_cast<ProcName>(273);
// 0x0088813A
extern const ProcName PROC_BS_VRBOX_SKY = static_cast<ProcName>(274);
// 0x00888146
extern const ProcName PROC_BS_FIREWORK_MGR = static_cast<ProcName>(280);
// 0x00888148
extern const ProcName PROC_BS_SHOOTING_STAR_MGR = static_cast<ProcName>(281);
// 0x0088814A
extern const ProcName PROC_BS_THUNDER_MGR = static_cast<ProcName>(282);
// 0x0088814C
extern const ProcName PROC_BS_WEATHER_RAIN = static_cast<ProcName>(283);
// 0x0088814E
extern const ProcName PROC_BS_WEATHER_SNOW = static_cast<ProcName>(284);
// 0x00888150
extern const ProcName PROC_BS_WEATHER_SAKURA = static_cast<ProcName>(285);
// 0x00888152
extern const ProcName PROC_BS_WEATHER_PAPER = static_cast<ProcName>(286);
// 0x00888154
extern const ProcName PROC_BS_INSECT_FIELD_MGR = static_cast<ProcName>(287);
// 0x00888156
extern const ProcName PROC_BS_INSECT_MUSEUM_MGR = static_cast<ProcName>(288);
// 0x00888158
extern const ProcName PROC_BS_FISH_FIELD_MGR = static_cast<ProcName>(289);
// 0x0088815A
extern const ProcName PROC_BS_FISH_MUSEUM_MGR = static_cast<ProcName>(290);
// 0x0088815C
extern const ProcName PROC_BS_SEALIFE_MUSEUM_MGR = static_cast<ProcName>(291);
// 0x0088815E
extern const ProcName PROC_BS_SHOW_MGR = static_cast<ProcName>(292);
// 0x00888218
extern const ProcName PROC_BS_COCKROACH_MGR = static_cast<ProcName>(385);
// 0x0088821C
extern const ProcName PROC_BS_PIKO_HAN_OBJ_MGR = static_cast<ProcName>(387);
// 0x00888222
extern const ProcName PROC_BS_NPC_MGR = static_cast<ProcName>(390);
// 0x00888226
extern const ProcName PROC_AC_NPC_DEMO_DOLL_SP = static_cast<ProcName>(392);
// 0x00888240
extern const ProcName PROC_AC_NPC_SP_ASAMI = static_cast<ProcName>(406);
// 0x0088824A
extern const ProcName PROC_AC_NPC_SP_DJKK = static_cast<ProcName>(411);
// 0x0088824C
extern const ProcName PROC_AC_NPC_SP_DREAM = static_cast<ProcName>(412);
// 0x0088824E
extern const ProcName PROC_AC_NPC_SP_EXHIBITION_IN = static_cast<ProcName>(413);
// 0x00888252
extern const ProcName PROC_AC_NPC_SP_FORTUNETELLER = static_cast<ProcName>(415);
// 0x00888256
extern const ProcName PROC_AC_NPC_SP_FUTA = static_cast<ProcName>(417);
// 0x0088825A
extern const ProcName PROC_AC_NPC_SP_HONMA = static_cast<ProcName>(419);
// 0x0088825C
extern const ProcName PROC_AC_NPC_SP_KAPPEIS_KID_ONE = static_cast<ProcName>(420);
// 0x00888266
extern const ProcName PROC_AC_NPC_SP_RESETSAN = static_cast<ProcName>(425);
// 0x00888270
extern const ProcName PROC_AC_NPC_SP_KAPPEI = static_cast<ProcName>(430);
// 0x00888274
extern const ProcName PROC_AC_NPC_SP_KAMEYAMA_TENT = static_cast<ProcName>(432);
// 0x00888276
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_ANNOUNCE = static_cast<ProcName>(433);
// 0x00888278
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_HIDE_AND_SEEK = static_cast<ProcName>(434);
// 0x0088827A
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_FLOWER = static_cast<ProcName>(435);
// 0x0088827C
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_FISH = static_cast<ProcName>(436);
// 0x0088827E
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_INSECT = static_cast<ProcName>(437);
// 0x00888280
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_BALLOON = static_cast<ProcName>(438);
// 0x00888282
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_BORROWING_RACE = static_cast<ProcName>(439);
// 0x00888284
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_DIVING = static_cast<ProcName>(440);
// 0x00888286
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_FOSSIL_DIG = static_cast<ProcName>(441);
// 0x00888288
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_MAZE = static_cast<ProcName>(442);
// 0x0088828A
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_MINING = static_cast<ProcName>(443);
// 0x0088828C
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_PIKO_HAN = static_cast<ProcName>(444);
// 0x0088828E
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_TREASURE_HUNT = static_cast<ProcName>(445);
// 0x00888292
extern const ProcName PROC_AC_NPC_SP_MASTER = static_cast<ProcName>(447);
// 0x00888298
extern const ProcName PROC_AC_NPC_SP_PL_SELECT = static_cast<ProcName>(450);
// 0x008882A0
extern const ProcName PROC_AC_NPC_SP_SECRETARY_IN = static_cast<ProcName>(454);
// 0x008882B2
extern const ProcName PROC_AC_NPC_SP_SISYO = static_cast<ProcName>(464);
// 0x008882B4
extern const ProcName PROC_AC_NPC_SP_STATION_MASTER = static_cast<ProcName>(465);
// 0x008882B6
extern const ProcName PROC_AC_NPC_SP_TOTAKEKE = static_cast<ProcName>(466);
// 0x008882B8
extern const ProcName PROC_AC_NPC_SP_TRAIN = static_cast<ProcName>(467);
// 0x008882C2
extern const ProcName PROC_AC_NPC_SP_PERIKO = static_cast<ProcName>(472);
// 0x008882C4
extern const ProcName PROC_AC_NPC_SP_PERIMI = static_cast<ProcName>(473);
// 0x008882CE
extern const ProcName PROC_AC_NPC_SP_UOMASA_TENT = static_cast<ProcName>(478);
// 0x008882D0
extern const ProcName PROC_AC_NPC_SP_SHOP_CATHERINE = static_cast<ProcName>(479);
// 0x008882D2
extern const ProcName PROC_AC_NPC_SP_SHOP_CAMP = static_cast<ProcName>(480);
// 0x008882D4
extern const ProcName PROC_AC_NPC_SP_SHOP_FUKO = static_cast<ProcName>(481);
// 0x008882D6
extern const ProcName PROC_AC_NPC_SP_SHOP_GRACE = static_cast<ProcName>(482);
// 0x008882D8
extern const ProcName PROC_AC_NPC_SP_SHOP_ISLAND = static_cast<ProcName>(483);
// 0x008882DA
extern const ProcName PROC_AC_NPC_SP_SHOP_KINUYO = static_cast<ProcName>(484);
// 0x008882DC
extern const ProcName PROC_AC_NPC_SP_SHOP_KATE = static_cast<ProcName>(485);
// 0x008882DE
extern const ProcName PROC_AC_NPC_SP_SHOP_GARDENING = static_cast<ProcName>(486);
// 0x008882E0
extern const ProcName PROC_AC_NPC_SP_SHOP_POLICE_A = static_cast<ProcName>(487);
// 0x008882E2
extern const ProcName PROC_AC_NPC_SP_SHOP_POLICE_B = static_cast<ProcName>(488);
// 0x008882E4
extern const ProcName PROC_AC_NPC_SP_SHOP_RECYCLE = static_cast<ProcName>(489);
// 0x008882E6
extern const ProcName PROC_AC_NPC_SP_SHOP_REMAKE = static_cast<ProcName>(490);
// 0x008882E8
extern const ProcName PROC_AC_NPC_SP_SHOP_SHOES = static_cast<ProcName>(491);
// 0x008882EA
extern const ProcName PROC_AC_NPC_SP_SHOP_TANUKICHI = static_cast<ProcName>(492);
// 0x008882EC
extern const ProcName PROC_AC_NPC_SP_SHOP_TUBUKICHI = static_cast<ProcName>(493);
// 0x008882EE
extern const ProcName PROC_AC_NPC_SP_SHOP_MAMEKICHI = static_cast<ProcName>(494);
// 0x008882F0
extern const ProcName PROC_AC_NPC_SP_SHOP_TUNEKICHI = static_cast<ProcName>(495);
// 0x008882F4
extern const ProcName PROC_AC_NPC_SP_TOUR_DESK = static_cast<ProcName>(497);
// 0x008882FA
extern const ProcName PROC_AC_NPC_SP_TAKUMI_REAL_ESTATE = static_cast<ProcName>(500);
// 0x008882FC
extern const ProcName PROC_AC_NPC_SP_MANUAL_IMPORT = static_cast<ProcName>(501);
// 0x008882FE
extern const ProcName PROC_AC_NPC_SP_SECRETARY_TAKUMI_IMPORT = static_cast<ProcName>(502);
// 0x00888300
extern const ProcName PROC_AC_NPC_SP_SENIOR_TAKUMI_IMPORT = static_cast<ProcName>(503);
// 0x00888302
extern const ProcName PROC_AC_NPC_SP_SECRETARY_UPDATE_TRAIN = static_cast<ProcName>(504);
// 0x00888306
extern const ProcName PROC_AC_NPC_SP_EXCHANGE_MASTER = static_cast<ProcName>(506);
// 0x00888308
extern const ProcName PROC_AC_NPC_SP_EXCHANGE_RESET = static_cast<ProcName>(507);
// 0x00888310
extern const ProcName PROC_AC_NPC_AUTO_CAMP_OUT = static_cast<ProcName>(511);
// 0x00888312
extern const ProcName PROC_BS_AUTO_CAMP_BIRD_MGR = static_cast<ProcName>(512);
// 0x00888318
extern const ProcName PROC_BS_SNOW_BALL_MGR = static_cast<ProcName>(515);
// 0x00888320
extern const ProcName PROC_AC_PHOTO_MGR = static_cast<ProcName>(519);
// 0x00888322
extern const ProcName PROC_BS_GROW_UP = static_cast<ProcName>(520);
// 0x00888324
extern const ProcName PROC_BS_CFL = static_cast<ProcName>(521);
// 0x00888328
extern const ProcName PROC_SCRIPT_BASE = static_cast<ProcName>(523);
// 0x0088832A
extern const ProcName PROC_FONT_BASE = static_cast<ProcName>(524);
// 0x0088832C
extern const ProcName PROC_PHOTO_BS_INVOKE = static_cast<ProcName>(525);
// 0x0088832E
extern const ProcName PROC_BS_CAMERA = static_cast<ProcName>(526);
// 0x00888330
extern const ProcName PROC_BS_SHADOW_VIEW_MGR = static_cast<ProcName>(527);
// 0x00888334
extern const ProcName PROC_BS_OBJ_CHECK_MGR = static_cast<ProcName>(529);
// 0x00888336
extern const ProcName PROC_BS_TITLE = static_cast<ProcName>(530);
// 0x0088833A
extern const ProcName PROC_BS_SV_MGR = static_cast<ProcName>(532);
// 0x00888364
extern const ProcName PROC_BS_TIME_BEL_WINDOW = static_cast<ProcName>(553);
// 0x00888380
extern const ProcName PROC_BS_MENU_LETTER = static_cast<ProcName>(567);
// 0x0088838C
extern const ProcName PROC_BS_MENU_BULLETIN_BOARD = static_cast<ProcName>(573);
// 0x00888392
extern const ProcName PROC_BS_BALLOON_CHAT = static_cast<ProcName>(576);
// 0x00888394
extern const ProcName PROC_BS_TELOP = static_cast<ProcName>(577);
// 0x008883D8
extern const ProcName PROC_BS_COMMON_DIALOG = static_cast<ProcName>(611);
// 0x008883E2
extern const ProcName PROC_BS_MENU_AMIIBO_PHOTO = static_cast<ProcName>(616);
// 0x008883E6
extern const ProcName PROC_BS_SV_DEMO = static_cast<ProcName>(621);
// 0x008883E8
extern const ProcName PROC_BS_HAND_ITEM = static_cast<ProcName>(622);
// 0x008883EA
extern const ProcName PROC_BS_FG_PROC = static_cast<ProcName>(623);
// 0x008883EE
extern const ProcName PROC_BS_GR_RENDERER = static_cast<ProcName>(625);
// 0x008883F0
extern const ProcName PROC_BS_RETURN_SAVE_MGR = static_cast<ProcName>(626);
// 0x008883F4
extern const ProcName PROC_BS_DEMO_CANCEL_MGR = static_cast<ProcName>(628);
// 0x008883F6
extern const ProcName PROC_BS_INCLUDE_SAVE = static_cast<ProcName>(629);
// 0x008883F8
extern const ProcName PROC_BS_UPDATE_SELECT = static_cast<ProcName>(630);
// 0x008883FA
extern const ProcName PROC_BS_TAKUMI_FLOW_LOADER = static_cast<ProcName>(631);
// 0x008883FC
extern const ProcName PROC_BS_SEAD_PARTICLE_MGR = static_cast<ProcName>(632);
// 0x00888408
extern const ProcName PROC_BS_MODEL_EFFECT_MGR = static_cast<ProcName>(638);
