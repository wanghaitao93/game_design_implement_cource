#ifndef __AppMacros_H__
#define __AppMacros_H__

#include <string>
using namespace std;

#define MAP_PATH string("map/")
#define MENU_PATH string("menu/")
#define GAME_PATH string("game/")
#define SOUND_PATH string("sound/")
#define PLIST_PATH string("plist/")
#define FONT_PATH string("fonts/")
//#define CHOOSE_PATH string("choose/")
#define WEAPON_PATH string("weapon/")
#define SELECT_PATH string("selectplane/")
#define SELECTGAME_PATH string("selectgame/")
#define C3B_PATH string("c3b/")
#define HELP_PATH string("help/")
#define LOAD_PATH string("load/")
#define ANIM_PATH string("anim/")
#define OPEN_PATH string("anim/open/")
#define BOOMB_PATH string("anim/boomB/")
#define BOOMS_PATH string("anim/boomS/")
#define BOOMP_PATH string("anim/boomP/")
#define ANIMLEIDIAN_PATH string("anim/button/")
#define TITLE_PATH string("anim/planeTitle/")
#define PROTECT_PATH string("anim/protect/")
#define BOOM_PATH string("anim/boom/")
#define LEVEL_PATH string("anim/addLevel/")
#define CLOUD_PATH string("cloud/")
#define PAUSE_PATH string("pause/")
#define GAMEMODE_PATH string("gameMode/")
#define VIDEO_PATH string("video/")
#define SHADER_PATH string("shaders/")
#define NET_PATH string("net/")

#define BACKGROUND_LEVEL -10		//背景场景层次
#define PLIST_LEVEL 0
#define NORMAL_LEVEL 2
#define MENU_LEVEL 5				//菜单层次
#define SET_LEVEL 10
#define GAME_LEVEL 12
#define MISSILE_LEVEL 20
#define PI 3.1415926				//圆周率
#define AVE_BLOOD 16.66666666666667	//血条每份占有的百分比
//#define
#define PLANE_LEVEL 30			//飞机层次
//封装用于碰撞检测的矩形区域
const static Rect BOUND_RECT = Rect(-380,-60,760,180 );
const static Rect ENEMY_BOUND_RECT = Rect(-480,60,960,380 );

#endif
enum entityTypes
{
	kButtonGo,
	kMenuSure,
	kCancel,
	kMenuSelect,
	kMenuButton,
	kBack,

	kTransform,
    kPlane_go,
    kBoomS,
    kBoomB,
    kGetCoin,
    kWarning,

    kBullet,
    kEnemyFodderL,
    kEnemyBigDude,
    kEnemyBoss
};
