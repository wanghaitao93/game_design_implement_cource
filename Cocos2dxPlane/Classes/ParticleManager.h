#ifndef _ParticleManager_H_
#define _ParticleManager_H_

#include "cocos2d.h"
using namespace cocos2d;
class ParticleManager
{
public:
	ParticleManager(Layer* layer);
	Layer* layer;

	void initRain();					//初始化下雨
	void initFire(Sprite3D *sprite);	//初始化火焰

};
#endif
