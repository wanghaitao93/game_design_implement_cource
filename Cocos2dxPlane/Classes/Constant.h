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

	//���============
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
	//��
	static std::string title[3];
	static std::string titleC[3];
	static std::string content[3];
	static std::string planeName[3];
	static std::string planeContent[3];
	static int custom;			//�ؿ�  0,1,2
	static int canPlay;			//0,1,2
	static int planeId;			//�ɻ���� 0,1,2
	static int canPlane;		//��������
	static bool loaded;			//�Ƿ��Ѿ�������
	static bool acceler;		//�Ƿ���������Ӧ
	static int planeHealth;		//�ɻ�������ֵ
	static int sensitivity;		//������Ӧ��������
	static int planeSkill[3][3];		//�ɻ�������
	static int bossSkill[3][3];		//�ɻ�������
	//-------------------�¼�
	static int bulletLevel;									//�ӵ���ĵȼ�12		0,1,2,3,4
	static int bulletPower;									//�ӵ�������	31		0,1,2,3,4
	static int missileLevel;								//�����ĵȼ�	13		0,1,2,3
//	static int skillNum;									//���еĸ���	13		0,1,2,3		���3��
//	static int defenseNum;									//�����ĵȼ�	13		0,1,2,3		����3��
	static int attackCount;									//���еĸ���	13		0,1,2,3		���3��
	static int protectCount;								//�����ĵȼ�	13		0,1,2,3		����3��
	static int enemyBulletSpeed;
	static Point planePoint;
	static bool isBulletPower;								//�Ƿ����ӵ��������ӳ�
	static int bulletPowerTime;								//�ӵ�������ʱ����		һ��1000
	static bool openCustom;									//�����ؿ�
	static bool gameingFlag;
};
#endif
