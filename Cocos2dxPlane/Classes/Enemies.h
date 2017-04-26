#ifndef __ENEMIES_H__
#define __ENEMIES_H__

#include "cocos2d.h"
#include "EnemyPlane.h"
using namespace std;
USING_NS_CC;

class FirstEnemy: public EnemyPlane
{
public:
	FirstEnemy();
	~FirstEnemy();
	void die();
	bool move();
	static vector<Sprite3D*> firstEnemyVec;
	static FirstEnemy* create();
	static FirstEnemy* webCreate();
};

class SecondEnemy: public EnemyPlane
{
public:
	SecondEnemy();
	~SecondEnemy();
	void die();
	bool shoot();
	bool move();
	void setPosition(const Vec2& position);
	void setRotation3D(const Vec3& rotation);
	static SecondEnemy* createLeft();
	static SecondEnemy* createRight();
	static SecondEnemy* webCreate(float posX);
	static vector<Sprite3D*> secondEnemyVec;
protected:
	int direction=1;
	float startY=0;
};

class ThirdEnemy: public EnemyPlane
{
public:
	ThirdEnemy();
	~ThirdEnemy();
	void die();
	bool shoot();
	bool move();
	void setPosition(const Vec2& position);
	void callDie();
	static vector<Sprite3D*> thirdEnemyVec;
	static ThirdEnemy* create(Node *goal);
	static ThirdEnemy* webCreate();

protected:
	int startX=0;
	int startY=0;
	Node *goal;
};

class FourthEnemy: public EnemyPlane
{
public:
	FourthEnemy();
	~FourthEnemy();
	void die();
	bool move();
	void startAnim();
	static FourthEnemy* create();
	static FourthEnemy* webCreate();
private:
	Point animLight;
};

class Boss: public EnemyPlane
{
public:
	Boss();
	~Boss();
//	void die();
	bool shoot();
	void startAnim();
	void die();
	void twoLineGoalShoots(int i);
	void twoLineShoots(int i);
	void twoSwingShoots(int i);
	void lineSectorShoots(int i);
	void twoSectorShoots(int i);
	void sectorShoots();
	void IrregularSectorShoots();
	bool move();
	static Boss* create();
	static Boss* webCreate();
private:
	int bossShootCounter=0;
	Point animLight;
};

#endif
