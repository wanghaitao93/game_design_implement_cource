#ifndef __HelpLayer_H__
#define __HelpLayer_H__

#include "cocos2d.h"
#include "GameSceneManager.h"

using namespace cocos2d;

class HelpLayer : public cocos2d::Layer
{
public:
	GameSceneManager* gsm;

	virtual bool init();
	//touch
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
   //Cocos2d-x中定义的一个宏，创建指定类型的对象
    //并做好相应的初始化与释放工作
    CREATE_FUNC(HelpLayer);

private:
    static const int helpCount=6;
    Sprite3D *help[helpCount];
    Sprite3D *rotateHelp;
    Sprite *point;
    int offsetX=0;
    int helpNum=0;
    int helpPre=0;
    int helpNext=0;
    int offsetAngle=0;
    int visitNum=0;
    bool touchFlag=true;
    bool isClicked=false;
    bool isLook=false;
//	void update(float dt);
    void goNextHelp(int index);
    void canChangeContent();
	void initMenu();
	void judgeCallback(float f);
//	void initPic();
	void menuExit();

};

#endif
