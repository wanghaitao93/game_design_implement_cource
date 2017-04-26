#include "Enemies.h"
#include "GameData.h"
#include "AppMacros.h"
#include "PublicApi.h"
#include "Bullet.h"
#include "ControlGame.h"
#include "First3DLayer.h"
#include "Web3DLayer.h"
#include "Constant.h"
#include <math.h>

using namespace std;
FirstEnemy::FirstEnemy(){}
FirstEnemy::~FirstEnemy(){}

vector<Sprite3D*> FirstEnemy::firstEnemyVec;
vector<Sprite3D*> SecondEnemy::secondEnemyVec;
vector<Sprite3D*> ThirdEnemy::thirdEnemyVec;
FirstEnemy* FirstEnemy::create()
{
	//����һ���л�����
	FirstEnemy *firstEnemy = new FirstEnemy();
	//�ж϶����Ƿ񴴽��ɹ�
	if (firstEnemy)
	{
		firstEnemy->plane =	Sprite3D::create(
				std::string("c3b/enemy1.obj"),
				std::string("c3b/enemy.png"));
		firstEnemy->plane->setRotation3D(Vec3(0,0,0));			//�ڷŷɻ���̬
		firstEnemy->addChild(firstEnemy->plane);				//���ɻ�����л��ڵ�
		firstEnemy->hp=50+Constant::enemyBulletSpeed*50;										//���õл�����ֵ����
		firstEnemy->score=20;									//���õл���������
		firstEnemy->step=5;										//���õл��н��ٶ�
		firstEnemy->setCameraMask((unsigned short)CameraFlag::USER1);
		firstEnemy->autorelease();
		firstEnemy->index=0;
		return firstEnemy;										//���صл�����
	}
	CC_SAFE_DELETE(firstEnemy);									//����л�δ�ɹ���������ȫɾ��
	return nullptr;												//���ؿ�ָ��
}

FirstEnemy* FirstEnemy::webCreate()
{
	//����һ���л�����
	FirstEnemy *firstEnemy = new FirstEnemy();
	//�ж϶����Ƿ񴴽��ɹ�
	if (firstEnemy)
	{
		firstEnemy->plane =	Sprite3D::create(
				std::string("c3b/enemy1.obj"),
				std::string("c3b/enemy.png"));
		firstEnemy->plane->setRotation3D(Vec3(0,0,0));			//�ڷŷɻ���̬
		firstEnemy->addChild(firstEnemy->plane);				//���ɻ�����л��ڵ�
		firstEnemy->setCameraMask((unsigned short)CameraFlag::USER1);
		firstEnemy->autorelease();
		return firstEnemy;										//���صл�����
	}
	CC_SAFE_DELETE(firstEnemy);									//����л�δ�ɹ���������ȫɾ��
	return nullptr;												//���ؿ�ָ��
}

void FirstEnemy::die()
{
	if(GameData::playerNumber==-1)
	{
		dieVec=this->getPosition3D();
	}
	EnemyPlane::die();											//���ø�������������
}

bool FirstEnemy::move()
{
	if(!EnemyPlane::move())
	{
		return false;
	}
	float rotationRadians;										//������ر���
	Vec2 vec=getPosition();										//��ȡ�����
	if(vec.y<0)													//����ɻ�������Ļ�±߽�
	{
		this->removeFromParent();								//�Ƴ��ɻ�
		return false;
	}
	rotationRadians=CC_DEGREES_TO_RADIANS(-getRotation());		//ת���Ƕ�Ϊ����
	vec=vec+Vec2(step*sin(rotationRadians),-step*cos(rotationRadians));	//���ݲ��������ƶ�����
	this->setPosition(vec);										//���·ɻ�λ��
	return true;
}

SecondEnemy::SecondEnemy(){}
SecondEnemy::~SecondEnemy(){}

