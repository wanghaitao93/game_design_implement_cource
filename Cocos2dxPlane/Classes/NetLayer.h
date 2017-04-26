#ifndef __NetLayer_H__
#define __NetLayer_H__

#include "cocos2d.h"
#include "GameSceneManager.h"

using namespace cocos2d;

class NetLayer : public cocos2d::Layer
{
public:
	GameSceneManager* gsm;
	int waitIndex;
	Sprite *waitting;			//�ȴ���
	Sprite *noConnectBoard;			//û�����Ӱ���
	Sprite *waittingConnect;			//�ȴ���
	virtual bool init();
	void goOneModel();
	void goNetModel();
	void goSelectGame();
	void ConnectNet(float f);
	void selectPlane(float f);
	void breakConnect(float f);		//�Ͽ�
	MenuItemImage* oneModel;
	MenuItemImage* netModel;
	MenuItemImage* netModel2;
	static bool isConnecting;
	//Cocos2d-x�ж����һ���꣬����ָ�����͵Ķ���
	//��������Ӧ�ĳ�ʼ�����ͷŹ���
	CREATE_FUNC(NetLayer);

private:
};

#endif
