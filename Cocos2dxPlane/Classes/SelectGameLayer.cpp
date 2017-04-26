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
	//����
	background=Sprite::create(SELECTGAME_PATH+std::string("selectGameBG.jpg"));
	background->setPosition(270,480);
	this->addChild(background,BACKGROUND_LEVEL);
	//���ǵ�����
	starsTexture=Director::getInstance()->getTextureCache()->addImage(SELECTGAME_PATH+std::string("tstars.png"));
	//�ɻ����ؿ�ѡ���
	planeSelect=Sprite::create(SELECTGAME_PATH+std::string("planeSelect.png"));
	planeSelect->setPosition(223,822);
	this->addChild(planeSelect,BACKGROUND_LEVEL+1);
	gameSelect=Sprite::create(SELECTGAME_PATH+std::string("gameSelect.png"));
	gameSelect->setPosition(350,878);
	this->addChild(gameSelect,BACKGROUND_LEVEL+1);
	//ѡ����ƶ�����
	ActionInterval *mbr = MoveBy::create(0.5,Point(127,0));
	ActionInterval *mbl = MoveBy::create(0.5,Point(-127,0));
	ActionInterval *dt  = DelayTime::create(0.2);
	planeSelect->runAction(RepeatForever::create(Sequence::create(
		dt->clone(),mbr->clone(),dt->clone(),mbr->clone(),dt->clone(),
		mbl->clone(),dt->clone(),mbl->clone(),nullptr)));
	gameSelect->runAction(RepeatForever::create(Sequence::create(
		mbr->clone(),dt->clone(),mbl->clone(),dt->clone(),mbl->clone(),
		dt->clone(),mbr->clone(),dt->clone(),nullptr)));

	//����
	MenuItemImage *menuBack=MenuItemImage::create(
		HELP_PATH+"back_btn.png",
		HELP_PATH+"back_btn_down.png",
		CC_CALLBACK_0(SelectGameLayer::goMainMenu,this)
	);
	menuBack->setPosition(Point(80,70));
	if(GameData::playerNumber!=-1){					//����������
		menuBack->setVisible(false);				//���ط��ذ�ť
		}
	//��ʼս��
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

	//��ť�ķ���Ч��
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
	//��ʼ���ؿ�����
	for(int i=0;i<3;i++){
		introduce[i] = Sprite::create(SELECTGAME_PATH+
			StringUtils::format("level%d.png",i+1));
		introduce[i]->setPosition(Point(135+270*(i-Constant::custom),485));
		cliper->addChild(introduce[i]);
	}

	//�ؿ����ܵ���ɫ���Ч��
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
	//��ʼ��boss��Ϣ ����
	for(int i=0;i<3;i++){
		stars[i]=Sprite::createWithTexture(starsTexture,Rect(0,0,98.0*Constant::bossSkill[Constant::custom][i]/100,15));
		stars[i]->setPosition(436-49,391-66*i);
		stars[i]->setAnchorPoint(Point(0,0.5));
		this->addChild(stars[i],MENU_LEVEL);
	}

	//��ʼ���ؿ�boss
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
	//��ʼ��boss��
	lock = Sprite::create(SELECTGAME_PATH+"suo.png");
	lock->setPosition(Point(360,550));
	lock->setPositionZ(200);
	lock->setVisible(false);
	this->addChild(lock, MENU_LEVEL);

	if(Constant::custom<=Constant::canPlay)
	{
		if(Constant::custom==1)
		{
			//���������Ź�������
			Animation3D *animationHero = Animation3D::create(C3B_PATH+std::string("boss2.c3b"));
			CCLOG("==================heroDuration==%f",animationHero->getDuration());//��ӡ��������
			if (animationHero)
			{
				//Ӣ�۸��ֶ���
				Animate3D *animateK = Animate3D::create(animationHero,0.0f,5.0f);
				//���������ٶ�
//				animateK->setSpeed(animateK->getSpeed()*0.9f);
				//ִ�ж���
				boss[Constant::custom]->runAction(RepeatForever::create(animateK));
			}
		}else if(Constant::custom==0)
		{
			//���������Ź�������
			Animation3D *animationHero = Animation3D::create(C3B_PATH+std::string("boss1.c3b"));
			CCLOG("==================heroDuration==%f",animationHero->getDuration());//��ӡ��������
			if (animationHero)
			{
				//Ӣ�۸��ֶ���
				Animate3D *animateK = Animate3D::create(animationHero,0.0f,1.0f);
				//���������ٶ�
				animateK->setSpeed(animateK->getSpeed()*0.6f);
				//ִ�ж���
				boss[Constant::custom]->runAction(RepeatForever::create(animateK));
			}
		}else
		{
			//���������Ź�������
			Animation3D *animationHero = Animation3D::create(C3B_PATH+std::string("boss3.c3b"));
			CCLOG("==================heroDuration==%f",animationHero->getDuration());//��ӡ��������
			if (animationHero)
			{
				//Ӣ�۸��ֶ���
				Animate3D *animateK = Animate3D::create(animationHero,5.0f,3.0f);
				//���������ٶ�
				animateK->setSpeed(animateK->getSpeed()*0.1f);
				//ִ�ж���
				boss[Constant::custom]->runAction(
						RepeatForever::create(
							Sequence::create(
									animateK,
									nullptr)
								));
			}
		}
	}
	//�����л��ؿ��ı���
	Sprite* changeGameBG=Sprite::create(SELECTGAME_PATH+std::string("changeGameBG.png"));
	changeGameBG->setPosition(Point(270,170));
	this->addChild(changeGameBG);
	ActionInterval *changeOpa=RepeatForever::create(Sequence::create(FadeOut::create(1),FadeIn::create(1),nullptr));
	changeGameBG->runAction(changeOpa);
	//�����л��ؿ�
	Sprite* changeGame=Sprite::create(SELECTGAME_PATH+std::string("changeGame.png"));
	changeGame->setPosition(Point(270,170));
	this->addChild(changeGame);

	//������  һ�㶼��Ҫ��
	AmbientLight *ambientLight=AmbientLight::create(Color3B(150, 150, 150));
	this->addChild(ambientLight);
	//�����
	DirectionLight *directionLight= DirectionLight::create(Vec3(0.0f, -1.0f, -1.0f), Color3B(200, 200, 200));
	this->addChild(directionLight);

	//����һ�����㴥������
	EventListenerTouchOneByOne* listenerTouches = EventListenerTouchOneByOne::create();
	//�����´�����
	listenerTouches->setSwallowTouches(true);
	//��ʼ����ʱ�ص�onTouchBegan����
	listenerTouches->onTouchBegan = CC_CALLBACK_2(SelectGameLayer::onMyTouchBegan, this);
	//��ʼ����ʱ�ص�onTouchMoved����
	listenerTouches->onTouchMoved = CC_CALLBACK_2(SelectGameLayer::onMyTouchMoved, this);
	//��������ʱ�ص�onTouchEnded����
	listenerTouches->onTouchEnded = CC_CALLBACK_2(SelectGameLayer::onMyTouchEnded, this);
	//��ӵ�������//
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerTouches, this);

	if(GameData::playerNumber==0)		//����
	{
		Sprite *background=Sprite::create(GAMEMODE_PATH+std::string("MenuBg.png"));
		background->setScale(2);
		background->setPosition3D(Vec3(270,480,201));
		this->addChild(background,MENU_LEVEL);

		waittingConnect=Sprite::create(SELECTGAME_PATH+std::string("waittingConnect0.png"));
		this->addChild(waittingConnect,MENU_LEVEL);
		waittingConnect->setPosition3D(Vec3(270,480,201));

		schedule(schedule_selector(SelectGameLayer::changWaitting),0.1f);

		lockInterface();				//��ס����
	}

	if(GameData::canPlay==1){
		changeLevel(1);
		Constant::openCustom=true;
	}else if(GameData::canPlay==2){
		//��ʼ��boss��
		Constant::openCustom=true;
		changeLevel(2);
	}
	if(Constant::openCustom)				//����ս���
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