SecondEnemy* SecondEnemy::createRight()
{
	//����һ����������
	SecondEnemy *secondEnemy = new SecondEnemy();
	//�ж϶����Ƿ񴴽��ɹ�
	if (secondEnemy)
	{
		secondEnemy->plane = Sprite3D::create(
						std::string("c3b/enemy2.obj"),
						std::string("c3b/enemy.png"));
		secondEnemy->addChild(secondEnemy->plane);
		secondEnemy->setRotation3D(Vec3(0,-60,50));
		secondEnemy->hp=80+Constant::enemyBulletSpeed*50;
		secondEnemy->score=30;
		secondEnemy->step=3;
		secondEnemy->direction=-1;
		secondEnemy->setCameraMask((unsigned short)CameraFlag::USER1);
		secondEnemy->autorelease();
		secondEnemy->index=1;
		return secondEnemy;
	}
	CC_SAFE_DELETE(secondEnemy);
	return nullptr;
}

SecondEnemy* SecondEnemy::createLeft()
{
	//����һ����������
	SecondEnemy *secondEnemy = new SecondEnemy();
	//�ж϶����Ƿ񴴽��ɹ�
	if (secondEnemy)
	{
		secondEnemy->plane = Sprite3D::create(
				std::string("c3b/enemy2.obj"),
				std::string("c3b/enemy.png"));
		secondEnemy->addChild(secondEnemy->plane);
		secondEnemy->setRotation3D(Vec3(0,60,-50));
		secondEnemy->hp=80+Constant::enemyBulletSpeed*50;
		secondEnemy->score=30;
		secondEnemy->step=3;
		secondEnemy->direction=1;
		secondEnemy->setCameraMask((unsigned short)CameraFlag::USER1);
		secondEnemy->autorelease();
		secondEnemy->index=1;
		return secondEnemy;
	}
	CC_SAFE_DELETE(secondEnemy);
	return nullptr;
}

SecondEnemy* SecondEnemy::webCreate(float posX)
{
	//����һ����������
	SecondEnemy *secondEnemy = new SecondEnemy();
	//�ж϶����Ƿ񴴽��ɹ�
	if (secondEnemy)
	{
		secondEnemy->plane = Sprite3D::create(
						std::string("c3b/enemy2.obj"),
						std::string("c3b/enemy.png"));
		secondEnemy->addChild(secondEnemy->plane);
		secondEnemy->setCameraMask((unsigned short)CameraFlag::USER1);
		if(posX>270)
		{
			secondEnemy->direction=-1;
		}else
		{
			secondEnemy->direction=1;
		}
		secondEnemy->autorelease();
		return secondEnemy;
	}
	CC_SAFE_DELETE(secondEnemy);
	return nullptr;
}

void SecondEnemy::setRotation3D(const Vec3& rotation)
{
	if(GameData::playerNumber!=-1)
	{
		plane->setRotation3D(rotation);
		Node::setRotation3D(Vec3(0,direction*60,direction*-50));
	}else
	{
		Node::setRotation3D(rotation);
	}
}

void SecondEnemy::setPosition(const Vec2& position)
{
	Node::setPosition(position);
	if(GameData::playerNumber!=-1)
	{
		return;
	}
	if(startY==0)
	{
		startY=position.y-100;
		this->setPositionX((direction==1)?0:540);
	}
}

void SecondEnemy::die()
{
	if(GameData::playerNumber==-1)
	{
		dieVec=this->getPosition3D();
	}
	EnemyPlane::die();
}

bool SecondEnemy::shoot()
{
	if(!EnemyPlane::shoot())
	{
		return false;
	}
	//�����ӵ�
	Point point=this->getPosition();
	ControlGame *controlTemp=dynamic_cast<ControlGame*>(this->getParent());
	Point goalPoint=controlTemp->plane->plane->getPosition();
	float angle=PublicApi::turnAngle(point,goalPoint);
	Bullet *b=Bullet::create(1,point,320+Constant::custom*100,goalPoint,angle);
	//b->setRotation(angle);
	controlTemp->controlBullet->addChild(b);
	return true;
}

