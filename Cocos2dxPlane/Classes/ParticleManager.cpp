#include "ParticleManager.h"
#include "AppMacros.h"

ParticleManager::ParticleManager(Layer* layer)
{
	this->layer=layer;
}
//下雨场景
void ParticleManager::initRain()
{
	ParticleSystemQuad* psq = ParticleSystemQuad::create(PLIST_PATH+std::string("raining.plist"));//从文件中加载粒子系统
	psq->retain();										//保持引用
	psq->setPosition(Vec2(350,400));
//	psq->setPosition(Vec2(270,960));
	psq->setRotation(180);
	layer->addChild(psq, PLIST_LEVEL);
}

void ParticleManager::initFire(Sprite3D *sprite)
{
	//创建火焰
	ParticleSystemQuad *psqMissile= ParticleFire::create();					//创建火焰粒子系统效果
	psqMissile->retain();													//保持引用
	psqMissile->setScaleX(0.1);
	psqMissile->setScaleY(0.2);
	psqMissile->setRotation(180);
	psqMissile->setTexture( Director::getInstance()->
				getTextureCache()->addImage(PLIST_PATH+std::string("fire.png")));//为粒子系统设置图片
	psqMissile->setPositionType(ParticleSystem::PositionType::RELATIVE);		//粒子附属于发射器，并且跟随发射器的位置变化而变化。  还有RELATIVE,FREE
	psqMissile->setPosition(0,-35);
	psqMissile->setCameraMask((unsigned short)CameraFlag::USER1);
	sprite->addChild(psqMissile,10);
}
