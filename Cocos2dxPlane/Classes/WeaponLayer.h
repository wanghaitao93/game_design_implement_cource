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
	Animate *animLevel;			//升级换帧动画

	//初始化的方法
	virtual bool init();
	//初始化菜单项
	void initMenu();
	//菜单按钮作用
	void menuSelect(int index);
	//执行动画
	void startAnim();
	//回调函数
	void callBack();
	//刷新项目等级
	void refreshGrade();
	//初始化边框
	void initFrame();
	//更换飞机
	void changePlane(int mode);
	//创建飞机
	void initPlane();
	//保存数据
	void saveData();
	//升级
	void upGrade();
	//解锁
	void unLock();
	//改变按钮
	void changeButton();
	//清除
	void clearPoint();

	//touch
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
	//刷新金钱显示
	void refreshLabel();
	//析构函数
	virtual ~WeaponLayer();
	CREATE_FUNC(WeaponLayer);
};
#endif
