#ifndef _Constant_H_
#define _Constant_H_
#include "cocos2d.h"
using namespace cocos2d;
class Constant{
public:

	//LoadingScene
	static bool soundLoaded;
	static bool particleLoaded;
	static bool soundFlag;

	//孙策============
	static int planeNumber;
	static float plane002X;
	static float plane002Y;
	static float plane002Z;
	static bool touchFlag;

	static int radiusData[14];
	static int scaleData[14];
	static int hurtData[4];
	static int bulletHurt;
	static int missileHurt;
//	static int bossBlood[3];

//	static float bulletNum;
	static bool isPause;
	//end=============

	//-----------------------------------------------------LI
	static int attackGrade[3];
	static int bloodGrade[3];
	static int bulletsGrade[3];
	static int goldGrade[3];
	static int attackGradePre[3];
	static int bloodGradePre[3];
	static int bulletsGradePre[3];
	static int goldGradePre[3];
	static int score;
	static int money;
	//-----------------------------------------------------LI
	//王
	static std::string title[3];
	static std::string titleC[3];
	static std::string content[3];
	static std::string planeName[3];
	static std::string planeContent[3];
	static int custom;			//关卡  0,1,2
	static int canPlay;			//0,1,2
	static int planeId;			//飞机编号 0,1,2
	static int canPlane;		//能玩的玩家
	static bool loaded;			//是否已经加载完
	static bool acceler;		//是否开启重力感应
	static int planeHealth;		//飞机的生命值
	static int sensitivity;		//重力感应的灵敏度
	static int planeSkill[3][3];		//飞机的性能
	static int bossSkill[3][3];		//飞机的性能
	//-------------------新加
	static int bulletLevel;									//子弹多的等级12		0,1,2,3,4
	static int bulletPower;									//子弹的威力	31		0,1,2,3,4
	static int missileLevel;								//导弹的等级	13		0,1,2,3
//	static int skillNum;									//大招的个数	13		0,1,2,3		最多3个
//	static int defenseNum;									//导弹的等级	13		0,1,2,3		做多3个
	static int attackCount;									//大招的个数	13		0,1,2,3		最多3个
	static int protectCount;								//导弹的等级	13		0,1,2,3		做多3个
	static int enemyBulletSpeed;
	static Point planePoint;
	static bool isBulletPower;								//是否有子弹的威力加成
	static int bulletPowerTime;								//子弹威力的时间条		一次1000
	static bool openCustom;									//开启关卡
	static bool gameingFlag;
};
#endif
