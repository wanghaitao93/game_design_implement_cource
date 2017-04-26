#ifndef _Bullet_H_
#define _Bullet_H_

#include "cocos2d.h"
#include "NewPlane.h"
using namespace cocos2d;

class ControlBullet:public Node
{
public :
	static ControlBullet* create(NewPlane *_plane);
	void update();
	NewPlane *plane;
};

class Bullet:public Sprite
{
public :
//	Bullet(int num,Point point1,float angle,float speed,Point point2,Layer *layer);//,Sprite *node
//	Sprite *plane;					//敌机或玩家飞机的节点
//	~Bullet();
//	Sprite3D* bullet;
//	BillBoard* bullet;
	static Bullet* create(int num,Point point1,float speed,Point point2,float angle);
	static Bullet* webCreate(int num,Point point, float angle);
	void updatePoint();		//定时回调更新位置
	int damage;
	int num;
	int liveFlag=0;

//	CREATE_FUNC(Bullet);
private:
	static Rect rTemp[];
	static int iTemp[];
	Point goalPoint;				//目标位置
	Point startPoint;				//起始位置
	float bSpeed;					//子弹的速度
	float angle;					//子弹旋转角度
//	void removeBullets();					//删除子弹
	void shootGoals();				//射击目标
//	float calAngle();				//计算角度
//	Sprite3D* bullet;
};

class Collision
{
public :
	static bool check(Vec2 local, Vec2 goal, int localN, int goalN);
};

#endif
