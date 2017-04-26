#include "SelectGameLayer.h"
#include "GameSceneManager.h"
#include "AppMacros.h"
#include "Constant.h"
#include "PlaySound.h"
#include "GameData.h"
#include "BNSocketUtil.h"
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

vector<Sprite3D*> SelectGameLayer::bossVec;

bool SelectGameLayer::init()
{
	//背景
	background=Sprite::create(SELECTGAME_PATH+std::string("selectGameBG.jpg"));
	background->setPosition(270,480);
	this->addChild(background,BACKGROUND_LEVEL);
	//星星的纹理
	starsTexture=Director::getInstance()->getTextureCache()->addImage(SELECTGAME_PATH+std::string("tstars.png"));
	//飞机、关卡选择框
	planeSelect=Sprite::create(SELECTGAME_PATH+std::string("planeSelect.png"));
	planeSelect->setPosition(223,822);
	this->addChild(planeSelect,BACKGROUND_LEVEL+1);
	gameSelect=Sprite::create(SELECTGAME_PATH+std::string("gameSelect.png"));
	gameSelect->setPosition(350,878);
	this->addChild(gameSelect,BACKGROUND_LEVEL+1);
	//选择框移动动画
	ActionInterval *mbr = MoveBy::create(0.5,Point(127,0));
	ActionInterval *mbl = MoveBy::create(0.5,Point(-127,0));
	ActionInterval *dt  = DelayTime::create(0.2);
	planeSelect->runAction(RepeatForever::create(Sequence::create(
		dt->clone(),mbr->clone(),dt->clone(),mbr->clone(),dt->clone(),
		mbl->clone(),dt->clone(),mbl->clone(),nullptr)));
	gameSelect->runAction(RepeatForever::create(Sequence::create(
		mbr->clone(),dt->clone(),mbl->clone(),dt->clone(),mbl->clone(),
		dt->clone(),mbr->clone(),dt->clone(),nullptr)));

	//返回
	MenuItemImage *menuBack=MenuItemImage::create(
		HELP_PATH+"back_btn.png",
		HELP_PATH+"back_btn_down.png",
		CC_CALLBACK_0(SelectGameLayer::goMainMenu,this)
	);
	menuBack->setPosition(Point(80,70));
	if(GameData::playerNumber!=-1){					//如果是网络版
		menuBack->setVisible(false);				//隐藏返回按钮
		}
	//开始战斗
	startGame=MenuItemImage::create(
		SELECTGAME_PATH+std::string("fight_btn.png"),
		SELECTGAME_PATH+std::string("fight_btn_down.png"),
		CC_CALLBACK_0(SelectGameLayer::goSelectPlaneAnim,this)
	);
	startGame->setPosition(Point(460,70));
	menu=Menu::create(menuBack,startGame,nullptr);
	menu->setPosition(Point::ZERO);
	this->addChild(menu,MENU_LEVEL);

	ClippingRectangleNode* cliper = ClippingRectangleNode::create(Rect(0,200,260,748));
	cliper->setPosition(Point(0,0));
	this->addChild(cliper);

	//按钮的发光效果
	Sprite *blend[2];
	for(int i=0;i<2;i++){
		blend[i]=Sprite::create(SELECTGAME_PATH+std::string("blend.png"));
			BlendFunc cbl = {GL_DST_COLOR, GL_ONE};
			blend[i]->setBlendFunc(cbl);
			blend[i]->setAnchorPoint(Point(0,0.5));
	}
	blend[0]->setPosition(65,30);
	blend[0]->runAction(
			RepeatForever::create(
					Sequence::create(
							MoveBy::create(1.0f,Vec2(-70,0)),
							MoveBy::create(1.0f,Vec2(70,0)),
							nullptr
							)));
	blend[1]->setPosition(-5,30);
	blend[1]->runAction(
			RepeatForever::create(
					Sequence::create(
							MoveBy::create(1.0f,Vec2(70,0)),
//							Place::create(Vec2(-5,30)),
							MoveBy::create(1.0f,Vec2(-70,0)),
							nullptr
							)));
	startGame->addChild(blend[0]);
	menuBack->addChild(blend[1]);
	//初始化关卡介绍
	for(int i=0;i<3;i++){
		introduce[i] = Sprite::create(SELECTGAME_PATH+
			StringUtils::format("level%d.png",i+1));
		introduce[i]->setPosition(Point(135+270*(i-Constant::custom),485));
		cliper->addChild(introduce[i]);
	}

	//关卡介绍的颜色混合效果
	Sprite *blendBig[3];
	for(int i=0;i<3;i++){
		blendBig[i]=Sprite::create(SELECTGAME_PATH+std::string("blendBig.png"));
		BlendFunc cbl = {GL_DST_COLOR, GL_ONE};
		blendBig[i]->setBlendFunc(cbl);
		blendBig[i]->setAnchorPoint(Point(0,1));
		blendBig[i]->setPosition(0,585);
		blendBig[i]->runAction(
					RepeatForever::create(
							Sequence::create(
									MoveBy::create(3.0f,Vec2(0,-485)),
									Place::create(Vec2(0,585)),
									nullptr
									)));
	introduce[i]->addChild(blendBig[i]);
	}
	//初始化boss信息 星星
	for(int i=0;i<3;i++){
		stars[i]=Sprite::createWithTexture(starsTexture,Rect(0,0,98.0*Constant::bossSkill[Constant::custom][i]/100,15));
		stars[i]->setPosition(436-49,391-66*i);
		stars[i]->setAnchorPoint(Point(0,0.5));
		this->addChild(stars[i],MENU_LEVEL);
	}

	//初始化关卡boss
	for(int i=0;i<3;i++){
		if(i==0)
		{
			boss[i]=Sprite3D::create(C3B_PATH+std::string("boss1.c3b"));
			boss[i]->setRotation3D(Vec3(30,-10,-50));
			boss[i]->setScale(0.7);
		}else if(i==1)
		{
			boss[i]=Sprite3D::create(C3B_PATH+std::string("boss2.c3b"));
			boss[i]->setRotation3D(Vec3(0,170,0));
			boss[i]->setScale(0.7);
		}else{
			boss[i]=Sprite3D::create(C3B_PATH+std::string("boss3.c3b"));
			boss[i]->setRotation3D(Vec3(30,0,0));
			boss[i]->setScale(0.8);
		}
		if(Constant::custom==i){
			boss[i]->setPosition3D(Vec3(370,580-((i==1)?90:0),80));
		}else{
			boss[i]->setPosition3D(Vec3(800,550-((i==1)?60:0),80));
		}
		this->addChild(boss[i],NORMAL_LEVEL);
	}
	//初始化boss锁
	lock = Sprite::create(SELECTGAME_PATH+"suo.png");
	lock->setPosition(Point(360,550));
	lock->setPositionZ(200);
	lock->setVisible(false);
	this->addChild(lock, MENU_LEVEL);

	if(Constant::custom<=Constant::canPlay)
	{
		if(Constant::custom==1)
		{
			//创建并播放骨骼动画
			Animation3D *animationHero = Animation3D::create(C3B_PATH+std::string("boss2.c3b"));
			CCLOG("==================heroDuration==%f",animationHero->getDuration());//打印动画周期
			if (animationHero)
			{
				//英雄各种动画
				Animate3D *animateK = Animate3D::create(animationHero,0.0f,5.0f);
				//骨骼动画速度
//				animateK->setSpeed(animateK->getSpeed()*0.9f);
				//执行动作
				boss[Constant::custom]->runAction(RepeatForever::create(animateK));
			}
		}else if(Constant::custom==0)
		{
			//创建并播放骨骼动画
			Animation3D *animationHero = Animation3D::create(C3B_PATH+std::string("boss1.c3b"));
			CCLOG("==================heroDuration==%f",animationHero->getDuration());//打印动画周期
			if (animationHero)
			{
				//英雄各种动画
				Animate3D *animateK = Animate3D::create(animationHero,0.0f,1.0f);
				//骨骼动画速度
				animateK->setSpeed(animateK->getSpeed()*0.6f);
				//执行动作
				boss[Constant::custom]->runAction(RepeatForever::create(animateK));
			}
		}else
		{
			//创建并播放骨骼动画
			Animation3D *animationHero = Animation3D::create(C3B_PATH+std::string("boss3.c3b"));
			CCLOG("==================heroDuration==%f",animationHero->getDuration());//打印动画周期
			if (animationHero)
			{
				//英雄各种动画
				Animate3D *animateK = Animate3D::create(animationHero,5.0f,3.0f);
				//骨骼动画速度
				animateK->setSpeed(animateK->getSpeed()*0.1f);
				//执行动作
				boss[Constant::custom]->runAction(
						RepeatForever::create(
							Sequence::create(
									animateK,
									nullptr)
								));
			}
		}
	}
	//滑动切换关卡的背景
	Sprite* changeGameBG=Sprite::create(SELECTGAME_PATH+std::string("changeGameBG.png"));
	changeGameBG->setPosition(Point(270,170));
	this->addChild(changeGameBG);
	ActionInterval *changeOpa=RepeatForever::create(Sequence::create(FadeOut::create(1),FadeIn::create(1),nullptr));
	changeGameBG->runAction(changeOpa);
	//滑动切换关卡
	Sprite* changeGame=Sprite::create(SELECTGAME_PATH+std::string("changeGame.png"));
	changeGame->setPosition(Point(270,170));
	this->addChild(changeGame);

	//环境光  一般都是要的
	AmbientLight *ambientLight=AmbientLight::create(Color3B(150, 150, 150));
	this->addChild(ambientLight);
	//定向光
	DirectionLight *directionLight= DirectionLight::create(Vec3(0.0f, -1.0f, -1.0f), Color3B(200, 200, 200));
	this->addChild(directionLight);

	//创建一个单点触摸监听
	EventListenerTouchOneByOne* listenerTouches = EventListenerTouchOneByOne::create();
	//设置下传触摸
	listenerTouches->setSwallowTouches(true);
	//开始触摸时回调onTouchBegan方法
	listenerTouches->onTouchBegan = CC_CALLBACK_2(SelectGameLayer::onMyTouchBegan, this);
	//开始触摸时回调onTouchMoved方法
	listenerTouches->onTouchMoved = CC_CALLBACK_2(SelectGameLayer::onMyTouchMoved, this);
	//触摸结束时回调onTouchEnded方法
	listenerTouches->onTouchEnded = CC_CALLBACK_2(SelectGameLayer::onMyTouchEnded, this);
	//添加到监听器//
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerTouches, this);

	if(GameData::playerNumber==0)		//客人
	{
		Sprite *background=Sprite::create(GAMEMODE_PATH+std::string("MenuBg.png"));
		background->setScale(2);
		background->setPosition3D(Vec3(270,480,201));
		this->addChild(background,MENU_LEVEL);

		waittingConnect=Sprite::create(SELECTGAME_PATH+std::string("waittingConnect0.png"));
		this->addChild(waittingConnect,MENU_LEVEL);
		waittingConnect->setPosition3D(Vec3(270,480,201));

		schedule(schedule_selector(SelectGameLayer::changWaitting),0.1f);

		lockInterface();				//锁住界面
	}

	if(GameData::canPlay==1){
		changeLevel(1);
		Constant::openCustom=true;
	}else if(GameData::canPlay==2){
		//初始化boss锁
		Constant::openCustom=true;
		changeLevel(2);
	}
	if(Constant::openCustom)				//如果刚解锁
		{
			Sprite *animlock = Sprite::create(SELECTGAME_PATH+"suo.png");
			animlock->setPosition(boss[Constant::custom]->getPosition()-Point(-10,0));
			animlock->setPositionZ(200);
			this->addChild(animlock, MENU_LEVEL);

			ActionInterval *moveUp=MoveBy::create(0.1,Point(10,10));
			ActionInterval *move=Sequence::create(moveUp,moveUp->reverse(),nullptr);
			animlock->runAction(Sequence::create(
					Repeat::create(move,5),
					RemoveSelf::create(),
					nullptr
					));
			Constant::openCustom=false;
	}
	return true;
}

