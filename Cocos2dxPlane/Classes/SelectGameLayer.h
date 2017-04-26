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
	//雷电特效
	Animate *animThunder;
	//文本显示框
	Sprite *introduce[3];
	//背景
	Sprite *background;
	//选择遮罩
	Sprite *planeSelect;
	Sprite *gameSelect;
	//BOSS位置
	int positionY=0;
	//
	int waitIndex;			//等待的编号
	//定时回调
	//转台
	Sprite3D *turnTable;
	Sprite *stars[3];				//xingxing
	Sprite *waittingConnect;
	Texture2D *starsTexture;		//星星
	//BOSS
	Sprite3D *boss[3];
	Sprite *lock;	//boss锁
	bool raise=false;	//boss上升还是下降

	static vector<Sprite3D*> bossVec;
	//开始游戏
	MenuItemImage *startGame;
	Menu *menu;
	//是否能切换关卡
	bool isCanSelect=true;

	virtual bool init();
	void updateMoveBoss(float f);
	void goMainMenu();						//返回主菜单
	void goSelectPlane();					//选飞机
	void goSelectPlaneAnim();				//选飞机完成动画
	void changeLevel(int index);			//改变标题
	void changWaitting(float f);
	void lockInterface();			//锁定界面


	bool touchFlag=false;

	int step=0;		//计算已经滑了多少
	int speed=10;	//滑动的速度
	int offset;		//根据加速度需要滑动的距离
	long timestart;	//开始触摸时间
	Point startPoint;//开始的位置
	Point sPoint[3];	//精灵的位置

	//可以触摸
	bool canMove=false;
	//touch
	bool onMyTouchBegan(Touch* touch, Event* event);
	void onMyTouchMoved(Touch* touch, Event* event);
	void onMyTouchEnded(Touch* touch, Event* event);

	CREATE_FUNC(SelectGameLayer);
};
#endif
