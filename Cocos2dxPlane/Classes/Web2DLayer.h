#ifndef __Web2DLayer_H__
#define __Web2DLayer_H__

#include "GameSceneManager.h"
#include "Web3DLayer.h"
#include "GameSet.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;


class Web2DLayer : public cocos2d::Layer
{
	public:
	static Web2DLayer* getInstance();
	GameSceneManager* gsm;
	LabelAtlas* goldLabel;
	LabelAtlas *attackLabel;	//攻击
	LabelAtlas *protectLabel;	//防御

	Web3DLayer * layer3D;
	GameSet *gameSet;

	MenuItemImage* continueGame;
	MenuItemImage* gameset;
	MenuItemImage* mainMenu;
	//初始化的方法
	virtual bool init();
	void completeGame(int flag);		//结束游戏
	void pauseGame();			//暂停游戏
	void sharkInjuredBG();			//闪动受伤的背景
	void setVisibleBG();			//隐藏背景
	void initPauseCode();		//初始化暂停界面
	void continueCallback();	//回到游戏回调方法
    CREATE_FUNC(Web2DLayer);
private:
    Sprite3D* gold;
	Sprite* pauseBG;
	Sprite* continuePauseBG;
	Sprite* waitting;
	MenuItemImage *deadBigBG;
	Sprite *deadSmallBG;
	Sprite3D *coin;
	Sprite *cloud[4];					//云彩

	Sprite *injureBG;				//受伤的背景
	Sprite *progress;
	LoadingBar *loadingbar;		// 子弹的进度条
	LoadingBar *loadingbar_plane;			// 玩家的进度条
	LoadingBar *progressOPlane;		//竖着的进度条
	Menu* menu;

    float angle=0;
	void goPause();				//跳转到暂停界面
	void update();				//定时回调--金币旋转
	void continueAnim();		//回到游戏的倒计时
	void voiceChangeCallback();	//声音改变回调方法
	void goMainMenu();			//跳转到主菜单场景
	void createCloud();			//创建云群
	void setGame();
	void changePro();			// 改变进度
	void useSkill(int number);			//使用技能
};
class CompleteGameLayer : public Layer
{
public :
	virtual bool init();		//初始化方法
	static CompleteGameLayer* create(int _flag);
private:
    void goMainMenu();			//跳转到主菜单场景
    void createBoard();			//游戏结算板
    void moveBoard();			//移动结算板
    void updateCoin();			//更新金币数量
	void goNext();				//到下一关
	void goSelf();				//重玩本关
    void setMenu();				//执行动画
    void setGameDate();			//设置游戏数据
    void updateWeb();			//监控方法
    void menuCallback(int index);	//菜单回调方法
    Web3DLayer * layer3D;		//3Dlayer
    Sprite *board;
    LabelAtlas* goldLabel;
    int coinCounter=0;
    int coinStep=0;
    int flag;
};

#endif
