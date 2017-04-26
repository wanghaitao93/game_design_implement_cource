#ifndef __First3DLayer_H__
#define __First3DLayer_H__

#include "cocos2d.h"
#include "Missile.h"
#include "NewPlane.h"
#include "ControlGame.h"
#include "Explosion.h"
#include "Bullet.h"
#include "Coin.h"
#include "EnemyPlane.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
using namespace cocos2d;
using namespace ui;
using namespace cocos2d::extension;
#define N 5000000
class First3DLayer : public cocos2d::Layer
{
public:
	Camera *camera;			//摄像机
//	GameSceneManager *gsm;
	float degree=0;
	Missile *ms;			//导弹类
	NewPlane *plane;			//飞机类
	Bullet* b;
	Sprite3D* back1;
	Sprite3D* back2;
//	Sprite3D* upSpeed;
	EnemyPlane *enemyPlane[2];	//敌机
	MotionStreak *myStreak; 		//拖尾
	Sprite *progress;			//进度条的背景
//	LoadingBar *loadingbar;		// 子弹的进度条
	Coin *coin;
	ControlGame *controlGame;
	ControlBullet *controlBullet;
	ExManager *exManager;
	Animate *protectAnim;
	ControlPotentiometer *potentiometer;	//圆盘进度条
	float progressNum=500;					//倒计时
	BillBoard *temp;						//保护罩
	BillBoard *tempProgress;


	DirectionLight *directionLight;			//定向光
	bool attackFlag=false;
	bool attackAnimFlag=false;
	bool protectFlag=false;
	bool protectAnimFlag=false;
//-----------------------------
	static std::vector<Sprite3D*> enemyVec;
	static std::vector<BillBoard*> bulletVec;
	static std::vector<BillBoard*> explosionVec;
	static std::vector<Sprite3D*> enemyDel;
	static std::vector<BillBoard*> bulletDel;
	static std::vector<BillBoard*> explosionDel;
///-------------------------
	virtual bool init();
	void createPlane();		//创建第几关
	void updateCreateMissile(float f);
	void update(float f);
	void changeCamera(float x);
	void deletePlane();				//删除飞机的定时回调
	void createMissile();			//创建导弹
    void resetGame();				//重置游戏信息
    void attackAll();				//攻击所有敌机
	void attackAllFinish();			//攻击敌机技能结束
	void protectMyself();			//保护飞机
	void protectMyselfFinish();		//保护飞机技能结束
	void updateProgress();
	void startGame();				//开始游戏
	void clearAllBullets();			//清除所有子弹
    //获取世界坐标
    void getAnimPoint(Vec3 nearP,Vec3 farP);
	//飞机的触摸监听
	bool onMyTouchBegan(Touch* touch, Event* event);
	void onMyTouchMoved(Touch* touch, Event* event);
	void onMyTouchEnded(Touch* touch, Event* event);
	//重力感应
	void onMyAcceleration(Acceleration* acc, Event* unused_event);	//重力感应回调方法
    CREATE_FUNC(First3DLayer);

private:
    int planeId;			//飞机编号
    int gameLevel;			//游戏关卡
    int attackGrade;		//攻击力等级
    int bloodGrade;			//血量
    int bulletsGrade;		//子弹的攻击速度
    int goldGrade;			//金币加成
    float enemy2Num[N];				//敌机2号编号
    float enemy3Num[N];				//敌机3号编号
    void getProperty();				//获取游戏的属性
    void playerBullets(float dt);	//玩家飞机子弹回调方法
    Animate *boomAnim;				//大爆炸
//    void changePro();				//改变进度条
    bool touchFlag=false;			//触摸标志位
};

enum Complete
{
	loseEnum,winEnum
};

#endif
