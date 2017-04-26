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
	Camera *camera;			//�����
//	GameSceneManager *gsm;
	float degree=0;
	Missile *ms;			//������
	NewPlane *plane;			//�ɻ���
	Bullet* b;
	Sprite3D* back1;
	Sprite3D* back2;
//	Sprite3D* upSpeed;
	EnemyPlane *enemyPlane[2];	//�л�
	MotionStreak *myStreak; 		//��β
	Sprite *progress;			//�������ı���
//	LoadingBar *loadingbar;		// �ӵ��Ľ�����
	Coin *coin;
	ControlGame *controlGame;
	ControlBullet *controlBullet;
	ExManager *exManager;
	Animate *protectAnim;
	ControlPotentiometer *potentiometer;	//Բ�̽�����
	float progressNum=500;					//����ʱ
	BillBoard *temp;						//������
	BillBoard *tempProgress;


	DirectionLight *directionLight;			//�����
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
	void createPlane();		//�����ڼ���
	void updateCreateMissile(float f);
	void update(float f);
	void changeCamera(float x);
	void deletePlane();				//ɾ���ɻ��Ķ�ʱ�ص�
	void createMissile();			//��������
    void resetGame();				//������Ϸ��Ϣ
    void attackAll();				//�������ел�
	void attackAllFinish();			//�����л����ܽ���
	void protectMyself();			//�����ɻ�
	void protectMyselfFinish();		//�����ɻ����ܽ���
	void updateProgress();
	void startGame();				//��ʼ��Ϸ
	void clearAllBullets();			//��������ӵ�
    //��ȡ��������
    void getAnimPoint(Vec3 nearP,Vec3 farP);
	//�ɻ��Ĵ�������
	bool onMyTouchBegan(Touch* touch, Event* event);
	void onMyTouchMoved(Touch* touch, Event* event);
	void onMyTouchEnded(Touch* touch, Event* event);
	//������Ӧ
	void onMyAcceleration(Acceleration* acc, Event* unused_event);	//������Ӧ�ص�����
    CREATE_FUNC(First3DLayer);

private:
    int planeId;			//�ɻ����
    int gameLevel;			//��Ϸ�ؿ�
    int attackGrade;		//�������ȼ�
    int bloodGrade;			//Ѫ��
    int bulletsGrade;		//�ӵ��Ĺ����ٶ�
    int goldGrade;			//��Ҽӳ�
    float enemy2Num[N];				//�л�2�ű��
    float enemy3Num[N];				//�л�3�ű��
    void getProperty();				//��ȡ��Ϸ������
    void playerBullets(float dt);	//��ҷɻ��ӵ��ص�����
    Animate *boomAnim;				//��ը
//    void changePro();				//�ı������
    bool touchFlag=false;			//������־λ
};

enum Complete
{
	loseEnum,winEnum
};

#endif