//��ס����
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
	if(GameData::SelectedGame){				//����Ѿ�ѡ����ؿ�
		goSelectPlane();
	}
}
//�ƶ�BOSS
void SelectGameLayer::updateMoveBoss(float f)
{
	if(GameData::playerNumber==-1){
		//�жϸ�BOSS�Ƿ��ѽ���
		if(Constant::custom<=Constant::canPlay){
			float bossY=boss[Constant::custom]->getPositionY();
			if(bossY<320){					//����͵�
				raise=false;
			}else if(bossY>350){			//����ߵ�
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
		//�жϸ�BOSS�Ƿ��ѽ���
		if(Constant::custom<=GameData::canPlay){
			float bossY=boss[Constant::custom]->getPositionY();
			if(bossY<320){					//����͵�
				raise=false;
			}else if(bossY>350){			//����ߵ�
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
//�ı�ѡ����Ϣ
void SelectGameLayer::changeLevel(int index)
{
	PlaySound::playSound(kMenuSure);
	Constant::custom=(Constant::custom+index)%3;
	int custom=Constant::custom;
	if(GameData::playerNumber==1){							//�����ı�ؿ�
		BNSocketUtil::sendInt(GameData::SEND_LEVEL);		//���͹ؿ��ȼ�
		BNSocketUtil::sendInt(Constant::custom);			//���͹ؿ��ȼ�
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

	if(GameData::playerNumber==-1){						//��ji
		if(Constant::custom>Constant::canPlay){
			startGame->setEnabled(false);
		}else{
			startGame->setEnabled(true);
		}
	}else{												//����
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
			//���������Ź�������
			Animation3D *animationHero = Animation3D::create(C3B_PATH+std::string("boss2.c3b"));
			CCLOG("==================heroDuration==%f",animationHero->getDuration());//��ӡ��������
			if (animationHero)
			{
				//Ӣ�۸��ֶ���
				Animate3D *animateK = Animate3D::create(animationHero,0.0f,5.0f);
				//���������ٶ�
//				animateK->setSpeed(animateK->getSpeed()*0.9f);
				//ִ�ж���
				boss[Constant::custom]->runAction(RepeatForever::create(animateK));
			}
		}else if(Constant::custom==0)
		{
			//���������Ź�������
			Animation3D *animationHero = Animation3D::create(C3B_PATH+std::string("boss1.c3b"));
			CCLOG("==================heroDuration==%f",animationHero->getDuration());//��ӡ��������
			if (animationHero)
			{
				//Ӣ�۸��ֶ���
				Animate3D *animateK = Animate3D::create(animationHero,0.0f,1.0f);
				//���������ٶ�
				animateK->setSpeed(animateK->getSpeed()*0.6f);
				//ִ�ж���
				boss[Constant::custom]->runAction(RepeatForever::create(animateK));
			}
		}else
		{
			//���������Ź�������
			Animation3D *animationHero = Animation3D::create(C3B_PATH+std::string("boss3.c3b"));
			CCLOG("==================heroDuration==%f",animationHero->getDuration());//��ӡ��������
			if (animationHero)
			{
				//���������Ź�������
				Animation3D *animationHero = Animation3D::create(C3B_PATH+std::string("boss3.c3b"));
				CCLOG("==================heroDuration==%f",animationHero->getDuration());//��ӡ��������
				if (animationHero)
				{
					//Ӣ�۸��ֶ���
					Animate3D *animateK1 = Animate3D::createWithFrames(animationHero,0,5);
					Animate3D *animateK2 = Animate3D::createWithFrames(animationHero,15,25);
					Animate3D *animateK3 = Animate3D::createWithFrames(animationHero,55,60);
					Animate3D *animateK4 = Animate3D::create(animationHero,5.0f,3.0f);
					//���������ٶ�
//					animateK1->setSpeed(animateK1->getSpeed()*0.2f);
//					animateK2->setSpeed(animateK2->getSpeed()*0.2f);
//					animateK3->setSpeed(animateK3->getSpeed()*0.2f);
					animateK4->setSpeed(animateK4->getSpeed()*0.001f);
					//ִ�ж���
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
	//�ı�boss��Ϣ ����
	for(int i=0;i<3;i++){
		stars[i]->setTextureRect(Rect(0,0,98.0*Constant::bossSkill[Constant::custom][i]/100,15));
	}
}

//�������˵�
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
