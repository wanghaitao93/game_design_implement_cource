#ifndef __SelectPlane_H__
#define __SelectPlane_H__

#include "cocos2d.h"
#include "GameSceneManager.h"

using namespace cocos2d;
using namespace std;

class SelectPlane : public cocos2d::Layer
{
public:
	GameSceneManager* gsm;			//������
	virtual bool init();			//��ʼ������

    CREATE_FUNC(SelectPlane);
private:
	int angle=360;						//�ɻ�ѡ��Ƕ�
	int step=0;							//�ɻ��ƶ�����
	int waitIndex=0;
	Texture2D *starsTexture;			//����
	MenuItemImage* menuItem[5];			//�˵���
	Sprite3D* planeobj[3];				//������ַɻ�
	Menu *menu;
	Sprite *backButtom;					//����
	Sprite *leftMove;					//���ƶ�
	Sprite *rightMove;					//���ƶ�
	Sprite *selectPlaneSort;			//ѡ��ɻ�����
	Sprite *stars[3];				//���ǹ�����,��������ֵ,���Ǽ���
	Sprite *lock;					//��

	Sprite *housePlane;				//����ѡ��ɻ������
	Sprite *playerPlane;			//���ѡ��ɻ������

	Sprite *waittingConnect;		//�ȴ��ľ���

	void initMenu();				//��ʼ��2D�����Ĳ˵���ť
	void initPlane3D();				//��ʼ��������ҷɻ�
	void menuSelect(int index);		//�˵���ť�ص�����
	void setPoint3D(float dt);		//�ɻ��ƶ���ʱ�ص�
	void goNextScene();				//��ת��һ����
	void changePlane(int index);	//����ѡ�ɻ���Ϣ
	void changeContent(int index);
	void initPlaneStar();			//��ʼ���ɻ���Ϣ ����
	void changePlaneStar();			//���ķɻ�����Ϣ ����
	void changeSelectPlane(float f);//�ı�ѡ��ķɻ�
	void planeFly();				//�ɻ�����
	void changWaitting(float f);	//�ȴ���
	//touch
	bool onMyTouchBegan(Touch* touch, Event* event);
	void onMyTouchMoved(Touch* touch, Event* event);
	void onMyTouchEnded(Touch* touch, Event* event);

};

#endif