bool SecondEnemy::move()
{
	if(!EnemyPlane::move())
	{
		return false;
	}
	float rotationRadians;									//������ر���
	Vec2 vec=getPosition();
	if(vec.x<-20||vec.x>560)								//����ɻ�������Ļ�±߽�
	{
		removeFromParent();									//�Ƴ��ɻ�
		return false;
	}
	if(direction==1)										//�жϷ��з���
	{
		vec.x+=step;
	}else
	{
		vec.x-=step;
	}
	float dx1=vec.x-270-90*direction;
	float dx2=vec.x-270+90*direction;
	vec.y=dx1*dx1/729+startY;									//����X����Y����
	this->setPosition(vec);										//���·ɻ�λ��
	plane->setRotation3D(Vec3(-direction*dx2/10,0,-direction*dx2/10));
//	plane->setRotation3D(Vec3((360-vec.x)/10,
//			direction*(90+(vec.x-90)/10),0));				//���½Ƕ�
	shoot();
	return true;
}

ThirdEnemy::ThirdEnemy(){}
ThirdEnemy::~ThirdEnemy(){}

ThirdEnemy* ThirdEnemy::create(Node *goal)
{
	//����һ����������
	ThirdEnemy *thirdEnemy = new ThirdEnemy();
	//�ж϶����Ƿ񴴽��ɹ�
	if (thirdEnemy)
	{
		thirdEnemy->goal=goal;
		thirdEnemy->plane = Sprite3D::create(
				std::string("c3b/enemy3.obj"),
				std::string("c3b/enemy.png"));
		thirdEnemy->plane->setScale(2);
		thirdEnemy->plane->setRotation3D(Vec3(0,0,-90));
		thirdEnemy->addChild(thirdEnemy->plane);
		thirdEnemy->hp=100+Constant::enemyBulletSpeed*50;
		thirdEnemy->score=50;
		thirdEnemy->step=3;
		thirdEnemy->setCameraMask((unsigned short)CameraFlag::USER1);
		thirdEnemy->autorelease();
		thirdEnemy->index=2;
		return thirdEnemy;
	}
	CC_SAFE_DELETE(thirdEnemy);
	return nullptr;
}

ThirdEnemy* ThirdEnemy::webCreate()
{
	//����һ����������
	ThirdEnemy *thirdEnemy = new ThirdEnemy();
	//�ж϶����Ƿ񴴽��ɹ�
	if (thirdEnemy)
	{
		thirdEnemy->plane = Sprite3D::create(
				std::string("c3b/enemy3.obj"),
				std::string("c3b/enemy.png"));
		thirdEnemy->plane->setScale(2);
		thirdEnemy->plane->setRotation3D(Vec3(0,0,-90));
		thirdEnemy->addChild(thirdEnemy->plane);
		thirdEnemy->setCameraMask((unsigned short)CameraFlag::USER1);
		thirdEnemy->autorelease();
		return thirdEnemy;
	}
	CC_SAFE_DELETE(thirdEnemy);
	return nullptr;
}

void ThirdEnemy::die()
{
	if(GameData::playerNumber!=-1)
	{
		Web3DLayer *w3Layer=dynamic_cast<Web3DLayer*>(this->getParent());
		w3Layer->exManager->createBig(this->getPosition3D(),0,0);
	}else
	{
		First3DLayer *f3Layer=dynamic_cast<First3DLayer*>(this->getParent()->getParent());
		f3Layer->exManager->createBig(this->getPosition3D(),2,0);
	}
	auto rotateTemp=RotateBy::create(2,Vec3(1500,0,0));
	auto scaleTemp=ScaleTo::create(2,0.5f);
	auto animateTemp=Spawn::create(rotateTemp,scaleTemp,nullptr);
	this->runAction(Sequence::create(animateTemp,
			CallFunc::create(CC_CALLBACK_0(ThirdEnemy::callDie,this)),
			nullptr));												//ִ��˳�򶯻�
}

void ThirdEnemy::callDie()
{
	EnemyPlane::die();
}

