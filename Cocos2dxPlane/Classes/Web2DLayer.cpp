#include "Web2DLayer.h"
#include "AppMacros.h"
#include "Constant.h"
#include "PlaySound.h"
#include "GameData.h"
#include "BNSocketUtil.h"
#include <string>

using namespace cocos2d;
using namespace std;

static Web2DLayer* layer=nullptr;
Web2DLayer *Web2DLayer::getInstance()
{
	if(!layer){
		return nullptr;
	}
	return layer;
}
//ʵ��Web2DLayer���е�init��������ʼ������
bool Web2DLayer::init()
{
	//���ø���ĳ�ʼ��
    if ( !Layer::init() )
    {
        return false;
    }
    layer=this;
    // �÷ֱ�ǩ
    Sprite *score=Sprite::create(GAME_PATH+std::string("defen.png"));
    score->setAnchorPoint(Point(0,1));
    score->setPosition(Point(5,950));
    this->addChild(score,MENU_LEVEL);
    //*****************��Ŀ******************
	goldLabel=LabelAtlas::create("0",GAME_PATH+std::string("numbers.png"),21,40,'0');
	goldLabel->setAnchorPoint(Point(0,1));
	goldLabel->setPosition(Vec2(100,950));
	this->addChild(goldLabel,MENU_LEVEL);
    //��ҽ����� Ѫ��
    Sprite* hpFrame=Sprite::create(GAME_PATH+std::string("progress_game_bk.png"));
	hpFrame->setAnchorPoint(Point(0,1));
	hpFrame->setPosition(Point(210,938));
	this->addChild(hpFrame,MENU_LEVEL);

    loadingbar_plane=LoadingBar::create();
    loadingbar_plane->setAnchorPoint(Point(0,1));
    loadingbar_plane->loadTexture(GAME_PATH+std::string("progress_game_track.png"));
    loadingbar_plane->setPercent(100);
    loadingbar_plane->setPosition(Point(210,938));
	this->addChild(loadingbar_plane,MENU_LEVEL+1);

	//��һλ��ұ���
	Sprite *bg=Sprite::create(NET_PATH+std::string("bg.png"));
	bg->setAnchorPoint(Point(0,0));
	bg->setPosition(Point(10,800));
	this->addChild(bg,MENU_LEVEL);
	//��һλ��ҵ�ͼ��
	int planeIndex;
	if(GameData::playerNumber==1){
		planeIndex=GameData::playerPlaneId;
	}else{
		planeIndex=GameData::housePlaneId;
	}
	Sprite3D *headPlane=Sprite3D::create(
			C3B_PATH+StringUtils::format("plane0%d.obj",planeIndex+1),C3B_PATH+StringUtils::format("plane0%d.jpg",planeIndex+1));
	headPlane->setRotation3D(Vec3(-10,10,0));
	headPlane->setScale(0.55);
	headPlane->setPosition(Point(60,850));
	this->addChild(headPlane,MENU_LEVEL);

	//���Ž������ı���
	Sprite *progressBG=Sprite::create(NET_PATH+std::string("progress_net_bk.png"));
	progressBG->setPosition(Point(10,800));
	progressBG->setAnchorPoint(Vec2(0,1));
	this->addChild(progressBG,MENU_LEVEL);
	//��һλ��ҵĽ�����
	progressOPlane=LoadingBar::create();
	progressOPlane->setAnchorPoint(Point(0,1));
	progressOPlane->loadTexture(NET_PATH+std::string("progress_net_track.png"));
	progressOPlane->setPercent(100);
	progressOPlane->setPosition(Point(10,800));
	this->addChild(progressOPlane,MENU_LEVEL+1);

	auto director = Director::getInstance();
	auto sched = director->getScheduler();
	sched->scheduleSelector(SEL_SCHEDULE(&Web2DLayer::update),this,0.01,false);

	//*****************��ͣ****************
	MenuItemImage* pause=MenuItemImage::create(
			GAME_PATH+std::string("pause.png"),
			MENU_PATH+std::string("pause_down.png"),
			CC_CALLBACK_0(Web2DLayer::goPause,this)

	);
	pause->setScale(1.2f);
	pause->setAnchorPoint(Point(1,1));
	pause->setPosition(Vec2(540,955));

	CCLOG("---------------------------------------11");
	MenuItemImage* attack=MenuItemImage::create(
			GAME_PATH+std::string("attack.png"),
			GAME_PATH+std::string("attack_down.png"),
			CC_CALLBACK_0(Web2DLayer::useSkill,this,1)

	);
	attack->setScale(1.2f);
	attack->setAnchorPoint(Point(0,0));
	attack->setPosition(Vec2(0,0));

	//��������
	MenuItemImage* protect=MenuItemImage::create(
			GAME_PATH+std::string("protect.png"),
			GAME_PATH+std::string("protect_down.png"),
			CC_CALLBACK_0(Web2DLayer::useSkill,this,2)

	);
	protect->setScale(1.2f);
	protect->setAnchorPoint(Point(1,0));
	protect->setPosition(Vec2(540,0));

	//��������ʱ�ı���
	deadBigBG=MenuItemImage::create(
			NET_PATH+std::string("deadBig.png"),
			NET_PATH+std::string("deadBig.png")
	);
	deadBigBG->setVisible(false);
	deadBigBG->setPosition(Vec2(270,480));
	//�����Է�������ͼƬ
	deadSmallBG=Sprite::create(NET_PATH+std::string("deadSmall.png"));
	deadSmallBG->setPosition(Point(10,800));
	deadSmallBG->setAnchorPoint(Point(0,0));
	this->addChild(deadSmallBG,MENU_LEVEL+1);
	deadSmallBG->setVisible(false);

	menu=Menu::create(pause,attack,protect,deadBigBG,nullptr);
	menu->setPosition(Point::ZERO);
	this->addChild(menu,MENU_LEVEL);

	//�����ӵ������Ľ�����
	progress=Sprite::create(GAME_PATH+std::string("sliderTrack_game.png"));
	progress->setAnchorPoint(Point(0,1));
	progress->setPosition(Point(150,50));
	progress->setVisible(false);
	this->addChild(progress,MENU_LEVEL);

	loadingbar=LoadingBar::create();
	loadingbar->setAnchorPoint(Point(0.5,1));
	loadingbar->loadTexture(GAME_PATH+std::string("sliderProgress_game.png"));
	loadingbar->setPercent(100);
	loadingbar->setPosition(Point(270,50));
	loadingbar->setVisible(false);
	this->addChild(loadingbar,MENU_LEVEL+1);

	//��������
	attackLabel=LabelAtlas::create("0",GAME_PATH+std::string("nums.png"),12,14,'0');
	attackLabel->setAnchorPoint(Point(0,0));
	attackLabel->setPosition(Vec2(62,10));
	attack->addChild(attackLabel,MENU_LEVEL);
	//��������
	protectLabel=LabelAtlas::create("0",GAME_PATH+std::string("nums.png"),12,14,'0');
	protectLabel->setAnchorPoint(Point(0,0));
	protectLabel->setPosition(Vec2(12,10));
	protect->addChild(protectLabel,MENU_LEVEL);

	Sprite *gameCustom=Sprite::create(GAME_PATH+StringUtils::format("game%d.png",GameData::level).c_str());
	gameCustom->setPosition(270,1100);
	this->addChild(gameCustom,MENU_LEVEL);

	ActionInterval *open=Sequence::create(EaseBounceOut::create(MoveBy::create(0.5,Vec2(0,-500))),
					DelayTime::create(1),
					MoveBy::create(0.5,Vec2(0,500)),
					nullptr);
	gameCustom->runAction(Sequence::create(open,RemoveSelf::create(),nullptr));

	if(GameData::level==1){
		createCloud();				//�����Ʋ�
	}


	//����ʱȫ������
	injureBG=Sprite::create(GAME_PATH+std::string("injuredBG.png"));
	this->addChild(injureBG,MENU_LEVEL+1);
	injureBG->setPosition(Vec2(270,480));
	injureBG->setVisible(false);
	//���ű�������
	PlaySound::playBackgroundG();
    return true;
}
//ʹ�ü���
void Web2DLayer::useSkill(int number)
{
	switch(number)
	{
	case 1:
		if(GameData::playerNumber==0){
			if(GameData::redAttackCount>0 &&!GameData::isRedAttack)
			{
				BNSocketUtil::sendInt(GameData::SEND_USESKILL);
				BNSocketUtil::sendInt(0);
			}
		}else{
			if(GameData::greenAttackCount>0 &&!GameData::isGreenAttack)
			{
				BNSocketUtil::sendInt(GameData::SEND_USESKILL);
				BNSocketUtil::sendInt(0);
			}
		}
		break;
	case 2:
		if(GameData::playerNumber==0){
			if(GameData::redProtectCount>0 &&!GameData::isRedProtect)
			{
				BNSocketUtil::sendInt(GameData::SEND_USESKILL);
				BNSocketUtil::sendInt(1);

			}
		}else{
			if(GameData::greenProtectCount>0 && !GameData::isGreenProtect)
			{
				BNSocketUtil::sendInt(GameData::SEND_USESKILL);
				BNSocketUtil::sendInt(1);

			}
		}
		break;
	}
}
//�����Ʋ�
void Web2DLayer::createCloud(){
	for(int i=0;i<4;i++){
		cloud[i]=Sprite::create(CLOUD_PATH+StringUtils::format("zbg%d.png",i).c_str());
		if(i%2==0){
			cloud[i]->setPosition(Point(100,(i+1)*1000));
		}else{
			cloud[i]->setPosition(Point(440,(i+1)*1000));
		}
		cloud[i]->setAnchorPoint(Point(0.5,1));
		cloud[i]->setScale(2);
		this->addChild(cloud[i],MENU_LEVEL-1);
	}
}
//��ͣ����
void Web2DLayer::goPause()
{
	if(GameData::isPause)
	{
		return;
	}
	BNSocketUtil::sendInt(GameData::SEND_STATE);
	BNSocketUtil::sendInt(2);			// ��Ϸ��ͣ
}
//�����Ʋʣ��÷֣���ҵ���ת
void Web2DLayer::update()
{
	angle++;
	if(angle>=360)
	{
		angle=0;
	}
	if(GameData::level==1){				//�ڶ���
		//�Ʋ��ƶ�
		for(int i=0;i<4;i++){
			float cloudY=cloud[i]->getPosition().y-1;
			cloud[i]->setPositionY(cloudY);
		}
		if(cloud[3]->getPosition().y<=0){
			for(int i=0;i<4;i++){
				int random=rand()%5;
				cloud[i]->setPosition(Point(100*i,(i+1)*1000));
		}
	}}
	//�÷�
	int aa=GameData::totalScore;
	stringstream ss;
	ss<<aa;
	string s1 = ss.str();
	goldLabel->setString(s1);

	//���и���
	int bb,cc;
	if(GameData::playerNumber==0){
		bb=GameData::redAttackCount;
		cc=GameData::redProtectCount;
	}else{
		bb=GameData::greenAttackCount;
		cc=GameData::greenProtectCount;
	}
	stringstream sss;
	sss<<bb;
	string s11 = sss.str();
	attackLabel->setString(s11);

	//��������
	stringstream ss2;
	ss2<<cc;
	string ss1 = ss2.str();
	protectLabel->setString(ss1);

	float playPlaneHealth=GameData::redPlaneHealth;		//��ҷɻ�������ֵ
	float housePlaneHealth=GameData::greenPlaneHealth;	//�����ɻ�������ֵ
	float playPercent=playPlaneHealth/2.0;				//��ȡ����ֵ
	float housePercent=housePlaneHealth/2.0;

	if(!GameData::isRedLift)
	{
		if(GameData::playerNumber==0){
			deadBigBG->setVisible(true);
		}else{
			deadSmallBG->setVisible(true);
		}
	}
	if(!GameData::isGreenLift)
	{
		if(GameData::playerNumber==0){
			deadSmallBG->setVisible(true);
		}else{
			deadBigBG->setVisible(true);
		}
	}
	if(GameData::playerNumber==0){						//���
		if(GameData::oldPlaneHealth!=playPlaneHealth){
			sharkInjuredBG();
			GameData::oldPlaneHealth=playPlaneHealth;
		}
		loadingbar_plane->setPercent(playPercent);		//��������ֵ�İٷֱ�
		progressOPlane->setPercent(housePercent);
	}else {
		if(GameData::oldPlaneHealth!=housePlaneHealth){
			sharkInjuredBG();
			GameData::oldPlaneHealth=housePlaneHealth;
		}
		loadingbar_plane->setPercent(housePercent);		//��������ֵ�İٷֱ�
		progressOPlane->setPercent(playPercent);
	}
	//�ӵ��ӳ�ʱ��ĸ���
	if(Constant::isBulletPower){
		Constant::bulletPowerTime--;
		loadingbar->setVisible(true);
		progress->setVisible(true);
		changePro();									//�������ĸ���
		if(Constant::bulletPowerTime==0){
			Constant::isBulletPower=false;				//û���ӵ�����
			Constant::bulletPower=0;					//�ӵ�����Ϊ0
			loadingbar->setVisible(false);
			progress->setVisible(false);
	}}
	if(GameData::gameState==2&&!Constant::isPause){
		Constant::isPause=true;
		initPauseCode();
	}
	if(GameData::gameState==1&&Constant::isPause)
	{
		Constant::isPause=false;
		continueCallback();
	}
	if(GameData::gameState==3&&!Constant::isPause){		//��Ϸʤ��
		Constant::isPause=true;
		CCLOG("win----");
		deadBigBG->setVisible(false);
		deadSmallBG->setVisible(false);
		layer3D->removeAllChildren();
		layer3D->unscheduleAllSelectors();
		layer3D->pause();
		this->unscheduleAllSelectors();
		this->addChild(CompleteGameLayer::create(1),1000);
	}
	if(GameData::gameState==4&&!Constant::isPause){		//��Ϸʧ��
		Constant::isPause=true;
		CCLOG("lose----");
		deadBigBG->setVisible(false);
		deadSmallBG->setVisible(false);
		layer3D->removeAllChildren();
		layer3D->unscheduleAllSelectors();
		layer3D->pause();
		this->unscheduleAllSelectors();
		this->addChild(CompleteGameLayer::create(0),1000);
	}
    if(!GameData::connectFlag)
    {
        GameData::reset();
        Constant::isPause=false;
        Director::getInstance()->resume();
        gsm->goMainMenu();
    }
}
// ��������
void Web2DLayer::sharkInjuredBG()
{
	injureBG->setVisible(true);
	injureBG->runAction(Sequence::create(
				FadeOut::create(0.3),FadeIn::create(0.3),
				CallFunc::create(CC_CALLBACK_0(Web2DLayer::setVisibleBG,this)),
				nullptr
				));
}
//�������˵ı���
void Web2DLayer::setVisibleBG()
{
	injureBG->setVisible(false);
}
//�ı������
void Web2DLayer::changePro()
{
	loadingbar->setVisible(true);
	float percent=Constant::bulletPowerTime/1000.0*100;
	loadingbar->setPercent(percent);
}
//��ʼ����ͣ����
void Web2DLayer::initPauseCode()
{
	menu->setEnabled(false);
	GameData::isPause=true;

	PlaySound::playSound(kMenuSure);
	PlaySound::pauseBackground();					//��ͣ��������
	//��ͣ����
	Device::setAccelerometerEnabled(false);			//��������������

	layer3D->pause();

	//**************��ͣ��Ϸ����**************
	pauseBG=Sprite::create(GAMEMODE_PATH+std::string("MenuBg.png"));
	pauseBG->setPosition(Point(270,480));
	this->addChild(pauseBG,MENU_LEVEL);

	Sprite *pause=Sprite::create(PAUSE_PATH+std::string("pauseBG.png"));
	pause->setPosition(Point(270,520));
	pauseBG->addChild(pause,MENU_LEVEL);

	//**************������Ϸ**************
	continueGame=MenuItemImage::create(
			PAUSE_PATH+std::string("continueGame_normal.png"),
			PAUSE_PATH+std::string("continueGame_pressed.png"),
			CC_CALLBACK_0(Web2DLayer::continueAnim,this)
			);
	continueGame->setPosition(Point(270,580));

	//**************��Ϸ���� **************
	gameset=MenuItemImage::create(
			PAUSE_PATH+std::string("setGame_normal.png"),
			PAUSE_PATH+std::string("setGame_pressed.png"),
			CC_CALLBACK_0(Web2DLayer::setGame,this)
			);
	gameset->setPosition(Point(270,480));

	//**************�ص����˵�**************
	mainMenu=MenuItemImage::create(
			PAUSE_PATH+std::string("mainMenu_normal.png"),
			PAUSE_PATH+std::string("mainMenu_pressed.png"),
			CC_CALLBACK_0(Web2DLayer::goMainMenu,this)
			);

	mainMenu->setPosition(Point(270,380));
	Menu *menu=Menu::create(continueGame,gameset,mainMenu,nullptr);

	menu->setPosition(Point::ZERO);

	pauseBG->addChild(menu,MENU_LEVEL);
}
//������Ϸ����
void Web2DLayer::continueAnim()
{
	this->removeChild(pauseBG,true);

	continuePauseBG=Sprite::create(GAMEMODE_PATH+std::string("MenuBg.png"));
	continuePauseBG->setPosition(Point(270,480));
	this->addChild(continuePauseBG,MENU_LEVEL+1);

	waitting=Sprite::create(NET_PATH+std::string("waiting.png"));				//�ȴ���...
	continuePauseBG->addChild(waitting);
	waitting->setPosition(270,480);
	waitting->runAction(RepeatForever::create(RotateBy::create(1.0,360)));

	BNSocketUtil::sendInt(GameData::SEND_CONTINUE);								//������Ϸ
}
void Web2DLayer::continueCallback()
{
	CCLOG("  ----------------continueCallback");
	this->removeChild(continuePauseBG,true);
	//����ͣ��־��Ϊfalse
	GameData::isPause=false;
	menu->setEnabled(true);
	layer3D->resume();
	Device::setAccelerometerEnabled(true);			//��������������
	PlaySound::resumeBackground();
}
//��ת�����˵�����
void Web2DLayer::goMainMenu()
{
    BNSocketUtil::sendInt(GameData::SEND_EXIT);
//    GameData::reset();
//    Constant::isPause=false;
//    Director::getInstance()->resume();
//    gsm->goMainMenu();
}
//��ͣ����
void Web2DLayer::pauseGame()
{
//	this->pause();
//	layer3D->pause();
//	layer3D->clearAllBullets();
}
void Web2DLayer::completeGame(int flag)
{
//	this->pause();
//	layer3D->pause();
//	Constant::gameingFlag=false;
//	CompleteGameLayer *cl=CompleteGameLayer::create(flag);
//	this->addChild(cl,1000);
}
//��Ϸ����
void Web2DLayer::setGame()
{
	continueGame->setEnabled(false);
	gameset->setEnabled(false);
	mainMenu->setEnabled(false);
	gameSet=new GameSet(this);
	gameSet->createSet(1);
}

