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

	//��ʼ���ķ���
	virtual bool init();

	//touch
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);

    void update(float delta);
    CREATE_FUNC(BeginLayer);
private:
    float perRate;	//��ת�Ƕ�
    Sprite3D* plane;
	Sprite* back1;
	Sprite* back2;
	Sprite* messageSprite;
	float perXA=1.0;//ÿ����ת��
	float perYA=10.0;
	float perZA=7.0;
	float perXW=3.1;//ÿ��ƫ����
	float perYW=7.0;
	float perZW=2.0;
	//����uv����ֵ
	Vec2	m_LightAni;
};

#endif