bool ThirdEnemy::shoot()
{
	if(!EnemyPlane::shoot())
	{
		return false;
	}
	//�����ӵ�
	Point point=this->getPosition();
	ControlGame *controlTemp=dynamic_cast<ControlGame*>(this->getParent());
	Point goalPoint=controlTemp->plane->plane->getPosition();
	float angle=PublicApi::turnAngle(point,goalPoint);
	Bullet *b=Bullet::create(1,point,320+Constant::custom*100,goalPoint,angle);
//	b->setRotation(angle);
	controlTemp->controlBullet->addChild(b);
	return true;
}

void ThirdEnemy::setPosition(const Vec2& position)
{
	Node::setPosition(position);
	if(GameData::playerNumber!=-1)
	{
		return;
	}
	dieVec=Vec3(position.x,position.y,0);
	if(startY==0)
	{
		startX=(rand()%2)*540;
		startY=position.y+rand()%300-150;
		Node::setPosition(Point(startX,startY));
		this->setRotation(rand()%360);
		auto moveTemp=MoveTo::create(1,position);
		auto scaleTemp=ScaleTo::create(1,0.5);
		this->runAction(moveTemp);
		plane->runAction(scaleTemp);
		return;
	}
}

bool ThirdEnemy::move()
{
	if(!EnemyPlane::move())
	{
		return false;
	}
	float goalDegree,rotationDegree,dx,dy,dDegree;			//������ر���
	Vec2 vec=getPosition();									//��ȡ�����
	if(timeCounter<60)
	{
		plane->setRotation3D(Vec3(timeCounter*6,0,-90));	//���ÿ�ʼ�ɻ���ת
		return false;
	}
	Vec2 goalVec=goal->getPosition();						//��ȡĿ���
	dy=goalVec.y-vec.y;
	dx=goalVec.x-vec.x;
	goalDegree=CC_RADIANS_TO_DEGREES(atan2(dy,dx));			//����Ƕ�
	rotationDegree=360-getRotation();						//�õ��Ƕ�
	if(dy<0)												//ͳһ�Ƕȷ���Ƚ�
	{
		goalDegree+=360;
	}
	dDegree=rotationDegree-goalDegree;
	if(abs(dDegree)<2)										//���������ȡ���ֹ����
	{
		shoot();
	}
	else if(dDegree>180 || (dDegree<0 && dDegree>-180))		//�жϷɻ�Ӧ�����ĸ�������ת
	{
		rotationDegree=rotationDegree+1;
		if(rotationDegree>360)
		{
			rotationDegree-=360;
		}
	}
	else
	{
		rotationDegree=rotationDegree-1;
		if(rotationDegree<0)
		{
			rotationDegree+=360;
		}
	}
	this->setRotation(360-rotationDegree);
	Vec2 position=this->getPosition();
	return true;
}

FourthEnemy::FourthEnemy(){}
FourthEnemy::~FourthEnemy(){}
FourthEnemy* FourthEnemy::create()
{
	//����һ���л�����
	FourthEnemy *fourthEnemy = new FourthEnemy();
	//�ж϶����Ƿ񴴽��ɹ�
	if (fourthEnemy)
	{
		fourthEnemy->plane = Sprite3D::create(std::string("c3b/enemy4.c3b"));
		fourthEnemy->plane->setRotation3D(Vec3(0,0,0));			//�ڷŷɻ���̬
		fourthEnemy->addChild(fourthEnemy->plane);				//���ɻ�����л��ڵ�
		fourthEnemy->hp=150+Constant::enemyBulletSpeed*50;		//���õл�����ֵ����
		fourthEnemy->score=20;									//���õл���������
		fourthEnemy->step=5;									//���õл��н��ٶ�
		fourthEnemy->setCameraMask((unsigned short)CameraFlag::USER1);//���������
		fourthEnemy->plane->setLightMask(0);					//���ù���
		PublicApi::init3DTexShader(fourthEnemy->plane,
				(std::string("c3b/enemy.png")).c_str(),
				(std::string("c3b/caustics.png")).c_str(),
				(SHADER_PATH+std::string("uv_bone.vsh")).c_str(),
				(SHADER_PATH+std::string("uv_bone.fsh")).c_str());//�����ɫ��
//		fourthEnemy->plane->setLightMask((unsigned int)LightFlag::LIGHT2);
		fourthEnemy->autorelease();
		fourthEnemy->index=3;
		return fourthEnemy;										//���صл�����
	}
	CC_SAFE_DELETE(fourthEnemy);								//����л�δ�ɹ���������ȫɾ��
	return nullptr;												//���ؿ�ָ��
}

