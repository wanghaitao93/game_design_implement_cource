#ifndef __SelectPlane_H__
#define __SelectPlane_H__

#include "cocos2d.h"
#include "GameSceneManager.h"

using namespace cocos2d;
using namespace std;

class SelectPlane : public cocos2d::Layer
{
public:
	GameSceneManager* gsm;			//所属者
	virtual bool init();			//初始化场景

    CREATE_FUNC(SelectPlane);
private:
	int angle=360;						//飞机选择角度
	int step=0;							//飞机移动步长
	int waitIndex=0;
	Texture2D *starsTexture;			//星星
	MenuItemImage* menuItem[5];			//菜单项
	Sprite3D* planeobj[3];				//玩家三种飞机
	Menu *menu;
	Sprite *backButtom;					//低栏
	Sprite *leftMove;					//左移动
	Sprite *rightMove;					//右移动
	Sprite *selectPlaneSort;			//选择飞机类型
	Sprite *stars[3];				//星星攻击力,星星生命值,星星技能
	Sprite *lock;					//锁

	Sprite *housePlane;				//房主选择飞机的情况
	Sprite *playerPlane;			//玩家选择飞机的情况

	Sprite *waittingConnect;		//等待的精灵

	void initMenu();				//初始化2D场景的菜单按钮
	void initPlane3D();				//初始化三种玩家飞机
	void menuSelect(int index);		//菜单按钮回调方法
	void setPoint3D(float dt);		//飞机移动定时回调
	void goNextScene();				//跳转下一场景
	void changePlane(int index);	//更改选飞机信息
	void changeContent(int index);
	void initPlaneStar();			//初始化飞机信息 星星
	void changePlaneStar();			//更改飞机的信息 星星
	void changeSelectPlane(float f);//改变选择的飞机
	void planeFly();				//飞机飞翔
	void changWaitting(float f);	//等待中
	//touch
	bool onMyTouchBegan(Touch* touch, Event* event);
	void onMyTouchMoved(Touch* touch, Event* event);
	void onMyTouchEnded(Touch* touch, Event* event);

};

#endif
