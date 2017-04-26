#include "Explosion.h"
#include "PlaySound.h"
#include "AppMacros.h"
#include "First3DLayer.h"
#include "GameData.h"

bool ExManager::init()
{
	if(!Node::init())
	{
		return false;
	}
	//da爆炸特效
	Vector<SpriteFrame*> animFrames;
	Animation *anim;

	for(int i=0;i<9;i++)
	{
		SpriteFrame *frame=SpriteFrame::create(
				BOOMB_PATH+StringUtils::format("boomB (%d).png",(i+1)).c_str(),Rect(0,0,128,128));
		animFrames.pushBack(frame);
	}
	anim=Animation::createWithSpriteFrames(animFrames,0.03125f);
	animateBig=Animate::create(anim);
	animateBig->retain();

	//小爆炸特效
	Vector<SpriteFrame*> animFrames2;
	Animation *anim2;

	for(int i=0;i<4;i++)
	{
		SpriteFrame *frame=SpriteFrame::create(
				BOOMS_PATH+StringUtils::format("boomSS (%d).png",(i+1)).c_str(),Rect(0,0,60,57));
		animFrames2.pushBack(frame);
	}
	anim2=Animation::createWithSpriteFrames(animFrames2,0.05f);
	animateSmall=Animate::create(anim2);
	animateSmall->retain();

	//飞机爆炸特效
	Vector<SpriteFrame*> animFrames3;
	Animation *anim3;

	for(int i=0;i<7;i++)
	{
		SpriteFrame *frame=SpriteFrame::create(
				BOOMP_PATH+StringUtils::format("boomP (%d).png",(i+1)).c_str(),Rect(0,0,250,266));
		animFrames3.pushBack(frame);
	}
	anim3=Animation::createWithSpriteFrames(animFrames3,0.025f);
	animatePlane=Animate::create(anim3);
	animatePlane->retain();

	return true;
}

void ExManager::createBig(Vec3 vec,int index,int score)
{
	Explosion* temp=Explosion::create(							//创建动画帧
		BOOMB_PATH+std::string("boomB (1).png"),
		Rect(0,0,128,128)
	);															//创建用来执行动画的精灵
	temp->setPosition3D(vec);									//计算并设置爆炸位置
	temp->setCameraMask((unsigned short)CameraFlag::USER1);		//设置标志板的摄像机
	this->addChild(temp);										//获取屏幕节点
	temp->startAnim(animateBig);
	if(GameData::playerNumber==-1)
	{
		createNewCoin(vec,index,score);
	}
	PlaySound::playSound(kBoomB);										//播放大爆炸
}

void ExManager::createSmall(Vec3 vec)
{
	Explosion* temp=Explosion::create(							//创建动画帧
		BOOMS_PATH+"boomSS (1).png",
		Rect(0,0,60,57)
	);															//创建用来执行动画的精灵
	temp->setPosition3D(vec);									//计算并设置爆炸位置
//	temp->setScale(0.5);										//设置爆炸大小
	temp->setCameraMask((unsigned short)CameraFlag::USER1);		//设置标志板的摄像机
	this->addChild(temp);										//获取屏幕节点
	temp->startAnim(animateSmall);
	PlaySound::playSound(kBoomS);										//播放小爆炸
}
void ExManager::createPlane(Vec3 vec)
{
	Explosion* temp=Explosion::create(							//创建动画帧
		BOOMP_PATH+std::string("boomP (1).png"),
		Rect(0,0,250,266)
	);
	temp->setPosition3D(vec+Vec3(0,0,0));						//计算并设置爆炸位置
	temp->setScale(0.5);										//设置爆炸大小
	temp->setCameraMask((unsigned short)CameraFlag::USER1);		//设置标志板的摄像机
	this->addChild(temp);
	temp->startAnim(animatePlane);
	PlaySound::playSound(kBoomS);								//播放小爆炸
}

//创建金币
void ExManager::createNewCoin(Vec3 vec,int index,int score)
{
	First3DLayer *f3Layer=dynamic_cast<First3DLayer*>(this->getParent());
	CCLOG("-------------------Point.x=%f,Point.y=%f",vec.x,vec.y);
	f3Layer->coin->createCoin(vec,index,score);
}

Explosion* Explosion::create(const std::string& filename, const Rect& rect)
{
	Explosion *explosion = new Explosion();						//创建新的爆炸
    if (explosion && explosion->initWithFile(filename, rect))	//初始化爆炸
    {
    	explosion->autorelease();
        return explosion;
    }
    CC_SAFE_DELETE(explosion);
    return nullptr;
}

void Explosion::startAnim(Animate *animate)						//执行动画的方法
{
	this->runAction(Sequence::create(animate->clone(),			//执行顺序动画
			CallFunc::create(CC_CALLBACK_0(Explosion::deleteMe,this)),
			nullptr));
}

void Explosion::deleteMe()
{
	this->removeFromParentAndCleanup(true);						//移除爆炸
}