FourthEnemy* FourthEnemy::webCreate()
{
	//����һ���л�����
	FourthEnemy *fourthEnemy = new FourthEnemy();
	//�ж϶����Ƿ񴴽��ɹ�
	if (fourthEnemy)
	{
		fourthEnemy->plane = Sprite3D::create(std::string("c3b/enemy4.c3b"));
		fourthEnemy->plane->setRotation3D(Vec3(0,0,0));			//�ڷŷɻ���̬
		fourthEnemy->addChild(fourthEnemy->plane);				//���ɻ�����л��ڵ�
		fourthEnemy->setCameraMask((unsigned short)CameraFlag::USER1);//���������
		fourthEnemy->plane->setLightMask(0);					//���ù���
		PublicApi::init3DTexShader(fourthEnemy->plane,
				(std::string("c3b/enemy.png")).c_str(),
				(std::string("c3b/caustics.png")).c_str(),
				(SHADER_PATH+std::string("uv_bone.vsh")).c_str(),
				(SHADER_PATH+std::string("uv_bone.fsh")).c_str());//�����ɫ��
		fourthEnemy->autorelease();
		return fourthEnemy;										//���صл�����
	}
	CC_SAFE_DELETE(fourthEnemy);								//����л�δ�ɹ���������ȫɾ��
	return nullptr;												//���ؿ�ָ��
}

void FourthEnemy::die()
{
	if(GameData::playerNumber==-1)
	{
		dieVec=this->getPosition3D();
	}
	EnemyPlane::die();											//���ø�������������
}

void FourthEnemy::startAnim()
{
	//���������Ź�������
	Animation3D *animation3D = Animation3D::create(std::string("c3b/enemy4.c3b"));
	if (animation3D)
	{
		//Ӣ�۸��ֶ���
		Animate3D *animate3D = Animate3D::create(animation3D,0.0f,1.0f);
		//ִ�ж���
		this->plane->runAction(RepeatForever::create(animate3D));
	}
}

bool FourthEnemy::move()
{
	if(!EnemyPlane::move())
	{
		return false;
	}
	animLight.x=(timeCounter%100)/100.0;
	animLight.y=(timeCounter%100)/100.0;
	auto glprogramstate = plane->getGLProgramState();
	glprogramstate->setUniformVec2("v_animLight",animLight);	//������ɫ��������λ��
	if(timeCounter==2)
	{
		startAnim();
	}
	float rotationRadians;										//������ر���
	Vec2 vec=getPosition();										//��ȡ�����
	if(vec.y<0)													//����ɻ�������Ļ�±߽�
	{
		this->removeFromParent();								//�Ƴ��ɻ�
		return false;
	}
	rotationRadians=CC_DEGREES_TO_RADIANS(-getRotation());		//ת���Ƕ�Ϊ����
	vec=vec+Vec2(step*sin(rotationRadians),-step*cos(rotationRadians));	//���ݲ��������ƶ�����
	this->setPosition(vec);										//���·ɻ�λ��
	return true;
}
Boss::Boss(){}
Boss::~Boss(){}

