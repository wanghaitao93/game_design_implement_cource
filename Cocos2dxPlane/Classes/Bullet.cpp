#include "Bullet.h"
#include "AppMacros.h"
#include "First3DLayer.h"
#include "EnemyPlane.h"
#include "ControlGame.h"
#include "PlaySound.h"
#include "Explosion.h"
#include "Constant.h"
#include <cmath>
#include "cocos2d.h"
using namespace cocos2d;

ControlBullet* ControlBullet::create(NewPlane *_plane)
{
	//创建子弹管理节点
	ControlBullet *controlBullet = new ControlBullet();
	//判断对象是否创建成功
	if (controlBullet)
	{
		controlBullet->plane=_plane;
		controlBullet->autorelease();
		return controlBullet;										//返回敌机对象
	}
	CC_SAFE_DELETE(controlBullet);								//如果敌机未成功创建，则安全删除
	return nullptr;
}

void ControlBullet::update()
{
	Vector<Node*> vTemp = this->getChildren();
	Vector<Node*>::iterator iter=vTemp.begin();
	ControlGame *controlGame=dynamic_cast<First3DLayer*>(this->getParent())->controlGame;
	for(;iter!=vTemp.end();iter++)
	{
		Bullet *temp = dynamic_cast<Bullet*>(*iter);
		if(temp)
		{
			temp->updatePoint();
			if(temp->num==1||temp->num==3||temp->num==4)			//敌机的子弹
			{
				Point localVec = temp->getPosition();
				Point goalVec = plane->plane->getPosition();
				if(Collision::check(localVec,goalVec,Constant::planeId+1,0))
				{
					plane->hurt(temp->damage);
					First3DLayer *f3Layer=dynamic_cast<First3DLayer*>(this->getParent());
					f3Layer->exManager->createPlane(temp->getPosition3D());
					CCLOG("bullet->plane");
					temp->removeFromParent();
				}
			}
			else													//玩家的子弹
			{
				Vector<Node*> cTemp = controlGame->getChildren();
				Vector<Node*>::iterator cIter=cTemp.begin();
				for(;cIter!=cTemp.end();cIter++)
				{
					EnemyPlane *eTemp = dynamic_cast<EnemyPlane*>(*cIter);
					if(eTemp)
					{
						Point localVec = temp->getPosition();		//子弹位置
						Point goalVec = eTemp->getPosition();		//敌机
						if(!eTemp->isDie && Collision::check(localVec,goalVec,0,eTemp->index+1))
						{
							if(eTemp->hurt(temp->damage,localVec))
							{
								eTemp->isDie=true;
							}
							temp->removeFromParent();
						}
					}
				}
			}
			if(!Rect(-100,-50,740,1600).containsPoint(temp->getPosition()))		//检测子弹是否出界
			{
				temp->removeFromParent();
			}
		}
	}
}
/**
 * 编号、出现位置、旋转角度、速度、敌方位置、场景
 * 0--玩家子弹，1--敌方子弹，2--boss子弹
 * 旋转角度表示其子弹本应对准的方向
 * 子弹的速度不一样，玩家子弹速度为3.0，敌方子弹2.0
 **/
Rect Bullet::rTemp[]={Rect(60,58,20,51),Rect(11,9,22,31),Rect(0,52,40,60),Rect(60,0,24,48),Rect(105,9,36,36)};	//玩家0，敌机1，玩家2，boss,boss园
int Bullet::iTemp[]={100,10,40,10,15};//玩家0，敌机1，玩家2，boss,boss园


Bullet* Bullet::create(int num,Point point1,float speed,Point point2,float angle)//Sprite *node,
{
	Bullet *bullet=new Bullet();
	if(bullet && bullet->initWithFile(GAME_PATH+std::string("bullets.png"),rTemp[num]))
	{
		if(num==1)
		{
			ParticleSystemQuad *psq = ParticleSystemQuad::create(PLIST_PATH+std::string("bullet.plist"));//从文件中加载粒子系统
			psq->setPositionType(ParticleSystem::PositionType::RELATIVE);
			psq->setScale(1.5);
			bullet->addChild(psq, PLIST_LEVEL);
			psq->setPosition(Point(8,0));
		}
		bullet->setPosition(point1);
		bullet->startPoint = point1;						//获取起始坐标
		bullet->goalPoint = point2;							//获取目标坐标
		bullet->bSpeed = speed/50;							//获取子弹速度
		bullet->num = num;
		bullet->angle=angle;								//角度
		bullet->damage = iTemp[num];
		bullet->setCameraMask((unsigned short)CameraFlag::USER1);
		bullet->setRotation(angle);
		bullet->autorelease();
		return bullet;
	}
	CC_SAFE_DELETE(bullet);									//如果敌机未成功创建，则安全删除
	return nullptr;
}

Bullet* Bullet::webCreate(int num,Point point,float angle)
{
	Bullet *bullet=new Bullet();
	if(bullet && bullet->initWithFile(GAME_PATH+std::string("bullets.png"),rTemp[num]))
	{
		if(num==1)
		{
			ParticleSystemQuad *psq = ParticleSystemQuad::create(PLIST_PATH+std::string("bullet.plist"));//从文件中加载粒子系统
			psq->setPositionType(ParticleSystem::PositionType::RELATIVE);
			psq->setScale(1.5);
			bullet->addChild(psq, PLIST_LEVEL);
			psq->setPosition(Point(8,0));
		}
		bullet->setPosition(point);
		bullet->setRotation(angle);
		bullet->setCameraMask((unsigned short)CameraFlag::USER1);
		bullet->autorelease();
		return bullet;
	}
	CC_SAFE_DELETE(bullet);									//如果敌机未成功创建，则安全删除
	return nullptr;
}

//时时更新子弹
void Bullet::updatePoint()
{
	//计算出子弹与飞机距离的宽度、高度以及最短距离
	float disX=goalPoint.x-startPoint.x;
	float disY=goalPoint.y-startPoint.y;
	float distance=sqrt(disX*disX+disY*disY);				//计算与目标点之间距离
	float dx=bSpeed*disX/distance;							//计算x方向步进值
	float dy=bSpeed*disY/distance;							//计算y方向步进值
	Point pTemp=this->getPosition()+Point(dx,dy);			//获取当前子弹位置
	this->setPosition(pTemp);
}

bool Collision::check(Vec2 local, Vec2 goal, int localN, int goalN)
{
	int localR[]={0,50,50,40};			//飞机
	int goalR[]={0,30,40,50,60,100,100,100};		//敌机	boss编号4,5,6
	if(local.getDistance(goal)<(localR[localN]+goalR[goalN]))
	{
		return true;
	}
	return false;
}
