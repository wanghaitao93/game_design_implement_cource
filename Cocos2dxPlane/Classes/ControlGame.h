#ifndef __ControlGame_H__
#define __ControlGame_H__

#include "cocos2d.h"
#include "NewPlane.h"
#include "Bullet.h"
USING_NS_CC;

class ControlGame: public Node
{
public:
	void createFirst1();
	void createFirst2();
	void createFirst3();
	void createSecond();
	void createThird();
	void createFourth();
	void update();
	void completeGame();
	void createBossNow();
	static ControlGame* create(NewPlane *_plane);
	NewPlane *plane;
	ControlBullet *controlBullet;
	CREATE_FUNC(ControlGame);
private:
	bool createBoss=false;
	bool isWin=false;
	int timeCounter=0;
	int childCount=0;
};

#endif
