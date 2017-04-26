#include "NetLayer.h"
#include "AppMacros.h"
#include "Constant.h"
#include "MainMenuScene.h"
#include "BNSocketUtil.h"
#include "GameData.h"

bool NetLayer::isConnecting=false;

bool NetLayer::init()
{
	//调用父类的初始化
	if ( !Layer::init() )
	{
		return false;
	}
	Sprite *bg=Sprite::create(NET_PATH+std::string("background.png"));
	this->addChild(bg);
	bg->setPosition(Vec2(270,480));

	Sprite *board=Sprite::create(NET_PATH+std::string("board.png"));
	this->addChild(board);
	board->setPosition(Vec2(270,480));

	oneModel=MenuItemImage::create(
						NET_PATH+std::string("onePlayer.png"),
						NET_PATH+std::string("onePlayer.png"),
						CC_CALLBACK_0(NetLayer::goOneModel,this)
						);
	oneModel->setPosition(Vec2(0,90));

	netModel=MenuItemImage::create(
						NET_PATH+std::string("twoPlayer.png"),
						NET_PATH+std::string("twoPlayer.png"),
						CC_CALLBACK_0(NetLayer::goNetModel,this)
						);
	netModel->setPosition(Vec2(0,-90));

	netModel2=MenuItemImage::create(
						NET_PATH+std::string("twoPlayer_unenable.png"),
						NET_PATH+std::string("twoPlayer_unenable.png")
						);
	netModel2->setVisible(false);
	netModel2->setPosition(Vec2(0,-75));


	Menu *menu=Menu::create(oneModel,netModel,netModel2,nullptr);
	this->addChild(menu);


	return true;
}
//单机模式
void NetLayer::goOneModel()
{
	oneModel->runAction(
			Sequence::create(
				ScaleTo::create(0.15,1.2f),
				ScaleTo::create(0.15,1.0f),
				CallFunc::create(CC_CALLBACK_0(NetLayer::goSelectGame,this)),
				nullptr
			));
}
void NetLayer::goSelectGame()
{
//    gsm->goSelectGame();
	if(Constant::loaded){
		gsm->goSelectGame();
	}else{
		gsm->goLoading();
	}

}
//联网模式
void NetLayer::goNetModel()
{
	new std::thread(&BNSocketUtil::threadConnectTask,"192.168.253.1", 9999);
	waitting=Sprite::create(std::string("net/waiting.png"));				//等待中...
	this->addChild(waitting);
	waitting->setPosition(270,480);
	waitting->runAction(RepeatForever::create(RotateBy::create(1.0,360)));
	isConnecting=true;
	schedule(schedule_selector(NetLayer::ConnectNet),0.3f);		//
	scheduleOnce(schedule_selector(NetLayer::breakConnect),5.0f);		//5秒后执行一次
}
//判断是否连接上
void NetLayer::ConnectNet(float f)
{
	if(GameData::playerNumber!=-1){
        CCLOG("recept-----layer");
		isConnecting=false;
		if(GameData::playerCount==1){	//只有一个人连接
			this->removeChild(waitting,MENU_LEVEL);
			netModel->setVisible(false);
			netModel2->setVisible(true);
			waittingConnect=Sprite::create(std::string("net/waittingConnect0.png"));
			this->addChild(waittingConnect,MENU_LEVEL);
			waittingConnect->setPosition(Vec2(270,480));
			schedule(schedule_selector(NetLayer::selectPlane),0.2f);
			this->unschedule(SEL_SCHEDULE(&NetLayer::ConnectNet));
			oneModel->setEnabled(false);
		}else if(GameData::playerCount==2){	//连接了两个人
			this->unschedule(SEL_SCHEDULE(&NetLayer::ConnectNet));
			goSelectGame();
		}
	}
	if(GameData::serviceFull){				// 如果服务器已满
		isConnecting=false;
		this->unschedule(SEL_SCHEDULE(&NetLayer::ConnectNet));
		this->removeChild(waitting,MENU_LEVEL);
		Sprite *pointBoard=Sprite::create(std::string("net/servicerFull.png"));
		pointBoard->setPosition(Point(Vec2(270,480)));
		this->addChild(pointBoard,MENU_LEVEL);

		pointBoard->setScale(0.1f);
		pointBoard->setPosition(Vec2(270,480));
		pointBoard->runAction(
				Sequence::create(
						EaseBounceOut::create(ScaleTo::create(0.3f,1.0f)),
						DelayTime::create(0.5f),
						RemoveSelf::create(),
						nullptr
		));
	}
}
//断开连接
void NetLayer::breakConnect(float f)
{
	isConnecting=false;
	if(GameData::connectFlag==false&&GameData::serviceFull==false){				//如果没有连接上
		this->removeChild(waitting,MENU_LEVEL);
		this->unschedule(SEL_SCHEDULE(&NetLayer::ConnectNet));
		noConnectBoard=Sprite::create(std::string("net/noConnected.png"));
			this->addChild(noConnectBoard,MENU_LEVEL);
			noConnectBoard->setScale(0.1f);
			noConnectBoard->setPosition(Vec2(270,480));
			noConnectBoard->runAction(
					Sequence::create(
							EaseBounceOut::create(ScaleTo::create(0.3f,1.0f)),
							DelayTime::create(0.5f),
							RemoveSelf::create(),
	//						CallFunc::create(CC_CALLBACK_0(NetLayer::goNetModel,this)),
							nullptr
			));
	}
}
// 等待选择飞机
void NetLayer::selectPlane(float f)
{
	waitIndex++;
	if(waitIndex==4){
		waitIndex=0;
	}
	waittingConnect->setTexture(StringUtils::format("net/waittingConnect%d.png",waitIndex));
	if(GameData::playerCount==2){
		goSelectGame();
	}
}

