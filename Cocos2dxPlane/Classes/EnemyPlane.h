#ifndef __ENEMYPLANE_H__
#define __ENEMYPLANE_H__

#define TIME_SPACE 60

#include "cocos2d.h"
#include "Missile.h"
USING_NS_CC;

class EnemyPlane : public Node
{
public:
//	Bullet* bullet;
	virtual bool hurt(int damage,Point point);
	virtual void die();		//�ڼ����ɻ�������
	virtual bool shoot();
	virtual bool move();
	virtual void startAnim(){};
	void setLayer(Layer *layer);
	void setMissile(Missile *missile);
	Missile *missile;
	bool isGoal=false;
	bool isDie=false;
	int hp=0;
	int index;			//�л����
	int liveFlag=0;
	Sprite3D *plane;
protected:
	Layer *gameLayer;
	Vec3 dieVec;
	int score=0;
	int timeCounter=0;
	int shootCounter=0;
	bool isShoot=false;
	float step=0;

};
#endif