//锁住界面
void SelectGameLayer::lockInterface()
{
	menu->setEnabled(false);
	isCanSelect=false;
}
void SelectGameLayer::changWaitting(float f)
{
	waitIndex++;
	if(waitIndex==4){
		waitIndex=0;
	}
	waittingConnect->setTexture(NET_PATH+StringUtils::format("waittingConnect%d.png",waitIndex));

	if(GameData::oldLevel!=GameData::level){
		if((GameData::oldLevel+1)%3==GameData::level){
			changeLevel(1);
			GameData::oldLevel=GameData::level;
		}else if((GameData::oldLevel+2)%3==GameData::level){
			changeLevel(2);
			GameData::oldLevel=GameData::level;
		}
	}
	if(GameData::SelectedGame){				//如果已经选择完关卡
		goSelectPlane();
	}
}
//移动BOSS
void SelectGameLayer::updateMoveBoss(float f)
{
	if(GameData::playerNumber==-1){
		//判断该BOSS是否已解锁
		if(Constant::custom<=Constant::canPlay){
			float bossY=boss[Constant::custom]->getPositionY();
			if(bossY<320){					//到最低点
				raise=false;
			}else if(bossY>350){			//到最高点
				raise=true;
			}
			if(raise){
				bossY--;
			}else{
				bossY++;
			}
			boss[Constant::custom]->setPositionY(bossY);
		}
	}else
	{
		//判断该BOSS是否已解锁
		if(Constant::custom<=GameData::canPlay){
			float bossY=boss[Constant::custom]->getPositionY();
			if(bossY<320){					//到最低点
				raise=false;
			}else if(bossY>350){			//到最高点
				raise=true;
			}
			if(raise){
				bossY--;
			}else{
				bossY++;
			}
			boss[Constant::custom]->setPositionY(bossY);
		}
	}

}
//改变选关信息
void SelectGameLayer::changeLevel(int index)
{
	PlaySound::playSound(kMenuSure);
	Constant::custom=(Constant::custom+index)%3;
	int custom=Constant::custom;
	if(GameData::playerNumber==1){							//房主改变关卡
		BNSocketUtil::sendInt(GameData::SEND_LEVEL);		//发送关卡等级
		BNSocketUtil::sendInt(Constant::custom);			//发送关卡等级
	}
	if(index==1)
	{
		introduce[(custom+2)%3]->runAction(MoveTo::create(0.3,Point(-135,485)));
		introduce[custom]->setPosition(Point(405,485));
		introduce[custom]->runAction(MoveTo::create(0.3,Point(135,485)));
		introduce[(custom+1)%3]->setPosition(Point(-270,485));
	}else if(index==2)
	{
		introduce[(custom+1)%3]->runAction(MoveTo::create(0.3,Point(405,485)));
		introduce[custom]->setPosition(Point(-135,485));
		introduce[custom]->runAction(MoveTo::create(0.3,Point(135,485)));
		introduce[(custom+2)%3]->setPosition(Point(405,485));
	}

	if(GameData::playerNumber==-1){						//单ji
		if(Constant::custom>Constant::canPlay){
			startGame->setEnabled(false);
		}else{
			startGame->setEnabled(true);
		}
	}else{												//联网
		if(Constant::custom>GameData::canPlay){
			startGame->setEnabled(false);
		}else{
			startGame->setEnabled(true);
		}
	}

	lock->setVisible(false);
	for(int i=0;i<3;i++)
	{
		boss[i]->setPosition(-200,-200);
		boss[i]->stopAllActions();
	}
	if(Constant::custom==1)
	{
		boss[Constant::custom]->setPosition3D(Vec3(390,490,80));
	}else{
		boss[Constant::custom]->setPosition3D(Vec3(365,580,80));
	}
	bool isCanPlay;
	if(GameData::playerNumber==-1){
		if(Constant::custom<=Constant::canPlay){
			isCanPlay=true;
		}else{
			isCanPlay=false;
		}
	}else{
		if(Constant::custom<=GameData::canPlay){
			isCanPlay=true;
		}else{
			isCanPlay=false;
		}
	}
	if(isCanPlay)
	{
		if(Constant::custom==1)
		{
			//创建并播放骨骼动画
			Animation3D *animationHero = Animation3D::create(C3B_PATH+std::string("boss2.c3b"));
			CCLOG("==================heroDuration==%f",animationHero->getDuration());//打印动画周期
			if (animationHero)
			{
				//英雄各种动画
				Animate3D *animateK = Animate3D::create(animationHero,0.0f,5.0f);
				//骨骼动画速度
//				animateK->setSpeed(animateK->getSpeed()*0.9f);
				//执行动作
				boss[Constant::custom]->runAction(RepeatForever::create(animateK));
			}
		}else if(Constant::custom==0)
		{
			//创建并播放骨骼动画
			Animation3D *animationHero = Animation3D::create(C3B_PATH+std::string("boss1.c3b"));
			CCLOG("==================heroDuration==%f",animationHero->getDuration());//打印动画周期
			if (animationHero)
			{
				//英雄各种动画
				Animate3D *animateK = Animate3D::create(animationHero,0.0f,1.0f);
				//骨骼动画速度
				animateK->setSpeed(animateK->getSpeed()*0.6f);
				//执行动作
				boss[Constant::custom]->runAction(RepeatForever::create(animateK));
			}
		}else
		{
			//创建并播放骨骼动画
			Animation3D *animationHero = Animation3D::create(C3B_PATH+std::string("boss3.c3b"));
			CCLOG("==================heroDuration==%f",animationHero->getDuration());//打印动画周期
			if (animationHero)
			{
				//创建并播放骨骼动画
				Animation3D *animationHero = Animation3D::create(C3B_PATH+std::string("boss3.c3b"));
				CCLOG("==================heroDuration==%f",animationHero->getDuration());//打印动画周期
				if (animationHero)
				{
					//英雄各种动画
					Animate3D *animateK1 = Animate3D::createWithFrames(animationHero,0,5);
					Animate3D *animateK2 = Animate3D::createWithFrames(animationHero,15,25);
					Animate3D *animateK3 = Animate3D::createWithFrames(animationHero,55,60);
					Animate3D *animateK4 = Animate3D::create(animationHero,5.0f,3.0f);
					//骨骼动画速度
//					animateK1->setSpeed(animateK1->getSpeed()*0.2f);
//					animateK2->setSpeed(animateK2->getSpeed()*0.2f);
//					animateK3->setSpeed(animateK3->getSpeed()*0.2f);
					animateK4->setSpeed(animateK4->getSpeed()*0.001f);
					//执行动作
					boss[Constant::custom]->runAction(
							RepeatForever::create(
								Sequence::create(
//										animateK1,
//										animateK2,
//										animateK3,
										animateK4,
										nullptr)
									));
				}
			}
		}
	}
	else
	{
		lock->setVisible(true);
	}
	//改变boss信息 星星
	for(int i=0;i<3;i++){
		stars[i]->setTextureRect(Rect(0,0,98.0*Constant::bossSkill[Constant::custom][i]/100,15));
	}
}

