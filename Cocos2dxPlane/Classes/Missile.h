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
	Node *enemyPlane;					//�л��Ľڵ�
	Layer *layer;
	Point missilePoint;					//����λ��
	Point goalPoint;					//Ŀ��λ��

	float timeCounter=0;				//������
	bool canMove=true;					//�����Ƿ���Ŀ��
	float radio=0;
	float speed=16;						//�������ٶ�

	int liveFlag=0;
	Sprite3D *missileM;
	ParticleSystemQuad* psqMissileM;
	static vector<Sprite3D*> missileVec;
//	//---------------------------------------
	Sprite3D *missile;
	Animate3D *animate;
	void MissileMove();				//�ƶ�����
	void shootEnemyPlane();			//��Ƶл�
	void move();
	void updateShoot();
	void vanish();					//������ʧ
	void createMissile();			//���������͵���������ϵͳ
	void getNewEnemyPlane();		//��ȡ�µĵл�����
//	CC_SYNTHESIZE(EnemyPlane*, _target, Target);
	int positionZ=0;
	int direction=0;
//	//��������
	ParticleSystemQuad* psqMissile;

	//�����ߵ�
	float a=0;
};

#endif
