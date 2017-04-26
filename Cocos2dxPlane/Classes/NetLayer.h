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
	Sprite *waitting;			//等待中
	Sprite *noConnectBoard;			//没有连接板子
	Sprite *waittingConnect;			//等待中
	virtual bool init();
	void goOneModel();
	void goNetModel();
	void goSelectGame();
	void ConnectNet(float f);
	void selectPlane(float f);
	void breakConnect(float f);		//断开
	MenuItemImage* oneModel;
	MenuItemImage* netModel;
	MenuItemImage* netModel2;
	static bool isConnecting;
	//Cocos2d-x中定义的一个宏，创建指定类型的对象
	//并做好相应的初始化与释放工作
	CREATE_FUNC(NetLayer);

private:
};

#endif
