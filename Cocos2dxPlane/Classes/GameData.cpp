#include "GameData.h"

const int GameData::SEND_CONNECT=1;					//发送游戏连接信息
const int GameData::RECEIVE_OK=2;					//接收链接成功信息
const int GameData::RECEIVE_FULL=3;					//接收服务器满信息
const int GameData::SEND_LEVEL=4;					//发送关卡切换信息
const int GameData::RECEIVE_LEVEL=5;				//接收关卡切换信息
const int GameData::SEND_SELECT=6;					//发送关卡确定信息，并进入下一界面
const int GameData::RECEIVE_SELECT=7;				//接收关卡确定信息
const int GameData::SEND_PLANE=8;					//发送飞机选择信息
const int GameData::RECEIVE_PLANE=9;				//接收飞机选择信息
const int GameData::SEND_START=10;					//发送开始游戏信息
const int GameData::RECEIVE_START=11;				//接收开始游戏信息
const int GameData::SEND_KEY=12;					//发送按键信息
const int GameData::SEND_STATE=13;					//发送游戏状态信息
const int GameData::RECEIVE_STATE=14;				//接收游戏状态信息
const int GameData::SEND_EXIT=15;					//发送退出信息
const int GameData::RECEIVE_COUNT=16;				//接收服务器数量信息
const int GameData::RECEIVE_EXIT=17;				//发送游戏退出信息

const int GameData::RECEIVE_EXPLOSION=18;			//接收爆炸位置数据
const int GameData::RECEIVE_MAINBULLET=19;			//接收玩家飞机子弹数据
const int GameData::RECEIVE_PLANEDATA=20;			//接收玩家飞机子弹数据
const int GameData::RECEIVE_ENEMYBULLET=21;			//接收敌机子弹数据
const int GameData::RECEIVE_AWARD=22;				//接收奖励数据
const int GameData::RECEIVE_ENEMYPLANE=23;			//接收敌机数据
const int GameData::RECEIVE_BOSSFLAG=24;			//接收boss出现数据
const int GameData::SEND_PLANEBEGAN=25;				//发送触摸开始飞机监听
const int GameData::SEND_PLANEMOVE=26;				//发送触摸移动监听
const int GameData::RECEIVE_PLANEMOVE=27;			//接收飞机目标点
const int GameData::RECEIVE_PLANECANMOVE=28;		//接收飞机是否可以移动
const int GameData::SEND_STARTACTION=29;			//发送开始状态
const int GameData::RECEIVE_MISSILEDATA=30;			//接收导弹数据
const int GameData::SEND_CONTINUE=31;				//发送继续游戏状态
const int GameData::RECEIVE_CANLEVEL=32;			//开启新的关卡的信息
const int GameData::SEND_USESKILL=33;				//使用技能
const int GameData::RECEIVE_USESKILL=34;			//接收技能

const char* GameData::gameData[]=
{
	"SEND_CONNECT","RECEIVE_OK","RECEIVE_FULL","SEND_LEVEL",
	"RECEIVE_LEVEL","SEND_SELECT","RECEIVE_SELECT","SEND_PLANE",
	"RECEIVE_PLANE","SEND_START","RECEIVE_START","SEND_KEY",
	"SEND_STATE","RECEIVE_STATE","SEND_EXIT","RECEIVE_COUNT",
	"RECEIVE_EXIT",

	"RECEIVE_EXPLOSION","RECEIVE_MAINBULLET","RECEIVE_PLANEDATA",
	"RECEIVE_ENEMYBULLET","RECEIVE_AWARD","RECEIVE_ENEMYPLANE",

	"RECEIVE_BOSSFLAG","SEND_PLANEBEGAN","SEND_PLANEMOVE",
	"RECEIVE_PALNEMOVE","RECEIVE_PLANECANMOVE","SEND_STARTACTION",
	"RECEIVE_MISSILEDATA","SEND_CONTINUE","RECEIVE_CANLEVEL","SEND_USESKILL","RECEIVE_USESKILL"
};												//接收消息数据