void CompleteGameLayer::goMainMenu()
{
	Web2DLayer *ml=dynamic_cast<Web2DLayer*>(this->getParent());
	GameData::reset();
	Constant::isPause=false;
	ml->gsm->goMainMenu();
	Director::getInstance()->resume();
}

void CompleteGameLayer::goNext()
{
	Web2DLayer *ml=dynamic_cast<Web2DLayer*>(this->getParent());
	GameData::reset();
	Constant::isPause=false;
	ml->gsm->goSelectGame();
	Director::getInstance()->resume();
}

void CompleteGameLayer::goSelf()
{
	Web2DLayer *ml=dynamic_cast<Web2DLayer*>(this->getParent());
	GameData::reset();
	Constant::isPause=false;
	ml->gsm->goSelectGame();
	Director::getInstance()->resume();
}

void CompleteGameLayer::updateWeb()
{
	if(GameData::gameState==GameData::GAME_WAIT)
	{
		goNext();
	}else if(GameData::connectFlag==false)
	{
		goMainMenu();
	}
}

void CompleteGameLayer::menuCallback(int index)
{
	switch(index)
	{
	case 0:
		BNSocketUtil::sendInt(GameData::SEND_STATE);
		BNSocketUtil::sendInt(GameData::GAME_WAIT);
		break;
	case 1:
		BNSocketUtil::sendInt(GameData::SEND_STATE);
		BNSocketUtil::sendInt(5);
		break;
	case 2:
		BNSocketUtil::sendInt(GameData::SEND_EXIT);
		break;
	}
}

