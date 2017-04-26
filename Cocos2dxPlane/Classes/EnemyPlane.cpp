#include "EnemyPlane.h"
#include "Constant.h"
#include "GameData.h"
#include "First3DLayer.h"
#include "Web3DLayer.h"
#include "AppMacros.h"

bool EnemyPlane::hurt(int damage,Point point)
{
	hp-=damage;
	if(hp<=0)
	{
		if(isGoal)
		{
			missile->canMove=false;
			isGoal=false;
		}
		die();
		return true;
	}
	if(point!=Point(0,0))
	{
		First3DLayer *f3Layer=dynamic_cast<First3DLayer*>(this->getParent()->getParent());
		f3Layer->exManager->createSmall(Vec3(point.x,point.y,0));
	}

	return false;
}
void EnemyPlane::setMissile(Missile *m)
{
	isGoal=true;
	missile=m;
}

void EnemyPlane::die()
{
	if(GameData::playerNumber!=-1)
	{
		this->removeAllChildren();
		Web3DLayer *layer=dynamic_cast<Web3DLayer*>(this->getParent());
		layer->exManager->createBig(this->getPosition3D(),0,0);
		this->removeFromParent();
		return;
	}
	this->removeAllChildren();									//移除飞机
	First3DLayer *f3Layer=dynamic_cast<First3DLayer*>(this->getParent()->getParent());
	f3Layer->exManager->createBig(this->getPosition3D(),index,score);
	this->removeFromParent();
}

void EnemyPlane::setLayer(Layer *layer)
{
	gameLayer=layer;
}

bool EnemyPlane::shoot()
{
	if(!isShoot)
	{
		return false;
	}else
	{
		shootCounter=0;
		isShoot=false;
		return true;
	}
}

bool EnemyPlane::move()
{
	timeCounter++;									//更新时间计数器
	shootCounter++;									//更新子弹计数器
	if(shootCounter==50)							//判断是否发送子弹的间隔
	{
		isShoot=true;
	}
	if(isDie)										//判断是否死亡
	{
		return false;
	}
	return true;
}
