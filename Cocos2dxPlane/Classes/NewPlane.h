#ifndef __NewPlane_H__
#define __NewPlane_H__

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class NewPlane :public cocos2d::Sprite3D
{
public:
	Sprite3D *plane;			//飞机指针
	Point animPoint;			//目标位置
	Point deltaPoint;			//move的偏移量
	Camera *camera;				//摄像机

	MotionStreak *myStreak; 	//拖尾
	int index;					//第几个场景
	Layer *layer;				//场景指针

	float rotateSpeed=0.5;		//飞机的旋转速度
	int maxRotateX=15;			//X轴最大的旋转角度
	int maxRotateY=50;			//Y轴最大的旋转角度
	int directionLeft=0;		//左右偏向 1--左 ,-1--右
	int directionFront=0;		//前后偏向 1--后 ,-1--前
	bool action=false;			//飞机可以继续进行监听
	float angleLeft=0;			//飞机左右旋转的角度
	float angleFront=0;			//飞机前后旋转的角度
	float angleX=90;			//飞机X轴偏转角度
	float angleY=0;				//飞机Y轴偏转角度
	bool tail=true;				//是否能创建拖尾效果

	static vector<Sprite3D*> planeVec;	//飞机容器

    NewPlane(Layer *layer);//构造函数
	void canMove();				//
	void oneMovePlane();		//began的移动
	void oneRotatePlane();		//began的旋转
	void planeReturn();			//飞机不停的恢复机身角度
	void updateRotate();		//飞机不停的恢复机身的角度
	void updatePlane(float f);	//旋转飞机角度
	void updateBackflip();//飞机后空翻
	void movePlane();			//move的移动
	void rotatePlane();			//move的旋转

	void hurt(int demage);
	void die();

	//重力传感回调方法
	void movePlane2(Point delta);
//	void changeModel();
//	//飞机的触摸监听
//	bool onMyTouchBegan(Touch* touch, Event* event);
//	void onMyTouchMoved(Touch* touch, Event* event);
//	void onMyTouchEnded(Touch* touch, Event* event);
};

#endif