bool GameData::isPause=false;			//暂停
bool GameData::serviceFull=false;
int GameData::level=0;
int GameData::oldLevel=0;
bool GameData::SelectedGame=false;
int GameData::playerNumber=-1;					//0-- wanjia ,1--房主
int GameData::housePlaneId=0;					//房主飞机Id
int GameData::playerPlaneId=0;					//玩家飞机Id
int GameData::oldHousePlaneId=0;				//房主飞机Id
int GameData::oldPlayerPlaneId=0;				//玩家飞机Id
int GameData::playerCount=0;
bool GameData::connectFlag=false;				//连接
int GameData::housePrepare=0;					//房主的准备情况  1--已准备
int GameData::playerPrepare=0;					//玩家的准备情况  1--已准备
int GameData::canPlay=0;						//玩家能玩关卡的选择
int GameData::redAttackCount=0;
int GameData::greenAttackCount=0;
int GameData::redProtectCount=0;
int GameData::greenProtectCount=0;
bool GameData::isRedAttack=false;			//红色飞机的大招
bool GameData::isGreenAttack=false;			//绿色飞机的大招
bool GameData::isRedProtect=false;			//红色飞机的保护
bool GameData::isGreenProtect=false;			//绿色飞机的保护

//----------LI----------------
std::vector<float> GameData::mainBulletVec;		//玩家子弹数据容器
std::vector<float> GameData::enemyBulletVec;	//敌机子弹数据容器
std::vector<float> GameData::enemyVec;			//敌机数据容器
std::vector<float> GameData::explosionVec;		//爆炸数据容器
std::vector<float> GameData::awardVec;			//奖励数据容器
std::vector<float> GameData::planeData;			//飞机数据容器
std::vector<float> GameData::missileVec;		//飞机数据容器

//数据处理标志位
bool GameData::explosionFlag=true;				//爆炸数据处理标志位
bool GameData::mainBulletFlag=true;				//玩家子弹数据处理标志位
bool GameData::planeDataFlag=true;				//玩家飞机数据处理标志位
bool GameData::enemyBulletFlag=true;			//敌机子弹数据处理标志位
bool GameData::awardFlag=true;					//奖励数据处理标志位
bool GameData::enemyFlag=true;					//敌机数据处理标志位
bool GameData::missileFlag=true;				//导弹数据处理标志位
int GameData::gameState=0;						//游戏标志位

const int GameData::GAME_WAIT=0;
const int GameData::GAME_START=1;
const int GameData::GAME_PAUSE=2;
const int GameData::GAME_WIN=3;
const int GameData::GAME_LOST=4;
//-------------------WANG
//游戏2D界面
int GameData::oldSelfScore=0;				//老的自己的分数
int GameData::totalScore=0;					//总分
int GameData::money=0;						//钱
float GameData::oldPlaneHealth=200;			//自己的老的血量
float GameData::redPlaneHealth=200;			//红色血量
float GameData::greenPlaneHealth=200;		//绿色飞机的血量
//3D
bool GameData::isPlaneCanMove=false;		//飞机是否可以移动
bool GameData::isChangePlaneMove=false;		//飞机是否可以移动
bool GameData::isRedLift=true;				//红色飞机是否活着
bool GameData::isGreenLift=true;			//绿色飞机是否活着

void GameData::reset()						//初始化数据
{
	isPause=false;
	level=0;
	oldLevel=0;
	SelectedGame=false;
	housePlaneId=0;
	playerPlaneId=0;
	oldHousePlaneId=0;
	oldPlayerPlaneId=0;
	housePrepare=0;
	playerPrepare=0;
	explosionFlag=true;
	mainBulletFlag=true;
	planeDataFlag=true;
	enemyBulletFlag=true;
	awardFlag=true;
	enemyFlag=true;
	missileFlag=true;
	gameState=0;
	oldSelfScore=0;
	totalScore=0;
	money=0;
	oldPlaneHealth=200;
	redPlaneHealth=200;
	greenPlaneHealth=200;
	isPlaneCanMove=false;
	isChangePlaneMove=false;
	isRedLift=true;
	isGreenLift=true;

	mainBulletVec.clear();
	enemyBulletVec.clear();
	enemyVec.clear();
	explosionVec.clear();
	awardVec.clear();
	planeData.clear();
	missileVec.clear();
}






