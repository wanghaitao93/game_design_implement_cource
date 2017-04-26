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
	//da��ը��Ч
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

	//С��ը��Ч
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

	//�ɻ���ը��Ч
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
	Explosion* temp=Explosion::create(							//��������֡
		BOOMB_PATH+std::string("boomB (1).png"),
		Rect(0,0,128,128)
	);															//��������ִ�ж����ľ���
	temp->setPosition3D(vec);									//���㲢���ñ�ըλ��
	temp->setCameraMask((unsigned short)CameraFlag::USER1);		//���ñ�־��������
	this->addChild(temp);										//��ȡ��Ļ�ڵ�
	temp->startAnim(animateBig);
	if(GameData::playerNumber==-1)
	{
		createNewCoin(vec,index,score);
	}
	PlaySound::playSound(kBoomB);										//���Ŵ�ը
}

void ExManager::createSmall(Vec3 vec)
{
	Explosion* temp=Explosion::create(							//��������֡
		BOOMS_PATH+"boomSS (1).png",
		Rect(0,0,60,57)
	);															//��������ִ�ж����ľ���
	temp->setPosition3D(vec);									//���㲢���ñ�ըλ��
//	temp->setScale(0.5);										//���ñ�ը��С
	temp->setCameraMask((unsigned short)CameraFlag::USER1);		//���ñ�־��������
	this->addChild(temp);										//��ȡ��Ļ�ڵ�
	temp->startAnim(animateSmall);
	PlaySound::playSound(kBoomS);										//����С��ը
}
void ExManager::createPlane(Vec3 vec)
{
	Explosion* temp=Explosion::create(							//��������֡
		BOOMP_PATH+std::string("boomP (1).png"),
		Rect(0,0,250,266)
	);
	temp->setPosition3D(vec+Vec3(0,0,0));						//���㲢���ñ�ըλ��
	temp->setScale(0.5);										//���ñ�ը��С
	temp->setCameraMask((unsigned short)CameraFlag::USER1);		//���ñ�־��������
	this->addChild(temp);
	temp->startAnim(animatePlane);
	PlaySound::playSound(kBoomS);								//����С��ը
}

//�������
void ExManager::createNewCoin(Vec3 vec,int index,int score)
{
	First3DLayer *f3Layer=dynamic_cast<First3DLayer*>(this->getParent());
	CCLOG("-------------------Point.x=%f,Point.y=%f",vec.x,vec.y);
	f3Layer->coin->createCoin(vec,index,score);
}

Explosion* Explosion::create(const std::string& filename, const Rect& rect)
{
	Explosion *explosion = new Explosion();						//�����µı�ը
    if (explosion && explosion->initWithFile(filename, rect))	//��ʼ����ը
    {
    	explosion->autorelease();
        return explosion;
    }
    CC_SAFE_DELETE(explosion);
    return nullptr;
}

void Explosion::startAnim(Animate *animate)						//ִ�ж����ķ���
{
	this->runAction(Sequence::create(animate->clone(),			//ִ��˳�򶯻�
			CallFunc::create(CC_CALLBACK_0(Explosion::deleteMe,this)),
			nullptr));
}

void Explosion::deleteMe()
{
	this->removeFromParentAndCleanup(true);						//�Ƴ���ը
}
