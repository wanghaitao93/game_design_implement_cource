#include "ControlGame.h"
#include "EnemyPlane.h"
#include "Enemies.h"
#include "Constant.h"
#include "PlaySound.h"
#include "AppMacros.h"
#include "My2DLayer.h"

ControlGame* ControlGame::create(NewPlane *_plane)
{
	//创建敌机管理节点
	ControlGame *controlGame = new ControlGame();
	//判断对象是否创建成功
	if (controlGame)
	{
		controlGame->plane=_plane;
		controlGame->autorelease();
		return controlGame;										//返回敌机对象
	}
	CC_SAFE_DELETE(controlGame);								//如果敌机未成功创建，则安全删除
	return nullptr;
}

void ControlGame::createFirst1()
{
	int iTemp=rand()%3;
	Vec2 vec=Vec2(200+70*iTemp,1400);
	EnemyPlane *temp1=FirstEnemy::create();
	temp1->setPosition(vec);
	temp1->setScale(1.2);
	this->addChild(temp1);
	EnemyPlane *temp2=FirstEnemy::create();
	temp2->setPosition(vec+Vec2(80,40));
	this->addChild(temp2);
	EnemyPlane *temp3=FirstEnemy::create();
	temp3->setPosition(vec+Vec2(-80,40));
	this->addChild(temp3);
}

void ControlGame::createFirst2()
{
	Vec2 vec=Vec2(0,1200);
	EnemyPlane *temp1=FirstEnemy::create();
	temp1->setPosition(vec);
	temp1->setRotation(-45);
	temp1->setScale(1.2);
	this->addChild(temp1);
	EnemyPlane *temp2=FirstEnemy::create();
	temp2->setPosition(vec+Vec2(30,90));
	temp2->setRotation(-45);
	this->addChild(temp2);
	EnemyPlane *temp3=FirstEnemy::create();
	temp3->setPosition(vec+Vec2(-90,-30));
	temp3->setRotation(-45);
	this->addChild(temp3);
}

void ControlGame::createFirst3()
{
	Vec2 vec=Vec2(540,1200);
	EnemyPlane *temp1=FirstEnemy::create();
	temp1->setPosition(vec);
	temp1->setRotation(45);
	temp1->setScale(1.2);
	this->addChild(temp1);
	EnemyPlane *temp2=FirstEnemy::create();
	temp2->setPosition(vec+Vec2(90,-30));
	temp2->setRotation(45);
	this->addChild(temp2);
	EnemyPlane *temp3=FirstEnemy::create();
	temp3->setPosition(vec+Vec2(-30,90));
	temp3->setRotation(45);
	this->addChild(temp3);
}

void ControlGame::createSecond()
{
	int iTemp1=rand()%2;
	int iTemp2=rand()%20;
	Vec2 vec=Vec2(540*iTemp1,600+30*iTemp2);
	EnemyPlane *temp1;
	if(iTemp1==0)
	{
		temp1=SecondEnemy::createRight();
	}
	else
	{
		temp1=SecondEnemy::createLeft();
	}
	temp1->setPosition(vec);
	this->addChild(temp1);
}

void ControlGame::createThird()
{
	int iTemp1=rand()%20;
	int iTemp2=rand()%10;
	Vec2 vec=Vec2(20+54*iTemp2,600+30*iTemp1);
	EnemyPlane *temp1=ThirdEnemy::create(plane->plane);
	temp1->setPosition(vec);
	this->addChild(temp1);
}

void ControlGame::createFourth()
{
	int iTemp;
	Vec2 vec;
	iTemp=rand()%12;
	vec=Vec2(100+30*iTemp,1400);
	EnemyPlane *temp1=FourthEnemy::create();
	temp1->setPosition(vec);
	this->addChild(temp1);
}

void ControlGame::completeGame()
{
	My2DLayer *m2l=dynamic_cast<My2DLayer*>(plane->plane->getScene()->getChildByTag(0));
	if(m2l)
	{
		m2l->completeGame(winEnum);
	}
}

