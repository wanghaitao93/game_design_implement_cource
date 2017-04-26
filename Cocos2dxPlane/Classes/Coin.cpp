#include "Coin.h"
#include "AppMacros.h"
#include "First3DLayer.h"
#include "EnemyPlane.h"
#include "ControlGame.h"
#include "PlaySound.h"
#include "Explosion.h"
#include "Constant.h"
#include "Bullet.h"
#include <cmath>
#include "cocos2d.h"
using namespace cocos2d;

void Coin::createVec()
{
	coinSpriteVec=new std::vector<Sprite3D*>();
	delCoinSpriteVec=new std::vector<Sprite3D*>();
}
void Coin::createCoin(Vec3 vec,int index,int s)
{
	score=s;								//����
	std::string color="";
	int rand1=rand()%8;						//��ȡ�����
	int rand2=rand()%6;
	if(rand1==0){
		switch(rand2){						//�ɻ�����
			case 0:color="dj12.png";break;
			case 1:color="dj31.png";break;
			case 2:color="dj13.png";break;
			case 3:color="dj32.png";break;
			case 4:color="dj33.png";break;
			case 5:color="dj22.png";break;
		}
	}else if(rand1==1||rand1==2){
		switch(index){						//�ɻ�����
			case 0:color="coin_gray.png";break;
			case 1:color="coin_blue.png";break;
			case 2:case 3:case 4:case 5:case 6:color="coin_gold.png";break;
			}
	}
	Sprite3D *coin;

	if(rand1==0){
		coin=Sprite3D::create(C3B_PATH+std::string("update.obj"),C3B_PATH+color);	//��������
	}else if(rand1==1||rand1==2){
		coin=Sprite3D::create(C3B_PATH+std::string("coin.obj"),C3B_PATH+color);	//�������
	}

	if(rand1==0||rand1==1||rand1==2)
	{
		int awardNum=0;
		if(rand1==0){
			awardNum=rand2+1;								//�����ı��
		}else {
			awardNum=index+7;								//�����ı��
		}
		coin->setPosition3D(vec);
		coin->setRotation3D(Vec3(-90,0,0));
		coin->runAction(RepeatForever::create(RotateBy::create(2.0f,Vec3(0,180,0))));
		this->addChild(coin);
		if(awardNum>9){
			awardNum=9;
		}
		coin->setTag(awardNum);
		coinSpriteVec->push_back(coin);
		coin->setCameraMask((unsigned short)CameraFlag::USER1);		//���ñ�־��������
	}
}

//���½���Ƿ񱻳Ե�
void Coin::update()
{
	std::vector<Sprite3D*>::iterator tempCoin=coinSpriteVec->begin();
	while(tempCoin!=coinSpriteVec->end())
	{
		Point localVec = (*tempCoin)->getPosition();
		Point goalVec = Constant::planePoint;
		if(!Rect(-100,-50,740,1600).containsPoint(localVec)
				||localVec.getDistance(goalVec)<=30)
		{
			delCoinSpriteVec->push_back(*tempCoin);
			tempCoin=coinSpriteVec->erase(tempCoin);
			PlaySound::playSound(kGetCoin);
			Constant::score=Constant::score+score;						//���·���
			int awardNum=(*tempCoin)->getTag();
			if(awardNum!=0){
				addAward(awardNum);										//��ӽ���
				addLabel(localVec,awardNum);							//���������ʾ
			}
			continue;
		}else if(localVec.getDistance(goalVec)<=100)
		{
			(*tempCoin)->runAction(
									Sequence::create(
									MoveTo::create(0.05f,Point(goalVec)),
									nullptr
									));
		}
		else
		{
			(*tempCoin)->setPosition(Point(localVec.x,localVec.y-5));
		}
		tempCoin++;
	}
	//��������Ļ�ĺ�Ҷ
	std::vector<Sprite3D*>::iterator delleaf=delCoinSpriteVec->begin();
	while(delleaf!=delCoinSpriteVec->end())
	{
		this->removeChild((*delleaf),true);
		delleaf=delCoinSpriteVec->erase(delleaf);
	}
}
//��ӽ���
void Coin::addAward(int awardNum)
{
	switch(awardNum){
		case 1:
			Constant::bulletLevel++;				// �ӵ���
			if(Constant::bulletLevel==5){
				Constant::bulletLevel=4;
			}
			break;
		case 2:
			Constant::bulletPower++;				//�ӵ�����
			Constant::bulletPowerTime=1000;			//�ӵ�����ʱ��Ϊ1000
			Constant::isBulletPower=true;			//����ӵ�����
			if(Constant::bulletPower==5){
				Constant::bulletPower=4;
			}
			break;
		case 3:
			Constant::missileLevel++;				// ��������
			if(Constant::missileLevel==4){
				Constant::missileLevel=3;
			}
			break;
		case 4:
			Constant::attackCount++;				// ���и���
			if(Constant::attackCount==4){
				Constant::attackCount=3;
			}
			break;
		case 5:
			Constant::protectCount++;				// ��������
			if(Constant::protectCount==4){
				Constant::protectCount=3;
			}
			break;
		case 6:
			Constant::planeHealth+=40;				// ��������ֵ
			if(Constant::planeHealth>=200){
				Constant::planeHealth=200;
			}
			break;
	}
}
// ���������ʾ
void Coin::addLabel(Point point,int awardNum)
{
	Sprite *sprite=Sprite::create(StringUtils::format("game/coinAward%d.png",awardNum).c_str());
	sprite->setPosition(point+Point(0,100));
	sprite->setScale(0.1f);
	this->addChild(sprite);
	sprite->runAction(Sequence::create(
			ScaleTo::create(0.5,1.0f),
			CallFunc::create(CC_CALLBACK_0(Coin::deleteLabel,this,sprite)),
			nullptr
			));
}
void Coin::deleteLabel(Sprite *sprite)
{
	this->removeChild(sprite,true);
}
void Coin::deleteCoin(Sprite3D *coin)
{

	PlaySound::playSound(kGetCoin);
	this->removeChild(coin,true);
}

Award* Award::webCreate(Point point,int num)
{
	std::string tex="c3b/";
	std::string obj="";
	switch(num)								//��������
	{
		case 1:tex+="dj12.png";break;
		case 2:tex+="dj31.png";break;
		case 3:tex+="dj13.png";break;
		case 4:tex+="dj32.png";break;
		case 5:tex+="dj33.png";break;
		case 6:tex+="dj22.png";break;
		case 7:tex+="coin_gray.png";break;
		case 8:tex+="coin_blue.png";break;
		case 9:tex+="coin_gold.png";break;
	}
	if(num<7)
	{
		obj=std::string("c3b/update.obj");
	}else
	{
		obj=std::string("c3b/coin.obj");
	}

	auto award = new Award();
	if(award && award->initWithFile(obj))
	{
		award->_contentSize = award->getBoundingBox().size;
		award->setTexture(tex);
		award->autorelease();
		award->setPosition(point);
		award->setRotation3D(Vec3(-90,0,0));
		award->runAction(RepeatForever::create(RotateBy::create(2.0f,Vec3(0,180,0))));
		award->setCameraMask((unsigned short)CameraFlag::USER1);		//���ñ�־��������
		return award;
	}
	CC_SAFE_DELETE(award);
	return nullptr;
}
