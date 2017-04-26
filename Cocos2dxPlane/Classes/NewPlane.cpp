#include "NewPlane.h"
#include "Constant.h"
#include "AppMacros.h"
#include "First3DLayer.h"
#include "My2DLayer.h"
#include "GameSceneManager.h"
#include "Web2DLayer.h"
#include "GameData.h"

using namespace cocos2d;

vector<Sprite3D*> NewPlane::planeVec;

NewPlane::NewPlane(Layer *gamelayer)
{
	this->layer=gamelayer;
	//�½��ɻ�
	plane=Sprite3D::create(C3B_PATH+StringUtils::format("plane0%d.obj",Constant::planeId+1),C3B_PATH+StringUtils::format("plane0%d.jpg",Constant::planeId+1));
	plane->setScale(0.1);
	plane->setRotation3D(Vec3(0,0,0));
	plane->setPosition3D(Vec3(270,-100,0));
	layer->addChild(plane,PLANE_LEVEL);
	plane->runAction(
			Sequence::create(
			Spawn::create(
			EaseBounceOut::create(MoveTo::create(1.2f,Point(270,180))),
			ScaleTo::create(1.2f,1.0f),
			RotateBy::create(1.2f,Vec3(0,-720,0)),nullptr),
			CallFunc::create(CC_CALLBACK_0(NewPlane::planeReturn,this)),nullptr
			));
	plane->setCameraMask((unsigned short)CameraFlag::USER1);		//��������
	canMove();														//�ɻ������ƶ�
}
//��ʼ��շ�
void NewPlane::rotatePlane()
{
	auto director = Director::getInstance();
	auto sched = director->getScheduler();
	sched->scheduleSelector(SEL_SCHEDULE(&NewPlane::updateBackflip),this,0.01,false);
}
//�ɻ���move�������ƶ��ͷɻ��Ƕȵĸı�
void NewPlane::movePlane()
{
	Vec3 angle=plane->getRotation3D();
	float angleX=angle.x;
	float angleY=angle.y;

	angleX=angleX-deltaPoint.y*rotateSpeed/2*(1-fabs(angleX)/maxRotateX);	//ǰ��
	if(angleX>0){
		angleX=0;
	}
	angleY=angleY+deltaPoint.x*rotateSpeed*(1-fabs(angleY)/maxRotateY);		//����

	plane->setRotation3D(Vec3(angleX,angleY,0));
	plane->setPosition(Vec2(animPoint));
	Constant::planePoint=animPoint;
	action=true;
}
//�ɻ������ƶ�
void NewPlane::canMove()
{
	action=true;
}
//began�ķɻ��ƶ�
void NewPlane::oneMovePlane()
{
	float planeX=plane->getPositionX();
	float planeY=plane->getPositionY();
	float enemyX=animPoint.x;
	float enemyY=animPoint.y;

	angleLeft=fabs(enemyX-planeX)/10;
	angleFront=fabs(enemyY-planeY)/20;
	if(angleLeft>=maxRotateY){
		angleLeft=maxRotateY;
	}else if(angleFront>=maxRotateX){
		angleFront=maxRotateX;
	}
	//��ת�Ƕ�
	auto director = Director::getInstance();
	auto sched = director->getScheduler();
	sched->scheduleSelector(SEL_SCHEDULE(&NewPlane::updatePlane),this,0.01,false);

	angleX=plane->getRotation3D().x;
	angleY=plane->getRotation3D().y;
	plane->runAction(
			Sequence::create(
				MoveTo::create(0.25,animPoint),
				CallFunc::create(CC_CALLBACK_0(NewPlane::canMove,this)),
				nullptr
	));
}
//began�ķɻ���ת
void NewPlane::oneRotatePlane()
{
	action=false;
	auto director = Director::getInstance();
	auto sched = director->getScheduler();
	sched->scheduleSelector(SEL_SCHEDULE(&NewPlane::updateBackflip),this,0.01,false);
	plane->runAction(
					Sequence::create(
							MoveTo::create(0.25,animPoint),
								CallFunc::create(CC_CALLBACK_0(NewPlane::canMove,this)),
									nullptr
								));
}
//ʱʱ���·ɻ��ĽǶ�
void NewPlane::updatePlane(float f)
{
	if(angleLeft>4){
		angleLeft=angleLeft-4;
		angleY=angleY+4*directionLeft;
	}
	if(angleFront>2){
		angleFront=angleFront-2;
		if(directionFront==-1){
			angleX=angleX+2*directionFront;
		}
	}
	if(angleLeft<=4&&angleFront<=2){
		this->unschedule(SEL_SCHEDULE(&NewPlane::updatePlane));
		action=true;
	}
	plane->setRotation3D(Vec3(angleX,angleY,0));
}

