#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__

#include "cocos2d.h"
USING_NS_CC;

class Explosion : public BillBoard
{
public:
	CREATE_FUNC(Explosion);
	static Explosion* create(const std::string& filename, const Rect& rect);
	void deleteMe();
	void startAnim(Animate *animate);
};

class ExManager : public Node
{
public:
	CREATE_FUNC(ExManager);
	bool init();
	void createBig(Vec3 vec,int index,int score);
	void createSmall(Vec3 vec);
	void createPlane(Vec3 vec);
	void createNewCoin(Vec3 vec,int index,int score);					//�������
	Animate *animateBig;				//��ը
	Animate *animateSmall;				//С��ը
	Animate *animatePlane;				//�ɻ��ı�ը
};

#endif
