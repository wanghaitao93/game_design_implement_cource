#ifndef _ParticleManager_H_
#define _ParticleManager_H_

#include "cocos2d.h"
using namespace cocos2d;
class ParticleManager
{
public:
	ParticleManager(Layer* layer);
	Layer* layer;

	void initRain();					//��ʼ������
	void initFire(Sprite3D *sprite);	//��ʼ������

};
#endif
