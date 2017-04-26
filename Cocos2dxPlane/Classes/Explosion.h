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
	void createNewCoin(Vec3 vec,int index,int score);					//创建金币
	Animate *animateBig;				//大爆炸
	Animate *animateSmall;				//小爆炸
	Animate *animatePlane;				//飞机的爆炸
};

#endif
