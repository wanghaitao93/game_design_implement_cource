#ifndef  _GameData_H_
#define  _GameData_H_

#include "cocos2d.h"

class GameData
{
public:
	static const int SEND_CONNECT;				//发送游戏连接信息
	static const int RECEIVE_OK;				//接收链接成功信息
	static const int RECEIVE_FULL;				//接收服务器满信息
	static const int SEND_LEVEL;				//发送关卡切换信息
	static const int RECEIVE_LEVEL;				//接收关卡切换信息
	static const int SEND_SELECT;				//发送关卡确定信息，并进入下一界面
	static const int RECEIVE_SELECT;			//接收关卡确定信息
	static const int SEND_PLANE;				//发送飞机选择信息
	static const int RECEIVE_PLANE;				//接收飞机选择信息
	static const int SEND_START;				//发送开始游戏信息
	static const int RECEIVE_START;				//接收开始游戏信息
	static const int SEND_KEY;					//发送按键信息
	static const int SEND_STATE;				//发送游戏状态信息
	static const int RECEIVE_STATE;				//接收游戏状态信息
	static const int SEND_EXIT;					//发送退出信息
	static const int RECEIVE_COUNT;				//接收服务器数量信息
	static const int RECEIVE_EXIT;				//接收游戏退出信息


	static const int RECEIVE_EXPLOSION;			//接收爆炸位置数据
	static const int RECEIVE_MAINBULLET;		//接收玩家飞机子弹数据
	static const int RECEIVE_PLANEDATA;			//接收玩家飞机子弹数据
	static const int RECEIVE_ENEMYBULLET;		//接收敌机子弹数据
	static const int RECEIVE_AWARD;				//接收奖励数据
	static const int RECEIVE_ENEMYPLANE;		//接收敌机数据
	static const int RECEIVE_BOSSFLAG;			//接收boss出现数据

	static const int SEND_PLANEBEGAN;			//发送点击屏幕监听
	static const int SEND_PLANEMOVE;			//发送触摸移动坐标
	static const int RECEIVE_PLANEMOVE;			//接收飞机移动的目标点以及自身角度
	static const int RECEIVE_PLANECANMOVE;		//接收飞机是否可以移动
	static const int SEND_STARTACTION;			//发送飞机的开始状态
	static const int RECEIVE_MISSILEDATA;		//发送导弹数据

	static const int SEND_CONTINUE;				//发送继续状态
	static const int RECEIVE_CANLEVEL;			//接收能玩关卡的限制
	static const int SEND_USESKILL;				//使用技能
	static const int RECEIVE_USESKILL;			//接收技能

	static const char* gameData[];

	static bool isPause;
	static bool serviceFull;
	static int level;
	static int oldLevel;
	static bool SelectedGame;			//是否选择完关卡
	static int playerNumber;			//0--客人，1--房主
	static int playerCount;
	static bool connectFlag;
	static int playerPlaneId;			//0-- wanjia ,1--房主
	static int housePlaneId;			//房主飞机Id
	static int oldPlayerPlaneId;		//0-- wanjia ,1--房主
	static int oldHousePlaneId;			//房主飞机Id
	static int housePrepare;			//房主准备的情况
	static int playerPrepare;			//玩家准备的情况
	static int canPlay;					//能玩的最多的关卡
	static int redAttackCount;
	static int greenAttackCount;
	static int redProtectCount;
	static int greenProtectCount;
	static bool isRedAttack;			//红色飞机的大招
	static bool isGreenAttack;			//绿色飞机的大招
	static bool isRedProtect;			//红色飞机的保护
	static bool isGreenProtect;			//绿色飞机的保护

	//----------LI----------------
	static std::vector<float> mainBulletVec;		//玩家子弹数据容器
	static std::vector<float> enemyBulletVec;		//敌机子弹数据容器
	static std::vector<float> enemyVec;				//敌机数据容器
	static std::vector<float> explosionVec;			//爆炸数据容器
	static std::vector<float> awardVec;				//奖励数据容器
	static std::vector<float > missileVec;			//导弹数据容器

	//数据处理标志位      0未接收   1已接受未处理   2处理中
	static bool explosionFlag;						//爆炸数据处理标志位
	static bool mainBulletFlag;						//玩家子弹数据处理标志位
	static bool planeDataFlag;						//玩家飞机数据处理标志位
	static bool enemyBulletFlag;					//敌机子弹数据处理标志位
	static bool awardFlag;							//奖励数据处理标志位
	static bool enemyFlag;							//敌机数据处理标志位
	static bool missileFlag;						//导弹数据处理标志位

	static int gameState;							//游戏标志位
	static int whoPause;							//是谁暂停的

	static const int GAME_WAIT;
	static const int GAME_START;
	static const int GAME_PAUSE;
	static const int GAME_WIN;
	static const int GAME_LOST;

	// 游戏2D界面
	static int oldSelfScore;			//老的分数
	static int totalScore;				//总分
	static int money;					//金钱
	static float oldPlaneHealth;		//自己的血量
	static float redPlaneHealth;		//玩家飞机的血量
	static float greenPlaneHealth;		//房主玩家的血量
	//游戏3D界面
	static float playerPlaneX;			//玩家飞机的X坐标
	static float playerPlaneY;			//玩家飞机的Y坐标
	static float housePlaneX;			//房主飞机的X坐标
	static float housePlaneY;			//房主飞机的Y坐标

	static bool isPlaneCanMove;			//是否飞机能移动
	static bool isChangePlaneMove;
	static bool isRedLift;
	static bool isGreenLift;

	static std::vector<float> planeData;

	static void reset();

};

#endif