Boss* Boss::create()
{
	//����һ����������
	Boss *boss = new Boss();
	//�ж϶����Ƿ񴴽��ɹ�
	if (boss)
	{
		boss->plane = Sprite3D::create(
						StringUtils::format("c3b/boss%d.c3b",Constant::custom+1).c_str());
		if(Constant::custom==0){
			boss->plane->setRotation3D(Vec3(90,0,0));
		}else if(Constant::custom==1){
			boss->plane->setRotation3D(Vec3(0,170,0));
		}else{
			boss->plane->setRotation3D(Vec3(30,0,0));
		}

		boss->addChild(boss->plane);
		boss->hp=5000;
		boss->score=1000;
		boss->setCameraMask((unsigned short)CameraFlag::USER1);
		boss->plane->setLightMask(0);					//���ù���
		std::string texture="";
		if(Constant::custom==0){
			texture=std::string("c3b/enemy.png");
		}else{
			texture=StringUtils::format("c3b/boss%d.jpg",Constant::custom+1).c_str();
		}
		PublicApi::init3DTexShader(boss->plane,
				texture.c_str(),
				(std::string("c3b/caustics.png")).c_str(),
				(SHADER_PATH+std::string("uv_bone.vsh")).c_str(),
				(SHADER_PATH+std::string("uv_bone.fsh")).c_str());//�����ɫ��
		boss->step=2;
		boss->index=Constant::custom+4;				//boss�ı��4,5,6
		return boss;
	}
	CC_SAFE_DELETE(boss);
	return nullptr;
}

Boss* Boss::webCreate()
{
	//����һ����������
	Boss *boss = new Boss();
	//�ж϶����Ƿ񴴽��ɹ�
	if (boss)
	{
		boss->plane = Sprite3D::create(
						StringUtils::format("c3b/boss%d.c3b",Constant::custom+1).c_str());
		if(GameData::level==0){
			boss->plane->setRotation3D(Vec3(90,0,0));
		}else if(GameData::level==1){
			boss->plane->setRotation3D(Vec3(0,170,0));
		}else{
			boss->plane->setRotation3D(Vec3(30,0,0));
		}
		boss->addChild(boss->plane);
		boss->setCameraMask((unsigned short)CameraFlag::USER1);
		boss->plane->setLightMask(0);					//���ù���
		std::string texture="";
		if(GameData::level==0){
			texture=std::string("c3b/enemy.png");
		}else{
			texture=StringUtils::format("c3b/boss%d.jpg",Constant::custom+1).c_str();
		}
		PublicApi::init3DTexShader(boss->plane,
				texture.c_str(),
				(std::string("c3b/caustics.png")).c_str(),
				(SHADER_PATH+std::string("uv_bone.vsh")).c_str(),
				(SHADER_PATH+std::string("uv_bone.fsh")).c_str());//�����ɫ��
		return boss;
	}
	CC_SAFE_DELETE(boss);
	return nullptr;
}

void Boss::startAnim()
{
	if(GameData::playerNumber!=-1)
	{
		//���������Ź�������
		Animation3D *animation3D = Animation3D::create(StringUtils::format("c3b/boss%d.c3b",GameData::level+1).c_str());
		if (animation3D)
		{
			if(GameData::level==0){
				//boss���ֶ���
				Animate3D *animate3D = Animate3D::create(animation3D,0.0f,1.0f);
				this->plane->runAction(RepeatForever::create(animate3D));
			}else if(GameData::level==1){
				//boss���ֶ���
				Animate3D *animate3D = Animate3D::create(animation3D,0.0f,5.0f);
				this->plane->runAction(RepeatForever::create(animate3D));
			}else{
				//boss���ֶ���
				Animate3D *animate3D = Animate3D::create(animation3D,0.0f,5.0f);
				this->plane->runAction(RepeatForever::create(animate3D));
			}
		}
		return;
	}
	//���������Ź�������
	Animation3D *animation3D = Animation3D::create(StringUtils::format("c3b/boss%d.c3b",Constant::custom+1).c_str());
	if (animation3D)
	{
		if(Constant::custom==0){
			//boss���ֶ���
			Animate3D *animate3D = Animate3D::create(animation3D,0.0f,1.0f);
			this->plane->runAction(RepeatForever::create(animate3D));
		}else if(Constant::custom==1){
			//boss���ֶ���
			Animate3D *animate3D = Animate3D::create(animation3D,0.0f,5.0f);
			this->plane->runAction(RepeatForever::create(animate3D));
		}else{
			//boss���ֶ���
			Animate3D *animate3D = Animate3D::create(animation3D,0.0f,5.0f);
			this->plane->runAction(RepeatForever::create(animate3D));
		}
	}
}

