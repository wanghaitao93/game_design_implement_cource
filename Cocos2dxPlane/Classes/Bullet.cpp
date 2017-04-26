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
	//�����ӵ�����ڵ�
	ControlBullet *controlBullet = new ControlBullet();
	//�ж϶����Ƿ񴴽��ɹ�
	if (controlBullet)
	{
		controlBullet->plane=_plane;
		controlBullet->autorelease();
		return controlBullet;										//���صл�����
	}
	CC_SAFE_DELETE(controlBullet);								//����л�δ�ɹ���������ȫɾ��
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
			if(temp->num==1||temp->num==3||temp->num==4)			//�л����ӵ�
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
			else													//��ҵ��ӵ�
			{
				Vector<Node*> cTemp = controlGame->getChildren();
				Vector<Node*>::iterator cIter=cTemp.begin();
				for(;cIter!=cTemp.end();cIter++)
				{
					EnemyPlane *eTemp = dynamic_cast<EnemyPlane*>(*cIter);
					if(eTemp)
					{
						Point localVec = temp->getPosition();		//�ӵ�λ��
						Point goalVec = eTemp->getPosition();		//�л�
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
			if(!Rect(-100,-50,740,1600).containsPoint(temp->getPosition()))		//����ӵ��Ƿ����
			{
				temp->removeFromParent();
			}
		}
	}
}
/**
 * ��š�����λ�á���ת�Ƕȡ��ٶȡ��з�λ�á�����
 * 0--����ӵ���1--�з��ӵ���2--boss�ӵ�
 * ��ת�Ƕȱ�ʾ���ӵ���Ӧ��׼�ķ���
 * �ӵ����ٶȲ�һ��������ӵ��ٶ�Ϊ3.0���з��ӵ�2.0
 **/
Rect Bullet::rTemp[]={Rect(60,58,20,51),Rect(11,9,22,31),Rect(0,52,40,60),Rect(60,0,24,48),Rect(105,9,36,36)};	//���0���л�1�����2��boss,boss԰
int Bullet::iTemp[]={100,10,40,10,15};//���0���л�1�����2��boss,boss԰


Bullet* Bullet::create(int num,Point point1,float speed,Point point2,float angle)//Sprite *node,
{
	Bullet *bullet=new Bullet();
	if(bullet && bullet->initWithFile(GAME_PATH+std::string("bullets.png"),rTemp[num]))
	{
		if(num==1)
		{
			ParticleSystemQuad *psq = ParticleSystemQuad::create(PLIST_PATH+std::string("bullet.plist"));//���ļ��м�������ϵͳ
			psq->setPositionType(ParticleSystem::PositionType::RELATIVE);
			psq->setScale(1.5);
			bullet->addChild(psq, PLIST_LEVEL);
			psq->setPosition(Point(8,0));
		}
		bullet->setPosition(point1);
		bullet->startPoint = point1;						//��ȡ��ʼ����
		bullet->goalPoint = point2;							//��ȡĿ������
		bullet->bSpeed = speed/50;							//��ȡ�ӵ��ٶ�
		bullet->num = num;
		bullet->angle=angle;								//�Ƕ�
		bullet->damage = iTemp[num];
		bullet->setCameraMask((unsigned short)CameraFlag::USER1);
		bullet->setRotation(angle);
		bullet->autorelease();
		return bullet;
	}
	CC_SAFE_DELETE(bullet);									//����л�δ�ɹ���������ȫɾ��
	return nullptr;
}

Bullet* Bullet::webCreate(int num,Point point,float angle)
{
	Bullet *bullet=new Bullet();
	if(bullet && bullet->initWithFile(GAME_PATH+std::string("bullets.png"),rTemp[num]))
	{
		if(num==1)
		{
			ParticleSystemQuad *psq = ParticleSystemQuad::create(PLIST_PATH+std::string("bullet.plist"));//���ļ��м�������ϵͳ
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
	CC_SAFE_DELETE(bullet);									//����л�δ�ɹ���������ȫɾ��
	return nullptr;
}

//ʱʱ�����ӵ�
void Bullet::updatePoint()
{
	//������ӵ���ɻ�����Ŀ�ȡ��߶��Լ���̾���
	float disX=goalPoint.x-startPoint.x;
	float disY=goalPoint.y-startPoint.y;
	float distance=sqrt(disX*disX+disY*disY);				//������Ŀ���֮�����
	float dx=bSpeed*disX/distance;							//����x���򲽽�ֵ
	float dy=bSpeed*disY/distance;							//����y���򲽽�ֵ
	Point pTemp=this->getPosition()+Point(dx,dy);			//��ȡ��ǰ�ӵ�λ��
	this->setPosition(pTemp);
}

bool Collision::check(Vec2 local, Vec2 goal, int localN, int goalN)
{
	int localR[]={0,50,50,40};			//�ɻ�
	int goalR[]={0,30,40,50,60,100,100,100};		//�л�	boss���4,5,6
	if(local.getDistance(goal)<(localR[localN]+goalR[goalN]))
	{
		return true;
	}
	return false;
}
