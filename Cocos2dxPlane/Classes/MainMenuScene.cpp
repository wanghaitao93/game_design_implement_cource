#include "MainMenuScene.h"
#include "AppMacros.h"
#include "Constant.h"
#include "ParticleManager.h"
#include "PlaySound.h"
#include "GameSet.h"
#include "GameData.h"
#include "BNSocketUtil.h"
#include "SimpleAudioEngine.h"
#include <string>

using namespace ui;
using namespace cocos2d;

//实现MainMenuScene类中的init方法，初始化布景
bool MainMenuScene::init()
{
	//调用父类的初始化
    if ( !Layer::init() )
    {
        return false;
    }

    addMenuItem();
    //创建一个精灵对象
    back1 = Sprite::create(MAP_PATH+std::string("mainLayerBg.jpg"));
    back1->setPosition(Point(270,0));
    back1->setAnchorPoint(Point(0.5,0));
    this->addChild(back1,BACKGROUND_LEVEL);
    title = Sprite::create(GAME_PATH+std::string("gameTitle2.png"));
    title->setPosition(Point(270,730));
	this->addChild(title,2);
    

	//创建一个单点触摸监听
	auto listener = EventListenerTouchOneByOne::create();
	//设置下传触摸
	listener->setSwallowTouches(true);
	//开始触摸时回调onTouchBegan方法
	listener->onTouchBegan = CC_CALLBACK_2(MainMenuScene::onMyTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(MainMenuScene::onMyTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(MainMenuScene::onMyTouchEnded, this);
//	//添加到监听器
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    //粒子系统
//    ParticleManager* pm=new ParticleManager(this);
//    pm->initRain();

    return true;
}
void MainMenuScene::addMenuItem()
{
    //创建开始菜单项
	menuItem[0] = MenuItemImage::create
    (
		   MENU_PATH+std::string("start.png"),			//平时的图片
		   MENU_PATH+std::string("start_btn.png"),		//选中时的图片
		   CC_CALLBACK_0(MainMenuScene::menuSelect, this,0) //点击时执行的回调方法
	);
    menuItem[0]->setPosition(Point(-270,500));

    //战机强化
    menuItem[1] = MenuItemImage::create
    (
		   MENU_PATH+std::string("strengthen.png"),		//平时的图片
		   MENU_PATH+std::string("strengthen_down.png"),//选中时的图片
		   CC_CALLBACK_0(MainMenuScene::menuSelect, this,1) //点击时执行的回调方法
	);
    menuItem[1]->setPosition(Point(810,380));
    //帮助
    menuItem[2] = MenuItemImage::create
    (
		   MENU_PATH+std::string("help.png"),			//平时的图片
		   MENU_PATH+std::string("help_down.png"),		//选中时的图片
		   CC_CALLBACK_0(MainMenuScene::menuSelect, this,3) //点击时执行的回调方法
	);
    menuItem[2]->setPosition(Point(-270,260));

    exitItem = MenuItemImage::create
    (
		   MENU_PATH+std::string("close_btn.png"),			//平时的图片
		   MENU_PATH+std::string("close_btn_down.png"),		//选中时的图片
		   CC_CALLBACK_0(MainMenuScene::menuSelect,this,4)	//点击时执行的回调方法
	);
    exitItem->setPosition(Point(45,50));
    //设置
    setItem = MenuItemImage::create
    (
		   MENU_PATH+std::string("set_btn.png"),			//平时的图片
		   MENU_PATH+std::string("set_btn_down.png"),		//选中时的图片
		   CC_CALLBACK_0(MainMenuScene::menuSelect, this,2) //点击时执行的回调方法
	);
    setItem->setPosition(Point(115,50));

	std::string voicePicName="";
	std::string voicePicNameP="";
	if(Constant::soundFlag==true){
		voicePicName="soundOpened_btn.png";
		voicePicNameP="soundOpened_btn_down.png";
	}else{
		voicePicName="soundClosed_btn.png";
		voicePicNameP="soundClosed_btn_down.png";
	}

	//声音
    soundItem = MenuItemImage::create
    (
    		MENU_PATH+voicePicName,					//平时的图片
    		MENU_PATH+voicePicNameP,				//选中时的图片
		   CC_CALLBACK_0(MainMenuScene::menuSelect, this,5) 	//点击时执行的回调方法
	);
    soundItem->setPosition(Point(185,50));

    menu = Menu::create(menuItem[0],menuItem[1],menuItem[2],setItem,exitItem,soundItem,nullptr);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, MENU_LEVEL);

    for(int i=0;i<3;i++)
	{
		menuItem[i]->runAction(Sequence::create(
			DelayTime::create(0.2*i),
			EaseBounceOut::create(MoveTo::create(0.5,Point(270,500-120*i))),nullptr));
		PlaySound::playSound(kButtonGo);							// 播放初始化按钮的声音
	}


}
//先择进入那个界面
void MainMenuScene::menuSelect(int index)
{
	if(index<6){							//播放声音
		PlaySound::playSound(kMenuSure);
	}else if(index==6){						//选择声音
		PlaySound::playSound(kMenuSelect);
	}else if(index==7){						//撤销
		PlaySound::playSound(kCancel);
	}
	SpriteFrame *touchFrame;
	SpriteFrame *gravityFrame;
	SpriteFrame *soundFrame;
	SpriteFrame *soundFrameP;
	//跳转界面
	switch(index){
		case 0:						//进入游戏
			netLayer = NetLayer::create();
			this->addChild(netLayer,MENU_LEVEL);
			isSelectModel=true;
			menu->setEnabled(false);
			break;
		case 1:						//战机强化
			gsm->goWeapon();
			break;
		case 2:						//设置
			createSetBox();
			for(int i=0;i<4;i++)
			{
				menuItem[i]->setEnabled(false);
			}

			exitItem->setEnabled(false);
			soundItem->setEnabled(false);
			break;
		case 3:						//帮助
			gsm->goHelp();
			break;
		case 4:						//退出游戏
			Director::getInstance()->end();
			break;
		case 5:						//声音
			if(Constant::soundFlag)
			{
				soundFrame=SpriteFrame::create(MENU_PATH+std::string("soundClosed_btn.png"),Rect(0,0,50,50));
				soundFrameP=SpriteFrame::create(MENU_PATH+std::string("soundClosed_btn_down.png"),Rect(0,0,50,50));
			}else{
				soundFrame=SpriteFrame::create(MENU_PATH+std::string("soundOpened_btn.png"),Rect(0,0,50,50));
				soundFrameP=SpriteFrame::create(MENU_PATH+std::string("soundOpened_btn_down.png"),Rect(0,0,50,50));
			}
			soundItem->setNormalSpriteFrame(soundFrame);
			soundItem->setSelectedSpriteFrame(soundFrameP);
			Constant::soundFlag=!Constant::soundFlag;
			if(!Constant::soundFlag){
				PlaySound::pauseBackground();
			}else{
				PlaySound::playBackground();
			}
			UserDefault::getInstance()->setIntegerForKey(
					"soundFlag",Constant::soundFlag);
			break;
		case 6:						//触摸开启与关闭
			if(!Constant::touchFlag){
				touchFrame=SpriteFrame::create(MENU_PATH+std::string("close.png"),Rect(0,0,68,29));

				gravityFrame=SpriteFrame::create(MENU_PATH+std::string("open.png"),Rect(0,0,68,29));
			}else{					//触摸关闭时重力感应开启
				touchFrame=SpriteFrame::create(MENU_PATH+std::string("open.png"),Rect(0,0,68,29));

				gravityFrame=SpriteFrame::create(MENU_PATH+std::string("close.png"),Rect(0,0,68,29));
			}
			Constant::touchFlag=!Constant::touchFlag;

			touchItem->setNormalSpriteFrame(touchFrame);
			touchItem->setSelectedSpriteFrame(touchFrame);

			gravityItem->setNormalSpriteFrame(gravityFrame);
			gravityItem->setSelectedSpriteFrame(gravityFrame);
			break;
		case 7:						//设置关闭
			for(int i=0;i<4;i++)
			{
				menuItem[i]->setEnabled(true);
			}
			exitItem->setEnabled(true);
			soundItem->setEnabled(true);
			this->removeChild(menuBg,true);
			break;
	}
}
//创建设置窗口
void MainMenuScene::createSetBox()
{
	setGame=new GameSet(this);
	setGame->createSet(0);
}
//触摸开始
bool MainMenuScene::onMyTouchBegan(Touch* touch, Event* event)
{
	return true;
}
//触摸
void MainMenuScene::onMyTouchMoved(Touch *touch, Event *event)
{
	auto target = static_cast<Sprite*>(	event->getCurrentTarget());//获取当前触摸对象，并转化为精灵类型
	Point location = target->convertToNodeSpace(touch->getLocation());//获取当前坐标
	if(isSelectModel && !NetLayer::isConnecting){
		deleteNetLayer();
	}
}
void MainMenuScene::onMyTouchEnded(Touch *touch, Event *event)
{
}

void MainMenuScene::deleteNetLayer()
{
	this->removeChild(netLayer,true);
	if(GameData::connectFlag)
	{
		BNSocketUtil::sendInt(100);
		BNSocketUtil::closeConnect();
	}
	isSelectModel=false;
	menu->setEnabled(true);
}