//�ɻ����к�շ�
void NewPlane::updateBackflip()
{
	float angleX=plane->getRotation3D().x;		//��ȡX��ƫת�Ƕ�
	float anlgeY=plane->getRotation3D().y;		//��ȡY��ƫת�Ƕ�

	angleX=angleX+9;							//����X��ƫת�Ƕ�

	if(angleX>=360){
		plane->setRotation3D(Vec3(0,angleY,0));
		this->unschedule(SEL_SCHEDULE(&NewPlane::updateBackflip));
		action=true;
	}else{
		plane->setRotation3D(Vec3(angleX,anlgeY,0));
	}
}
//���������Ƕ�
void NewPlane::planeReturn()
{
	plane->setRotation3D(Vec3(0,0,0));
	auto director = Director::getInstance();
	auto sched = director->getScheduler();
	sched->scheduleSelector(SEL_SCHEDULE(&NewPlane::updateRotate),this,0.002,false);
}
//�ɻ���ͣ�Ļָ���ת�Ƕ�
void NewPlane::updateRotate()
{
	float angleX=plane->getRotation3D().x;	//��ȡ�ɻ�X����ת�Ƕ�
	float angleY=plane->getRotation3D().y;	//��ȡ�ɻ�Y����ת�Ƕ�

	if(angleX>=1){							//�ж�X��ĽǶ�
		angleX=angleX-0.3;
	}else if(angleX<=-1){
		angleX=angleX+0.3;
	}else{
		angleX=0;
	}
	if(angleY>=1){							//�ж�Y��ĽǶ�
		angleY=angleY-0.5;
	}else if(angleY<=-1){
		angleY=angleY+0.5;
	}else{
		angleY=0;
	}
	plane->setRotation3D(Vec3(angleX,angleY,0));	//���÷ɻ�����ת�Ƕ�
}
void NewPlane::movePlane2(Point delta)
{
	//��ȡ��ҷɻ���λ��
	float planeX=plane->getPositionX();
	float planeY=plane->getPositionY();

	animPoint=Point(planeX+delta.x,planeY+delta.y);
	deltaPoint=delta;
	if(animPoint.x>540){
		animPoint.x=540;
	}else if(animPoint.x<0){
		animPoint.x=0;
	}
	if(animPoint.y>1400){
		animPoint.y=1400;
	}else if(animPoint.y<0){
		animPoint.y=0;
	}

	if((fabs(delta.x)>0||fabs(delta.y)>0)){
		if(tail){
			//��βЧ��===============
			//��һΪ��������ʱ�����ζ�Ϊ��β�������ڶ�������С����,����Ϊ��β�����Ŀ��,����Ϊ������ɫֵ������Ϊ��ʹ�õ�����
			myStreak = MotionStreak::create(0.15, 1, 15, Color3B(255,255,200), GAME_PATH+"streak.png");//MotionStreak::create(0.4, 1, 17, Color3B(255,255,255), pics_PATH+"streak.png")
			myStreak->setBlendFunc(BlendFunc::ADDITIVE);//���û��
			layer->addChild(myStreak,20);//��myStreak��ӵ�������
			//���ô˲�������Ⱦ�������
			myStreak->setCameraMask((unsigned short)CameraFlag::USER1);
			tail=false;
		}
		if(myStreak){								//�������βЧ��
			myStreak->setPosition(Point(animPoint.x,animPoint.y-80));
		}
		if((delta.x)>0){
			directionLeft=1;					//�ҷ�
		}else{
			directionLeft=-1;					//���
		}
		if(delta.y>0){							//��ǰ��б  	90-
			directionFront=-1;
		}else if((delta.y)>-50){				//�����б  90+
			directionFront=1;
		}else{
			directionFront=0;					//��շ�
		}

		if(directionFront==0){
			if(action){
				rotatePlane();
		}}
		movePlane();
	}
	if(myStreak&&Constant::touchFlag&&delta.x==0&&delta.y==0)
	{
		layer->removeChild(myStreak,true);//�Ƴ�myStreak
		tail=true;							//�����Լ���������β
	}
}
void NewPlane::hurt(int damage)
{
	bool isCanHurt=true;
//	if(GameData::playerNumber==-1){					// ������
//		isCanHurt=!dynamic_cast<First3DLayer*>(layer)->protectAnimFlag;
//	}else{
//		isCanHurt=!dynamic_cast<Web3DLayer*>(layer)->protectAnimFlag;
//	}
	if(isCanHurt)
	{
		if(GameData::playerNumber==-1){
			int health=Constant::planeHealth;
			int remainHealth=health-damage;
			if(remainHealth>0){
				Constant::planeHealth=remainHealth;
	//			First3DLayer *f3Layer=dynamic_cast<First3DLayer*>(layer);
	//			First3DLayer->exManager->createPlane(plane->getPosition3D());
				My2DLayer *m2l=dynamic_cast<My2DLayer*>(layer->getScene()->getChildByTag(0));
				if(m2l){
						m2l->sharkInjuredBG();
				}
			}else{
				My2DLayer *m2l=dynamic_cast<My2DLayer*>(layer->getScene()->getChildByTag(0));
				if(m2l){
					m2l->pauseGame();			//��ͣ��Ϸ
					m2l->completeGame(loseEnum);//��Ϸ���
			}}
		}else{											//����ģʽ
			int health;
			if(GameData::playerNumber==0){		//���
				health=GameData::redPlaneHealth;
			}else{								//����
				health=GameData::greenPlaneHealth;
			}
			int remainHealth=health-damage;
			if(remainHealth>0){
				if(GameData::playerNumber==0){		//���
					GameData::redPlaneHealth=remainHealth;
				}else{								//����
					GameData::greenPlaneHealth=remainHealth;
				}
				Web2DLayer *m2l=dynamic_cast<Web2DLayer*>(layer->getScene()->getChildByTag(0));
				if(m2l){
						m2l->sharkInjuredBG();
				}
			}else{
				Web2DLayer *m2l=dynamic_cast<Web2DLayer*>(layer->getScene()->getChildByTag(0));
				if(m2l){
					m2l->pauseGame();			//��ͣ��Ϸ
					m2l->completeGame(loseEnum);//��Ϸ���
			}}
		}

	}
}
