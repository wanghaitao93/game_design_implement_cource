#ifndef __WeaponLayer_H__
#define __WeaponLayer_H__

#include "GameSceneManager.h"
#include "cocos2d.h"
using namespace cocos2d;

class WeaponLayer : public cocos2d::Layer
{
public:
	GameSceneManager* gsm;
	Sprite* back;
	Sprite* goldView;
	Sprite* tailSprite1;
	Sprite* tailSprite2;
	Sprite* frame;
	Sprite3D* coin;
	Sprite3D *menu3D[4];
	Sprite3D* plane[3];
	Sprite *lock;
	Sprite *point;
	ActionInterval *tailAction1;
	ActionInterval *tailAction2;
	LabelAtlas *priceLabel;
	LabelAtlas *goldLabel;
	Animate *animate;
	Menu *menu;
	MenuItem *gold;
	MenuItem *bullets;
	MenuItem *blood;
	MenuItem *attack;
	MenuItem *toLeft;
	MenuItem *toRight;
	MenuItem *upgrade;
	MenuItem *buyButton;
	int selectID=0;
	int timeCounter=0;
	int price=0;
	int coinAngle=0;
	bool initParticle=false;
	Animate *animLevel;			//������֡����

	//��ʼ���ķ���
	virtual bool init();
	//��ʼ���˵���
	void initMenu();
	//�˵���ť����
	void menuSelect(int index);
	//ִ�ж���
	void startAnim();
	//�ص�����
	void callBack();
	//ˢ����Ŀ�ȼ�
	void refreshGrade();
	//��ʼ���߿�
	void initFrame();
	//�����ɻ�
	void changePlane(int mode);
	//�����ɻ�
	void initPlane();
	//��������
	void saveData();
	//����
	void upGrade();
	//����
	void unLock();
	//�ı䰴ť
	void changeButton();
	//���
	void clearPoint();

	//touch
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
	//ˢ�½�Ǯ��ʾ
	void refreshLabel();
	//��������
	virtual ~WeaponLayer();
	CREATE_FUNC(WeaponLayer);
};
#endif
