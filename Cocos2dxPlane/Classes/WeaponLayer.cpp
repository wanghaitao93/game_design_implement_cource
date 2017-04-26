#include "WeaponLayer.h"
#include "AppMacros.h"
#include "Constant.h"
#define left -1
#define right 1

#include "cocos2d.h"
//USING_NS_CC;
using namespace cocos2d;

bool WeaponLayer::init()
{
	//���ø���ĳ�ʼ��
    if ( !Layer::init() )
    {
        return false;
    }

    //����һ���������
    back = Sprite::create(WEAPON_PATH+std::string("weaponBG.png"));
    back->setPosition(Point(270,0));
    back->setAnchorPoint(Point(0.5,0));
    this->addChild(back, BACKGROUND_LEVEL);
    //���������۸��
    goldView = Sprite::create(WEAPON_PATH+std::string("gold.png"));
    goldView->setPosition(Point(270,220));
    this->addChild(goldView);
    //���������۸�����
    priceLabel=LabelAtlas::create("0",MENU_PATH+std::string("bnum1.png"),28,36,'0');
	priceLabel->setPosition(Point(110,38));
	priceLabel->setScale(0.8);
	//������Ǯ����
	goldView->addChild(priceLabel);
	goldLabel=LabelAtlas::create("0",MENU_PATH+std::string("bnum1.png"),28,36,'0');
	goldLabel->setPosition(Point(80,840));
	goldLabel->setScale(0.8);
	this->addChild(goldLabel);
	//�������
	coin = Sprite3D::create(
		C3B_PATH+std::string("coin.obj"),
		C3B_PATH+std::string("coin_gold.jpg")
	);
	coin->setPosition(Point(55,855));
	coin->setScale(0.7);
	coin->setRotation3D(Vec3(-85,0,0));
	this->addChild(coin);

	//��ʼ�� ��
	lock=Sprite::create(SELECTGAME_PATH+std::string("suo.png"));
	lock->setPosition3D(Vec3(270,645,150));
	lock->setVisible(false);
	this->addChild(lock,MENU_LEVEL);
	//��ʼ���˵�
    initMenu();
    initPlane();
//    initFrame();
    refreshGrade();
    refreshLabel();
    startAnim();
    menu3D[selectID]->runAction(RepeatForever::create(RotateBy::create(2,Vec3(0,360,0))));
	//����һ�����㴥������
	auto listener = EventListenerTouchOneByOne::create();
	//�����´�����
	listener->setSwallowTouches(true);
	//��ʼ����ʱ�ص�onTouchBegan����
	listener->onTouchBegan = CC_CALLBACK_2(WeaponLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(WeaponLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(WeaponLayer::onTouchEnded, this);
	//��ӵ�������
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);

	//������  һ�㶼��Ҫ��
	AmbientLight *ambientLight=AmbientLight::create(Color3B(100, 100, 100));
	this->addChild(ambientLight);
	//�����
	DirectionLight *directionLight= DirectionLight::create(Vec3(-1.0f, -1.0f, -1.0f), Color3B(200, 200, 200));
//	coin->addChild(directionLight);
//	for(int i=0;i<3;i++)
//	{
//		menu3D[i]->addChild(directionLight);
//		plane[i]->addChild(directionLight);
//	}
//	auto tintto1 = TintTo::create(4, 255, 0, 0);
//	auto tintto2 = TintTo::create(4, 0, 255, 0);
//	auto tintto3 = TintTo::create(4, 0, 0, 255);
//	auto tintto4 = TintTo::create(4, 255, 255, 255);
//	auto seq = Sequence::create(tintto1,tintto2, tintto3, tintto4, nullptr);
//	directionLight->runAction(RepeatForever::create(seq));
	this->addChild(directionLight);

	//���Ż�֡����
	Vector<SpriteFrame*> animFrames;
	Animation *anim;

	for(int i=0;i<63;i++)
	{
		SpriteFrame *frame=SpriteFrame::create(
				LEVEL_PATH+StringUtils::format("level (%d).png",(i+1)).c_str(),Rect(0,0,128,128));
		animFrames.pushBack(frame);
	}
	anim=Animation::createWithSpriteFrames(animFrames,0.02f);
	animLevel=Animate::create(anim);
	animLevel->retain();

	//��ʼ��tishi
	point=Sprite::create(WEAPON_PATH+std::string("lockBG.png"));
	point->setPosition3D(Vec3(270,520,60));
	this->addChild(point,MENU_LEVEL+1);
	point->setScale(0.1f);
	point->setVisible(false);
	point->runAction(EaseBounceOut::create(ScaleTo::create(0.2f,1.0)));
//
//	MenuItemImage *closeItem=MenuItemImage::create(
//			WEAPON_PATH+std::string("setClose.png"),
//			WEAPON_PATH+std::string("setClose_down.png"),
//			CC_CALLBACK_0(WeaponLayer::clearPoint,this)
//			);
//	closeItem->setPosition3D(Vec3(420,224,61));
//	closeItem->setAnchorPoint(Point(1,1));
//
//	Menu *menu=Menu::create(closeItem,nullptr);
//	menu->setPosition(Point::ZERO);
//	point->addChild(menu,MENU_LEVEL+2);
    return true;
}

void WeaponLayer::initFrame()
{
	Vector<SpriteFrame*> anim;									//��������֡����
	for(int i=0;i<20;i++)										//ѭ������
	{
		SpriteFrame* spriteFrame=SpriteFrame::create			//��������֡
		(
			ANIM_PATH+StringUtils::format("move/move (%d).png",i+1),
			Rect(0,0,152,152)
		);
		anim.pushBack(spriteFrame);								//���뼯��
	}
	Animation *animation = 										//������������
			Animation::createWithSpriteFrames(anim,0.1f,1);
	animate = Animate::create(animation);						//��������֡����
	animate->retain();
	frame = Sprite::create();
	frame->setPosition(70,390);
	frame->setScaleX(1.2f);
	frame->setScaleY(1.4f);
	this->addChild(frame,10);
	frame->runAction(RepeatForever::create(animate));
}

void WeaponLayer::startAnim()
{
	//��������Ч��********************
	ActionInterval *st=ScaleTo::create(0.5,1);
	//ֹͣ���ж���*******************
//	frame->setVisible(false);
//	frame->stopAllActions();
	attack->stopAllActions();
	blood->stopAllActions();
	bullets->stopAllActions();
	gold->stopAllActions();
	//�������а�ťλ��****************
	attack->setScale(0);
	blood->setScale(0);
	bullets->setScale(0);
	gold->setScale(0);
	//���Ŷ���*********************
	attack->runAction(Sequence::create(DelayTime::create(0.1),
			st->clone(),nullptr));
	blood->runAction(Sequence::create(DelayTime::create(0.4),
			st->clone(),nullptr));
	bullets->runAction(Sequence::create(DelayTime::create(0.2),
			st->clone(),nullptr));
	gold->runAction(Sequence::create(DelayTime::create(0.3),
			st->clone(),nullptr));
//	frame->runAction(Sequence::create(DelayTime::create(1),
//		CallFunc::create(CC_CALLBACK_0(WeaponLayer::callBack,this)),nullptr));
}

void WeaponLayer::callBack()
{
	frame->setVisible(true);
	frame->runAction(RepeatForever::create(animate));
}

void WeaponLayer::initMenu()
{
	menu=Menu::create();
	menu->setPosition(Point::ZERO);
	this->addChild(menu);
	//���ذ�ť**********************************
	MenuItem *backWelcome = MenuItemImage::create
	(
		HELP_PATH+"back_btn.png",
		HELP_PATH+"back_btn_down.png",
		CC_CALLBACK_0(WeaponLayer::menuSelect, this, 0) //���ʱִ�еĻص�����
	);
	backWelcome->setPosition(Point(80,70));
	menu->addChild(backWelcome);
	//�����ӳ�**********************************
	attack = MenuItemImage::create
	(
	   WEAPON_PATH+std::string("attack_add.png"),	//ƽʱ��ͼƬ
	   WEAPON_PATH+std::string("attack_add.png"),	//ѡ��ʱ��ͼƬ
	   CC_CALLBACK_0(WeaponLayer::menuSelect, this, 1) //���ʱִ�еĻص�����
	);
	attack->setPosition(Point(70,400));
	Sprite *attack_level=Sprite::create(WEAPON_PATH+std::string("zero.png"));
	attack_level->setPosition(Point(7,13));
	attack_level->setScale(0.5);
	attack_level->setAnchorPoint(Point(0,1));
	attack->addChild(attack_level,0,0);
	menu3D[0] = Sprite3D::create(C3B_PATH+std::string("gun.obj"),
				C3B_PATH+std::string("gun.jpg"));
	menu3D[0]->setPosition3D(Vec3(68,60,50));
	menu3D[0]->setScale(0.9f);
	attack->addChild(menu3D[0]);
	menu->addChild(attack,0,0);
	//�����ӳ�*********************************
	blood = MenuItemImage::create
	(
		   WEAPON_PATH+std::string("blood_add.png"),	//ƽʱ��ͼƬ
		   WEAPON_PATH+std::string("blood_add.png"),	//ѡ��ʱ��ͼƬ
		   CC_CALLBACK_0(WeaponLayer::menuSelect, this, 2) //���ʱִ�еĻص�����
	);
	blood->setPosition(Point(185,440));
	Sprite *blood_level=Sprite::create(WEAPON_PATH+std::string("zero.png"));
	blood_level->setPosition(Point(7,13));
	blood_level->setScale(0.5);
	blood_level->setAnchorPoint(Point(0,1));
	blood->addChild(blood_level,0,0);
	menu3D[1] = Sprite3D::create(C3B_PATH+std::string("update.obj"),
			C3B_PATH+std::string("dj22.png"));
	menu3D[1]->setPosition3D(Vec3(63,60,50));
	menu3D[1]->setRotation3D(Vec3(90,0,0));
	menu3D[1]->setScale(1.25);
	menu3D[1]->setForceDepthWrite(true);
	blood->addChild(menu3D[1]);
	menu->addChild(blood,0,1);
	//�ӵ�����**********************************
	bullets = MenuItemImage::create
	(
		   WEAPON_PATH+std::string("bullets_add.png"),	//ƽʱ��ͼƬ
		   WEAPON_PATH+std::string("bullets_add.png"),	//ѡ��ʱ��ͼƬ
		   CC_CALLBACK_0(WeaponLayer::menuSelect, this, 3) //���ʱִ�еĻص�����
	);
	bullets->setPosition(Point(300,400));
	Sprite *bullets_level=Sprite::create(WEAPON_PATH+std::string("zero.png"));
	bullets_level->setPosition(Point(7,13));
	bullets_level->setScale(0.5);
	bullets_level->setAnchorPoint(Point(0,1));
	bullets->addChild(bullets_level,0,0);
	menu3D[2] = Sprite3D::create(C3B_PATH+std::string("Missile4.obj"),
			C3B_PATH+std::string("Missile.png"));
	menu3D[2]->setPosition3D(Vec3(60,65,50));
	menu3D[2]->setRotation(-45);
	menu3D[2]->setScale(1.5);
	bullets->addChild(menu3D[2]);
	menu->addChild(bullets,0,2);
	//��Ҽӳ�**********************************
	gold = MenuItemImage::create
	(
		   WEAPON_PATH+std::string("gold_add.png"),	//ƽʱ��ͼƬ
		   WEAPON_PATH+std::string("gold_add.png"),	//ѡ��ʱ��ͼƬ
		   CC_CALLBACK_0(WeaponLayer::menuSelect, this, 4) //���ʱִ�еĻص�����
	);
	gold->setPosition(Point(415,440));
	Sprite *gold_level=Sprite::create(WEAPON_PATH+std::string("zero.png"));
	gold_level->setPosition(Point(7,13));
	gold_level->setScale(0.5);
	gold_level->setAnchorPoint(Point(0,1));
	gold->addChild(gold_level,0,0);
	menu3D[3] = Sprite3D::create(C3B_PATH+std::string("coin.obj"),
			C3B_PATH+std::string("coin_gold.jpg"));
	menu3D[3]->setPosition3D(Vec3(50,60,50));
	menu3D[3]->setRotation3D(Vec3(90,0,0));
	menu3D[3]->setScale(1.3);
	gold->addChild(menu3D[3]);
	menu->addChild(gold,0,3);
    //����************************************
    toLeft=MenuItemImage::create(
    	WEAPON_PATH+std::string("left_btn.png"),
    	WEAPON_PATH+std::string("left_btn_down.png"),
		CC_CALLBACK_0(WeaponLayer::menuSelect,this,5)
	);
    toLeft->setPosition(Point(80,650));
    menu->addChild(toLeft);
    //����************************************
    toRight=MenuItemImage::create(
    	WEAPON_PATH+std::string("right_btn.png"),
    	WEAPON_PATH+std::string("right_btn_down.png"),
		CC_CALLBACK_0(WeaponLayer::menuSelect,this,6)
	);
    toRight->setPosition(Point(460,650));
    menu->addChild(toRight);
	//������ť***********************************
	upgrade = MenuItemImage::create
	(
		   WEAPON_PATH+std::string("upgrade_btn.png"),	//ƽʱ��ͼƬ
		   WEAPON_PATH+std::string("upgrade_btn_down.png"),	//ѡ��ʱ��ͼƬ
		   CC_CALLBACK_0(WeaponLayer::menuSelect, this, 7) //���ʱִ�еĻص�����
	);
	upgrade->setPosition(Point(460,70));
	menu->addChild(upgrade);
	//����ť***********************************
	buyButton = MenuItemImage::create
	(
		   WEAPON_PATH+std::string("buy_btn.png"),	//ƽʱ��ͼƬ
		   WEAPON_PATH+std::string("buy_btn_down.png"),	//ѡ��ʱ��ͼƬ
		   CC_CALLBACK_0(WeaponLayer::menuSelect, this, 8) //���ʱִ�еĻص�����
	);
	buyButton->setPosition(Point(460,70));
	buyButton->setVisible(false);
	menu->addChild(buyButton);

}

void WeaponLayer::saveData()
{
	int i=Constant::planeId;
	UserDefault::getInstance()->setIntegerForKey(
		StringUtils::format("attack%d",i).c_str(),Constant::attackGrade[i]);
	UserDefault::getInstance()->setIntegerForKey(
		StringUtils::format("blood%d",i).c_str(),Constant::bloodGrade[i]);
	UserDefault::getInstance()->setIntegerForKey(
		StringUtils::format("bullets%d",i).c_str(),Constant::bulletsGrade[i]);
	UserDefault::getInstance()->setIntegerForKey(
		StringUtils::format("gold%d",i).c_str(),Constant::goldGrade[i]);
	UserDefault::getInstance()->setIntegerForKey(
		std::string("canPlane").c_str(),Constant::canPlane);			// ��ȡ�����ķɻ�
}

void WeaponLayer::initPlane()
{
	for(int i=0;i<3;i++)
	{
		plane[i] = Sprite3D::create(
			C3B_PATH+StringUtils::format("plane0%d.obj",i+1).c_str(),
			C3B_PATH+StringUtils::format("plane0%d.jpg",i+1).c_str()
		);
		plane[i]->setRotation3D(Vec3(-30,0,0));
		if(i==Constant::planeId){
			plane[i]->setPosition3D(Vec3(270,650,60
					));
		}else{
			plane[i]->setPosition3D(Vec3(800,650,60));
		}
		plane[i]->setScale(1.3);
		this->addChild(plane[i]);
	}
}

void WeaponLayer::changePlane(int mode)
{
	for(int i=0;i<3;i++)
	{
		plane[i]->stopAllActions();
		plane[i]->setRotation3D(Vec3(-30,0,0));
	}
	int planeId=Constant::planeId;
	plane[(planeId+3+mode)%3]->setPositionX(270);
	plane[planeId]->setPositionX(270-540*mode);
	plane[(planeId+3-mode)%3]->setPositionX(270-540*mode);
	ActionInterval *mb=MoveBy::create(1,Point(540*mode,0));
	ActionInterval *eeio=EaseElasticOut::create(mb);
	plane[(planeId+3+mode)%3]->runAction(eeio->clone());
	plane[planeId]->runAction(eeio);
}
void WeaponLayer::changeButton()
{
	if(Constant::planeId<=Constant::canPlane){	//�����ǰ�Ѿ���������
		lock->setVisible(false);
		upgrade->setVisible(true);
		buyButton->setVisible(false);
	}else{										//�����ǰ���ܽ���
		lock->setVisible(true);
		upgrade->setVisible(false);
		buyButton->setVisible(true);
	}
}
void WeaponLayer::refreshGrade()
{
	int intTemp[]={Constant::attackGrade[Constant::planeId],
			Constant::bloodGrade[Constant::planeId],
			Constant::bulletsGrade[Constant::planeId],
			Constant::goldGrade[Constant::planeId]};
	for(int i=0;i<4;i++)
	{
		switch(intTemp[i])
		{
		case 0:
			((Sprite*)menu->getChildByTag(i)->getChildByTag(0))->
					setTexture(WEAPON_PATH+std::string("zero.png"));
			break;
		case 1:
			((Sprite*)menu->getChildByTag(i)->getChildByTag(0))->
					setTexture(WEAPON_PATH+std::string("one.png"));
			break;
		case 2:
			((Sprite*)menu->getChildByTag(i)->getChildByTag(0))->
					setTexture(WEAPON_PATH+std::string("two.png"));
			break;
		case 3:
			((Sprite*)menu->getChildByTag(i)->getChildByTag(0))->
					setTexture(WEAPON_PATH+std::string("three.png"));
			break;
		case 4:
			((Sprite*)menu->getChildByTag(i)->getChildByTag(0))->
					setTexture(WEAPON_PATH+std::string("four.png"));
			break;
		}
	}
}

void WeaponLayer::menuSelect(int index)
{
	switch(index)
	{
	case 0:
		gsm->goMainMenu();
		return;
	case 1:
	case 2:
	case 3:
	case 4:
		menu3D[selectID]->stopAllActions();
		selectID=index-1;
		menu3D[selectID]->runAction(RepeatForever::create(RotateBy::create(2,Vec3(0,360,0))));
		break;
	case 5:
		{
			Constant::planeId=(Constant::planeId+2)%3;
			toLeft->stopAllActions();
			ActionInterval *lstb=ScaleTo::create(0.2,2);
			ActionInterval *lsts=ScaleTo::create(0.2,1);
			toLeft->runAction(Sequence::create(lstb,lsts,nullptr));
			changePlane(left);
			changeButton();
			startAnim();
		}
		break;
	case 6:
		{
			Constant::planeId=(Constant::planeId+1)%3;
			toRight->stopAllActions();
			ActionInterval *rstb=ScaleTo::create(0.2,2);
			ActionInterval *rsts=ScaleTo::create(0.2,1);
			toRight->runAction(Sequence::create(rstb,rsts,nullptr));
			changePlane(right);
			changeButton();
			startAnim();
		}
		break;
	case 7:									//����
		this->upGrade();
		this->saveData();
		break;
	case 8:									// ����ɻ�
		this->unLock();
		this->saveData();
		break;
	}
	refreshLabel();
	refreshGrade();
}
//����
void WeaponLayer::unLock()
{
	if(Constant::planeId==Constant::canPlane+1){		//�����һ��
		lock->runAction(
			Sequence::create(
				Blink::create(1.0f,4),
				CallFunc::create(CC_CALLBACK_0(WeaponLayer::changeButton,this)),
				nullptr
			));
		Constant::canPlane++;			//����ӵ�еķɻ�����һ
	}else{
		point->setVisible(true);		//��ʾ��ʾ��
		point->setScale(0.1f);
		point->runAction(
				Sequence::create(
						EaseBounceOut::create(ScaleTo::create(0.5,1.0)),
						DelayTime::create(1.0f),
						ScaleTo::create(0.5,0.1f),
						CallFunc::create(CC_CALLBACK_0(WeaponLayer::clearPoint,this)),
						nullptr
			));
	}
}
//�����ʾ
void WeaponLayer::clearPoint(){
	point->setVisible(false);
}
void WeaponLayer::upGrade()
{
	int *intTemp[]={&Constant::attackGrade[Constant::planeId],
		&Constant::bloodGrade[Constant::planeId],
		&Constant::bulletsGrade[Constant::planeId],
		&Constant::goldGrade[Constant::planeId]};
	if(Constant::money > price && *intTemp[selectID]<4)
	{
		Constant::money-=price;
		UserDefault::getInstance()->setIntegerForKey("money",Constant::money);
		//��������
		(*intTemp[selectID])++;
		//���Ż�֡����
		Sprite *a=Sprite::create();
		this->addChild(a);
		Vec3 vec;
		if(selectID==0){
			vec=Vec3(attack->getPosition().x+22,attack->getPosition().y-5,100);
		}else if(selectID==1){
			vec=Vec3(blood->getPosition().x+8,blood->getPosition().y-10,100);
		}else if(selectID==2){
			vec=Vec3(bullets->getPosition().x-5,bullets->getPosition().y-5,100);
		}else{
			vec=Vec3(gold->getPosition().x-20,gold->getPosition().y-10,100);
		}
//		a->setPosition3D(vec);
		a->setPosition3D(vec);
		a->runAction(Sequence::create(
				animLevel,
				RemoveSelf::create(),
				nullptr));
	}else{
		//ҡ��������ť
		upgrade->runAction(Sequence::create(
			MoveBy::create(0.05,Point(-10,0)),
			MoveBy::create(0.05,Point(20,0)),
			MoveBy::create(0.05,Point(-10,0)),
			nullptr));
	}
}

void WeaponLayer::refreshLabel()
{
	int intTemp[]={Constant::attackGrade[Constant::planeId],
		Constant::bloodGrade[Constant::planeId],
		Constant::bulletsGrade[Constant::planeId],
		Constant::goldGrade[Constant::planeId]};
	switch(intTemp[selectID])
	{
	case 0:
		price=800;
		break;
	case 1:
		price=2000;
		break;
	case 2:
		price=3600;
		break;
	case 3:
		price=6000;
		break;
	case 4:
		priceLabel->setString("");
		return;
	}
	if(Constant::planeId>Constant::canPlane){
		price=50000+Constant::planeId*10000;
	}
	stringstream ss1;
	ss1<<price;
	string s1 = ss1.str();
	priceLabel->setString(s1);
	stringstream ss2;
	ss2<<(Constant::money);
	string s2 = ss2.str();
	goldLabel->setString(s2);

}

bool WeaponLayer::onTouchBegan(Touch* touch, Event* event)
{
	return true;
}
void WeaponLayer::onTouchMoved(Touch *touch, Event *event)
{
	if(Constant::planeId<=Constant::canPlane){
		if(Rect(110,530,430,770).containsPoint(touch->getLocation()))
		{
			Point delta=touch->getDelta();
			if(fabs(delta.x)>0){
				float planeX=plane[Constant::planeId]->getRotation3D().x-delta.y/2;
				float planeY=plane[Constant::planeId]->getRotation3D().y+delta.x/2;
				plane[Constant::planeId]->setRotation3D(Vec3(planeX,planeY,0));
			}
		}
	}

}
void WeaponLayer::onTouchEnded(Touch *touch, Event *event)
{

}

WeaponLayer::~WeaponLayer()
{

}
