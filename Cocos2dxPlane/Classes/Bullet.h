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
//	Sprite *plane;					//�л�����ҷɻ��Ľڵ�
//	~Bullet();
//	Sprite3D* bullet;
//	BillBoard* bullet;
	static Bullet* create(int num,Point point1,float speed,Point point2,float angle);
	static Bullet* webCreate(int num,Point point, float angle);
	void updatePoint();		//��ʱ�ص�����λ��
	int damage;
	int num;
	int liveFlag=0;

//	CREATE_FUNC(Bullet);
private:
	static Rect rTemp[];
	static int iTemp[];
	Point goalPoint;				//Ŀ��λ��
	Point startPoint;				//��ʼλ��
	float bSpeed;					//�ӵ����ٶ�
	float angle;					//�ӵ���ת�Ƕ�
//	void removeBullets();					//ɾ���ӵ�
	void shootGoals();				//���Ŀ��
//	float calAngle();				//����Ƕ�
//	Sprite3D* bullet;
};

class Collision
{
public :
	static bool check(Vec2 local, Vec2 goal, int localN, int goalN);
};

#endif
