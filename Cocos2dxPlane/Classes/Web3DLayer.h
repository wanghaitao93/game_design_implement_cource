#ifndef __Web3DLayer_H__
#define __Web3DLayer_H__

#include "cocos2d.h"
#include "Bullet.h"
#include "EnemyPlane.h"
#include "Coin.h"
#include "Explosion.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"

using namespace cocos2d;
using namespace ui;
using namespace cocos2d::extension;

class Web3DLayer : public Layer
{
public:
	virtual bool init();
	void updateEnemyBullet(float dt);
	void updateMainBullet(float dt);
	void updateEnemy(float dt);
	void updateAward(float dt);
	void updateBackground(float dt);
	void updatePlane(float f);
	void updateMissile(float f);
	void startServer(float f);
	void startGame(float f);
	void createPlane();
	void deletePlane(int index);				//删除飞机
	void playBoom();
	//获取世界坐标
	void getAnimPoint(Vec3 nearP,Vec3 farP);
	//飞机的触摸监听
	bool onMyTouchBegan(Touch* touch, Event* event);
	void onMyTouchMoved(Touch* touch, Event* event);
	void onMyTouchEnded(Touch* touch, Event* event);
	//重力感应
	void onMyAcceleration(Acceleration* acc, Event* unused_event);	//重力感应回调方法
	CREATE_FUNC(Web3DLayer);

	Sprite3D *playerPlane;	//玩家飞机
	Sprite3D *housePlane;	//房主飞机
	Sprite3D *back1;
	Sprite3D *back2;
	Camera *camera;
	MotionStreak *myStreak; 		//拖尾
	ControlPotentiometer *potentiometer;	//圆盘进度条
	float progressNum=500;					//倒计时
	BillBoard *tempBoard1;						//保护罩
	BillBoard *tempBoard2;						//保护罩
	BillBoard *tempProgress;
	Sprite *progress;				//进度条的背景
	float playerPlaneX,playerPlaneY,housePlaneX,housePlaneY,playerAngleX,playerAngleY,playerAngleZ,
						houseAngleX,houseAngleY,houseAngleZ,redHealth,greenHealth,score,redAttackCount,greenAttackCount,redProtectCount,greenProtectCount;
	Animate *protectAnim;
	Animate *boomAnim;				//大爆炸
	bool touchFlag=false;			//触摸标志位
	Vec2 animPoint;					//目标点
	DirectionLight *directionLight;				//方向光

	ExManager *exManager;

	std::map<int, Bullet *> enemyBulletMap;
	std::map<int, Bullet *> mainBulletMap;
	std::map<int, EnemyPlane *> enemyMap;
	std::map<int, Award *> awardMap;
	std::map<int ,Missile *> missileMap;
};

#endif
