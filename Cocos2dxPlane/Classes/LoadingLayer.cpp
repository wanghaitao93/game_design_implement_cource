#include "LoadingLayer.h"
#include "First3DLayer.h"
#include "My2DLayer.h"
#include "NewPlane.h"
#include "Missile.h"
#include "Enemies.h"
#include "Constant.h"
#include "SelectGameLayer.h"

using namespace cocos2d;
using namespace std;

bool LoadingLayer::init()
{
	if ( !Layer::init() )
	{
		return false;
	}

	InitBk();

	// 进度条背景
	Sprite *loadingBG=Sprite::create(std::string("load/progress_load_bk.png"));
	loadingBG->setPosition(Point(70,540));
	loadingBG->setAnchorPoint(Point(0,1));
	this->addChild(loadingBG,MENU_LEVEL);					//添加到上面的背景中

	loadingbar=LoadingBar::create();
	loadingbar->setAnchorPoint(Point(0,1));
	loadingbar->loadTexture(LOAD_PATH+std::string("progress_load_track.png"));
	loadingbar->setPercent(0);
	loadingbar->setPosition(Point(70,540));
	this->addChild(loadingbar,MENU_LEVEL+1);

	LoadingResource();
	return true;
}
//创建加载背景
void LoadingLayer::InitBk()
{
	//背景
	Sprite *back=Sprite::create(LOAD_PATH+std::string("loadingBG.jpg"));
	back->setPosition(Point(270,480));
	this->addChild(back,BACKGROUND_LEVEL);

}
//加载资源
void LoadingLayer::LoadingResource()
{
	LoadingPic();
}
//加载图片
void LoadingLayer::LoadingPic()
{
	auto TexureCache=Director::getInstance()->getTextureCache();
	for(std::string strTemp:picGameNames)
	{
		TexureCache->addImageAsync(GAME_PATH+strTemp,CC_CALLBACK_0(LoadingLayer::LoadingCallback,this));
	}
	for(std::string strTemp:picC3bNames)
	{
		TexureCache->addImageAsync(C3B_PATH+strTemp,CC_CALLBACK_0(LoadingLayer::LoadingCallback,this));
	}
	for(std::string strTemp:picGameModelNames)
	{
		TexureCache->addImageAsync(GAMEMODE_PATH+strTemp,CC_CALLBACK_0(LoadingLayer::LoadingCallback,this));
	}
	for(std::string strTemp:picHelpNames)
	{
		TexureCache->addImageAsync(HELP_PATH+strTemp,CC_CALLBACK_0(LoadingLayer::LoadingCallback,this));
	}
	for(std::string strTemp:picMenuNames)
	{
		TexureCache->addImageAsync(MENU_PATH+strTemp,CC_CALLBACK_0(LoadingLayer::LoadingCallback,this));
	}
	for(std::string strTemp:picNetNames)
	{
		TexureCache->addImageAsync(NET_PATH+strTemp,CC_CALLBACK_0(LoadingLayer::LoadingCallback,this));
	}
	for(std::string strTemp:picSgNames)
	{
		TexureCache->addImageAsync(SELECTGAME_PATH+strTemp,CC_CALLBACK_0(LoadingLayer::LoadingCallback,this));
	}
	for(std::string strTemp:picSelectNames)
	{
		TexureCache->addImageAsync(SELECT_PATH+strTemp,CC_CALLBACK_0(LoadingLayer::LoadingCallback,this));
	}
	for(int i=0;i<4;i++){
		TexureCache->addImageAsync(CLOUD_PATH+StringUtils::format("zbg%d.png",i).c_str(),CC_CALLBACK_0(LoadingLayer::LoadingCallback,this));
	}
	for(int i=0;i<5;i++){
		TexureCache->addImageAsync(PROTECT_PATH+StringUtils::format("bh%d.png",i).c_str(),CC_CALLBACK_0(LoadingLayer::LoadingCallback,this));
	}
}

void LoadingLayer::LoadingCallback()
{
	++currentNum;
	int percent=(int)(((float)currentNum / totalNum) * 100);
    if(percent<100)
    {
        loadingbar->setPercent(percent);
    }
	CCLOG("--------------------------------currentNum=%d,totalNum=%d",currentNum,totalNum);
	if(currentNum==5){
		LoadingOBJ();
	}
	if(currentNum>=totalNum){
		CCLOG("--------------------------------total");
        this->unscheduleAllCallbacks();
		GotoNextScene();
	}
}
void LoadingLayer::LoadingOBJ()
{
	int i=0;
	objPath.push_back("c3b/boss1.c3b");
	objPath.push_back("c3b/boss2.c3b");
	objPath.push_back("c3b/boss3.c3b");
	objPath.push_back("c3b/coin.obj");
	objPath.push_back("c3b/enemy1.obj");
	objPath.push_back("c3b/enemy2.obj");
	objPath.push_back("c3b/enemy3.obj");
	objPath.push_back("c3b/enemy4.c3b");
	objPath.push_back("c3b/gun.obj");
	objPath.push_back("c3b/life.obj");
	objPath.push_back("c3b/Missile4.obj");
	objPath.push_back("c3b/plane01.obj");
	objPath.push_back("c3b/plane02.obj");
	objPath.push_back("c3b/plane03.obj");
	objPath.push_back("c3b/update.obj");
	for(const auto& path:objPath)
	{
		Sprite3D::createAsync(path,CC_CALLBACK_2(LoadingLayer::printf,this),(void*)i++);
	}
}
void LoadingLayer::printf(Sprite3D* sprite, void* index)
{
	CCLOG("-------------load  -----index=%d",index);
}
void LoadingLayer::GotoNextScene(){
	Constant::loaded=true;
	gsm->goMainMenu();
}