void ControlGame::update()
{
	timeCounter++;
	if(timeCounter>=3000+Constant::custom*1000)
	{
		if(!createBoss && childCount==0)
		{
			//警告的背景图片
			Sprite *warningBG=Sprite::create(GAME_PATH+std::string("warning0.png"));
			warningBG->setPosition(270,700);
			this->addChild(warningBG,MENU_LEVEL);
			warningBG->runAction(Sequence::create(FadeOut::create(0.5f),FadeIn::create(0.5f),
								FadeOut::create(0.5f),FadeIn::create(0.5f),
								RemoveSelf::create(),
								CallFunc::create(CC_CALLBACK_0(ControlGame::createBossNow,this)),
								nullptr
									));

			//警告的感叹号
			Sprite *warningMask=Sprite::create(GAME_PATH+std::string("warning1.png"));
			warningMask->setPosition(268,680);
			this->addChild(warningMask,MENU_LEVEL);
			warningMask->runAction(Sequence::create(Blink::create(2,2),RemoveSelf::create(),nullptr));

			PlaySound::playSound(kWarning);
			createBoss=true;
		}else if(createBoss && childCount==0)
		{
			if(isWin)
			{
				return;
			}
			isWin = true;
			int direction=1;						//飞机飞行的方向,默认左面
			float step=abs(1000-Constant::planePoint.y)/600.0;	//步骤
			if(Constant::planePoint.x>=270){		//如果在右面
				direction=-1;
			}
			ActionInterval *planeRotate1=RotateTo::create(0.5*step,Vec3(0,direction*60,-90));
			ActionInterval *planeRotate2=RotateTo::create(1.0f,Vec3(0,0,-90));
			ActionInterval *planeMove1=MoveTo::create(0.5*step,Vec2(270,200));
			ActionInterval *planeMove2=MoveTo::create(1.0f,Vec2(270,1600));
			ActionInterval *planeCome=Spawn::create(planeRotate1,planeMove1,nullptr);
			ActionInterval *planeGo=Spawn::create(planeRotate2,planeMove2,nullptr);
			My2DLayer *m2l=dynamic_cast<My2DLayer*>(plane->plane->getScene()->getChildByTag(0));
			if(m2l)
			{
				m2l->pauseGame();
			}
			plane->plane->runAction(Sequence::create(planeCome,planeGo,
					CallFunc::create(CC_CALLBACK_0(ControlGame::completeGame,this)),nullptr));
		}
	}else if(timeCounter%((100-Constant::custom*8)-timeCounter/(100-Constant::custom*8))==0 && timeCounter<3000+Constant::custom*1000)
	{
		int iTemp=rand()%16;
		createSecond();
		if(iTemp==0)
		{
			createFirst1();
		}else if(iTemp==1)
		{
			createFirst2();
		}else if(iTemp==2)
		{
			createFirst3();
		}else if(iTemp>=3 && iTemp<6)
		{
			createSecond();
		}else if(iTemp>=7 && iTemp<=9)
		{
			createThird();
		}else if(iTemp>=10 && iTemp<=15)
		{
			createFourth();
		}
	}

	Vector<Node*> vTemp = this->getChildren();
	Vector<Node*>::iterator iter=vTemp.begin();
	childCount=vTemp.size();
	for(;iter!=vTemp.end();iter++)
	{
		EnemyPlane *temp = dynamic_cast<EnemyPlane*>(*iter);
		if(temp)
		{
			temp->move();
			Point localVec = temp->getPosition();
			Point goalVec=plane->plane->getPosition();
			if(!temp->isDie && Collision::check(localVec,goalVec,Constant::planeId,temp->index+1))
			{
				if(temp->index>=4){
					plane->hurt(10000000);
				}else{
					plane->hurt(temp->hp/5);
				}
				temp->hurt(1000,goalVec);
				temp->isDie=true;
			}else if(!Rect(-100,-50,740,1600).containsPoint(localVec))
			{
				temp->hurt(10000000,Point(0,0));
			}
		}
	}
}
void ControlGame::createBossNow()
{
	EnemyPlane *temp1=Boss::create();
	temp1->setPosition3D(Vec3(270,1200,0));
	this->addChild(temp1,0,0);
}
