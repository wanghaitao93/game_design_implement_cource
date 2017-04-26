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
//实现Web2DLayer类中的init方法，初始化布景
bool Web2DLayer::init()
{
	//调用父类的初始化
    if ( !Layer::init() )
    {
        return false;
    }
    layer=this;
    // 得分标签
    Sprite *score=Sprite::create(GAME_PATH+std::string("defen.png"));
    score->setAnchorPoint(Point(0,1));
    score->setPosition(Point(5,950));
    this->addChild(score,MENU_LEVEL);
    //*****************数目******************
	goldLabel=LabelAtlas::create("0",GAME_PATH+std::string("numbers.png"),21,40,'0');
	goldLabel->setAnchorPoint(Point(0,1));
	goldLabel->setPosition(Vec2(100,950));
	this->addChild(goldLabel,MENU_LEVEL);
    //玩家进度条 血量
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

	//另一位玩家背景
	Sprite *bg=Sprite::create(NET_PATH+std::string("bg.png"));
	bg->setAnchorPoint(Point(0,0));
	bg->setPosition(Point(10,800));
	this->addChild(bg,MENU_LEVEL);
	//另一位玩家的图表
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

	//竖着进度条的背景
	Sprite *progressBG=Sprite::create(NET_PATH+std::string("progress_net_bk.png"));
	progressBG->setPosition(Point(10,800));
	progressBG->setAnchorPoint(Vec2(0,1));
	this->addChild(progressBG,MENU_LEVEL);
	//另一位玩家的进度条
	progressOPlane=LoadingBar::create();
	progressOPlane->setAnchorPoint(Point(0,1));
	progressOPlane->loadTexture(NET_PATH+std::string("progress_net_track.png"));
	progressOPlane->setPercent(100);
	progressOPlane->setPosition(Point(10,800));
	this->addChild(progressOPlane,MENU_LEVEL+1);

	auto director = Director::getInstance();
	auto sched = director->getScheduler();
	sched->scheduleSelector(SEL_SCHEDULE(&Web2DLayer::update),this,0.01,false);

	//*****************暂停****************
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

	//防御方法
	MenuItemImage* protect=MenuItemImage::create(
			GAME_PATH+std::string("protect.png"),
			GAME_PATH+std::string("protect_down.png"),
			CC_CALLBACK_0(Web2DLayer::useSkill,this,2)

	);
	protect->setScale(1.2f);
	protect->setAnchorPoint(Point(1,0));
	protect->setPosition(Vec2(540,0));

	//创建死亡时的背景
	deadBigBG=MenuItemImage::create(
			NET_PATH+std::string("deadBig.png"),
			NET_PATH+std::string("deadBig.png")
	);
	deadBigBG->setVisible(false);
	deadBigBG->setPosition(Vec2(270,480));
	//创建对方死亡的图片
	deadSmallBG=Sprite::create(NET_PATH+std::string("deadSmall.png"));
	deadSmallBG->setPosition(Point(10,800));
	deadSmallBG->setAnchorPoint(Point(0,0));
	this->addChild(deadSmallBG,MENU_LEVEL+1);
	deadSmallBG->setVisible(false);

	menu=Menu::create(pause,attack,protect,deadBigBG,nullptr);
	menu->setPosition(Point::ZERO);
	this->addChild(menu,MENU_LEVEL);

	//创建子弹威力的进度条
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

	//攻击个数
	attackLabel=LabelAtlas::create("0",GAME_PATH+std::string("nums.png"),12,14,'0');
	attackLabel->setAnchorPoint(Point(0,0));
	attackLabel->setPosition(Vec2(62,10));
	attack->addChild(attackLabel,MENU_LEVEL);
	//防御个数
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
		createCloud();				//创建云彩
	}


	//受伤时全屏闪动
	injureBG=Sprite::create(GAME_PATH+std::string("injuredBG.png"));
	this->addChild(injureBG,MENU_LEVEL+1);
	injureBG->setPosition(Vec2(270,480));
	injureBG->setVisible(false);
	//播放背景音乐
	PlaySound::playBackgroundG();
    return true;
}
//使用技能
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
//创建云彩
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
//暂停界面
void Web2DLayer::goPause()
{
	if(GameData::isPause)
	{
		return;
	}
	BNSocketUtil::sendInt(GameData::SEND_STATE);
	BNSocketUtil::sendInt(2);			// 游戏暂停
}
//更新云彩，得分，金币的旋转
void Web2DLayer::update()
{
	angle++;
	if(angle>=360)
	{
		angle=0;
	}
	if(GameData::level==1){				//第二关
		//云彩移动
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
	//得分
	int aa=GameData::totalScore;
	stringstream ss;
	ss<<aa;
	string s1 = ss.str();
	goldLabel->setString(s1);

	//大招个数
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

	//防御个数
	stringstream ss2;
	ss2<<cc;
	string ss1 = ss2.str();
	protectLabel->setString(ss1);

	float playPlaneHealth=GameData::redPlaneHealth;		//玩家飞机的生命值
	float housePlaneHealth=GameData::greenPlaneHealth;	//房主飞机的生命值
	float playPercent=playPlaneHealth/2.0;				//获取生命值
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
	if(GameData::playerNumber==0){						//玩家
		if(GameData::oldPlaneHealth!=playPlaneHealth){
			sharkInjuredBG();
			GameData::oldPlaneHealth=playPlaneHealth;
		}
		loadingbar_plane->setPercent(playPercent);		//设置生命值的百分比
		progressOPlane->setPercent(housePercent);
	}else {
		if(GameData::oldPlaneHealth!=housePlaneHealth){
			sharkInjuredBG();
			GameData::oldPlaneHealth=housePlaneHealth;
		}
		loadingbar_plane->setPercent(housePercent);		//设置生命值的百分比
		progressOPlane->setPercent(playPercent);
	}
	//子弹加成时间的更改
	if(Constant::isBulletPower){
		Constant::bulletPowerTime--;
		loadingbar->setVisible(true);
		progress->setVisible(true);
		changePro();									//进度条的更改
		if(Constant::bulletPowerTime==0){
			Constant::isBulletPower=false;				//没有子弹威力
			Constant::bulletPower=0;					//子弹威力为0
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
	if(GameData::gameState==3&&!Constant::isPause){		//游戏胜利
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
	if(GameData::gameState==4&&!Constant::isPause){		//游戏失败
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
// 闪动受伤
void Web2DLayer::sharkInjuredBG()
{
	injureBG->setVisible(true);
	injureBG->runAction(Sequence::create(
				FadeOut::create(0.3),FadeIn::create(0.3),
				CallFunc::create(CC_CALLBACK_0(Web2DLayer::setVisibleBG,this)),
				nullptr
				));
}
//设置受伤的背景
void Web2DLayer::setVisibleBG()
{
	injureBG->setVisible(false);
}
//改变进度条
void Web2DLayer::changePro()
{
	loadingbar->setVisible(true);
	float percent=Constant::bulletPowerTime/1000.0*100;
	loadingbar->setPercent(percent);
}
//初始化暂停界面
void Web2DLayer::initPauseCode()
{
	menu->setEnabled(false);
	GameData::isPause=true;

	PlaySound::playSound(kMenuSure);
	PlaySound::pauseBackground();					//暂停背景声音
	//暂停场景
	Device::setAccelerometerEnabled(false);			//开启重力传感器

	layer3D->pause();

	//**************暂停游戏背景**************
	pauseBG=Sprite::create(GAMEMODE_PATH+std::string("MenuBg.png"));
	pauseBG->setPosition(Point(270,480));
	this->addChild(pauseBG,MENU_LEVEL);

	Sprite *pause=Sprite::create(PAUSE_PATH+std::string("pauseBG.png"));
	pause->setPosition(Point(270,520));
	pauseBG->addChild(pause,MENU_LEVEL);

	//**************继续游戏**************
	continueGame=MenuItemImage::create(
			PAUSE_PATH+std::string("continueGame_normal.png"),
			PAUSE_PATH+std::string("continueGame_pressed.png"),
			CC_CALLBACK_0(Web2DLayer::continueAnim,this)
			);
	continueGame->setPosition(Point(270,580));

	//**************游戏设置 **************
	gameset=MenuItemImage::create(
			PAUSE_PATH+std::string("setGame_normal.png"),
			PAUSE_PATH+std::string("setGame_pressed.png"),
			CC_CALLBACK_0(Web2DLayer::setGame,this)
			);
	gameset->setPosition(Point(270,480));

	//**************回到主菜单**************
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
//继续游戏动画
void Web2DLayer::continueAnim()
{
	this->removeChild(pauseBG,true);

	continuePauseBG=Sprite::create(GAMEMODE_PATH+std::string("MenuBg.png"));
	continuePauseBG->setPosition(Point(270,480));
	this->addChild(continuePauseBG,MENU_LEVEL+1);

	waitting=Sprite::create(NET_PATH+std::string("waiting.png"));				//等待中...
	continuePauseBG->addChild(waitting);
	waitting->setPosition(270,480);
	waitting->runAction(RepeatForever::create(RotateBy::create(1.0,360)));

	BNSocketUtil::sendInt(GameData::SEND_CONTINUE);								//继续游戏
}
void Web2DLayer::continueCallback()
{
	CCLOG("  ----------------continueCallback");
	this->removeChild(continuePauseBG,true);
	//将暂停标志设为false
	GameData::isPause=false;
	menu->setEnabled(true);
	layer3D->resume();
	Device::setAccelerometerEnabled(true);			//开启重力传感器
	PlaySound::resumeBackground();
}
//跳转到主菜单场景
void Web2DLayer::goMainMenu()
{
    BNSocketUtil::sendInt(GameData::SEND_EXIT);
//    GameData::reset();
//    Constant::isPause=false;
//    Director::getInstance()->resume();
//    gsm->goMainMenu();
}
//暂停比赛
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
//游戏设置
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
	//***************未完成*********************
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
	//等级评价的动画
	ActionInterval *scaleTo=ScaleTo::create(0.5,1.0);				//缩小动画
	ActionInterval *esi=EaseQuadraticActionIn::create(scaleTo);		//由慢到快
	CallFunc *cf=CallFunc::create(CC_CALLBACK_0(CompleteGameLayer::moveBoard,this));
	Sequence *s2=Sequence::create(esi,cf,nullptr);
	grade->runAction(s2);
	CCLOG("--------------------------------1    3");
}

void CompleteGameLayer::moveBoard()
{
	//****************移动*********************
	ActionInterval *mt=MoveTo::create(1,Point(270,550));
	CallFunc *cf=CallFunc::create(CC_CALLBACK_0(CompleteGameLayer::setMenu,this));
	board->runAction(Sequence::create(mt,cf,nullptr));
	CCLOG("--------------------------------1    4");
}

void CompleteGameLayer::setMenu()
{
	this->schedule(SEL_SCHEDULE(&CompleteGameLayer::updateWeb),0.1);
	CCLOG("--------------------------------1    5");
	//****************菜单*********************
	MenuItem *go;
	if(flag==0)
	{
		//重新游戏
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
		//继续游戏
		go=MenuItemImage::create
		(
			MENU_PATH+std::string("continue.png"),
			MENU_PATH+std::string("continue_down.png"),
			MENU_PATH+std::string("continue_disabled.png"),
			CC_CALLBACK_0(CompleteGameLayer::menuCallback,this,1)
		);
	}
	go->setPosition(Point(-100,300));
	//回主菜单
	MenuItem *goMain=MenuItemImage::create
	(
		MENU_PATH+std::string("goMenu.png"),
		MENU_PATH+std::string("goMenu_down.png"),
		MENU_PATH+std::string("goMenu_disabled.png"),
		CC_CALLBACK_0(CompleteGameLayer::menuCallback,this,2)
	);
	goMain->setPosition(Point(640,200));
	//菜单
	Menu *menu=Menu::create(go,goMain,nullptr);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);
	CCLOG("--------------------------------1    6");
	//菜单动画
	go->runAction(EaseElasticOut::create(MoveTo::create(0.75,Point(270,300))));
	ActionInterval *mt=EaseElasticOut::create(MoveTo::create(0.75,Point(270,200)));
	goMain->runAction(Sequence::create(DelayTime::create(0.25),mt,nullptr));
	if(GameData::playerNumber==0){
		go->setEnabled(false);
		goMain->setEnabled(false);
	}
}
//设置游戏数据
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
	//**************界面背景******************
	Sprite* loseBG=Sprite::create(MENU_PATH+std::string("MenuBg.png"));
	loseBG->setPosition(Point(270,480));
	this->addChild(loseBG);
	//**************游戏结算框********************
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

		//计算金币跳动大小**********
		coinStep=(GameData::totalScore-50)/100+1;
		//金币数值****************
		goldLabel=LabelAtlas::create("0",GAME_PATH+std::string("numbers.png"),21,40,'0');
		goldLabel->setScale(1.2f);
		goldLabel->setPosition(Point(200,58));
		board->addChild(goldLabel);
	}

	//***************定时回调********************
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
	CC_SAFE_DELETE(completeGameLayer);								//如果结束场景未成功创建，则安全删除
	return nullptr;
}