//返回主菜单
void SelectGameLayer::goMainMenu()
{
	PlaySound::playSound(kBack);
	gsm->goMainMenu();
}
void SelectGameLayer::goSelectPlane()
{
	if(GameData::playerNumber==1){
		BNSocketUtil::sendInt(GameData::SEND_SELECT);
	}
	gsm->goSelectPlane();
}
void SelectGameLayer::goSelectPlaneAnim()
{
	menu->setEnabled(false);
	PlaySound::playSound(kMenuButton);
	gameSelect->stopAllActions();
	Point pTemp=Point(223+127*Constant::custom,878);
	gameSelect->runAction(Sequence::create(
		MoveTo::create(0.3,pTemp),Blink::create(0.7,3),
		CallFunc::create(CC_CALLBACK_0(SelectGameLayer::goSelectPlane,this)),nullptr));
}
bool SelectGameLayer::onMyTouchBegan(Touch* touch, Event* event)
{
	if(isCanSelect&&Rect(0,140,540,200).containsPoint(touch->getLocation()))
	{
		touchFlag=true;
	}
	return true;
}
void SelectGameLayer::onMyTouchMoved(Touch *touch, Event *event)
{
	if(touchFlag&&Rect(0,140,540,200).containsPoint(touch->getLocation()))
	{
		Vec2 vTemp=touch->getDelta();
		if(vTemp.x<-10)
		{
			this->changeLevel(1);
			touchFlag=false;
		}else if(vTemp.x>10)
		{
			this->changeLevel(2);
			touchFlag=false;
		}
	}
}
void SelectGameLayer::onMyTouchEnded(Touch *touch, Event *event)
{
	touchFlag=false;
}
