#include "NetLayer.h"
#include "AppMacros.h"
#include "Constant.h"
#include "MainMenuScene.h"
#include "BNSocketUtil.h"
#include "GameData.h"

bool NetLayer::isConnecting=false;

bool NetLayer::init()
{
	//���ø���ĳ�ʼ��
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
//����ģʽ
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
//����ģʽ
void NetLayer::goNetModel()
{
	new std::thread(&BNSocketUtil::threadConnectTask,"192.168.253.1", 9999);
	waitting=Sprite::create(std::string("net/waiting.png"));				//�ȴ���...
	this->addChild(waitting);
	waitting->setPosition(270,480);
	waitting->runAction(RepeatForever::create(RotateBy::create(1.0,360)));
	isConnecting=true;
	schedule(schedule_selector(NetLayer::ConnectNet),0.3f);		//
	scheduleOnce(schedule_selector(NetLayer::breakConnect),5.0f);		//5���ִ��һ��
}
//�ж��Ƿ�������
void NetLayer::ConnectNet(float f)
{
	if(GameData::playerNumber!=-1){
        CCLOG("recept-----layer");
		isConnecting=false;
		if(GameData::playerCount==1){	//ֻ��һ��������
			this->removeChild(waitting,MENU_LEVEL);
			netModel->setVisible(false);
			netModel2->setVisible(true);
			waittingConnect=Sprite::create(std::string("net/waittingConnect0.png"));
			this->addChild(waittingConnect,MENU_LEVEL);
			waittingConnect->setPosition(Vec2(270,480));
			schedule(schedule_selector(NetLayer::selectPlane),0.2f);
			this->unschedule(SEL_SCHEDULE(&NetLayer::ConnectNet));
			oneModel->setEnabled(false);
		}else if(GameData::playerCount==2){	//������������
			this->unschedule(SEL_SCHEDULE(&NetLayer::ConnectNet));
			goSelectGame();
		}
	}
	if(GameData::serviceFull){				// �������������
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
//�Ͽ�����
void NetLayer::breakConnect(float f)
{
	isConnecting=false;
	if(GameData::connectFlag==false&&GameData::serviceFull==false){				//���û��������
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
// �ȴ�ѡ��ɻ�
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

