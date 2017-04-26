#include "ParticleManager.h"
#include "AppMacros.h"

ParticleManager::ParticleManager(Layer* layer)
{
	this->layer=layer;
}
//���곡��
void ParticleManager::initRain()
{
	ParticleSystemQuad* psq = ParticleSystemQuad::create(PLIST_PATH+std::string("raining.plist"));//���ļ��м�������ϵͳ
	psq->retain();										//��������
	psq->setPosition(Vec2(350,400));
//	psq->setPosition(Vec2(270,960));
	psq->setRotation(180);
	layer->addChild(psq, PLIST_LEVEL);
}

void ParticleManager::initFire(Sprite3D *sprite)
{
	//��������
	ParticleSystemQuad *psqMissile= ParticleFire::create();					//������������ϵͳЧ��
	psqMissile->retain();													//��������
	psqMissile->setScaleX(0.1);
	psqMissile->setScaleY(0.2);
	psqMissile->setRotation(180);
	psqMissile->setTexture( Director::getInstance()->
				getTextureCache()->addImage(PLIST_PATH+std::string("fire.png")));//Ϊ����ϵͳ����ͼƬ
	psqMissile->setPositionType(ParticleSystem::PositionType::RELATIVE);		//���Ӹ����ڷ����������Ҹ��淢������λ�ñ仯���仯��  ����RELATIVE,FREE
	psqMissile->setPosition(0,-35);
	psqMissile->setCameraMask((unsigned short)CameraFlag::USER1);
	sprite->addChild(psqMissile,10);
}
