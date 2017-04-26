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
	void deletePlane(int index);				//ɾ���ɻ�
	void playBoom();
	//��ȡ��������
	void getAnimPoint(Vec3 nearP,Vec3 farP);
	//�ɻ��Ĵ�������
	bool onMyTouchBegan(Touch* touch, Event* event);
	void onMyTouchMoved(Touch* touch, Event* event);
	void onMyTouchEnded(Touch* touch, Event* event);
	//������Ӧ
	void onMyAcceleration(Acceleration* acc, Event* unused_event);	//������Ӧ�ص�����
	CREATE_FUNC(Web3DLayer);

	Sprite3D *playerPlane;	//��ҷɻ�
	Sprite3D *housePlane;	//�����ɻ�
	Sprite3D *back1;
	Sprite3D *back2;
	Camera *camera;
	MotionStreak *myStreak; 		//��β
	ControlPotentiometer *potentiometer;	//Բ�̽�����
	float progressNum=500;					//����ʱ
	BillBoard *tempBoard1;						//������
	BillBoard *tempBoard2;						//������
	BillBoard *tempProgress;
	Sprite *progress;				//�������ı���
	float playerPlaneX,playerPlaneY,housePlaneX,housePlaneY,playerAngleX,playerAngleY,playerAngleZ,
						houseAngleX,houseAngleY,houseAngleZ,redHealth,greenHealth,score,redAttackCount,greenAttackCount,redProtectCount,greenProtectCount;
	Animate *protectAnim;
	Animate *boomAnim;				//��ը
	bool touchFlag=false;			//������־λ
	Vec2 animPoint;					//Ŀ���
	DirectionLight *directionLight;				//�����

	ExManager *exManager;

	std::map<int, Bullet *> enemyBulletMap;
	std::map<int, Bullet *> mainBulletMap;
	std::map<int, EnemyPlane *> enemyMap;
	std::map<int, Award *> awardMap;
	std::map<int ,Missile *> missileMap;
};

#endif