void CompleteGameLayer::updateCoin()
{
	CCLOG("--------------------------------1    0");
	if(flag==1)
	{
		coinCounter+=coinStep;
		if(coinCounter<=GameData::totalScore)
		{
			stringstream ss;
			ss<<(coinCounter/10);
			string s = ss.str();
			goldLabel->setString(s);
			return;
		}
		stringstream ss;
		ss<<(GameData::totalScore/10);
		string s = ss.str();
		goldLabel->setString(s);
		GameData::money+=GameData::totalScore/10;
		UserDefault::getInstance()->setIntegerForKey("money",GameData::money);
		CCLOG("--------------------------------1    1");
	}
	this->unscheduleAllSelectors();
	//***************δ���*********************
	Sprite *grade;
	if(flag==0)
	{
		grade=Sprite::create(MENU_PATH+std::string("incomplete.png"));
		grade->setPosition(320,30);
	}else if(flag==1)
	{
		int i=5-GameData::totalScore/1000;
		if(i<0)
		{
			i=0;
		}
		grade=Sprite::create(
			MENU_PATH+std::string(
				StringUtils::format("pf%d.png",i)
		));
		grade->setPosition(310,30);
		grade->setRotation(-30);
	}
	CCLOG("--------------------------------1    2");
	board->addChild(grade,2);
	grade->setScale(20);
	//�ȼ����۵Ķ���
	ActionInterval *scaleTo=ScaleTo::create(0.5,1.0);				//��С����
	ActionInterval *esi=EaseQuadraticActionIn::create(scaleTo);		//��������
	CallFunc *cf=CallFunc::create(CC_CALLBACK_0(CompleteGameLayer::moveBoard,this));
	Sequence *s2=Sequence::create(esi,cf,nullptr);
	grade->runAction(s2);
	CCLOG("--------------------------------1    3");
}

