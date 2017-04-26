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
		//方向 1--右,-1--左
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
		missile->setPosition3D(Vec3(pointX,pointY,pointZ));		//设置位置
		missile->setRotation3D(Vec3(angleX,angleY,angleZ));		//设置角度
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
void Missile::shootEnemyPlane()								//射击目标
{
	//添加粒子系统
	ParticleManager *pm=new ParticleManager(layer);
	pm->initFire(missile);
	//自身旋转
//	missile->runAction(RepeatForever::create(RotateBy::create(0.5f,Vec3(0,360,0))));
	//开启定时回调，倒计时车启动时间
	auto director = Director::getInstance();
	auto sched = director->getScheduler();
	sched->scheduleSelector(SEL_SCHEDULE(&Missile::updateShoot),this,0.01,false);
}
//追踪目标
void Missile::updateShoot()
{
	ControlGame *controlGame=dynamic_cast<First3DLayer*>(layer)->controlGame;
	Vector<Node*> cTemp = controlGame->getChildren();
	std::vector<Node*>::iterator result= cTemp.find(enemyPlane);
	if(!Constant::gameingFlag){			//如果没有进行游戏
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

			//获取导弹的旋转角度
			float missileAngleZ=(missile->getRotation3D().z);	//获取Z轴的旋转角度
			speed+=0.05*(Constant::missileLevel+1);				//设置导弹的速度
			if(speed>=20){										//最大速度不能大于16
				speed=20;
			}
			float rotateAngle=5.0-speed*0.3;					//设置每次旋转角度
			if(rotateAngle<=0.2){								//最小旋转角度不能低于0.2
				rotateAngle=0.2;
			}
			if(missileZ<-10){									//如果是-10,则停止上升
				missileZ=missileZ+10;							//导弹Z值自加
			}
			//设置导弹的坐标
			missile->setPosition3D(Vec3(missileX+speed*sin(CC_DEGREES_TO_RADIANS(missileAngleZ)),missileY+speed*cos(CC_DEGREES_TO_RADIANS(missileAngleZ)),missileZ));	//设置坐标
			//获取导弹和目标体的角度
			double temp=CC_RADIANS_TO_DEGREES(atan2(enemyY-missileY,enemyX-missileX));
			if(temp>90){			//如果大于90度，则改变方向
				temp=450-temp;
			}else{
				temp=90-temp;
			}
//			CCLOG("-------------------4444");
			//修改导弹角度
			float a=(float)((temp-missileAngleZ>0)?(temp-missileAngleZ):(360+missileAngleZ-temp));
			if(a>180){
				missileAngleZ=(missileAngleZ-rotateAngle>=0)?(missileAngleZ-rotateAngle):(missileAngleZ-rotateAngle+360);
			}else{
				missileAngleZ=(missileAngleZ+rotateAngle<360)?(missileAngleZ+rotateAngle):(missileAngleZ+rotateAngle-360);
			}
//			CCLOG("-------------------5555");
			//获取Y轴旋转的角度
			float angleY=missile->getRotation3D().y;
			//设置导弹角度
			missile->setRotation3D(Vec3(0,angleY,missileAngleZ));
			//设置导弹大小
			float radio=missile->getScale();
			if(radio<1){
				missile->setScale(radio+0.01);
			}
//			CCLOG("-------------------6666");
			//计时
			timeCounter=timeCounter+0.01;
			if(timeCounter>=5){				//
				vanish();					//注销导弹
			}
			//碰撞检测
			ControlGame *controlGame=dynamic_cast<First3DLayer*>(layer)->controlGame;
			Vector<Node*> cTemp = controlGame->getChildren();
			Vector<Node*>::iterator cIter=cTemp.begin();
			for(;cIter!=cTemp.end();cIter++)
			{
				EnemyPlane *eTemp = dynamic_cast<EnemyPlane*>(*cIter);
				if(eTemp&&canMove)
				{
					Point localVec = missile->getPosition();
					Point goalVec = eTemp->getPosition();			//敌机的位置
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
//导弹消失
void Missile::vanish()
{
	canMove=false;
	this->unschedule(SEL_SCHEDULE(&Missile::updateShoot));
	layer->removeChild(missile);
//	First3DLayer *f3Layer=dynamic_cast<First3DLayer*>(layer);
//	f3Layer->exManager->createBig(missile->getPosition3D(),0,0);
	delete this;
}
//获取新的敌机目标
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

