#include "First3DLayer.h"
#include "AppMacros.h"
#include "GameSceneManager.h"
#include "Missile.h"
#include "NewPlane.h"
#include "Constant.h"
#include "PublicApi.h"
#include "ControlGame.h"
#include "EnemyPlane.h"
#include "math.h"
#include "cocos2d.h"
#include <string>
//USING_NS_CC;
using namespace cocos2d;
using namespace ui;

std::vector<Sprite3D*> First3DLayer::enemyVec;
std::vector<BillBoard*> First3DLayer::bulletVec;
std::vector<BillBoard*> First3DLayer::explosionVec;
std::vector<Sprite3D*> First3DLayer::enemyDel;
std::vector<BillBoard*> First3DLayer::bulletDel;
std::vector<BillBoard*> First3DLayer::explosionDel;

bool First3DLayer::init()
{
	//���ø���ĳ�ʼ��
    if ( !Layer::init())
    {
        return false;
    }
    Constant::gameingFlag=true;			//������Ϸ��
    back1 = Sprite3D::create(MENU_PATH+"background.obj",MENU_PATH+StringUtils::format("background%d.jpg",Constant::custom).c_str());
	back1->setPosition3D(Vec3(270,1200,-500));
	this->addChild(back1,BACKGROUND_LEVEL);

	back2 = Sprite3D::create(MENU_PATH+"background.obj",MENU_PATH+StringUtils::format("background%d.jpg",Constant::custom).c_str());
	back2->setPosition3D(Vec3(270,1200+2400,-500));
	this->addChild(back2,BACKGROUND_LEVEL);

	getProperty();					//��ȡ��Ϸ��һЩ����

	//������  һ�㶼��Ҫ��
	AmbientLight *ambientLight=AmbientLight::create(Color3B(150, 150, 150));
	ambientLight->setLightFlag(LightFlag::LIGHT1);
	this->addChild(ambientLight);
	//�����
	directionLight= DirectionLight::create(Vec3(0.0f, -1.0f, -1.0f), Color3B(200, 200, 200));
	directionLight->setLightFlag(LightFlag::LIGHT2);
	this->addChild(directionLight);

	//����
	Vector<SpriteFrame*> animFrames;
	Animation *anim;

	for(int i=0;i<5;i++)
	{
		SpriteFrame *frame=SpriteFrame::create(
				PROTECT_PATH+StringUtils::format("bh%d.png",(i)).c_str(),Rect(0,0,180,180));
		animFrames.pushBack(frame);
	}
	anim=Animation::createWithSpriteFrames(animFrames,0.2);
	protectAnim=Animate::create(anim);
	protectAnim->retain();
	//��ը
	Vector<SpriteFrame*> animFrames1;
	Animation *anim1;

	for(int i=0;i<64;i++)
	{
		SpriteFrame *frame=SpriteFrame::create(
				BOOM_PATH+StringUtils::format("100%d.png",(i)).c_str(),Rect(0,0,256,256));
		animFrames1.pushBack(frame);
	}
	anim1=Animation::createWithSpriteFrames(animFrames1,0.02);
	boomAnim=Animate::create(anim1);
	boomAnim->retain();

	//���ô˲�������Ⱦ�������
	this->setCameraMask((unsigned short)CameraFlag::USER1);

	Sprite *a=Sprite::create();
	this->addChild(a);
	a->runAction(Sequence::create(DelayTime::create(2),
			CallFunc::create(CC_CALLBACK_0(First3DLayer::startGame,this)),
			nullptr));

    return true;
}
//��ʼ��Ϸ
void First3DLayer::startGame()
{
	createPlane();					//�����ɻ�
	//�����Ϸ�ӵ����л�
	controlBullet = ControlBullet::create(plane);
	this->addChild(controlBullet);
	controlGame = ControlGame::create(plane);
	controlGame->controlBullet=controlBullet;
	this->addChild(controlGame);
	exManager = ExManager::create();
	this->addChild(exManager);
	coin=Coin::create();							//�������
	coin->createVec();
	this->addChild(coin);
	//��ʱ�ص������ƶ���תObjģ���Լ�c3tģ��
	schedule(schedule_selector(First3DLayer::update), 0.01f);

	//�����Ľ�����
	string route1 = GAME_PATH + "loadingBack.png";		//����ͼƬ
	string route2 = GAME_PATH + "load.png";				//��������ͼƬ
	string route3 = GAME_PATH + "transparent.png";		//��ťͼƬ

	potentiometer = ControlPotentiometer::create(				//����Բ������������
												 route1.c_str(),route2.c_str(),route3.c_str());
	potentiometer->setMinimumValue(0);
	potentiometer->setMaximumValue(100);
	potentiometer->setValue(100);
	potentiometer->setEnabled(false);
	tempProgress=BillBoard::create();
	tempProgress->addChild(potentiometer);
	tempProgress->setPosition3D(Vec3(0,-10,0));
	tempProgress->setVisible(false);
	plane->plane->addChild(tempProgress,MENU_LEVEL);
	tempProgress->setCameraMask((unsigned short)CameraFlag::USER1);

	temp=BillBoard::create();
	temp->setPosition(0,-10);
	temp->runAction(RepeatForever::create(protectAnim));
	temp->setVisible(false);
	plane->plane->addChild(temp,MENU_LEVEL);
	temp->setCameraMask((unsigned short)CameraFlag::USER1);
	//�����ӵ�
	schedule(SEL_SCHEDULE(&First3DLayer::playerBullets),0.15,5000, 0.01f);

	//����һ�����㴥������
	auto listener = EventListenerTouchOneByOne::create();
	//�����´�����
	listener->setSwallowTouches(true);
	//��ʼ����ʱ�ص�onTouchBegan����
	listener->onTouchBegan = CC_CALLBACK_2(First3DLayer::onMyTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(First3DLayer::onMyTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(First3DLayer::onMyTouchEnded, this);
//	//��ӵ�������
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);

	//������Ӧ
	Device::setAccelerometerEnabled(true);			//��������������
	auto listener1 = EventListenerAcceleration::create(	//����������
	CC_CALLBACK_2(First3DLayer::onMyAcceleration, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);	//ע�������

}
//��ȡ��Ϸ��һЩ����
void First3DLayer::getProperty()
{
	planeId=Constant::planeId;			//�ɻ����
	gameLevel=Constant::custom;			//�ؿ��ȼ�
	bulletsGrade=Constant::attackGradePre[planeId]+Constant::attackGrade[planeId];	//�ӵ��Ĺ����ٶ�
	attackGrade=Constant::attackGradePre[planeId]+Constant::attackGrade[planeId];	//������
	bloodGrade=Constant::bloodGradePre[planeId]+Constant::bloodGrade[planeId];		//Ѫ��
	goldGrade=Constant::goldGradePre[planeId]+Constant::goldGrade[planeId];			//��Ҽӳ�
}
//������Ϸ��Ϣ
void First3DLayer::resetGame()
{
	Constant::planeHealth=200;
	Constant::score=0;
	Constant::bulletLevel=0;								//�ӵ���ĵȼ�
	Constant::bulletPower=0;								//�ӵ�������
	Constant::missileLevel=0;								//�����ĵȼ�
	Constant::attackCount=0;								//���еĸ���
	Constant::protectCount=0;								//�����ĵȼ�
	Constant::isBulletPower=false;							//�����ӵ�����û��
	delete plane;
	plane=nullptr;
	this->removeAllChildren();
}
//�����ӵ�
void First3DLayer::playerBullets(float dt)
{
	Point point=plane->plane->getPosition();
//	float angle=PublicApi::turnAngle(Point(100,200),Point(500,600));
	int bulletNum1=0;
	int bulletNum2=2;

	if(Constant::bulletPower==1||Constant::bulletPower==2||Constant::bulletPower==3){
		bulletNum1=2;
	}

	if(planeId==0){
		if(Constant::bulletLevel==0){
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(-40,50),600+300*bulletsGrade,point+Point(-40,1000),0));
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(40,50),600+300*bulletsGrade,point+Point(40,1000),0));
		}else if(Constant::bulletLevel==1){
			controlBullet->addChild(Bullet::create(bulletNum2,point+Point(0,80),600+300*bulletsGrade,point+Point(0,1000),0));
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(-40,50),600+300*bulletsGrade,point+Point(-40,1000),0));
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(40,50),600+300*bulletsGrade,point+Point(40,1000),0));
		}else if(Constant::bulletLevel==2){
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(-20,50),600+300*bulletsGrade,point+Point(-20,1000),0));
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(20,50),600+300*bulletsGrade,point+Point(20,1000),0));
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(-60,30),600+300*bulletsGrade,point+Point(-60,1000),0));
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(60,30),600+300*bulletsGrade,point+Point(60,1000),0));
		}else if(Constant::bulletLevel==3||Constant::bulletLevel==4){
			controlBullet->addChild(Bullet::create(bulletNum2,point+Point(0,80),600+300*bulletsGrade,point+Point(0,1000),0));
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(-80,50),600+300*bulletsGrade,point+Point(-80,1000),0));
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(80,50),600+300*bulletsGrade,point+Point(80,1000),0));
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(-40,30),600+300*bulletsGrade,point+Point(-40,1000),0));
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(40,30),600+300*bulletsGrade,point+Point(40,1000),0));
		}
	}else if(planeId==1){
		if(Constant::bulletLevel==0){
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(-10,50),600+300*bulletsGrade,point+Point(-97,1000),-5));
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(10,50),600+300*bulletsGrade,point+Point(97,1000),5));
		}else if(Constant::bulletLevel==1){
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(0,80),600+300*bulletsGrade,point+Point(0,1000),0));
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(-40,50),600+300*bulletsGrade,point+Point(-216,1050),-10));
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(40,50),600+300*bulletsGrade,point+Point(216,1050),10));
		}else if(Constant::bulletLevel==2){
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(-10,50),600+300*bulletsGrade,point+Point(-97,1000),-5));
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(10,50),600+300*bulletsGrade,point+Point(97,1000),5));
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(-40,30),600+300*bulletsGrade,point+Point(-216,1050),-10));
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(40,30),600+300*bulletsGrade,point+Point(216,1050),10));
		}else if(Constant::bulletLevel==3||Constant::bulletLevel==4){
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(0,80),600+300*bulletsGrade,point+Point(0,1000),0));
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(-20,50),600+300*bulletsGrade,point+Point(-108,1050),-5));
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(20,50),600+300*bulletsGrade,point+Point(108,1050),5));
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(-40,30),600+300*bulletsGrade,point+Point(-216,1050),-10));
			controlBullet->addChild(Bullet::create(bulletNum1,point+Point(40,30),600+300*bulletsGrade,point+Point(216,1050),10));
		}
	}else if(planeId==2){
		if(Constant::bulletLevel==0){
			controlBullet->addChild(Bullet::create(bulletNum2,point+Point(10,80),600+300*bulletsGrade,point+Point(10,1000),0));
			controlBullet->addChild(Bullet::create(bulletNum2,point+Point(-10,80),600+300*bulletsGrade,point+Point(-10,1000),0));
		}else if(Constant::bulletLevel==1||Constant::bulletLevel==2){
			controlBullet->addChild(Bullet::create(bulletNum2,point+Point(10,80),600+300*bulletsGrade,point+Point(10,1000),0));
			controlBullet->addChild(Bullet::create(bulletNum2,point+Point(-10,80),600+300*bulletsGrade,point+Point(-10,1000),0));
			controlBullet->addChild(Bullet::create(bulletNum2,point+Point(-40,50),600+300*bulletsGrade,point+Point(-216,1050),-10));
			controlBullet->addChild(Bullet::create(bulletNum2,point+Point(40,50),600+300*bulletsGrade,point+Point(216,1050),10));
		}else if(Constant::bulletLevel==3||Constant::bulletLevel==4){
			controlBullet->addChild(Bullet::create(bulletNum2,point+Point(0,80),600+300*bulletsGrade,point+Point(-10,1000),0));
			controlBullet->addChild(Bullet::create(bulletNum2,point+Point(20,80),600+300*bulletsGrade,point+Point(20,1000),0));
			controlBullet->addChild(Bullet::create(bulletNum2,point+Point(-20,80),600+300*bulletsGrade,point+Point(-20,1000),0));
			controlBullet->addChild(Bullet::create(bulletNum2,point+Point(-60,30),600+300*bulletsGrade,point+Point(-236,1050),-10));
			controlBullet->addChild(Bullet::create(bulletNum2,point+Point(60,30),600+300*bulletsGrade,point+Point(236,1050),10));
		}
	}

}
//�����ɻ��͵���
void First3DLayer::createPlane()
{
	plane=new NewPlane(this);			//�����ɻ�
//	plane->planeReturn();					//һֱ���ؽǶ�
	createMissile();						//��������
}
//��������
void First3DLayer::createMissile()
{
//	ms=new Missile(this);
	float missileTime=2;
//			5-Constant::missileLevel;
	schedule(schedule_selector(First3DLayer::updateCreateMissile),missileTime);
}
//���µ����Ĵ���
void First3DLayer::updateCreateMissile(float f)
{
	//��ȡ��ҷɻ���λ��
	float planeX=(plane->plane)->getPositionX();
	float planeY=(plane->plane)->getPositionY();

	enemyPlane[0]=nullptr;
	enemyPlane[1]=nullptr;
//	ControlGame *controlGame=dynamic_cast<First3DLayer*>(this->getParent())->controlGame;
	Vector<Node*> cTemp = controlGame->getChildren();
	Vector<Node*>::iterator cIter=cTemp.begin();
	for(;cIter!=cTemp.end();cIter++)
	{
		EnemyPlane *eTemp = dynamic_cast<EnemyPlane*>(*cIter);
		if(eTemp)
		{
			if(enemyPlane[0]){
				enemyPlane[1]=eTemp;
				break;
			}else{
				enemyPlane[0]=eTemp;
			}
		}
	}
	if(!enemyPlane[1]){
		enemyPlane[1]=enemyPlane[0];
	}
	//��������
	if(enemyPlane[0])
	{
		ms=new Missile(enemyPlane[0],this,-1,1);
		enemyPlane[0]->setMissile(ms);
	}
	if(enemyPlane[1])
	{
		ms=new Missile(enemyPlane[1],this,1,1);
		enemyPlane[1]->setMissile(ms);
	}
}
//ɾ���ɻ�
void First3DLayer::deletePlane()
{
	plane->unschedule(SEL_SCHEDULE(&NewPlane::updateRotate));
	this->unschedule(SEL_SCHEDULE(&First3DLayer::update));
	this->unschedule(SEL_SCHEDULE(&First3DLayer::updateCreateMissile));
	this->unschedule(SEL_SCHEDULE(&First3DLayer::updateProgress));
}

