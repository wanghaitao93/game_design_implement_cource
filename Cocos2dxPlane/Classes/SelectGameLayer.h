#ifndef _SelectGameLayer_H_
#define _SelectGameLayer_H_

#include "cocos2d.h"
#include "GameSceneManager.h"
using namespace cocos2d;
using namespace std;

class SelectGameLayer:public cocos2d::Layer
{
public:
	GameSceneManager *gsm;
	//�׵���Ч
	Animate *animThunder;
	//�ı���ʾ��
	Sprite *introduce[3];
	//����
	Sprite *background;
	//ѡ������
	Sprite *planeSelect;
	Sprite *gameSelect;
	//BOSSλ��
	int positionY=0;
	//
	int waitIndex;			//�ȴ��ı��
	//��ʱ�ص�
	//ת̨
	Sprite3D *turnTable;
	Sprite *stars[3];				//xingxing
	Sprite *waittingConnect;
	Texture2D *starsTexture;		//����
	//BOSS
	Sprite3D *boss[3];
	Sprite *lock;	//boss��
	bool raise=false;	//boss���������½�

	static vector<Sprite3D*> bossVec;
	//��ʼ��Ϸ
	MenuItemImage *startGame;
	Menu *menu;
	//�Ƿ����л��ؿ�
	bool isCanSelect=true;

	virtual bool init();
	void updateMoveBoss(float f);
	void goMainMenu();						//�������˵�
	void goSelectPlane();					//ѡ�ɻ�
	void goSelectPlaneAnim();				//ѡ�ɻ���ɶ���
	void changeLevel(int index);			//�ı����
	void changWaitting(float f);
	void lockInterface();			//��������


	bool touchFlag=false;

	int step=0;		//�����Ѿ����˶���
	int speed=10;	//�������ٶ�
	int offset;		//���ݼ��ٶ���Ҫ�����ľ���
	long timestart;	//��ʼ����ʱ��
	Point startPoint;//��ʼ��λ��
	Point sPoint[3];	//�����λ��

	//���Դ���
	bool canMove=false;
	//touch
	bool onMyTouchBegan(Touch* touch, Event* event);
	void onMyTouchMoved(Touch* touch, Event* event);
	void onMyTouchEnded(Touch* touch, Event* event);

	CREATE_FUNC(SelectGameLayer);
};
#endif