bool Boss::move()
{
	if(!EnemyPlane::move())
	{
		return false;
	}
	if(Constant::custom==0){							//�Ե�һ��boss�����ƶ�
		float pX = this->getPositionX();				//��ȡ��ǰbossX����
		if(pX<100||pX>440)								//����ֵ��ת��
		{
			step=-step;
		}
		pX+=step/2.0f;									//����bossX����
		this->setPositionX(pX);							//����bossλ��
	}
	this->shoot();										//�����ӵ�
	//������ɫ��
	animLight.x=(timeCounter%100)/100.0;
	animLight.y=(timeCounter%100)/100.0;
	auto glprogramstate = plane->getGLProgramState();
	glprogramstate->setUniformVec2("v_animLight",animLight);	//������ɫ��������λ��

	if(timeCounter==2)
	{
		startAnim();
	}
	return true;
}

bool Boss::shoot()
{
	if(!EnemyPlane::shoot())
	{
		return false;
	}
	bossShootCounter++;
//	switch(this->index)
//	{
//	default:
//	case 4:
//		twoLineShoots(0);
//		break;
//	case 5:
//		break;
//	case 6:
//		break;
//	}
	if(bossShootCounter%3==0){
		if(bossShootCounter>=0&&bossShootCounter<12){
			twoLineShoots(0);
		}else if(bossShootCounter>=12&&bossShootCounter<24){
			lineSectorShoots(0);
		}else if(bossShootCounter>=24&&bossShootCounter<48){
			sectorShoots();
		}else if(bossShootCounter>=48){
			twoSectorShoots(0);
		}
	}


	return true;
}

void Boss::die()
{
	EnemyPlane::die();
}

void Boss::twoLineGoalShoots(int i)
{
	if(i>6)
	{
		return;
	}
	//�����ӵ�
	Point point=this->getPosition();
	ControlGame *controlTemp=dynamic_cast<ControlGame*>(this->getParent());
	Point goalPoint=controlTemp->plane->plane->getPosition();
	float angle1=PublicApi::turnAngle(point-Point(50,30),goalPoint);			//�����ӵ��Ƕ�
	float angle2=PublicApi::turnAngle(point-Point(-50,30),goalPoint);
	Bullet *b1=Bullet::create(3,point-Point(50,30),500+Constant::custom*100,goalPoint,angle1);	//�����ӵ�
	Bullet *b2=Bullet::create(3,point-Point(-50,30),500+Constant::custom*100,goalPoint,angle2);
	b1->setRotation(angle1);													//�����ӵ��Ƕ�
	b2->setRotation(angle2);
	controlTemp->controlBullet->addChild(b1);									//����ӵ�
	controlTemp->controlBullet->addChild(b2);
	//�ӳ������ص�������
	this->runAction(Sequence::create(DelayTime::create(0.2),
	CallFunc::create(CC_CALLBACK_0(Boss::twoLineGoalShoots,this,i+1)),nullptr));
}

void Boss::twoSwingShoots(int i)
{
	if(i>24)
	{
		return;
	}
	//�����ӵ�
	Point point=this->getPosition();
	Point point1=point-Point(100,100);
	Point point2=point-Point(-100,100);
	ControlGame *controlTemp=dynamic_cast<ControlGame*>(this->getParent());
	Point goalPoint1=Point(270+(6-abs(i-12))*270,-200);
	Point goalPoint2=Point(270-(6-abs(i-12))*270,-200);
	float angle1=PublicApi::turnAngle(point1,goalPoint1);
	float angle2=PublicApi::turnAngle(point2,goalPoint2);
	Bullet *b1=Bullet::create(4,point1,500+Constant::custom*100,goalPoint1,angle1);
	Bullet *b2=Bullet::create(4,point1,500+Constant::custom*100,goalPoint2,angle2);
	b1->setRotation(angle1);
	b2->setRotation(angle2);
	controlTemp->controlBullet->addChild(b1);
	controlTemp->controlBullet->addChild(b2);
	//�ӳ����һ�ص�������
	this->runAction(Sequence::create(DelayTime::create(0.05),
	CallFunc::create(CC_CALLBACK_0(Boss::twoSwingShoots,this,i+1)),nullptr));
}