void First3DLayer::update(float dt)
{
    
	controlGame->update();
	controlBullet->update();
	coin->update();
	//������������
	int yy=back1->getPosition().y;
	if(yy==-1200)
	{
		yy=1200+2400;
	}
	back1->setPositionY(yy-3);

	yy=back2->getPosition().y;
	if(yy==-1200)
	{
		yy=1200+2400;
	}
	back2->setPositionY(yy-3);

	//����Ƿ�Ӧִ�м���
	if(attackFlag)
	{
		attackAll();
	}
	if(protectFlag)
	{
		CCLOG("---------------upddate");
		protectMyself();
	}

}
void First3DLayer::changeCamera(float x)
{
	if(x>40){
		x=x-500;
	}
	camera->setPosition3D(Vec3(270,-218.4*2,1200));		//���������λ��
	camera->lookAt(Vec3(270+x/2,480,0), Vec3(0,0.2747,0.9615));//���������Ŀ����Լ�up����
}
//��ȡ��������
void First3DLayer::getAnimPoint(Vec3 near,Vec3 far)
{
	float x0=near.x;
	float y0=near.y;
	float z0=near.z;
	float x1=far.x;
	float y1=far.y;
	float z1=far.z;

	float t=-z0/(z1-z0);
	float x=x0+(x1-x0)*t;
	float y=y0+(y1-y0)*t;
	plane->animPoint=Vec2(x,y);
}
//������ʼ
bool First3DLayer::onMyTouchBegan(Touch* touch, Event* event)
{
	if(touchFlag){			//���ڴ�����
		return false;
	}else{					//�Ѿ�̧��
		touchFlag=true;
	}
	if(plane->action&&(!Constant::touchFlag)){	//�жϴ������Ƿ���Ŀ��ķ�Χ��
		plane->action=false;
		auto target = static_cast<Sprite*>(	event->getCurrentTarget());//��ȡ��ǰ�������󣬲�ת��Ϊ��������
		Point location = target->convertToNodeSpace(touch->getLocation());//��ȡ��ǰ����
		//��ȡ��ҷɻ���λ��
		float planeX=plane->plane->getPositionX();
		float planeY=plane->plane->getPositionY();

		Size size = Director::getInstance()->getWinSize();
		Vec3 nearP(location.x, size.height-location.y-100, -1.0f);
		Vec3 farP(location.x, size.height-location.y-100, 0.0f);

		camera->unproject(size, &nearP, &nearP);
		camera->unproject(size, &farP, &farP);

		getAnimPoint(nearP,farP);
		//��βЧ��===============
		//��һΪ��������ʱ�����ζ�Ϊ��β�������ڶ�������С����,����Ϊ��β�����Ŀ��,����Ϊ������ɫֵ������Ϊ��ʹ�õ�����
		myStreak = MotionStreak::create(0.15, 1, 15, Color3B(255,255,200), GAME_PATH+"streak.png");//MotionStreak::create(0.4, 1, 17, Color3B(255,255,255), pics_PATH+"streak.png")
		myStreak->setBlendFunc(BlendFunc::ADDITIVE);//���û��
		this->addChild(myStreak,20);//��myStreak��ӵ�������
		//���ô˲�������Ⱦ�������
		myStreak->setCameraMask((unsigned short)CameraFlag::USER1);
		location=plane->animPoint;					//������������ϵ
		if((location.x-planeX)>1){
			plane->directionLeft=1;					//�ҷ�
		}else{
			plane->directionLeft=-1;				//���
		}
		if((location.y-planeY)>0){					//��ǰ��б  	90-
			plane->directionFront=-1;
		}else if((planeY-location.y)<450){			//�����б  90+
			plane->directionFront=1;
		}else{
			plane->directionFront=0;				//��շ�
		}

		if(plane->directionFront==0){
			plane->oneRotatePlane();
		}else{
			plane->oneMovePlane();
		}
	}
	return true;
}
//����
void First3DLayer::onMyTouchMoved(Touch *touch, Event *event)
{
	if(plane->action&&(!Constant::touchFlag)&&touchFlag){
//		plane->action=false;						//�ƶ���־λ����false
		auto target = static_cast<Sprite*>(	event->getCurrentTarget());//��ȡ��ǰ�������󣬲�ת��Ϊ��������
		Point location = target->convertToNodeSpace(touch->getLocation());//��ȡ��ǰ����
		Point delta=touch->getDelta();

		Size size = Director::getInstance()->getWinSize();
		Vec3 nearP(location.x, size.height-location.y-100, -1.0f);
		Vec3 farP(location.x, size.height-location.y-100, 0.0f);
		camera->unproject(size, &nearP, &nearP);
		camera->unproject(size, &farP, &farP);
		getAnimPoint(nearP,farP);
		plane->deltaPoint=delta;
		//��ȡ��ҷɻ���λ��

		location=plane->animPoint;
        if(myStreak)
        {
            myStreak->setPosition(location-Point(0,80));
        }
		plane->animPoint=location;

		if((fabs(delta.x)>1||fabs(delta.y)>1)){

			if((delta.x)>0){
				plane->directionLeft=1;				//�ҷ�
			}else{
				plane->directionLeft=-1;				//���
			}
			if(delta.y>0){							//��ǰ��б  	90-
				plane->directionFront=-1;
			}else if((delta.y)>-50){				//�����б  90+
				plane->directionFront=1;
			}else{
				plane->directionFront=0;					//��շ�
			}
			if(plane->directionFront==0){
				if(plane->action){
					plane->rotatePlane();
			}}
			plane->movePlane();
}}}