void CompleteGameLayer::moveBoard()
{
	//****************�ƶ�*********************
	ActionInterval *mt=MoveTo::create(1,Point(270,550));
	CallFunc *cf=CallFunc::create(CC_CALLBACK_0(CompleteGameLayer::setMenu,this));
	board->runAction(Sequence::create(mt,cf,nullptr));
	CCLOG("--------------------------------1    4");
}

void CompleteGameLayer::setMenu()
{
	this->schedule(SEL_SCHEDULE(&CompleteGameLayer::updateWeb),0.1);
	CCLOG("--------------------------------1    5");
	//****************�˵�*********************
	MenuItem *go;
	if(flag==0)
	{
		//������Ϸ
		go=MenuItemImage::create
		(
			MENU_PATH+std::string("reset.png"),
			MENU_PATH+std::string("reset_down.png"),
			MENU_PATH+std::string("reset_disabled.png"),
			CC_CALLBACK_0(CompleteGameLayer::menuCallback,this,0)
		);
	}
	else if(flag==1)
	{
		//������Ϸ
		go=MenuItemImage::create
		(
			MENU_PATH+std::string("continue.png"),
			MENU_PATH+std::string("continue_down.png"),
			MENU_PATH+std::string("continue_disabled.png"),
			CC_CALLBACK_0(CompleteGameLayer::menuCallback,this,1)
		);
	}
	go->setPosition(Point(-100,300));
	//�����˵�
	MenuItem *goMain=MenuItemImage::create
	(
		MENU_PATH+std::string("goMenu.png"),
		MENU_PATH+std::string("goMenu_down.png"),
		MENU_PATH+std::string("goMenu_disabled.png"),
		CC_CALLBACK_0(CompleteGameLayer::menuCallback,this,2)
	);
	goMain->setPosition(Point(640,200));
	//�˵�
	Menu *menu=Menu::create(go,goMain,nullptr);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);
	CCLOG("--------------------------------1    6");
	//�˵�����
	go->runAction(EaseElasticOut::create(MoveTo::create(0.75,Point(270,300))));
	ActionInterval *mt=EaseElasticOut::create(MoveTo::create(0.75,Point(270,200)));
	goMain->runAction(Sequence::create(DelayTime::create(0.25),mt,nullptr));
	if(GameData::playerNumber==0){
		go->setEnabled(false);
		goMain->setEnabled(false);
	}
}
//������Ϸ����
void CompleteGameLayer::setGameDate()
{
//	if(GameData::level==Constant::canPlay&&Constant::canPlay<=1){
//			GameData::canPlay++;
//			GameData::level++;
//			Constant::openCustom=true;
//		}
//	UserDefault::getInstance()->setIntegerForKey("custom",GameData::level);
//	UserDefault::getInstance()->setIntegerForKey("canPlay",Constant::canPlay);
}
bool CompleteGameLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}
	//**************���汳��******************
	Sprite* loseBG=Sprite::create(MENU_PATH+std::string("MenuBg.png"));
	loseBG->setPosition(Point(270,480));
	this->addChild(loseBG);
	//**************��Ϸ�����********************
	if(flag==0)
	{
		board = Sprite::create(MENU_PATH+std::string("gameLose.png"));
		board->setPosition(Point(270,450));
		this->addChild(board,1);
	}else if(flag==1)
	{
		board = Sprite::create(MENU_PATH+std::string("gameSuccess.png"));
		board->setPosition(Point(270,450));
		this->addChild(board,1);

		//������������С**********
		coinStep=(GameData::totalScore-50)/100+1;
		//�����ֵ****************
		goldLabel=LabelAtlas::create("0",GAME_PATH+std::string("numbers.png"),21,40,'0');
		goldLabel->setScale(1.2f);
		goldLabel->setPosition(Point(200,58));
		board->addChild(goldLabel);
	}

	//***************��ʱ�ص�********************
	this->schedule(SEL_SCHEDULE(&CompleteGameLayer::updateCoin),0.01);
	return true;
}

CompleteGameLayer* CompleteGameLayer::create(int _flag)
{
	CompleteGameLayer *completeGameLayer = new CompleteGameLayer();
	if(completeGameLayer)
	{
		completeGameLayer->flag=_flag;
		if(completeGameLayer->init())
		{
			completeGameLayer->autorelease();
			return completeGameLayer;
		}
	}
	CC_SAFE_DELETE(completeGameLayer);								//�����������δ�ɹ���������ȫɾ��
	return nullptr;
}
