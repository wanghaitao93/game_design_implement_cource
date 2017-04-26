#ifndef __NewPlane_H__
#define __NewPlane_H__

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class NewPlane :public cocos2d::Sprite3D
{
public:
	Sprite3D *plane;			//�ɻ�ָ��
	Point animPoint;			//Ŀ��λ��
	Point deltaPoint;			//move��ƫ����
	Camera *camera;				//�����

	MotionStreak *myStreak; 	//��β
	int index;					//�ڼ�������
	Layer *layer;				//����ָ��

	float rotateSpeed=0.5;		//�ɻ�����ת�ٶ�
	int maxRotateX=15;			//X��������ת�Ƕ�
	int maxRotateY=50;			//Y��������ת�Ƕ�
	int directionLeft=0;		//����ƫ�� 1--�� ,-1--��
	int directionFront=0;		//ǰ��ƫ�� 1--�� ,-1--ǰ
	bool action=false;			//�ɻ����Լ������м���
	float angleLeft=0;			//�ɻ�������ת�ĽǶ�
	float angleFront=0;			//�ɻ�ǰ����ת�ĽǶ�
	float angleX=90;			//�ɻ�X��ƫת�Ƕ�
	float angleY=0;				//�ɻ�Y��ƫת�Ƕ�
	bool tail=true;				//�Ƿ��ܴ�����βЧ��

	static vector<Sprite3D*> planeVec;	//�ɻ�����

    NewPlane(Layer *layer);//���캯��
	void canMove();				//
	void oneMovePlane();		//began���ƶ�
	void oneRotatePlane();		//began����ת
	void planeReturn();			//�ɻ���ͣ�Ļָ�����Ƕ�
	void updateRotate();		//�ɻ���ͣ�Ļָ�����ĽǶ�
	void updatePlane(float f);	//��ת�ɻ��Ƕ�
	void updateBackflip();//�ɻ���շ�
	void movePlane();			//move���ƶ�
	void rotatePlane();			//move����ת

	void hurt(int demage);
	void die();

	//�������лص�����
	void movePlane2(Point delta);
//	void changeModel();
//	//�ɻ��Ĵ�������
//	bool onMyTouchBegan(Touch* touch, Event* event);
//	void onMyTouchMoved(Touch* touch, Event* event);
//	void onMyTouchEnded(Touch* touch, Event* event);
};

#endif
