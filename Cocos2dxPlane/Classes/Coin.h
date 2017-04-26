#ifndef _COIN_H_
#define _COIN_H_

#include "cocos2d.h"
using namespace cocos2d;

class Coin:public BillBoard
{
private :
	int score=0;
	int step=80;
//	int awardNum=0;				//金币状态  1--子弹多，2-子弹威力，3-导弹等级，4-大招个数,5-防御个数,6--gray,7--blue,8--gold
public :
	void createVec();
	void createCoin(Vec3 vec,int index,int score);	//创建金币
	void moveCoin();
	void deleteCoin(Sprite3D *coin);				//删除金币
	void update();
	void addAward(int awardNum);					//添加奖励
	void addLabel(Point point,int awardNum);		//添加文字提示
	void deleteLabel(Sprite *sprite);				//删除提示文字
	std::vector<Sprite3D*>* coinSpriteVec;		//金币的容器
	std::vector<Sprite3D*>* delCoinSpriteVec;		//金币的容器

	CREATE_FUNC(Coin);
};

class Award : public Sprite3D
{
public:
	static Award* webCreate(Point point,int num);
	int liveFlag=0;
};

#endif
