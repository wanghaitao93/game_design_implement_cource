#include "Missile.h"
#include "AppMacros.h"
#include "First3DLayer.h"
#include "ParticleManager.h"
#include "ControlGame.h"
#include "Constant.h"
//#include "EnemyPlane.h"


using namespace std;
using namespace cocos2d;

vector<Sprite3D*> Missile::missileVec;
Missile::Missile(){}
Missile::Missile(Node *enemy,Layer *layer,int d,float r)
{
	this->layer=layer;
	//getNewEnemyPlane();
	enemyPlane=enemy;
	missile=Sprite3D::create(std::string("c3b/Missile4.obj"),std::string("c3b/Missile.png"));
	if(missile&&enemyPlane!=nullptr){
		missile->setScale(r);
		missile->setPosition(Constant::planePoint-Point(0,50));
		layer->addChild(missile,MISSILE_LEVEL);
		//���� 1--��,-1--��
		radio=r;
		direction=d;
		move();
		missile->setCameraMask((unsigned short)CameraFlag::USER1);
	}

}
Missile* Missile::createWebMissile(float pointX,float pointY,float pointZ,
		float angleX,float angleY,float angleZ)
{
	Missile *missile=new Missile();
//	Missile *missile=(Missile*)Sprite3D::create(C3B_PATH+std::string("Missile4.obj"),C3B_PATH+std::string("Missile.png"));
	if(missile&&missile->initWithFile(C3B_PATH+std::string("Missile4.obj"))){
		missile->_contentSize=missile->getBoundingBox().size;
		missile->setTexture(C3B_PATH+std::string("Missile.png"));
		missile->setPosition3D(Vec3(pointX,pointY,pointZ));		//����λ��
		missile->setRotation3D(Vec3(angleX,angleY,angleZ));		//���ýǶ�
		missile->setCameraMask((unsigned short)CameraFlag::USER1);
	}
	return missile;
}
void Missile::move()
{
	auto director = Director::getInstance();
	auto sched = director->getScheduler();
	missile->runAction(Sequence::create(
											Spawn::create(RotateBy::create(0.5,Vec3(0,0,direction*45)),
											MoveBy::create(0.5,Vec2(100.0*direction*radio,-100*radio)),nullptr),
											CallFunc::create(CC_CALLBACK_0(Missile::shootEnemyPlane,this)),
											nullptr
											));
}
void Missile::shootEnemyPlane()								//���Ŀ��
{
	//�������ϵͳ
	ParticleManager *pm=new ParticleManager(layer);
	pm->initFire(missile);
	//������ת
//	missile->runAction(RepeatForever::create(RotateBy::create(0.5f,Vec3(0,360,0))));
	//������ʱ�ص�������ʱ������ʱ��
	auto director = Director::getInstance();
	auto sched = director->getScheduler();
	sched->scheduleSelector(SEL_SCHEDULE(&Missile::updateShoot),this,0.01,false);
}
//׷��Ŀ��
void Missile::updateShoot()
{
	ControlGame *controlGame=dynamic_cast<First3DLayer*>(layer)->controlGame;
	Vector<Node*> cTemp = controlGame->getChildren();
	std::vector<Node*>::iterator result= cTemp.find(enemyPlane);
	if(!Constant::gameingFlag){			//���û�н�����Ϸ
		vanish();
	}else if(!canMove){
//		canMove=false;
////		CCLOG("-------------asdf");
//		this->unschedule(SEL_SCHEDULE(&Missile::updateShoot));
////		this->unschedule(SEL_SCHEDULE(&Missile::updateRotate));
//		layer->removeChild(missile);
//		delete this;
		vanish();
	}else if(result==cTemp.end()&&canMove){
//		getNewEnemyPlane();
		vanish();
	}
	else if(canMove&&result!=cTemp.end()){
			float enemyX=enemyPlane->getPositionX();
			float enemyY=enemyPlane->getPositionY();
			float missileX=missile->getPositionX();
			float missileY=missile->getPositionY();
			float missileZ=missile->getPositionZ();

			//��ȡ��������ת�Ƕ�
			float missileAngleZ=(missile->getRotation3D().z);	//��ȡZ�����ת�Ƕ�
			speed+=0.05*(Constant::missileLevel+1);				//���õ������ٶ�
			if(speed>=20){										//����ٶȲ��ܴ���16
				speed=20;
			}
			float rotateAngle=5.0-speed*0.3;					//����ÿ����ת�Ƕ�
			if(rotateAngle<=0.2){								//��С��ת�ǶȲ��ܵ���0.2
				rotateAngle=0.2;
			}
			if(missileZ<-10){									//�����-10,��ֹͣ����
				missileZ=missileZ+10;							//����Zֵ�Լ�
			}
			//���õ���������
			missile->setPosition3D(Vec3(missileX+speed*sin(CC_DEGREES_TO_RADIANS(missileAngleZ)),missileY+speed*cos(CC_DEGREES_TO_RADIANS(missileAngleZ)),missileZ));	//��������
			//��ȡ������Ŀ����ĽǶ�
			double temp=CC_RADIANS_TO_DEGREES(atan2(enemyY-missileY,enemyX-missileX));
			if(temp>90){			//�������90�ȣ���ı䷽��
				temp=450-temp;
			}else{
				temp=90-temp;
			}
//			CCLOG("-------------------4444");
			//�޸ĵ����Ƕ�
			float a=(float)((temp-missileAngleZ>0)?(temp-missileAngleZ):(360+missileAngleZ-temp));
			if(a>180){
				missileAngleZ=(missileAngleZ-rotateAngle>=0)?(missileAngleZ-rotateAngle):(missileAngleZ-rotateAngle+360);
			}else{
				missileAngleZ=(missileAngleZ+rotateAngle<360)?(missileAngleZ+rotateAngle):(missileAngleZ+rotateAngle-360);
			}
//			CCLOG("-------------------5555");
			//��ȡY����ת�ĽǶ�
			float angleY=missile->getRotation3D().y;
			//���õ����Ƕ�
			missile->setRotation3D(Vec3(0,angleY,missileAngleZ));
			//���õ�����С
			float radio=missile->getScale();
			if(radio<1){
				missile->setScale(radio+0.01);
			}
//			CCLOG("-------------------6666");
			//��ʱ
			timeCounter=timeCounter+0.01;
			if(timeCounter>=5){				//
				vanish();					//ע������
			}
			//��ײ���
			ControlGame *controlGame=dynamic_cast<First3DLayer*>(layer)->controlGame;
			Vector<Node*> cTemp = controlGame->getChildren();
			Vector<Node*>::iterator cIter=cTemp.begin();
			for(;cIter!=cTemp.end();cIter++)
			{
				EnemyPlane *eTemp = dynamic_cast<EnemyPlane*>(*cIter);
				if(eTemp&&canMove)
				{
					Point localVec = missile->getPosition();
					Point goalVec = eTemp->getPosition();			//�л���λ��
					if(!eTemp->isDie && Collision::check(localVec,goalVec,0,eTemp->index+1)&&canMove)
					{
						canMove=false;
						eTemp->hurt(50+Constant::missileLevel*25,goalVec);
						dynamic_cast<EnemyPlane*>(enemyPlane)->isGoal=false;
						enemyPlane=nullptr;
						break;
					}
				}
			}
	}
}
//������ʧ
void Missile::vanish()
{
	canMove=false;
	this->unschedule(SEL_SCHEDULE(&Missile::updateShoot));
	layer->removeChild(missile);
//	First3DLayer *f3Layer=dynamic_cast<First3DLayer*>(layer);
//	f3Layer->exManager->createBig(missile->getPosition3D(),0,0);
	delete this;
}
//��ȡ�µĵл�Ŀ��
void Missile::getNewEnemyPlane()
{
	enemyPlane=nullptr;
	ControlGame *controlGame=dynamic_cast<First3DLayer*>(layer)->controlGame;
	Vector<Node*> cTemp = controlGame->getChildren();
	Vector<Node*>::iterator cIter=cTemp.begin();
	for(;cIter!=cTemp.end();cIter++)
	{
		EnemyPlane *eTemp = dynamic_cast<EnemyPlane*>(*cIter);
		if(eTemp)
		{
			enemyPlane=eTemp;
			eTemp->setMissile(this);
			break;
		}
	}
	CCLOG("-----------getenemyPalne  222");
}

