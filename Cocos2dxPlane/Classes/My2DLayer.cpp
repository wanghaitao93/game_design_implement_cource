#include "My2DLayer.h"
#include "AppMacros.h"
#include "Constant.h"
#include "PlaySound.h"
#include <string>

#define coinX 40.0
//USING_NS_CC;
using namespace cocos2d;
using namespace std;

//实现My2DLayer类中的init方法，初始化布景
bool My2DLayer::init()
{
	//调用父类的初始化
    if ( !Layer::init() )
    {
        return false;
    }
    CCLOG("---------------------------------------00");
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

	//*****************开启金币旋转定时回调******************
	auto director = Director::getInstance();
	auto sched = director->getScheduler();
	sched->scheduleSelector(SEL_SCHEDULE(&My2DLayer::update),this,0.01,false);

	//*****************暂停****************
	MenuItemImage* pause=MenuItemImage::create(
			GAME_PATH+std::string("pause.png"),
			MENU_PATH+std::string("pause_down.png"),
			CC_CALLBACK_0(My2DLayer::goPause,this)

	);
	pause->setScale(1.2f);
	pause->setAnchorPoint(Point(1,1));
	pause->setPosition(Vec2(540,955));

	CCLOG("---------------------------------------11");
	MenuItemImage* attack=MenuItemImage::create(
			GAME_PATH+std::string("attack.png"),
			GAME_PATH+std::string("attack_down.png"),
			CC_CALLBACK_0(My2DLayer::useSkill,this,1)

	);
	attack->setScale(1.2f);
	attack->setAnchorPoint(Point(0,0));
	attack->setPosition(Vec2(0,0));

	//防御方法
	MenuItemImage* protect=MenuItemImage::create(
			GAME_PATH+std::string("protect.png"),
			GAME_PATH+std::string("protect_down.png"),
			CC_CALLBACK_0(My2DLayer::useSkill,this,2)

	);
	protect->setScale(1.2f);
	protect->setAnchorPoint(Point(1,0));
	protect->setPosition(Vec2(540,0));

	menu=Menu::create(pause,attack,protect,nullptr);
//	menu->setScale(1.2f);
	menu->setPosition(Point::ZERO);
	this->addChild(menu,MENU_LEVEL);

	//创建子弹威力的进度条
	progress=Sprite::create(GAMEMODE_PATH+std::string("sliderTrack.png"));
	progress->setAnchorPoint(Point(0,1));
	progress->setPosition(Point(150,50));
	progress->setVisible(false);
	this->addChild(progress,MENU_LEVEL);

	loadingbar=LoadingBar::create();
	loadingbar->setAnchorPoint(Point(0.5,1));
	loadingbar->loadTexture(GAMEMODE_PATH+std::string("sliderProgress.png"));
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

	Sprite *gameCustom=Sprite::create(GAME_PATH+StringUtils::format("game%d.png",Constant::custom).c_str());
	gameCustom->setPosition(270,1100);
	this->addChild(gameCustom,MENU_LEVEL);

	ActionInterval *open=Sequence::create(EaseBounceOut::create(MoveBy::create(0.5,Vec2(0,-500))),
					DelayTime::create(1),
					MoveBy::create(0.5,Vec2(0,500)),
					nullptr);
	gameCustom->runAction(Sequence::create(open,RemoveSelf::create(),nullptr));

	if(Constant::custom==1){
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
void My2DLayer::useSkill(int number)
{
	switch(number)
	{
	case 1:
		if(Constant::attackCount>0 && !layer3D->attackAnimFlag)
		{
			Constant::attackCount--;
			layer3D->attackFlag=true;
		}
		break;
	case 2:
		if(Constant::protectCount>0 && !layer3D->protectAnimFlag)
		{
			Constant::protectCount--;
			layer3D->protectFlag=true;
		}
		break;
	}
}
//创建云彩
void My2DLayer::createCloud(){
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
void My2DLayer::goPause()
{
	if(Constant::isPause)
	{
		return;
	}
	menu->setEnabled(false);
	Constant::isPause=true;
	PlaySound::playSound(kMenuSure);
	PlaySound::pauseBackground();		//暂停背景声音
	initPauseCode();
}
//更新云彩，得分，金币的旋转
void My2DLayer::update()
{
	angle++;
	if(angle>=360)
	{
		angle=0;
//		this->unscheduleAllSelectors();
	}
	if(Constant::custom==1){			//第二关
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
	int aa=Constant::score;
	stringstream ss;
	ss<<aa;
	string s1 = ss.str();
	goldLabel->setString(s1);

	//大招个数
	int bb=Constant::attackCount;
	stringstream sss;
	sss<<bb;
	string s11 = sss.str();
	attackLabel->setString(s11);

	//防御个数
	int cc=Constant::protectCount;
	stringstream ss2;
	ss2<<cc;
	string ss1 = ss2.str();
	protectLabel->setString(ss1);

	int planeHealth=Constant::planeHealth;				//更新飞机的生命值
	int percent=planeHealth/2;							//获取生命值
	loadingbar_plane->setPercent(percent);						//设置生命值的百分比

	//子弹加成时间的更改
	if(Constant::isBulletPower){
		Constant::bulletPowerTime--;
		loadingbar->setVisible(true);
		progress->setVisible(true);
		changePro();							//进度条的更改
		if(Constant::bulletPowerTime==0){
			Constant::isBulletPower=false;		//没有子弹威力
			Constant::bulletPower=0;			//子弹威力为0
			loadingbar->setVisible(false);
			progress->setVisible(false);
	}}
}
// 闪动受伤
void My2DLayer::sharkInjuredBG()
{
	injureBG->setVisible(true);
	injureBG->runAction(Sequence::create(
				FadeOut::create(0.3),FadeIn::create(0.3),
//				FadeOut::create(0.3),FadeIn::create(0.3),
				CallFunc::create(CC_CALLBACK_0(My2DLayer::setVisibleBG,this)),
				nullptr
				));
}
void My2DLayer::setVisibleBG()
{
	injureBG->setVisible(false);
}
//改变进度条
void My2DLayer::changePro()
{
	loadingbar->setVisible(true);
	float percent=Constant::bulletPowerTime/1000.0*100;
	loadingbar->setPercent(percent);
}
//初始化暂停界面
void My2DLayer::initPauseCode()
{
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
			PAUSE_PATH+"continueGame_normal.png",
			PAUSE_PATH+"continueGame_pressed.png",
			CC_CALLBACK_0(My2DLayer::continueAnim,this)
			);
	continueGame->setPosition(Point(270,580));
	//**************游戏设置 **************
	gameset=MenuItemImage::create(
			PAUSE_PATH+std::string("setGame_normal.png"),
			PAUSE_PATH+std::string("setGame_pressed.png"),
			CC_CALLBACK_0(My2DLayer::setGame,this)
			);
	gameset->setPosition(Point(270,480));

	//**************回到主菜单**************
	mainMenu=MenuItemImage::create(
			PAUSE_PATH+std::string("mainMenu_normal.png"),
			PAUSE_PATH+std::string("mainMenu_pressed.png"),
			CC_CALLBACK_0(My2DLayer::goMainMenu,this)
			);
	mainMenu->setPosition(Point(270,380));
	Menu *menu=Menu::create(continueGame,gameset,mainMenu,nullptr);
	menu->setPosition(Point::ZERO);
	pauseBG->addChild(menu,MENU_LEVEL);

}
//继续游戏动画
void My2DLayer::continueAnim()
{
	//倒计时动画
	Vector<SpriteFrame*> animFrames;
	Animation *anim;
	Animate *ani;
	SpriteFrame *frame;
	for(int i=0;i<3;i++)
	{
		frame=SpriteFrame::create(GAME_PATH+
			std::string("resumeNumber.png"),Rect(i*200,0,200,200));
		animFrames.pushBack(frame);
	}
	anim=Animation::createWithSpriteFrames(animFrames,1);
	ani=Animate::create(anim);
	Sprite *countDown = Sprite::createWithSpriteFrame(frame);
	countDown->setPosition(270,540);
	this->addChild(countDown);
	countDown->runAction(Sequence::create(ani,RemoveSelf::create(),nullptr));

	this->removeChild(pauseBG,true);
	//旋转背景
	Sprite *rotatePlane=Sprite::create(SELECT_PATH+std::string("planeRotate.png"));
	rotatePlane->setPosition(270,540);
//	BlendFunc bf = {GL_DST_COLOR, GL_ONE};
//	rotatePlane->setBlendFunc(bf);
	this->addChild(rotatePlane);
	rotatePlane->runAction(Sequence::create(Repeat::create(RotateBy::create(0.5,180),6),
		CallFunc::create(CC_CALLBACK_0(My2DLayer::continueCallback,this)),
		RemoveSelf::create(),nullptr));

}
void My2DLayer::continueCallback()
{
	//将暂停标志设为false
	Constant::isPause=false;
	menu->setEnabled(true);
	layer3D->resume();
	Device::setAccelerometerEnabled(true);			//开启重力传感器
	PlaySound::resumeBackground();
}
//跳转到主菜单场景
void My2DLayer::goMainMenu()
{
	PlaySound::playSound(kMenuSelect);
	layer3D->deletePlane();
	Constant::isPause=false;
	layer3D->resetGame();
	gsm->goMainMenu();
}
//暂停比赛
void My2DLayer::pauseGame()
{
	this->pause();
	layer3D->pause();
	layer3D->clearAllBullets();
}
void My2DLayer::completeGame(int flag)
{
	this->pause();
	layer3D->pause();
	Constant::gameingFlag=false;
	CompleteLayer *cl=CompleteLayer::create(flag);
	this->addChild(cl,1000);
}
//游戏设置
void My2DLayer::setGame()
{
	continueGame->setEnabled(false);
	gameset->setEnabled(false);
	mainMenu->setEnabled(false);
	gameSet=new GameSet(this);
	gameSet->createSet(1);
}

void CompleteLayer::goMainMenu()
{
	My2DLayer *ml=dynamic_cast<My2DLayer*>(this->getParent());
	ml->layer3D->deletePlane();
	ml->layer3D->resetGame();
	Constant::isPause=false;
	ml->gsm->goMainMenu();
	Director::getInstance()->resume();
}

void CompleteLayer::goNext()
{
	My2DLayer *ml=dynamic_cast<My2DLayer*>(this->getParent());
	ml->layer3D->deletePlane();
	ml->layer3D->resetGame();
	Constant::isPause=false;

	ml->gsm->goSelectGame();
	Director::getInstance()->resume();
}

void CompleteLayer::goSelf()
{
	My2DLayer *ml=dynamic_cast<My2DLayer*>(this->getParent());
	ml->layer3D->deletePlane();
	ml->layer3D->resetGame();
	Constant::isPause=false;
	ml->gsm->goFirst();
	Director::getInstance()->resume();
}

void CompleteLayer::updateCoin()
{
	CCLOG("--------------------------------1    0");
	if(flag==winEnum)
	{
		coinCounter+=coinStep;
		if(coinCounter<=Constant::score)
		{
			stringstream ss;
			ss<<(coinCounter/10);
			string s = ss.str();
			goldLabel->setString(s);
			return;
		}
		stringstream ss;
		ss<<(Constant::score/10);
		string s = ss.str();
		goldLabel->setString(s);
		Constant::money+=Constant::score/10;
		UserDefault::getInstance()->setIntegerForKey("money",Constant::money);
		CCLOG("--------------------------------1    1");
	}
	this->unscheduleAllSelectors();
	//***************未完成*********************
	Sprite *grade;
	if(flag==loseEnum)
	{
		grade=Sprite::create(MENU_PATH+std::string("incomplete.png"));
		grade->setPosition(320,30);
	}else if(flag==winEnum)
	{
		int i=5-Constant::score/1000;
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
	CallFunc *cf=CallFunc::create(CC_CALLBACK_0(CompleteLayer::moveBoard,this));
	Sequence *s2=Sequence::create(esi,cf,nullptr);
	grade->runAction(s2);
	CCLOG("--------------------------------1    3");
}

void CompleteLayer::moveBoard()
{
	//****************移动*********************
	ActionInterval *mt=MoveTo::create(1,Point(270,550));
	CallFunc *cf=CallFunc::create(CC_CALLBACK_0(CompleteLayer::setMenu,this));
	board->runAction(Sequence::create(mt,cf,nullptr));
	CCLOG("--------------------------------1    4");
}

void CompleteLayer::setMenu()
{
	CCLOG("--------------------------------1    5");
	//****************菜单*********************
	MenuItem *go;
	if(flag==loseEnum)
	{
		//重新游戏
		go=MenuItemImage::create
		(
			MENU_PATH+std::string("reset.png"),
			MENU_PATH+std::string("reset_down.png"),
			CC_CALLBACK_0(CompleteLayer::goSelf,this)
		);
	}
	else if(flag==winEnum)
	{
		//继续游戏
		go=MenuItemImage::create
		(
			MENU_PATH+std::string("continue.png"),
			MENU_PATH+std::string("continue_down.png"),
			CC_CALLBACK_0(CompleteLayer::goNext,this)
		);
		//设置游戏数据
		setGameDate();
	}
	go->setPosition(Point(-100,300));
	//回主菜单
	MenuItem *goMain=MenuItemImage::create
	(
		MENU_PATH+std::string("goMenu.png"),
		MENU_PATH+std::string("goMenu_down.png"),
		CC_CALLBACK_0(CompleteLayer::goMainMenu,this)
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
}
//设置游戏数据
void CompleteLayer::setGameDate()
{
	if(Constant::custom==Constant::canPlay&&Constant::canPlay<=1){
			Constant::canPlay++;
			Constant::custom++;
			Constant::openCustom=true;
		}
	UserDefault::getInstance()->setIntegerForKey("custom",Constant::custom);
	UserDefault::getInstance()->setIntegerForKey("canPlay",Constant::canPlay);
}
bool CompleteLayer::init()
{
	CCLOG("---------------------------------------333");
	if(!Layer::init())
	{
		return false;
	}
	//**************界面背景******************
	Sprite* loseBG=Sprite::create(GAMEMODE_PATH+std::string("MenuBg.png"));
	loseBG->setPosition(Point(270,480));
	this->addChild(loseBG);
	//**************游戏结算框********************
	if(flag==loseEnum)
	{
		board = Sprite::create(MENU_PATH+std::string("gameLose.png"));
		board->setPosition(Point(270,450));
		this->addChild(board,1);
	}else if(flag==winEnum)
	{
		board = Sprite::create(MENU_PATH+std::string("gameSuccess.png"));
		board->setPosition(Point(270,450));
		this->addChild(board,1);

		//计算金币跳动大小**********
		coinStep=(Constant::score-50)/100+1;
		//金币数值****************
		goldLabel=LabelAtlas::create("0",GAME_PATH+std::string("numbers.png"),21,40,'0');
		goldLabel->setScale(1.2f);
		goldLabel->setPosition(Point(200,58));
		board->addChild(goldLabel);
	}

	//***************定时回调********************
	this->schedule(SEL_SCHEDULE(&CompleteLayer::updateCoin),0.01);
	return true;
}

CompleteLayer* CompleteLayer::create(int _flag)
{
	CompleteLayer *completeLayer = new CompleteLayer();
	if(completeLayer)
	{
		completeLayer->flag=_flag;
		if(completeLayer->init())
		{
			completeLayer->autorelease();
			return completeLayer;
		}
	}
	CC_SAFE_DELETE(completeLayer);								//如果结束场景未成功创建，则安全删除
	return nullptr;
}
