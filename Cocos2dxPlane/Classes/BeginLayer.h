#ifndef __BeginLayer_H__
#define __BeginLayer_H__

#include "GameSceneManager.h"
#include "cocos2d.h"
#include "ui/UIVideoPlayer.h"
//#include "3d/CCAnimation3D.h"
//#include "3d/CCAnimate3D.h"
using namespace cocos2d::experimental::ui;
using namespace cocos2d;

class BeginLayer : public cocos2d::Layer
{
public:
	GameSceneManager* gsm;

	int degree=0;

	//初始化的方法
	virtual bool init();

	//touch
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);

    void update(float delta);
    CREATE_FUNC(BeginLayer);
private:
    float perRate;	//旋转角度
    Sprite3D* plane;
	Sprite* back1;
	Sprite* back2;
	Sprite* messageSprite;
	float perXA=1.0;//每次旋转度
	float perYA=10.0;
	float perZA=7.0;
	float perXW=3.1;//每次偏移量
	float perYW=7.0;
	float perZW=2.0;
	//纹理uv滚动值
	Vec2	m_LightAni;
};

#endif
