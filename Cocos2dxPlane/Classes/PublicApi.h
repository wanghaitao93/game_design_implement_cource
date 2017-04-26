#ifndef _PublicApi_H_
#define _PublicApi_H_
#include "cocos2d.h"
using namespace cocos2d;
class PublicApi
{
public:
	PublicApi();
	static float hpPercent(float hp);									//剩余生命值百分比
	static float turnAngle(Point start,Point goal);						//子弹旋转角度
//	static Point start;
//	static Point goal;
	bool meetGoals(Point point1,Point point2,int num1,int num2);		//碰撞检测
	int setWhichMethods();												//判断操作模式
    static void init3DShader(Sprite3D *sprite,const char *texture,const char *vsh,const char *fsh);
    static void init3DOutLineShader(Sprite3D *sprite);
    static void init3DTexShader(Sprite3D *sprite,const char *texture1,
    		const char *texture2,const char *vsh,const char *fsh);
};
#endif