void Boss::twoLineShoots(int i)
{
	if(i>10)
	{
		return;
	}
	//�����ӵ�
	Point point=this->getPosition();
	ControlGame *controlTemp=dynamic_cast<ControlGame*>(this->getParent());
	Point goalPoint=controlTemp->plane->plane->getPosition();
	Bullet *b1=Bullet::create(3,point-Point(100,50),500+Constant::custom*100,point-Point(100,2000),180);	//�����ӵ�
	Bullet *b2=Bullet::create(3,point-Point(-100,50),500+Constant::custom*100,point-Point(-100,2000),180);
	controlTemp->controlBullet->addChild(b1);									//����ӵ�
	controlTemp->controlBullet->addChild(b2);
	//�ӳ����һ���ص�������
	this->runAction(Sequence::create(DelayTime::create(0.1),
	CallFunc::create(CC_CALLBACK_0(Boss::twoLineShoots,this,i+1)),nullptr));
}

void Boss::lineSectorShoots(int i)
{
	if(i>10)
	{
		return;
	}
	//�����ӵ�
	Point point=this->getPosition();
	ControlGame *controlTemp=dynamic_cast<ControlGame*>(this->getParent());
	Point goalPoint=Point(270+(i-5)*170,-200);
	float angle1=PublicApi::turnAngle(point-Point(40,40),goalPoint);
	Bullet *b1=Bullet::create(4,point-Point(0,40),500+Constant::custom*100,goalPoint,angle1);
	b1->setRotation(angle1);
	controlTemp->controlBullet->addChild(b1);
	//�ӳ������ص�������
	this->runAction(Sequence::create(DelayTime::create(0.1),
	CallFunc::create(CC_CALLBACK_0(Boss::lineSectorShoots,this,i+1)),nullptr));
}

void Boss::sectorShoots()
{
	//�����ӵ�
	Point point=this->getPosition();
	ControlGame *controlTemp=dynamic_cast<ControlGame*>(this->getParent());
	for(int i=0;i<11;i++)
	{
		Point goalPoint=Point(270+(i-5)*170,-200);
		float angle1=PublicApi::turnAngle(point-Point(30,30),goalPoint);
		Bullet *b1=Bullet::create(4,point-Point(0,30),500+Constant::custom*100,goalPoint,angle1);
		b1->setRotation(angle1);
		controlTemp->controlBullet->addChild(b1);
	}
}

//����������
void Boss::IrregularSectorShoots()
{
	//�����ӵ�
//	Point point=this->getPosition();
	ControlGame *controlTemp=dynamic_cast<ControlGame*>(this->getParent());
	for(int i=0;i<11;i++)
	{
		Point goalPoint=Point(270+(i-6)*180,-200);
		float angle1=PublicApi::turnAngle(Point(100,600)+i*Point(30,30),goalPoint);
		Bullet *b1=Bullet::create(4,Point(100,600)+i*Point(30,30),100+Constant::custom*100+i*40,goalPoint,angle1);
		b1->setRotation(angle1);
		controlTemp->controlBullet->addChild(b1);
	}
}
void Boss::twoSectorShoots(int i)
{
	if(i>3)
	{
		return;
	}
	//�����ӵ�
	Point point=this->getPosition();
	ControlGame *controlTemp=dynamic_cast<ControlGame*>(this->getParent());
	for(int j=0;j<11;j++)
	{
		Point goalPoint=Point(270+(j-5)*170,-200);
		float angle1=PublicApi::turnAngle(point-Point(30,30),goalPoint);
		Bullet *b1=Bullet::create(4,point-Point(0,30),500+i*10+Constant::custom*100,goalPoint,angle1);
		b1->setRotation(angle1);
		controlTemp->controlBullet->addChild(b1);
	}
	//�ӳ������ص�������
	this->runAction(Sequence::create(DelayTime::create(0.1),
	CallFunc::create(CC_CALLBACK_0(Boss::twoSectorShoots,this,i+1)),nullptr));
}
