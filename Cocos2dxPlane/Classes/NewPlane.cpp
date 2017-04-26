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
	//新建飞机
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
	plane->setCameraMask((unsigned short)CameraFlag::USER1);		//添加摄像机
	canMove();														//飞机可以移动
}
//开始后空翻
void NewPlane::rotatePlane()
{
	auto director = Director::getInstance();
	auto sched = director->getScheduler();
	sched->scheduleSelector(SEL_SCHEDULE(&NewPlane::updateBackflip),this,0.01,false);
}
//飞机的move监听的移动和飞机角度的改变
void NewPlane::movePlane()
{
	Vec3 angle=plane->getRotation3D();
	float angleX=angle.x;
	float angleY=angle.y;

	angleX=angleX-deltaPoint.y*rotateSpeed/2*(1-fabs(angleX)/maxRotateX);	//前后
	if(angleX>0){
		angleX=0;
	}
	angleY=angleY+deltaPoint.x*rotateSpeed*(1-fabs(angleY)/maxRotateY);		//左右

	plane->setRotation3D(Vec3(angleX,angleY,0));
	plane->setPosition(Vec2(animPoint));
	Constant::planePoint=animPoint;
	action=true;
}
//飞机可以移动
void NewPlane::canMove()
{
	action=true;
}
//began的飞机移动
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
	//旋转角度
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
//began的飞机旋转
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
//时时更新飞机的角度
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

//飞机进行后空翻
void NewPlane::updateBackflip()
{
	float angleX=plane->getRotation3D().x;		//获取X轴偏转角度
	float anlgeY=plane->getRotation3D().y;		//获取Y轴偏转角度

	angleX=angleX+9;							//更改X轴偏转角度

	if(angleX>=360){
		plane->setRotation3D(Vec3(0,angleY,0));
		this->unschedule(SEL_SCHEDULE(&NewPlane::updateBackflip));
		action=true;
	}else{
		plane->setRotation3D(Vec3(angleX,anlgeY,0));
	}
}
//返回正常角度
void NewPlane::planeReturn()
{
	plane->setRotation3D(Vec3(0,0,0));
	auto director = Director::getInstance();
	auto sched = director->getScheduler();
	sched->scheduleSelector(SEL_SCHEDULE(&NewPlane::updateRotate),this,0.002,false);
}
//飞机不停的恢复旋转角度
void NewPlane::updateRotate()
{
	float angleX=plane->getRotation3D().x;	//获取飞机X轴旋转角度
	float angleY=plane->getRotation3D().y;	//获取飞机Y轴旋转角度

	if(angleX>=1){							//判断X轴的角度
		angleX=angleX-0.3;
	}else if(angleX<=-1){
		angleX=angleX+0.3;
	}else{
		angleX=0;
	}
	if(angleY>=1){							//判断Y轴的角度
		angleY=angleY-0.5;
	}else if(angleY<=-1){
		angleY=angleY+0.5;
	}else{
		angleY=0;
	}
	plane->setRotation3D(Vec3(angleX,angleY,0));	//设置飞机的旋转角度
}
void NewPlane::movePlane2(Point delta)
{
	//获取玩家飞机的位置
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
			//拖尾效果===============
			//参一为消隐动画时长，参二为拖尾条带相邻顶点间的最小距离,参三为拖尾条带的宽度,参四为顶点颜色值，参五为所使用的纹理
			myStreak = MotionStreak::create(0.15, 1, 15, Color3B(255,255,200), GAME_PATH+"streak.png");//MotionStreak::create(0.4, 1, 17, Color3B(255,255,255), pics_PATH+"streak.png")
			myStreak->setBlendFunc(BlendFunc::ADDITIVE);//设置混合
			layer->addChild(myStreak,20);//将myStreak添加到布景中
			//设置此布景层渲染用摄像机
			myStreak->setCameraMask((unsigned short)CameraFlag::USER1);
			tail=false;
		}
		if(myStreak){								//如果有拖尾效果
			myStreak->setPosition(Point(animPoint.x,animPoint.y-80));
		}
		if((delta.x)>0){
			directionLeft=1;					//右飞
		}else{
			directionLeft=-1;					//左飞
		}
		if(delta.y>0){							//向前倾斜  	90-
			directionFront=-1;
		}else if((delta.y)>-50){				//向后倾斜  90+
			directionFront=1;
		}else{
			directionFront=0;					//后空翻
		}

		if(directionFront==0){
			if(action){
				rotatePlane();
		}}
		movePlane();
	}
	if(myStreak&&Constant::touchFlag&&delta.x==0&&delta.y==0)
	{
		layer->removeChild(myStreak,true);//移除myStreak
		tail=true;							//还可以继续创建拖尾
	}
}
void NewPlane::hurt(int damage)
{
	bool isCanHurt=true;
//	if(GameData::playerNumber==-1){					// 单机版
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
					m2l->pauseGame();			//暂停游戏
					m2l->completeGame(loseEnum);//游戏完成
			}}
		}else{											//联网模式
			int health;
			if(GameData::playerNumber==0){		//玩家
				health=GameData::redPlaneHealth;
			}else{								//房主
				health=GameData::greenPlaneHealth;
			}
			int remainHealth=health-damage;
			if(remainHealth>0){
				if(GameData::playerNumber==0){		//玩家
					GameData::redPlaneHealth=remainHealth;
				}else{								//房主
					GameData::greenPlaneHealth=remainHealth;
				}
				Web2DLayer *m2l=dynamic_cast<Web2DLayer*>(layer->getScene()->getChildByTag(0));
				if(m2l){
						m2l->sharkInjuredBG();
				}
			}else{
				Web2DLayer *m2l=dynamic_cast<Web2DLayer*>(layer->getScene()->getChildByTag(0));
				if(m2l){
					m2l->pauseGame();			//暂停游戏
					m2l->completeGame(loseEnum);//游戏完成
			}}
		}

	}
}