void First3DLayer::onMyTouchEnded(Touch *touch, Event *event)
{
	touchFlag=false;
	if(myStreak&&!Constant::touchFlag){
		this->removeChild(myStreak,true);//�Ƴ�myStreak
        myStreak=nullptr;
	}
}
//������Ӧ��
void First3DLayer::onMyAcceleration(Acceleration* acc, Event* unused_event)
{
	if(Constant::touchFlag){
		if((acc->x+0.6)>0){
			plane->movePlane2(Point(-acc->y*(10+Constant::sensitivity),(acc->x+0.6)*(10+Constant::sensitivity/2)));
		}else{
			plane->movePlane2(Point(-acc->y*(10+Constant::sensitivity),(acc->x+0.6)*(10+Constant::sensitivity)));
		}
	}
}
//�������зɻ�
void First3DLayer::attackAll()
{
	attackFlag=false;
	attackAnimFlag=true;
	CCLOG("run attack all plane");
	//�������зɻ�
	Vector<Node*> vTemp = controlGame->getChildren();
	Vector<Node*>::iterator iter=vTemp.begin();
	for(;iter!=vTemp.end();iter++)
	{
		EnemyPlane *temp = dynamic_cast<EnemyPlane*>(*iter);
		if(temp)
		{
			temp->hurt(300,Point(0,0));
		}
	}
	//�ӵ�
	Vector<Node*> vTemp2 = controlBullet->getChildren();
	Vector<Node*>::iterator iter2=vTemp2.begin();
	for(;iter2!=vTemp2.end();iter2++)
	{
		Bullet *temp2 = dynamic_cast<Bullet*>(*iter2);
		if(temp2 && temp2->num==1)
		{
			exManager->createPlane(temp2->getPosition3D());
			temp2->removeFromParent();
		}
	}
	//��������ִ�б�ը����
	Sprite *temp=Sprite::create();
	temp->setPosition(270,480);
	temp->setScale(2.5f);
	this->addChild(temp);
	temp->runAction(Sequence::create(boomAnim,
		CallFunc::create(CC_CALLBACK_0(First3DLayer::attackAllFinish,this)),
		RemoveSelf::create(),nullptr));
}
void First3DLayer::clearAllBullets()
{
	//�ӵ�
	Vector<Node*> vTemp2 = controlBullet->getChildren();
	Vector<Node*>::iterator iter2=vTemp2.begin();
	for(;iter2!=vTemp2.end();iter2++)
	{
		Bullet *temp2 = dynamic_cast<Bullet*>(*iter2);
		if(temp2)				//��������ӵ�
		{
			exManager->createPlane(temp2->getPosition3D());
			temp2->removeFromParent();
		}
	}
}
//�����ɻ����ܽ���
void First3DLayer::attackAllFinish()
{
	attackAnimFlag=false;
	CCLOG("finish attack all plane");
}
//�����ɻ�
void First3DLayer::protectMyself()
{
	CCLOG("---------------protectmyself");
	protectFlag=false;
	protectAnimFlag=true;
	CCLOG("run protect my plane");

	tempProgress->setVisible(true);
	temp->setVisible(true);
	auto director = Director::getInstance();
	auto sched = director->getScheduler();
	sched->scheduleSelector(SEL_SCHEDULE(&First3DLayer::updateProgress),this,0.01,false);

}
//����Բ�̽�����
void First3DLayer::updateProgress()
{
	progressNum--;
	potentiometer->setValue(progressNum/5);
	if(progressNum==0){
		progressNum=500;
		temp->setVisible(false);
		tempProgress->setVisible(false);
		protectMyselfFinish();
		this->unschedule(SEL_SCHEDULE(&First3DLayer::updateProgress));
	}
}
//�����ɻ����ܽ���
void First3DLayer::protectMyselfFinish()
{
	protectAnimFlag=false;
//	potentiometer->setEnabled(false);
	CCLOG("finish protect my plane");
}
