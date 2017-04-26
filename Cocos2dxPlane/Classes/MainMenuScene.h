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

	//开始游戏的转动
	Sprite *startRotate1;
	Sprite *startRotate2;
	Sprite* menuBg;

	//按钮
	MenuItemImage* menuItem[3];			//0-开始游戏，1-战机强化，2-帮助
	MenuItemImage* setItem;
	MenuItemImage* aboutItem;
	MenuItemImage* exitItem;
	MenuItemImage* soundItem;
	MenuItemImage* touchItem;
	MenuItemImage* gravityItem;

	Menu* menu;

	bool isSelectModel=false;
	//初始化的方法
	virtual bool init();
	void addMenuItem();
	void menuSelect(int index);		//选择进入界面
	void createSetBox();
	void menuTouch();
	void menuGravity();

	void deleteNetLayer();
	//触摸监听
	bool onMyTouchBegan(Touch* touch, Event* event);
	void onMyTouchMoved(Touch* touch, Event* event);
	void onMyTouchEnded(Touch* touch, Event* event);
    //拖拉条的触摸事件
//    void sliderEvent(Object* pSender, SliderEventType type);

    //Cocos2d-x中定义的一个宏，创建指定类型的对象
    //并做好相应的初始化与释放工作
    CREATE_FUNC(MainMenuScene);
};

#endif
