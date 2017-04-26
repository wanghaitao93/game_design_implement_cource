#ifndef _Missile_H_
#define _Missile_H_

#include "cocos2d.h"
//#include "EnemyPlane.h"
using namespace cocos2d;
using namespace std;

class Missile:public Sprite3D
{
public :
	Missile();
	Missile(Node *enemyPlane,Layer *layer,int i,float radio);
	static Missile* createWebMissile(float pointX,float pointY,float pointZ,
				float angleX,float angleY,float angleZ);
	Node *enemyPlane;					//敌机的节点
	Layer *layer;
	Point missilePoint;					//导弹位置
	Point goalPoint;					//目标位置

	float timeCounter=0;				//计数器
	bool canMove=true;					//导弹是否有目标
	float radio=0;
	float speed=16;						//导弹的速度

	int liveFlag=0;
	Sprite3D *missileM;
	ParticleSystemQuad* psqMissileM;
	static vector<Sprite3D*> missileVec;
//	//---------------------------------------
	Sprite3D *missile;
	Animate3D *animate;
	void MissileMove();				//移动导弹
	void shootEnemyPlane();			//设计敌机
	void move();
	void updateShoot();
	void vanish();					//导弹消失
	void createMissile();			//创建导弹和导弹的粒子系统
	void getNewEnemyPlane();		//获取新的敌机引用
//	CC_SYNTHESIZE(EnemyPlane*, _target, Target);
	int positionZ=0;
	int direction=0;
//	//导弹火焰
	ParticleSystemQuad* psqMissile;

	//抛物线的
	float a=0;
};

#endif
