#include "GameSet.h"
#include "AppMacros.h"
#include "Constant.h"
#include "PlaySound.h"
#include "MainMenuScene.h"
#include "My2DLayer.h"
#include "Web2DLayer.h"
#include "GameData.h"
#include "cocos2d.h"
using namespace cocos2d;
using namespace ui;

GameSet::GameSet(Layer *gameLayer)
{
	this->layer=gameLayer;
}
void GameSet::createSet(int i)
{
	index=i;
	if(i==0){
		Director::getInstance()->pause();		//暂停
	}
	//创建背景
	menuBg=Sprite::create(GAMEMODE_PATH+std::string("MenuBg.png"));
	menuBg->setPosition(Point(270,480));
	layer->addChild(menuBg,SET_LEVEL);

	Sprite* setBox=Sprite::create(GAMEMODE_PATH+std::string("gameModeBG.png"));
	setBox->setPosition(Point(270,520));
	menuBg->addChild(setBox,1);
	setBox->runAction(EaseBounceOut::create(ScaleTo::create(0.2,1.0f)));

	std::string touchPicName="";
	std::string gravityPicName="";
	if(!Constant::touchFlag)
	{
		touchPicName="open.png";
		gravityPicName="close.png";
	}else{
		touchPicName="close.png";
		gravityPicName="open.png";
	}
	touchItem = MenuItemImage::create
	(
		GAMEMODE_PATH+touchPicName,
		GAMEMODE_PATH+touchPicName,
		CC_CALLBACK_0(GameSet::menuSelect, this,0)
	);
	touchItem->setPosition(
		Point(300,280)
	);

	gravityItem = MenuItemImage::create
	(
		GAMEMODE_PATH+gravityPicName,					//平时的图片
		GAMEMODE_PATH+gravityPicName,					//选中时的图片
		CC_CALLBACK_0(GameSet::menuSelect, this,0) //点击时执行的回调方法
	);
	gravityItem->setPosition(
		Point(300,182)
	);

	MenuItemImage* closeSet = MenuItemImage::create
	(
		   GAMEMODE_PATH+std::string("setClose.png"),				//平时的图片
		   GAMEMODE_PATH+std::string("setClose_down.png"),			//选中时的图片
		   CC_CALLBACK_0(GameSet::menuSelect, this,1) 	//点击时执行的回调方法
	);
	closeSet->setPosition(
		Point(435,440)
	);
	closeSet->setAnchorPoint(Point(1,1));

	Menu* menu = Menu::create(touchItem,gravityItem,closeSet,nullptr);
	menu->setPosition(Point::ZERO);
	setBox->addChild(menu, 2);

	//添加进度条
	//拖拉条相关====================================================================
	Slider* slider = Slider::create();
	slider->loadBarTexture(GAMEMODE_PATH+std::string("sliderTrack.png"));
	slider->loadSlidBallTextures(GAMEMODE_PATH+std::string("sliderballnormal.png"),GAMEMODE_PATH+std::string("sliderballpressed.png"), "");
	slider->loadProgressBarTexture(GAMEMODE_PATH+std::string("sliderProgress.png"));
	slider->setAnchorPoint(Point(0, 1));
	slider->setPercent(50);
	slider->setPosition(Point(200,95));
	slider->addEventListenerSlider(layer, sliderpercentchangedselector(GameSet::sliderEvent));
	setBox->addChild(slider,2);//添加到布景

}
//先择进入那个界面
void GameSet::menuSelect(int i)
{
	if(i==0){						//选择声音
		PlaySound::playSound(kMenuSelect);
	}else if(i==1){						//撤销
		PlaySound::playSound(kCancel);
	}

	SpriteFrame *touchFrame;
	SpriteFrame *gravityFrame;
	SpriteFrame *soundFrame;
	//跳转界面
	switch(i){
		case 0:						//触摸开启与关闭
			if(!Constant::touchFlag){
				touchFrame=SpriteFrame::create(GAMEMODE_PATH+std::string("close.png"),Rect(0,0,80,38));

				gravityFrame=SpriteFrame::create(GAMEMODE_PATH+std::string("open.png"),Rect(0,0,80,38));
			}else{					//触摸关闭时重力感应开启
				touchFrame=SpriteFrame::create(GAMEMODE_PATH+std::string("open.png"),Rect(0,0,80,38));

				gravityFrame=SpriteFrame::create(GAMEMODE_PATH+std::string("close.png"),Rect(0,0,80,38));
			}
			Constant::touchFlag=!Constant::touchFlag;

			touchItem->setNormalSpriteFrame(touchFrame);
			touchItem->setSelectedSpriteFrame(touchFrame);

			gravityItem->setNormalSpriteFrame(gravityFrame);
			gravityItem->setSelectedSpriteFrame(gravityFrame);
			break;
		case 1:						//设置关闭
			if(index==0){
				MainMenuScene *scene=dynamic_cast<MainMenuScene*>(layer);
				for(int i=0;i<4;i++)
				{
					(scene->menuItem[i])->setEnabled(true);
				}
				(scene->exitItem)->setEnabled(true);
				(scene->soundItem)->setEnabled(true);
				Director::getInstance()->resume();
			}else if(index==1){
				if(GameData::playerNumber==-1){
					My2DLayer *scene=dynamic_cast<My2DLayer*>(layer);
					(scene->continueGame)->setEnabled(true);
					(scene->gameset)->setEnabled(true);
					(scene->mainMenu)->setEnabled(true);
				}else{
					Web2DLayer *scene=dynamic_cast<Web2DLayer*>(layer);
					(scene->continueGame)->setEnabled(true);
					(scene->gameset)->setEnabled(true);
					(scene->mainMenu)->setEnabled(true);
				}


			}

			layer->removeChild(menuBg,true);

			break;
	}
}
//拖拉条触摸事件
void GameSet::sliderEvent(Ref* pSender, SliderEventType type)
{
	if (type == SLIDER_PERCENTCHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(pSender);//将pSender转换为Slider类型
		int percent = slider->getPercent();
		Constant::sensitivity=percent;
	}
}
