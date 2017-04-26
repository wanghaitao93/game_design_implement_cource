#ifndef _PublicApi_H_
#define _PublicApi_H_
#include "cocos2d.h"
using namespace cocos2d;
class PublicApi
{
public:
	PublicApi();
	static float hpPercent(float hp);									//ʣ������ֵ�ٷֱ�
	static float turnAngle(Point start,Point goal);						//�ӵ���ת�Ƕ�
//	static Point start;
//	static Point goal;
	bool meetGoals(Point point1,Point point2,int num1,int num2);		//��ײ���
	int setWhichMethods();												//�жϲ���ģʽ
    static void init3DShader(Sprite3D *sprite,const char *texture,const char *vsh,const char *fsh);
    static void init3DOutLineShader(Sprite3D *sprite);
    static void init3DTexShader(Sprite3D *sprite,const char *texture1,
    		const char *texture2,const char *vsh,const char *fsh);
};
#endif
