#pragma once

#include "decomp.h"

namespace oml {
namespace framework {
class Process;
} // namespace framework
} // namespace oml

// the process classes of the game: the index into the tables of the ProcessManager (name after libgarden; the
// constants of the classes are variables of another file, see stage)
enum ProcName : u16
{
    PROC_NAME_BEGIN = 0,
    PROC_NAME_USED_COUNT = 0x27F,
    PROC_NAME_COUNT = 0x29D,
};

// the process classes (variables of another file; names after the process table of the binary)
extern const ProcName PROC_SC_STAGE;
extern const ProcName PROC_SC_STAGE_MINI_GAME_0;
extern const ProcName PROC_SC_STAGE_MINI_GAME_1;
extern const ProcName PROC_BS_MINI_GAME_0_BG;
extern const ProcName PROC_BS_MINI_GAME_0_MGR;
extern const ProcName PROC_BS_MINI_GAME_1_MGR;
extern const ProcName PROC_BS_AUDIO_LOAD_SIGNAL;
extern const ProcName PROC_BS_OUTDOOR_VIEW_MGR;
extern const ProcName PROC_BS_BUOY_MGR;
extern const ProcName PROC_BS_INDOOR_VIEW_MGR;
extern const ProcName PROC_BS_TOUR_FIELD_RES_MGR;
extern const ProcName PROC_BS_INDOOR_PLATE_MGR;
extern const ProcName PROC_BS_ART_PLATE_MGR;
extern const ProcName PROC_BS_FOSSIL_PLATE_MGR;
extern const ProcName PROC_BS_FORTUNE_VIEW_MGR;
extern const ProcName PROC_BS_CLUB_444_VIEW_MGR;
extern const ProcName PROC_BS_CHAR_ROOM_VIEW_MGR;
extern const ProcName PROC_BS_TOUR_MGR;
extern const ProcName PROC_BS_BG_SE_MGR;
extern const ProcName PROC_BS_EVENT_MGR;
extern const ProcName PROC_BS_PLAYER_MGR;
extern const ProcName PROC_BS_INDOOR_DOOR_SE;
extern const ProcName PROC_BS_STRC_MGR;
extern const ProcName PROC_BS_MY_DESIGN_EDIT_MODEL;
extern const ProcName PROC_BS_STRC_CATALOG;
extern const ProcName PROC_BS_FLAG;
extern const ProcName PROC_BS_OBJ_CONTROL;
extern const ProcName PROC_AC_ROBJ_TRAIN;
extern const ProcName PROC_AC_ROBJ_COOLER_BOX;
extern const ProcName PROC_AC_BROCHURE_MSG;
extern const ProcName PROC_AC_ROBJ_PROJECTOR;
extern const ProcName PROC_AC_ROBJ_RAILWAY;
extern const ProcName PROC_AC_ROBJ_GATE;
extern const ProcName PROC_AC_ROBJ_SEWING_MACHINE;
extern const ProcName PROC_AC_ROBJ_SPEAKER;
extern const ProcName PROC_AC_ROBJ_POSTER;
extern const ProcName PROC_AC_ROBJ_CATALOG_MACHINE;
extern const ProcName PROC_AC_ROBJ_PERMA_MACHINE;
extern const ProcName PROC_AC_ROBJ_RESET_CHAIR;
extern const ProcName PROC_AC_ROBJ_RESET_TV;
extern const ProcName PROC_AC_ROBJ_RESET_MACHINE;
extern const ProcName PROC_AC_ROBJ_LIVE_CHAIR;
extern const ProcName PROC_AC_ROBJ_PLAYER_SELECT;
extern const ProcName PROC_BS_FTR_MGR;
extern const ProcName PROC_BS_TOUR_BALLOON_MGR;
extern const ProcName PROC_BS_VILLAGE_BALLOON_MGR;
extern const ProcName PROC_BS_FOG_MGR;
extern const ProcName PROC_BS_ESCAPE_FOG_MGR;
extern const ProcName PROC_BS_LIGHT_MGR;
extern const ProcName PROC_BS_AMIIBO_CAMERA;
extern const ProcName PROC_BS_AMIIBO_PHOTO_MGR;
extern const ProcName PROC_BS_MENU_AMIIBO_SHUTTER;
extern const ProcName PROC_BS_VRBOX_SKY;
extern const ProcName PROC_BS_FIREWORK_MGR;
extern const ProcName PROC_BS_SHOOTING_STAR_MGR;
extern const ProcName PROC_BS_THUNDER_MGR;
extern const ProcName PROC_BS_WEATHER_RAIN;
extern const ProcName PROC_BS_WEATHER_SNOW;
extern const ProcName PROC_BS_WEATHER_SAKURA;
extern const ProcName PROC_BS_WEATHER_PAPER;
extern const ProcName PROC_BS_INSECT_FIELD_MGR;
extern const ProcName PROC_BS_INSECT_MUSEUM_MGR;
extern const ProcName PROC_BS_FISH_FIELD_MGR;
extern const ProcName PROC_BS_FISH_MUSEUM_MGR;
extern const ProcName PROC_BS_SEALIFE_MUSEUM_MGR;
extern const ProcName PROC_BS_SHOW_MGR;
extern const ProcName PROC_BS_COCKROACH_MGR;
extern const ProcName PROC_BS_PIKO_HAN_OBJ_MGR;
extern const ProcName PROC_BS_NPC_MGR;
extern const ProcName PROC_AC_NPC_DEMO_DOLL_SP;
extern const ProcName PROC_AC_NPC_SP_ASAMI;
extern const ProcName PROC_AC_NPC_SP_DJKK;
extern const ProcName PROC_AC_NPC_SP_DREAM;
extern const ProcName PROC_AC_NPC_SP_EXHIBITION_IN;
extern const ProcName PROC_AC_NPC_SP_FORTUNETELLER;
extern const ProcName PROC_AC_NPC_SP_FUTA;
extern const ProcName PROC_AC_NPC_SP_HONMA;
extern const ProcName PROC_AC_NPC_SP_KAPPEIS_KID_ONE;
extern const ProcName PROC_AC_NPC_SP_RESETSAN;
extern const ProcName PROC_AC_NPC_SP_KAPPEI;
extern const ProcName PROC_AC_NPC_SP_KAMEYAMA_TENT;
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_ANNOUNCE;
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_HIDE_AND_SEEK;
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_FLOWER;
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_FISH;
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_INSECT;
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_BALLOON;
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_BORROWING_RACE;
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_DIVING;
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_FOSSIL_DIG;
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_MAZE;
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_MINING;
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_PIKO_HAN;
extern const ProcName PROC_AC_NPC_SP_KOTOBUKI_TREASURE_HUNT;
extern const ProcName PROC_AC_NPC_SP_MASTER;
extern const ProcName PROC_AC_NPC_SP_PL_SELECT;
extern const ProcName PROC_AC_NPC_SP_SECRETARY_IN;
extern const ProcName PROC_AC_NPC_SP_SISYO;
extern const ProcName PROC_AC_NPC_SP_STATION_MASTER;
extern const ProcName PROC_AC_NPC_SP_TOTAKEKE;
extern const ProcName PROC_AC_NPC_SP_TRAIN;
extern const ProcName PROC_AC_NPC_SP_PERIKO;
extern const ProcName PROC_AC_NPC_SP_PERIMI;
extern const ProcName PROC_AC_NPC_SP_UOMASA_TENT;
extern const ProcName PROC_AC_NPC_SP_SHOP_CATHERINE;
extern const ProcName PROC_AC_NPC_SP_SHOP_CAMP;
extern const ProcName PROC_AC_NPC_SP_SHOP_FUKO;
extern const ProcName PROC_AC_NPC_SP_SHOP_GRACE;
extern const ProcName PROC_AC_NPC_SP_SHOP_ISLAND;
extern const ProcName PROC_AC_NPC_SP_SHOP_KINUYO;
extern const ProcName PROC_AC_NPC_SP_SHOP_KATE;
extern const ProcName PROC_AC_NPC_SP_SHOP_GARDENING;
extern const ProcName PROC_AC_NPC_SP_SHOP_POLICE_A;
extern const ProcName PROC_AC_NPC_SP_SHOP_POLICE_B;
extern const ProcName PROC_AC_NPC_SP_SHOP_RECYCLE;
extern const ProcName PROC_AC_NPC_SP_SHOP_REMAKE;
extern const ProcName PROC_AC_NPC_SP_SHOP_SHOES;
extern const ProcName PROC_AC_NPC_SP_SHOP_TANUKICHI;
extern const ProcName PROC_AC_NPC_SP_SHOP_TUBUKICHI;
extern const ProcName PROC_AC_NPC_SP_SHOP_MAMEKICHI;
extern const ProcName PROC_AC_NPC_SP_SHOP_TUNEKICHI;
extern const ProcName PROC_AC_NPC_SP_TOUR_DESK;
extern const ProcName PROC_AC_NPC_SP_TAKUMI_REAL_ESTATE;
extern const ProcName PROC_AC_NPC_SP_MANUAL_IMPORT;
extern const ProcName PROC_AC_NPC_SP_SECRETARY_TAKUMI_IMPORT;
extern const ProcName PROC_AC_NPC_SP_SENIOR_TAKUMI_IMPORT;
extern const ProcName PROC_AC_NPC_SP_SECRETARY_UPDATE_TRAIN;
extern const ProcName PROC_AC_NPC_SP_EXCHANGE_MASTER;
extern const ProcName PROC_AC_NPC_SP_EXCHANGE_RESET;
extern const ProcName PROC_AC_NPC_AUTO_CAMP_OUT;
extern const ProcName PROC_BS_AUTO_CAMP_BIRD_MGR;
extern const ProcName PROC_BS_SNOW_BALL_MGR;
extern const ProcName PROC_AC_PHOTO_MGR;
extern const ProcName PROC_BS_GROW_UP;
extern const ProcName PROC_BS_CFL;
extern const ProcName PROC_SCRIPT_BASE;
extern const ProcName PROC_FONT_BASE;
extern const ProcName PROC_PHOTO_BS_INVOKE;
extern const ProcName PROC_BS_CAMERA;
extern const ProcName PROC_BS_SHADOW_VIEW_MGR;
extern const ProcName PROC_BS_OBJ_CHECK_MGR;
extern const ProcName PROC_BS_TITLE;
extern const ProcName PROC_BS_SV_MGR;
extern const ProcName PROC_BS_TIME_BEL_WINDOW;
extern const ProcName PROC_BS_BALLOON_CHAT;
extern const ProcName PROC_BS_TELOP;
extern const ProcName PROC_BS_MENU_AMIIBO_PHOTO;
extern const ProcName PROC_BS_SV_DEMO;
extern const ProcName PROC_BS_HAND_ITEM;
extern const ProcName PROC_BS_FG_PROC;
extern const ProcName PROC_BS_GR_RENDERER;
extern const ProcName PROC_BS_RETURN_SAVE_MGR;
extern const ProcName PROC_BS_DEMO_CANCEL_MGR;
extern const ProcName PROC_BS_INCLUDE_SAVE;
extern const ProcName PROC_BS_UPDATE_SELECT;
extern const ProcName PROC_BS_TAKUMI_FLOW_LOADER;
extern const ProcName PROC_BS_SEAD_PARTICLE_MGR;
extern const ProcName PROC_BS_MODEL_EFFECT_MGR;

// the kind of a process (names after libgarden)
enum ProcType : u8
{
    PROC_TYPE_BASE = 0,
    PROC_TYPE_SCENE = 1,
    PROC_TYPE_ACTOR = 2,
};

class ProcMgr
{
public:
    // NULL for a process class out of range or a stopped parent
    static oml::framework::Process* Create(ProcName name, oml::framework::Process* parent, u32 arg, ProcType type); // 0x0052A32C (name after libgarden)
    void Create(ProcName, unsigned long, ProcType); // 0x0052A3B4 | libgarden [tier A]
};
