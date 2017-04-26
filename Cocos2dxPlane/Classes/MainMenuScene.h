#ifndef __MainMenuScene_H__
#define __MainMenuScene_H__

#include "GameSceneManager.h"
#include "cocos2d.h"
#include "3d/CCAnimation3D.h"
#include "3d/CCAnimate3D.h"
#include "ui/CocosGUI.h"
#include "GameSet.h"
#include "NetLayer.h"
using namespace cocos2d;
using namespace ui;

class MainMenuScene : public cocos2d::Layer
{
public:
	GameSceneManager* gsm;
	NetLayer *netLayer;
	GameSet *setGame;
	Sprite* back1;
	Sprite* title;
	Sprite* messageSprite;

	//��ʼ��Ϸ��ת��
	Sprite *startRotate1;
	Sprite *startRotate2;
	Sprite* menuBg;

	//��ť
	MenuItemImage* menuItem[3];			//0-��ʼ��Ϸ��1-ս��ǿ����2-����
	MenuItemImage* setItem;
	MenuItemImage* aboutItem;
	MenuItemImage* exitItem;
	MenuItemImage* soundItem;
	MenuItemImage* touchItem;
	MenuItemImage* gravityItem;

	Menu* menu;

	bool isSelectModel=false;
	//��ʼ���ķ���
	virtual bool init();
	void addMenuItem();
	void menuSelect(int index);		//ѡ��������
	void createSetBox();
	void menuTouch();
	void menuGravity();

	void deleteNetLayer();
	//��������
	bool onMyTouchBegan(Touch* touch, Event* event);
	void onMyTouchMoved(Touch* touch, Event* event);
	void onMyTouchEnded(Touch* touch, Event* event);
    //�������Ĵ����¼�
//    void sliderEvent(Object* pSender, SliderEventType type);

    //Cocos2d-x�ж����һ���꣬����ָ�����͵Ķ���
    //��������Ӧ�ĳ�ʼ�����ͷŹ���
    CREATE_FUNC(MainMenuScene);
};

#endif
