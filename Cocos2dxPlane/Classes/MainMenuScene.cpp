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

//ʵ��MainMenuScene���е�init��������ʼ������
bool MainMenuScene::init()
{
	//���ø���ĳ�ʼ��
    if ( !Layer::init() )
    {
        return false;
    }

    addMenuItem();
    //����һ���������
    back1 = Sprite::create(MAP_PATH+std::string("mainLayerBg.jpg"));
    back1->setPosition(Point(270,0));
    back1->setAnchorPoint(Point(0.5,0));
    this->addChild(back1,BACKGROUND_LEVEL);
    title = Sprite::create(GAME_PATH+std::string("gameTitle2.png"));
    title->setPosition(Point(270,730));
	this->addChild(title,2);
    

	//����һ�����㴥������
	auto listener = EventListenerTouchOneByOne::create();
	//�����´�����
	listener->setSwallowTouches(true);
	//��ʼ����ʱ�ص�onTouchBegan����
	listener->onTouchBegan = CC_CALLBACK_2(MainMenuScene::onMyTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(MainMenuScene::onMyTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(MainMenuScene::onMyTouchEnded, this);
//	//��ӵ�������
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    //����ϵͳ
//    ParticleManager* pm=new ParticleManager(this);
//    pm->initRain();

    return true;
}
void MainMenuScene::addMenuItem()
{
    //������ʼ�˵���
	menuItem[0] = MenuItemImage::create
    (
		   MENU_PATH+std::string("start.png"),			//ƽʱ��ͼƬ
		   MENU_PATH+std::string("start_btn.png"),		//ѡ��ʱ��ͼƬ
		   CC_CALLBACK_0(MainMenuScene::menuSelect, this,0) //���ʱִ�еĻص�����
	);
    menuItem[0]->setPosition(Point(-270,500));

    //ս��ǿ��
    menuItem[1] = MenuItemImage::create
    (
		   MENU_PATH+std::string("strengthen.png"),		//ƽʱ��ͼƬ
		   MENU_PATH+std::string("strengthen_down.png"),//ѡ��ʱ��ͼƬ
		   CC_CALLBACK_0(MainMenuScene::menuSelect, this,1) //���ʱִ�еĻص�����
	);
    menuItem[1]->setPosition(Point(810,380));
    //����
    menuItem[2] = MenuItemImage::create
    (
		   MENU_PATH+std::string("help.png"),			//ƽʱ��ͼƬ
		   MENU_PATH+std::string("help_down.png"),		//ѡ��ʱ��ͼƬ
		   CC_CALLBACK_0(MainMenuScene::menuSelect, this,3) //���ʱִ�еĻص�����
	);
    menuItem[2]->setPosition(Point(-270,260));

    exitItem = MenuItemImage::create
    (
		   MENU_PATH+std::string("close_btn.png"),			//ƽʱ��ͼƬ
		   MENU_PATH+std::string("close_btn_down.png"),		//ѡ��ʱ��ͼƬ
		   CC_CALLBACK_0(MainMenuScene::menuSelect,this,4)	//���ʱִ�еĻص�����
	);
    exitItem->setPosition(Point(45,50));
    //����
    setItem = MenuItemImage::create
    (
		   MENU_PATH+std::string("set_btn.png"),			//ƽʱ��ͼƬ
		   MENU_PATH+std::string("set_btn_down.png"),		//ѡ��ʱ��ͼƬ
		   CC_CALLBACK_0(MainMenuScene::menuSelect, this,2) //���ʱִ�еĻص�����
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

	//����
    soundItem = MenuItemImage::create
    (
    		MENU_PATH+voicePicName,					//ƽʱ��ͼƬ
    		MENU_PATH+voicePicNameP,				//ѡ��ʱ��ͼƬ
		   CC_CALLBACK_0(MainMenuScene::menuSelect, this,5) 	//���ʱִ�еĻص�����
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
		PlaySound::playSound(kButtonGo);							// ���ų�ʼ����ť������
	}


}
//��������Ǹ�����
void MainMenuScene::menuSelect(int index)
{
	if(index<6){							//��������
		PlaySound::playSound(kMenuSure);
	}else if(index==6){						//ѡ������
		PlaySound::playSound(kMenuSelect);
	}else if(index==7){						//����
		PlaySound::playSound(kCancel);
	}
	SpriteFrame *touchFrame;
	SpriteFrame *gravityFrame;
	SpriteFrame *soundFrame;
	SpriteFrame *soundFrameP;
	//��ת����
	switch(index){
		case 0:						//������Ϸ
			netLayer = NetLayer::create();
			this->addChild(netLayer,MENU_LEVEL);
			isSelectModel=true;
			menu->setEnabled(false);
			break;
		case 1:						//ս��ǿ��
			gsm->goWeapon();
			break;
		case 2:						//����
			createSetBox();
			for(int i=0;i<4;i++)
			{
				menuItem[i]->setEnabled(false);
			}

			exitItem->setEnabled(false);
			soundItem->setEnabled(false);
			break;
		case 3:						//����
			gsm->goHelp();
			break;
		case 4:						//�˳���Ϸ
			Director::getInstance()->end();
			break;
		case 5:						//����
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
		case 6:						//����������ر�
			if(!Constant::touchFlag){
				touchFrame=SpriteFrame::create(MENU_PATH+std::string("close.png"),Rect(0,0,68,29));

				gravityFrame=SpriteFrame::create(MENU_PATH+std::string("open.png"),Rect(0,0,68,29));
			}else{					//�����ر�ʱ������Ӧ����
				touchFrame=SpriteFrame::create(MENU_PATH+std::string("open.png"),Rect(0,0,68,29));

				gravityFrame=SpriteFrame::create(MENU_PATH+std::string("close.png"),Rect(0,0,68,29));
			}
			Constant::touchFlag=!Constant::touchFlag;

			touchItem->setNormalSpriteFrame(touchFrame);
			touchItem->setSelectedSpriteFrame(touchFrame);

			gravityItem->setNormalSpriteFrame(gravityFrame);
			gravityItem->setSelectedSpriteFrame(gravityFrame);
			break;
		case 7:						//���ùر�
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
//�������ô���
void MainMenuScene::createSetBox()
{
	setGame=new GameSet(this);
	setGame->createSet(0);
}
//������ʼ
bool MainMenuScene::onMyTouchBegan(Touch* touch, Event* event)
{
	return true;
}
//����
void MainMenuScene::onMyTouchMoved(Touch *touch, Event *event)
{
	auto target = static_cast<Sprite*>(	event->getCurrentTarget());//��ȡ��ǰ�������󣬲�ת��Ϊ��������
	Point location = target->convertToNodeSpace(touch->getLocation());//��ȡ��ǰ����
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
