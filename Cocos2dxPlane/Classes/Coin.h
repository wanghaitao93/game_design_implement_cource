#ifndef _COIN_H_
#define _COIN_H_

#include "cocos2d.h"
using namespace cocos2d;

class Coin:public BillBoard
{
private :
	int score=0;
	int step=80;
//	int awardNum=0;				//���״̬  1--�ӵ��࣬2-�ӵ�������3-�����ȼ���4-���и���,5-��������,6--gray,7--blue,8--gold
public :
	void createVec();
	void createCoin(Vec3 vec,int index,int score);	//�������
	void moveCoin();
	void deleteCoin(Sprite3D *coin);				//ɾ�����
	void update();
	void addAward(int awardNum);					//��ӽ���
	void addLabel(Point point,int awardNum);		//���������ʾ
	void deleteLabel(Sprite *sprite);				//ɾ����ʾ����
	std::vector<Sprite3D*>* coinSpriteVec;		//��ҵ�����
	std::vector<Sprite3D*>* delCoinSpriteVec;		//��ҵ�����

	CREATE_FUNC(Coin);
};

class Award : public Sprite3D
{
public:
	static Award* webCreate(Point point,int num);
	int liveFlag=0;
};

#endif
